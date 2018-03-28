from os import popen
from Command import Command


class ShellCommand(Command):
    """a shell command class"""

    IN_BKG = True

    def __init__(self, command):
        super(ShellCommand, self).__init__(command)
        self.output = ''

    def run_test(self, background=(not IN_BKG)):
        """simulates running the command"""
        if self.command is None or len(self.command) == 0:
            print 'ERROR: There is no command to be run'
            return
        if not (type(background) is bool):
            print 'ERROR: Only able to accept bools to \
run in bkg or not'
            return
        cmd = Command(self.command)
        if background:
            cmd.add('&')
        print cmd.get()

    def run(self, background=(not IN_BKG)):
        """"Executes the command"""
        if self.command is None or len(self.command) == 0:
            print 'ERROR: There is no command to be run'
            return
        if not (type(background) is bool):
            print 'ERROR: Only able to accept bools to \
run in bkg or not'
            return
        cmd = Command(self.command)
        if background:
            cmd.add('&')
        print cmd.get()
        self.output = popen(cmd.get()).read()

    def output(self):
        """gets the output of the command"""
        return self.output
