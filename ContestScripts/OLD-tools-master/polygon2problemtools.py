#/usr/bin/env python3
import os,math
from sys import argv
import argparse
from shutil import copyfile,make_archive,rmtree
import xml.etree.ElementTree

################################################################################
#   Helpers
################################################################################
def fatal(s):
	print(s)
	exit(1)

def ensure_dir(s):
	if not os.path.exists(s):
		os.makedirs(s)

def ensure_no_dir(s):
	if os.path.exists(s):
		rmtree(s)

################################################################################
#   Parsing parameters
################################################################################

parser = argparse.ArgumentParser(description='Process Polygon Package to problemtools format.')
parser.add_argument('package',       type=str, help='Path of the Polygon package')
parser.add_argument('-o','--output', type=str, help='Path of the output problemtools directory (default: ./<PKG>-problemtools)')
parser.add_argument('--zip',         action='store_true', help='Zip the problemtools directory')
parser.add_argument('--delete-dir',  action='store_true', help='Delete the problemtools directory')
parser.add_argument('--no-statement',action='store_true', help='Do not make the problem statement')
parser.add_argument('--domjudge',    action='store_true', help='Make domjudge.ini')
parser.add_argument('--shortcode',   type=str, help='The shortcode for the problem on DOMjudge')
parser.add_argument('--colour',      type=str, help='The problem colour for DOMjudge (in RRGGBB format)')
parser.add_argument('--ignore-bad-sols', action='store_true', help='Ignore solutions that are not AC/TLE/WA.')
args = parser.parse_args()

################################################################################
#   Some global variables...
################################################################################
PACKAGE_FULL = args.package
PACKAGE_NAME = os.path.splitext(os.path.basename(args.package))[0]
DEFAULT_OUTPUT_DIR = PACKAGE_NAME+'-problemtools/'
OUTPUT_DIR = DEFAULT_OUTPUT_DIR if not args.output else args.output + '/'

if PACKAGE_FULL.endswith(".zip"):
	#Extracting the package
	unzipped = True
	import zipfile
	if not os.path.isfile(args.package):
		fatal("[ERROR] PACKAGE ZIP NOT FOUND")
	zip_ref = zipfile.ZipFile(args.package, 'r')
	ensure_no_dir(PACKAGE_DIR)
	zip_ref.extractall(PACKAGE_DIR)
	zip_ref.close()
else:
	PACKAGE_DIR = args.package
STATEMENT_DIR = PACKAGE_DIR+'/statement-sections/english/'

num_cases = [0,0]
num_soln  = [0,0,0]

################################################################################
#   Set up the directory
################################################################################
if not os.path.exists(PACKAGE_DIR):
	fatal("[ERROR] PACKAGE DIRECTORY NOT FOUND")

ensure_no_dir(OUTPUT_DIR)
ensure_dir(OUTPUT_DIR)

ensure_dir(OUTPUT_DIR+'/data')
ensure_dir(OUTPUT_DIR+'/data/sample')
ensure_dir(OUTPUT_DIR+'/data/secret')

ensure_dir(OUTPUT_DIR + '/submissions')
ensure_dir(OUTPUT_DIR + '/submissions/accepted')
ensure_dir(OUTPUT_DIR + '/submissions/wrong_answer')
ensure_dir(OUTPUT_DIR + '/submissions/time_limit_exceeded')

################################################################################
#   XML Parsing
################################################################################
root = xml.etree.ElementTree.parse(PACKAGE_DIR+'/problem.xml').getroot()
problem_name = root.find('names').find('name').attrib['value']
timelimit = int(math.ceil(float(root.find('judging').find('testset').find('time-limit').text)/1000.0))
solutions = root.find('assets').find('solutions').findall('solution')

################################################################################
#   DOMjudge ini file
################################################################################
if args.domjudge:
	assert args.shortcode
	assert args.colour

	desc = open(OUTPUT_DIR+'/domjudge-problem.ini','w+')
	desc.write("probid='"+args.shortcode+"'\n")
	desc.write("name='"+problem_name.replace("'","`")+"'\n")
	desc.write("timelimit='"+str(timelimit)+"'\n")
	desc.write("color='"+args.colour+"'\n")
	desc.close()
else:
	if args.shortcode:
		fatal("--shortcode is only valid with --domjudge")
	if args.colour:
		fatal("--colour is only valid with --domjudge")

