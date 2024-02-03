import csv
from os import read
import sys
from io import StringIO
import string
import random


OFFICIAL_PARTICIPANTS_CATEGORY_ID = 3
UNOFFICIAL_PARTICIPANTS_CATEGORY_ID = 4
START_ID = 5

def read_team_csv(data):
    reader = csv.DictReader(StringIO(data))
    teams = []
    for row in reader:
        teams.append(row)
    return teams


def is_official(team):
    return team['Unofficial (0 - cancelled, 1 unofficial, 2 ok)'] == '2'

def write_team_tsv(teams, start_id):
    out = ['File_Version\t2']
    for team in teams:
        # the (integer) team ID. Must be unique
        # an external ID, e.g. from the ICPC CMS, may be empty
        # the category ID this team belongs to
        # the team name as used in the web interface
        # the institution name as used on the scoreboard
        # the institution short name as used in the jury interface and certain exports
        # a country code in form of ISO 3166-1 alpha-3
        # an external institution ID, e.g. from the ICPC CMS, may be empty
        official = is_official(team)
        codemap = {'AU': 'AUS', 'NZ': 'NZL'}
        # This is necesary or domjudge complains.
        shortmap = {
            'The University of Adelaide': 'UADE',
            'The University of Auckland': 'UAUC',
            'Monash University': 'UMON',
            'Swinburne University of Technology': 'USWI',
            'University of Melbourne': 'UMEL',
            'University of Canterbury': 'UCAN',
            'The University of Queensland': 'UQUE',
            'The Australian National University': 'UANU',
            'The University of New South Wales': 'UNSW',
            'The University of Sydney': 'USYD',
            'Macquarie University': 'UMAC',
            'The University of Western Australia': 'UWA',
            'The University of Waikato': 'UWAI',
            'Victoria University of Wellington': 'UVIW'
        }
        res = '%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s' % (
            str(start_id),
            '',
            str(OFFICIAL_PARTICIPANTS_CATEGORY_ID if official else UNOFFICIAL_PARTICIPANTS_CATEGORY_ID),
            team['team'],
            team['institution'],
            shortmap[team['institution']],
            codemap[team['country']],
            '',
        )
        team['teamid'] = start_id
        out.append(res)
        start_id += 1

    return '\n'.join(out)

def write_account_tsv(teams):
    out = ['accounts\t1']
    # start_id corresponds to the team id. It should be the same as called from
    # write_team_tsv.  domjudge automatically maps user e.g. team5 to team with id 5.
    for team in teams:
        # the user type, one of team or judge
        # the full name of the user
        # the username
        # the password
        team['username'] = 'team%03d' % team['teamid']
        team['password'] = ''.join(random.choices(string.ascii_letters + string.digits, k=6))
        res = '%s\t%s\t%s\t%s' % (
            'team',
            team['team'],
            team['username'],
            team['password'],
        )
        out.append(res)
    return '\n'.join(out)

def write_team_summary_csv(teams, f):
    writer = csv.writer(f)
    writer.writerow(['name', 'username','password','affiliation', 'is official?'])
    for team in teams:
        writer.writerow([team['team'], team['username'], team['password'],
        team['institution'], 'yes' if is_official(team) else 'no'])


data = open(sys.argv[1]).read()
teams = read_team_csv(data)

with open('teams.tsv', 'w') as f:
    f.write(write_team_tsv(teams, START_ID))
with open('accounts.tsv', 'w') as f:
    f.write(write_account_tsv(teams))
with open('team_summary.csv', 'w') as f:
    write_team_summary_csv(teams, f)
