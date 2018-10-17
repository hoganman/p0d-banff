#include "p0dAntiNumuCCAnalysis.hxx"
#include "p0dAntiNumuCCSelection.hxx"
#include "Parameters.hxx"
//#include "p0dCCQEOOFVSystematics.hxx"
//#include "p0dPileUpCorrection.hxx"
//#include "oaAnalysisP0DTrackerTreeConverter.hxx"
#include "baseToyMaker.hxx"
#include <iostream>
#include "CutUtils.hxx"
#include "PIDUtils.cxx"
//using std::cout;
//using std::endl;
//added
#include "FiducialVolumeDefinition.hxx"
#include "Parameters.hxx"
#include "CategoriesUtils.hxx"
#include "BasicUtils.hxx"
#include "P0DAnalysisUtils.hxx"
#include "P0DGeometryManager.hxx"
#include "p0dRatioUtils.hxx"
#include "p0dRatioClasses.hxx"

//********************************************************************
p0dAntiNumuCCAnalysis::p0dAntiNumuCCAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("p0dAntiNumuCCAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("P0DANTINUMUCCANALYSISROOT")));

  // Create a p0dAntiNumuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  // N.B. this resets the _flux to NULL in _p0dNumuCCAnalysis so if that is used to fill, we must call
  // _p0dNumuCCAnalysis->DefineConfigurations() which sets up the _flux weighting.
  _p0dNumuCCAnalysis = new p0dNumuCCAnalysis(this);

  // Use the p0dAntiNumuCCAnalysis (in practice that means that the sabe box and event will be used for the p0dAntiNumuCCAnalysis as for this analysis)
  UseAnalysis(_p0dNumuCCAnalysis);

}

