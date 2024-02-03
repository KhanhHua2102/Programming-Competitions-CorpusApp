import json
import os
import pathlib
import re
import shutil
import string
import sys
import tempfile
import webcolors
import yaml
import zipfile
import logging
from tqdm import tqdm

from p2d._version import __version__
from p2d import (domjudge_api,
                 generate_domjudge_package,
                 generate_testlib_for_domjudge,
                 parse_polygon_package,
                 polygon_api,
                 tex_utilities)
RESOURCES_PATH = os.path.join(
    os.path.split(os.path.realpath(__file__))[0], 'resources')

# Custom formatter for the console logger handler.
# Adapted from https://stackoverflow.com/questions/384076/how-can-i-color-python-logging-output
class Pol2DomLoggingFormatter(logging.Formatter):
    INDENT = 0

    bold_gray = '\033[1;90m'
    bold_green = '\033[1;92m'
    bold_yellow = '\033[1;33m'
    bold_red = '\033[1;91m'
    yellow = '\033[33m'
    red = '\033[91m'
    reset = '\033[0m'

    COLORS = {
        logging.DEBUG: (bold_gray, reset),
        logging.INFO: (bold_green, reset),
        logging.WARNING: (bold_yellow, yellow),
        logging.ERROR: (bold_red, red)
    }

    def format(self, record):
        level_color, message_color = self.COLORS.get(record.levelno)
        padding = ' ' * (10 - len(record.levelname))
        log_fmt = '  ' * self.INDENT + level_color + '{levelname}' + self.reset + padding + message_color + '{message}' + self.reset
        formatter = logging.Formatter(log_fmt, style='{')
        return formatter.format(record)

def configure_logging(verbosity):
    console_handler = logging.StreamHandler()
    console_handler.setLevel(eval('logging.' + verbosity.upper()))
    console_handler.setFormatter(Pol2DomLoggingFormatter())

    logging.basicConfig(
        format='{levelname}\t{message}',
        style='{',
        level=eval('logging.' + verbosity.upper()),
        handlers=[console_handler]
    )
    
    requests_log = logging.getLogger('requests.packages.urllib3')
    requests_log.setLevel(logging.DEBUG)
    requests_log.propagate = True

def manage_download(config, polygon_dir, problem):
    if 'polygon_id' not in problem:
        logging.warning('Skipped because polygon_id is not specified.')
        return
    
    # Check versions
    local_version = problem.get('polygon_version', -1)
    latest_package = polygon_api.get_latest_package_id(
        config['polygon']['key'], config['polygon']['secret'], problem['polygon_id'])

    logging.debug('For problem %s the selected package is %s.'
                  % (problem['name'], latest_package[1]))

    if latest_package[0] == -1:
        logging.warning('No packages were found on Polygon.')
        return

    if latest_package[0] < local_version:
        logging.warning('The local version is newer than the Polygon version.')
        return

    if latest_package[0] == local_version:
        logging.info('The Polygon package is up to date.')
        return

    pathlib.Path(polygon_dir).mkdir(exist_ok=True)
    
    # Download the package
    package_zip = os.path.join(polygon_dir, problem['name'] + '.zip')
    polygon_api.download_package(
        config['polygon']['key'], config['polygon']['secret'],
        problem['polygon_id'], latest_package[1], package_zip)

    # Unzip the package
    if not zipfile.is_zipfile(package_zip):
        logging.error(
            'There was an error downloading the package zip to \'%s\'.'
            % package_zip)
        return

    with zipfile.ZipFile(package_zip, 'r') as f:
        logging.debug('Unzipping the Polygon package \'%s\'.' % package_zip)
        f.extractall(polygon_dir)

    logging.info('Downloaded and unzipped the Polygon package into '
                 '\'%s\'.' % os.path.join(polygon_dir))
    problem['polygon_version'] = latest_package[0]

