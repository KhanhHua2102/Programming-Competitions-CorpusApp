import subprocess, os, random, hashlib, filecmp, difflib

from shutil import rmtree

# Define directories of existing polygon/problemtools problemsets

INPUT_DIR = "C:/Users/sanjeevan/Downloads/sppc-2022-main-Regionals 2/sppc-2022-main-Regionals/Regionals/polygon"
PROBLEM_TOOLS_DIR = "C:/Users/sanjeevan/Downloads/sppc-2022-main-Regionals 2/sppc-2022-main-Regionals/Regionals"
OUTPUT_DIR = "C:/Users/sanjeevan/Documents/CITS3200/created"
SCRIPT_PATH = "./ContestScripts/OLD-tools-master/polygon2problemtools.py"

# Ensure output dir is clean

if os.path.exists(OUTPUT_DIR):
	rmtree(OUTPUT_DIR)
        
if not os.path.exists(OUTPUT_DIR):
	os.makedirs(OUTPUT_DIR)

# Run script

for d in os.listdir(INPUT_DIR):
    directory = os.path.join(INPUT_DIR, d)
    if os.path.isdir(directory):
        name = os.path.basename(directory)
        colour = hashlib.md5(str(random.randint(0, 10000)).encode()).hexdigest()[:6]
        output = os.path.join(OUTPUT_DIR, 'domjudge', name)

        print(f"converting {directory} and placing in {output}, ok? (ctrl-c to stop)")
        #input()

        command = [
            'python3',
            SCRIPT_PATH,
            '--shortcode',
            name,
            '--colour',
            colour,
            '--domjudge',
            '-o',
            output,
            directory
        ]
        subprocess.run(command)

        # Convert files to Unix format
        for root, _, files in os.walk(output):
            for filename in files:
                file_path = os.path.join(root, filename)
                subprocess.run(['dos2unix', '-q', file_path])

        print("\n\n")
        break # ONLY comparing A right now

# Compare differences

def compare_files(file1, file2):
    if not filecmp.cmp(file1, file2, shallow=False):
        print(f"\nContent difference found between {file1} and {file2}")

def compare_directories(dir1, dir2):
    for name in os.listdir(dir1):
        path1 = os.path.join(dir1, name)
        path2 = os.path.join(dir2, name)

        if os.path.isdir(path1) and os.path.isdir(path2):
            compare_directories(path1, path2)  # Recursively compare subdirectories
        elif os.path.isfile(path1) and os.path.isfile(path2):
            compare_files(path1, path2)  # Compare file contents
        else:
            print(f"\nDifference found:")
            print(f"  In {dir1}, {name} is a {'directory' if os.path.isdir(path1) else 'file'}")
            print(f"  In {dir2}, {name} is a {'directory' if os.path.isdir(path2) else 'file' if os.path.exists(path2) else 'not found'}")
            return

    # Check for files/directories in dir2 that are not in dir1
    for name in os.listdir(dir2):
        if name not in os.listdir(dir1):
            print(f"\nDifference found: {name} exists in {dir2} but not in {dir1}")



compare_directories(OUTPUT_DIR, PROBLEM_TOOLS_DIR)