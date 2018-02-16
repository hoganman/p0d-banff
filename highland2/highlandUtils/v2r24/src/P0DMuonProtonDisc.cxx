#include "P0DMuonProtonDisc.hxx"

#include "TMath.h"

#include "BaseDataClasses.hxx"
#include "ND280AnalysisUtils.hxx"

//#define debug 1

//**************************************************************************
P0DMuonProtonDisc::P0DMuonProtonDisc(std::string filename){
//**************************************************************************    
  //Angle bins
  std::string anglename[]={"All","0-30","30-60",">60"};
  
  //Declare default software version
  //fDefaultSoftwareVersion="v10r11p9";
  fDefaultSoftwareVersion="v11r31";//This is default software for prod6B

  std::cout << "### Initializing P0DMuonProtonDisc..." << std::endl;
  std::cout << "## Loading file " << filename << " to get software version..." << std::endl;
  string version = P0DMuonProtonDisc::GetFileSoftwareVersion(filename);
  if(version=="NULL"){
    version = fDefaultSoftwareVersion;
    std::cout <<"# Software version not specified in tree, using default version: " << version << std::endl;
  }
  else{
    std::cout <<"# Using PDFs for software version: " << version << std::endl;
  }

  std::cout << "## Loading files..." << std::endl;
  std::cout << "# LikelihoodPDFs-" << version << ".root..." << std::endl;
  TFile *lfile = new TFile(Form("$HIGHLANDUTILSROOT/data/P0DMuonProtonDiscInputs/LikelihoodPDFs-%s.root",version.c_str()));
  if(!lfile) {
    std::cout << "# Error loading file LikelihoodPDFs-" << version << ".root!" << std::endl;
    exit(1);
  }   

  std::cout << "# LikelihoodPDFs-nowater-" << version << ".root..." << std::endl;
  TFile *lfilewaterout = new TFile(Form("$HIGHLANDUTILSROOT/data/P0DMuonProtonDiscInputs/LikelihoodPDFs-nowater-%s.root",version.c_str()));
  if(!lfilewaterout) {
    std::cout << "# Error loading file LikelihoodPDFs-nowater-" << version << ".root!" << std::endl;
    exit(1);
  }   
  gDirectory->cd();
  
  for(int angle=0;angle<4;angle++){
    //Get Likelihood method PDFs
    //waterin
    fLCoarse[angle]       = (TH2F*)lfile->Get(Form("Likelihood-Coarse-%s",anglename[angle].c_str()));
    fLFineDistance[angle] = (TH2F*)lfile->Get(Form("Likelihood-FineDistance-%s",anglename[angle].c_str()));
    fLFineCharge[angle]   = (TH2F*)lfile->Get(Form("Likelihood-FineCharge-%s",anglename[angle].c_str()));
    fLFine[angle]         = (TH2F*)lfile->Get(Form("Likelihood-Fine-%s",anglename[angle].c_str()));

    //waterout
    fLCoarseWaterOut[angle]       = (TH2F*)lfilewaterout->Get(Form("Likelihood-Coarse-%s-WaterOut",anglename[angle].c_str()));
    fLFineDistanceWaterOut[angle] = (TH2F*)lfilewaterout->Get(Form("Likelihood-FineDistance-%s-WaterOut",anglename[angle].c_str()));
    fLFineChargeWaterOut[angle]   = (TH2F*)lfilewaterout->Get(Form("Likelihood-FineCharge-%s-WaterOut",anglename[angle].c_str()));
    fLFineWaterOut[angle]         = (TH2F*)lfilewaterout->Get(Form("Likelihood-Fine-%s-WaterOut",anglename[angle].c_str()));
  }

  string path = getenv("HIGHLANDUTILSROOT");
  string checkversion = Form("%s/data/P0DMuonProtonDiscInputs/%s_water_MuonPDF.txt",path.c_str(),version.c_str());
  if(!CheckIfFileExists(checkversion)){
    version = fDefaultSoftwareVersion;
    std::cout <<"Software version's PDF files not in input directory, using default version: " <<version << std::endl;
  }

  std::cout << "Loading " << Form("P0DMuonProtonDiscInputs/%s_water_MuonPDF.txt",version.c_str()) 
            << "....Extracting Water In PID Constants" << std::endl;

  P0DMuonProtonDisc::loadPIDValues(fChargeNorm,fChargeMeans,fChargeSigmas,
                                 Form("%s/data/P0DMuonProtonDiscInputs/%s_water_MuonPDF.txt",path.c_str(),version.c_str()),6);

  std::cout << "Loading " << Form("P0DMuonProtonDiscInputs/%s_nowater_MuonPDF.txt",version.c_str()) 
            << "....Extracting Water Out PID Constants" << std::endl;

  P0DMuonProtonDisc::loadPIDValues(fChargeNormWaterOut,fChargeMeansWaterOut,fChargeSigmasWaterOut,
                                 Form("%s/data/P0DMuonProtonDiscInputs/%s_nowater_MuonPDF.txt",path.c_str(),version.c_str()),6);

  std::cout << Form("$HIGHLANDUTILSROOT/data/P0DMuonProtonDiscInputs/%s_nowater_MuonPDF.txt",version.c_str()) << std::endl;
 
  std::cout << "Collected PID Constant Values" << std::endl;
  for(int i=0;i<4;i++){
    std::cout << "Angle: " << anglename[i] << std::endl;
    for(uint j=0;j<fChargeNorm[i].size();j++){
      std::cout << "\t" << fChargeNorm[i][j] 
                << "\t" << fChargeMeans[i][j] 
                << "\t" << fChargeSigmas[i][j] << std::endl;
    }
  }  
}

