import os
import argparse
from p2d import tex_utilities

def generate_problem_latex(BASE_PACKAGE_DIR, BASE_OUTPUT_DIR):

    latex_statement = r"""
    \intextsep2mm
    \begin{wrapfigure}{r}{50mm}
    \includegraphics[width=50mm]{img.png}
    \end{wrapfigure}
    ~
    """

    for item in os.listdir(BASE_PACKAGE_DIR):
        item_path = os.path.join(BASE_PACKAGE_DIR, item)
        if os.path.isdir(item_path) and tex_utilities.is_valid_label(item):
            OUTPUT_DIR = os.path.join(BASE_OUTPUT_DIR, item)
            STATEMENT_DIR = os.path.join(BASE_PACKAGE_DIR, item, 'statement-sections', 'english')
            tex_utilities.ensure_dir(OUTPUT_DIR+'/problem_statement')
            statement = open(os.path.join(OUTPUT_DIR,'problem_statement/problem.tex'),'w')
            statement.write("\\problemname{" + open(os.path.join(STATEMENT_DIR,'name.tex')).read() + "}")
            statement.write("\n\n") 
            statement.write(open(os.path.join(STATEMENT_DIR,'legend.tex')).read())
            statement.write("\n\n")
            statement.write("\\section*{Input}")
            statement.write("\n\n")
            statement.write(open(os.path.join(STATEMENT_DIR,'input.tex')).read())
            statement.write("\n\n")
            statement.write("\\section*{Output}")
            statement.write("\n\n")
            statement.write(open(os.path.join(STATEMENT_DIR,'output.tex')).read())
            statement.write("\n")
            statement.close()

def main():
    """
    Main function to create problem LaTeX files from polygon problems.
    """
    usage_msg = """%(prog)s [options] src dst"""

    parser = argparse.ArgumentParser(
        description="Create problem LaTeX files from a folder of polygon programming contest problems",
        usage=usage_msg
    )

    # src -> positional and optional argument
    parser.add_argument("src", type=str, nargs="?", help="Source folder containing labelled polygon problems")
    parser.add_argument("--src", type=str, dest="src_flag", help="Source folder containing labelled polygon problems")

    # dst -> positional and optional argument
    parser.add_argument("dst", type=str, nargs="?", help="Destination folder where the labelled DOMjudge problems are located")
    parser.add_argument("--dst", type=str, dest="dst_flag", help="Destination folder where the labelled DOMjudge problems are located")

    args = parser.parse_args()

    # use src and dst values
    # prioritise flags if given
    src = args.src_flag or args.src
    dst = args.dst_flag or args.dst

    # Check if src and dst are provided
    if src is None or dst is None:
        parser.error("Both src and dst are required. Use positional arguments or flags.")

    generate_problem_latex(src, dst)

if __name__ == "__main__":
    main()
