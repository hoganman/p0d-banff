"""A template abstract different machine directory"""
import p0dbanff
import os


class Directory(p0dbanff.p0dbanff):
    """a general class for a directory"""

    def __init__(self, directory_name):
        super(Directory, self).__init__()
        self.directory_name = directory_name

    def __str__(self):
        return 'Directory: %s' % (self.get())

    def get(self):
        """returns the directory path"""
        return self.directory_name

    def exists(self):
        """checks if the directory in question exists"""
        return int(os.path.isdir(self.get()))

    def cd(self):
        """change to the directory"""
        if not self.exists():
            print 'ERROR: unable to cd to ' + self.get()
        try:
            os.chdir(self.get())
        except os.error as err:
            print 'ERROR: ' + str(err)
        return

    def mkdir(self):
        """make directory"""
        try:
            os.mkdir(self.get())
        except os.error as err:
            print 'ERROR: ' + str(err)
            print 'Could not create directory %s' % self.get()
        return