///Likelihood PID method access functions
//**************************************************************************
double P0DMuonProtonDisc::GetLikelihoodCoarse(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength){
//**************************************************************************
  double overallDistance  = 0.0;
  double outputLikelihood = 0.0;
  int numNodesUsed = 0;

  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);
  double trackangle = trackDir.Theta()*180/TMath::Pi();

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle >=0 && trackangle <= 30) anglebin = 1;
  else if(trackangle > 30 && trackangle <= 60) anglebin = 2;
  else anglebin = 3;

  float previousIndex = 0; 
  float currentIndex = 0;
  
  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);

  //Loop over nodes(clusters) in track
  for(int j=0;j<Track->nClusters;j++){
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-1]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j != (Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-2]);
      if(!next_n) continue;
    }
          
    AnaP0DHit** hits = n->Hits;
    if(j==0){
	    float tempLayer = p0dUtils::GetLayerFromGeomId(hits[0]->GeomID);
	    float tempP0dule = p0dUtils::GetP0DuleFromGeomId(hits[0]->GeomID);

	    previousIndex = tempLayer*0.5+tempP0dule;
	    currentIndex=previousIndex;
    }
    else{
	    float tempLayer = p0dUtils::GetLayerFromGeomId(hits[0]->GeomID);
	    float tempP0dule = p0dUtils::GetP0DuleFromGeomId(hits[0]->GeomID);

	    currentIndex = tempLayer*0.5+tempP0dule;
    }

    if(currentIndex-previousIndex >0.5) outputLikelihood+=-10;
    previousIndex = currentIndex;

    //Get node position
    TLorentzVector nodePosition = anaUtils::ArrayToTLorentzVector(n->Position);     
    TVector3 nodeDirection(-9999,-9999,-9999);

    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;

    double costheta = nodeDirection.CosTheta();
   
    //Correct node energy with the angle (costheta) 
    double nodeEnergy = n->EDeposit*costheta;
    nodeEnergy = nodeEnergy*energyCorrection;

    //Calculate the overall distance travelled (dX) by adding up all distances between nodes.
    if(j!=0){
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j]);	 
	    if(!n2) continue;

	    //TLorentzVector prevnodePosition = n2->Position;
	    TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);     

	    //	  double prevnodeEnergy = n2->EDeposit;
	    double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));
	    overallDistance+=distance;

	    if(overallDistance <= useLength){
	      numNodesUsed++;
	      if(isWater) outputLikelihood+=TMath::Log(fLCoarse[anglebin]->Interpolate(overallDistance,nodeEnergy));
	      if(!isWater) outputLikelihood+=TMath::Log(fLCoarseWaterOut[anglebin]->Interpolate(overallDistance,nodeEnergy));
	    }
    }
  }//end of loop over nodes

  if(numNodesUsed==0) numNodesUsed=1;

  return outputLikelihood/numNodesUsed;
}

