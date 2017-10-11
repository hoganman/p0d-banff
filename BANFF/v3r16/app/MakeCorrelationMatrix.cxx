#include "TMath.h"
#include "TFile.h"
#include "TMatrixDSym.h"
#include "TH2D.h"
#include "TAxis.h"
#include "TColor.h"
#include "TCanvas.h"
#include "TStyle.h"
#include <iostream>

//OutputFileName is a pdf file.
//Usage:MakeCorrelationMatrix.exe inputFileName CovarianceMatrixName outputFileName 
int main(int argc, char** argv){

    if(argc < 4){
        std::cout << "Command line options are incorrect." << std::endl;
        std::cout << "Usage: MakeCorrelationMatrix.exe inputfile.root CovarianceMatrixName outputfile.pdf" << std::endl;
        return 1;
    }

    //Set a good palette.
    gStyle->SetPalette(1,0);  // use the nice blue->white->red palette
    const Int_t NRGBs = 3;
    const Int_t NCont = 255;

    Double_t stops[NRGBs] = { 0.00, 0.5, 1.00 };
    Double_t red[NRGBs]   = { 0.00, 1.0, 1.00 };
    Double_t green[NRGBs] = { 0.00, 1.0, 0.00 };
    Double_t blue[NRGBs]  = { 1.00, 1.0, 0.00 };
    TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    gStyle->SetNumberContours(NCont);
    gStyle->SetOptStat(0);


    TFile* covFile = new TFile(argv[1]);
    TMatrixDSym* cov = (TMatrixDSym*)(covFile->Get(argv[2]));

    //Extract the diagonal entries for the covariance matrix to use to make the
    //correlation matrix (corr(x,y) = cov(x,y)/((sigma_x)*(sigma_y))).
    double* sqrtDiag = new double[cov->GetNrows()];

    for(int i = 0; i < cov->GetNrows(); i++){

        sqrtDiag[i] = TMath::Sqrt((*cov)(i,i));

    }

    //Now make a new TMatrixDSym to store the correlation matrix in.
    TMatrixDSym* corr = new TMatrixDSym(cov->GetNrows());

    for(int i = 0; i < cov->GetNrows(); i++){

        for(int j = 0; j < cov->GetNrows(); j++){

           (*corr)(i,j) = (*cov)(i,j)/(sqrtDiag[i]*sqrtDiag[j]);
        }
    }

    //Now, convert corr into a TH2D for better draw options.
    TH2D* corrHist = new TH2D(*corr);
    corrHist->GetXaxis()->SetTitle("Parameter number");
    corrHist->GetYaxis()->SetTitle("Parameter number");
    corrHist->GetZaxis()->SetTitle("Correlation");
    corrHist->GetZaxis()->SetRangeUser(-1.0,1.0);

    gStyle->SetPadRightMargin(0.15);
    TCanvas* c1 = new TCanvas();
    c1->cd();

    corrHist->Draw("ZCOL");
    c1->SaveAs(argv[3]);
}
