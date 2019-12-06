import math
import numpy as np
import ROOT
import sys
from scipy.stats import poisson


class val_with_error(object):
    """"""

    def __init__(self, init=0, err=0):
        self.mean = init
        self.err = err

    def __add__(self, other):
        mean = self.mean + other.mean
        err = math.sqrt(self.err*self.err + other.err*other.err)
        return val_with_error(mean, err)

    def __str__(self):
        return '%f +/- %f' % (self.mean, self.err)


class func1D_with_err(object):
    """"""

    def __init__(self, name, func_mean, func_err):
        self.func_mean = ROOT.TF1('%s_mean' % name, func_mean)
        self.func_err = ROOT.TF1('%s_err' % name, func_err)

    def eval(self, x):
        """"""
        mean = self.func_mean.Eval(x)
        err = self.func_err.Eval(x)
        return val_with_error(mean, err)


class func2D_with_err(object):
    """"""

    def __init__(self, name, func_mean, func_err):
        self.func_mean = ROOT.TF2('%s_mean' % name, func_mean)
        self.func_err = ROOT.TF2('%s_err' % name, func_err)

    def eval(self, x, y):
        """"""
        mean = self.func_mean.Eval(x, y)
        err = self.func_err.Eval(x, y)
        return val_with_error(mean, err)


def LoopOverHist(hist, Enu_cut=9999):
    """"""
    val_err = val_with_error()
    for bin in range(1, hist.GetNbinsX()+2):
        if hist.GetBinLowEdge(bin) >= Enu_cut:
            continue
        content = hist.GetBinContent(bin)
        err = hist.GetBinError(bin)
        new_val_err = val_with_error(content, err)
        val_err = val_err + new_val_err
    return val_err


def ProbCPC(Nmu_Nmubar_ratio, Ne_noCP, Neb_noCP, **kargs):
    n_steps = 100
    epsilon = 0.0001
    inf = 1000
    Assym = 0.293642
    if 'n_steps' in kargs.keys():
        n_steps = kargs['n_steps']
    if 'epsilon' in kargs.keys():
        epsilon = kargs['epsilon']
    if 'inf' in kargs.keys():
        inf = kargs['inf']
    if 'Assym' in kargs.keys():
        Assym = kargs['Assym']
    prob_cpc = 0.0
    Ne_pdf = poisson(Ne_noCP)
    Neb_pdf = poisson(Neb_noCP)

    for Ne in range(0, inf):
        non_zero_terms = 0
        for Neb in range(0, inf):
            Neb_prime = Nmu_Nmubar_ratio * Neb
            step_arg = Ne - Neb_prime - Assym * (Ne + Neb_prime)
            if step_arg < 0:
                continue
            prob = Ne_pdf.pmf(Ne)
            if prob <= 0:
                continue
            prob *= Neb_pdf.pmf(Neb)
            if prob <= 0:
                continue
            old_term = prob_cpc
            prob_cpc = old_term + prob
            non_zero_terms += 1
            if non_zero_terms >= n_steps:
                if (prob_cpc - old_term) / old_term < epsilon:
                    break
    return prob_cpc


def ProbCPCNH(Nmu_Nmub_ratio, **kargs):
    """Normal hierarchy"""
    NeNoCPNH = 61.4
    NebNoCPNH = 9.0
    return ProbCPC(Nmu_Nmub_ratio, NeNoCPNH, NebNoCPNH, **kargs)


def ProbCPCIH(Nmu_Nmub_ratio, **kargs):
    """Inverted Hierarchy"""
    NeNoCPIH = 54.4
    NebNoCPIH = 9.8
    return ProbCPC(Nmu_Nmub_ratio, NeNoCPIH, NebNoCPIH, **kargs)


