import program


class macro(program.program):
    """"""

    def __init__(self, prog_name, macro_name):
        super(macro, self).__init__(prog_name)
        self.macro_name = macro_name

    def __str__(self):
        full_str = [self.execut, self.macro_name, self.option_string()]
        return ' '.join(full_str)
