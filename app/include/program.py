from option import option


class program(object):
    """"""

    def __init__(self, prog_name):
        self.name = prog_name
        self.options = list()
        self.descpt = ''

    def __str__(self):
        return self.name

    def set_descpt(self, descpt):
        """set program description"""
        self.descpt = descpt

    def add_option(self, charOpt, strOpt, descript, usr_input=''):
        """add an input option for the program"""
        if not (type(charOpt) is str
                and type(strOpt) is str
                and type(descript) is str
                and type(usr_input) is str):
            print 'ERROR: inputs must be of 4 strings'
            return
        opt = option(charOpt, strOpt, descript, usr_input)
        self.options.append(opt)

    def get_option(self, search_param):
        """search for all available options using char/str opt"""
        for opt in self.options:
            if search_param is opt.charOpt or search_param is opt.stringOpt:
                return opt
        return None

    def help_msg(self):
        """"""
        synopsis = '\t%s' % (self.name)
        if len(self.options) > 0:
            synopsis = synopsis + (' [OPTIONS]')
        print 'NAME'
        print synopsis
        print ''
        print 'DESCRIPTION'
        print '\t%s\n' % (self.descpt)
        print 'OPTIONS\n'
        for opt in self.options:
            print '\t%s' % (opt.help_msg())
            print ''
