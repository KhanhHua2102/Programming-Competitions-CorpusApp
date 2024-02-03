import os
import pathlib
import re
import shutil
import subprocess
import sys
import tempfile
import logging
import inflect
from pylatex import Command, Tabular
from pylatex.utils import NoEscape

from p2d._version import __version__
RESOURCES_PATH = os.path.join(
    os.path.split(os.path.realpath(__file__))[0], 'resources')


# Execute pdflatex on tex_file.
# tex_file is a .tex file
def tex2pdf(tex_file):
    logging.debug('Executing pdflatex on \'%s\'.' % tex_file)
    if not tex_file.endswith('.tex'):
        logging.error('The argument tex_file=\'%s\' passed to tex2pdf is not a .tex file.' % tex_file)
        exit(1)
    
    tex_dir = os.path.dirname(tex_file)
    tex_name = os.path.basename(tex_file)[:-4] # Without extension
    command_as_list = ['pdflatex', '-interaction=nonstopmode', '--shell-escape',
                       '-output-dir=' + tex_dir, '-jobname=%s' % tex_name,
                       tex_file]
    logging.debug('pdflatex command = ' + ' '.join(command_as_list))
    pdflatex = subprocess.run(command_as_list, stdout=subprocess.PIPE,
                              shell=False)
    if pdflatex.returncode != 0:
        logging.error(' '.join(command_as_list) + '\n'
                      + pdflatex.stdout.decode("utf-8"))
        logging.error('The pdflatex command returned an error.')
        exit(1)

    tex_pdf = os.path.join(tex_dir, tex_name + '.pdf')

# Returns a string containing the tex of the statement (only what shall go
# inside \begin{document} \end{document}).
# The samples (.in/.out) and the images are copied in tex_dir/samples and
# tex_dir/images respectively.
def generate_statement_tex(problem, tex_dir):
    pathlib.Path(os.path.join(tex_dir, 'samples')).mkdir(exist_ok=True)
    pathlib.Path(os.path.join(tex_dir, 'images')).mkdir(exist_ok=True)
    
    samples_tex = ''

    sample_cnt = 0
    for sample in problem['statement']['samples']:
        sample_cnt += 1

        sample_path = os.path.join(tex_dir, 'samples',
                                   problem['name'] + '-' + str(sample_cnt))

        shutil.copyfile(sample['in'], sample_path + '.in')
        shutil.copyfile(sample['out'], sample_path + '.out')

        samples_tex += '\\sample{%s}\n' % sample_path

        if sample['explanation']:
            samples_tex += '\\sampleexplanation{%s}\n' % sample['explanation']

    if sample_cnt == 0:
        logging.error('No samples found.')
        exit(1)

    with open(os.path.join(RESOURCES_PATH, 'statement_template.tex')) as f:
        statement_template = f.read()
    
    # Some of these sections may be empty, in that case remove them.
    for section_title in ['input', 'output', 'interaction']:
        section_content = problem['statement'][section_title]
        if section_content is None or str(section_content).strip() == '':
            statement_template = statement_template.replace(
                '\\section*{%s}' % section_title.capitalize(), ''
            )

    replacements_statement = {
        'LABEL': problem['label'],
        'COLOR': problem['color'],
        'TITLE': problem['title'],
        'TIMELIMIT': problem['timelimit'],
        'MEMORYLIMIT': problem['memorylimit'],
        'LEGEND': problem['statement']['legend'],
        'INPUT': problem['statement']['input'],
        'OUTPUT': problem['statement']['output'],
        'INTERACTION': problem['statement']['interaction'],
        'SAMPLES': samples_tex
    }
    for placeholder in replacements_statement:
        statement_template = statement_template.replace(
            '??%s??' % placeholder, str(replacements_statement[placeholder]))

    for image in problem['statement']['images']:
        # Giving a name depending on the problem name to the image to avoid
        # collisions with images of other statements/solutions.
        image_unique_name = os.path.join(
            'images', problem['name'] + '-' + image[0])
        statement_template = statement_template.replace(image[0], image_unique_name)
        shutil.copyfile(image[1], os.path.join(tex_dir, image_unique_name))

    return statement_template


