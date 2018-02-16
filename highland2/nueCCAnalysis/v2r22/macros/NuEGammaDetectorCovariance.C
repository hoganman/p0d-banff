/***********************************************************************
EXAMPLE to build detector covariance including the migration between
samples

Options:- 
filename1: Path to nue file
filename2: Path to gamma file
filename3: Path to file containing migrating events between the nue/gamma samples
(You can leave filename3 empty if you don't have the migrating sample)
Important: Keep the order of the files above, otherwise the covariance will be wrong!!

outfile: Name of the output root file to save all histograms
AngularRanges: Build detector covariance in angular ranges. 
In this case you have to redefine the binning in GetRegionsCov and in GetmigrationRegionsCov

You can chance the default binning in the first few lines in NuEGammaDetectorCovariance
************************************************************************/


#include <iostream>
#include <map>
#include <vector>

const Int_t NFGDs = 2;
const Int_t NMAXSYST = 40;

void NuEGammaDetectorCovariance(TString filename1="nuefile.root", TString filename2="gammafile.root", TString filename3="nuegammacorrfile.root", TString outfile="fhc_fulldet_syst", bool AngularRanges = false){

  gStyle->SetOptStat(0);

  // Define binning to plot individual systematics
  const Int_t nn = 6;
  Double_t parnum[nn] = {0,600,1100,1800,2700,10000};

  // Histograms
  TH2D* DetCov[4];
  for(Int_t i = 0; i < 4; i++)
    DetCov[i] = NULL;

  for(Int_t i = 0; i < 3; i++){ // Three samples

    // Access file
    TFile *FileMC = NULL;

    if(i == 0)
      FileMC  = new TFile(filename1.Data(), "READ");
    else if(i == 1)
      FileMC  = new TFile(filename2.Data(), "READ");
    else if(i == 2)
      FileMC  = new TFile(filename3.Data(), "READ");

    // Skip if file not found
    if(!FileMC) continue;
    if(FileMC->GetNkeys() <= 0) continue;

    TTree *ConfigTree = (TTree*)FileMC->Get("config");

    // Number of toys
    const Int_t Ntoys = GetNToys(ConfigTree);
    std::cout << "INFO::Number of toys for file " << i << " = " << Ntoys << std::endl;

    std::map<std::string, Int_t> WeightSyst1 = GetWeightSyst(ConfigTree);
    const Int_t nWeightSyst1 =  WeightSyst1.size();

    Int_t chargeid_index = FindWeightSystematic(ConfigTree, "ChargeIDEff");
    Int_t tpctrackeff_index = FindWeightSystematic(ConfigTree, "TpcTrackEff");
    Int_t tpcclustereff_index = FindWeightSystematic(ConfigTree, "TpcClusterEff");

    Int_t weightsyst[3] = {chargeid_index, tpctrackeff_index, tpcclustereff_index};

    // Do not count this systematics when the total systematic is evaluated
    Int_t nuflux_index  = FindWeightSystematic(ConfigTree, "FluxWeightNu");
    Int_t anuflux_index = FindWeightSystematic(ConfigTree, "FluxWeightAntiNu");
    //std::cout << nuflux_index << " , " << anuflux_index << std::endl;

    // Get the default and systematics tree
    TTree *SystTree    = (TTree*)FileMC->Get("all_syst");
    //TTree *DefaultTree = (TTree*)FileMC->Get("default");

    // Total systematic and covariance
    std::cout << "INFO::Running on systematic: ALL" << std::endl;
    if(AngularRanges)
      DetCov[i] = GetRegionsCov(SystTree, Ntoys, nWeightSyst1, nuflux_index, anuflux_index, i+1);
    else
      DetCov[i] = GetALLSystCov(SystTree, parnum, nn, Ntoys, nWeightSyst1, nuflux_index, anuflux_index, i+1);
  
  Int_t xbins = 2*DetCov[0]->GetNbinsX();
  Int_t ybins = 2*DetCov[0]->GetNbinsY();
  DetCov[3]   = new TH2D("covfull",  "covfull", xbins,0,xbins, ybins,0,ybins);

  for(Int_t i = 1; i <= DetCov[0]->GetNbinsX(); i++){
    for(Int_t j = 1; j <= DetCov[0]->GetNbinsY(); j++){
      //std::cout << "Filling bin " << i << " , " << j << std::endl;
      DetCov[3]->SetBinContent(i, j, DetCov[0]->GetBinContent(i,j));
    }
  }
  std::cout << std::endl;
  for(Int_t i = 1; i <= DetCov[1]->GetNbinsX(); i++){
    for(Int_t j = 1; j <= DetCov[1]->GetNbinsY(); j++){
      DetCov[3]->SetBinContent(DetCov[1]->GetNbinsX()+i,  DetCov[1]->GetNbinsY()+j, DetCov[1]->GetBinContent(i,j));
      //std::cout << "Filling bin " << DetCov[1]->GetNbinsX()+i << " , " << DetCov[1]->GetNbinsY()+j << std::endl;
    }
  }
  std::cout << std::endl;
  if(DetCov[2]){
    for(Int_t i = 1; i <= DetCov[2]->GetNbinsX(); i++){
      for(Int_t j = 1; j <= DetCov[2]->GetNbinsY(); j++){
	DetCov[3]->SetBinContent(i,  DetCov[2]->GetNbinsY()+j, DetCov[2]->GetBinContent(i,j));
	DetCov[3]->SetBinContent(DetCov[2]->GetNbinsX()+i,  j, DetCov[2]->GetBinContent(i,j));
	//std::cout << "Filling bin " << i << " , " << DetCov[2]->GetNbinsY()+j << " , " << DetCov[2]->GetBinContent(i,j) << std::endl;
	//std::cout << "Filling bin " << DetCov[2]->GetNbinsX()+i << " , " << j << " , " << DetCov[2]->GetBinContent(i,j) << std::endl;
      }
    }
  }

  TMatrixDSym covmatrix(xbins);
  for(Int_t i = 1; i <= DetCov[3]->GetNbinsX(); i++){
    for(Int_t j = 1; j <= DetCov[3]->GetNbinsY(); j++){
      covmatrix(i-1, j-1) = DetCov[3]->GetBinContent(i,j);
    }
  }
  
  //covmatrix.Print("v");
  TString mat_str = outfile;
  outfile += TString(".root");
  TFile *f = new TFile(outfile.Data(), "RECREATE");

  TCanvas *c = new TCanvas("CovCanvas","CovCanvas");
  c->Divide(2,2);
  for(Int_t i = 0; i < 4; i++){
    if(!DetCov[i]) continue;
    c->cd(i+1);
    DetCov[i]->SetTitle("Detector Covariance");
    DetCov[i]->GetXaxis()->SetTitle("sign #times |V_{ij}|");
    DetCov[i]->Draw("colz");
    DetCov[i]->Write();
  }

  c->Write();

  covmatrix.Write(mat_str.Data());
}

// --------------------------------------------------------
Int_t FindWeightSystematic(TTree *config, std::string syst_name){

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
TH2D* GetALLSystCov(TTree *syst, Double_t* binning, const Int_t nbins, const Int_t NToys, const Int_t nWeights, Int_t ignore_syst1, Int_t ignore_syst2, Int_t Index){
 
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
  histname.Form("_Index%i",Index);
  histname = TString("AllSystCov") + histname;

  TH2D* covhisto   = new TH2D(histname.Data(),  histname.Data(), nbins-1,0,nbins-1, nbins-1,0,nbins-1);
  
  TString histname;
  TH1D* histos1[NToys];
  for(Int_t k = 0; k < NToys; k++){
    histname.Form("histo_syst_%i_Index_%i",k,Index);
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
  
  for(Int_t jj = 0; jj < nbins; jj++){

    for(Int_t k = 0; k < NToys; k++){
      Double_t toyevents = (Double_t)histos1[k]->GetBinContent(jj+1);
      defevents1[jj] += toyevents;
    }

    defevents1[jj] = defevents1[jj]/(Double_t)(NToys);
  }

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
TH2D* GetCovMigration(TTree *syst, Double_t* binning, const Int_t nbins, const Int_t NToys, const Int_t nWeights, Int_t ignore_syst1, Int_t ignore_syst2, Int_t Index){
 
  Float_t selelec_mom[NToys], selelec_costheta[NToys], PairTrack_InvMass[NToys];
  Float_t weight_syst[NToys][nWeights], weight_syst_total[NToys];
  Int_t NTOYS; Int_t NWEIGHTSYST;
  
  syst->SetBranchAddress("selelec_mom",       &selelec_mom);
  syst->SetBranchAddress("selelec_costheta",  &selelec_costheta);
  syst->SetBranchAddress("PairTrack_InvMass", &PairTrack_InvMass);
  syst->SetBranchAddress("weight_syst",       &weight_syst);
  syst->SetBranchAddress("weight_syst_total", &weight_syst_total);
  syst->SetBranchAddress("NTOYS",             &NTOYS);
  syst->SetBranchAddress("NWEIGHTSYST",       &NWEIGHTSYST);
  
  // Define histos
  TString histname;
  histname.Form("_Index%i",Index);
  histname = TString("AllSystCov") + histname;

  TH2D* covhisto   = new TH2D(histname.Data(),  histname.Data(), nbins-1,0,nbins-1, nbins-1,0,nbins-1);
  
  TString histname;
  TH1D* defhistos1[NToys];
  TH1D* defhistos2[NToys]; 
  TH1D* histos1[NToys];
  TH1D* histos2[NToys];
  for(Int_t k = 0; k < NToys; k++){
    histname.Form("histo1_syst_%i_Index_%i",k,Index);
    histos1[k]       = new TH1D(histname.Data(),histname.Data(),nbins-1,binning);
    histname.Form("histo2_syst_%i_Index_%i",k,Index);
    histos2[k]       = new TH1D(histname.Data(),histname.Data(),nbins-1,binning);
    
    histname.Form("defhisto1_syst_%i_Index_%i",k,Index);
    defhistos1[k]    = new TH1D(histname.Data(),histname.Data(),nbins-1,binning);
    histname.Form("defhisto2_syst_%i_Index_%i",k,Index);
    defhistos2[k]    = new TH1D(histname.Data(),histname.Data(),nbins-1,binning);
  }

  // Fill histograms
  for(Int_t i = 0; i < syst->GetEntries(); i++){
    syst->GetEntry(i);

    for(Int_t k = 0; k < NTOYS; k++){
      // Protection
      if(selelec_mom[k] <= 0.) continue;
      if(PairTrack_InvMass[k] <= 0.) continue;

      Double_t w1 = 1.0;
      for(Int_t kk = 0; kk < NWEIGHTSYST; kk++){
	if(kk == ignore_syst1 || kk == ignore_syst2) continue;
	w1 *=  weight_syst[k][kk];
      }
      
      // Protection against large weights 
      if(w1 > 100.0 || w1 <= 0.0) continue;

      if(PairTrack_InvMass[k] < 55) 
	defhistos1[k]->Fill(selelec_mom[k],w1);
      if(PairTrack_InvMass[k] > 110)
	defhistos1[k]->Fill(selelec_mom[k],w1);
	//defhistos2[k]->Fill(selelec_mom[k],w1);

      if(k > 1){
	if(selelec_costheta[k] == selelec_costheta[k-1] && PairTrack_InvMass[k] < 55 && PairTrack_InvMass[k-1] > 110){
	  histos1[k]->Fill(selelec_mom[k],w1);
	}
	if(selelec_costheta[k] == selelec_costheta[k-1] && PairTrack_InvMass[k] > 110 && PairTrack_InvMass[k-1] < 55)
	  histos2[k]->Fill(selelec_mom[k],w1);
      }
    }
  }

  // Get default number of events in each bin
  Double_t defevents1[nbins] = {0.0};
  Double_t defevents2[nbins] = {0.0};
  
  for(Int_t jj = 0; jj < nbins; jj++){

    for(Int_t k = 0; k < NToys; k++){
      Double_t toyevents = (Double_t)defhistos1[k]->GetBinContent(jj+1);
      defevents1[jj] += toyevents;
      //toyevents = (Double_t)defhistos2[k]->GetBinContent(jj+1);
      //defevents2[jj] += toyevents;
    }

    defevents1[jj] = defevents1[jj]/(Double_t)(NToys);
    //defevents2[jj] = defevents2[jj]/(Double_t)(NToys);
  }

  for(Int_t jj = 0; jj < nbins; jj++)
    std::cout << defevents1[jj] << " - " << defevents2[jj] << std::endl;
  
  Double_t covmat[NToys][NToys] = {{0.0}};
  for(Int_t i = 0; i < nbins; i++){
    for(Int_t j = 0; j < nbins; j++){

      for(Int_t k = 0; k < NToys; k++){
	Double_t toyevents1 = defevents1[i] - (Double_t)histos1[k]->GetBinContent(i+1) - (Double_t)histos2[k]->GetBinContent(i+1);
	Double_t toyevents2 = defevents1[j] - (Double_t)histos1[k]->GetBinContent(j+1) - (Double_t)histos2[k]->GetBinContent(j+1);

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
  
  for(Int_t k = 0; k < NToys; k++){
    delete defhistos1[k];
    delete defhistos2[k];
    delete histos1[k];
    delete histos2[k];
  }

  return covhisto;
}

// --------------------------------------------------------
TH2D* GetRegionsCov(TTree *syst, const Int_t NToys, const Int_t nWeights, Int_t ignore_syst1, Int_t ignore_syst2, Int_t Index){
 
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
      histname.Form("histo_syst_%i_region_%i_Index_%i",k,i,Index);
      if(i == 0)
	histos1[i][k] = new TH1D(histname.Data(), histname.Data(), nn_1-1, parnum_1);
      else if(i == 1)
	histos1[i][k] = new TH1D(histname.Data(), histname.Data(), nn_2-1, parnum_2);
    }
  }

  histname.Form("DetectorCovarianceRegions_Index%i",Index);
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
TH2D* GetMigrationRegionsCov(TTree *syst, const Int_t NToys, const Int_t nWeights, Int_t ignore_syst1, Int_t ignore_syst2, Int_t Index){
 
  Float_t selelec_mom[NToys], selelec_costheta[NToys], PairTrack_InvMass[NToys];
  Float_t weight_syst[NToys][nWeights], weight_syst_total[NToys];
  Int_t NTOYS;
  
  syst->SetBranchAddress("selelec_mom",       &selelec_mom);
  syst->SetBranchAddress("selelec_costheta",  &selelec_costheta);
  syst->SetBranchAddress("PairTrack_InvMass", &PairTrack_InvMass);
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
  TH1D* defhistos1[NRegions][NToys]; 
  TH1D* defhistos2[NRegions][NToys]; 
  TH1D* histos1[NRegions][NToys];
  TH1D* histos2[NRegions][NToys];

  for(Int_t i=0; i < NRegions; i++){
    for(Int_t k = 0; k < NToys; k++){

      if(i == 0){
	histname.Form("histo1_syst_%i_region_%i_Index_%i",k,i,Index);
	histos1[i][k] = new TH1D(histname.Data(), histname.Data(), nn_1-1, parnum_1);
	histname.Form("histo2_syst_%i_region_%i_Index_%i",k,i,Index);
	histos2[i][k] = new TH1D(histname.Data(), histname.Data(), nn_1-1, parnum_1);

	histname.Form("def1_syst_%i_region_%i_Index_%i",k,i,Index);
	defhistos1[i][k] = new TH1D(histname.Data(), histname.Data(), nn_1-1, parnum_1);
	histname.Form("def2_syst_%i_region_%i_Index_%i",k,i,Index);
	defhistos2[i][k] = new TH1D(histname.Data(), histname.Data(), nn_1-1, parnum_1);
      }
      else if(i == 1){
	histname.Form("histo1_syst_%i_region_%i_Index_%i",k,i,Index);
	histos1[i][k] = new TH1D(histname.Data(), histname.Data(), nn_2-1, parnum_2);
	histname.Form("histo2_syst_%i_region_%i_Index_%i",k,i,Index);
	histos2[i][k] = new TH1D(histname.Data(), histname.Data(), nn_2-1, parnum_2);

	histname.Form("def1_syst_%i_region_%i_Index_%i",k,i,Index);
	defhistos1[i][k] = new TH1D(histname.Data(), histname.Data(), nn_2-1, parnum_2);
	histname.Form("def2_syst_%i_region_%i_Index_%i",k,i,Index);
	defhistos2[i][k] = new TH1D(histname.Data(), histname.Data(), nn_2-1, parnum_2);
      }

    }
  }

  histname.Form("DetectorCovarianceRegions_Index%i",Index);
  TH2D* covhisto   = new TH2D(histname.Data(),  histname.Data(), allbins,0,allbins, allbins,0,allbins);

   // Fill histograms
  for(Int_t i = 0; i < syst->GetEntries(); i++){
    syst->GetEntry(i);

    for(Int_t j=0; j < NRegions; j++){
      for(Int_t k = 0; k < NToys; k++){
	// Protection
	if(selelec_mom[k] <= 0.) continue;
	if(PairTrack_InvMass[k] <= 0.) continue;
	
	Double_t w1 = 1.0;
	for(Int_t kk = 0; kk < nWeights; kk++){
	  if(kk == ignore_syst1 || kk == ignore_syst2) continue;
	  w1 *=  weight_syst[k][kk];
	}
	
	// Protection against large weights 
	if(w1 > 100.0 || w1 <= 0.0) continue;
      
	if(selelec_costheta[k] >= aparnum[j] && selelec_costheta[k] <=  aparnum[j+1]){
	  if(PairTrack_InvMass[k] < 55) 
	    defhistos1[j][k]->Fill(selelec_mom[k],w1);
	  if(PairTrack_InvMass[k] > 110) 
	    defhistos1[j][k]->Fill(selelec_mom[k],w1);

	  if(k > 1){
	    if(selelec_costheta[k] == selelec_costheta[k-1] && PairTrack_InvMass[k] < 55 && PairTrack_InvMass[k-1] > 110){
	      histos1[j][k]->Fill(selelec_mom[k],w1);
	    }
	    if(selelec_costheta[k] == selelec_costheta[k-1] && PairTrack_InvMass[k] > 110 && PairTrack_InvMass[k-1] < 55)
	      histos2[j][k]->Fill(selelec_mom[k],w1);
	  }
	}

      }
    }
  }
  /*
  // Get default number of events in each bin
  Double_t defevents1[nbins] = {0.0};
  Double_t defevents2[nbins] = {0.0};
  
  for(Int_t jj = 0; jj < nbins; jj++){

    for(Int_t k = 0; k < NToys; k++){
      Double_t toyevents = (Double_t)defhistos1[k]->GetBinContent(jj+1);
      defevents1[jj] += toyevents;
      //toyevents = (Double_t)defhistos2[k]->GetBinContent(jj+1);
      //defevents2[jj] += toyevents;
    }

    defevents1[jj] = defevents1[jj]/(Double_t)(NToys);
    //defevents2[jj] = defevents2[jj]/(Double_t)(NToys);
  }

  for(Int_t jj = 0; jj < nbins; jj++)
    std::cout << defevents1[jj] << " - " << defevents2[jj] << std::endl;
  
  Double_t covmat[NToys][NToys] = {{0.0}};
  for(Int_t i = 0; i < nbins; i++){
    for(Int_t j = 0; j < nbins; j++){

      for(Int_t k = 0; k < NToys; k++){
	Double_t toyevents1 = defevents1[i] - (Double_t)histos1[k]->GetBinContent(i+1) - (Double_t)histos2[k]->GetBinContent(i+1);
	Double_t toyevents2 = defevents1[j] - (Double_t)histos1[k]->GetBinContent(j+1) - (Double_t)histos2[k]->GetBinContent(j+1);

	if(defevents1[i] != 0 && defevents1[j] != 0)
	  covmat[i][j] += (defevents1[i] - toyevents1)*(defevents1[j] - toyevents2) / ( defevents1[i]*defevents1[j]*NToys);
      } // ntoys
      
      Int_t covsign = 1;
      if(covmat[i][j] < 0){
	covsign = -1;
	covmat[i][j] = -covmat[i][j];
      }

      //covmat[i][j] = 1.0 - covmat[i][j];

      covhisto->SetBinContent(i+1,j+1,covsign*sqrt(covmat[i][j]));
    }
  }
  */

  Double_t defevents1[allbins] = {0.0};
  Double_t toyevents1[allbins][NToys] = {{0.0}};
  Double_t toyevents2[allbins][NToys] = {{0.0}};
  Int_t counter = 0;
  for(Int_t i=0; i < NRegions; i++){
    for(Int_t jj = 0; jj < binregion[i]-1; jj++){

      for(Int_t k = 0; k < NToys; k++){
	Double_t toyevents = (Double_t)defhistos1[i][k]->GetBinContent(jj+1);
	defevents1[counter] += toyevents;
	toyevents1[counter][k] = (Double_t)histos1[i][k]->GetBinContent(jj+1);
	toyevents2[counter][k] = (Double_t)histos2[i][k]->GetBinContent(jj+1);
      }
      
      defevents1[counter] = defevents1[counter]/(Double_t)(NToys);
      counter++;
    }
  }
  
  Double_t covmat[NToys][NToys] = {{0.0}};

  for(Int_t i = 0; i < allbins; i++){
    for(Int_t j = 0; j < allbins; j++){
      for(Int_t k = 0; k < NToys; k++){
	Double_t toyevents11 = defevents1[i] - toyevents1[i][k] - toyevents2[i][k];
	Double_t toyevents21 = defevents1[j] - toyevents1[i][k] - toyevents2[i][k];
	//Double_t toyevents1 = toyevents[i][k];
	//Double_t toyevents2 = toyevents[j][k];
	if(defevents1[i] != 0 && defevents1[j] != 0)
	  covmat[i][j] += (defevents1[i] - toyevents11)*(defevents1[j] - toyevents21) / ( defevents1[i]*defevents1[j]*NToys);
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
