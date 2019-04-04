#include "TH1D.h"
#include "TH2F.h"
#include "TVectorT.h"
#include "TFile.h"
#include "TMath.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TF1.h"
#include "TLine.h"
#include "TDirectoryFile.h"

Int_t GetGlobalBin(TH2F* variance, Int_t startBin, Int_t binx, Int_t biny);
Bool_t IsEqualAbs(Double_t v1, Double_t v2, Double_t delta=0.0001);
TH2F* pth_fracerror = NULL;
TH2F* pth_mean = NULL;
TCanvas* canvas = NULL;

struct binning_t
{
    char name[50];
    char title[50];

    int nmom;
    double* mom;
    int nmom_det;
    double* mom_det;

    int ncos;
    double* cos;
    int ncos_det;
    double* cos_det;

    binning_t(const char* Name="", const char* Title="", int n_Mom=0, double* Mom=0, int n_Mom_det=0, double* Mom_det = 0, int n_Cos=0, double* Cos=0, int n_Cos_det=0, double* Cos_det=0)
    {
        strcpy(name, Name);
        strcpy(title, Title);
        nmom = n_Mom;
        nmom_det = n_Mom_det;
        ncos = n_Cos;
        ncos_det = n_Cos_det;

        mom = Mom;
        cos = Cos;
        mom_det = Mom_det;
        cos_det = Cos_det;
    }
};