# Returns a string containing the tex source of the solution (only what shall
# go inside \begin{document} \end{document}).
# The images are copied in tex_dir/images.
def generate_solution_tex(problem, tex_dir):
    with open(os.path.join(RESOURCES_PATH, 'solution_template.tex')) as f:
        solution_template = f.read()

    replacements_solution = {
        'LABEL': problem['label'],
        'COLOR': problem['color'],
        'TITLE': problem['title'],
        'AUTHOR': problem['author'],
        'PREPARATION': problem['preparation'],
        'SOLUTION': problem['statement']['tutorial']
    }
    for placeholder in replacements_solution:
        solution_template = solution_template.replace(
            '??%s??' % placeholder, str(replacements_solution[placeholder]))

    for image in problem['statement']['images']:
        # Giving a name depending on the problem name to the image to avoid
        # collisions with images of other statements/solutions.
        image_unique_name = os.path.join(
            'images', problem['name'] + '-' + image[0])
        solution_template = solution_template.replace(image[0], image_unique_name)
        shutil.copyfile(image[1], os.path.join(tex_dir, image_unique_name))

    return solution_template


# Function to transform the statement of one or more problems (the content of
# \begin{document} ... \end{document}) into a pdf file.
#
# It performs the following operations:
# 1. Fill document_template.tex document tag with document_content;
# 2. Replace the placeholders in params using params;
# 3. Save the resulting tex in tex_file;
# 4. Compile tex_file. If tex_file = 'path/name.tex', the pdf file produced is
#    'path/name.pdf'.
#
# params is a dictionary with keys contest_name, hide_balloon, hide_tlml.
def compile_document_template(document_content, tex_file, params):
    replacements_document = {
        'CONTESTNAME': params['contest_name'],
        'SHOWBALLOON': 0 if params['hide_balloon'] else 1,
        'SHOWTLML': 0 if params['hide_tlml'] else 1,
        'DOCUMENTCONTENT': document_content
    }
    with open(os.path.join(RESOURCES_PATH, 'document_template.tex')) as f:
        document_template = f.read()

    for placeholder in replacements_document:
        document_template = document_template.replace(
            '??%s??' % placeholder, str(replacements_document[placeholder]))

    with open(tex_file, 'w') as f:
        f.write(document_template)

    if os.name == 'nt':
        tex2pdf(tex_file)

# Produces problemname-statement.{tex,pdf}, which are respectively the tex source
# and the pdf of the statement, in the directory tex_dir.
#   params is a dictionary with keys contest_name, hide_balloon, hide_tlml.
def generate_statement_pdf(problem, tex_dir, params):
    statement_tex = generate_statement_tex(problem, tex_dir)
    compile_document_template(
        statement_tex,
        os.path.join(tex_dir, problem['name'] + '-statement.tex'),
        params)

# Produces problemname-solution.{tex,pdf}, which are respectively the tex source
# and the pdf of the solution, in the directory tex_dir.
#   params is a dictionary with keys contest_name, hide_balloon, hide_tlml.
def generate_solution_pdf(problem, tex_dir, params):
    solution_tex = generate_solution_tex(problem, tex_dir)
    compile_document_template(
        solution_tex,
        os.path.join(tex_dir, problem['name'] + '-solution.tex'),
        params)

