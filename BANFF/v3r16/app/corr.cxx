#include "TH2.h"
#include "TPad.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TMatrixDSym.h"
#include<string>

void corr(){
	gStyle->SetOptStat(0);
	gStyle->SetPaintTextFormat("4.2f");
	gStyle->SetTextFont(40);
	gROOT->SetBatch(kTRUE);
	//TFile *_file0 = TFile::Open("SetT2KStyle.cxx");
	gROOT->ProcessLine(".L /data/lasorak/t2k/work/frozen/highland2/highlandTools/v2r18/src/SetT2KStyle.H");
	SetT2KStyle(1);
	//TFile *_file0 = TFile::Open("Fit_PostFit_170123_OldEb.root");
	//TFile *_file0 = TFile::Open("Asimov_PostFit_20170123_CCQENORM.root");
	TFile* inputFile = new TFile("CC4piMultiPi_MultiPi_SK.root");
	TH2D corr("Total_Correlation_Matrix","Correlation Matrix",31, 0, 31, 31, 0, 31);
	TH2D corr_pre("Prefit_Correlation_Matrix","Prefit Correlation Matrix",131, 0, 131, 131, 0, 131);
	TH2D corr_post("Postfit_Correlation_Matrix","Postfit Correlation Matrix",131, 0, 131, 131, 0, 131);
	string binLabels[31];
	TObjArray* params = (TObjArray*)(inputFile->Get("param_list"));
	TMatrixDSym* postfit_cov = (TMatrixDSym*)(inputFile->Get("postfit_cov"));
	TMatrixDSym* prefit_cov = (TMatrixDSym*)(inputFile->Get("prefit_cov"));
	for (int i = 100; i<131; i++){
		binLabels[i-100] = (((TObjString*)(params->At(i)))->GetString()).Data();
		std::cout<<binLabels[i-100]<<std::endl;
	}
	for(int i = 100; i<131; i++){
		for(int j = 100; j<131; j++){
			if(fabs((*postfit_cov)(i,j)/sqrt((*postfit_cov)(i,i)*(*postfit_cov)(j,j)))>0.01){
				corr.SetBinContent(corr.GetBin(i-99,j-99),(*postfit_cov)(i,j)/sqrt((*postfit_cov)(i,i)*(*postfit_cov)(j,j)));
			}
		}
		corr.GetXaxis()->SetBinLabel(corr.GetBin(i-99),binLabels[i-100].c_str());
		std::cout<<binLabels[i-100]<<std::endl;
		corr.GetYaxis()->SetBinLabel(corr.GetBin(i-99),binLabels[i-100].c_str());
	}
	for(int i = 0; i<131; i++){
		for(int j = 0; j<131; j++){
			if(fabs((*prefit_cov)(i,j)/sqrt((*prefit_cov)(i,i)*(*prefit_cov)(j,j)))>0.01){
				corr_pre.SetBinContent(corr_pre.GetBin(i,j),(*prefit_cov)(i,j)/sqrt((*prefit_cov)(i,i)*(*postfit_cov)(j,j)));
			}
			if(fabs((*postfit_cov)(i,j)/sqrt((*postfit_cov)(i,i)*(*postfit_cov)(j,j)))>0.01){
				corr_post.SetBinContent(corr_post.GetBin(i,j),(*postfit_cov)(i,j)/sqrt((*postfit_cov)(i,i)*(*postfit_cov)(j,j)));
			}
		}
	}
	TCanvas *c2 = new TCanvas("c1","Hop",10,10,1000,900);
	corr_post.GetXaxis()->SetRangeUser(0,131);
	corr_post.GetYaxis()->SetRangeUser(0,131);
	corr_post.GetXaxis()->SetTitle("Parameter Number");
	corr_post.GetYaxis()->SetTitle("Parameter Number");
	corr_post.GetZaxis()->SetRangeUser(-0.99, 1.0);
  	corr_post.Draw("COLZ");
	c2->SaveAs("corr_post.pdf");
	
	corr_pre.GetXaxis()->SetRangeUser(0,131);
	corr_pre.GetYaxis()->SetRangeUser(0,131);
	corr_pre.GetZaxis()->SetRangeUser(-0.99, 1.0);
	corr_pre.GetXaxis()->SetTitle("Parameter Number");
	corr_pre.GetYaxis()->SetTitle("Parameter Number");
  	corr_pre.Draw("COLZ");
	c2->SaveAs("corr_pre.pdf");
	
	corr.GetXaxis()->LabelsOption("v");
	corr.GetYaxis()->LabelsOption("v");
	corr.GetXaxis()->SetLabelSize(0.025);
	corr.GetYaxis()->SetLabelSize(0.025);
	corr.GetZaxis()->SetRangeUser(-0.99, 1.0);
	TCanvas *c1 = new TCanvas("c1","Hop",10,10,1000,900);
	c1->SetLeftMargin(0.17);
	c1->SetBottomMargin(0.2);
	corr.Draw("COLZ");
	corr.SetMarkerSize(0.5);
	corr.Draw("TEXT,SAME");
	
	TPad *grid = new TPad("grid","",0,0,1,1);
	grid->SetLeftMargin(0.17);
	grid->SetBottomMargin(0.2);
	grid->Draw();
	grid->cd();
	grid->SetGrid();
	grid->SetFillStyle(4000);
	grid->SetFrameFillStyle(0);
	TH2 *hgrid = new TH2C("hgrid","",31,0.,31.,31,0.,31.);
	hgrid->Draw();
	hgrid->GetXaxis()->SetNdivisions(31);
	hgrid->GetYaxis()->SetNdivisions(31);
	hgrid->GetYaxis()->SetLabelOffset(999.);
	hgrid->GetXaxis()->SetLabelOffset(999.);
	
	c1->SaveAs("corr.pdf");
	
}
