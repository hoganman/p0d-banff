"""The information associated with a TH1"""
import p0dbanff


class ROOTH1(p0dbanff.p0dbanff):
    """store the save name, plot var and binning, and axes labels"""

    def __init__(self, plot_var='', n_bins=0, low_val=0, high_val=-1,
                 x_title='', y_title=''):
        super(ROOTH1, self).__init__()
        self.name = 'ROOTH1'
        self.plot_var = plot_var
        self.n_bins = n_bins
        self.low_X = low_val
        self.high_X = high_val
        self.x_title = x_title
        self.y_title = y_title
        self.minimum = -1
        self.maximum = -1
        self.log_y = False
        self.th1d = None
        self.use_TH1D = False

    def init_from_TH1D(self, plot_var, th1d_obj,
                       x_title, y_title):
        """a similar method to the constructor, uses the TH1D instead"""
        self.name = 'ROOTH1'
        self.plot_var = plot_var
        self.n_bins = th1d_obj.GetNbinsX()
        self.low_X = th1d_obj.GetXaxis().GetBinLowEdge(1)
        self.high_X = th1d_obj.GetXaxis().GetBinLowEdge(self.n_bins)
        self.x_title = str(x_title)
        self.y_title = str(y_title)
        self.minimum = -1
        self.maximum = -1
        self.HStack = None
        self.th1d = th1d_obj
        self.use_TH1D = bool(self.th1d)

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

    def set(self, H1):
        """set the pointer"""
        self.H1 = H1
        self.name = H1.GetName()
