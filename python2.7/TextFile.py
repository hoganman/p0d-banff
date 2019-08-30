"""A class to check the read or write text files"""
import File


class TextFile(File.File):
    """The object that holds basic text file info """

    IS_OPEN = 2
    IS_NOT_OPEN = 3
    IS_CLOSED = 4

    def __init__(self, input_file_name='', open_option='r'):
        super(TextFile, self).__init__(input_file_name)
        self.file_name = input_file_name
        self.open_option = open_option
        self.open_status = self.IS_NOT_OPEN
        self.file = None
        self.open()

    def __str__(self):
        return 'Text file %r' % self.file_name

    def __del__(self):
        if self.open_status == self.IS_OPEN:
            self.close()

    def open(self):
        """opens the file"""
        self.open_status = self.INDETERMINATE
        if self.file:
            # print 'File already open, nothing happens...'
            self.open_status = self.IS_OPEN
        elif self.file is None:
            try:
                self.file = open(self.get_file_name(), self.open_option)
                self.open_status = self.IS_OPEN
            except IOError:
                print 'The file could not be opened'
                self.open_status = self.IS_NOT_OPEN
        else:
            print 'The file could not be opened'
        return self.open_status

    def close(self):
        """closes the file"""
        self.open_status = self.INDETERMINATE
        if self.file is None:
            print 'ERROR: Cannot close file since it is not set'
            self.open_status = self.INDETERMINATE
        else:
            try:
                self.file.close()
                self.file = None
                self.open_status = self.IS_CLOSED
            except IOError:
                print 'The file is not open, there is nothing to close'
                self.open_status = self.INDETERMINATE
        return self.open_status


class WriteTextFile(TextFile):
    """The object that writes a text file"""

    def __init__(self, input_file_name):
        super(WriteTextFile, self).__init__(input_file_name, 'w')

    def __str__(self):
        return 'Writable text file %r with format %r' % (
            self.file_name, self.open_option)

    def write(self, input_text):
        """writes text to file"""
        if self.file is None:
            print "ERROR: The file not opened yet. Please open it first"
        else:
            if type(input_text) is list:
                input_text = '\n'.join(input_text)
            try:
                self.file.write(input_text)
            except IOError:
                print 'IOError: The file is not writable'


class ReadTextFile(TextFile):
    """The object that reads text file"""

    def __init__(self, input_file_name):
        super(ReadTextFile, self).__init__(input_file_name)
        self.file_name = input_file_name

    def __str__(self):
        return 'Readable text file %r' % self.file_name

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