# Transforms the Polygon package contained in polygon_dir (already extracted)
# into an equivalent DOMjudge package in domjudge_dir. In domjudge_dir the
# package is contained extracted, but also a zip of the package itself is
# present.
# Moreover, this function creates the two tex files:
#   tex_dir/problem['name']-statement.tex
#   tex_dir/problem['name']-solution.tex
def manage_convert(config, polygon_dir, domjudge_dir, tex_dir, problem):
    # Check versions
    polygon_version = problem.get('polygon_version', -1)
    domjudge_version = problem.get('domjudge_local_version', -1)

    if polygon_version == -1:
        logging.warning('The Polygon package is not present locally.')
        # return

    if polygon_version < domjudge_version:
        logging.warning('The version of the local DOMjudge package is more '
                        'up to date the the local Polygon package.')
        # return

    if polygon_version == domjudge_version:
        logging.info('The local DOMjudge package is already up to date.')
        # return

    # Parse the Polygon package
    problem_package = parse_polygon_package.parse_problem_from_polygon(polygon_dir)

    if problem_package['name'] != problem['name']:
        logging.error('The name of the problem does not coincide with the name of the problem in Polygon, which is \'%s\'.' % problem_package['name'])
        exit(1)

    # Set some additional properties of the problem (not present in Polygon)
    missing_keys = list(filter(lambda key: key not in problem or not problem[key],
                               ['label', 'color', 'author', 'preparation']))
    if missing_keys:
        logging.warning('The keys %s are not set in config.yaml for this problem.' % missing_keys)
    
    problem_package['label'] = problem.get('label', '?')
    problem_package['color'] = convert_to_hex(problem.get('color', 'Black'))

    if 'override_time_limit' in problem:
        problem_package['timelimit'] = problem['override_time_limit']

    if 'override_memory_limit' in problem:
        problem_package['memorylimit'] = problem['override_memory_limit']

    problem_package['author'] = problem.get('author', '')
    problem_package['preparation'] = problem.get('preparation', '')

    logging.debug(json.dumps(problem_package, sort_keys=True, indent=4))

    # Generate the tex sources of statement and solution.
    problem_tex = tex_utilities.generate_statement_tex(problem_package, tex_dir)
    solution_tex = tex_utilities.generate_solution_tex(problem_package, tex_dir)

    statement_file = problem['name'] + '-statement-content.tex'
    solution_file = problem['name'] + '-solution-content.tex'
    
    with open(os.path.join(tex_dir, statement_file), 'w') as f:
        f.write(problem_tex)
    with open(os.path.join(tex_dir, solution_file), 'w') as f:
        f.write(solution_tex)

    # Generate the DOMjudge package.
    
    # The following three lines guarantee that in the end domjudge_dir
    # directory is empty.
    pathlib.Path(domjudge_dir).mkdir(exist_ok=True)
    shutil.rmtree(domjudge_dir)
    pathlib.Path(domjudge_dir).mkdir()

    generate_domjudge_package.generate_domjudge_package(
        problem_package, domjudge_dir, tex_dir,
        {
            'contest_name': config['contest_name'],
            'hide_balloon': config.get('hide_balloon', False),
            'hide_tlml': config.get('hide_tlml', False)
        })

    logging.info('Converted the Polygon package to the DOMjudge package \'%s\'.',
                 domjudge_dir)

    # Zip the package
    # A temporary file is necessary since make_archive goes crazy if the .zip
    # belongs to the directory that must be compressed.
    with tempfile.NamedTemporaryFile(suffix='.zip', mode='w', encoding='utf-8') as f:
        shutil.make_archive(f.name[:-4], 'zip', domjudge_dir)
        shutil.copyfile(f.name, os.path.join(domjudge_dir, problem['label'] + '.zip'))
    problem['domjudge_local_version'] = polygon_version

def manage_domjudge(config, domjudge_dir, problem):
    # Check versions
    local_version = problem.get('domjudge_local_version', -1)
    server_version = problem.get('domjudge_server_version', -1)

    if local_version == -1:
        logging.warning('The DOMjudge package is not present locally.')
        return

    if local_version < server_version:
        logging.warning('The version of the DOMjudge package on the server is '
                        'more up to date than the local one.')
        return

    if local_version == server_version:
        logging.info('The DOMjudge package on the server is already up to date.')
        return

        
    # Adding the problem to the contest if it was not already done.
    if 'domjudge_id' not in problem:
        if not domjudge_api.add_problem_to_contest_api(problem, config['domjudge']):
            logging.error('There was an error while adding the problem '
                          'to the contest in the DOMjudge server.')
            return

    # Sending the problem package to the server.
    assert('domjudge_id' in problem)
    zip_file = os.path.join(domjudge_dir, problem['name'] + '.zip')
    zip_file_copy = os.path.join(domjudge_dir,
                                 problem['domjudge_externalid'] + '.zip')
    shutil.copyfile(zip_file, zip_file_copy)
    
    if not domjudge_api.update_problem_api(
            zip_file_copy, problem['domjudge_id'], config['domjudge']):
        logging.error('There was an error while updating the problem '
                      'in the DOMjudge server.')
        return
            
    problem['domjudge_server_version'] = local_version

    logging.info('Updated the DOMjudge package on the server \'%s\', with id = \'%s\'.' % (config['domjudge']['server'], problem['domjudge_id']))