# Produces the complete problem set of a contest and saves it as
# tex_dir/statements.pdf.
#   problems is a list of problem names, in the order they shall appear.
#   tex_dir must contain problem-statement-content.tex for each problem in
#   problems.
#   params is a dictionary with keys contest_name, hide_balloon, hide_tlml.
def generate_statements_pdf(problems, frontpage, tex_dir, params):
    problemset_tex = ''

    if frontpage:
        frontpage = os.path.abspath(frontpage)
        problemset_tex += '\\includepdf{%s}\n' % frontpage
        problemset_tex += '\\insertblankpageifnecessary\n\n'

    for problem in problems:
        maybe_tex = os.path.join(tex_dir, problem + '-statement-content.tex')
        if not os.path.isfile(maybe_tex):
            logging.warning('The tex source \'%s\' does not exist; but it is required to generate the pdf with all problems.' % maybe_tex)
            continue
        problemset_tex += '\\input{%s-statement-content.tex}\n' % problem
        problemset_tex += '\\insertblankpageifnecessary\n\n'

    # Executing pdflatex twice because otherwise the command
    # \insertblankpageifnecessary does not produce the correct output.
    for _ in range(2):
        compile_document_template(
                problemset_tex,
                os.path.join(tex_dir, 'statements.tex'),
                params)

# Produces the complete editorial of a contest and saves it as tex_dir/solutions.pdf.
#   problems is a list of problem names, in the order they shall appear.
#   tex_dir must contain problemname-solution-content.tex for each problem
#   in problems.
#   params is a dictionary with keys contest_name, hide_balloon, hide_tlml.
def generate_solutions_pdf(problems, frontpage, tex_dir, params):
    solutions_tex = ''
    
    if frontpage:
        frontpage = os.path.abspath(frontpage)
        solutions_tex += '\\includepdf{%s}\n\n' % frontpage

    for problem in problems:
        maybe_tex = os.path.join(tex_dir, problem + '-solution-content.tex')
        if not os.path.isfile(maybe_tex):
            logging.warning('The tex source \'%s\' does not exist; but it is required to generate the pdf with all solutions.' % maybe_tex)
            continue
        solutions_tex += '\\input{%s-solution-content.tex}\n' % problem
        solutions_tex += '\\clearpage\n'
    
    compile_document_template(
            solutions_tex,
            os.path.join(tex_dir, 'solutions.tex'),
            params)

def get_tex_path(src_folder):
    """
    Get the path to the problem.tex file in a problem folder.
    """
    # List of potential paths to the problem.tex file
    potential_paths = ['problem_statement/problem.tex']   
    # Check each potential path
    for path in potential_paths:
        check_path = os.path.join(src_folder, path)
        if os.path.exists(check_path):
            return check_path
    return None

def get_label_from_path(file_path):
    """
    Extract a label from a file path.
    """
    return os.path.basename(os.path.normpath(file_path))

def is_valid_label(label):
    """
    Check if a label is a single uppercase letter.
    """
    return len(label) == 1 and label.isupper()

def extract_contest_tex_data(tex_path=None):
    """
    Extract contest data from a LaTeX file.
    """
    if tex_path is None:
        return None
    with open(tex_path, 'r') as f:
        data = f.read()
    # Define patterns to search for in the LaTeX data
    patterns = {
        'problemname': [r'\\problemname{([^}]+)}'],
    }
    
    # Extract data based on patterns
    extracted_data = {}
    for key, value_patterns in patterns.items():
        matches = []
        for pattern in value_patterns:
            matches += re.findall(pattern, data)
        extracted_data[key] = matches[0].strip() if matches else None
    return extracted_data

def are_strings_consecutive_uppercase_letters(strings):
    """
    Check if a list of strings are consecutive uppercase letters.
    """
    if not strings or len(strings) < 1:
        return False
    # Check if each string is a single uppercase letter and consecutive
    for i in range(len(strings) - 1):
        if (
            len(strings[i]) == 1
            and strings[i].isupper()
            and len(strings[i + 1]) == 1
            and strings[i + 1].isupper()
            and ord(strings[i]) + 1 != ord(strings[i + 1])
        ):
            return False
    return True

