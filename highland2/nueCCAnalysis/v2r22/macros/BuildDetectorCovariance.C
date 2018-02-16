/***********************************************************************
EXAMPLE to build detector covariance and make plots of the fractional 
uncertainty  as a function of momentum

Options:- 
filename1: Path to FGD1 file
filename2: Path to FGD2 file
(You can leave one empty if not interested in both FGDs)
outfile: Name of the output root file to save all histograms
IndividualSyst: Plot each individual systematic
save: Save the histograms in pdf and png
AngularRanges: Build detector covariance in angular ranges. 
In this case you have to redefine the binning in GetRegionsCov

You can chance the binning in the first few lines in BuildDetectorCovariance
************************************************************************/

#include <iostream>
#include <map>
#include <vector>

const Int_t NFGDs = 2;
const Int_t NMAXSYST = 40;

void BuildDetectorCovariance(TString filename1="fgd1file.root", TString filename2="fgd2file.root", TString oufile="det_syst.root", bool IndividualSyst = true, bool save = false, bool AngularRanges = false){

  gStyle->SetOptStat(0);

  // Define binning to plot individual systematics
  const Int_t nn = 11;
  Double_t parnum[nn] = {0,200,400,600,800,1200,1600,2000,3000,4000,5000};

  // Histograms
  TH2D* DetCov[NFGDs];
  TH1D* AllSyst_1D[NFGDs];
  TH1D* FGDsyst_histo[NFGDs][NMAXSYST];

  // Variation systematic - hard coded
  const Int_t varsyst = 8;
  std::string SelectSys[varsyst] = {"bfield_syst", "momscale_syst", "momresol_syst", "tpcpid_syst", "ecal_emresol_syst", "ecal_emscale_syst", "tof_resol_syst", "fgdpid_syst"};

  for(Int_t i = 0; i < NFGDs; i++){ // Two FGDs
    // Initialise histograms
    TString histname;
    histname.Form("CovHisto_FGD%i",i+1);
    AllSyst_1D[i] = new TH1D(histname.Data(), histname.Data(), nn-1, parnum);
    DetCov[i] = NULL;
    for(Int_t k = 0; k < NMAXSYST; k++){
      histname.Form("Syst_histo_FGD%i_%i",i+1,k);
      FGDsyst_histo[i][k] = new TH1D(histname.Data(),histname.Data(),nn-1,parnum);
    }
    
    // Access file
    TFile *FileMC = NULL;

    if(i == 0)
      FileMC  = new TFile(filename1.Data(), "READ");
    else if(i == 1)
      FileMC  = new TFile(filename2.Data(), "READ");

    // Skip if file not found
    if(!FileMC) continue;
    if(FileMC->GetNkeys() <= 0) continue;

    // Get the order of the weight systematics
    TTree *ConfigTree = (TTree*)FileMC->Get("config");
    std::map<std::string, Int_t> WeightSyst1 = GetWeightSyst(ConfigTree);
    const Int_t nWeightSyst1 =  WeightSyst1.size();
    std::cout << "INFO::Total number of weight systematics for FGD" << i+1 << " = " << nWeightSyst1 << std::endl;

    // Number of toys
    const Int_t Ntoys = GetNToys(ConfigTree);
    std::cout << "INFO::Number of toys = " << Ntoys << std::endl;
    
    // Do not count this systematics when the total systematic is evaluated
    Int_t nuflux_index  = IgnoreSystematic(ConfigTree, "FluxWeightNu");
    Int_t anuflux_index = IgnoreSystematic(ConfigTree, "FluxWeightAntiNu");

    // Get the default and systematics tree
    TTree *SystTree    = (TTree*)FileMC->Get("all_syst");
    //TTree *DefaultTree = (TTree*)FileMC->Get("default");
    
    if(AngularRanges){
      DetCov[i] = GetRegionsCov(SystTree, Ntoys, nWeightSyst1, nuflux_index, anuflux_index, i+1);
    }
    else{
      if(IndividualSyst){
	// Weight systematic
	std::map<std::string, Int_t>::iterator exp;
	for( exp = WeightSyst1.begin(); exp != WeightSyst1.end(); exp++ ){
	  Int_t syst = exp->second;
	  std::string syst_name = exp->first;
	  std::cout << "INFO::Running on systematic number " <<  syst << " , with name " << syst_name.c_str() << std::endl;
	  FGDsyst_histo[i][syst] = GetSingleSystHisto(SystTree, parnum, nn, Ntoys, nWeightSyst1, syst, syst_name, true, i+1);
	  FGDsyst_histo[i][syst]->SetTitle(syst_name.c_str());
	}
	
	// Variation systematic
	for(Int_t k = 0; k < varsyst; k++){
	  // Check if the variation syst tree exist
	  if(FileMC->GetListOfKeys()->Contains(SelectSys[k].c_str())){
	    std::cout << "INFO::Running on variation systematic " << SelectSys[k].c_str() << std::endl;
	    TTree *VarSysTree = (TTree*)FileMC->Get(SelectSys[k].c_str());
	    FGDsyst_histo[i][nWeightSyst1+k] = GetSingleSystHisto(VarSysTree, parnum, nn, Ntoys, 0, k, SelectSys[k], false, i+1);
	    FGDsyst_histo[i][nWeightSyst1+k]->SetTitle(SelectSys[k].c_str());
	    //if(VarSysTree)
	    //delete VarSysTree;
	  }
	}
	
      }

      // Total systematic and covariance
      std::cout << "INFO::Running on systematic: ALL" << std::endl;  
      DetCov[i] = GetALLSystCov(SystTree, parnum, nn, Ntoys, nWeightSyst1, nuflux_index, anuflux_index, i+1);
  
      for(Int_t ii = 0; ii <= DetCov[i]->GetNbinsX(); ii++){
	for(Int_t j = 0; j <= DetCov[i]->GetNbinsY(); j++){
	  if(ii == j){
	    AllSyst_1D[i]->SetBinContent(ii, DetCov[i]->GetBinContent(ii,j));
	  }
	}
      }

    }
  }

  TFile *f = new TFile(oufile.Data(), "RECREATE");

  for(Int_t i = 0; i < 2; i++){

    for(Int_t k = 0; k < NMAXSYST; k++){
      if(FGDsyst_histo[i][k]->GetEntries() == 0) continue;
      FGDsyst_histo[i][k]->GetXaxis()->SetTitle("p [MeV/c]");
      FGDsyst_histo[i][k]->GetYaxis()->SetTitle("Relative Error");
      FGDsyst_histo[i][k]->SetLineColor(i+1);
      FGDsyst_histo[i][k]->SetLineStyle(i+7);
      FGDsyst_histo[i][k]->Write();
    }
    
    if(AllSyst_1D[i]->GetEntries() > 0){
      AllSyst_1D[i]->SetTitle("All systematics");
      AllSyst_1D[i]->GetXaxis()->SetTitle("p [MeV/c]");
      AllSyst_1D[i]->GetYaxis()->SetTitle("Relative Error");
      AllSyst_1D[i]->SetLineColor(i+1);
      AllSyst_1D[i]->SetLineStyle(i+7);
      AllSyst_1D[i]->Write();
    }
    
    if(!DetCov[i]) continue;
    DetCov[i]->SetTitle("Detector Covariance");
    DetCov[i]->GetXaxis()->SetTitle("Bin");
    DetCov[i]->GetYaxis()->SetTitle("Bin");
    DetCov[i]->Write();
  }

  f->Close();

  if(save)
    SaveHistograms(oufile);
}

