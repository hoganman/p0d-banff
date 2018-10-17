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

//#include "P0DELossResolSystematics.hxx"
//#include "P0DELossScaleSystematics.hxx"
//#include "MomentumResolSystematics.hxx"
//#include "MomentumScaleSystematics.hxx"


//********************************************************************
p0dAntiNumuCCAnalysis::p0dAntiNumuCCAnalysis(AnalysisAlgorithm* ana) : baseTrackerAnalysis(ana) {
//********************************************************************

  // Add the package version
  ND::versioning().AddPackage("p0dAntiNumuCCAnalysis", anaUtils::GetSoftwareVersionFromPath((std::string)getenv("P0DANTINUMUCCANALYSISROOT")));

  // Create a p0dAntiNumuCCAnalysis passing this analysis to the constructor. In that way the same managers are used
  // N.B. this resets the _flux to NULL in _p0dNumuCCAnalysis so if that is used to fill, we must call
  // _p0dNumuCCAnalysis->DefineConfigurations() which sets up the _flux weighting.
  _numuCCAnalysis = new numuCCAnalysis(this);
  //added Feb7
  _p0dNumuCCAnalysis = new p0dNumuCCAnalysis(this);
  //end added

  // Use the p0dAntiNumuCCAnalysis (in practice that means that the sabe box and event will be used for the p0dAntiNumuCCAnalysis as for this analysis)
  UseAnalysis(_numuCCAnalysis);
  //UseAnalysis(_p0dNumuCCAnalysis);

}

