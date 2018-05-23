#include "FluxWeighting.hxx"
#include "ND280AnalysisUtils.hxx"
#include "Parameters.hxx"


//********************************************************************
FluxWeighting::FluxWeighting(){
  //********************************************************************


  std::string fluxfolder = ND::params().GetParameterS("psycheND280Utils.FluxWeighting.Folder");
  std::string version    = ND::params().GetParameterS("psycheND280Utils.FluxWeighting.Version");
  std::string tuning     = ND::params().GetParameterS("psycheND280Utils.FluxWeighting.Tuning");
  bool fineBinning       = (bool)ND::params().GetParameterI("psycheND280Utils.FluxWeighting.FineBinning");

  LoadHistograms(fluxfolder,version,tuning,fineBinning);
}


//********************************************************************
FluxWeighting::FluxWeighting(const std::string& fluxfolder,
    const std::string& version,
    const std::string& tuning,
    bool               fineBinning){
  //********************************************************************

  LoadHistograms(fluxfolder,version,tuning,fineBinning);
}

//********************************************************************
FluxWeighting::~FluxWeighting(){
  //********************************************************************

  ResetHistograms();

}

//********************************************************************
void FluxWeighting::ResetHistograms(){
  //********************************************************************

  for (UInt_t ifile = 0; ifile < NMAXFLUXFILES; ifile++){ 
    for (UInt_t iplane = 0; iplane < kNPLANES; iplane++){  
      for (UInt_t nutype = 0; nutype < 4; nutype++){
        if (_flux_weight[ifile][iplane][nutype]) delete _flux_weight[ifile][iplane][nutype];
        _flux_weight[ifile][iplane][nutype] = NULL;
      }
    }
  }
}

//********************************************************************
void FluxWeighting::LoadHistograms(const std::string& fluxfolder,
    const std::string& version,
    const std::string& tuning,
    bool               fineBinning){
  //********************************************************************

  // Fill with NULL 
  for (UInt_t ifile = 0; ifile < NMAXFLUXFILES; ifile++){ 
    for (UInt_t iplane = 0; iplane < kNPLANES; iplane++){  
      for (UInt_t nutype = 0; nutype < 4; nutype++){
        _flux_weight[ifile][iplane][nutype] = NULL;
      }
    }
  } 

  std::string mode_default = "";

  UInt_t NFLUXFILES = 5;
  if (fluxfolder.find("13a") != std::string::npos) {
    NFLUXFILES = 8;
    if (fluxfolder.find("av1.1") != std::string::npos){
      NFLUXFILES = 12;
      if (fluxfolder.find("2016") != std::string::npos){
        NFLUXFILES = 14;
      }
    }
    if (fluxfolder.find("av2") != std::string::npos){
      mode_default = "_numode";
      NFLUXFILES = 15;
    }

  }



  std::cout << " ------------------------------------------------------ "
    << std::endl;
  std::cout << " ------------------------------------------------------ "
    << std::endl;
  std::cout << "   Flux reweight is enabled " << std::endl;
  std::cout << "   Folder   : " << fluxfolder << std::endl;
  std::cout << "   File     : "
    << fluxfolder + "/runXXX/" + "ndX" + "_" + version +
    "_runXXX.root"
    << std::endl;
  std::cout << "   Tuning   : " << tuning << std::endl;
  std::cout << " ------------------------------------------------------ "
    << std::endl;
  std::cout << " ------------------------------------------------------ "
    << std::endl;

  std::string run_name[NMAXFLUXFILES]   = {"run1","run2","run3b","run3c","run4","run5a","run5b","run5c","run6b","run6c","run6d","run6e","run7b","run7c","run8"};
  Bool_t      antinumode[NMAXFLUXFILES] = {false ,false ,false  ,false  ,false ,false  ,false  ,true   ,true   ,true   ,true   ,true   ,true   ,false  ,false};

  // Plane available for ND280
  std::string plane_name[kNPLANES] = {"nd5", "nd6", "nd13"};

  std::string nutype_name[4] = {"numu", "numub", "nue", "nueb"};


  std::string sFineBinning = ""; 
  if (fineBinning) sFineBinning = "_fine"; 

  // loop over flux files
  for (UInt_t ifile = 0; ifile < NFLUXFILES; ifile++){
    // Loop over planes types
    for (UInt_t iplane = 0; iplane < kNPLANES; iplane++){

      std::string sAntiNuMode = mode_default;
      if (antinumode[ifile]) sAntiNuMode = "_antinumode";

      std::string file_name = "/" + run_name[ifile] + "/" + plane_name[iplane] + "_" + version + "_" + 
        run_name[ifile] + sAntiNuMode + sFineBinning + ".root";
     
      file_name.insert(0, fluxfolder);
      
      if (!file_exists(file_name.c_str()))
        continue;
      
      // Open the flux file
      TFile* file = new TFile(file_name.c_str(), "READ");

      if (!file) continue;
      
      // Loop over neutrino types
      for (UInt_t nutype = 0; nutype < 4; nutype++){

        // Get the histograms
        _flux_weight[ifile][iplane][nutype] = dynamic_cast<TH1D*>(file->Get(("enu_"
                + plane_name[iplane]   + "_"
                + tuning               + "_"
                + nutype_name[nutype]  + "_ratio").c_str()));

        if (_flux_weight[ifile][iplane][nutype]) {
          _flux_weight[ifile][iplane][nutype] = static_cast<TH1D*>(_flux_weight[ifile][iplane][nutype]->Clone());
          _flux_weight[ifile][iplane][nutype]->SetDirectory(NULL);
        }
      }
      if (file) file->Close();
      delete file;
    }

  }
  if (!_flux_weight[0][0][0] || !_flux_weight[0][0][1] || !_flux_weight[0][0][2] || !_flux_weight[0][0][3]) {
    std::cerr
      << "ERROR: Flux weighting was requested, but could not be initialised."
      << std::endl;
    std::cerr
      << "  Specify a valid flux file and tuning in your parameters file, or"
      << std::endl;
    std::cerr
      << "  disable the flux weighting. Flux files can be downloaded from"
      << std::endl;
    std::cerr << "  http://www.t2k.org/beam/NuFlux/FluxRelease" << std::endl;
    exit(EXIT_FAILURE);
  }
}

