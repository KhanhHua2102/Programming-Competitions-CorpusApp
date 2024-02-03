#! /usr/bin/env python
import os
import re
import warnings
import inflect
from pylatex import Document, Command, Tabular
from pylatex.utils import NoEscape

def get_tex_path(src_folder):
    # List of potential paths to the problem.tex file
    potential_paths = ['problem_statement/problem.tex']
    
    # Check each potential path
    for path in potential_paths:
        check_path = os.path.join(src_folder, path)
        if os.path.exists(check_path):
            return check_path
    return None

def get_label_from_path(file_path):
    # Extract the label from a file path
    return os.path.basename(os.path.normpath(file_path))

def is_valid_label(label):
    # Check if a label is valid (single uppercase letter)
    return len(label) == 1 and label.isupper()

def extract_contest_tex_data(tex_path=None):

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
    if not strings or len(strings) <= 1:
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
    sorted_labels = sorted(labels)
    return are_strings_consecutive_uppercase_letters(sorted_labels)

def find_missing_letters(strings):
    if not strings:
        return []
    
    # Create a set of all uppercase letters
    alphabet_set = set(chr(ord('A') + i) for i in range(26))
    
    # Create a set of found uppercase letters from the input strings
    found_set = set(s for s in strings if len(s) == 1 and s.isupper())
    
    # Find missing letters
    missing_letters = sorted(list(alphabet_set - found_set))
    return missing_letters

def get_missing_labels(labels):
    return find_missing_letters(labels)

def extract_contest_data(src):
    """
    Extract contest data from the given source directory.

    Args:
        src (str): The source directory containing problem data.

    Returns:
        dict: A dictionary containing extracted contest data.
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
        print(problem_labels)
        warnings.warn(f"WARNING: Missing labels\n{get_missing_labels(problem_labels)}", None)
        return

    return problem_info

def escape_special_characters(input_string):
    """
    Escape special characters in the input string.

    Args:
        input_string (str): The input string to escape.

    Returns:
        str: The input string with special characters escaped.
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

        print(f'Error: File {generated_filename}.tex not found.')
    
    return

def build_custom_preamble():
    return [
        Command('usepackage', 'graphicx,tikz'),
        NoEscape((r'\newcommand{\HRule}{\rule{\linewidth}{0.5mm}}')),
        NoEscape("")
    ]

def build_custom_separator():
    return [
        NoEscape((r"\vskip 20pt")),
        NoEscape((r"\hrule")),
        NoEscape((r"\vskip 20pt")),
        NoEscape(("")),
    ]

def build_title_section(contest_title, contest_date):
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
    # Create a Tabular and add rows for problems
    with contest_doc.create(Tabular('l@{:\hskip4pt}l')) as table:
        for label in problems:
            table.add_row([label, NoEscape(problems[label]['problemname'])])
    return

def build_footer(problems):
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
    return [
        NoEscape(r'\vskip 20pt'),
        NoEscape(escape_special_characters(problem_answer_details)),
        NoEscape("")
    ]

def build_definition_sections(contest_doc, definitions, custom_separator):
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

def build_contest_tex(contest_name, contest_shortname, definitions, generated_filename, problems, header_logo_filename, contest_date, contest_cover_filename, contest_title, problem_answer_details):
    # Sort problems dictionary
    problems = dict(sorted(problems.items()))

    custom_preamble = build_custom_preamble()
    custom_separator = build_custom_separator()
    title_section = build_title_section(contest_title, contest_date)
    footer = build_footer(problems)
    problem_answer_details = build_problem_answer_details(problem_answer_details)

    contest_doc = Document(documentclass="problemset", document_options=["noproblemids", "a4paper"])
    contest_doc.preamble.clear()

    for command in custom_preamble:
        contest_doc.preamble.append(command)

    contest_doc.append(NoEscape(""))
    contest_doc.preamble.append(Command('contestname', contest_name))
    contest_doc.preamble.append(Command('contestshortname', contest_shortname))

    if header_logo_filename is not None:
        contest_doc.preamble.append(Command('headerlogo', NoEscape(header_logo_filename)))

    contest_doc.append(NoEscape(""))
    contest_doc.append(NoEscape(r'\thispagestyle{empty}'))
    contest_doc.append(NoEscape(r'\begin{center}'))
    contest_doc.append(Command('vspace*', NoEscape(r'\fill')))
    contest_doc.append(NoEscape(r'\includegraphics[width=10cm]{' + contest_cover_filename + r'} \\'))
    contest_doc.append(NoEscape(""))

    # Add title section to the document
    for section in title_section:
        contest_doc.append(section)

    # Create the problem table within the contest document
    build_problem_table(contest_doc, problems)
    for item in [
        NoEscape(r"}"), 
        NoEscape(r"\vspace*{\fill}"),
        NoEscape(r"\end{center}"),
        NoEscape(r""),
        NoEscape(r"\clearpage"),
        NoEscape(r""),
        NoEscape(r"~")]:
        contest_doc.append(item)

    # Add footer section
    for item in footer:
        contest_doc.append(item)

    # Add problem answer details
    for detail in problem_answer_details:
        contest_doc.append(detail)
    
    for separator in custom_separator:
        contest_doc.append(separator)

    # Add definition sections
    build_definition_sections(contest_doc, definitions, custom_separator)

    contest_doc.append(NoEscape(""))
    
    # Include problems in the document
    for label in problems:
        contest_doc.append(Command('includeproblem', label))

    contest_doc.append(NoEscape(""))
    # Generate the LaTeX document
    contest_doc.generate_tex(generated_filename)
    return