//**************************************************************************
double P0DMuonProtonDisc::GetLikelihoodCoarseBeg(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength){ 
//**************************************************************************

  double overallDistance  = 0.0;
  double outputLikelihood = 0.0;
  int numNodesUsed = 0;

  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);
  double trackangle = trackDir.Theta()*180/TMath::Pi();

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle >=0 && trackangle <= 30) anglebin = 1;
  else if(trackangle > 30 && trackangle <= 60) anglebin = 2;
  else anglebin = 3;

  float previousIndex = 0; 
  float currentIndex = 0;
  
  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);

  //Loop over nodes(clusters) in track
  for(int j=0;j<Track->nClusters;j++){
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[j]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j!=(Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[j+1]);
      if(!next_n) continue;
    }
          
    AnaP0DHit** hits = n->Hits;
    if(j==0){
	    float tempLayer = p0dUtils::GetLayerFromGeomId(hits[0]->GeomID);
	    float tempP0dule = p0dUtils::GetP0DuleFromGeomId(hits[0]->GeomID);

	    previousIndex = tempLayer*0.5+tempP0dule;
	    currentIndex=previousIndex;
    }
    else{
	    float tempLayer = p0dUtils::GetLayerFromGeomId(hits[0]->GeomID);
	    float tempP0dule = p0dUtils::GetP0DuleFromGeomId(hits[0]->GeomID);

	    currentIndex = tempLayer*0.5+tempP0dule;
    }

    if(currentIndex-previousIndex >0.5) outputLikelihood+=-10;
    previousIndex = currentIndex;

    //Get node position
    TLorentzVector nodePosition = anaUtils::ArrayToTLorentzVector(n->Position);
    TVector3 nodeDirection(-9999,-9999,-9999);

    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;

    double costheta = nodeDirection.CosTheta();
   
    //Correct node energy with the angle (costheta) 
    double nodeEnergy = n->EDeposit*costheta;
    nodeEnergy = nodeEnergy*energyCorrection;

    //Calculate the overall distance travelled (dX) by adding up all distances between nodes.
    if(j!=0){
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[j-1]);	 
	    if(!n2) continue;

	    //TLorentzVector prevnodePosition = n2->Position;
	    TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);

	    //	  double prevnodeEnergy = n2->EDeposit;
	    double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));
	    overallDistance+=distance;

	    if(overallDistance <= useLength){
	      numNodesUsed ++;
	      if(isWater) outputLikelihood+=TMath::Log(fLCoarse[anglebin]->Interpolate(overallDistance,nodeEnergy));
	      if(!isWater) outputLikelihood+=TMath::Log(fLCoarseWaterOut[anglebin]->Interpolate(overallDistance,nodeEnergy));
	    }
    }
  }//end of loop over nodes

  if(numNodesUsed==0) numNodesUsed=1;

  return outputLikelihood/numNodesUsed;
}

//**************************************************************************
double P0DMuonProtonDisc::GetLikelihoodFineCharge(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength){  
//**************************************************************************
  double overallDistance=0.0;
  double outputLikelihood = 0.0;

  //vector<short> nodes = Track->Nodes;
  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);
  double trackangle = trackDir.Theta()*180/TMath::Pi();  

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle>=0&&trackangle<=30) anglebin=1;
  else if(trackangle>30&&trackangle<=60) anglebin=2;
  else anglebin=3;

  double binwidthX = fLFineCharge[anglebin]->GetXaxis()->GetBinWidth(5);
  double binwidthY = fLFineCharge[anglebin]->GetYaxis()->GetBinWidth(5);

  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);

  //for(int j=0;j<nodes.size();j++){
  for(int j=0;j<Track->nClusters;j++){
    //ND::TP0DReconModule::TP0DNode* n = GetP0DNode(nodes[nodes.size()-j-1]);
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-1]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j != (Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-2]);
      if(!next_n) continue;
    }

    TLorentzVector nodePosition = anaUtils::ArrayToTLorentzVector(n->Position);
    TVector3 nodeDirection(-9999,-9999,-9999);

    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;

    double costheta = nodeDirection.CosTheta();
    double nodeEnergy = n->EDeposit*costheta;
    nodeEnergy = nodeEnergy*energyCorrection;

    if(j!=0){
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j]);
      if(!n2) continue;

      TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);
      
      //	  double prevnodeEnergy = n2->EDeposit;
      double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));
      overallDistance+=distance;

      if(overallDistance<=useLength){
	      if(isWater) 
          outputLikelihood+=TMath::Log(fLFineCharge[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1));
	      if(!isWater) 
          outputLikelihood+=TMath::Log(fLFineChargeWaterOut[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1));
      }
    }
  }
  return outputLikelihood;
}

//**************************************************************************
double P0DMuonProtonDisc::GetLikelihoodFineDistance(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength){  
//**************************************************************************

  double overallDistance=0.0;
  double outputLikelihood = 0.0;

  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);
  double trackangle = trackDir.Theta()*180/TMath::Pi();  

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle>=0&&trackangle<=30) anglebin=1;
  else if(trackangle>30&&trackangle<=60) anglebin=2;
  else anglebin=3;

  double binwidthX = fLFineDistance[anglebin]->GetXaxis()->GetBinWidth(5);
  double binwidthY = fLFineDistance[anglebin]->GetYaxis()->GetBinWidth(5);

  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);

  for(int j=0;j<Track->nClusters;j++){
    //ND::TP0DReconModule::TP0DNode* n = GetP0DNode(nodes[nodes.size()-j-1]);
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-1]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j != (Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-2]);
      if(!next_n) continue;
    }
 
    TLorentzVector nodePosition = n->Position;
    TVector3 nodeDirection(-9999,-9999,-9999);

    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;

    double costheta = nodeDirection.CosTheta();
    double nodeEnergy = n->EDeposit*costheta;
    nodeEnergy = nodeEnergy*energyCorrection;

    if(j!=0){
      //ND::TP0DReconModule::TP0DNode* n2 = GetP0DNode(nodes[nodes.size()-j]);
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j]);
      if(!n2) continue;

      TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);
      
      //	  double prevnodeEnergy = n2->EDeposit;
      double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));
      overallDistance+=distance;

      if(overallDistance<=useLength){
	      if(isWater) outputLikelihood+=TMath::Log(fLFineDistance[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1));
	      if(!isWater) outputLikelihood+=TMath::Log(fLFineDistanceWaterOut[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1));
      }
    }
  }
  return outputLikelihood;
}