//********************************************************************
void FluxWeighting::UpdateBunchWeight(AnaBunchB& bunch, AnaTrueVertexB* vertex,
    int RunPeriod){
  //********************************************************************
  Float_t weight = GetWeight(vertex, RunPeriod);
  bunch.Weight *= weight;
}

//********************************************************************
void FluxWeighting::UpdateEventWeight(AnaEventB& event,
    AnaTrueVertexB* vertex){
  //********************************************************************
  Float_t weight =
    GetWeight(vertex, anaUtils::GetRunPeriod(event.EventInfo.Run));
  event.Weight *= weight;
}

//********************************************************************
Float_t FluxWeighting::GetWeight(AnaTrueVertexB* vertex, int RunPeriod, FluxWeighting::Plane plane) const{
  //********************************************************************
  Float_t weight = 1.;

  if (vertex) {
    weight = GetWeight(*vertex, RunPeriod, plane);
  }

  return weight;
}

//********************************************************************
Float_t FluxWeighting::GetWeight(const AnaTrueVertexB& vertex, int RunPeriod, FluxWeighting::Plane plane) const{
  //********************************************************************
  Float_t weight = 1.;
  const UInt_t NMAXRUNPERIODS = 19;

  //-------------------------------- flux_file index   RunPeriod    flux file name     air/water                                                      
  //-----------------------------------------------------------------------------------------------------------------------------
  Int_t fluxfile_index[NMAXRUNPERIODS] = {0,            // 0          run1             water
                                          1,            // 1          run2             water
                                          1,            // 2          run2             air
                                          2,            // 3          run3b            air
                                          3,            // 4          run3c            air
                                          4,            // 5          run4             water
                                          4,            // 6          run4             air
                                          5,            // 7          run5a            water 
                                          7,            // 8          run5c            water
                                          8,            // 9          run6b            air
                                          9,            //10          run6c            air
                                          10,           //11          run6d            air
                                          11,           //12          run6e            air
                                          11,           //13          run6             air
                                          11,           //14          run6e            air
                                          12,           //15          run7b            water
                                          13,           //16          run7c            water
                                          14,           //17          run8             water
                                          14};          //18          run8             air

  Int_t ifile=0;
  if (RunPeriod>(Int_t)NMAXRUNPERIODS-1 || RunPeriod<0){
    std::cerr << "Unknown run period " << RunPeriod
      << ": setting event weight to 1." << std::endl;
    return weight;
  }
  else
    ifile = fluxfile_index[RunPeriod];

  Int_t nutype=0;
  switch (vertex.NuPDG) {
    case 14:
      nutype=0;
      break;
    case -14: 
      nutype=1;
      break;
    case 12:
      nutype=2;
      break;
    case -12:
      nutype=3;
      break;
    default:
      std::cerr << "Unknown neutrino flavour " << vertex.NuPDG
        << ": setting event weight to 1." << std::endl;
      return weight;
      break;
  }


  TH1D* hist = _flux_weight[ifile][plane][nutype];

  if (hist) {
    int bin = hist->FindBin(vertex.NuEnergy / 1000.);
    weight = hist->GetBinContent(bin);
  } else {
    std::cerr << "Couldn't get correct flux weighting histogram - check you "
      "have the right tuning folder specified in the parameters file"
      << std::endl;
  }

  return weight;
}
