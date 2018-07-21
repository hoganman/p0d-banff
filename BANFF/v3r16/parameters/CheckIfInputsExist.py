#!/usr/bin/env python
"""Program checks the parameter file and checks if the inputs or
outputs exist"""

from sys import argv
from CheckFilePython import checkfile as CheckROOTFile
from TextFile import ReadTextFile
from File import File


def get_input_files(input_file_name):
    """returns dict of input files"""
    input_files = {}
    banff_parameter_file = ReadTextFile(input_file_name)
    banff_parameter_file.open()
    parameter_contents = banff_parameter_file.get_file_as_list()
    # banff_parameter_file.close()
    for line in parameter_contents:
        if len(line) == 0 or line.find('Input') == -1:
            continue
        banff_index = line.index('BANFF')
        equals_index = line.index('=')
        right_angle_index = line.index('>')
        parameter_key = line[banff_index:equals_index].strip()
        parameter_value = line[equals_index+1:right_angle_index].strip()
        input_files[parameter_key] = parameter_value
    return input_files


def main(argv):
    """Runs the main program"""
    input_files = get_input_files('BANFF.parameters_Data.dat')
    print '------------------------------------------------------------'
    print '   Input                 Filename                   Exists  '
    print '------------------------------------------------------------'
    for inputkey, file_name in sorted(input_files.iteritems()):
        file_checker = File(file_name)
        file_status = file_checker.INDETERMINATE
        if file_checker.is_root_file():
            file_status = CheckROOTFile(file_name)
        elif file_checker.is_text_file():
            file_status = file_checker.exists()
        else:
            file_status = 0
        print '%s   %s   %d ' % (inputkey, file_name, file_status == 1)


if __name__ == "__main__":
    main(argv[1:])