// --------------------------------------------------------
void SaveHistograms(TString filename){

  TFile *f = new TFile(filename.Data(), "READ");
  TList *list = f->GetListOfKeys();

  bool HasFGD1 = false;
  for(Int_t i = 0; i < list->GetEntries(); i++){
    std::string name = std::string(list->At(i)->GetName());
    if(name.find("FGD1") != std::string::npos){
      HasFGD1 = true;
      break;
    }
  }

  bool HasFGD2 = true;
  for(Int_t i = 0; i < list->GetEntries(); i++){
    std::string name = std::string(list->At(i)->GetName());
    if(name.find("FGD2") != std::string::npos){
      HasFGD2 = true;
      break;
    }
  }

  if(!HasFGD1 && !HasFGD2) return;

  TString plotname;
  if( (HasFGD1 && !HasFGD2) || (!HasFGD1 && HasFGD2) ){
    for(Int_t i = 0; i < list->GetEntries(); i++){
      std::string name = std::string(list->At(i)->GetName());
      if(name.find("AllSystCov") != std::string::npos){
	TH2D *h = (TH2D*)f->Get(name.c_str());
	TCanvas *c =  new TCanvas(name.c_str(), name.c_str());
	h->Draw("colz");

	plotname = TString(name.c_str()) + TString(".pdf");
	c->SaveAs(plotname.Data());
	plotname = TString(name.c_str()) + TString(".png");
	c->SaveAs(plotname.Data());

	continue;
      }
      // Histo
      TH1D *h1 = (TH1D*)f->Get(name.c_str());
      // Canvas
      TCanvas *c =  new TCanvas(str.c_str(), str.c_str());
      h1->Draw();
      // Legend
      TLegend *mylegend1 = new TLegend(0.50,0.55,0.7,0.81);
      mylegend1->SetFillColor(0); // white background
      mylegend1->SetBorderSize(0);
      mylegend1->SetTextSize(0.036);
      std::string lname;
      if(HasFGD1)
	lname = "FGD1";
      if(HasFGD2)
	lname = "FGD2";
      mylegend1->AddEntry(h1   , lname.c_str(),"l");
      mylegend1->Draw();

      plotname = TString(name.c_str()) + TString(".pdf");
      c->SaveAs(plotname.Data());
      plotname = TString(name.c_str()) + TString(".png");
      c->SaveAs(plotname.Data());
    }
  }

  if(HasFGD1 && HasFGD2){
    for(Int_t i = 0; i < list->GetEntries(); i++){
      std::string name = std::string(list->At(i)->GetName());

      if(name.find("AllSystCov") != std::string::npos){
	TH2D *h = (TH2D*)f->Get(name.c_str());
	TCanvas *c =  new TCanvas(name.c_str(), name.c_str());
	h->Draw("colz");

	plotname = TString(name.c_str()) + TString(".pdf");
	c->SaveAs(plotname.Data());
	plotname = TString(name.c_str()) + TString(".png");
	c->SaveAs(plotname.Data());

	continue;
      }

      if(name.find("FGD2") != std::string::npos) continue;

      std::size_t pos = name.find("_FGD");
      std::string str = name.substr(0,pos);
      std::string secname = "NULL";
      for(Int_t j = 0; j < list->GetEntries(); j++){
	std::string name2 = std::string(list->At(j)->GetName());
	if(name == name2) continue;
	if(name2.find(str.c_str()) != std::string::npos){
	  secname = name2;
	  break;
	}
      }

      // Histo
      TH1D *h1 = (TH1D*)f->Get(name.c_str());
      TH1D *h2 = NULL;
      if(secname != "NULL")
	h2 = (TH1D*)f->Get(secname.c_str());

      // Canvas
      TCanvas *c =  new TCanvas(str.c_str(), str.c_str());
      h1->Draw();
      if(h2)
	h2->Draw("same");

      // Legend
      TLegend *mylegend1 = new TLegend(0.50,0.55,0.7,0.81);
      mylegend1->SetFillColor(0); // white background
      mylegend1->SetBorderSize(0);
      mylegend1->SetTextSize(0.036);
      mylegend1->AddEntry(h1   , "FGD1", "l");
      mylegend1->AddEntry(h2   , "FGD2", "l");
      mylegend1->Draw();

      plotname = TString(str.c_str()) + TString(".pdf");
      c->SaveAs(plotname.Data());
      plotname = TString(str.c_str()) + TString(".png");
      c->SaveAs(plotname.Data());
    }
  }

  f->Close();

}

