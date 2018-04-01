""" A way to run simple commands  """
import p0dbanff


class Command(p0dbanff.p0dbanff):
    """a command class"""

    def __init__(self, command):
        super(Command, self).__init__()
        self.command = command

    def __str__(self):
        return self.get()

    def get(self):
        """returns command"""
        return self.command

    def add(self, addition, spacer=' '):
        """adds more to the command"""
        self.command = self.command + spacer + addition

    def run_test(self):
        """simulates running the command"""
        print self.command
