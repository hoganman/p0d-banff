"""The information associated with a THStack"""
import ROOTH1


class ROOTHStack(ROOTH1.ROOTH1):
    """store the save name, plot var and binning, and axes labels"""

    def __init__(self, plot_var='', n_bins=0, low_val=0, high_val=-1,
                 x_title='', y_title=''):
        super(ROOTHStack, self).__init__(plot_var, n_bins, low_val, high_val, x_title, y_title)
        self.name = 'ROOTHStack'
        self.HStack = None

    def init_from_TH1D(self, plot_var, th1d_obj,
                       x_title, y_title):
        """a similar method to the constructor, uses the TH1D instead"""
        super(ROOTHStack, self).__init__(plot_var, th1d_obj, x_title, y_title)
        self.HStack = None

    def setHStack(self, HStack):
        """set the pointer"""
        self.HStack = HStack
        self.name = HStack.GetName()