//**************************************************************************
double P0DMuonProtonDisc::GetLikelihoodFine(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength){  
//**************************************************************************

  double overallDistance  = 0.0;
  double outputLikelihood = 0.0;

  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);
  double trackangle = trackDir.Theta()*180/TMath::Pi();  

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle>=0&&trackangle<=30) anglebin=1;
  else if(trackangle>30&&trackangle<=60) anglebin=2;
  else anglebin=3;

  double binwidthX = fLFine[anglebin]->GetXaxis()->GetBinWidth(5);
  double binwidthY = fLFine[anglebin]->GetYaxis()->GetBinWidth(5);

  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);

  for(int j=0;j<Track->nClusters;j++){
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-1]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j != (Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-2]);
      if(!next_n) continue;
    }

    TLorentzVector nodePosition = n->Position;
    TVector3 nodeDirection(-9999,-9999,-9999);

    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;

    double costheta = nodeDirection.CosTheta();
    double nodeEnergy = n->EDeposit*costheta;
    nodeEnergy = nodeEnergy*energyCorrection;

    if(j!=0){      
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j]);
      if(!n2) continue;

      TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);
      
      //	  double prevnodeEnergy = n2->EDeposit;
      double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));
      overallDistance+=distance;

      if(overallDistance <= useLength){
	      if(isWater){
	        if(fLFine[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1)!=0){
	          outputLikelihood+=TMath::Log(fLFine[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1));
	        }
	        else outputLikelihood+=TMath::Log(0.000000001);
	      }
	      if(!isWater){
	        if(fLFineWaterOut[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1)!=0){
	          outputLikelihood+=TMath::Log(fLFineWaterOut[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1));
	        }
	        else outputLikelihood+=TMath::Log(0.000000001);
	      }
      }
    }
  }
  return outputLikelihood;
}

//**************************************************************************
double P0DMuonProtonDisc::GetLikelihoodFineBeg(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength){  
//**************************************************************************

  double overallDistance  = 0.0;
  double outputLikelihood = 0.0;

  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);
  double trackangle = trackDir.Theta()*180/TMath::Pi();  

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle>=0&&trackangle<=30) anglebin=1;
  else if(trackangle>30&&trackangle<=60) anglebin=2;
  else anglebin=3;

  double binwidthX = fLFine[anglebin]->GetXaxis()->GetBinWidth(5);
  double binwidthY = fLFine[anglebin]->GetYaxis()->GetBinWidth(5);

  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);

  for(int j=0;j<Track->nClusters;j++){
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[j]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j != (Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[j+1]);
      if(!next_n) continue;
    }
    
    TLorentzVector nodePosition = n->Position;
    TVector3 nodeDirection(-9999,-9999,-9999);

    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;
   
    double costheta = nodeDirection.CosTheta();
    double nodeEnergy = n->EDeposit*costheta;
    nodeEnergy = nodeEnergy*energyCorrection;

    if(j!=0){      
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[j-1]);
      if(!n2) continue;

      TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);
      
      //	  double prevnodeEnergy = n2->EDeposit;
      double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));
      overallDistance+=distance;

      if(overallDistance<=useLength){
	      if(isWater){
	        if(fLFine[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1)!=0){
	          outputLikelihood+=TMath::Log(fLFine[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1));
	        }
	        else outputLikelihood+=TMath::Log(0.000000001);
	      }
	      if(!isWater){
	        if(fLFineWaterOut[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1)!=0){
	          outputLikelihood+=TMath::Log(fLFineWaterOut[anglebin]->GetBinContent((int)(overallDistance/binwidthX)+1,(int)(nodeEnergy/binwidthY)+1));
	        }
	        else outputLikelihood+=TMath::Log(0.000000001);
	      }
      }
    }
  }
  return outputLikelihood;
}

