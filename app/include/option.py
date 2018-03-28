class option(object):
    """a generic option to a program using getopt format"""

    def __init__(self, charOpt='', stringOpt='', descpt='', usr_input=''):
        self.charOpt = charOpt
        self.stringOpt = stringOpt
        self.descpt = descpt
        self.usr_input = usr_input

    def get_char_opt(self):
        """get'er for character option"""
        return self.charOpt

    def get_string_opt(self):
        """get'er for string option"""
        return self.stringOpt

    def get_descpt(self):
        """get'er for option description"""
        return self.descpt

    def get_input(self, usr_input):
        """get'er for user input"""
        return self.usr_input

    def help_msg(self):
        """get'er for how option works"""
        return '-%s, --%s\t%s' % (self.charOpt, self.stringOpt, self.descpt)

    def set_descpt(self, descpt):
        """set'er for option description"""
        self.descpt = descpt

    def set_char_opt(self, charOpt):
        """set'er for character option"""
        self.charOpt = charOpt

    def set_strong_opt(self, strongOp):
        """set'er for string option"""
        self.strongOp = strongOp

    def set_input(self, usr_input):
        """set'er for user input"""
        self.usr_input = usr_input

    def is_set(self):
        """checks if the user has set input to this option"""
        return len(self.usr_input) > 0
