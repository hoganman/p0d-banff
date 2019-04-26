#include"TFile.h"
#include"TString.h"
#include"TSystem.h"
#include"TChain.h"
#include"TMath.h"
#include"TCut.h"
#include "TH1.h"
#include "TF1.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TString.h"
#include <cstring>
#include <string>
#include <vector>
#include <exception>
#include "TMultiGraph.h"
#include "TGraphErrors.h"
//#include "P0DBANFFInterface.hxx"
#include "TLegend.h"

void P0DAgingAna(const char* fileDirectory);
Int_t langaupro(Double_t *params, Double_t &maxx, Double_t &FWHM);
TF1 *langaufit(TH1F *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF);
Double_t langaufun(Double_t *x, Double_t *par);
Double_t* LandauWidth;
Double_t* GaussWidth;
Double_t* MPV;
Double_t* Time;
TMultiGraph *mg;

void P0DAgingAna(const char* fileDirectory)
{
    TChain* P0DSandMuonHits = new TChain("P0DSandMuonHits", "");

    const Int_t Second = 1;
    const Int_t Minute = 60 * Second;
    const Int_t Hour = 60 * Minute;
    const Int_t Day = 24 * Hour;
    const Int_t Year = TMath::Nint(365.25 * Day);
    const Int_t SecondDivisions = TMath::Nint(Year / 12.);

    const Int_t firstRunNumber = 2555;
    const Int_t lastRunNumber = 14420;
    const Int_t firstSubRunNumber = 0;
    const Int_t lastSubRunNumber = 296;
    for(Int_t runNumber = firstRunNumber; runNumber < lastRunNumber; ++runNumber)
    {
        for(Int_t subrunNumber = firstSubRunNumber; subrunNumber < lastSubRunNumber; ++subrunNumber)
        {
            TString file = TString::Format("oa_nd_spl_000%05d-%04d_p0dmod_psmu_v11r31p15.root", runNumber, subrunNumber);
            const char* findResult = gSystem->FindFile(fileDirectory, file);
            if( !findResult )
                continue;
            printf("Adding %s\n", findResult);
            P0DSandMuonHits->Add(findResult);
        }
    }
    const Long64_t nEvents = P0DSandMuonHits->Draw("Time_Event>>htemp", "Time_Event > 0 && Alt$(Time_Event,-1)>0", "goff");
    TH1F* htemp = static_cast<TH1F*>(gDirectory->FindObject("htemp"));
    const Double_t T_0 = htemp->GetBinLowEdge(htemp->FindFirstBinAbove(0));
    const Double_t T_N = htemp->GetBinLowEdge(htemp->FindLastBinAbove(0)+1);
    const Int_t nDivisions = TMath::Nint(TMath::Ceil((T_N - T_0) / SecondDivisions));
    const std::string DrawFcn = "10*LightYieldPerMIPPerLength";

    Double_t t_min = T_0, t_max = t_min + SecondDivisions;
    printf("There are %d divisions to analyze\n", nDivisions);
    std::vector<Double_t> LandauWidth_Vect;
    std::vector<Double_t> LandauWidthEy_Vect;
    std::vector<Double_t> GaussWidth_Vect;
    std::vector<Double_t> GaussWidthEy_Vect;
    std::vector<Double_t> MPV_Vect;
    std::vector<Double_t> MPVEy_Vect;
    std::vector<Double_t> Time_Vect;
    std::vector<Double_t> TimeEx_Vect;
    UInt_t index = 0;

    while(t_max < T_N)
    {

        std::string histName = Form("hist_%d_%d", TMath::Nint(t_min), TMath::Nint(t_max));
        TH1F* hist = new TH1F(histName.c_str(), "", 120, 0, 225);
        std::string drawCmd = Form("%s>>%s", DrawFcn.c_str(), histName.c_str());
        const TCut time_range_cut(TString::Format("%f<Time_Event&&Time_Event<=%f", t_min, t_max));
        if (P0DSandMuonHits->Draw(drawCmd.c_str(), time_range_cut, "goff") < 10000)
        {
            t_min = t_max;
            t_max += SecondDivisions;
            ++index;
            continue;
        }

        // Setting fit range and start values
        Double_t fr[2];
        Double_t sv[4], pllo[4], plhi[4], fp[4], fpe[4];
        fr[0]=hist->GetBinLowEdge(1);
        fr[1]=150;

        pllo[0]=1.0; pllo[1]=0.0; pllo[2]=1.0; pllo[3]=1.;
        plhi[0]=50.0; plhi[1]=150.0; plhi[2]=100*hist->Integral(); plhi[3]=50.0;
        sv[0]=1.8; sv[1]=35.0; sv[2]=1e4; sv[3]=10.0;

        Double_t chisqr;
        Int_t    ndf;
        TF1 *fit = langaufit(hist,fr,sv,pllo,plhi,fp,fpe,&chisqr,&ndf);

        Double_t HitResponsePeak, HitResponseFWHM;
        langaupro(fp,HitResponsePeak,HitResponseFWHM);
        Double_t meantime = 0.5 * (t_max + t_min);

        LandauWidth_Vect.push_back(fit->GetParameter(0));
        LandauWidthEy_Vect.push_back(fit->GetParError(0));
        GaussWidth_Vect.push_back(fit->GetParameter(3));
        GaussWidthEy_Vect.push_back(fit->GetParError(3));
        MPV_Vect.push_back(fit->GetParameter(1));
        MPVEy_Vect.push_back(fit->GetParError(1));
        Time_Vect.push_back((meantime - T_0)/Year);
        TimeEx_Vect.push_back(0.5 * SecondDivisions/Year);

        delete fit;
        delete hist;
        t_min = t_max;
        t_max += SecondDivisions;
        ++index;
    }
    printf("Done analyzing\n");
    LandauWidth = &LandauWidth_Vect[0];
    GaussWidth = &GaussWidth_Vect[0];
    MPV = &MPV_Vect[0];
    Time = &Time_Vect[0];
    Double_t* LandauWidthEy = &LandauWidthEy_Vect[0];
    Double_t* GaussWidthEy = &GaussWidthEy_Vect[0];
    Double_t* MPVEy = &MPVEy_Vect[0];
    Double_t* TimeEx = &TimeEx_Vect[0];
    for(UInt_t entry = 0; entry < MPV_Vect.size(); ++entry)
    {
        printf("%9f: %5f %5f %5f\n", Time[entry], MPV[entry], GaussWidth[entry], LandauWidth[entry]);
    }
    UInt_t nGraphPoints = MPV_Vect.size();
    mg = new TMultiGraph();
    TGraphErrors* MPVgraph = new TGraphErrors(nGraphPoints, Time, MPV, TimeEx, MPVEy);
    MPVgraph->SetLineColor(P0DBANFFInterface::kcbBlue);
    MPVgraph->SetLineWidth(3);
    TGraphErrors* GaussWidthgraph = new TGraphErrors(nGraphPoints, Time, GaussWidth, TimeEx, GaussWidthEy);
GaussWidthgraph->SetLineColor(P0DBANFFInterface::kcbRed);
GaussWidthgraph->SetLineWidth(3);
    TGraphErrors* LandauWidthgraph = new TGraphErrors(nGraphPoints, Time, LandauWidth, TimeEx, LandauWidthEy);
LandauWidthgraph->SetLineColor(P0DBANFFInterface::kcbOrange);
LandauWidthgraph->SetLineWidth(3);
    mg->Add(MPVgraph);
    mg->Add(GaussWidthgraph);
    mg->Add(LandauWidthgraph);
    mg->Draw("A");
    mg->GetXaxis()->SetTitle("Time [Seconds]");
    mg->GetYaxis()->SetTitle("Bar Response [PEU / cm]");
    mg->Draw("AP");
    TF1* fitpol1 = new TF1("fitpol1", "pol1", Time_Vect[0], Time_Vect[Time_Vect.size()-1]);
    fitpol1->SetParameter(0, 37.6);
    fitpol1->SetParameter(1, -2.08662e-08);
    TLegend* l = new TLegend(0.4, 0.4, 0.89, 0.8, "");
    l->AddEntry(MPVgraph, "MPV", "LPE");
    l->AddEntry(LandauWidthgraph, "Landau Width", "LPE");
    l->AddEntry(GaussWidthgraph, "Gaussian Width", "LPE");
    //l->AddEntry((TObject*)0, "p_{0} [PEU / cm ] (%)       =  36.9 (100%)", "");
    //l->AddEntry((TObject*)0, "p_{1} [PEU / cm / Year] (%) = -0.66 (-1.78%)", "");
    MPVgraph->Fit(fitpol1);
    l->SetFillStyle(0);
    l->SetBorderSize(0);
    l->SetLineWidth(0);
    l->Draw();
}