// --------------------------------------------------------
std::map<std::string, Int_t> GetWeightSyst(TTree *config){

  TClonesArray *arr = new TClonesArray("SystematicBase");
  config->SetBranchAddress("SYST",       &arr);

  std::map<std::string, Int_t> WeightSyst;

  for(Int_t i = 0; i < config->GetEntries(); i++){
    arr->Clear();
    config->GetEntry(i);
    
    Int_t n = arr->GetEntriesFast();

    Int_t enume = 0;
    for(Int_t j = 0; j < n; j++){
      SystematicBase* systbase = (SystematicBase*)arr->At(j);

      // Only weight systematics
      if(systbase->Type() == 0) continue;
      // Only enabled systematics
      if(!systbase->IsEnabled()) continue;

      WeightSyst[systbase->Name()] = enume;
      enume++;
    }
    break;
  }

  return WeightSyst;
}

// --------------------------------------------------------
Int_t IgnoreSystematic(TTree *config, std::string syst_name){

  TClonesArray *arr = new TClonesArray("SystematicBase");
  config->SetBranchAddress("SYST",       &arr);

  Int_t systnum = -1;
  for(Int_t i = 0; i < config->GetEntries(); i++){
    arr->Clear();
    config->GetEntry(i);
    
    Int_t n = arr->GetEntriesFast();

    Int_t enume = 0;
    for(Int_t j = 0; j < n; j++){
      SystematicBase* systbase = (SystematicBase*)arr->At(j);

      // Only weight systematics
      if(systbase->Type() == 0) continue;
      // Only enabled systematics
      if(!systbase->IsEnabled()) continue;

      if(systbase->Name() == syst_name){
	systnum = enume;
	break;
      }

      enume++;
    }
    break;
  }
   
  return systnum;
}

