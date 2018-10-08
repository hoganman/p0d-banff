#ifndef PLOTTINGSAMPLE_HXX
#define PLOTTINGSAMPLE_HXX

#include "TChain.h"
#include "TString.h"
#include "TCut.h"

class  PlottingSample : public TObject
{
public:

    PlottingSample()
        : chain(NULL), plotTitle(""), saveTitle(""),
                scale(1.0), data_pot(1.0), is_FHC(kTRUE)
    {}

    virtual ~PlottingSample(){}

    PlottingSample(TChain* input_chain, const TString& plot_title,
            const TString& save_title);

    TChain* GetTChain() {return chain;}

protected:

    TChain* chain;

public:

    TString plotTitle;
    TString saveTitle;
    Double_t scale;
    Double_t data_pot;
    Bool_t is_FHC;

    ClassDef(PlottingSample, 1)
};

#endif
