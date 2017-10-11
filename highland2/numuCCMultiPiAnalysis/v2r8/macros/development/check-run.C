#include <iostream>
#include "TMath.h"
#include "TRandom.h"
#include "TFile.h"
#include "TH1.h"
#include "TCanvas.h"

const Int_t cNBin = 20;
const Int_t cNSam = 2;
//const Double_t cPOTRatio = 1.00301/4.29779; //e19
const Double_t cPOTRatio = 24.1564/4.29779; //e19
//const Double_t cPOTRatio = 2.0;	// for testing
const Int_t cNToy = 200000;

TRandom2* gen;
TH1F * h_run5, *h_run6, *h_run5_w;

void GenerateRun5(TH1F * hist) {
	Double_t randnum = -1.;

	for (Int_t i=1; i<=cNBin*cNSam; i++) { //bin0=underflow
		Double_t mean = h_run5->GetBinContent(i); 
		randnum = gen->PoissonD(mean);
//		cout << " mean " << mean << " rand " << randnum << endl;
		hist->SetBinContent(i,randnum);
	}
	return hist;
}

void GenerateRun6(TH1F * hist) {
	Double_t randnum = -1.;

	for (Int_t i=1; i<=cNBin*cNSam; i++) { //bin0=underflow
//		Double_t mean = h_run4_w->GetBinContent(i); 
		Double_t mean = h_run5->GetBinContent(i)*cPOTRatio; //FIXME - test for different ratios
		randnum = gen->PoissonD(mean);
//		cout << " mean " << mean << " rand " << randnum << endl;
		hist->SetBinContent(i,randnum);
	}
	return hist;
}

Double_t CalculatePOTRatio(TH1F * h_old, TH1F * h_new) {
	Double_t oldBin = 0., newBin = 0.;
	Double_t fitPOTR = 0., sumnew = 0., sumold = 0.;

	for (Int_t i=1;i<=cNBin*cNSam; i++) {
		oldBin = h_old->GetBinContent(i);
		newBin = h_new->GetBinContent(i);
		sumold += oldBin;
		sumnew += newBin;
	}
	fitPOTR=sumnew/sumold;
//	cout << " Fitted R POT " << fitPOTR << " +- " << fitPOTR * sqrt(1./sumold + 1./sumnew) << " (true: " << cPOTRatio << ")\n";

	return fitPOTR;
}

Double_t CalculatePOTRatioPrint(TH1F * h_old, TH1F * h_new) {
	Double_t oldBin = 0., newBin = 0.;
	Double_t fitPOTR = 0., sumnew = 0., sumold = 0.;

	for (Int_t i=1;i<=cNBin*cNSam; i++) {
		oldBin = h_old->GetBinContent(i);
		newBin = h_new->GetBinContent(i);
		sumold += oldBin;
		sumnew += newBin;
	}
	fitPOTR=sumnew/sumold;
	cout<<"sumnew "<<sumnew<<" sumold "<<sumold<< "\n";
	cout << " Fitted R POT " << fitPOTR << " +- " << fitPOTR * sqrt(1./sumold + 1./sumnew) << " (true: " << cPOTRatio << ")\n";

	return fitPOTR;
}

Double_t CalculateLnull(TH1F * h_old, TH1F * h_new) {
	Double_t sum = 0.;
	Double_t alpha = 0.;
	Double_t oldBin = 0., newBin = 0.;

	for (Int_t i=1;i<=cNBin*cNSam; i++) {
		oldBin = h_old->GetBinContent(i);
		newBin = h_new->GetBinContent(i);
		alpha = (oldBin+newBin)/(1.+cPOTRatio);
		if (alpha<=0 || !newBin || !oldBin) continue;
		sum += ((-1.*alpha+oldBin*TMath::Log(alpha)) + (-1.*cPOTRatio*alpha + newBin*TMath::Log(alpha*cPOTRatio)));
//		cout << " Lnull: bin " << i << " run4 " << oldBin << " run5 " << newBin << " sum " << sum << endl;
	}
//	cout << " Lnull: " << sum << endl;
	return sum;
}

Double_t CalculateLindep(TH1F * h_old, TH1F * h_new) {
	Double_t sum = 0.;
	Double_t oldBin = 0., newBin = 0.;

	for (Int_t i=1;i<=cNBin*cNSam; i++) {
		oldBin = h_old->GetBinContent(i);
		newBin = h_new->GetBinContent(i);
		if (!newBin || !oldBin) continue;
		sum += ((-1.*oldBin+oldBin*TMath::Log(oldBin)) + (-1.*newBin + newBin*TMath::Log(newBin)));
//		cout << " Lindep: bin " << i << " run4 " << oldBin << " run5 " << newBin << " sum " << sum << endl;
	}
//	cout << " Lindep: " << sum << endl;	
	return sum;
}

Double_t CalculateDelta(TH1F * h_old, TH1F * h_new) {
	Double_t delta = -2.*(CalculateLnull(h_old, h_new) - CalculateLindep(h_old,h_new));
	return delta;
}

