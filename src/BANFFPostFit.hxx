#ifndef BANFFPOSTFIT_HXX
#define BANFFPOSTFIT_HXX

#include "TObjArray.h"
#include "TString.h"
#include "TFile.h"
#include "TVectorT.h"
#include "TMatrixT.h"
#include "TAxis.h"

class BANFFPostFit : public TObject
{
public:

    BANFFPostFit();

    BANFFPostFit(TString inputFileName);

    virtual ~BANFFPostFit();

    TFile* GetInputFile() {return inputFile;}

    void SetFile(TString inputFileName);

    TObjArray* GetParameters() {return param_list;}

    TVectorT<Double_t>* GetPrefit_Params() {return prefit_params;}

    TVectorT<Double_t>* GetPostfit_Params() {return postfit_params;}

    TMatrixT<Double_t>* GetPrefit_Cov() {return prefit_cov;}

    TMatrixT<Double_t>* GetPostfit_Cov() {return postfit_cov;}

    TAxis* Get_SK_NuMode_NuMu_bins()    {return sk_numode_numu_bins;}
    TAxis* Get_SK_NuMode_NuMubar_bins() {return sk_numode_numub_bins;}
    TAxis* Get_SK_NuMode_NuE_bins()     {return sk_numode_nue_bins;}
    TAxis* Get_SK_NuMode_NuEbar_bins()  {return sk_numode_nueb_bins;}
    TAxis* Get_SK_ANuMode_NuMu_bins()   {return sk_anumode_numu_bins;}
    TAxis* Get_SK_ANuMode_NuMubar_bins(){return sk_anumode_numub_bins;}
    TAxis* Get_SK_ANuMode_NuE_bins()    {return sk_anumode_nue_bins;}
    TAxis* Get_SK_ANuMode_NuEbar_bins() {return sk_anumode_nueb_bins;}

    void Dump(TString prefit_cov_csv="", TString postfit_cov_csv="");

protected:

    TFile* inputFile;  //!
    TObjArray *param_list;  //!
    TVectorT<Double_t> *prefit_params;  //!
    TMatrixT<Double_t> *prefit_cov;  //!
    TVectorT<Double_t> *postfit_params;  //!
    TMatrixT<Double_t> *postfit_cov;  //!
    TAxis *sk_numode_numu_bins;  //!
    TAxis *sk_numode_numub_bins;  //!
    TAxis *sk_numode_nue_bins;  //!
    TAxis *sk_numode_nueb_bins;  //!
    TAxis *sk_anumode_numu_bins;  //!
    TAxis *sk_anumode_numub_bins;  //!
    TAxis *sk_anumode_nue_bins;  //!
    TAxis *sk_anumode_nueb_bins;  //!

    UInt_t nSKNuModeNuMuBins;
    UInt_t nSKNuModeNuMuBarBins;
    UInt_t SKNuModeNueBins;
    UInt_t nSKNuModeNueBarBins;
    UInt_t nSKANuModeNuMuBins;
    UInt_t nSKANuModeNuMuBarBins;
    UInt_t nSKANuModeNueBins;
    UInt_t nSKANuModeNueBarBins;

    void Init();

public:
    ClassDef(BANFFPostFit,1)

};

#endif
