"""A general purpose tool to extract text file info"""


class TextFile(object):
    """The object that holds basic file info """

    def __init__(self, input_file_name):
        self.file_name = input_file_name
        self.file = None

    def __str__(self):
        return 'Text file %r' % (self.file_name)

    def get_file_name(self):
        """returns the file name"""
        return self.file_name

    def open(self, option='r'):
        """opens the file"""
        self.file = open(self.get_file_name(), option)

    def close(self):
        """closes the file"""
        try:
            self.file.close()
        except IOError:
            print 'The file is not open, there is nothing to close'


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
        if self.file is None:
            print "ERROR: The file not openned yet. Please open it first"
        else:
            try:
                retlist = self.file.read().split('\n')
                self.close()
            except IOError:
                print 'ERROR: The file is not openable'
        return retlist