//********************************************************************
bool p0dAntiNumuCCAnalysis::Initialize(){
//********************************************************************

  // initialize the used analysis
 // if (!_numuCCAnalysis->Initialize()) return false;
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
  _numuCCAnalysis->DefineCorrections();

}

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineSystematics(){
//********************************************************************
  //
  //_numuCCAnalysis->DefineSystematics();
  //_p0dNumuCCAnalysis->DefineSystematics();
  //baseTrackerAnalysis::DefineSystematics();

  //syst().AddVariationSystematic(kP0dMomScale,"P0DMomScale", new P0dMomScaleSystematics("P0DMomScale"));
  //evar().AddEventVariation(kP0dMomScale,"P0DMomScale", new P0dMomScaleSystematics("P0DMomScale"));
  //syst().AddVariationSystematic(kP0dMomResol,"P0DMomResol",new P0dMomResolSystematics("P0DMomResol"));
  //syst().AddVariationSystematic(kTrackerMomResol,"TrackerMomResol",new TrackerMomResolSystematics());
  //syst().AddVariationSystematic(kTrackerMomScale,"TrackerMomScale",new TrackerMomScaleSystematics());
  //syst().AddVariationSystematic(kTrackerTPCPID,"TrackerTPCPID",new TrackerTPCPIDSystematics());
 // syst().AddVariationSystematic(kTrackerBFieldDistortion,"TrackerBFieldDistortion",new TrackerBFieldDistortionSystematics());


  //evar().AddEventVariation(kP0DELossResolVar, "Test Var", new P0DELossResolSystematics());
  //evar().AddEventVariation(kP0DELossScaleVar, "Test Var2", new P0DELossScaleSystematics());
  //syst().AddVariationSystematic(kP0dMomScale, "Test Var2", new P0DELossScaleSystematics());

  //evar().AddEventVariation(kTestResol, "test Resol", new MomentumResolSystematics()); //det crossing error either flag
  //evar().AddEventVariation(kTestResol, "test Resol", new MomentumScaleSystematics()); //works with p0dFV=0 or 1
  //evar().AddEventVariation(kTestResol, "test Resol", new P0DELossResolSystematics()); //works with p0dFV=0 or 1
  //evar().AddEventVariation(kTestScale, "test Scale", new P0DELossScaleSystematics());
  //evar().AddEventVariation(kTestResol, "test Resol", new P0DELossScaleSystematics()); //works with p0dFV=0 or 1
  //syst().AddVariationSystematic(kTestResol, "test Resol", new MomentumResolSystematics());


  //P0DELossResolSystematics

                             
}

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineConfigurations(){
//********************************************************************
  //
  //_numuCCAnalysis->DefineConfigurations();
  //_p0dNumuCCAnalysis->DefineConfigurations();
  //baseTrackerAnalysis::DefineConfigurations();

  Int_t ntoys = ND::params().GetParameterI("baseAnalysis.Systematics.NumberOfToys");
  Int_t randomSeed = ND::params().GetParameterI("baseAnalysis.Systematics.RandomSeed");
  //Int_t randomSeed = 13241232; 

  //bool enableSingleVariationSystConf = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleVariationSystConfigurations");
  //bool enableSingleWeightSystConf    = (bool)ND::params().GetParameterI("baseAnalysis.Configurations.EnableSingleWeightSystConfigurations");
 

  //baseTrackerAnalysis::DefineConfigurations();

  //added from v2r9 of p0dNumuCC Feb 7 2017
  // Enable all Event Weights in the default and all_syst configurations

  //for (std::vector<ConfigurationBase* >::iterator it= conf().GetConfigurations().begin();it!=conf().GetConfigurations().end();it++){
  //Int_t index = (*it)->GetIndex();
  //if (index != ConfigurationManager::default_conf && (index != all_syst || !_enableAllSystConfig)) continue;
  //}

  //AddConfiguration(conf(), test_syst, ntoys, randomSeed, new baseToyMaker(randomSeed));
  //conf().EnableSystematic(kTestResol, test_syst);
  //conf().EnableEventVariation(kTestResol, test_syst); //need both for systematic to do anything 

  //conf().EnableSystematic(kTestScale, test_syst);
  //conf().EnableEventVariation(kTestScale, test_syst);
  //conf().EnableSystematic(kP0DELossResolVar, test_syst);
  //conf().EnableSystematic(kP0DELossScaleVar, test_syst);


/*

  AddConfiguration(conf(), test_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
  conf().EnableSystematic(kP0dMomScale,p0dmomscale_syst);
  conf().EnableEventVariation(kP0dMomResol,p0dmomresol_syst);
  conf().EnableEventVariation(kTrackerMomResol,trackermomresol_syst);
  conf().EnableEventVariation(kTrackerMomScale,trackermomscale_syst);
  conf().EnableEventVariation(kTrackerTPCPID,trackertpcpid_syst);
  conf().EnableEventVariation(kTrackerBFieldDistortion,trackerbfielddistortion_syst);
  conf().EnableEventWeight(kTrackerChargeIDEff,trackerchargeideff_syst);
  conf().EnableEventWeight(kTrackerTPCClusterEff,trackertpcclustereff_syst);
  conf().EnableEventWeight(kTrackerTPCTrackEff,trackertpctrackeff_syst);
  //conf().EnableEventWeight(kP0dMass,p0dmass_syst);
  //conf().EnableEventWeight(kP0dOOFV,p0doofv_syst);
  //conf().EnableEventWeight(kP0dVeto,p0dveto_syst);
*/
  //added from above old code
  //AddConfiguration(conf(), p0dmomscale_syst,ntoys,randomSeed,new baseToyMaker(randomSeed));
  //conf().EnableSystematic(kP0dMomScale,p0dmomscale_syst);

  //end added


}

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineMicroTrees(bool addBase){
//********************************************************************
  _numuCCAnalysis->DefineMicroTrees(addBase);
  //_p0dNumuCCAnalysis->DefineMicroTrees(addBase);

  AddVar4VF(output(), p0dTPC_PID, "PID from TPC pulls");   
  AddVarI(output(),   np0dNew,        "number of p0d reconstructed tracks in bunch");
  AddVarI(output(),   nNodesShort,        "number of nodes in short track (two tracks)");
  AddVarF(output(),   TwoTrackSepSqrd,     "Separation squared for two track events");
  AddVarF(output(),   TwoTrackLength,     "Length of the second track for two track events");
  AddVar4VF(output(),  TrkChargeRatio, "sorted vector of charge ratios relative to minimum for first 4 tracks in bunch");
  AddVar4VF(output(),  CloseTrkChargeRatio, "vector of charge ratios relative to minimum for first 4 tracks in bunch Close");
  AddVar4VF(output(),  TrkChargeRatioErr, "sorted vector of charge ratio errors squared");
  AddVar4VF(output(),  TrkChargeRatioPDGs, "corresponding true PDGs from TrkChargeRaio tracks");
  AddVar4VF(output(),  TrkChargeRatioNClusters, "corresponding number of nodes from TrkChargeRaio tracks");
  AddVar4VF(output(),  TCR_PosX, "Charge Ratio Tracks X Position");
  AddVar4VF(output(),  TCR_PosY, "Charge Ratio Tracks Y Position");
  AddVar4VF(output(),  TCR_PosZ, "Charge Ratio Tracks Z Position");
  AddVar4VF(output(),  nMuLike, "Number of muon like tracks near selected muon start");
  AddVarI(output(),   TagMichel1,        "Number of tagged muon decays by TP0DMuonDecayTag");
  AddVarI(output(),   TagMichel2,        "Number of tagged muon decays by TP0DTagMuonDecay");
  AddVarI(output(),   PIDnShowers,        "Number of showers in event");
  AddToyVarF(output(),   selmu_momNew,     "test toy momentum");
  AddVarF(output(), p0dELoss, "test p0dELoss");
  AddVarF(output(), trueP0dELoss, "test trueP0dELoss");
  AddVarF(output(), TrkChargeMin, "Minimum Track Charge Deposit Per Node");
  AddVarI(output(), eCalVeto, "rough ECal Veto");
  //^ this is the one Damon uses currently
}
//