void checkrun() {
	h_run5 = new TH1F("h_run5","Dist. for run5", Int_t (cNSam*cNBin),0.,cNSam*cNBin);
	h_run6 = new TH1F("h_run6","Dist. for run6", Int_t (cNSam*cNBin),0.,cNSam*cNBin);
	if (cNSam == 3) {
	    TFile * f_input = new TFile("hist3.root","READ");
	    TFile * f_output = new TFile("result3.root","RECREATE");
	}
	else if (cNSam == 2) {
	    TFile * f_input = new TFile("hist2.root","READ");
	    TFile * f_output = new TFile("result2.root","RECREATE");
	}
	else cout << "No input files!\n";

	h_run5 = (TH1F*)f_input->Get("run5");
	h_run6 = (TH1F*)f_input->Get("run6");
	h_run5_w = new TH1F(*h_run5);
	h_run5->Sumw2();
	h_run6->Sumw2();
	h_run5_w->Sumw2();
	h_run5_w->Scale(cPOTRatio);
	//check the histograms
	TCanvas * c1 = new TCanvas("c1", "Input histograms", 900, 300);
	c1->Divide(3);
	c1->cd(1); h_run5->Draw();
	c1->cd(2); h_run5_w->Draw();
	c1->cd(3); h_run6->Draw();

	gen = new TRandom2();
	gen->SetSeed(0);

	Double_t fPOTRatio = CalculatePOTRatioPrint(h_run5, h_run6);
	Double_t dLnull = CalculateLnull(h_run5, h_run6);
	Double_t dLindep = CalculateLindep(h_run5, h_run6);
	Double_t deltaRun6 = -2.*(dLnull-dLindep);//CalculateDelta(h_run4, h_run5);
	cout << " Lnull " << dLnull << " Lindep " << dLindep << " delta " << deltaRun6 << endl;

	// Toy MC

	TH1F * h_POTRatio = new TH1F("potr","Fitted POT Ratio for Toy MC;POT Ratio",100,4.5,7.);
	//TH1F * h_POTRatio = new TH1F("potr","Fitted POT Ratio for Toy MC;POT Ratio",100,1.4,2.8);
	TH1F * h_Delta = new TH1F("delta","2DeltaLL for Toy MC;2DeltaLL",100,0.,100.);
	TH1F * h_Lnull = new TH1F("lnull","Lnull;Lnull",100,0.0,50000.);
	TH1F * h_Lindep = new TH1F("lindep","Lindep;Lindep",100,0.0,50000.);

	Double_t higherRPOT = 0., higherDelta = 0.;

	for (Int_t iToy=0; iToy<cNToy ; iToy++) {
		TH1F * dist5 = new TH1F("test5","", Int_t (cNSam*cNBin),0.,cNSam*cNBin);
		TH1F * dist6 = new TH1F("test6","", Int_t (cNSam*cNBin),0.,cNSam*cNBin);
		GenerateRun5(dist5);
		GenerateRun6(dist6);
/*		if (iToy<9) {
			c2->cd(iToy+1);
			dist4->DrawCopy();
			dist5->DrawCopy("same");
		}
*/		Double_t fitRPOT = CalculatePOTRatio(dist5, dist6);
		Double_t Lnull = CalculateLnull(dist5, dist6);
		Double_t Lindep = CalculateLindep(dist5, dist6);
		Double_t deltaToy = -2.* (Lnull-Lindep); //CalculateDelta(dist4, dist5);
//		std::cout << "delta Toy " << iToy << " " << deltaToy << std::endl;
		h_POTRatio->Fill(fitRPOT);
		h_Delta->Fill(deltaToy);
		h_Lnull->Fill(Lnull);
		h_Lindep->Fill(Lindep);
		if (fitRPOT>fPOTRatio) higherRPOT++;
		if (deltaToy>deltaRun6) higherDelta++;
		delete dist5;
		delete dist6;
	}
//	TCanvas * c2 = new TCanvas("c2", "Toy MC histograms", 900, 900);
//	c2->Divide(3,3);
	TCanvas * c3 = new TCanvas("c3", "Toy MC RPOT and Delta distributions", 600, 300);
	c3->Divide(2,1);

	c3->cd(1); h_POTRatio->Draw();
	Float_t ymax = h_POTRatio->GetMaximum();
	TLine *line1 = new TLine(fPOTRatio,0.,fPOTRatio,ymax);
	line1->SetLineColor(kRed);
	line1->Draw();
	c3->cd(2); h_Delta->Draw();
	ymax = h_Delta->GetMaximum();
	TLine *line2 = new TLine(deltaRun6,0.,deltaRun6,ymax);
	line2->SetLineColor(kRed);
	line2->Draw();
//	c3->cd(3); h_Lnull->Draw();
//	c3->cd(4); h_Lindep->Draw();
	cout << " \% of higher POT Ratios: " << higherRPOT << " / " << cNToy << " = " << higherRPOT/cNToy << endl;;
	cout << "Delta for run 6 " << deltaRun6 << endl;
	cout << " \% of higher Deltas " << higherDelta << " / " << cNToy << " = " << higherDelta/cNToy << endl;

	f_output->Write();
//	f_input->Close();
//	f_output->Close();
}
