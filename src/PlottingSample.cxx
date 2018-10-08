#define PLOTTINGSAMPLE_CXX

#include "PlottingSample.hxx"
ClassImp(PlottingSample)

//**************************************************
PlottingSample::PlottingSample(TChain* input_chain, const TString &plot_title,
            const TString &save_title) :
    scale(1.0), data_pot(1.0), is_FHC(kTRUE)
//**************************************************
{
    chain = input_chain;
    plotTitle = plot_title;
    saveTitle = save_title;
}


