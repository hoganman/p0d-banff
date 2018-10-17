#include "p0dRatioUtils.hxx"


namespace p0dRatioUtils  {}


//********************************************************************
int p0dRatioUtils::TagMichel(AnaP0DVertex** p0dVertices, int nVerts, int* nMichelOut){
  //********************************************************************
  /*Usage: input is the vector of p0dVertecies and the number of vertecies for the event gotten by: 
    AnaP0DVertex* p0dVertices[NMAXVERTICES];
    Int_t nVerts = p0dUtils::GetAllP0DVertices(GetEvent(),p0dVertices);
    output is a vector of ints the 0 index is the number of muon decays counted by algorithm: TP0DMuonDecayTag 
    the 1 index is the same as the 0 index but counted by algorithm: TP0DTagMuonDecay 
Author: Thomas Campbell <thomascampbell1@gmail.com>
   */
  //int nMichelOut[2];
  int nMichel1=0;
  int nMichel2=0;

  //loop over vertecies in the event
  for(Int_t iv=0;iv<nVerts; iv++ ){
    nMichel1=0;
    nMichel2=0;
    AnaP0DVertex* p0dVertex = p0dVertices[iv];
    // ------ Loop over clusters in the vertex ----------      
    for(Int_t ic=0;ic<p0dVertex->nClusters; ic++ ){
      AnaP0DCluster* p0dCluster = static_cast<AnaP0DCluster*>(p0dVertex->Clusters[ic]);
      //new TagMichel Fill
      if(p0dCluster->AlgorithmName.compare("TP0DMuonDecayTag") == 0){
	//cout << "TP0DMuonDecayTag" << p0dCluster->AlgorithmName << endl;
	nMichel1++;
      }
      else if(p0dCluster->AlgorithmName.compare("TP0DTagMuonDecay") == 0){
	nMichel2++;
      }
    }
  }
  nMichelOut[0]=nMichel1;
  nMichelOut[1]=nMichel2;

  return 0;
}

