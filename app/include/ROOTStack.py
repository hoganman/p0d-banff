"""The information associated with a THStack"""

# from ROOT import THStack


class ROOTHStack(object):
    """store the save name, plot var and binning, and axes labels"""

    def __init__(self, plot_var='', n_bins=0, low_val=0, high_val=-1,
                 x_title='', y_title=''):
        self.name = 'ROOTHStack'
        self.plot_var = plot_var
        self.n_bins = n_bins
        self.low_X = low_val
        self.high_X = high_val
        self.x_title = x_title
        self.y_title = y_title
        self.minimum = -1
        self.maximum = -1
        self.log_y = False
        self.HStack = None

    def __str__(self):
        return self.name

    def set_log_y(self, log_y=True):
        """should log_y also be shown"""
        self.log_y = log_y

    def set_min(self, minimum=1):
        """define the minimum for the stack"""
        self.minimum = minimum

    def set_max(self, maximum):
        """define the maximum for the stack"""
        self.maximum = maximum

    def set(self, HStack):
        """set the pointer"""
        self.HStack = HStack
        self.name = HStack.GetName()
