
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include <TString.h>
#include <TArrayF.h>
#include <TFile.h>
#include <TChain.h>
#include <TTree.h>

using namespace std;

void WeightCheck(TString fnam1, TString fnam2="", TString fnamOut="check_weights.root", int iVerbose=0) {
  
  fnamOut.ReplaceAll(".root","");
  
  TArrayF *weights = NULL;
  TArrayF *weights2 = NULL;
  Int_t nweights=0,nweights2=0;
  
  int iErr=0;
  bool fScndFile=false;
  
  TChain trIn("weightstree");
  trIn.Add(fnam1.Data());
  cout << "File1: " << fnam1 << ", nevt=" << trIn.GetEntries() << endl;
  if (trIn.SetBranchAddress("weights", &weights)) iErr+=1;
  if (trIn.SetBranchAddress("nweights",&nweights)) iErr+=2;
  
  TChain tr2("weightstree");
  if (fnam2!="") {
    tr2.Add(fnam2.Data());
    cout << "File2: " << fnam2 << ", nevt=" << tr2.GetEntries() << endl;
    if (tr2.SetBranchAddress("weights", &weights2)) iErr+=4;
    if (tr2.SetBranchAddress("nweights",&nweights2)) iErr+=8;
    fScndFile=true;
  }
  
  if (iErr) {
    cout << "Wrong input file format! iErr=" << iErr << endl;
    exit(-1);
  }
  
  TObjArray *BranchList = trIn.GetListOfBranches();
  int nBranches = BranchList->GetEntries();
  
  TString *strSystName = new TString[nBranches];// name of the systematic parameters
  TArrayF **SystVal = new TArrayF*[nBranches];
  
  int nSyst=0;// # of systematic parameters
  cout << "List of systematic parameters:" << endl;
  for (int i=0; i<nBranches; i++) {
    TString tmpStr(BranchList->At(i)->GetName());
    if (tmpStr(0,2)=="br") {
      strSystName[nSyst]=tmpStr(2,tmpStr.Length());
      SystVal[nSyst]=NULL;
      trIn.SetBranchAddress(tmpStr.Data(),&SystVal[nSyst]);
      cout << "  " << strSystName[nSyst] << endl;
      nSyst++;
    }
  }
  cout << nSyst << " systematic parameters are reweighted." << endl << endl;
  
  int nevt = trIn.GetEntries();
  trIn.GetEntry(0);
  if (fScndFile) {
    int nevt_2 = tr2.GetEntries();
    tr2.GetEntry(0);
    if (nevt_2!=nevt) {
      cout << "# of events does not match between the two files!" << endl;
      exit(-1);
    }
    if (nweights2!=nweights) {
      cout << "# of weights does not match between the two files!" << endl;
      exit(-1);
    }
  }
  
  int nvars=nweights;
  cout << nvars << " systematic parameter variations:" << endl;
  for (int j=0; j<nvars; j++) {// loop over systematic variations
    cout << Form("  Var#%d:",j);
    for (int iSyst=0; iSyst<nSyst; iSyst++) {
      Float_t tmpVal = SystVal[iSyst]->At(j);//value of this parameter
      if (!(fabs(tmpVal)<1e-6)) {//the parameter is varied
        cout << Form(" %s->%f,",strSystName[iSyst].Data(),tmpVal);
      }
    }
    cout << endl;
  }
  cout << endl;
  
  TH1D *h_mismatch = new TH1D("hmismatch","# of events with mismatching weights",nvars,-0.5,nvars-0.5);
  TH1D *h_neg = new TH1D("hneg","# of events with weight<0",nvars,-0.5,nvars-0.5);
  TH1D *h_zero = new TH1D("hzero","# of events with weight=0",nvars,-0.5,nvars-0.5);
  TH1D *h_one = new TH1D("hone","# of events with weight=1",nvars,-0.5,nvars-0.5);
  TH1D *h_gt10 = new TH1D("hgt10","# of events with weight>10",nvars,-0.5,nvars-0.5);
  TH1D *h_gt100 = new TH1D("hgt100","# of events with weight>100",nvars,-0.5,nvars-0.5);
  TH1D *h_nan = new TH1D("hnan","# of events with weight=nan",nvars,-0.5,nvars-0.5);
  
  cout << "Looping over " << nevt << " events..." << endl << endl;
    
  for (int j=0; j<nvars; j++) {// loop over systematic variations
    
    if (iVerbose) cout << Form("Var#%d:",j) << endl;
    
    for (int i=0; i<nevt; i++) {
      trIn.GetEntry(i);
      
      if (fScndFile) {// check the consistency between the two files
        tr2.GetEntry(i);
        if (!(fabs(weights->At(j)-weights2->At(j))<=fabs(1e-6*weights2->At(j)))) {// weights don't match between two files
          if (iVerbose) cout << "  Event " << i << ": weights don't match!" << Form(" w1=%f, w2=%f",weights->At(j),weights2->At(j)) << endl;
          h_mismatch->Fill(j);
        }
      }
      
      double wgtVal = weights->At(j);
      
      if (wgtVal<0.) {
        if (iVerbose) cout << "  Event " << i << ": weight<0" << endl;
        h_neg->Fill(j);
      }
      else if (wgtVal==0.) {
        h_zero->Fill(j);
      }
      else if (wgtVal==1.) {
        h_one->Fill(j);
      }
      else if (wgtVal>10.) {
        h_gt10->Fill(j);
        if (wgtVal>100.) {
          h_gt100->Fill(j);
        }
      }
      else if (!(wgtVal<=10.)) {//nan
        if (iVerbose) cout << "  Event " << i << ": weight=nan" << endl;
        h_nan->Fill(j);
      }
      
    }
  }
  
  TFile *fout = new TFile(Form("%s.root",fnamOut.Data()),"RECREATE");
  
  TCanvas *c = new TCanvas("c1","c1",600,400);
  
  if (fScndFile) {
    h_mismatch->Draw();
    c->Print(Form("%s_mismatch.pdf",fnamOut.Data()));
    h_mismatch->Write();
  }
  
  h_neg->Draw();
  c->Print(Form("%s_neg.pdf",fnamOut.Data()));
  h_neg->Write();
  
  h_zero->Draw();
  c->Print(Form("%s_zero.pdf",fnamOut.Data()));
  h_zero->Write();
  
  h_one->Draw();
  c->Print(Form("%s_one.pdf",fnamOut.Data()));
  h_one->Write();
  
  h_gt10->Draw();
  c->Print(Form("%s_gt10.pdf",fnamOut.Data()));
  h_gt10->Write();
  
  h_gt100->Draw();
  c->Print(Form("%s_gt100.pdf",fnamOut.Data()));
  h_gt100->Write();
  
  h_nan->Draw();
  c->Print(Form("%s_nan.pdf",fnamOut.Data()));
  h_nan->Write();
  
  delete fout;
  
}