//********************************************************************
bool p0dAntiNumuCCAnalysis::Initialize(){
//********************************************************************

  // initialize the used analysis
  if (!_p0dNumuCCAnalysis->Initialize()) return false;

  // Minimum accum level to save event into the output tree
  SetMinAccumCutLevelToSave(ND::params().GetParameterI("p0dAntiNumuCCAnalysis.MinAccumLevelToSave"));

  // for whether or not P0D Recon directory variables are read
  _isUsingReconDirP0DNew = ND::params().GetParameterI("highlandIO.P0DDataClasses.UseReconDirP0DNew");

  //for anti neutrino switch
  _IsAntiNu = ND::params().GetParameterI("p0dAntiNumuCCAnalysis.IsAntiNu");

  // save all numuCC in truth tree
  //_saveAllNumuCC = ND::params().GetParameterI("p0dAntiNumuCCAnalysis.TruthTree.SaveAllNumuCC");

  //anaUtils::AddStandardAntiNumuCategories();

  return true;
}

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineSelections(){
//********************************************************************

  // ----- Inclusive CC -----------
  sel().AddSelection("kP0DAntiNumuCC",           "p0dAntiNumuCC selection",     new p0dAntiNumuCCSelection(false));
}

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineCorrections(){
//********************************************************************
  //
  _p0dNumuCCAnalysis->DefineCorrections();

}

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineSystematics(){
//********************************************************************
  //
  _p0dNumuCCAnalysis->DefineSystematics();

                             
}

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineConfigurations(){
//********************************************************************
  //
  _p0dNumuCCAnalysis->DefineConfigurations();

  Int_t ntoys = ND::params().GetParameterI("baseAnalysis.Systematics.NumberOfToys");
  Int_t randomSeed = ND::params().GetParameterI("baseAnalysis.Systematics.RandomSeed");

  bool enableSingleVariationSystConf = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleVariationSystConfigurations");
  bool enableSingleWeightSystConf    = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleWeightSystConfigurations");
  //AddConfiguration(conf(),all_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
  // std::cout <<"Enable single syst conf: "<<enableSingleSystConf<<std::endl;
  // std::cout <<"Enable P0D Mom scale: "<<ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableP0DMomentumScale")<<std::endl;
  // std::cout <<"Number of toys: "<<ntoys<<std::endl;
/*  if (enableSingleVariationSystConf){
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableP0DMomentumScale")){
      AddConfiguration(conf(), p0dmomscale_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kP0dMomScale,p0dmomscale_syst);
    }
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableP0DMomentumResol")){
      AddConfiguration(conf(), p0dmomresol_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kP0dMomResol,p0dmomresol_syst);
    }
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableTrackerMomentumResol")){
      AddConfiguration(conf(), trackermomresol_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kTrackerMomResol,trackermomresol_syst);
    }
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableTrackerMomentumScale")){
      AddConfiguration(conf(), trackermomscale_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kTrackerMomScale,trackermomscale_syst);
    }
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableTrackerTPCPID")){
      AddConfiguration(conf(), trackertpcpid_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kTrackerTPCPID,trackertpcpid_syst);
    }
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableTrackerBFieldDistortion")){
      AddConfiguration(conf(), trackerbfielddistortion_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kTrackerBFieldDistortion,trackerbfielddistortion_syst);
    }
  }
  if (enableSingleWeightSystConf){
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableTrackerChargeConf")){
      AddConfiguration(conf(), trackerchargeideff_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kTrackerChargeIDEff,trackerchargeideff_syst);
    }
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableTrackerTPCClusterEff")){
      AddConfiguration(conf(), trackertpcclustereff_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kTrackerTPCClusterEff,trackertpcclustereff_syst);
    }
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableTrackerTPCTrackEff")){
      AddConfiguration(conf(), trackertpctrackeff_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
      conf().EnableSystematic(kTrackerTPCTrackEff,trackertpctrackeff_syst);
    }
    if (ND::params().GetParameterI("p0dNumuCCAnalysis.Systematics.EnableP0DMass")){
      AddConfiguration(conf(), p0dmass_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
*/

}

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************
  _p0dNumuCCAnalysis->DefineMicroTrees(addBase);

  AddVar4VF(output(), p0dTPC_PID, "PID from TPC pulls");   
  AddVarI(output(),   np0dNew,        "number of p0d reconstructed tracks in bunch");
  AddVarI(output(),   nNodesShort,        "number of nodes in short track (two tracks)");
  AddVarF(output(),   TwoTrackSepSqrd,     "Separation squared for two track events");
  AddVarF(output(),   TwoTrackLength,     "Length of the second track for two track events");
  AddVar4VF(output(),  TrkChargeRatio, "sorted vector of charge ratios relative to minimum for first 4 tracks in bunch");
  AddVar4VF(output(),  TrkChargeRatioPDGs, "corresponding true PDGs from TrkChargeRaio tracks");
  AddVarI(output(),   TagMichel1,        "Number of tagged muon decays by TP0DMuonDecayTag");
  AddVarI(output(),   TagMichel2,        "Number of tagged muon decays by TP0DTagMuonDecay");
}
//

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineTruthTree(){
  //********************************************************************

  // Variables from baseTrackerAnalysis (run, event, ...)
  _p0dNumuCCAnalysis->DefineTruthTree();

  //--- muon variables -------
  AddVarF(output(),   newmu_mom,   "true mu momentum");
  AddVarF(output(),   newmu_costheta,   "true mu costheta");
}

//********************************************************************
void p0dAntiNumuCCAnalysis::FillMicroTrees(bool addBase){
  //********************************************************************
  // Try to grab NNodes from tracks.  will need to go to p0dRecon stuff to get charge per hit though

  // Fill variables from p0dAntiNumuCCAnalysis
  _p0dNumuCCAnalysis->FillMicroTrees(addBase);

  AnaTrackB* p0dTracks[NMAXPARTICLES];    
  int nP0D         = anaUtils::GetAllTracksUsingDet(GetEvent(), SubDetId::kP0D,  p0dTracks);

  output().FillVar(np0dNew, nP0D);

  // Get all vertices of type AnaP0DVertex
  AnaP0DVertex* p0dVertices[NMAXVERTICES];
  Int_t nVerts = p0dUtils::GetAllP0DVertices(GetEvent(),p0dVertices);

  //count Michel tags from either algorithm
  int TagMichelFill[2];
  int TagMichelReturn = p0dRatioUtils::TagMichel(p0dVertices, nVerts, TagMichelFill); 
  if(TagMichelReturn==0) output().FillVar(TagMichel1, TagMichelFill[0]);
  if(TagMichelReturn==0) output().FillVar(TagMichel2, TagMichelFill[1]);

  //Get charge ratios 
  int nTracksForChargeRatio = 4;
  float* TrkChargeRatioFill = new float[nTracksForChargeRatio];
  float* TrkChargeRatioPDGsFill = new float[nTracksForChargeRatio];

  p0dRatioPID* testPIDout = new p0dRatioPID();
  testPIDout->ChargeRatio = 1.22;
  testPIDout->PDG = -13;
  testPIDout->nClusters = 22;
  cout << testPIDout->ChargeRatio << " " << testPIDout->PDG << " " << testPIDout->nClusters << endl;

  
  p0dRatioPID* RatioPIDOut = new p0dRatioPID[nTracksForChargeRatio];

  for(int i=0; i<4; i++){
    TrkChargeRatioPDGsFill[i]=-9999;
    TrkChargeRatioFill[i]=555.;
  }
  int MipPIDReturn = p0dRatioUtils::MipPID(p0dVertices, nVerts, TrkChargeRatioFill, TrkChargeRatioPDGsFill, nTracksForChargeRatio);
  //seg fault caused by this ^  :(
  //int MipPIDReturn=0;
  //TrkChargeRatio is a 4 vector here, this analysis does not need to fill more than 4 charge ratios for now
  if(MipPIDReturn==0){
    output().FillVectorVarFromArray(TrkChargeRatio,TrkChargeRatioFill,4);
    output().FillVectorVarFromArray(TrkChargeRatioPDGs,TrkChargeRatioPDGsFill,4);
  }

}