Double_t langaufun(Double_t *x, Double_t *par) {

   //Fit parameters:
   //par[0]=Width (scale) parameter of Landau density
   //par[1]=Most Probable (MP, location) parameter of Landau density
   //par[2]=Total area (integral -inf to inf, normalization constant)
   //par[3]=Width (sigma) of convoluted Gaussian function
   //
   //In the Landau distribution (represented by the CERNLIB approximation),
   //the maximum is located at x=-0.22278298 with the location parameter=0.
   //This shift is corrected within this function, so that the actual
   //maximum is identical to the MP parameter.

      // Numeric constants
      const Double_t invsq2pi = 0.3989422804014;   // (2 pi)^(-1/2)
      const Double_t mpshift  = -0.22278298;       // Landau maximum location

      // Control constants
      Double_t np = 100.0;      // number of convolution steps
      Double_t sc =   5.0;      // convolution extends to +-sc Gaussian sigmas

      // Variables
      Double_t xx;
      Double_t mpc;
      Double_t fland;
      Double_t sum = 0.0;
      Double_t xlow,xupp;
      Double_t step;
      Double_t i;


      // MP shift correction
      mpc = par[1] - mpshift * par[0];

      // Range of convolution integral
      xlow = x[0] - sc * par[3];
      xupp = x[0] + sc * par[3];

      step = (xupp-xlow) / np;

      // Convolution integral of Landau and Gaussian by sum
      for(i=1.0; i<=np/2; i++) {
         xx = xlow + (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);

         xx = xupp - (i-.5) * step;
         fland = TMath::Landau(xx,mpc,par[0]) / par[0];
         sum += fland * TMath::Gaus(x[0],xx,par[3]);
      }

      return (par[2] * step * sum * invsq2pi / par[3]);
}