// --------------------------------------------------------
Int_t GetNToys(TTree *config){

  TClonesArray *arr = new TClonesArray("ConfigurationBase");
  config->SetBranchAddress("CONF",       &arr);
  
  Int_t Ntoys = -1;
  for(Int_t i = 0; i < config->GetEntries(); i++){
    arr->Clear();
    config->GetEntry(i);

    Int_t n = arr->GetEntriesFast();

    for(Int_t j = 0; j < n; j++){
      ConfigurationBase* confbase = (ConfigurationBase*)arr->At(j);
      if(confbase->GetNToys() == 1) continue;
      Ntoys = confbase->GetNToys();
      break;
    }
    break;
  }

  return Ntoys;
}

// --------------------------------------------------------
TH1D* GetSingleSystHisto(TTree *syst, Double_t* binning, const Int_t nbins, const Int_t NToys, const Int_t nWeights, Int_t syst_num, std::string syst_name, bool IsWeight, Int_t FGD){
 
  Float_t selelec_mom[NToys];
  Float_t weight_syst[NToys][nWeights], weight_syst_total[NToys];
  Int_t NTOYS;
  
  syst->SetBranchAddress("selelec_mom",       &selelec_mom);
  syst->SetBranchAddress("NTOYS",             &NTOYS);
  if(IsWeight){
    syst->SetBranchAddress("weight_syst",       &weight_syst);
    syst->SetBranchAddress("weight_syst_total", &weight_syst_total);
  }

  // Define histos
  TString histname;
  histname.Form("_FGD%i",FGD);
  histname = TString(syst_name.c_str()) + histname;
  TH1D* diffhistos1 = new TH1D(histname.Data(),histname.Data(),nbins-1,binning);
  
  TString histname;
  TH1D* histos1[NToys];
  for(Int_t k = 0; k < NToys; k++){
    histname.Form("histo1_syst_%i_%i_FGD_%i",syst_num,k,FGD);
    histos1[k]       = new TH1D(histname.Data(),histname.Data(),nbins-1,binning);
  }

  // Fill histograms
  for(Int_t i = 0; i < syst->GetEntries(); i++){
    syst->GetEntry(i);

    for(Int_t k = 0; k < NToys; k++){
      // Protection
      if(selelec_mom[k] <= 0.) continue;
      if(IsWeight){
	Double_t w1 = weight_syst[k][syst_num];
      
	// Protection against large weights 
	if(w1 > 100.0 || w1 <= 0.0) continue;
	
	histos1[k]->Fill(selelec_mom[k],w1);
      }
      else{
	histos1[k]->Fill(selelec_mom[k]);
      }

    }
  }
  
  // Get default number of events in each bin
  Double_t defevents1[nbins] = {0.0};
  Double_t allevents = 0.0;
  
  for(Int_t jj = 0; jj < nbins; jj++){

    for(Int_t k = 0; k < NToys; k++){
      Double_t toyevents = (Double_t)histos1[k]->GetBinContent(jj+1);
      defevents1[jj] += toyevents;
      allevents += toyevents;
    }

    defevents1[jj] = defevents1[jj]/(Double_t)(NToys);
  }

  allevents = allevents/(Double_t)(NToys);

  Double_t cov1[nbins] = {0.0};
  Double_t rms = 0.0;
  for(Int_t jj = 0; jj < nbins; jj++){
    for(Int_t k = 0; k < NToys; k++){
      Double_t toyevents1 = (Double_t)histos1[k]->GetBinContent(jj+1);
      if(defevents1[jj] != 0.0)
	cov1[jj] += (defevents1[jj] - toyevents1)*(defevents1[jj] - toyevents1) / ( defevents1[jj]*defevents1[jj]*NToys );
    }

    diffhistos1->SetBinContent( jj+1,sqrt(cov1[jj]) );
  }

  for(Int_t k = 0; k < NToys; k++){
    Double_t temp1 = 0.0; 
    for(Int_t jj = 0; jj < nbins; jj++){
      temp1 += (Double_t)histos1[k]->GetBinContent(jj+1);
    }

    if(allevents != 0)
      rms += (allevents - temp1)*(allevents - temp1) / ( allevents*allevents*NToys ); 
  }

  std::cout << "INFO::Total error is = " << sqrt(rms) << std::endl;

  for(Int_t k = 0; k < NToys; k++)
    delete histos1[k];

  return diffhistos1;

}

