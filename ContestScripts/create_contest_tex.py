#! /usr/bin/env python
"""
A python script which creates a context latex file from a folder of DOMjudge programming contest problems.
"""
import yaml
import argparse
from helper import extract_contest_data, build_contest_tex, clean_tex_file
import os

def create_contest_tex(src , dst):
    print("Extracting contest data...")
    problem_info = extract_contest_data(src)
    
    with open(os.path.join(src,'config.yaml'), 'r') as file:
        config = yaml.safe_load(file)
    new_dst = os.path.join(dst, config['contest_latex_filename'])
    
    print("Building contest latex file...")
    build_contest_tex(config['contest_name'], config['contest_shortname'], config['definitions'], new_dst, problem_info, config['header_logo_filename'], config['contest_date'], config['contest_cover_filename'], config['contest_title'], config['problem_answer_details'])
    
    print("Cleaning contest latex...")
    clean_tex_file(new_dst)
    
    print("Created " + str(new_dst) + ".tex")
    return

def main():
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

    create_contest_tex(src, dst)

if __name__ == "__main__":
    main()
    
# create_contest_tex(
#     src = "/Users/vybien/Desktop/Projects/cits3200-project/ContestScripts/testing/xxRegionals", 
#     dst = "/Users/vybien/Desktop/Projects/cits3200-project/ContestScripts/testing/xxRegionals")