def GetProbCPCHist(errorband_file, **kargs):
    """"""

    name = ''
    Enu_cut = 9999
    FHC_POT = 14.7E20
    RHC_POT = 7.6E20
    if 'Enu_cut' in kargs.keys():
        Enu_cut = kargs['Enu_cut']
    if 'name' in kargs.keys():
        name = kargs['name']

    pred_file = ROOT.TFile(errorband_file)

    hNmu = pred_file.Get('hNumuBand')
    hNmubar = pred_file.Get('hNumubarBand')

    Nmu = LoopOverHist(hNmu, Enu_cut)
    Nmubar = LoopOverHist(hNmubar, Enu_cut)

    print '%s Nmu: %s' % (name, str(Nmu))
    print '%s Nmubar: %s' % (name, str(Nmubar))

    Nmu_bkg_func = func1D_with_err('Nmu_bkg', '66.48/7.48E20*x', '14.458/7.48E20*x')
    Nmubar_bkg_func = func1D_with_err('Nmubar_bkg', '44.41/7.47E20*x', '9.597/7.47E20*x')

    Nmu_true_function = '%f - 66.48/7.48E20*x' % Nmu.mean
    Nmubar_true_function = '%f - 44.41/7.47E20*y' % Nmubar.mean
    ratio_function = '(%s)/(%s)' % (Nmu_true_function, Nmubar_true_function)
    Nmu_Nmubar_ratio = func2D_with_err('Nmu_Nmubar_ratio',
                                       ratio_function,
                                       'TMath::Abs(%s)*TMath::Sqrt((%f**2+%s)/TMath::Power(%s, 2.)+(%f**2+%s)/TMath::Power(%s, 2.))' % (ratio_function, Nmu.err, Nmu_bkg_func.func_err.GetTitle(), Nmu_true_function, Nmubar.err, Nmubar_bkg_func.func_err.GetTitle(), Nmubar_true_function))

    ratio_val_err = val_with_error(Nmu_Nmubar_ratio.func_mean.Eval(FHC_POT, RHC_POT),
                                   Nmu_Nmubar_ratio.func_err.Eval(FHC_POT, RHC_POT))

    print 'ratio function %s' % Nmu_Nmubar_ratio.func_mean.GetTitle()
    print 'ratio function eval with POT: %f' % ratio_val_err.mean

    print 'ratio error %s' % Nmu_Nmubar_ratio.func_err.GetTitle()
    print 'ratio error eval with POT: %f' % ratio_val_err.err
    gaus = ROOT.TF1('gaus', 'gaus(0)', -10., 10.)
    gaus.SetParameters(1./math.sqrt(2.*math.pi), 0., 1.)
    throwsNH = np.zeros(34)
    throwsIH = np.zeros(34)
    seed = ROOT.TDatime().Get()
    ROOT.gRandom.SetSeed(seed)
    for var in range(len(throwsNH)):
        ratio_throw = ratio_val_err.mean + gaus.GetRandom() * ratio_val_err.err
        print 'variation NH %d: ratio = %f' % (var, ratio_throw)
        throwsNH[var] = ProbCPCNH(ratio_throw)
    print 'NH throws'
    print throwsNH
    seed = ROOT.TDatime().Get()
    ROOT.gRandom.SetSeed(seed)
    for var in range(len(throwsIH)):
        ratio_throw = ratio_val_err.mean + gaus.GetRandom() * ratio_val_err.err
        print 'variation IH %d: ratio = %f' % (var, ratio_throw)
        throwsIH[var] = ProbCPCIH(ratio_throw)
    print 'IH throws'
    print throwsIH


def main(argv):
    Enu_cut = 30  # GeV
    prefit_pred = ROOT.TFile('ErrorBands_prefit_BANFF_2017bxsec.root')
    P0D_pred = ROOT.TFile('ErrorBands_P0DBANFF_run_2-8_2017bxsec.root')
    FGD_pred = ROOT.TFile('ErrorBands_FGDBANFF_run_2-6_2017bxsec_TN324.root')

    GetProbCPCHist(prefit_pred.GetName(),
                   **{'name': 'prefit',
                      'Enu_cut': Enu_cut})

    GetProbCPCHist(P0D_pred.GetName(),
                   **{'name': 'P0D postfit',
                      'Enu_cut': Enu_cut})

    GetProbCPCHist(FGD_pred.GetName(),
                   **{'name': 'FGD postfit',
                      'Enu_cut': Enu_cut})


if __name__ == '__main__':
    main(sys.argv)
