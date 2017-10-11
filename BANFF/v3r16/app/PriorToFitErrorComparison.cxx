#include "TGraph.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TMath.h"
#include "TFile.h"
#include "TMatrixDSym.h"
#include "TAxis.h"

//An app for comparing the prior error on a parameter to its fitted error.
//Usage:PriorToFitErrorComparison.exe priorStartingParameterNumber fitStartingParameterNumber NParams priorFile covName fitFile outputFileName
int main(int argc, char** argv){

    if(argc < 8){
        std::cout << "Command line options are incorrect." << std::endl;
        std::cout << "Usage: PriorToFitErrorComparison.exe priorStartingParameterNumber fitStartingParameterNumber NParams priorFile covName fitFile outputFileName" << std::endl;
        return 1;
    }

    //Parameter number to start at.
    int priorStartingParam = atoi(argv[1]);
    int fitStartingParam = atoi(argv[2]);

    //These are parameters 0 to 50.
    int nParams = atoi(argv[3]);
    Double_t* priorErrors = new Double_t[nParams];
    Double_t* fittedErrors = new Double_t[nParams];
    Double_t* paramNumber = new Double_t[nParams];

    //Make the parameter number relative to its position in the fit.
    for(int i = 0; i < nParams; i++){
        paramNumber[i] = (Double_t)(i + fitStartingParam);
    }


    //Load the nominal covariance from the flux file.
    TFile* priorFile = new TFile(argv[4]);
    TMatrixDSym* priorCov = (TMatrixDSym*)(priorFile->Get(argv[5]));

    //Load the fitted covariance from the fit file.
    TFile* fitFile = new TFile(argv[6]);
    TMatrixDSym* fitCov = (TMatrixDSym*)(fitFile->Get("FittedCovariance0"));

    //Now loop through the parameters, filling the arrays.
    for(int i = 0; i < nParams; i++){

        int priorParam = i + priorStartingParam;
        int fitParam = i + fitStartingParam;

        priorErrors[i] = TMath::Sqrt((*priorCov)(priorParam,priorParam));
        fittedErrors[i] = TMath::Sqrt((*fitCov)(fitParam,fitParam));

    }

    TGraph* priorGraph = new TGraph(nParams,paramNumber,priorErrors);
    TGraph* fitGraph = new TGraph(nParams,paramNumber,fittedErrors);

    priorGraph->SetMarkerStyle(20);
    fitGraph->SetMarkerStyle(20);
    fitGraph->SetMarkerColor(kRed);
    fitGraph->SetLineColor(kRed);
    TCanvas* c1 = new TCanvas("errorcomparison");
    c1->cd();
    TMultiGraph *mg = new TMultiGraph();
    mg->SetTitle(";Parameter number;Error on parameter");
    mg->Add(priorGraph,"P");
    mg->Add(fitGraph,"P");
    mg->Draw("A");
    c1->SetGridx(1);
    //mg->GetXaxis()->SetNdivisions((-1.0*nParams/4));
    c1->SetGridy(1);
    c1->Modified();
    c1->Update();

    TLegend *leg = new TLegend(0.70,0.905,0.99,0.995);
    leg->SetFillColor(kWhite);
    leg->AddEntry(priorGraph, "Prior error", "P");
    leg->AddEntry(fitGraph, "Fitted error", "P");
    leg->Draw();

    c1->Print(argv[7]);

}
