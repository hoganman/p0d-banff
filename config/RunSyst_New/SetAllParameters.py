"""This small script acts like sed -i for all input files
input 1 through N are all the xml files you wish to alter
The parameterChanges dictionary defines what you want to have
ie if you do NOT want to plot data, have 'PLOTDATA': '\"0\"'
"""

import sys
import os

parameterChanges = {
        'PLOTDATA': '\"0\"',
        'DRAW_ENU': '\"1\"',
        'DRAW_PMU': '\"1\"',
        'DRAW_COSTHETAMU': '\"1\"',
        'DRAW_THETAMU': '\"0\"',
        'DRAW_Q2': '\"0\"',
        'DRAW_W2': '\"0\"',
        'DRAW_BJORKEN_X': '\"0\"',
        'DRAW_BJORKEN_Y': '\"0\"',
        'DRAW_ENERGY_TRANSFER_NU': '\"0\"',
        'DRAW_P0DFIT_PMU_COSTHETAMU': '\"0\"',
        'DRAW_P0DCOVFHC_PMU_COSTHETAMU': '\"0\"',
        'SHOW_LOGZ': '\"0\"',
        'APPLY_FLUX_WEIGHTS': '\"0\"',
        'APPLY_EVENT_WEIGHTS': '\"1\"',
        'PLOTTOPOLOGY': '\"1\"',
        'PLOTNEUTNUREACTIONCODES': '\"1\"',
        'PLOTNEUTANTINUREACTIONCODES': '\"1\"',
        'PLOTNEUTCCQELIKEREACTIONCODES': '\"1\"'
        }


argv = sys.argv[1:]
if not os.path.isdir('bkup'):
    os.mkdir('bkup')
for aFile in argv:
    openedFile = open(aFile)
    old_contents = openedFile.read().split('\n')
    new_contents = list(old_contents)
    openedFile.close()
    os.system('mv {} bkup/'.format(aFile))
    lineNumber = 0
    while lineNumber < len(new_contents):
        line = new_contents[lineNumber]
        for parameter, new_value in parameterChanges.iteritems():
            if parameter not in line:
                continue
            lineNumber = lineNumber + 1
            old_value = '\"{}\"'.format(str(abs(int(new_value.strip('"'))-1)))
            if new_value in new_contents[lineNumber]:
                lineNumber = lineNumber + 1
                break
            new_contents[lineNumber] = new_contents[lineNumber].replace(old_value, new_value)
            lineNumber = lineNumber + 1
            break
        lineNumber = lineNumber + 1
    replacement = open(aFile, 'w')
    for aLine in new_contents:
        replacement.write('{}\n'.format(aLine))
    replacement.close()