///Chi2 PID method access functions
//**************************************************************************
double P0DMuonProtonDisc::GetChi2TrackEnd(AnaP0DParticle *Track,bool isWater,float energyCorrection,float useLength,bool noAngle){
//**************************************************************************

  double overallDistance=0.0;

  vector<float> pullvalues;
  
  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);
  double trackangle = trackDir.Theta()*180/TMath::Pi();

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle>=0&&trackangle<=30) anglebin=1;
  else if(trackangle>30&&trackangle<=60) anglebin=2;
  else anglebin=3;
  if(noAngle) anglebin=0;

  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);

  for(int j=0;j<Track->nClusters;j++){
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-1]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j != (Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-2]);
      if(!next_n) continue;
    }

    TLorentzVector nodePosition = n->Position;
    TVector3 nodeDirection(-9999,-9999,-9999);

    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;

    double costheta = nodeDirection.CosTheta();
    double nodeEnergy = n->EDeposit*costheta;
    nodeEnergy = nodeEnergy*energyCorrection;

    if(j!=0){    
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j]);
      if(!n2) continue;

      TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);
      
      //	  double prevnodeEnergy = n2->EDeposit;
      double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));
      overallDistance+=distance;

      if(overallDistance<=useLength){
	      //Calc x^2/NDOF
	      if(isWater){
	        if(overallDistance>=0.&&overallDistance<=fChi2BinSize){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][0])/fChargeSigmas[anglebin][0];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize&&overallDistance<=fChi2BinSize*2){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][1])/fChargeSigmas[anglebin][1];
	          pullvalues.push_back(pullvaluePDF);
	        }	    
	        if(overallDistance>fChi2BinSize*2&&overallDistance<=fChi2BinSize*3){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][2])/fChargeSigmas[anglebin][2];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*3&&overallDistance<=fChi2BinSize*4){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][3])/fChargeSigmas[anglebin][3];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*4&&overallDistance<=fChi2BinSize*5){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][4])/fChargeSigmas[anglebin][4];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*5&&overallDistance<=fChi2BinSize*6){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][5])/fChargeSigmas[anglebin][5];
	          pullvalues.push_back(pullvaluePDF);
	        }
	      }
	      if(!isWater){
	        if(overallDistance>=0.&&overallDistance<=fChi2BinSize){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][0])/fChargeSigmasWaterOut[anglebin][0];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize&&overallDistance<=fChi2BinSize*2){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][1])/fChargeSigmasWaterOut[anglebin][1];
	          pullvalues.push_back(pullvaluePDF);
	        }	    
	        if(overallDistance>fChi2BinSize*2&&overallDistance<=fChi2BinSize*3){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][2])/fChargeSigmasWaterOut[anglebin][2];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*3&&overallDistance<=fChi2BinSize*4){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][3])/fChargeSigmasWaterOut[anglebin][3];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*4&&overallDistance<=fChi2BinSize*5){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][4])/fChargeSigmasWaterOut[anglebin][4];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*5&&overallDistance<=fChi2BinSize*6){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][5])/fChargeSigmasWaterOut[anglebin][5];
	          pullvalues.push_back(pullvaluePDF);
	        }
	      }
      }
    }
  }

  double outputChi2 = 0.0;
  if(pullvalues.size()!=0) outputChi2 = 0.0;
  else outputChi2 = -999;

  for(uint i=0;i<pullvalues.size();i++){
    outputChi2=outputChi2+pullvalues[i]*pullvalues[i];//Chi^2 method
  }

  return outputChi2/(pullvalues.size()-1);
}

//**************************************************************************
double P0DMuonProtonDisc::GetChi2TrackBeg(AnaP0DParticle *Track,bool isWater,float energyCorrection,float tracklength,bool noAngle){
//**************************************************************************

  double overallDistance = 0.0;

  vector<float> pullvalues;

  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);
  double trackangle = trackDir.Theta()*180/TMath::Pi();

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle>=0&&trackangle<=30) anglebin=1;
  else if(trackangle>30&&trackangle<=60) anglebin=2;
  else anglebin=3;
  if(noAngle) anglebin=0;

  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);

  for(int j=0;j<Track->nClusters;j++){   
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[j]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j != (Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[j+1]);
      if(!next_n) continue;
    }

    TLorentzVector nodePosition = n->Position;
    TVector3 nodeDirection(-9999,-9999,-9999);

    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;
  
    double costheta = nodeDirection.CosTheta();
    double nodeEnergy = n->EDeposit*costheta;
    nodeEnergy = nodeEnergy*energyCorrection;

    if(j!=0){
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[j-1]);
      if(!n2) continue;

      TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);
      
      //	  double prevnodeEnergy = n2->EDeposit;
      double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));
      overallDistance+=distance;

      //Calc x^2/NDOF
      if(overallDistance<=tracklength){
	      if(isWater){
	        if(overallDistance>=0.&&overallDistance<=fChi2BinSize){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][0])/fChargeSigmas[anglebin][0];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize&&overallDistance<=fChi2BinSize*2){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][1])/fChargeSigmas[anglebin][1];
	          pullvalues.push_back(pullvaluePDF);
	        }	    
	        if(overallDistance>fChi2BinSize*2&&overallDistance<=fChi2BinSize*3){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][2])/fChargeSigmas[anglebin][2];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*3&&overallDistance<=fChi2BinSize*4){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][3])/fChargeSigmas[anglebin][3];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*4&&overallDistance<=fChi2BinSize*5){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][4])/fChargeSigmas[anglebin][4];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*5&&overallDistance<=fChi2BinSize*6){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][5])/fChargeSigmas[anglebin][5];
	          pullvalues.push_back(pullvaluePDF);
	        }
	      }
	      if(!isWater){
	        if(overallDistance>=0.&&overallDistance<=fChi2BinSize){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][0])/fChargeSigmasWaterOut[anglebin][0];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize&&overallDistance<=fChi2BinSize*2){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][1])/fChargeSigmasWaterOut[anglebin][1];
	          pullvalues.push_back(pullvaluePDF);
	        }	    
	        if(overallDistance>fChi2BinSize*2&&overallDistance<=fChi2BinSize*3){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][2])/fChargeSigmasWaterOut[anglebin][2];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*3&&overallDistance<=fChi2BinSize*4){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][3])/fChargeSigmasWaterOut[anglebin][3];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*4&&overallDistance<=fChi2BinSize*5){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][4])/fChargeSigmasWaterOut[anglebin][4];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*5&&overallDistance<=fChi2BinSize*6){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][5])/fChargeSigmasWaterOut[anglebin][5];
	          pullvalues.push_back(pullvaluePDF);
	        }
	      }
      }
    }
  }

  double outputChi2 = 0.0;
  if(pullvalues.size()!=0) outputChi2 = 0.0;
  else outputChi2 = -999;
 
  for(uint i=0;i<pullvalues.size();i++){
    outputChi2=outputChi2+pullvalues[i]*pullvalues[i];//Chi^2 method
  }

  return outputChi2/(pullvalues.size()-1);
}

