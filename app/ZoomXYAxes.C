#include "TFile.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TMath.h"
#include "../src/P0DBANFFInterface.hxx"

void ZoomXYAxes(const char* fileCanvasHist0="fileName0,CanvasName0,HistName0", Double_t zoomX_low=0, Double_t zoomX_high=30000, Double_t zoomY_low=-1, Double_t zoomY_high=1)
{

    const UInt_t nFiles = 1;
    const UInt_t FILENAME = 0;
    const UInt_t CANVASNAME = 1;
    const UInt_t HISTNAME = 2;
    std::vector<TString> tuple0 = P0DBANFFInterface::SplitString(fileCanvasHist0, ',');
    std::vector<TString> allTuples[nFiles] = {tuple0};
    TString inputs[nFiles] = {fileCanvasHist0};
    for(UInt_t index = 0; index < nFiles; ++index)
    {
        if(inputs[index].Length() < 1 || allTuples[index][FILENAME].Length() < 1)
            continue;
        TFile* file = TFile::Open(allTuples[index][FILENAME].Data());
        if (!file)
        {
            printf("no file");
            continue;
        }
        TCanvas* Canvas = static_cast<TCanvas*>(file->Get(allTuples[index][CANVASNAME].Data()));
        if(!Canvas)
        {
            printf("no canvas");
            continue;
        }
        Canvas->Draw();
        TH2* hist = P0DBANFFInterface::GetTH2FromCanvas(Canvas, allTuples[index][HISTNAME].Data());
        if(!hist)
        {
            printf("no hist");
            continue;
        }
        hist->GetXaxis()->SetRangeUser(zoomX_low, zoomX_high);
        hist->GetYaxis()->SetRangeUser(zoomY_low, zoomY_high);
        gPad->Update();
        TString filename(allTuples[index][FILENAME]);
        filename.Remove(filename.Last('.'));
        filename.Append(TString::Format("xRange_%.3f_to_%.3f_yRange_%.3f_to_%.3f", zoomX_low, zoomX_high, zoomY_low, zoomY_high));
        filename.ReplaceAll(".", "_");
        P0DBANFFInterface::SaveCanvasAs(Canvas, filename.Data());
        file->Close();
    }
}