// --------------------------------------------------------
TH2D* GetRegionsCov(TTree *syst, const Int_t NToys, const Int_t nWeights, Int_t ignore_syst1, Int_t ignore_syst2, Int_t FGD){
 
  Float_t selelec_mom[NToys], selelec_costheta[NToys];
  Float_t weight_syst[NToys][nWeights], weight_syst_total[NToys];
  Int_t NTOYS;
  
  syst->SetBranchAddress("selelec_mom",       &selelec_mom);
  syst->SetBranchAddress("selelec_costheta",  &selelec_costheta);
  syst->SetBranchAddress("weight_syst",       &weight_syst);
  syst->SetBranchAddress("weight_syst_total", &weight_syst_total);
  syst->SetBranchAddress("NTOYS",             &NTOYS);

  const Int_t ann = 3;
  Double_t aparnum[ann] = {-1.0,0.72,1.0};

  const Int_t nn_1 = 3;
  Double_t parnum_1[nn_1] = {0,600,100000};

  const Int_t nn_2 = 6;
  Double_t parnum_2[nn_2] = {0,600,1100,1800,2700,100000};

  const Int_t allbins = nn_1 + nn_2 - 2;
  const Int_t NRegions = ann - 1;
  Int_t binregion[NRegions] = {nn_1, nn_2};

  TString histname;
  TH1D* histos1[NRegions][NToys];

  for(Int_t i=0; i < NRegions; i++){
    for(Int_t k = 0; k < NToys; k++){
      histname.Form("histo_syst_%i_region_%i_FGD_%i",k,i,FGD);
      if(i == 0)
	histos1[i][k] = new TH1D(histname.Data(), histname.Data(), nn_1-1, parnum_1);
      else if(i == 1)
	histos1[i][k] = new TH1D(histname.Data(), histname.Data(), nn_2-1, parnum_2);
    }
  }

  histname.Form("DetectorCovarianceRegions_FGD%i",FGD);
  TH2D* covhisto   = new TH2D(histname.Data(),  histname.Data(), allbins,0,allbins, allbins,0,allbins);

   // Fill histograms
  for(Int_t i = 0; i < syst->GetEntries(); i++){
    syst->GetEntry(i);

    for(Int_t j=0; j < NRegions; j++){
      for(Int_t k = 0; k < NToys; k++){
	// Protection
	if(selelec_mom[k] <= 0.) continue;
	
	Double_t w1 = 1.0;
	for(Int_t kk = 0; kk < nWeights; kk++){
	  if(kk == ignore_syst1 || kk == ignore_syst2) continue;
	  w1 *=  weight_syst[k][kk];
	}
	
	// Protection against large weights 
	if(w1 > 100.0 || w1 <= 0.0) continue;
      
	if(selelec_costheta[k] >= aparnum[j] && selelec_costheta[k] <=  aparnum[j+1])
	  histos1[j][k]->Fill(selelec_mom[k],w1);
      }
    }
  }
  
  
  Double_t defevents1[allbins] = {0.0};
  Double_t toyevents[allbins][NToys] = {{0.0}};
  Int_t counter = 0;
  for(Int_t i=0; i < NRegions; i++){
    for(Int_t jj = 0; jj < binregion[i]-1; jj++){

      for(Int_t k = 0; k < NToys; k++){
	Double_t toyevents1 = (Double_t)histos1[i][k]->GetBinContent(jj+1);
	defevents1[counter] += toyevents1;
	toyevents[counter][k] = histos1[i][k]->GetBinContent(jj+1);
      }
      
      defevents1[counter] = defevents1[counter]/(Double_t)(NToys);
      counter++;
    }
  }
  
  Double_t covmat[NToys][NToys] = {{0.0}};

  for(Int_t i = 0; i < allbins; i++){
    for(Int_t j = 0; j < allbins; j++){
      for(Int_t k = 0; k < NToys; k++){
	Double_t toyevents1 = toyevents[i][k];
	Double_t toyevents2 = toyevents[j][k];
	if(defevents1[i] != 0 && defevents1[j] != 0)
	  covmat[i][j] += (defevents1[i] - toyevents1)*(defevents1[j] - toyevents2) / ( defevents1[i]*defevents1[j]*NToys);
      } // ntoys
      Int_t covsign = 1;
      if(covmat[i][j] < 0){
	covsign = -1;
	covmat[i][j] = -covmat[i][j];
      }
      covhisto->SetBinContent(i+1,j+1,covsign*sqrt(covmat[i][j]));
    }
  }

  return covhisto;
  
}

