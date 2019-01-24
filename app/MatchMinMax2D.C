#include "TFile.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TMath.h"
#include "../src/P0DBANFFInterface.hxx"

void MatchMinMax2D(const char* fileCanvasHist0="fileName0,CanvasName0,HistName0", const char* fileCanvasHist1="fileName1,CanvasName1,HistName1", const char* fileCanvasHist2="", const char* fileCanvasHist3="")
{

    const UInt_t nFiles = 4;
    const UInt_t FILENAME = 0;
    const UInt_t CANVASNAME = 1;
    const UInt_t HISTNAME = 2;
    std::vector<TString> tuple0 = P0DBANFFInterface::SplitString(fileCanvasHist0, ',');
    std::vector<TString> tuple1 = P0DBANFFInterface::SplitString(fileCanvasHist1, ',');
    std::vector<TString> tuple2 = P0DBANFFInterface::SplitString(fileCanvasHist2, ',');
    std::vector<TString> tuple3 = P0DBANFFInterface::SplitString(fileCanvasHist3, ',');
    std::vector<TString> allTuples[nFiles] = {tuple0, tuple1, tuple2, tuple3};
    TString inputs[nFiles] = {fileCanvasHist0, fileCanvasHist1, fileCanvasHist2, fileCanvasHist3};

    Double_t min = 1e99;
    Double_t max = -1;
    for(UInt_t index = 0; index < nFiles; ++index)
    {
        if(inputs[index].Length() < 1)
            continue;
        if(allTuples[index][FILENAME].Length() < 1)
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
        printf("max: %.3f\n", hist->GetMaximum());
        if(hist->GetMaximum() > max)
            max = hist->GetMaximum();
        if(hist->GetMinimum() < min)
            min = hist->GetMinimum();
        file->Close();
    }
    printf("min: %.3f\nmax: %.3f\n", min, max);
    for(UInt_t index = 0; index < nFiles; ++index)
    {
        if(inputs[index].Length() < 1)
            continue;
        if(!P0DBANFFInterface::IsGoodFile(allTuples[index][FILENAME].Data()))
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
        hist->SetMaximum(max);
        hist->SetMinimum(min);
        Canvas->cd();
        hist->Draw("COLZ");
        TString filename(allTuples[index][FILENAME]);
        filename.Remove(filename.Last('.'));
        printf("file: %s\n", filename.Data());
        P0DBANFFInterface::SaveCanvasAs(Canvas, filename.Data());
        file->Close();
    }
}