TF1 *langaufit(TH1F *his, Double_t *fitrange, Double_t *startvalues, Double_t *parlimitslo, Double_t *parlimitshi, Double_t *fitparams, Double_t *fiterrors, Double_t *ChiSqr, Int_t *NDF)
{
   // Once again, here are the Landau * Gaussian parameters:
   //   par[0]=Width (scale) parameter of Landau density
   //   par[1]=Most Probable (MP, location) parameter of Landau density
   //   par[2]=Total area (integral -inf to inf, normalization constant)
   //   par[3]=Width (sigma) of convoluted Gaussian function
   //
   // Variables for langaufit call:
   //   his             histogram to fit
   //   fitrange[2]     lo and hi boundaries of fit range
   //   startvalues[4]  reasonable start values for the fit
   //   parlimitslo[4]  lower parameter limits
   //   parlimitshi[4]  upper parameter limits
   //   fitparams[4]    returns the final fit parameters
   //   fiterrors[4]    returns the final fit errors
   //   ChiSqr          returns the chi square
   //   NDF             returns ndf

   Int_t i;
   Char_t FunName[100];

   sprintf(FunName,"Fitfcn_%s",his->GetName());

   TF1 *ffitold = (TF1*)gROOT->GetListOfFunctions()->FindObject(FunName);
   if (ffitold) delete ffitold;

   TF1 *ffit = new TF1(FunName,langaufun,fitrange[0],fitrange[1],4);
   ffit->SetParameters(startvalues);
   ffit->SetParNames("Width","MP","Area","GSigma");

   for (i=0; i<4; i++) {
      ffit->SetParLimits(i, parlimitslo[i], parlimitshi[i]);
   }

   his->Fit(FunName,"RB0");   // fit within specified range, use ParLimits, do not plot

   ffit->GetParameters(fitparams);    // obtain fit parameters
   for (i=0; i<4; i++) {
      fiterrors[i] = ffit->GetParError(i);     // obtain fit parameter errors
   }
   ChiSqr[0] = ffit->GetChisquare();  // obtain chi^2
   NDF[0] = ffit->GetNDF();           // obtain ndf

   return (ffit);              // return fit function

}


Int_t langaupro(Double_t *params, Double_t &maxx, Double_t &FWHM) {

   // Seaches for the location (x value) at the maximum of the
   // Landau-Gaussian convolute and its full width at half-maximum.
   //
   // The search is probably not very efficient, but it's a first try.

   Double_t p,x,fy,fxr,fxl;
   Double_t step;
   Double_t l,lold;
   Int_t i = 0;
   Int_t MAXCALLS = 10000;


   // Search for maximum

   p = params[1] - 0.1 * params[0];
   step = 0.05 * params[0];
   lold = -2.0;
   l    = -1.0;


   while ( (l != lold) && (i < MAXCALLS) ) {
      i++;

      lold = l;
      x = p + step;
      l = langaufun(&x,params);

      if (l < lold)
         step = -0.1*step;

      p += step;
   }

   if (i == MAXCALLS)
      return (-1);

   maxx = x;

   fy = 0.5*l;


   // Search for right x location of fy

   p = maxx + params[0];
   step = params[0];
   lold = -2.0;
   l    = -1e300;
   i    = 0;


   while ( (l != lold) && (i < MAXCALLS) ) {
      i++;

      lold = l;
      x = p + step;
      l = TMath::Abs(langaufun(&x,params) - fy);

      if (l > lold)
         step = -0.1*step;

      p += step;
   }

   if (i == MAXCALLS)
      return (-2);

   fxr = x;


   // Search for left x location of fy

   p = maxx - 0.5 * params[0];
   step = -params[0];
   lold = -2.0;
   l    = -1e300;
   i    = 0;

   while ( (l != lold) && (i < MAXCALLS) ) {
      i++;

      lold = l;
      x = p + step;
      l = TMath::Abs(langaufun(&x,params) - fy);

      if (l > lold)
         step = -0.1*step;

      p += step;
   }

   if (i == MAXCALLS)
      return (-3);


   fxl = x;

   FWHM = fxr - fxl;
   return (0);
}