///Pull PID method access functions
//**************************************************************************
double P0DMuonProtonDisc::GetPullTrackEnd(AnaP0DParticle *Track,bool isWater,float energyCorrection,float tracklength,bool noAngle){
//**************************************************************************

  double overallDistance=0.0;

  vector<float> pullvalues;

  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);   
  double trackangle = trackDir.Theta()*180/TMath::Pi();

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle>=0&&trackangle<=30) anglebin=1;
  else if(trackangle>30&&trackangle<=60) anglebin=2;
  else anglebin=3;
  if(noAngle) anglebin=0;

  #ifdef debug
    std::cerr << "P0DMuonProtonDisc::GetPullTrackEnd: anglebin=" << anglebin << std::endl;
  #endif

  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);

  #ifdef debug
    std::cerr << "P0DMuonProtonDisc::GetPullTrackEnd: Track->nClusters=" << Track->nClusters << std::endl;
  #endif

  for(int j=0;j<Track->nClusters;j++){    
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-1]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j != (Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j-2]);
      if(!next_n) continue;
    }

    TLorentzVector nodePosition = n->Position;
    TVector3 nodeDirection(-9999,-9999,-9999);
  
    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      //nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
      nodeDirection = (nodePosition.Vect()-nextNodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;
   
    double costheta = nodeDirection.CosTheta();
    double nodeEnergy = n->EDeposit*costheta;
    nodeEnergy = nodeEnergy*energyCorrection;

    #ifdef debug
      std::cerr << "P0DMuonProtonDisc::GetPullTrackEnd: "
                << "j = " << j
                << ", node# = " << Track->nClusters-j-1
                << ", nodePosition = (" << nodePosition.X() << "," << nodePosition.Y() << "," << nodePosition.Z() << ")" 
                << ", nodeDirection = (" << nodeDirection.X() << "," << nodeDirection.Y() << "," << nodeDirection.Z() << ")" 
                //<< ", prevNodeDirection= (" << prevNodeDirection.X() << "," << prevNodeDirection.Y() << "," << prevNodeDirection.Z() << ")"
                << ",n->EDeposit = " << n->EDeposit << ", costheta = " << costheta << ", nodeEnergy = " << nodeEnergy << std::endl;
    #endif
   
    if(j!=0){	    
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[Track->nClusters-j]);
	    if(!n2) continue;

  	  TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);
	
	    //	  double prevnodeEnergy = n2->EDeposit;
	    double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));
	    overallDistance+=distance;
     
      #ifdef debug
        std::cerr << "P0DMuonProtonDisc::GetPullTrackEnd:" 
                  //<< " node# = " << Track->nClusters-j-1
                  << " prevnode# = " << Track->nClusters-j
                  << ", prevnodePosition = (" << prevnodePosition.X() << "," << prevnodePosition.Y() << "," << prevnodePosition.Z() << ")"                       << ", distance = " << distance << ", ovarallDistance = " << overallDistance << std::endl;
      #endif
 
	    if(overallDistance<=tracklength){
	  
        //Calc x^2/NDOF
	      if(isWater){
          int idx = -1;
	        if(overallDistance>=0.&&overallDistance<=fChi2BinSize){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][0])/fChargeSigmas[anglebin][0];
	          pullvalues.push_back(pullvaluePDF);
            idx=0;
	        }
	        if(overallDistance>fChi2BinSize&&overallDistance<=fChi2BinSize*2){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][1])/fChargeSigmas[anglebin][1];
	          pullvalues.push_back(pullvaluePDF);
            idx=1;
	        }	    
	        if(overallDistance>fChi2BinSize*2&&overallDistance<=fChi2BinSize*3){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][2])/fChargeSigmas[anglebin][2];
	          pullvalues.push_back(pullvaluePDF);
            idx=2;
	        }
	        if(overallDistance>fChi2BinSize*3&&overallDistance<=fChi2BinSize*4){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][3])/fChargeSigmas[anglebin][3];
	          pullvalues.push_back(pullvaluePDF);
            idx=3;
	        }
	        if(overallDistance>fChi2BinSize*4&&overallDistance<=fChi2BinSize*5){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][4])/fChargeSigmas[anglebin][4];
	          pullvalues.push_back(pullvaluePDF);
            idx=4;
	        }
	        if(overallDistance>fChi2BinSize*5&&overallDistance<=fChi2BinSize*6){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][5])/fChargeSigmas[anglebin][5];
	          pullvalues.push_back(pullvaluePDF);
            idx=5;
	        }
          if(idx!=-1){
            #ifdef debug
              std::cerr << "====> P0DMuonProtonDisc::GetPullTrackEnd:" 
                        << "Bin=" << idx << ", pullvaluePDF = (" << nodeEnergy << "-" << fChargeMeans[anglebin][idx] << ")/" << fChargeSigmas[anglebin][idx]
                        << " = " << (nodeEnergy-fChargeMeans[anglebin][idx])/fChargeSigmas[anglebin][idx] << std::endl;

            #endif
          }
	      }
	      if(!isWater){
	        if(overallDistance>=0.&&overallDistance<=fChi2BinSize){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][0])/fChargeSigmasWaterOut[anglebin][0];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize&&overallDistance<=fChi2BinSize*2){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][1])/fChargeSigmasWaterOut[anglebin][1];
	          pullvalues.push_back(pullvaluePDF);
	        }	    
	        if(overallDistance>fChi2BinSize*2&&overallDistance<=fChi2BinSize*3){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][2])/fChargeSigmasWaterOut[anglebin][2];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*3&&overallDistance<=fChi2BinSize*4){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][3])/fChargeSigmasWaterOut[anglebin][3];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*4&&overallDistance<=fChi2BinSize*5){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][4])/fChargeSigmasWaterOut[anglebin][4];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*5&&overallDistance<=fChi2BinSize*6){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][5])/fChargeSigmasWaterOut[anglebin][5];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        }
	      }
      }
    }

  double outputChi2 = 0.0;
  if(pullvalues.size()!=0) outputChi2 = 0.0;
  else outputChi2 = -999;

  for(uint i=0;i<pullvalues.size();i++){
    #ifdef debug
      std::cout << "P0DMuonProtonDisc::GetPullTrackEnd: pullvalues["<<i<<"]=" << pullvalues[i] 
                << ", outputChi=" << outputChi2 << std::endl;
    #endif

    outputChi2=outputChi2+pullvalues[i];//pull method
  }

 return outputChi2;
}