void GetSystematicsMatrix(std::string inputfilename)
{
    const Int_t nUniverse = 2000;
    const Int_t nMinBinsForVariations = 250;
    const UInt_t nBinning = 6;
    TFile* inputfile = TFile::Open(inputfilename.c_str());
    TH2F* Total_Covariance_Matrix = static_cast<TH2F*>(inputfile->Get("Total_Covariance_Matrix"));
    TH1D* Nominal = static_cast<TH1D*>(inputfile->Get("Nominal"));
    TH1D* Varied_Mean = static_cast<TH1D*>(inputfile->Get("Varied_Mean"));
    TVectorT<double>* Mean_Value = static_cast< TVectorT<double>* >(inputfile->Get("Mean_Value"));
    TDirectoryFile* BinVariations = static_cast<TDirectoryFile*>(inputfile->Get("BinVariations"));

    struct binning_t all_binning[nBinning];

    //P0D FHC numu CC 1-track
    int    P0DFHCNumuCC1Tr_Mom_NBin = 14;
    double P0DFHCNumuCC1Tr_Mom_Bin[15] =     {0, 300, 400, 500, 600, 700, 800, 1000, 1250, 1500, 2000, 3000, 4000, 5500, 30000};
    int    P0DFHCNumuCC1Tr_Det_Mom_NBin = 10;
    double P0DFHCNumuCC1Tr_Det_Mom_Bin[11] = {0, 300, 400, 600, 800, 1250, 2000, 3000, 4000, 5500, 30000};

    int    P0DFHCNumuCC1Tr_Cos_NBin = 8;
    double P0DFHCNumuCC1Tr_Cos_Bin[9] =     {-1, 0.7, 0.8 , 0.88, 0.94, 0.96, 0.975, 0.99, 1.};
    int    P0DFHCNumuCC1Tr_Det_Cos_NBin = 6;
    double P0DFHCNumuCC1Tr_Det_Cos_Bin[7] = {-1, 0.7, 0.8 ,0.94, 0.975, 0.99, 1.};

    //P0D FHC numu CC N-tracks
    int    P0DFHCNumuCCnTr_Mom_NBin = 15;
    double P0DFHCNumuCCnTr_Mom_Bin[16] =     {0, 400, 500, 600, 700, 800, 1000, 1200, 1500, 1800, 2200, 2700, 3500, 5000, 10000, 30000};
    int    P0DFHCNumuCCnTr_Det_Mom_NBin = 8;
    double P0DFHCNumuCCnTr_Det_Mom_Bin[9] = {0, 400, 600, 800, 1200, 2200, 3500, 10000, 30000};

    int    P0DFHCNumuCCnTr_Cos_NBin = 8;
    double P0DFHCNumuCCnTr_Cos_Bin[9] =     {-1, 0.65, 0.77, 0.85, 0.9, 0.94, 0.97, 0.99, 1.};
    int    P0DFHCNumuCCnTr_Det_Cos_NBin = 6;
    double P0DFHCNumuCCnTr_Det_Cos_Bin[7] = {-1, 0.65, 0.77, 0.85, 0.9, 0.97, 1.};

    //P0D RHC numubar CC 1-track
    int    P0DRHCANumuCC1Tr_Mom_NBin = 11;
    double P0DRHCANumuCC1Tr_Mom_Bin[12] =     {0, 400, 500, 600, 700, 800, 1000, 1250, 1500, 2000, 3000, 30000};
    int    P0DRHCANumuCC1Tr_Det_Mom_NBin = 8;
    double P0DRHCANumuCC1Tr_Det_Mom_Bin[9] = {0, 400, 500, 600, 800, 1250, 2000, 3000, 30000};

    int    P0DRHCANumuCC1Tr_Cos_NBin = 8;
    double P0DRHCANumuCC1Tr_Cos_Bin[9] =     {-1, 0.82, 0.87, 0.9, 0.93, 0.95, 0.97, 0.99, 1.};
    int    P0DRHCANumuCC1Tr_Det_Cos_NBin = 5;
    double P0DRHCANumuCC1Tr_Det_Cos_Bin[6] = {-1, 0.82, 0.9, 0.95, 0.99, 1.};

    //P0D RHC numubar CC N-tracks
    int    P0DRHCANumuCCnTr_Mom_NBin = 8;
    double P0DRHCANumuCCnTr_Mom_Bin[9] =     {0, 500, 900, 1250, 1600, 2000, 3000, 8000, 30000};
    int    P0DRHCANumuCCnTr_Det_Mom_NBin = 8;
    double P0DRHCANumuCCnTr_Det_Mom_Bin[9] = {0, 500, 900, 1250, 1600, 2000, 3000, 8000, 30000};

    int    P0DRHCANumuCCnTr_Cos_NBin = 6;
    double P0DRHCANumuCCnTr_Cos_Bin[7] =     {-1, 0.8, 0.89, 0.95, 0.97, 0.99, 1.};
    int    P0DRHCANumuCCnTr_Det_Cos_NBin = 6;
    double P0DRHCANumuCCnTr_Det_Cos_Bin[7] = {-1, 0.8, 0.89, 0.95, 0.97, 0.99, 1.};

    //P0D RHCNumuCC1Tr
    int    P0DRHCNumuCC1Tr_Mom_NBin = 6;
    double P0DRHCNumuCC1Tr_Mom_Bin[7] =     {0, 400, 600, 800, 1100, 2000, 10000};
    int    P0DRHCNumuCC1Tr_Det_Mom_NBin = 6;
    double P0DRHCNumuCC1Tr_Det_Mom_Bin[7] = {0, 400, 600, 800, 1100, 2000, 10000};

    int    P0DRHCNumuCC1Tr_Cos_NBin = 9;
    double P0DRHCNumuCC1Tr_Cos_Bin[10] =     {-1, 0.78, 0.84, 0.89, 0.92, 0.95, 0.97, 0.98, 0.99, 1.0};
    int    P0DRHCNumuCC1Tr_Det_Cos_NBin = 7;
    double P0DRHCNumuCC1Tr_Det_Cos_Bin[8] = {-1, 0.78, 0.84, 0.92, 0.95, 0.98, 0.99, 1.0};

    //P0D RHCNumuCCnTr
    int    P0DRHCNumuCCnTr_Mom_NBin = 8;
    double P0DRHCNumuCCnTr_Mom_Bin[9] =     {0, 400, 600, 800, 1000, 1500, 2000, 3000, 10000};
    int    P0DRHCNumuCCnTr_Det_Mom_NBin = 7;
    double P0DRHCNumuCCnTr_Det_Mom_Bin[8] = {0, 400, 600, 1000, 1500, 2000, 3000, 10000};

    int    P0DRHCNumuCCnTr_Cos_NBin = 9;
    double P0DRHCNumuCCnTr_Cos_Bin[10] =     {-1, 0.7, 0.8, 0.85, 0.9, 0.94, 0.965, 0.98, 0.99, 1.0};
    int    P0DRHCNumuCCnTr_Det_Cos_NBin = 7;
    double P0DRHCNumuCCnTr_Det_Cos_Bin[8] = {-1, 0.7, 0.8, 0.85, 0.94, 0.98, 0.99, 1.0};

    const struct binning_t P0DFHCNumuCC1Tr ("P0DFHCNumuCC1Tr", "FHC #nu_{#mu} CC 1-Track", P0DFHCNumuCC1Tr_Mom_NBin, P0DFHCNumuCC1Tr_Mom_Bin, P0DFHCNumuCC1Tr_Det_Mom_NBin, P0DFHCNumuCC1Tr_Det_Mom_Bin, P0DFHCNumuCC1Tr_Cos_NBin, P0DFHCNumuCC1Tr_Cos_Bin, P0DFHCNumuCC1Tr_Det_Cos_NBin, P0DFHCNumuCC1Tr_Det_Cos_Bin);
    const struct binning_t P0DFHCNumuCCnTr ("P0DFHCNumuCCnTr", "FHC #nu_{#mu} CC n-Tracks", P0DFHCNumuCCnTr_Mom_NBin, P0DFHCNumuCCnTr_Mom_Bin, P0DFHCNumuCCnTr_Det_Mom_NBin, P0DFHCNumuCCnTr_Det_Mom_Bin, P0DFHCNumuCCnTr_Cos_NBin, P0DFHCNumuCCnTr_Cos_Bin, P0DFHCNumuCCnTr_Det_Cos_NBin, P0DFHCNumuCCnTr_Det_Cos_Bin);
    const struct binning_t P0DRHCANumuCC1Tr("P0DRHCANumuCC1Tr", "RHC #bar{#nu}_{#mu} CC 1-Track", P0DRHCANumuCC1Tr_Mom_NBin, P0DRHCANumuCC1Tr_Mom_Bin, P0DRHCANumuCC1Tr_Det_Mom_NBin, P0DRHCANumuCC1Tr_Det_Mom_Bin, P0DRHCANumuCC1Tr_Cos_NBin, P0DRHCANumuCC1Tr_Cos_Bin, P0DRHCANumuCC1Tr_Det_Cos_NBin, P0DRHCANumuCC1Tr_Det_Cos_Bin);
    const struct binning_t P0DRHCANumuCCnTr("P0DRHCANumuCCnTr", "RHC #bar{#nu}_{#mu} CC n-Tracks", P0DRHCANumuCCnTr_Mom_NBin, P0DRHCANumuCCnTr_Mom_Bin, P0DRHCANumuCCnTr_Det_Mom_NBin, P0DRHCANumuCCnTr_Det_Mom_Bin, P0DRHCANumuCCnTr_Cos_NBin, P0DRHCANumuCCnTr_Cos_Bin, P0DRHCANumuCCnTr_Det_Cos_NBin, P0DRHCANumuCCnTr_Det_Cos_Bin);
    const struct binning_t P0DRHCNumuCC1Tr("P0DRHCNumuCC1Tr", "RHC #nu_{#mu} CC 1-Track", P0DRHCNumuCC1Tr_Mom_NBin, P0DRHCNumuCC1Tr_Mom_Bin, P0DRHCNumuCC1Tr_Det_Mom_NBin, P0DRHCNumuCC1Tr_Det_Mom_Bin, P0DRHCNumuCC1Tr_Cos_NBin, P0DRHCNumuCC1Tr_Cos_Bin, P0DRHCNumuCC1Tr_Det_Cos_NBin, P0DRHCNumuCC1Tr_Det_Cos_Bin);
    const struct binning_t P0DRHCNumuCCnTr("P0DRHCNumuCCnTr", "RHC #nu_{#mu} CC n-Tracks", P0DRHCNumuCCnTr_Mom_NBin, P0DRHCNumuCCnTr_Mom_Bin, P0DRHCNumuCCnTr_Det_Mom_NBin, P0DRHCNumuCCnTr_Det_Mom_Bin, P0DRHCNumuCCnTr_Cos_NBin, P0DRHCNumuCCnTr_Cos_Bin, P0DRHCNumuCCnTr_Det_Cos_NBin, P0DRHCNumuCCnTr_Det_Cos_Bin);
    UInt_t nSamples = 0;
    //all_binning[nSamples++] = P0DFHCNumuCC1Tr;
    //all_binning[nSamples++] = P0DFHCNumuCCnTr;
    //all_binning[nSamples++] = P0DRHCANumuCC1Tr;
    //all_binning[nSamples++] = P0DRHCANumuCCnTr;
    //all_binning[nSamples++] = P0DRHCNumuCC1Tr;
    //all_binning[nSamples++] = P0DRHCNumuCCnTr;


    TLegend* legend = new TLegend(0.25, 0.91, 0.75, 0.975, "");
    int startbin = 1;//Total_Covariance_Matrix->GetXaxis()->GetNbins()/2;
    for(UInt_t binning_index = 0; binning_index < nSamples; ++binning_index)
    {
        canvas = new TCanvas("canvas", "", 1200, 500);
        canvas->Divide(2,1);
        struct binning_t binning = all_binning[binning_index];
        legend->SetHeader(binning.title);
        int nBinsX = binning.nmom;
        int nBinsY = binning.ncos;
        int nBinsX_det = binning.nmom_det;
        int nBinsY_det = binning.ncos_det;

        pth_mean = new TH2F("pth_mean", "", nBinsX, 0, nBinsX, nBinsY, 0, nBinsY);
        pth_mean->GetXaxis()->SetLabelSize(0.04);
        pth_mean->GetYaxis()->SetLabelSize(0.05);
        pth_mean->GetYaxis()->SetTitleOffset(2);
        pth_mean->GetYaxis()->CenterTitle();
        pth_mean->SetMaximum(TMath::Max(pth_mean->GetMaximum(), 2.5));
        pth_mean->SetMinimum(0.5);
        pth_fracerror = new TH2F("pth_fracerror", "", nBinsX, 0, nBinsX, nBinsY, 0, nBinsY);
        pth_fracerror->GetXaxis()->SetLabelSize(0.04);
        pth_fracerror->GetYaxis()->SetLabelSize(0.05);
        pth_fracerror->GetYaxis()->SetTitleOffset(2);
        pth_fracerror->GetYaxis()->CenterTitle();
        pth_fracerror->SetMaximum(1.e-2);
        pth_fracerror->SetMinimum(1.e-7);

        pth_mean->GetXaxis()->SetTitle("Momentum [MeV/c]");
        pth_fracerror->GetXaxis()->SetTitle("Momentum [MeV/c]");
        pth_mean->GetZaxis()->SetTitle("Obs. Normalisation");
        pth_fracerror->GetZaxis()->SetTitle("Fractional Error");
        for(int binX = 1; binX <= nBinsX; ++binX)
        {
            pth_mean->GetXaxis()->SetBinLabel(binX, TString::Format("(%5.f,%5.f]", binning.mom[binX-1], binning.mom[binX]));
            pth_fracerror->GetXaxis()->SetBinLabel(binX, pth_mean->GetXaxis()->GetBinLabel(binX));
        }

        pth_fracerror->GetYaxis()->SetTitle("cos#theta");
        pth_mean->GetYaxis()->SetTitle("cos#theta");
        for(int binY = 1; binY <= nBinsY; ++binY)
        {
            pth_mean->GetYaxis()->SetBinLabel(binY, TString::Format("(%.3f,%.3f]", binning.cos[binY-1], binning.cos[binY]));
            pth_fracerror->GetYaxis()->SetBinLabel(binY, pth_mean->GetYaxis()->GetBinLabel(binY));
        }

        for(int binY = 1; binY <= nBinsY; ++binY)
        {
            for(int binX = 1; binX <= nBinsX; ++binX)
            {
                int globalbin = (startbin-1) + nBinsX*(binY-1) + binX;
                double nominal = Nominal->GetBinContent(globalbin);
                double variance = Total_Covariance_Matrix->GetBinContent(globalbin, globalbin);
                pth_mean->SetBinContent(binX, binY, (*Mean_Value)(globalbin));
                pth_fracerror->SetBinContent(binX, binY, TMath::Sqrt(variance/2000)/nominal);

            }
        }

        TH2F* hists[2] = {pth_mean, pth_fracerror};
        for(int canvas_index = 1; canvas_index <= 2; ++canvas_index)
        {
            canvas->cd(canvas_index);
            canvas->cd(canvas_index)->Clear();
            //canvas->cd(canvas_index)->SetLogz(canvas_index==2);
            canvas->cd(canvas_index)->SetLogz();
            canvas->cd(canvas_index)->SetRightMargin(0.15);
            canvas->cd(canvas_index)->SetBottomMargin(0.12);
            canvas->cd(canvas_index)->SetLeftMargin(0.19);
            hists[canvas_index-1]->GetZaxis()->SetTitleSize(1.25 * hists[canvas_index-1]->GetZaxis()->GetTitleSize());
            hists[canvas_index-1]->Draw("COLZ");
            legend->Draw();

            if(nBinsY > nBinsY_det || nBinsX > nBinsX_det)
            {
                for(int biny = 1; biny <= nBinsY; ++biny)
                {
                    for(int biny_det = 1; biny_det <= nBinsY_det; ++biny_det)
                    {
                        double v1 = binning.cos[biny];
                        double v2 = binning.cos_det[biny_det];
                        //printf("%f %f\n", v1, v2);
                        if(IsEqualAbs(v1, v2))
                        {
                            //printf("draw line\n");
                            double x1, x2, y1, y2;
                            x1 = hists[canvas_index-1]->GetXaxis()->GetBinLowEdge(1);
                            x2 = hists[canvas_index-1]->GetXaxis()->GetBinUpEdge(nBinsX);
                            y1 = hists[canvas_index-1]->GetYaxis()->GetBinUpEdge(biny);
                            y2 = y1;
                            TLine newline(x1, y1, x2, y2);
                            newline.SetLineStyle(2);
                            newline.SetLineWidth(3);
                            newline.DrawClone();
                            break;
                        }
                    }
                }
                for(int binx = 1; binx <= nBinsX; ++binx)
                {
                    for(int binx_det = 1; binx_det <= nBinsX_det; ++binx_det)
                    {
                        double v1 = binning.mom[binx];
                        double v2 = binning.mom_det[binx_det];
                        //printf("%f %f\n", v1, v2);
                        if(IsEqualAbs(v1, v2))
                        {
                            double x1, x2, y1, y2;
                            x1 = hists[canvas_index-1]->GetXaxis()->GetBinUpEdge(binx);
                            x2 = x1;
                            y1 = hists[canvas_index-1]->GetYaxis()->GetBinLowEdge(1);
                            y2 = hists[canvas_index-1]->GetYaxis()->GetBinUpEdge(nBinsY);
                            //printf("draw line\n");
                            //printf("%f %f %f %f\n", x1, y1, x2, y2);
                            TLine newline(x1, y1, x2, y2);
                            newline.SetLineStyle(2);
                            newline.SetLineWidth(3);
                            newline.DrawClone();
                            break;
                        }
                    }
                }
            }

        }

        startbin += nBinsX * nBinsY;
        //interface.SaveCanvasAs(canvas, binning.name);
        delete pth_mean;
        delete pth_fracerror;
        delete canvas;

    }

    TFile* newfile = new TFile("spreads.root", "RECREATE");
    newfile->cd();
    for(Int_t bin = startbin;  bin <= Total_Covariance_Matrix->GetNbinsX(); ++bin)
    {
        canvas = new TCanvas(TString::Format("Bin_%d_NEvent_Spread", bin), "", 800, 600);
        TCanvas* BinVariation = static_cast<TCanvas*>(BinVariations->Get(TString::Format("Bin_%d_NEvent_Spread", bin)));
        TH1D* BinVariation_Hist = static_cast<TH1D*>(BinVariation->GetPrimitive(TString::Format("Bin_%d_NEvent_Spread_Hist", bin)));
        TH1D* BinVariation_clone = new TH1D("BinVariation_Hist", "", BinVariation_Hist->GetNbinsX(), BinVariation_Hist->GetBinLowEdge(1), BinVariation_Hist->GetXaxis()->GetBinUpEdge(BinVariation_Hist->GetNbinsX()));
        for(Int_t clonebin = 1; clonebin <= BinVariation_clone->GetNbinsX(); ++clonebin)
        {
            BinVariation_clone->SetBinContent(clonebin, BinVariation_Hist->GetBinContent(clonebin));
        }

        Int_t modulus = 2;
        while(BinVariation_clone->GetNbinsX() > nMinBinsForVariations)
        {
            if(BinVariation_clone->GetNbinsX() % modulus == 0)
            {
                BinVariation_clone->Rebin(modulus);
            }
            else
            {
                ++modulus;
            }
        }
        BinVariation_clone->SetMaximum(1.1*BinVariation_clone->GetMaximum());
        BinVariation_clone->SetMinimum(0.9*BinVariation_clone->GetMinimum());
        const Double_t nominalValue = Nominal->GetBinContent(bin);

        TF1 *covariance_gauss = new TF1("covariance_gauss", "gaus", BinVariation_clone->GetBinLowEdge(1), BinVariation_clone->GetXaxis()->GetBinUpEdge(BinVariation_clone->GetNbinsX()));
        TF1 *fit_gauss = new TF1("fit_gauss", "gaus", BinVariation_clone->GetBinLowEdge(1), BinVariation_clone->GetXaxis()->GetBinUpEdge(BinVariation_clone->GetNbinsX()));
        //covariance_gauss->SetParameter(0, 10 * BinVariation_clone->Integral() / (sqrt(TMath::Pi()*2.)*sqrt(Total_Covariance_Matrix->GetBinContent(bin,bin))*Nominal->GetBinContent(bin)));
        fit_gauss->SetParameters(10 * BinVariation_clone->Integral(), Varied_Mean->GetBinContent(bin), sqrt(Total_Covariance_Matrix->GetBinContent(bin,bin))*Nominal->GetBinContent(bin));
        canvas->cd();
        BinVariation_clone->Fit(fit_gauss, "", "");
        covariance_gauss->SetParameter(0, fit_gauss->GetParameter(0));
        covariance_gauss->FixParameter(1, Nominal->GetBinContent(bin));
        covariance_gauss->FixParameter(2, sqrt(Total_Covariance_Matrix->GetBinContent(bin,bin))*Nominal->GetBinContent(bin));
        BinVariation_clone->Draw("E0");
        covariance_gauss->Draw("SAME");
        fit_gauss->Draw("SAME");
        canvas->Write();
        delete fit_gauss;
        delete covariance_gauss;
        delete BinVariation_clone;
        delete canvas;
    }
    newfile->Write();
    newfile->Close();

}

Int_t GetGlobalBin(TH2F* variance, Int_t startBin, Int_t binx, Int_t biny)
{
    return (startBin-1) + variance->GetXaxis()->GetNbins()*(biny-1) + binx;
}

Bool_t IsEqualAbs(Double_t v1, Double_t v2, Double_t delta)
{
    return fabs(v1-v2) < delta;
}