# Updates config with the data of the problems in the specified contest.
def fill_config_from_contest(config, contest_id):
    contest_problems = polygon_api.get_contest_problems(
        config['polygon']['key'], config['polygon']['secret'],
        contest_id
    )
    logging.info('Fetched problems from contest {}.'.format(contest_id))

    new_problems = []

    for label in contest_problems:
        problem = contest_problems[label]
        if problem['deleted']:
            continue
        if problem['id'] not in [p['polygon_id'] for p in config['problems']]:
            config['problems'].append({
                'name': problem['name'],
                'polygon_id': problem['id']
            })
            new_problems.append(problem['name'])
        config_problem = [p for p in config['problems'] if p['polygon_id'] == problem['id']][0]
        if 'label' not in config_problem:
            config_problem['label'] = label
        if 'color' not in config_problem:
            config_problem['color'] = 'Black'
        for field in ['author', 'preparation']:
            if field not in config_problem:
                config_problem[field] = ''
    
    if len(new_problems) > 0:
        logging.info('Found new problems: {}.'.format(', '.join(new_problems)))
    else:
        logging.info('No new problems were found in the contest.')

# Generates contest_dir/tex/statements.pdf and contest_dir/tex/solutions.pdf.
def generate_statements_solutions(config, contest_dir):
    pdf_generation_params = {
        'contest_name': config['contest_name'],
        'hide_balloon': config.get('hide_balloon', False),
        'hide_tlml': config.get('hide_tlml', False)
    }

    # Sorting problems by label.
    label_and_name = [(p['label'], p['name']) for p in config['problems']]
    label_and_name.sort()
    sorted_names = [p[1] for p in label_and_name]
    
    tex_utilities.generate_statements_pdf(
            sorted_names,
            config.get('front_page_statements'),
            os.path.join(contest_dir, 'tex'),
            pdf_generation_params)

    tex_utilities.generate_solutions_pdf(
            sorted_names,
            config.get('front_page_solutions'),
            os.path.join(contest_dir, 'tex'),
            pdf_generation_params)

    logging.info('Successfully generated \'%s\' and \'%s\'.' %
        (os.path.join(contest_dir, 'tex', 'statements.pdf'),
        os.path.join(contest_dir, 'tex', 'solutions.pdf')))

def load_config_yaml(contest_dir):
    config_yaml = os.path.join(contest_dir, 'config.yaml')

    if not os.path.isfile(config_yaml):
        logging.error('The file %s was not found.' % config_yaml)
        exit(1)
    with open(config_yaml, 'r') as f:
        try:
            config = yaml.safe_load(f)
            logging.debug(config)
            return config
        except yaml.YAMLError as exc: # TODO: Is this except meaningful?
            print(exc)
            exit(1)

# Validation of the structure of config.yaml, enforcing the presence of
# mandatory keys and checking that no unexpected keys are present.
def validate_config_yaml(config):
    if 'contest_name' not in config or 'problems' not in config:
        logging.error('The keys \'contest_name\' and \'problems\' must be present in \'config.yaml\'.')
        exit(1)
    
    top_level_keys = ['contest_name', 'polygon', 'domjudge', 'front_page_statements', 
                      'front_page_solutions', 'problems', 'hide_balloon', 'hide_tlml',
                      'contest_title', 'contest_shortname', 'problem_answer_details',
                      'definitions', 'contest_latex_filename', 'header_logo_filename',
                      'contest_cover_filename', 'contest_date']

    wrong_keys = list(set(config.keys()) - set(top_level_keys))
    if wrong_keys:
        logging.warning(
            'The key \'%s\' is not expected as top-level key in \'config.yaml\'. The expected keys are: %s.' % (wrong_keys[0], ', '.join(top_level_keys)))

    polygon_keys = ['key', 'secret']
    domjudge_keys = ['server', 'username', 'password', 'contest_id']
    if 'polygon' in config and\
            set(config['polygon'].keys()) != set(polygon_keys):
        logging.warning('The subdictionary \'polygon\' of \'config.yaml\' must contain they keys: %s.' % ', '.join(polygon_keys))

    if 'domjudge' in config and\
            set(config['domjudge'].keys()) != set(domjudge_keys):
        logging.warning('The subdictionary \'domjudge\' of \'config.yaml\' must contain they keys: %s.' % ', '.join(domjudge_keys))

    problem_keys = ['name', 'label', 'color', 'author', 'preparation', 'override_time_limit', 'override_memory_limit', 'polygon_id', 'polygon_version', 'domjudge_local_version', 'domjudge_server_version', 'domjudge_id', 'domjudge_externalid']

    for problem in config['problems']:
        if 'name' not in problem:
            logging.error('All problems described in \'config.yaml\' must contain the key \'name\'.')
            exit(1)
        wrong_keys = list(set(problem.keys()) - set(problem_keys))
        if wrong_keys:
            logging.warning('The key \'%s\' in the description of problem \'%s\' in \'config.yaml\' is not expected. The expected keys are: %s.' % (wrong_keys[0], problem['name'], ', '.join(problem_keys)))

