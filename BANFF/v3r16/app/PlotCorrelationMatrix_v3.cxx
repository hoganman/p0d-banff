#include "TH2.h"
#include "TPad.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TFile.h"
#include "TMatrixDSym.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TObjString.h"
#include "TObjArray.h"
#include <string>
#include <iostream>

void PlotCorrelationMatrix_v3() {
	gStyle->SetOptStat(0);
	gStyle->SetPaintTextFormat("4.2f");
	gStyle->SetTextFont(40);
	gROOT->SetBatch(kTRUE);

    TFile *inputFile = new TFile("../../pbs/outputs/postfit_MultiTrackRHC_v3r34_data_Runs2-6b_18Oct18_det_only.root");
	TObjArray*   params      = (TObjArray*)   (inputFile->Get("param_list"));
	TMatrixDSym* postfit_cov = (TMatrixDSym*) (inputFile->Get("postfit_cov"));
	TMatrixDSym* prefit_cov  = (TMatrixDSym*) (inputFile->Get("prefit_cov"));

    int n_xsec  =  31;
    int n_flux  = 100;
    int n_dete  = 556;
    int n_total = n_xsec + n_flux + n_dete;

    TH2D corr_xsec("XSec_Correlation_Matrix"    , "XSec Correlation Matrix"    ,n_xsec , 0, n_xsec , n_xsec , 0, n_xsec );
    TH2D corr_flux("Flux_Correlation_Matrix"    , "Flux Correlation Matrix"    ,n_flux , 0, n_flux , n_flux , 0, n_flux );
    TH2D corr_dete("Detector_Correlation_Matrix", "Detector Correlation Matrix",n_dete , 0, n_dete , n_dete , 0, n_dete );
    TH2D corr_pref("Prefit_Correlation_Matrix"  , "Prefit Correlation Matrix"  ,n_total, 0, n_total, n_total, 0, n_total);
    TH2D corr_post("Postfit_Correlation_Matrix" , "Postfit Correlation Matrix" ,n_total, 0, n_total, n_total, 0, n_total);
    TH2D cov_xsec("XSec_Covariance_Matrix"    , "XSec Covariance Matrix"    ,n_xsec , 0, n_xsec , n_xsec , 0, n_xsec );
    TH2D cov_flux("Flux_Covariance_Matrix"    , "Flux Covariance Matrix"    ,n_flux , 0, n_flux , n_flux , 0, n_flux );
    TH2D cov_dete("Detector_Covariance_Matrix", "Detector Covariance Matrix",n_dete , 0, n_dete , n_dete , 0, n_dete );
    TH2D cov_pref("Prefit_Covariance_Matrix"  , "Prefit Covariance Matrix"  ,n_total, 0, n_total, n_total, 0, n_total);
    TH2D cov_post("Postfit_Covariance_Matrix" , "Postfit Covariance Matrix" ,n_total, 0, n_total, n_total, 0, n_total);

    std::string binLabels[31];
	for (int i = n_flux+n_dete; i<n_total; i++){
		binLabels[i-n_flux-n_dete] = (((TObjString*)(params->At(i)))->GetString()).Data();
		//std::cout<<binLabels[i-n_flux-n_dete]<<std::endl;
	}

    double correlation = -999.;

    // Flux correlation matrix
    for (int i = 0; i < n_flux; i++) {
        for (int j = 0; j < n_flux; j++) {
            cov_flux.SetBinContent(cov_flux.GetBin(i+1,j+1), (*postfit_cov)(i,j));
            correlation = (*postfit_cov)(i,j) / sqrt((*postfit_cov)(i,i) * (*postfit_cov)(j,j));
            if (fabs(correlation) > 0.01) { corr_flux.SetBinContent(corr_flux.GetBin(i+1,j+1), correlation); }
        }
    }
    std::cout << "Finished flux" <<std::endl;

    // XSec correlation matrix
	for(int i = n_flux+n_dete; i < n_total; i++){
		for(int j = n_flux+n_dete; j < n_total; j++){
            cov_xsec.SetBinContent(cov_xsec.GetBin(i-n_flux-n_dete+1,j-n_flux-n_dete+1), (*postfit_cov)(i,j));
            correlation = (*postfit_cov)(i,j) / sqrt((*postfit_cov)(i,i) * (*postfit_cov)(j,j));
			if (fabs(correlation) > 0.01) { corr_xsec.SetBinContent(corr_xsec.GetBin(i-n_flux-n_dete+1,j-n_flux-n_dete+1), correlation); }
		}
		corr_xsec.GetXaxis()->SetBinLabel(corr_xsec.GetBin(i-n_flux-n_dete+1), binLabels[i-n_flux-n_dete].c_str());
		corr_xsec.GetYaxis()->SetBinLabel(corr_xsec.GetBin(i-n_flux-n_dete+1), binLabels[i-n_flux-n_dete].c_str());
		cov_xsec.GetXaxis()->SetBinLabel(cov_xsec.GetBin(i-n_flux-n_dete+1), binLabels[i-n_flux-n_dete].c_str());
		cov_xsec.GetYaxis()->SetBinLabel(cov_xsec.GetBin(i-n_flux-n_dete+1), binLabels[i-n_flux-n_dete].c_str());
	}
    std::cout << "Finished xsec" <<std::endl;

    // Detector correlation matrix
	for(int i = n_flux; i < n_flux+n_dete; i++){
		for(int j = n_flux; j < n_flux+n_dete; j++){
            cov_dete.SetBinContent(cov_dete.GetBin(i-n_flux+1,j-n_flux+1), (*postfit_cov)(i,j));
            correlation = (*postfit_cov)(i,j) / sqrt((*postfit_cov)(i,i) * (*postfit_cov)(j,j));
			if (fabs(correlation) > 0.01) { corr_dete.SetBinContent(corr_dete.GetBin(i-n_flux+1,j-n_flux+1), correlation); }
		}
	}
    std::cout << "Finished detector" <<std::endl;

    // Prefit and Postfit correlation matrices
    double correlation_pref = -999.;
    double correlation_post = -999.;
	for(int i = 0; i < n_total; i++){
		for(int j = 0; j < n_total; j++){
            cov_pref.SetBinContent(cov_pref.GetBin(i+1,j+1), (*prefit_cov)(i,j));
            cov_post.SetBinContent(cov_post.GetBin(i+1,j+1), (*postfit_cov)(i,j));
            correlation_pref = (*prefit_cov)(i,j)  / sqrt((*prefit_cov)(i,i)  * (*prefit_cov)(j,j) );
            correlation_post = (*postfit_cov)(i,j) / sqrt((*postfit_cov)(i,i) * (*postfit_cov)(j,j));
			if(fabs(correlation_pref) > 0.01) { corr_pref.SetBinContent(corr_pref.GetBin(i+1,j+1), correlation_pref); }
			if(fabs(correlation_post) > 0.01) { corr_post.SetBinContent(corr_post.GetBin(i+1,j+1), correlation_post); }
		}
	}
    std::cout << "Finished prefit and postfit" <<std::endl;

    TCanvas dummy("dummy","dummy",800,600);
    dummy.Print("v3_correlation_matrices.pdf[");

    TCanvas c0("c0","Prefit",800,600);
    cov_pref.GetXaxis()->SetRangeUser(0,n_total);
    cov_pref.GetYaxis()->SetRangeUser(0,n_total);
    cov_pref.GetZaxis()->SetRangeUser(-0.01, 0.01);
	cov_pref.GetXaxis()->SetTitle("Parameter Number");
	cov_pref.GetYaxis()->SetTitle("Parameter Number");
    cov_pref.Draw("COLZ");
    c0.Print("v3_correlation_matrices.pdf");

    TCanvas c1("c1","Prefit",800,600);
    corr_pref.GetXaxis()->SetRangeUser(0,n_total);
    corr_pref.GetYaxis()->SetRangeUser(0,n_total);
    corr_pref.GetZaxis()->SetRangeUser(-0.99, 1.00);
	corr_pref.GetXaxis()->SetTitle("Parameter Number");
	corr_pref.GetYaxis()->SetTitle("Parameter Number");
    corr_pref.Draw("COLZ");
    c1.Print("v3_correlation_matrices.pdf");

	TCanvas c2("c2","Postfit",800,600);
	cov_post.GetXaxis()->SetRangeUser(0,n_total);
	cov_post.GetYaxis()->SetRangeUser(0,n_total);
	cov_post.GetZaxis()->SetRangeUser(-0.01, 0.01);
	cov_post.GetXaxis()->SetTitle("Parameter Number");
	cov_post.GetYaxis()->SetTitle("Parameter Number");
  	cov_post.Draw("COLZ");
	c2.Print("v3_correlation_matrices.pdf");

	TCanvas c3("c3","Postfit",800,600);
	corr_post.GetXaxis()->SetRangeUser(0,n_total);
	corr_post.GetYaxis()->SetRangeUser(0,n_total);
	corr_post.GetZaxis()->SetRangeUser(-0.99, 1.00);
	corr_post.GetXaxis()->SetTitle("Parameter Number");
	corr_post.GetYaxis()->SetTitle("Parameter Number");
  	corr_post.Draw("COLZ");
	c3.Print("v3_correlation_matrices.pdf");

    TCanvas c4("c4","Flux Postfit",800,600);
    cov_flux.GetXaxis()->SetRangeUser(0,n_flux);
    cov_flux.GetYaxis()->SetRangeUser(0,n_flux);
    cov_flux.GetZaxis()->SetRangeUser(-0.01, 0.01);
	cov_flux.GetXaxis()->SetTitle("Parameter Number");
	cov_flux.GetYaxis()->SetTitle("Parameter Number");
    cov_flux.Draw("COLZ");
    c4.Print("v3_correlation_matrices.pdf");

    TCanvas c5("c5","Flux Postfit",800,600);
    corr_flux.GetXaxis()->SetRangeUser(0,n_flux);
    corr_flux.GetYaxis()->SetRangeUser(0,n_flux);
    corr_flux.GetZaxis()->SetRangeUser(-0.99, 1.00);
	corr_flux.GetXaxis()->SetTitle("Parameter Number");
	corr_flux.GetYaxis()->SetTitle("Parameter Number");
    corr_flux.Draw("COLZ");
    c5.Print("v3_correlation_matrices.pdf");

    TCanvas c6("c6","Detector Postfit",800,600);
    cov_dete.GetXaxis()->SetRangeUser(n_flux+n_xsec,n_total);
    cov_dete.GetYaxis()->SetRangeUser(n_flux+n_xsec,n_total);
    cov_dete.GetZaxis()->SetRangeUser(-0.01, 0.01);
	cov_dete.GetXaxis()->SetTitle("Parameter Number");
	cov_dete.GetYaxis()->SetTitle("Parameter Number");
    cov_dete.Draw("COLZ");
    c6.Print("v3_correlation_matrices.pdf");

    TCanvas c7("c7","Detector Postfit",800,600);
    corr_dete.GetXaxis()->SetRangeUser(n_flux+n_xsec,n_total);
    corr_dete.GetYaxis()->SetRangeUser(n_flux+n_xsec,n_total);
    corr_dete.GetZaxis()->SetRangeUser(-0.99, 1.00);
	corr_dete.GetXaxis()->SetTitle("Parameter Number");
	corr_dete.GetYaxis()->SetTitle("Parameter Number");
    corr_dete.Draw("COLZ");
    c7.Print("v3_correlation_matrices.pdf");

    TCanvas c8("c8","Xsec Postfit",800,600);
	c8.SetLeftMargin(0.17);
	c8.SetBottomMargin(0.2);
    cov_xsec.GetXaxis()->LabelsOption("v");
    cov_xsec.GetYaxis()->LabelsOption("v");
	cov_xsec.GetXaxis()->SetLabelSize(0.025);
	cov_xsec.GetYaxis()->SetLabelSize(0.025);
	cov_xsec.GetZaxis()->SetRangeUser(-0.01, 0.01);
	cov_xsec.Draw("COLZ");
	cov_xsec.SetMarkerSize(0.5);
	cov_xsec.Draw("TEXT,SAME");

	TPad *grid1 = new TPad("grid1","",0,0,1,1);
	grid1->SetLeftMargin(0.17);
	grid1->SetBottomMargin(0.2);
	grid1->Draw();
	grid1->cd();
	grid1->SetGrid();
	grid1->SetFillStyle(4000);
	grid1->SetFrameFillStyle(0);
	TH2 *hgrid1 = new TH2C("hgrid1","",31,0.,31.,31,0.,31.);
	hgrid1->Draw();
	hgrid1->GetXaxis()->SetNdivisions(31);
	hgrid1->GetYaxis()->SetNdivisions(31);
	hgrid1->GetYaxis()->SetLabelOffset(999.);
	hgrid1->GetXaxis()->SetLabelOffset(999.);
	c8.Print("v3_correlation_matrices.pdf");

    TCanvas c9("c9","Xsec Postfit",800,600);
    corr_xsec.GetXaxis()->LabelsOption("v");
    corr_xsec.GetYaxis()->LabelsOption("v");
	corr_xsec.GetXaxis()->SetLabelSize(0.025);
	corr_xsec.GetYaxis()->SetLabelSize(0.025);
	corr_xsec.GetZaxis()->SetRangeUser(-0.99, 1.0);
	c9.SetLeftMargin(0.17);
	c9.SetBottomMargin(0.2);
	corr_xsec.Draw("COLZ");
	corr_xsec.SetMarkerSize(0.5);
	corr_xsec.Draw("TEXT,SAME");

	TPad *grid2 = new TPad("grid2","",0,0,1,1);
	grid2->SetLeftMargin(0.17);
	grid2->SetBottomMargin(0.2);
	grid2->Draw();
	grid2->cd();
	grid2->SetGrid();
	grid2->SetFillStyle(4000);
	grid2->SetFrameFillStyle(0);
	TH2 *hgrid2 = new TH2C("hgrid2","",31,0.,31.,31,0.,31.);
	hgrid2->Draw();
	hgrid2->GetXaxis()->SetNdivisions(31);
	hgrid2->GetYaxis()->SetNdivisions(31);
	hgrid2->GetYaxis()->SetLabelOffset(999.);
	hgrid2->GetXaxis()->SetLabelOffset(999.);
	c9.Print("v3_correlation_matrices.pdf");

    dummy.Print("v3_correlation_matrices.pdf]");
}
