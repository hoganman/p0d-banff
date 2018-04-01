from os import getenv
from os.path import join


class p0dbanff(object):
    """The basic object for this project"""

    def __init__(self):
        self.root = getenv('P0DBANFFROOT')
        self.path = getenv('PATH')
        self.ldpath = getenv('LD_LIBRARY_PATH')
        self.pythonpath = getenv('PYTHONPATH')
        self.source_script = join(self.root, 'Setup-P0DBANFF.sh')
