"""A template abstract different machine directory"""
import p0dbanff
import os


class Directory(p0dbanff.p0dbanff):
    """a general class for a directory"""

    def __init__(self, directory_name=''):
        super(Directory, self).__init__()
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
        if len(directory_name) <= 0:
            directory_name = self.get()
        if not self.exists(directory_name):
            print 'ERROR: unable to cd to ' + directory_name
        try:
            os.chdir(directory_name)
        except os.error as err:
            print 'ERROR: ' + str(err)
        return

    def mkdir(self, directory_name=''):
        """make directory"""
        if len(directory_name) <= 0:
            directory_name = self.get()
        try:
            os.makedirs(directory_name)
        except os.error as err:
            print 'ERROR: ' + str(err)
        return
