import p0dbanff
from ROOT import TChain
from ROOTFile import ROOTFile


class ROOTChain(p0dbanff.p0dbanff):
    """To add files to a TChain commonly used in this analysis"""

    def __init__(self, tree_name):
        super(ROOTChain, self).__init__()
        self.tree_name = tree_name
        self.chain = TChain(self.tree_name, '')

    def __str__(self):
        return self.tree_name

    def get(self):
        """return the TChain itself"""
        return self.chain

    def add_from_file(self, file_name):
        """Add one file to the TChain"""
        if ROOTFile.exists(file_name):
            self.chain.Add(file_name)
        else:
            print 'Unable to add file named "%s"' % (file_name)

    def add(self, file_name):
        """Overloaded method to add a single file"""
        self.add_from_file(file_name)

    def add_from_files(self, file_names, first_num=1, last_num=100):
        """Add files which have a single %d in it"""
        # first check if file_names has a %d in it
        if file_names.find('%d') == -1:
            self.add_from_file(file_names)
        else:
            for file_num in range(first_num, last_num+1):
                file_name = file_names % (file_num)
                self.add_from_file(file_name)


def get(chain_name, file_names, first_num=1, last_num=100):
    """a dirty method to get a TChain from many numbered files"""
    chain = ROOTChain(chain_name)
    chain.add_from_files(file_names, first_num, last_num)
    return chain.get()