def are_labels_correct(labels):
    """
    Check if a list of labels are correct (consecutive uppercase letters).
    """
    sorted_labels = sorted(labels)
    return are_strings_consecutive_uppercase_letters(sorted_labels)

def find_missing_letters(strings):
    """
    Find missing uppercase letters in a list of strings between the lowest and highest letters.
    """
    if not strings:
        return []

    # Determine the lowest and highest letters available in the input strings
    letters = [s for s in strings if len(s) == 1 and s.isupper()]
    
    if not letters:
        return []

    lowest_letter = min(letters)
    highest_letter = max(letters)

    # Create a set of all uppercase letters between the lowest and highest letters
    alphabet_set = set(chr(ord(lowest_letter) + i) for i in range(ord(highest_letter) - ord(lowest_letter) + 1))

    # Create a set of found uppercase letters from the input strings
    found_set = set(letters)

    # Find missing letters
    missing_letters = sorted(list(alphabet_set - found_set))

    return missing_letters

def get_missing_labels(labels):
    """
    Get missing labels from a list of labels.
    """
    return find_missing_letters(labels)

def extract_contest_data(src):
    """
    Extract contest data from a source directory.
    """
    problem_info = {}

    # Loop through each problem in the source directory
    for problem_label in os.listdir(src):
        problem_path = os.path.join(src, problem_label)

        # Skip non-directories and invalid labels
        if not os.path.isdir(problem_path) or not is_valid_label(get_label_from_path(problem_path)):
            continue
        problem_info[problem_label] = extract_contest_tex_data(tex_path=get_tex_path(problem_path))
    # Sort the problem_info dictionary
    problem_info = dict(sorted(problem_info.items()))

    problem_labels = list(problem_info.keys())
    # Check if labels are correct
    if not are_labels_correct(problem_labels):
        logging.warning(f"WARNING: Missing labels\n{get_missing_labels(problem_labels)}\nProblem labels available: {problem_labels}", None)
        return

    return problem_info

def escape_special_characters(input_string):
    """
    Escape special characters in the input string.
    """
    # Define a dictionary of special characters and their escaped versions
    special_characters = {
        '\t': r'\t',
        '\a': r'\a',
        '\c': r'\c',
        '\f': r'\f',
        '\e': r'\e',
        '\g': r'\g',
        '\h': r'\h',
        '\i': r'\i',
        '\j': r'\j',
        '\k': r'\k',
        '\l': r'\l',
        '\m': r'\m',
        '\o': r'\o',
        '\p': r'\p',
        '\q': r'\q',
        '\r': r'\r',
        '\v': r'\v',
        '\y': r'\y',
        '\z': r'\z',
    }

    # Create a regular expression pattern to match any of the special characters
    pattern = re.compile("|".join(re.escape(char) for char in special_characters))

    # Replace the matched special characters with their escaped versions
    escaped_string = pattern.sub(lambda match: special_characters[match.group(0)], input_string)

    return escaped_string


def create_contest_title(contest_title):
    """
    Create a formatted contest title.
    """
    # Split the contest title into words
    words = contest_title.strip().split()

    # Define title formatting components
    title_start = r'\textsc{\Huge '
    title_end = r'}\\[0.5cm]'
    title_separator = r' \\[0.3cm] '

    # Calculate the split point for the title lines
    split_point = round(len(words) - 2)

    # Create the two title lines
    line1 = " ".join(words[:split_point])

    line2 = " ".join(words[split_point:])

    # Combine the title components to create the complete title
    complete_title = title_start + line1 + title_separator + line2 + title_end

    return complete_title

