"""A class to check the read or write text files"""

import File


class TextFile(File.File):
    """The object that holds basic text file info """

    IS_OPEN = 2
    IS_CLOSED = 3

    def __init__(self, input_file_name=''):
        super(TextFile, self).__init__(input_file_name)
        self.file_name = input_file_name
        self.file = None

    def __str__(self):
        return 'Text file %r' % (self.file_name)

    def open(self, option='r'):
        """opens the file"""
        status = self.INDETERMINATE
        if not self.exists():
            print 'ERROR: file %s does not exist' % (self.get_file_name())
        elif self.file:
            # print 'File already open, nothing happens...'
            status = self.IS_OPEN
        elif self.file is None:
            try:
                self.file = open(self.get_file_name(), option)
                status = self.IS_OPEN
            except IOError:
                print 'The file could not be openned'
        else:
            print 'The file could not be openned'
        return status

    def close(self):
        """closes the file"""
        status = self.INDETERMINATE
        if self.file is None:
            print 'ERROR: Cannot close file since it is not set'
        else:
            try:
                self.file.close()
                self.file = None
                status = self.IS_CLOSED
            except IOError:
                print 'The file is not open, there is nothing to close'
        return status


class WriteTextFile(TextFile):
    """The object that writes a text file"""

    def __init__(self, input_file_name, input_option='w'):
        super(WriteTextFile, self).__init__(input_file_name)
        self.option = input_option

    def __str__(self):
        return 'Writable text file %r with format %r' % (
            self.file_name, self.option)

    def write(self, input_text):
        """writes text to file"""
        if self.file is None:
            print "ERROR: The file not openned yet. Please open it first"
        else:
            try:
                self.file.write(input_text)
            except IOError:
                print 'ERROR: The file is not writtable'


class ReadTextFile(TextFile):
    """The object that reads text file"""

    def __init__(self, input_file_name):
        super(ReadTextFile, self).__init__(input_file_name)
        self.file_name = input_file_name

    def __str__(self):
        return 'Readable text file %r' % (self.file_name)

    def get_file_as_list(self):
        """returns the file contents as a list"""
        retlist = None
        status = self.open()
        if status is self.IS_OPEN:
            retlist = self.file.read().split('\n')
            while '' in retlist:
                del retlist[retlist.index('')]
        else:
            print 'ERROR: Could not get list'
        self.close()
        return retlist
