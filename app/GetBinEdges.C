#include "TFile.h"
#include "TCanvas.h"
#include "TH1D.h"
#include "TMath.h"


void GetBinEdges(const char* fileName, const char* canvasName="gCanvas", const char* histName="hNoErrors")
{
    TFile *_file0 = TFile::Open(fileName);
    TCanvas* Canvas = static_cast<TCanvas*>(_file0->Get(canvasName));
    Canvas->Draw();
    TH1D* hist = static_cast<TH1D*>(Canvas->GetPrimitive(histName));
    Int_t size = hist->GetNbinsX()+2;
    Double_t* binEdges = new Double_t[size];
    Double_t* cosbinEdges = new Double_t[size];
    Int_t* sortedIndices = new Int_t[size];
    for(Int_t bin = 1; bin <= hist->GetNbinsX()+2; ++bin){ binEdges[bin-1] = hist->GetXaxis()->GetBinLowEdge(bin); cosbinEdges[bin-1] = TMath::Cos(TMath::DegToRad()*binEdges[bin-1]);}
    TMath::Sort(size, cosbinEdges, sortedIndices, kFALSE);
    printf("Momentum (%d): ", size); for(UInt_t index = 0; index < size; ++index) {printf("%.3f, ", binEdges[index]);} printf("\n");
    printf("CosTheta (%d): ", size); for(UInt_t index = 0; index < size; ++index) {printf("%.3f, ", cosbinEdges[sortedIndices[index]]);} printf("\n");
    _file0->Close();
    delete binEdges;
    delete cosbinEdges;
    delete sortedIndices;
}