def clean_tex_file(generated_filename, lines_to_remove = [
        r'\usepackage[T1]{fontenc}%',
        r'\usepackage[utf8]{inputenc}%',
        r'\usepackage{lmodern}%',
        r'\usepackage{textcomp}%',
        r'\usepackage{lastpage}%'
    ]):
    """
    Clean a LaTeX file by removing specified lines.
    """
    try:
        # Open the .tex file for reading
        with open(f'{generated_filename}.tex', 'r') as tex_file:
            tex_content = tex_file.readlines()

        # Create a list to store the modified content
        modified_tex_content = []

        # Iterate through the lines in the original .tex content
        for line in tex_content:
            # Check if the current line contains any of the lines to be removed
            if any(line.strip() == remove_line for remove_line in lines_to_remove):
                continue  # Skip this line
            if "%\n" in line:
                line = line.replace("%\n", "\n")
            # Add the line to the modified content
            modified_tex_content.append(line)

        # Write the modified content back to the .tex file
        with open(f'{generated_filename}.tex', 'w') as tex_file:
            tex_file.writelines(modified_tex_content)
    
    except FileNotFoundError:

        logging.error(f'Error: File {generated_filename}.tex not found.')
    
    return

def build_custom_preamble():
    """
    Build a custom LaTeX preamble.
    """
    return [
        Command('usepackage', 'graphicx,tikz'),
        NoEscape((r'\newcommand{\HRule}{\rule{\linewidth}{0.5mm}}')),
        NoEscape("")
    ]

def build_custom_separator():
    """
    Build a custom LaTeX separator for sections.
    """
    return [
        NoEscape((r"\vskip 20pt")),
        NoEscape((r"\hrule")),
        NoEscape((r"\vskip 20pt")),
        NoEscape(("")),
    ]

def build_title_section(contest_title, contest_date):
    """
    Build the title section of the contest LaTeX document.
    """
    contest_title = create_contest_title(contest_title)
    title_section = [
        NoEscape(contest_title),
        NoEscape(""),
        NoEscape(r'\textsc{\Large ' + contest_date + r'}\\[0.3cm]'),
        NoEscape(""),
        NoEscape(r'\HRule \\[0.3cm]'),
        NoEscape(""),
        NoEscape(r'{ \huge \bfseries Contest Problems \\[0.3cm] }'),
        NoEscape(""),
        NoEscape(r'\HRule \\[0.5cm]'),
        NoEscape(""),
        NoEscape(r'{\large')
    ]
    return title_section

def build_problem_table(contest_doc, problems):
    """
    Build the problem table in the contest LaTeX document.
    """
    # Create a Tabular and add rows for problems
    with contest_doc.create(Tabular('l@{:\hskip4pt}l')) as table:
        for label in problems:
            table.add_row([label, NoEscape(problems[label]['problemname'])])
    return

def build_footer(problems):
    """
    Build the footer section of the contest LaTeX document.
    """
    p = inflect.engine()
    return [
        NoEscape(r'\vskip 40pt'),
        NoEscape(""),
        NoEscape(r'\begin{center}'),
        f'This contest contains {p.number_to_words(len(problems))} problems. Good luck.',
        NoEscape(r'\end{center}'),
        NoEscape("")
    ]

def build_problem_answer_details(problem_answer_details):
    """
    Build the problem answer details section of the contest LaTeX document.
    """
    return [
        NoEscape(r'\vskip 20pt'),
        NoEscape(escape_special_characters(problem_answer_details)),
        NoEscape("")
    ]

def build_definition_sections(contest_doc, definitions, custom_separator):
    """
    Build the definition sections in the contest LaTeX document.
    """
    for d, definition in enumerate(definitions):
        # Add a custom separator before each definition (except the first one)
        if d > 0:
            for separator in custom_separator:
                contest_doc.append(separator)
        
        section = [
            NoEscape(r'\subsection*{Definition ' + str(d + 1) + '}'),
            NoEscape(""),
            NoEscape(escape_special_characters(definition))
        ]
        
        for item in section:
            contest_doc.append(item)

def ensure_dir(s):
    """
    Create a directory if it does not already exist.
    """
    if not os.path.exists(s):
        os.makedirs(s)