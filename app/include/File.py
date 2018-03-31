"""A template abstract different machine files"""
import p0dbanff
from os.path import isfile
from os.path import isdir


class File(p0dbanff.p0dbanff):
    """a general class for a file"""

    INDETERMINATE = -1
    IS_BAD = 0
    IS_GOOD = 1

    def __init__(self, input_file_name=''):
        super(File, self).__init__()
        self.file_name = input_file_name

    def __str__(self):
        return 'File %r' % (self.file_name)

    def get_file_name(self):
        """returns the file name"""
        return self.file_name

    def exists(self, file_name=''):
        """checks if the file in question exists"""
        result = 0
        if len(file_name) == 0:
            file_name = self.get_file_name()
        result = int(isfile(self.get_file_name()))
        if result != 1:
            print 'ERROR! File "%s" does not exist!' % (file_name)
        return result

    def is_file(self, file_name=''):
        """test if a file"""
        if len(file_name) == 0:
            return int(isfile(self.get_file_name()))
        else:
            return int(isfile(file_name))

    def is_dir(self, directory_name):
        """checks if is in fact a directory"""
        if len(directory_name) == 0:
            return int(isdir(self.get_file_name()))
        else:
            return int(isdir(directory_name))

    def contains(self, search_text):
        """searches for a string in the file name"""
        return self.get_file_name().find(search_text) != -1

    def is_extension(self, ext):
        """searches if the extension is ext"""
        return self.contains(ext)

    def is_root_file(self):
        """checks if is ROOT .root file"""
        return self.is_extension('.root')

    def is_text_file(self):
        """checks if is txt file"""
        return self.is_extension('.txt')