################################################################################
#   Timelimit file
################################################################################
timelim_file = open(OUTPUT_DIR+'/.timelimit','w+')
timelim_file.write(str(timelimit) + "\n'")
timelim_file.close()

################################################################################
#   Test Cases
################################################################################
sampletests = sorted(filter(lambda x:(x.startswith("example") and not x.endswith(".a")) and (not x.endswith(".tex")),os.listdir(STATEMENT_DIR)))
testidx = 1
for test in sampletests:
	print(test)
	copyfile(STATEMENT_DIR+'/'+test      , OUTPUT_DIR+'/data/sample/'+str(testidx)+'.in')
	copyfile(STATEMENT_DIR+'/'+test+".a" , OUTPUT_DIR+'/data/sample/'+str(testidx)+'.ans')
	testidx += 1
	num_cases[0] += 1

tests = filter(lambda x:not x.endswith(".a"),os.listdir(PACKAGE_DIR+'/tests'))
for test in tests:
	copyfile(PACKAGE_DIR+'/tests/'+test      , OUTPUT_DIR+'/data/secret/'+test+'.in')
	copyfile(PACKAGE_DIR+'/tests/'+test+".a" , OUTPUT_DIR+'/data/secret/'+test+'.ans')
	num_cases[1] += 1

################################################################################
#   Solutions
################################################################################
for soln in solutions:
	verdict = soln.get('tag')
	path    = soln.find('source').get('path')
	prog    = os.path.basename(path)

	output_verdict = "huh"
	if verdict == "main" or verdict == "accepted":
		output_verdict = "accepted"
		num_soln[0] += 1
	if verdict == "wrong-answer" or verdict == "rejected":
		output_verdict = "wrong_answer"
		num_soln[1] += 1
	if verdict == "time-limit-exceeded":
		output_verdict = "time_limit_exceeded"
		num_soln[2] += 1
	# Arbitrarily call this OR a TLE, since domjudge doesn't seem to support ORs.
	if verdict == "time-limit-exceeded-or-memory-limit-exceeded":
		print("WARNING: Treating solution {} with verdict=time-limit-exceeded-or-memory-limit-exceeded as TLE.".format(prog))
		output_verdict = "time_limit_exceeded"
		num_soln[2] += 1
	# Arbitrarily call this OR a TLE, since domjudge doesn't seem to support ORs.
	if verdict == "time-limit-exceeded-or-accepted":
		print("WARNING: Treating solution {} with verdict=time-limit-exceeded-or-accepted as TLE.".format(prog))
		output_verdict = "time_limit_exceeded"
		num_soln[2] += 1

	if output_verdict == "huh":
		if args.ignore_bad_sols:
			print("I don\'t know what verdict="+verdict+" is. Use AC, WA, TLE.")
			continue
		else:
			fatal("I don\'t know what verdict="+verdict+" is. Use AC, WA, TLE.")

	copyfile(PACKAGE_DIR + '/' + path, OUTPUT_DIR + 'submissions/' + output_verdict + '/' + prog)

################################################################################
#   Make the problem statement
################################################################################
if not args.no_statement:
	ensure_dir(OUTPUT_DIR+'/problem_statement')

	statement = open(OUTPUT_DIR+'/problem_statement/problem.tex','w+')
	statement.write("\\problemname{" + open(STATEMENT_DIR+'name.tex').read() + "}")
	statement.write("\n\n")
	statement.write(open(STATEMENT_DIR+'legend.tex').read())
	statement.write("\n\n")
	statement.write("\\section*{Input}")
	statement.write("\n\n")
	statement.write(open(STATEMENT_DIR+'input.tex').read())
	statement.write("\n\n")
	statement.write("\\section*{Output}")
	statement.write("\n\n")
	statement.write(open(STATEMENT_DIR+'output.tex').read())
	statement.write("\n")
	statement.close()

################################################################################
#   Zip the directory:
################################################################################
if args.zip:
	make_archive(OUTPUT_DIR, 'zip', OUTPUT_DIR)

################################################################################
#   Clean-up
################################################################################
if PACKAGE_FULL.endswith(".zip"):
	rmtree(PACKAGE_DIR)
if args.delete_dir:
	rmtree(OUTPUT_DIR)

################################################################################
#   Some information...
################################################################################
print(PACKAGE_FULL)
print(sum(num_soln) ," solutions (",num_soln[0]," AC / " ,num_soln[1], " WA / ", num_soln[2] , "TLE)")
print(sum(num_cases)," cases     (",num_cases[0]," Sample / " , num_cases[1], " secret)")
