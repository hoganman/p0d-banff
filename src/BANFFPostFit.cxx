#define BANFFPOSTFIT_CXX

#include "BANFFPostFit.hxx"
#include "stdio.h"
#include <iostream>
#include <map>
ClassImp(BANFFPostFit)

//**************************************************
BANFFPostFit::BANFFPostFit()
//**************************************************
{
    Init();
}

//**************************************************
BANFFPostFit::BANFFPostFit(TString inputFileName)
//**************************************************
{
    SetFile(inputFileName);
}

//**************************************************
BANFFPostFit::~BANFFPostFit()
//**************************************************
{
    if (inputFile) inputFile->Close();
}

//**************************************************
void BANFFPostFit::SetFile(TString inputFileName)
//**************************************************
{
    Init();
    inputFile = TFile::Open(inputFileName);
    if(!inputFile)
        return;
    param_list = static_cast<TObjArray*>(inputFile->Get("param_list"));
    prefit_params = static_cast<TVectorT<Double_t>*>(inputFile->Get("prefit_params"));
    prefit_cov = static_cast<TMatrixT<Double_t>*>(inputFile->Get("prefit_cov"));
    postfit_params = static_cast<TVectorT<Double_t>*>(inputFile->Get("postfit_params"));
    postfit_cov = static_cast<TMatrixT<Double_t>*>(inputFile->Get("postfit_cov"));
    sk_numode_numu_bins = static_cast<TAxis*>(inputFile->Get("sk_numode_numu_bins"));
    sk_numode_numub_bins = static_cast<TAxis*>(inputFile->Get("sk_numode_numub_bins"));
    sk_numode_nue_bins = static_cast<TAxis*>(inputFile->Get("sk_numode_nue_bins"));
    sk_numode_nueb_bins = static_cast<TAxis*>(inputFile->Get("sk_numode_nueb_bins"));
    sk_anumode_numu_bins = static_cast<TAxis*>(inputFile->Get("sk_anumode_numu_bins"));
    sk_anumode_numub_bins = static_cast<TAxis*>(inputFile->Get("sk_anumode_numub_bins"));
    sk_anumode_nue_bins = static_cast<TAxis*>(inputFile->Get("sk_anumode_nue_bins"));
    sk_anumode_nueb_bins = static_cast<TAxis*>(inputFile->Get("sk_anumode_nueb_bins"));
}


//**************************************************
void BANFFPostFit::Init()
//**************************************************
{

    inputFile = NULL;
    param_list = NULL;
    prefit_params = NULL;
    prefit_cov = NULL;
    postfit_params = NULL;
    postfit_cov = NULL;
    sk_numode_numu_bins = NULL;
    sk_numode_numub_bins = NULL;
    sk_numode_nue_bins = NULL;
    sk_numode_nueb_bins = NULL;
    sk_anumode_numu_bins = NULL;
    sk_anumode_numub_bins = NULL;
    sk_anumode_nue_bins = NULL;
    sk_anumode_nueb_bins = NULL;

    nSKNuModeNuMuBins =     sk_numode_numu_bins->GetNbins();
    nSKNuModeNuMuBarBins =  sk_numode_numub_bins->GetNbins();
    SKNuModeNueBins =       sk_numode_nue_bins->GetNbins();
    nSKNuModeNueBarBins =   sk_numode_nueb_bins->GetNbins();
    nSKANuModeNuMuBins =    sk_anumode_numu_bins->GetNbins();
    nSKANuModeNuMuBarBins = sk_anumode_numub_bins->GetNbins();
    nSKANuModeNueBins =     sk_anumode_nue_bins->GetNbins();
    nSKANuModeNueBarBins =  sk_anumode_nueb_bins->GetNbins();

}

