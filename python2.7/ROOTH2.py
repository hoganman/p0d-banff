"""The information associated with a TH2"""
import p0dbanff


class ROOTH2(p0dbanff.p0dbanff):
    """"""

    def __init__(self, varX='', varY='', nbinsX=0, lowX=0, highX=-1,
                 nbinsY=0, lowY=0, highY=0, x_title='', y_title=''):
        super(ROOTH2, self).__init__()
        self.name = 'ROOTH2'
        self.varX = varX
        self.varY = varY
        self.nbinsX = nbinsX
        self.lowX = lowX
        self.highX = highX
        self.nbinsY = nbinsY
        self.lowY = lowY
        self.highY = highY
        self.x_title = x_title
        self.y_title = y_title
        self.z_title = ''
        self.minimum = -1
        self.maximum = -1
        self.th2 = None
        self.use_TH2 = False