//********************************************************************
int p0dRatioUtils::MipPID(AnaP0DVertex** p0dVertices, int nVerts, p0dRatioPID* p0dRatioPIDOut, int nTracks, float* temp_selmu_pos){
//********************************************************************
  /*
Usage:This function will take in a p0d vertex and write information relevant to a "poor man's PID" of p0d tracks in 
the event.  
The current implementation is to sort all tracks in event by the "average middle of track charge deposit ratio" to 
the selected muon.  The user can then decide how many sorted output tracks are written.  The sorted structure was 
developed with the intent of rejecting pions (mips will be the first sorted entries provided that everything works, 
so one really only needs to look at
the second entry in the written output to ensure there is only one mip).  

input is the vector of p0dVertecies and the number of vertecies for the event gotten by: 
AnaP0DVertex* p0dVertices[NMAXVERTICES];
Int_t nVerts = p0dUtils::GetAllP0DVertices(GetEvent(),p0dVertices);

other input is an array of p0dRatioPIDs to be filled.  This stores the various relevant variable one might want to write
to the highland output tree
nTracks is the desired number of tracks to look at (sorted)
output will be indicate sucess.  meaningless for now
asdf
Author: Thomas Campbell <thomascampbell1@gmail.com>
   */

  int eCalVetoOut=0;

  for(Int_t iv=0;iv<nVerts; iv++ ){
    AnaP0DVertex* p0dVertex = p0dVertices[iv];

    int tempNP = p0dVertex->nParticles;
    int tempNShowers=0;
    

    float *TrkChargeAve = new float [tempNP];
    float *tempTrkChargeRatioPDGs = new float [tempNP];
    float *tempTrkChargeRatioNClusters = new float [tempNP];
    float *TrkChargeErr = new float [tempNP];
    float *tempTrkChargeRatioPDGsOut = new float [tempNP];
    float *tempTrkChargeRatioNClustersOut = new float [tempNP];
    float *TrkChargeErrOut = new float [tempNP];

    //Position stuff
    Float_t** TrkChargePos = new Float_t* [tempNP];
    Float_t** TrkChargePosOut = new Float_t* [tempNP];

    float TrkChargeMin = 0;
    for(int i=0; i<tempNP; i++){
      TrkChargePos[i] = new Float_t[4];
      TrkChargePosOut[i] = new Float_t[4];
    }

    // ------ Loop over particles in the vertex ----------
    for(Int_t ip=0;ip<p0dVertex->nParticles; ip++ ){
      // Get the P0DParticle from the track in the vertex. That means either the P0Donly track or the segment inside a global track
      AnaP0DParticle* p0dParticle = p0dUtils::GetP0DParticle(p0dVertex->Particles[ip]);
      if (!p0dParticle) continue;

      //Added
      //cout << p0dParticle->Type << endl;
      if((p0dParticle->Type)==1) tempNShowers++;
      //endAdded


      if (p0dParticle->TrueObject){
	tempTrkChargeRatioPDGs[ip]=p0dParticle->GetTrueParticle()->PDG;
      }
      else{
	tempTrkChargeRatioPDGs[ip]=-9999;
      }





      // ------ Loop over clusters in the particle ----------      
      int nhits=0;
      int nClustersInParticle=p0dParticle->nClusters;
      tempTrkChargeRatioNClusters[ip]=nClustersInParticle;
      float* tempChargeVec = new float[nClustersInParticle];
      Float_t* tempTrkChargePos = new Float_t[4];

      for(Int_t ic=0;ic<p0dParticle->nClusters; ic++ ){
	float tempHitChargeAve=0.;
	float tempHitChargeTotal=0.;
	AnaP0DCluster* p0dCluster = static_cast<AnaP0DCluster*>(p0dParticle->Clusters[ic]);


	float* tempNodePosDiff = new float[4];
	float tempNodeCos=-1;
	if(ic<p0dParticle->nClusters-1){
	  AnaP0DCluster* p0dClusterNext = static_cast<AnaP0DCluster*>(p0dParticle->Clusters[ic+1]);

	  float* tempNodePos = new float[4];
	  float* tempNodePosNext = new float[4];
	  tempNodePos = p0dCluster->Position;
	  if(ic==0) tempTrkChargePos = p0dCluster->Position;
	  tempNodePosNext = p0dClusterNext->Position;
	  for(int i=0; i<4; i++){
	    tempNodePosDiff[i]=(tempNodePosNext[i]-tempNodePos[i]);
	  }
	  tempNodeCos = TMath::Abs(tempNodePosDiff[2]/TMath::Sqrt(tempNodePosDiff[1]*tempNodePosDiff[1]+tempNodePosDiff[2]*tempNodePosDiff[2]));
	}
	else tempNodeCos=1.0;

	float nHitsInCluster=p0dCluster->nHits;
	// loop over the hits
	for(Int_t ih=0;ih<p0dCluster->nHits; ih++ ){
	  if (nhits>=200) break;
	  AnaP0DHit* p0dHit = static_cast<AnaP0DHit*>(p0dCluster->Hits[ih]);
	  tempHitChargeTotal+=p0dHit->Charge;
	  nhits++;
	}//Hits in Cluster in particle in vertex
	//tempHitChargeAve=tempHitChargeTotal/(nHitsInCluster*1.0);
	tempHitChargeAve=tempHitChargeTotal/(nHitsInCluster*1.0)*tempNodeCos;
	tempChargeVec[ic]=tempHitChargeAve;
	//cout << "tempHitChargeAve: " << tempHitChargeAve << endl;
	//ecal veto
	Float_t* tempVetoZ = new Float_t[4];
	tempVetoZ = p0dCluster->Position;
	if(tempVetoZ[2]<-2970.0){
	  eCalVetoOut++;
	}
	//end ecal veto
      }//Clusters in particle in vertex

      float tempTrackChargeAve=0.;
      float tempTrackChargeRMS=0.;
      float tempTrackChargeErr=0.;
      float tempTrackChargeTotal=0.;
      float aveCounter=0.;
      for(int tmpi=(nClustersInParticle/4); tmpi<((nClustersInParticle*3)/4); tmpi++){
	tempTrackChargeTotal+=tempChargeVec[tmpi];
	aveCounter+=1.0;
      }
      if(aveCounter>0){
	tempTrackChargeAve=tempTrackChargeTotal/aveCounter;
      }
      else{
	tempTrackChargeAve=555;
      }
      //could calculate RMS here
      for(int tmpi=(nClustersInParticle/4); tmpi<((nClustersInParticle*3)/4); tmpi++){
	tempTrackChargeRMS+=(tempChargeVec[tmpi]-tempTrackChargeAve)*(tempChargeVec[tmpi]-tempTrackChargeAve);
	//cout << (tempChargeVec[tmpi]-tempTrackChargeAve)*(tempChargeVec[tmpi]-tempTrackChargeAve) << endl;
      }
      if(aveCounter>0){
	//tempTrackChargeErr=tempTrackChargeRMS*tempTrackChargeRMS/(aveCounter*aveCounter*aveCounter);
	tempTrackChargeErr=TMath::Sqrt(tempTrackChargeRMS/(aveCounter*aveCounter))/tempTrackChargeAve;
      }
      else{
	tempTrackChargeErr=0.0;
      }


      TrkChargeAve[ip]=tempTrackChargeAve;
      TrkChargeErr[ip]=tempTrackChargeErr;
      tempTrkChargePos = p0dParticle->PositionStart;
      for(int i=0; i<4; i++){
	TrkChargePos[ip][i]=tempTrkChargePos[i];
      }
    }//particles in vertex

    //Find minimum TrkAve (out dated)

    TrkChargeMin=TrkChargeAve[0];
    for(int iMin=0; iMin<tempNP; iMin++){
      if(TrkChargeAve[iMin]<TrkChargeMin){
	TrkChargeMin=TrkChargeAve[iMin];
      }
    }
    float* TrkChargeRatioFill = new float[tempNP];
    float* CloseTrkChargeRatioFill = new float[tempNP];
    for(int iRatio=0; iRatio<tempNP; iRatio++){
      TrkChargeRatioFill[iRatio]=TrkChargeAve[iRatio]/TrkChargeMin;
      CloseTrkChargeRatioFill[iRatio]=0.0;
    }

    //match with the most clusters (hits) is the match
    //or just search for MIPs near selmu_pos??
    ////here, just check that ratio is less than 1.5*(ratio for match), if no match, use minimum ratio
    float nMuLikeCloseOut=0.0;
    if(tempNP<=1) nMuLikeCloseOut=1.0;
    else{ 
      int MatchIndex=-1;
      int MatchesFound=0;
      for(int iMatch=0; iMatch<tempNP; iMatch++){
	float matchDiff=0.0;
	for(int i=0; i<3; i++) matchDiff+=(TrkChargePos[iMatch][i]-temp_selmu_pos[i])*(TrkChargePos[iMatch][i]-temp_selmu_pos[i]);
	if(matchDiff<2500){ 
	  MatchIndex=iMatch;
	  MatchesFound++;
	  if(TrkChargeRatioFill[iMatch]<1.5){
	    nMuLikeCloseOut+=1.0;
	    CloseTrkChargeRatioFill[iMatch]=TrkChargeRatioFill[iMatch];
	  }
	}
      }
    }
    if(nMuLikeCloseOut==0) nMuLikeCloseOut=1.0;




    //sort the ratios
    float tempMin=9999;
    float tempMax=-1;
    float tempMinHold=-999;
    int iFoundMin=-1;
    int iFoundMax=-1;
    float* tempSortedChargeRatio = new float[tempNP];

    for(int SortCounter=0; SortCounter<tempNP/2; SortCounter++){
      for(int i=0; i<tempNP; i++){
	if(TrkChargeRatioFill[i]<=tempMin && TrkChargeRatioFill[i]!=-555){
	  tempMin=TrkChargeRatioFill[i];
	  iFoundMin=i;
	}
	if(TrkChargeRatioFill[i]>=tempMax && TrkChargeRatioFill[i]!=-555){
	  tempMax=TrkChargeRatioFill[i];
	  iFoundMax=i;
	}
      }
      tempSortedChargeRatio[SortCounter]=tempMin;
      tempTrkChargeRatioPDGsOut[SortCounter]=tempTrkChargeRatioPDGs[iFoundMin];
      tempTrkChargeRatioNClustersOut[SortCounter]=tempTrkChargeRatioNClusters[iFoundMin];
      TrkChargeErrOut[SortCounter]=TrkChargeErr[iFoundMin];
      //TrkChargePosOut[SortCounter]=TrkChargePos[iFoundMin];
      if(iFoundMin>=0){
	for(int i=0; i<4; i++) TrkChargePosOut[SortCounter][i] = TrkChargePos[iFoundMin][i];
      }
      tempSortedChargeRatio[tempNP-SortCounter-1]=tempMax;
      tempTrkChargeRatioPDGsOut[tempNP-SortCounter-1]=tempTrkChargeRatioPDGs[iFoundMax];
      tempTrkChargeRatioNClustersOut[tempNP-SortCounter-1]=tempTrkChargeRatioNClusters[iFoundMax];
      TrkChargeErrOut[tempNP-SortCounter-1]=TrkChargeErr[iFoundMax];
      if(iFoundMax>=0){
	for (int i=0; i<4; i++) TrkChargePosOut[tempNP-SortCounter-1][i]=TrkChargePos[iFoundMax][i];
      }
      //flip min and max
      tempMinHold=tempMin;
      tempMin=tempMax;
      tempMax=tempMinHold;
      TrkChargeRatioFill[iFoundMin]=-555;
      TrkChargeRatioFill[iFoundMax]=-555;
      //loop again
      int tempOdd=tempNP/2;
      if(!(SortCounter+1)<(tempNP/2)){
	if((tempNP%2)==1){
	  for(int i=0; i<tempNP; i++){
	    if(TrkChargeRatioFill[i]!=-555){
	      tempSortedChargeRatio[tempOdd]=TrkChargeRatioFill[i];
	      tempTrkChargeRatioPDGsOut[tempOdd]=tempTrkChargeRatioPDGs[i];
	      tempTrkChargeRatioNClustersOut[tempOdd]=tempTrkChargeRatioNClusters[i];
	      TrkChargeErrOut[tempOdd]=TrkChargeErr[i];
	      if(iFoundMax>=0){
		for (int y=0; y<4; y++) TrkChargePosOut[tempOdd][y]=TrkChargePos[i][y];
	      }

	    }
	  }
	}
      }
    }//Sort Counter

    if(tempNP==1){
      tempSortedChargeRatio[0]=1.0;
      tempTrkChargeRatioPDGsOut[0]=-9999;
    }


    //cout << "New fill" << endl;
    for(int i=0; i<nTracks; i++){
      if(i<tempNP){
	//if(tempNP>1) cout << tempSortedChargeRatio[i] << " " << TrkChargePosOut[i][0] << " " << TrkChargePosOut[i][1] << " " << TrkChargePosOut[i][2] << endl;
	p0dRatioPIDOut[i].ChargeRatio = tempSortedChargeRatio[i];
	p0dRatioPIDOut[i].CloseChargeRatio = CloseTrkChargeRatioFill[i];
	p0dRatioPIDOut[i].ChargeMin = TrkChargeMin;
	p0dRatioPIDOut[i].nMuLike = nMuLikeCloseOut;
	p0dRatioPIDOut[i].ChargeRatioErr = TrkChargeErrOut[i];
	p0dRatioPIDOut[i].PosX = TrkChargePosOut[i][0];
	p0dRatioPIDOut[i].PosY = TrkChargePosOut[i][1];
	p0dRatioPIDOut[i].PosZ = TrkChargePosOut[i][2];
	p0dRatioPIDOut[i].PDG = tempTrkChargeRatioPDGsOut[i];
	p0dRatioPIDOut[i].nClusters = tempTrkChargeRatioNClustersOut[i];
	p0dRatioPIDOut[i].nShowers = tempNShowers;
	p0dRatioPIDOut[i].eCalVeto = eCalVetoOut;
      }
      //else{
	//TrkChargeRatioOut[i]=555.;
	//p0dRatioPIDOut[i].ChargeRatio = 555.;
	//TrkChargeRatioPDGsOut[i]=-9999;
	//p0dRatioPIDOut[i].PDG = -9999;
      //}
    }
  }//vertecies

  return 0;
}//fxn