def save_config_yaml(config, contest_dir):
    with open(os.path.join(contest_dir, 'config.yaml'), 'w', encoding='utf-8') as f:
        yaml.safe_dump(config, f, default_flow_style=False, sort_keys=False)

# Removes from the contest directory all the data relative to the problem and
# updates accordingly the versioning of the problem.
# The argument problem is the dictionary describing the problem present in
# config.yaml.
def remove_problem_data(problem, contest_dir):
    # Remove the versions from config.yaml.
    problem['polygon_version'] = -1
    problem['domjudge_local_version'] = -1
    problem['domjudge_server_version'] = -1

    # Delete the directories polygon/problem_name, domjudge/problem_name.
    for dir_name in ['polygon', 'domjudge']:
        dir_path = os.path.join(contest_dir, dir_name, problem['name'])
        if os.path.isdir(dir_path):
            shutil.rmtree(dir_path)

    # Delete the files of the problem from tex/.
    for file_name in ['statement', 'statement-content',
                      'solution', 'solution-content']:
        for extension in ['tex', 'aux', 'log', 'out', 'pdf']:
            file_path = os.path.join(contest_dir, 'tex',
                    problem['name'] + '-' + file_name + '.' + extension)
            if os.path.isfile(file_path):
                os.remove(file_path)

    for sample_in in pathlib.Path(contest_dir, 'tex', 'samples').glob(
            problem['name'] + '*.in'):
        sample_in.unlink()

    for sample_out in pathlib.Path(contest_dir, 'tex', 'samples').glob(
            problem['name'] + '-*.out'):
        sample_out.unlink()

    for image in pathlib.Path(contest_dir, 'tex', 'images').glob(
            problem['name'] + '-*'):
        image.unlink()

# Get a color in one of the following formats:
# - #FF11AB (hexadecimal, upper case, with #)
# - #ff11ab (hexadecimal, lower case, with #)
# - PapayaWhip (HTML color https://htmlcolorcodes.com/color-names/, camel case)
# - papayawhip (HTML color https://htmlcolorcodes.com/color-names/, lower case)
# and converts it to its standard 6-digit hexadecimal representation (e.g., FF11AB).
def convert_to_hex(color):
    error_message = 'The color \'%s\' specified in config.yaml is not a valid html color (see https://htmlcolorcodes.com/color-names/) or a valid hexadecimal color (e.g., #ABC123). ' % color
    
    if color[0] == '#':
        color = color[1:]
        if not re.fullmatch(r'[A-Fa-f0-9]{6}', color):
            logging.error(error_message)
            exit(1)
    else:
        try:
            color = webcolors.name_to_hex(color)[1:]
        except ValueError:
            logging.error(error_message)
            exit(1)
    
    return color.upper()

# Returns a tqdm object that wraps the iterable.
# The object is itself an iterable that yields the same values,
# and additionally displays a progress bar which is updated after each iteration.
def wrap_iterable_in_tqdm(iterable, total, unit_scale=False, desc=None):
    return tqdm(
        iterable,
        total=total,
        ncols=100,
        desc=desc,
        leave=False,
        file=sys.stdout,
        colour='cyan',
        unit='kB',
        unit_scale=unit_scale,
        bar_format='{l_bar}{bar}| \033[33m[ETA: {remaining}, {rate:6.2f} {unit}/s]\033[0m',
        delay=3
    )