// --------------------------------------------------------
TH2D* GetALLSystCov(TTree *syst, Double_t* binning, const Int_t nbins, const Int_t NToys, const Int_t nWeights, Int_t ignore_syst1, Int_t ignore_syst2, Int_t FGD){
 
  Float_t selelec_mom[NToys], selelec_costheta[NToys];
  Float_t weight_syst[NToys][nWeights], weight_syst_total[NToys];
  Int_t NTOYS; Int_t NWEIGHTSYST;
  
  syst->SetBranchAddress("selelec_mom",       &selelec_mom);
  syst->SetBranchAddress("selelec_costheta",  &selelec_costheta);
  syst->SetBranchAddress("weight_syst",       &weight_syst);
  syst->SetBranchAddress("weight_syst_total", &weight_syst_total);
  syst->SetBranchAddress("NTOYS",             &NTOYS);
  syst->SetBranchAddress("NWEIGHTSYST",       &NWEIGHTSYST);
  
  // Define histos
  TString histname;
  histname.Form("_FGD%i",FGD);
  histname = TString("AllSystCov") + histname;

  TH2D* covhisto   = new TH2D(histname.Data(),  histname.Data(), nbins-1,0,nbins-1, nbins-1,0,nbins-1);
  
  TString histname;
  TH1D* histos1[NToys];
  for(Int_t k = 0; k < NToys; k++){
    histname.Form("histo_syst_%i_FGD_%i",k,FGD);
    histos1[k]       = new TH1D(histname.Data(),histname.Data(),nbins-1,binning);
  }

  // Fill histograms
  for(Int_t i = 0; i < syst->GetEntries(); i++){
    syst->GetEntry(i);

    for(Int_t k = 0; k < NTOYS; k++){
      // Protection
      if(selelec_mom[k] <= 0.) continue;

      Double_t w1 = 1.0;
      for(Int_t kk = 0; kk < NWEIGHTSYST; kk++){
	if(kk == ignore_syst1 || kk == ignore_syst2) continue;
	w1 *=  weight_syst[k][kk];
      }
      
      // Protection against large weights 
      if(w1 > 100.0 || w1 <= 0.0) continue;
      
      histos1[k]->Fill(selelec_mom[k],w1);
    }
  }
  
  // Get default number of events in each bin
  Double_t defevents1[nbins] = {0.0};
  Double_t allevents = 0.0;
  for(Int_t jj = 0; jj < nbins; jj++){

    for(Int_t k = 0; k < NToys; k++){
      Double_t toyevents = (Double_t)histos1[k]->GetBinContent(jj+1);
      defevents1[jj] += toyevents;
    }

    defevents1[jj] = defevents1[jj]/(Double_t)(NToys);
    allevents += defevents1[jj];
  }

  Double_t rms = 0.0;
  for(Int_t k = 0; k < NToys; k++){
    Double_t temp = 0.0; 
    for(Int_t jj = 0; jj < nbins; jj++){
      temp += (Double_t)histos1[k]->GetBinContent(jj+1);
    }
    if(allevents != 0)
      rms += (allevents - temp)*(allevents - temp) / ( allevents*allevents*NToys );
  }

  std::cout << "INFO::Total error is = " << sqrt(rms) << std::endl;

  Double_t covmat[NToys][NToys] = {{0.0}};
  for(Int_t i = 0; i < nbins; i++){
    for(Int_t j = 0; j < nbins; j++){

      for(Int_t k = 0; k < NToys; k++){
	Double_t toyevents1 = (Double_t)histos1[k]->GetBinContent(i+1);
	Double_t toyevents2 = (Double_t)histos1[k]->GetBinContent(j+1);

	if(defevents1[i] != 0 && defevents1[j] != 0)
	  covmat[i][j] += (defevents1[i] - toyevents1)*(defevents1[j] - toyevents2) / ( defevents1[i]*defevents1[j]*NToys);
      } // ntoys
      
      Int_t covsign = 1;
      if(covmat[i][j] < 0){
	covsign = -1;
	covmat[i][j] = -covmat[i][j];
      }
      covhisto->SetBinContent(i+1,j+1,covsign*sqrt(covmat[i][j]));
    }
  }

  for(Int_t k = 0; k < NToys; k++)
    delete histos1[k];

  return covhisto;

}

