"""doc string"""
from ROOT import TFile, TIter, TKey, TAxis
import sys


def main():
    """main"""
    if len(sys.argv) == 1:
        print "ERROR: Missing input obs norm file"
        print usage()
        sys.exit(1)
    if len(sys.argv) > 2:
        print "ERROR: Too many inputs"
        print usage()
        sys.exit(1)
    input_file_name = sys.argv[1]
    obsnorm_file = TFile(input_file_name)
    if not obsnorm_file.IsOpen():
        print "ERROR: file %s does not exist" % (input_file_name)
        print usage()
        sys.exit(1)
    nkeys = obsnorm_file.GetListOfKeys().GetSize()
    key = TKey()
    key_iter = TIter(obsnorm_file.GetListOfKeys())
    key = key_iter()
    bin_sum = int(0)
    for i in range(0, nkeys):
        obj_name = key.GetName()
        # print obj_name
        if obj_name.find("axis") != -1:
            axis = TAxis(key.ReadObj())
            bin_sum += axis.GetNbins()
            print obj_name
            print int(axis.GetNbins())
        key = key_iter()

    # print "The total number of bins = "


def usage():
    """error exception print statement"""
    return "usage: CountNumberOfAxisBins.py ObsNorm.root\n"


if __name__ == "__main__":
    main()
