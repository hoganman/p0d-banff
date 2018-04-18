"""set the conventions for using this software"""
import p0dbanff


class RunName(p0dbanff.p0dbanff):
    """store upper and lower case spellings of runs"""

    def __init__(self, lowercase, uppercase):
        super(RunName, self).__init__()
        self.lower = lowercase
        self.upper = uppercase

    def upper_case(self):
        """get upper case seplling"""
        return self.upper

    def low(self):
        """get lower case seplling"""
        return self.lower


# MC version
RUN2W = RunName('run2-water', 'Run2_Water')
RUN2A = RunName('run2-air', 'Run2_Air')
RUN3B = RunName('run3b-air', 'Run3b_Air')
RUN3C = RunName('run3c-air', 'Run3c_Air')
RUN4W = RunName('run4-water', 'Run4_Water')
RUN4A = RunName('run4-air', 'Run4_Air')
RUN5C = RunName('run5c-water', 'Run5c_Water')
RUN6B = RunName('run6b-air', 'Run6b_Air')
RUN6C = RunName('run6c-air', 'Run6c_Air')
RUN6D = RunName('run6d-air', 'Run6d_Air')
RUN6E = RunName('run6e-air', 'Run6e_Air')
RUN7B = RunName('run7b-water', 'Run7b_Water')

# Data version
RUN2WDATA = RunName('run2-water', 'Run2_Water_data')
RUN2ADATA = RunName('run2-air', 'Run2_Air_data')
RUN3BDATA = RunName('run3b-air', 'Run3b_Air_data')
RUN3CDATA = RunName('run3c-air', 'Run3c_Air_data')
RUN4WDATA = RunName('run4-water', 'Run4_Water_data')
RUN4ADATA = RunName('run4-air', 'Run4_Air_data')
RUN5CDATA = RunName('run5c-water', 'Run5c_Water_data')
RUN6BDATA = RunName('run6b-air', 'Run6b_Air_data')
RUN6CDATA = RunName('run6c-air', 'Run6c_Air_data')
RUN6DDATA = RunName('run6d-air', 'Run6d_Air_data')
RUN6EDATA = RunName('run6e-air', 'Run6e_Air_data')
RUN7BDATA = RunName('run7b-water', 'Run7b_Water_data')
