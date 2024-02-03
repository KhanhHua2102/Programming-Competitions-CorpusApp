#! /usr/bin/env python
"""
A python script which creates a context latex file from
a folder of DOMjudge programming contest problems.
"""
import os
import argparse
import yaml
import logging
from p2d import tex_utilities
from pylatex import Document, Command
from pylatex.utils import NoEscape

def build_contest_tex(contest_name, contest_shortname, definitions, generated_filename, problems, header_logo_filename, contest_date, contest_cover_filename, contest_title, problem_answer_details):
    """
    Build the contest LaTeX document.
    """
    # Sort problems dictionary
    problems = dict(sorted(problems.items()))

    custom_preamble = tex_utilities.build_custom_preamble()
    custom_separator = tex_utilities.build_custom_separator()
    title_section = tex_utilities.build_title_section(contest_title, contest_date)
    footer = tex_utilities.build_footer(problems)
    problem_answer_details = tex_utilities.build_problem_answer_details(problem_answer_details)

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
    tex_utilities.build_problem_table(contest_doc, problems)
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
    tex_utilities.build_definition_sections(contest_doc, definitions, custom_separator)

    contest_doc.append(NoEscape(""))
    
    # Include problems in the document
    for label in problems:
        contest_doc.append(Command('includeproblem', label))

    contest_doc.append(NoEscape(""))
    # Generate the LaTeX document
    contest_doc.generate_tex(generated_filename)
    return


def create_contest_latex(src, dst):
    """
    Create a contest LaTeX file from DOMjudge problems.
    """
    logging.info("Extracting contest data")
    problem_info = tex_utilities.extract_contest_data(src)
    
    with open(os.path.join(src,'../config.yaml'), 'r') as file:
        config = yaml.safe_load(file)
    new_dst = os.path.join(dst, config['contest_latex_filename'])
    
    logging.info("Building contest latex file")
    build_contest_tex(config['contest_name'], config['contest_shortname'], config['definitions'], new_dst, problem_info, config['header_logo_filename'], config['contest_date'], config['contest_cover_filename'], config['contest_title'], config['problem_answer_details'])
    
    logging.info("Cleaning contest latex")
    tex_utilities.clean_tex_file(new_dst)
    
    logging.info("Created " + str(new_dst) + ".tex")
    return

def main():
    """
    Main function to create a contest LaTeX file from DOMjudge problems.
    """
    usage_msg = """%(prog)s [options] src dst"""

    parser = argparse.ArgumentParser(
        description="Create a context LaTeX file from a folder of DOMjudge programming contest problems",
        usage=usage_msg  # Set the custom usage message
    )

    # src -> positional and optional argument
    parser.add_argument("src", type=str, nargs="?", help="Source folder containing DOMjudge problems")
    parser.add_argument("--src", type=str, dest="src_flag", help="Source folder containing DOMjudge problems")

    # dst -> positional and optional argument
    parser.add_argument("dst", type=str, nargs="?", help="Destination folder where the LaTeX file will be created")
    parser.add_argument("--dst", type=str, dest="dst_flag", help="Destination folder where the LaTeX file will be created")

    args = parser.parse_args()

    # use src and dst values
    # prioritise flags if given
    src = args.src_flag or args.src
    dst = args.dst_flag or args.dst

    # Check if src and dst are provided
    if src is None or dst is None:
        parser.error("Both src and dst are required. Use positional arguments or flags.")

    create_contest_latex(src, dst)

if __name__ == "__main__":
    main()