//********************************************************************
void p0dAntiNumuCCAnalysis::FillToyVarsInMicroTrees(bool addBase){
  //********************************************************************

  // Variables from the p0dAntiNumuCCAnalysis analysis (including the ones in baseTrackerAnalysis by default, otherwise addBase should be false
  _p0dNumuCCAnalysis->FillToyVarsInMicroTrees(addBase);
}

//********************************************************************
bool p0dAntiNumuCCAnalysis::CheckFillTruthTree(const AnaTrueVertex& vtx){
  //********************************************************************

  //if (_saveAllNumuCC) return _p0dNumuCCAnalysis->CheckFillTruthTree(vtx);
  //else return (_p0dNumuCCAnalysis->CheckFillTruthTree(vtx) &&
  //           anaUtils::GetTopology(vtx, SubDetId::kP0D) == 0);
  //return _p0dNumuCCAnalysis->CheckFillTruthTree(vtx);
  bool numuCC;
  if(_IsAntiNu) numuCC=vtx.ReacCode<0 && vtx.ReacCode>-30 && vtx.NuPDG==-14;
  else numuCC=vtx.ReacCode>0 && vtx.ReacCode<30 && vtx.NuPDG==14;

  //bool numuCC=vtx.ReacCode<0 && vtx.ReacCode>-30 && vtx.NuPDG==-14;// && vtx.LeptonPDG==13;  
  //bool numuCC=vtx.ReacCode>0 && vtx.ReacCode<30 && vtx.NuPDG==14;// && vtx.LeptonPDG==13;  
  //return (anaUtils::InFiducialVolume(SubDetId::kP0D, vtx.Position, FVDef::FVdefminP0D,FVDef::FVdefmaxP0D) && numuCC);
  bool inFV = anaUtils::InFiducialVolume(SubDetId::kP0D, vtx.Position);
  return (inFV && numuCC);

}

//********************************************************************
void p0dAntiNumuCCAnalysis::FillTruthTree(const AnaTrueVertex& vtx){
//********************************************************************
  //_p0dNumuCCAnalysis->FillTruthTreeBase(vtx,SubDetId::kP0D,true);
  _p0dNumuCCAnalysis->FillTruthTreeBase(vtx,SubDetId::kP0D);
//added
  output().FillVar(newmu_costheta,(Float_t)cos(anaUtils::ArrayToTVector3(vtx.LeptonDir).Angle(anaUtils::ArrayToTVector3(vtx.NuDir))));
  output().FillVar(newmu_mom,vtx.LeptonMom);
//end added

}

//********************************************************************
void p0dAntiNumuCCAnalysis::FillCategories(){
//********************************************************************
  //_p0dNumuCCAnalysis->FillCategories();
  //bool IsAntinu
  //if(_IsAntiNu) IsAntinu = true;
  //else IsAntinu = false;

  // For the muon candidate
  //old//anaUtils::FillCategories(_event, static_cast<AnaTrack*>(box().MainTrack),"", SubDetId::kP0D,IsAntinu);
  //anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"",SubDetId::kP0D,IsAntinu);
  anaUtils::FillCategories(&GetEvent(), static_cast<AnaTrack*>(box().MainTrack),"",SubDetId::kP0D,_IsAntiNu);
 
}
