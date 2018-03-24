"""A template abstract different machine directory"""

import os


class Directory(object):
    """a general class for a directory"""

    def __init__(self, directory_name=''):
        self.directory_name = directory_name

    def __str__(self):
        return 'Directory: %s' % (self.get())

    def get(self):
        """returns the directory path"""
        return self.directory_name

    def exists(self, directory_name=''):
        """checks if the directory in question exists"""
        if len(directory_name) == 0:
            return int(os.path.isdir(self.get()))
        else:
            return int(os.path.isdir(directory_name))

    def cd(self, directory_name=''):
        """change to the directory"""
        if len(directory_name) == 0:
            return int(os.chdir(self.get()))
        else:
            return int(os.chdir(directory_name))

    def mkdir(self, directory_name=''):
        """make directory"""
        if len(directory_name) == 0:
            return int(os.makedirs(self.get()))
        else:
            return int(os.makedirs(directory_name))
