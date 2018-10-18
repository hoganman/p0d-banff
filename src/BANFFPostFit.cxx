#define BANFFPOSTFIT_CXX

#include "BANFFPostFit.hxx"
ClassImp(BANFFPostFit)
#include "CanvasCoordinates.hxx"
#include "HEPConstants.hxx"
#include "TLegend.h"
#include "TGaxis.h"
#include "stdio.h"
#include <algorithm>
#include <iostream>
#include <map>

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
    inputFile = TFile::Open(inputFileName);
    if(!inputFile)
        return;
    Init();
}


//**************************************************
void BANFFPostFit::Init()
//**************************************************
{
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

    nSKNuModeNuMuBins = 0;
    nSKNuModeNuMuBarBins = 0;
    SKNuModeNueBins = 0;
    nSKNuModeNueBarBins = 0;
    nSKANuModeNuMuBins = 0;
    nSKANuModeNuMuBarBins = 0;
    nSKANuModeNueBins = 0;
    nSKANuModeNueBarBins = 0;
    AllHistograms.clear();

    if(!inputFile)
        return;

    if(inputFile->Get("param_list"))
        param_list = static_cast<TObjArray*>(inputFile->Get("param_list"));
    if(inputFile->Get("prefit_params"))
        prefit_params = static_cast<TVectorT<Double_t>*>(inputFile->Get("prefit_params"));
    if(inputFile->Get("prefit_cov"))
        prefit_cov = static_cast<TMatrixT<Double_t>*>(inputFile->Get("prefit_cov"));
    if(inputFile->Get("postfit_params"))
        postfit_params = static_cast<TVectorT<Double_t>*>(inputFile->Get("postfit_params"));
    if(inputFile->Get("postfit_cov"))
        postfit_cov = static_cast<TMatrixT<Double_t>*>(inputFile->Get("postfit_cov"));
    if(inputFile->Get("sk_numode_numu_bins"))
    {
        sk_numode_numu_bins = static_cast<TAxis*>(inputFile->Get("sk_numode_numu_bins"));
        nSKNuModeNuMuBins =     sk_numode_numu_bins->GetNbins();
    }
    if(inputFile->Get("sk_numode_numub_bins"))
    {
        sk_numode_numub_bins = static_cast<TAxis*>(inputFile->Get("sk_numode_numub_bins"));
        nSKNuModeNuMuBarBins =  sk_numode_numub_bins->GetNbins();
    }
    if(inputFile->Get("sk_numode_nue_bins"))
    {
        sk_numode_nue_bins = static_cast<TAxis*>(inputFile->Get("sk_numode_nue_bins"));
        SKNuModeNueBins =       sk_numode_nue_bins->GetNbins();
    }
    if(inputFile->Get("sk_numode_nueb_bins"))
    {
        sk_numode_nueb_bins = static_cast<TAxis*>(inputFile->Get("sk_numode_nueb_bins"));
        nSKNuModeNueBarBins =   sk_numode_nueb_bins->GetNbins();
    }
    if(inputFile->Get("sk_anumode_numu_bins"))
    {
        sk_anumode_numu_bins = static_cast<TAxis*>(inputFile->Get("sk_anumode_numu_bins"));
        nSKANuModeNuMuBins =    sk_anumode_numu_bins->GetNbins();
    }
    if(inputFile->Get("sk_anumode_numub_bins"))
    {
        sk_anumode_numub_bins = static_cast<TAxis*>(inputFile->Get("sk_anumode_numub_bins"));
        nSKANuModeNuMuBarBins = sk_anumode_numub_bins->GetNbins();
    }
    if(inputFile->Get("sk_anumode_nue_bins"))
    {
        sk_anumode_nue_bins = static_cast<TAxis*>(inputFile->Get("sk_anumode_nue_bins"));
        nSKANuModeNueBins =     sk_anumode_nue_bins->GetNbins();
    }
    if(inputFile->Get("sk_anumode_nueb_bins"))
    {
        sk_anumode_nueb_bins = static_cast<TAxis*>(inputFile->Get("sk_anumode_nueb_bins"));
        nSKANuModeNueBarBins =  sk_anumode_nueb_bins->GetNbins();
    }

    const TList* keys = inputFile->GetListOfKeys();
    for(Int_t keyIndex = 0; keyIndex < keys->GetSize(); ++keyIndex)
    {
        const TString name = keys->At(keyIndex)->GetName();
        if(name.Contains("rxnPredMC") || name.Contains("_prefit") || name.Contains("_data") || name.Contains("_postfit"))
        {
            TObject* searchObject = P0DBANFFInterface::FindObjectInFileByName(inputFile, name);
            AllHistograms[name] = static_cast< THnT<double>* >(searchObject);
        }
    }

}