//**************************************************************************
double P0DMuonProtonDisc::GetPullTrackBeg(AnaP0DParticle *Track,bool isWater,float energyCorrection,float tracklength,bool noAngle){
//**************************************************************************

  double overallDistance=0.0;
  vector<float> pullvalues;

  TVector3 trackDir = anaUtils::ArrayToTVector3(Track->DirectionStart);
  double trackangle = trackDir.Theta()*180/TMath::Pi();

  //figure out angle bin for PDF
  int anglebin=-1;
  if(trackangle>=0&&trackangle<=30) anglebin=1;
  else if(trackangle>30&&trackangle<=60) anglebin=2;
  else anglebin=3;
  if(noAngle) anglebin=0;
 
  //This is a direction for a first node of the track which is assumed to be the same as for previous node
  TVector3 prevNodeDirection(-9999,-9999,-9999);
 
  for(int j=0;j<Track->nClusters;j++){  
    AnaP0DCluster* n = static_cast<AnaP0DCluster*>(Track->Clusters[j]);
    if(!n) continue;

    AnaP0DCluster* next_n = NULL;
    if(j != (Track->nClusters-1)){
      next_n = static_cast<AnaP0DCluster*>(Track->Clusters[j+1]);
      if(!next_n) continue;
    }

    TLorentzVector nodePosition = n->Position;
    TVector3 nodeDirection(-9999,-9999,-9999);
  
    //Get node direction.    
    if(j!=(Track->nClusters-1)){
      TLorentzVector nextNodePosition = anaUtils::ArrayToTLorentzVector(next_n->Position);
        
      //Direction is estimated from the positions of the node and next node
      nodeDirection = (nextNodePosition.Vect()-nodePosition.Vect()).Unit();
    }
    else // In case of the first node assume the direction is the same as the direction for the previous node.
      nodeDirection = prevNodeDirection;

    //Remember previous node direction
    prevNodeDirection = nodeDirection;
   
    double costheta = nodeDirection.CosTheta();
    double nodeEnergy = n->EDeposit*costheta;

    nodeEnergy = nodeEnergy*energyCorrection;

    if(j!=0){	    
      AnaP0DCluster* n2 = static_cast<AnaP0DCluster*>(Track->Clusters[j-1]);
	    if(!n2) continue;

	    TLorentzVector prevnodePosition = anaUtils::ArrayToTLorentzVector(n2->Position);
	
	    //	  double prevnodeEnergy = n2->EDeposit;
	    double distance=TMath::Sqrt((nodePosition.X()-prevnodePosition.X())*(nodePosition.X()-prevnodePosition.X())+
				                          (nodePosition.Y()-prevnodePosition.Y())*(nodePosition.Y()-prevnodePosition.Y())+
				                          (nodePosition.Z()-prevnodePosition.Z())*(nodePosition.Z()-prevnodePosition.Z()));

	    overallDistance+=distance;
   
	    //Calc x^2/NDOF
	    if(overallDistance<=tracklength){
	      if(isWater){
	        if(overallDistance>=0.&&overallDistance<=fChi2BinSize){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][0])/fChargeSigmas[anglebin][0];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize&&overallDistance<=fChi2BinSize*2){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][1])/fChargeSigmas[anglebin][1];
	          pullvalues.push_back(pullvaluePDF);
	        }	    
	        if(overallDistance>fChi2BinSize*2&&overallDistance<=fChi2BinSize*3){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][2])/fChargeSigmas[anglebin][2];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*3&&overallDistance<=fChi2BinSize*4){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][3])/fChargeSigmas[anglebin][3];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*4&&overallDistance<=fChi2BinSize*5){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][4])/fChargeSigmas[anglebin][4];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*5&&overallDistance<=fChi2BinSize*6){
	          float pullvaluePDF = (nodeEnergy-fChargeMeans[anglebin][5])/fChargeSigmas[anglebin][5];
	          pullvalues.push_back(pullvaluePDF);
	        }
	      }
	      if(!isWater){
	        if(overallDistance>=0.&&overallDistance<=fChi2BinSize){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][0])/fChargeSigmasWaterOut[anglebin][0];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize&&overallDistance<=fChi2BinSize*2){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][1])/fChargeSigmasWaterOut[anglebin][1];
	          pullvalues.push_back(pullvaluePDF);
	        }	    
	        if(overallDistance>fChi2BinSize*2&&overallDistance<=fChi2BinSize*3){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][2])/fChargeSigmasWaterOut[anglebin][2];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*3&&overallDistance<=fChi2BinSize*4){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][3])/fChargeSigmasWaterOut[anglebin][3];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*4&&overallDistance<=fChi2BinSize*5){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][4])/fChargeSigmasWaterOut[anglebin][4];
	          pullvalues.push_back(pullvaluePDF);
	        }
	        if(overallDistance>fChi2BinSize*5&&overallDistance<=fChi2BinSize*6){
	          float pullvaluePDF = (nodeEnergy-fChargeMeansWaterOut[anglebin][5])/fChargeSigmasWaterOut[anglebin][5];
	          pullvalues.push_back(pullvaluePDF);
	        }
	      }
	    }
    }
  }

  double outputChi2 = 0.0;
  if(pullvalues.size()!=0) outputChi2 = 0.0;
  else outputChi2 = -999;

  for(uint i=0;i<pullvalues.size();i++){
     outputChi2=outputChi2+pullvalues[i];//pull method
  }
  return outputChi2;
}