// --------------------------------------------------------
TH1D* GetALLSystHisto(TTree *syst, Double_t* binning, const Int_t nbins, const Int_t NToys, const Int_t nWeights, Int_t ignore_syst1, Int_t ignore_syst2, Int_t FGD){
 
  Float_t selelec_mom[NToys];
  Float_t weight_syst[NToys][nWeights], weight_syst_total[NToys];
  Int_t NTOYS;
  
  syst->SetBranchAddress("selelec_mom",       &selelec_mom);
  syst->SetBranchAddress("weight_syst",       &weight_syst);
  syst->SetBranchAddress("weight_syst_total", &weight_syst_total);
  syst->SetBranchAddress("NTOYS",             &NTOYS);

  // Define histos
  TString histname;
  histname.Form("_%i",FGD);
  histname = TString("AllSyst") + histname;
  TH1D* diffhistos1 = new TH1D(histname.Data(),histname.Data(),nbins-1,binning);
  
  TString histname;
  TH1D* histos1[NToys];
  for(Int_t k = 0; k < NToys; k++){
    histname.Form("histo1_syst_%i_FGD_%i",k,FGD);
    histos1[k]       = new TH1D(histname.Data(),histname.Data(),nbins-1,binning);
  }

  // Fill histograms
  for(Int_t i = 0; i < syst->GetEntries(); i++){
    syst->GetEntry(i);

    for(Int_t k = 0; k < NToys; k++){
      // Protection
      if(selelec_mom[k] <= 0.) continue;

      Double_t w1 = 1.0;
      for(Int_t kk = 0; kk < nWeights; kk++){
	if(kk == ignore_syst1 || kk == ignore_syst2) continue;
	w1 *=  weight_syst[k][kk];
      }
      
      // Protection against large weights 
      if(w1 > 100.0 || w1 <= 0.0) continue;
      
      histos1[k]->Fill(selelec_mom[k],w1);
    }
  }
  
  // Get default number of events in each bin
  Double_t defevents1[nbins] = {0.0};
  Double_t allevents = 0.0;
  
  for(Int_t jj = 0; jj < nbins; jj++){

    for(Int_t k = 0; k < NToys; k++){
      Double_t toyevents = (Double_t)histos1[k]->GetBinContent(jj+1);
      defevents1[jj] += toyevents;
      allevents += toyevents;
    }

    defevents1[jj] = defevents1[jj]/(Double_t)(NToys);
  }

  allevents = allevents/(Double_t)(NToys);

  Double_t cov1[nbins] = {0.0};
  Double_t rms = 0.0;
  for(Int_t jj = 0; jj < nbins; jj++){
    for(Int_t k = 0; k < NToys; k++){
      Double_t toyevents1 = (Double_t)histos1[k]->GetBinContent(jj+1);
      if(defevents1[jj] != 0.0)
	cov1[jj] += (defevents1[jj] - toyevents1)*(defevents1[jj] - toyevents1) / ( defevents1[jj]*defevents1[jj]*NToys );
    }

    diffhistos1->SetBinContent( jj+1,sqrt(cov1[jj]) );
  }

  for(Int_t k = 0; k < NToys; k++){
    Double_t temp1 = 0.0; 
    for(Int_t jj = 0; jj < nbins; jj++){
      temp1 += (Double_t)histos1[k]->GetBinContent(jj+1);
    }

    if(allevents != 0)
      rms += (allevents - temp1)*(allevents - temp1) / ( allevents*allevents*NToys ); 
  }

  for(Int_t k = 0; k < NToys; k++)
    delete histos1[k];

  return diffhistos1;

}