//**************************************************
void BANFFPostFit::Dump(TString prefit_cov_csv, TString postfit_cov_csv)
//**************************************************
{
    if(!param_list)
    {
        P0DBANFFInterface::Error(this, "ERROR: No parameter list is set, did you set the input file?");
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

    for(Int_t axis_index = 0; axis_index < n_axes; ++axis_index)
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


//**************************************************
THnT<double>* BANFFPostFit::GetTHn(const TString &name) const
//**************************************************
{
    std::map< TString, THnT<double>* >::const_iterator it = AllHistograms.find(name);
    if(it != AllHistograms.end())
        return it->second;

    P0DBANFFInterface::Warning(this, TString::Format("Unable to find %s using full name. Trying TString::Contains", name.Data()));
    for(it = AllHistograms.begin(); it != AllHistograms.end(); ++it)
    {
        if(it->first.Contains(name))
        {
            P0DBANFFInterface::Announce(this, TString::Format("Found potential match, returning %s", it->first.Data()));
            return it->second;
        }
    }
    return NULL;
}


//**************************************************
TCanvas* BANFFPostFit::GetDataPrePostfitMCWithProjection(const TString &name,
        const Int_t &projection, const Double_t &normalizeBinsBy ) const
//**************************************************
{
    const TString prefitName = name + "_prefit";
    THnT<double>* prefit = GetTHn(prefitName);
    if(!prefit)
    {
        P0DBANFFInterface::Error(this, TString::Format("Unable to get prefit for %s", name.Data()));
        return NULL;
    }
    const TString dataName = name + "_data";
    THnT<double>* data = GetTHn(dataName);
    if(!data)
    {
        P0DBANFFInterface::Error(this, TString::Format("Unable to get data for %s", name.Data()));
        return NULL;
    }
    const TString postfitName = name + "_postfit_0_0";
    THnT<double>* postfit = GetTHn(postfitName);
    if(!postfit)
    {
        P0DBANFFInterface::Warning(this, TString::Format("Unable to get data for %s", name.Data()));
    }

    TH1D* prefit_1D = static_cast<TH1D*>(prefit->Projection(projection)->Clone(TString::Format("hPrefit_%s", prefitName.Data())));
    TH1D* postfit_1D = postfit ? static_cast<TH1D*>(postfit->Projection(projection)->Clone(TString::Format("hPostfit_%s", postfitName.Data()))) : NULL;
    TH1D* data_1D = static_cast<TH1D*>(data->Projection(projection, "E")->Clone(TString::Format("hData_%s", dataName.Data())));
    if( normalizeBinsBy > 0 )
    {
        for(Int_t bin = 1; bin <= prefit_1D->GetXaxis()->GetNbins(); ++bin)
        {
            gStyle->SetOptStat(0000);
            Double_t prefit_binContent = prefit_1D->GetBinContent(bin);
            prefit_binContent *= normalizeBinsBy / prefit_1D->GetXaxis()->GetBinWidth(bin);
            prefit_1D->SetBinContent(bin, prefit_binContent);
            Double_t data_binContent = data_1D->GetBinContent(bin);
            data_binContent *= normalizeBinsBy / data_1D->GetXaxis()->GetBinWidth(bin);
            data_1D->SetBinContent(bin, data_binContent);
            if(!postfit)
               continue;
            Double_t postfit_binContent = postfit_1D->GetBinContent(bin);
            postfit_binContent *= normalizeBinsBy / postfit_1D->GetXaxis()->GetBinWidth(bin);
            postfit_1D->SetBinContent(bin, postfit_binContent);
        }
    }

    prefit_1D->SetLineWidth(3);
    data_1D->SetLineWidth(3);

    data_1D->SetMarkerStyle(P0DBANFFInterface::kDataMarkerStyle);
    prefit_1D->SetLineStyle(P0DBANFFInterface::kDashedLineStyle);

    data_1D->SetMinimum(0);

    Double_t max = std::max(prefit_1D->GetMaximum(), data_1D->GetMaximum());
    max = postfit_1D ? 1.1 * std::max(max, postfit_1D->GetMaximum()) : 1.1 * max;

    data_1D->SetMaximum(max);
    prefit_1D->SetMaximum(max);

    TString xAxisTitle;
    TString yAxisTitle;
    if(projection == 0)
    {
        if(normalizeBinsBy > 0)
            yAxisTitle = TString::Format("Events/(%.f MeV/c)", normalizeBinsBy);
        else
            yAxisTitle = "Events/bin";
        xAxisTitle = "Lepton Candidate Momentum [MeV/c]";
    }
    if(projection == 1)
    {
        if(normalizeBinsBy > 0)
            yAxisTitle = TString::Format("Events/(%.f)", normalizeBinsBy);
        else
            yAxisTitle = "Events/bin";
        xAxisTitle = "Lepton Candidate Angle [cos(#theta)]";
    }

    P0DBANFFInterface::PrettyUpTH1(data_1D, xAxisTitle, yAxisTitle, P0DBANFFInterface::kcbBlack);
    P0DBANFFInterface::PrettyUpTH1(prefit_1D, xAxisTitle, yAxisTitle, P0DBANFFInterface::kcbSky);

    TCanvas* canvas = new TCanvas(TString::Format("canvas_%s", name.Data()), "", 800,600);
    TPad* pad1 = new TPad("pad1", "pad1", 0.0, 0.25, 1.0, 1.0);
    pad1->SetBottomMargin(0.1);
    pad1->Draw();
    pad1->cd();
    data_1D->Draw();
    prefit_1D->Draw("samehist");
    const CanvasCoordinates coords;
    TLegend* legend = new TLegend(coords.Legend_RHS_X1, coords.Legend_RHS_Y1, coords.Legend_RHS_X2, coords.Legend_RHS_Y2, name);
    legend->AddEntry(data_1D, "Data", "LPE");
    legend->AddEntry(prefit_1D, "Prefit", "L");
    legend->SetLineColor(0);
    legend->SetBorderSize(0);
    legend->SetFillStyle(0);

    if(postfit_1D)
    {
        postfit_1D->SetLineColor(P0DBANFFInterface::kcbBlack);
        postfit_1D->SetLineStyle(P0DBANFFInterface::kSolidLineStyle);
        postfit_1D->SetLineWidth(3);
        postfit_1D->SetMaximum(max);
        legend->AddEntry(postfit_1D, "Postfit", "L");
        postfit_1D->Draw("samehist");
    }
    legend->Draw();

    canvas->cd();
    TPad* pad2 = new TPad("pad2", "pad2", 0, 0.05, 1, 0.225);
    pad2->SetTopMargin(0);
    pad2->SetBottomMargin(0.1);
    pad2->SetGridx();  // vertical grid
    pad2->SetGridy();  // horizontal grid
    pad2->Draw();
    pad2->cd();      // pad2 becomes the current pad

    // Define the ratio plot
    TH1D* data_prefit_ratio = static_cast<TH1D*>(data_1D->Clone(TString::Format("data_prefit_ratio_%s", name.Data())));
    data_prefit_ratio->SetLineColor(P0DBANFFInterface::kcbBlack);
    data_prefit_ratio->Divide(prefit_1D);
    const Double_t data_prefit_ratio_max_bin = data_prefit_ratio->GetMaximumBin();
    const Double_t data_prefit_ratio_min_bin = data_prefit_ratio->GetMinimumBin();
    data_prefit_ratio->SetMaximum(std::min(1.5, 1.1*(data_prefit_ratio->GetBinContent(data_prefit_ratio_max_bin)+data_prefit_ratio->GetBinError(data_prefit_ratio_max_bin))));  // range
    data_prefit_ratio->SetMinimum(std::max(0.5, 0.9*(data_prefit_ratio->GetBinContent(data_prefit_ratio_min_bin)-data_prefit_ratio->GetBinError(data_prefit_ratio_min_bin))));  // range
    data_prefit_ratio->SetStats(0);       // No statistics on lower plot
    data_prefit_ratio->SetMarkerStyle(P0DBANFFInterface::kDataMarkerStyle);

    TLine* line = new TLine(data_prefit_ratio->GetXaxis()->GetBinLowEdge(1), 1, data_prefit_ratio->GetXaxis()->GetBinUpEdge(data_prefit_ratio->GetXaxis()->GetNbins()), 1);
    line->SetLineWidth(3);
    line->SetLineStyle(P0DBANFFInterface::kThickDashedLineStyle);
    line->SetLineColor(P0DBANFFInterface::kcbBlue);
    data_prefit_ratio->Draw("ep");       // Draw the ratio plot
    line->Draw();
    data_prefit_ratio->Draw("ep same");   // Draw the ratio plot

    // Y axis ratio plot settings
    data_prefit_ratio->GetYaxis()->SetTitle("Data/MC");
    data_prefit_ratio->GetYaxis()->SetNdivisions(505);
    data_prefit_ratio->GetYaxis()->SetLabelSize(3*prefit_1D->GetYaxis()->GetLabelSize());
    data_prefit_ratio->GetYaxis()->SetTitleOffset(0.2);
    data_prefit_ratio->GetYaxis()->SetTitleSize(0.2);

    // X axis ratio plot settings
    data_prefit_ratio->GetXaxis()->SetTitleOffset(prefit_1D->GetXaxis()->GetTitleOffset());
    data_prefit_ratio->GetXaxis()->SetTitleSize(0.15);
    data_prefit_ratio->GetXaxis()->SetLabelSize(3* prefit_1D->GetXaxis()->GetLabelSize());
    //data_prefit_ratio->GetXaxis()->SetLabelSize(15);
    return canvas;
}


//**************************************************
TCanvas* BANFFPostFit::GetReactionCodePrePostfitMC(const TString &name,
        const Int_t &projection, const Double_t &normalizeBinsBy) const
//**************************************************
{
    const HEPConstants hep;
    std::map< TString, THnT<double>* >::const_iterator it;

    for(it = AllHistograms.begin(); it != AllHistograms.end(); ++it)
    {
        const TString histogramName = it->second->GetName();
        if(!histogramName.Contains("rxn"))
            continue;
        const Int_t reactionCode = GetReactionCode(histogramName);
    }
}

//**************************************************
Int_t BANFFPostFit::GetReactionCode(const TString &histogramName) const
//**************************************************
{
    if(!histogramName.Contains("rxn"))
    {
        P0DBANFFInterface::Error(this, "Input histogram does NOT contain \"rxn\". No reaction code obtained");
        return 0;
    }
    const Int_t alteredReactionCode =
        TString(histogramName(histogramName.Index("_rxnPredMC_")+11,3)).Atoi();
    return kReactionCodeOffset - alteredReactionCode;
}