//*******************************************************************************************************************
void P0DMuonProtonDisc::loadPIDValues(vector< vector<float> > &pidconst,
                                    vector< vector<float> > &pidmean,
                                    vector< vector<float> >&pidsigma,
                                    string filename, 
                                    int binnumber){
//*******************************************************************************************************************
  ifstream pidfile(filename.c_str());
  float temp[3];
  for(int angle=0;angle<4;angle++){
    vector<float> tempconst;
    vector<float> tempmean;
    vector<float> tempsigma;
    for(int i=0;i<binnumber;i++){
      pidfile >> temp[0] >> temp[1] >> temp[2];
      tempconst.push_back(temp[0]);
      tempmean.push_back(temp[1]);
      tempsigma.push_back(temp[2]);
    }
    pidconst.push_back(tempconst);
    pidmean.push_back(tempmean);
    pidsigma.push_back(tempsigma);
  }
  pidfile.close();
}

//*********************************************************
bool P0DMuonProtonDisc::CheckIfFileExists(string filename){
//*********************************************************
  bool fileexists = false;
  ifstream testfile(filename.c_str());
  if(testfile) fileexists = true;
 
  testfile.close();
  return fileexists;
}

//**************************************************************
string P0DMuonProtonDisc::GetFileSoftwareVersion(string filename){
//**************************************************************
  string outputstring("NULL");
 
  TFile *tempfile = new TFile(filename.c_str());
  if(!tempfile) return outputstring;

  TTree *temptree = (TTree*)tempfile->Get("HeaderDir/BasicHeader");
  if(!temptree) return outputstring;

  char version[50];
  temptree->SetBranchAddress("SoftwareVersion",&version);
  temptree->GetEntry(0);
  //string outputstring = version;
  outputstring = version;

  tempfile->Close();
  return outputstring;
}