//**************************************************
void BANFFPostFit::Dump(TString prefit_cov_csv, TString postfit_cov_csv)
//**************************************************
{
    if(!param_list)
    {
        std::cout << "ERROR: No parameter list is set, did you set the input file? " << std::endl;
        return;
    }

    printf("+-------------------------------------+\n");
    printf("|%-20s | %5s | %5s +\n", "Parameter name", "Pre", "Post");
    printf("+-------------------------------------+\n");
    for(Int_t i = 0; i < param_list->GetSize(); ++i)
        printf("|%-20s   %.3f   %.3f |\n", param_list->At(i)->GetName(), prefit_params->GetMatrixArray()[i], postfit_params->GetMatrixArray()[i]);
    printf("+-------------------------------------+\n");

    const Int_t n_axes = 8;
    TAxis* axes[n_axes] = {sk_numode_numu_bins , sk_numode_numub_bins , sk_numode_nue_bins , sk_numode_nueb_bins , sk_anumode_numu_bins , sk_anumode_numub_bins , sk_anumode_nue_bins, sk_anumode_nueb_bins};
    //UInt_t nBins_per_axis[n_axes] = {nSKNuModeNuMuBins,nSKNuModeNuMuBarBins,SKNuModeNueBins,nSKNuModeNueBarBins,nSKANuModeNuMuBins,nSKANuModeNuMuBarBins,nSKANuModeNueBins,nSKANuModeNueBarBins};
    TString names[n_axes] = {"sk_numode_numu_bins" , "sk_numode_numub_bins" , "sk_numode_nue_bins" , "sk_numode_nueb_bins" , "sk_anumode_numu_bins" , "sk_anumode_numub_bins" , "sk_anumode_nue_bins", "sk_anumode_nueb_bins"};

    for(UInt_t axis_index = 0; axis_index < n_axes; ++axis_index)
    {
        TAxis* const axis = axes[axis_index];
        const TString name = names[axis_index];
        printf("+-------------------------------+\n");
        printf(" Flux bins: %s \n", name.Data());
        printf("+-------------------------------+\n");
        printf(" index | bin # | bin range [GeV]\n");
        printf("+-------------------------------+\n");
        for(Int_t bin = 1; bin <= axis->GetNbins(); ++bin)
        {
            printf(" %-5d   %-5d   %.3f-%.3f\n", bin - 1, bin, axis->GetBinLowEdge(bin), axis->GetBinUpEdge(bin));
        }
        printf("+-------------------------------+\n");
    }

    if (prefit_cov_csv.Length() > 0)
    {
        FILE* prefix_cov_file = fopen(prefit_cov_csv.Data(), "w");
        fprintf(prefix_cov_file, "#%s", param_list->At(0)->GetName());
        //print the header
        for(Int_t i = 1; i < param_list->GetSize(); ++i)
        {
            fprintf(prefix_cov_file, ",%s", param_list->At(i)->GetName());
        }
        fprintf(prefix_cov_file, "\n");
        const UInt_t nCols = prefit_cov->GetNcols();
        for(UInt_t col = 0; col < nCols; ++col)
        {
            const Int_t col_index = col*nCols;
            Double_t cov_value = prefit_cov->GetMatrixArray()[col_index];
            fprintf(prefix_cov_file, "%.3f", cov_value);
            for(UInt_t row = 1; row < nCols; ++row)
            {
                cov_value = prefit_cov->GetMatrixArray()[row+col_index];
                fprintf(prefix_cov_file, ",%.3f", cov_value);
            }
            fprintf(prefix_cov_file, "\n");
        }
        fclose(prefix_cov_file);
        printf("%s has been created!\n", prefit_cov_csv.Data());
    }

    if (postfit_cov_csv.Length() > 0)
    {
        FILE* prefix_cov_file = fopen(postfit_cov_csv.Data(), "w");
        fprintf(prefix_cov_file, "#%s", param_list->At(0)->GetName());
        //print the header
        for(Int_t i = 1; i < param_list->GetSize(); ++i)
        {
            fprintf(prefix_cov_file, ",%s", param_list->At(i)->GetName());
        }
        fprintf(prefix_cov_file, "\n");
        const UInt_t nCols = postfit_cov->GetNcols();
        for(UInt_t col = 0; col < nCols; ++col)
        {
            const Int_t col_index = col*nCols;
            Double_t cov_value = postfit_cov->GetMatrixArray()[col_index];
            fprintf(prefix_cov_file, "%.3f", cov_value);
            for(UInt_t row = 1; row < nCols; ++row)
            {
                cov_value = postfit_cov->GetMatrixArray()[row+col_index];
                fprintf(prefix_cov_file, ",%.3f", cov_value);
            }
            fprintf(prefix_cov_file, "\n");
        }
        fclose(prefix_cov_file);
        printf("%s has been created!\n", postfit_cov_csv.Data());
    }

}