//********************************************************************
void p0dAntiNumuCCAnalysis::DefineTruthTree(){
  //********************************************************************

  // Variables from baseTrackerAnalysis (run, event, ...)
  _numuCCAnalysis->DefineTruthTree();
  //_p0dNumuCCAnalysis->DefineTruthTree();

  //--- muon variables -------
  AddVarF(output(),   newmu_mom,   "true mu momentum");
  AddVarF(output(),   newmu_costheta,   "true mu costheta");
}

//********************************************************************
void p0dAntiNumuCCAnalysis::FillMicroTrees(bool addBase){
  //********************************************************************
  // Try to grab NNodes from tracks.  will need to go to p0dRecon stuff to get charge per hit though

  // Fill variables from p0dAntiNumuCCAnalysis
  _numuCCAnalysis->FillMicroTrees(addBase);
  //_p0dNumuCCAnalysis->FillMicroTrees(addBase);
  //
  Float_t* temp_selmu_pos = new Float_t[4];
  temp_selmu_pos = box().MainTrack->PositionStart;
  

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
  float* CloseTrkChargeRatioFill = new float[nTracksForChargeRatio];
  float* TrkChargeRatioErrFill = new float[nTracksForChargeRatio];
  float* TrkChargeRatioPDGsFill = new float[nTracksForChargeRatio];
  float* TrkChargeRatioNClustersFill = new float[nTracksForChargeRatio];
  float* TrkChargeRatioPosXFill = new float[nTracksForChargeRatio];
  float* TrkChargeRatioPosYFill = new float[nTracksForChargeRatio];
  float* TrkChargeRatioPosZFill = new float[nTracksForChargeRatio];
  float* nMuLikeFill = new float[nTracksForChargeRatio];

  
  p0dRatioPID* RatioPIDOut = new p0dRatioPID[nTracksForChargeRatio];


  int MipPIDReturn = p0dRatioUtils::MipPID(p0dVertices, nVerts, RatioPIDOut, nTracksForChargeRatio, temp_selmu_pos);
  //seg fault caused by this ^  :(
  //int MipPIDReturn=0;
  //TrkChargeRatio is a 4 vector here, this analysis does not need to fill more than 4 charge ratios for now
  //cout << "selmu: " << temp_selmu_pos[0] << " " << temp_selmu_pos[1] << " " << temp_selmu_pos[2] << endl;

  for(int i=0; i<4; i++){
    if(RatioPIDOut[1].ChargeRatio<555){
      //cout << RatioPIDOut[i].ChargeRatio << " " << RatioPIDOut[i].nClusters << " " << RatioPIDOut[i].PosX << " " << RatioPIDOut[i].PosY << " " << RatioPIDOut[i].PosZ << endl;
    }
    TrkChargeRatioFill[i]=RatioPIDOut[i].ChargeRatio;
    CloseTrkChargeRatioFill[i]=RatioPIDOut[i].CloseChargeRatio;
    TrkChargeRatioErrFill[i]=RatioPIDOut[i].ChargeRatioErr;
    TrkChargeRatioPDGsFill[i]=RatioPIDOut[i].PDG;
    TrkChargeRatioNClustersFill[i]=RatioPIDOut[i].nClusters;
    TrkChargeRatioPosXFill[i] = RatioPIDOut[i].PosX;
    TrkChargeRatioPosYFill[i] = RatioPIDOut[i].PosY;
    TrkChargeRatioPosZFill[i] = RatioPIDOut[i].PosZ;
    nMuLikeFill[i] = RatioPIDOut[i].nMuLike;
  }


  if(MipPIDReturn==0){
    output().FillVectorVarFromArray(TrkChargeRatio,TrkChargeRatioFill,4);
    output().FillVectorVarFromArray(CloseTrkChargeRatio,CloseTrkChargeRatioFill,4);
    output().FillVectorVarFromArray(TrkChargeRatioErr,TrkChargeRatioErrFill,4);
    output().FillVectorVarFromArray(TrkChargeRatioPDGs,TrkChargeRatioPDGsFill,4);
    output().FillVectorVarFromArray(TrkChargeRatioNClusters,TrkChargeRatioNClustersFill,4);
    output().FillVectorVarFromArray(TCR_PosX,TrkChargeRatioPosXFill,4);
    output().FillVectorVarFromArray(TCR_PosY,TrkChargeRatioPosYFill,4);
    output().FillVectorVarFromArray(TCR_PosZ,TrkChargeRatioPosZFill,4);
    output().FillVar(PIDnShowers, RatioPIDOut[0].nShowers);
    output().FillVar(eCalVeto, RatioPIDOut[0].eCalVeto);
    output().FillVar(TrkChargeMin, RatioPIDOut[0].ChargeMin);
    output().FillVectorVarFromArray(nMuLike,nMuLikeFill,4);
  }


  //added syst momentum info
  /*
  if(box().MainTrack){
    AnaTrackB* track = static_cast<AnaTrackB*>(box().MainTrack);
    AnaP0DTrackerTrack* track_pt = (AnaP0DTrackerTrack*) track;

    //if (!track->nP0DSegments) continue;
    //if (!track->GetTrueParticle()) continue;
    //if (track->GetTrueParticle()->nDetCrossings < 2) continue;
    AnaP0DParticleB* p0d = (AnaP0DParticleB*) track->P0DSegments[0];//returns an AnaP0DParticle*
    if(track->nP0DSegments){
      //cout << "P0D segment" << endl;
      if(track->GetTrueParticle()){
	//cout << "TrueParticle" << endl;
	if(track->GetTrueParticle()->nDetCrossings>=2){
	  //cout << "DetCrossings" << endl;

	  Float_t& p = track->Momentum;

	  Float_t p0dlength = p0d->Length;
	  Float_t p0dEloss = track->Momentum-track_pt->TrackerMomentum;
	  //Float_t p0dEloss = p0d->ELoss;
	  Float_t trueMomentum = track->GetTrueParticle()->Momentum;
	  AnaDetCrossingB* cross = track->GetTrueParticle()->DetCrossings[1];// Check index before using
	  Float_t postP0DMomentum = sqrt(cross->EntranceMomentum[0]*cross->EntranceMomentum[0]
	      + cross->EntranceMomentum[1]*cross->EntranceMomentum[1]
	      + cross->EntranceMomentum[2]*cross->EntranceMomentum[2]);
	  Float_t trueP0dEloss = trueMomentum - postP0DMomentum;
	  //store p0dEloss and trueP0dEloss
	  //cout << p0dEloss << " " << trueP0dEloss << endl;
	  output().FillVar(p0dELoss, p0dEloss);
	  output().FillVar(trueP0dELoss, trueP0dEloss);
	  */

	  /*
	     this is how the momentum is varies
	     Float_t scale, scaleError;

	     if (p0dEloss > p) continue;
	  // Apply the momentum scale factor
	  p += (scale + scaleError * toy.GetToyVariations(_index)->Variations[0]) * (p0dEloss - trueP0dEloss);
	   */
	   /*
        }//if det cross
      }// if true particle
    }//if p0d segment
  }//box.maintrack
*/


}

//********************************************************************
void p0dAntiNumuCCAnalysis::FillToyVarsInMicroTrees(bool addBase){
  //********************************************************************

  // Variables from the p0dAntiNumuCCAnalysis analysis (including the ones in baseTrackerAnalysis by default, otherwise addBase should be false
  _numuCCAnalysis->FillToyVarsInMicroTrees(addBase);
  //_p0dNumuCCAnalysis->FillToyVarsInMicroTrees(addBase);

  if (box().MainTrack){
    // Since we are applying a systematic that varies the momentum we need to save the momentum for each toy
    output().FillToyVar(selmu_momNew, box().MainTrack->Momentum);    
  }


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
  //_p0dNumuCCAnalysis->FillTruthTreeBase(vtx,SubDetId::kP0D);
  _numuCCAnalysis->FillTruthTreeBase(vtx,SubDetId::kP0D,_IsAntiNu);
  //_p0dNumuCCAnalysis->FillTruthTreeBase(vtx,SubDetId::kP0D,_IsAntiNu);
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
