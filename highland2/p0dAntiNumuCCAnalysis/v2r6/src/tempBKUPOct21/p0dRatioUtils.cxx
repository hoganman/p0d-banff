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
int p0dRatioUtils::MipPID(AnaP0DVertex** p0dVertices, int nVerts, float* TrkChargeRatioOut, float* TrkChargeRatioPDGsOut, int nTracks){
//********************************************************************
  /*Usage: input is the vector of p0dVertecies and the number of vertecies for the event gotten by: 
    AnaP0DVertex* p0dVertices[NMAXVERTICES];
    Int_t nVerts = p0dUtils::GetAllP0DVertices(GetEvent(),p0dVertices);
    other input is the pointer to the array of track charge deposit ratios to be filled 
    and the number of desired tracks to fill
    more input will be a pointer to the arraw of track PDGs to be filled
    output will be indicate sucess.  meaningless for now
Author: Thomas Campbell <thomascampbell1@gmail.com>
   */
   

  for(Int_t iv=0;iv<nVerts; iv++ ){
    AnaP0DVertex* p0dVertex = p0dVertices[iv];

    int tempNP = p0dVertex->nParticles;
    float *TrkChargeAve = new float [tempNP];
    float *tempTrkChargeRatioPDGs = new float [tempNP];
    float *tempTrkChargeRatioPDGsOut = new float [tempNP];
    // ------ Loop over particles in the vertex ----------
    for(Int_t ip=0;ip<p0dVertex->nParticles; ip++ ){
      // Get the P0DParticle from the track in the vertex. That means either the P0Donly track or the segment inside a global track
      AnaP0DParticle* p0dParticle = p0dUtils::GetP0DParticle(p0dVertex->Particles[ip]);
      if (!p0dParticle) continue;


      if (p0dParticle->TrueObject){
	tempTrkChargeRatioPDGs[ip]=p0dParticle->GetTrueParticle()->PDG;
      }
      else{
	tempTrkChargeRatioPDGs[ip]=-9999;
      }




      // ------ Loop over clusters in the particle ----------      
      int nhits=0;
      int nClustersInParticle=p0dParticle->nClusters;
      float* tempChargeVec = new float[nClustersInParticle];

      for(Int_t ic=0;ic<p0dParticle->nClusters; ic++ ){
	float tempHitChargeAve=0.;
	float tempHitChargeTotal=0.;
	AnaP0DCluster* p0dCluster = static_cast<AnaP0DCluster*>(p0dParticle->Clusters[ic]);

	float nHitsInCluster=p0dCluster->nHits;
	// loop over the hits
	for(Int_t ih=0;ih<p0dCluster->nHits; ih++ ){
	  if (nhits>=200) break;
	  AnaP0DHit* p0dHit = static_cast<AnaP0DHit*>(p0dCluster->Hits[ih]);
	  tempHitChargeTotal+=p0dHit->Charge;
	  nhits++;
	}//Hits in Cluster in particle in vertex
	tempHitChargeAve=tempHitChargeTotal/(nHitsInCluster*1.0);
	tempChargeVec[ic]=tempHitChargeAve;
      }//Clusters in particle in vertex
      float tempTrackChargeAve=0.;
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
      TrkChargeAve[ip]=tempTrackChargeAve;
    }//particles in vertex

    //Find minimum TrkAve

    float TrkChargeMin=TrkChargeAve[0];
    for(int iMin=0; iMin<tempNP; iMin++){
      if(TrkChargeAve[iMin]<TrkChargeMin){
	TrkChargeMin=TrkChargeAve[iMin];
      }
    }
    float* TrkChargeRatioFill = new float[tempNP];
    for(int iRatio=0; iRatio<tempNP; iRatio++){
      TrkChargeRatioFill[iRatio]=TrkChargeAve[iRatio]/TrkChargeMin;
    }
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
      tempSortedChargeRatio[tempNP-SortCounter-1]=tempMax;
      tempTrkChargeRatioPDGsOut[tempNP-SortCounter-1]=tempTrkChargeRatioPDGs[iFoundMax];
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
	    }
	  }
	}
      }
    }//Sort Counter

    if(tempNP==1){
      tempSortedChargeRatio[0]=1.0;
      tempTrkChargeRatioPDGsOut[0]=-9999;
    }

    for(int i=0; i<nTracks; i++){
      if(i<tempNP){
	TrkChargeRatioOut[i]=tempSortedChargeRatio[i];
	TrkChargeRatioPDGsOut[i]=tempTrkChargeRatioPDGsOut[i];
      }
      else{
	TrkChargeRatioOut[i]=555.;
	TrkChargeRatioPDGsOut[i]=-9999;
      }
    }
  }//vertecies

  return 0;
}//fxn
