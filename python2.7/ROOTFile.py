"""a class to check the status of a ROOT file before trying to open it"""
from File import File
from ROOT import TFile
import os


class ROOTFile(File):
    """Basically to check if a file exists"""

    def __init__(self, input_file_name=''):
        super(ROOTFile, self).__init__(input_file_name)
        self.status = self.checkfile()

    def valid(self):
        """A bool equivalent of checkfile"""
        return self.status == self.IS_GOOD

    def checkfile(self):
        """Checks if the file is bad (0) or good (1)"""
        if not self.exists():
            return self.IS_BAD
        if not self.is_root_file():
            return self.IS_BAD
        infile = None
        tmp_status = self.INDETERMINATE
        try:
            infile = TFile.Open(self.get_file_name())
        except IOError:
            tmp_status = self.IS_BAD
            infile = None
        if not infile or not infile.IsOpen():
            tmp_status = self.IS_BAD
        elif infile.IsRaw():
            tmp_status = self.IS_BAD
        elif not infile.IsBinary():
            tmp_status = self.IS_BAD
        elif not infile.GetListOfKeys().GetSize():
            tmp_status = self.IS_BAD
        elif infile.IsZombie():
            tmp_status = self.IS_BAD
        elif infile.TestBit(TFile.kRecovered):
            tmp_status = self.IS_BAD
        else:
            tmp_status = self.IS_GOOD
        if infile:
            infile.Close()
        return tmp_status


def CheckAllInDirectory(indirectory):
    """For a given directory, check each ROOT TFile"""
    if not os.path.isdir(indirectory):
        print 'ERROR: ', indirectory, 'is NOT a valid directory!'
        return False
    list_dir = os.listdir(indirectory)
    ret_status = True
    bad_files = list()
    for a_file_name in list_dir:
        if '.root' not in a_file_name:
            continue
        full_name = os.path.join(indirectory, a_file_name)
        test_File = ROOTFile(full_name)
        if not test_File.valid():
            if ret_status:
                print 'ALERT: a file is NOT valid!'
            ret_status = False
            bad_files.append(a_file_name)
    if len(bad_files) > 0:
        print 'The following files are BAD:'
        for bad_file in sorted(bad_files):
            print '....', bad_file
    return ret_status
