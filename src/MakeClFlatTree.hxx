//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Oct 12 00:17:09 2017 by ROOT version 5.34/09
// from TTree flattree/7
// found on file: /physics/data/BANFF/asg2018oa/Splines_testing_psyche_v3r26/MCrun5w_Splines.root
//////////////////////////////////////////////////////////

#ifndef MakeClFlatTree_h
#define MakeClFlatTree_h

#include<vector>
#include<string>

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class MakeClFlatTree : public TObject {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           sNBunches;
   Int_t           sNTotalTrueVertices;
   Int_t           sNTotalTrueTracks;
   Int_t           sIsMC;
   Int_t           sIsSand;
   Int_t           sRun;
   Int_t           sSubRun;
   Int_t           sEvt;
   Int_t           sBeamGoodSpill;
   Int_t           sDQGoodDaq;
   Int_t           sTriggerFGDCosmic;
   Int_t           sTriggerTripTCosmic;
   Double_t        sTriggerCTMTriggerPattern[6];
   Int_t           sRooVtxEntry;
   Int_t           sInputFileIndex;
   Double_t        sPOTSincePreviousSpill;
   Int_t           sNTrueVertices;
   Int_t           sTrueVertexID[13];   //[sNTrueVertices]
   Int_t           sTrueVertexNuPDG[13];   //[sNTrueVertices]
   Float_t         sTrueVertexNuEnergy[13];   //[sNTrueVertices]
   Float_t         sTrueVertexPosition[13][4];   //[sNTrueVertices]
   Int_t           sTrueVertexNTrueTracks[13];   //[sNTrueVertices]
   Int_t           sTrueVertexBunch[13];   //[sNTrueVertices]
   Int_t           sTrueVertexDetector[13];   //[sNTrueVertices]
   Float_t         sTrueVertexLeptonDir[13][3];   //[sNTrueVertices]
   Float_t         sTrueVertexLeptonMom[13];   //[sNTrueVertices]
   Int_t           sTrueVertexLeptonPDG[13];   //[sNTrueVertices]
   Int_t           sTrueVertexNBaryons[13];   //[sNTrueVertices]
   Int_t           sTrueVertexNReconTracks[13];   //[sNTrueVertices]
   Int_t           sTrueVertexNReconVertices[13];   //[sNTrueVertices]
   Float_t         sTrueVertexNuDir[13][3];   //[sNTrueVertices]
   Float_t         sTrueVertexNuParentDecPoint[13][4];   //[sNTrueVertices]
   Int_t           sTrueVertexNuParentPDG[13];   //[sNTrueVertices]
   Float_t         sTrueVertexProtonDir[13][3];   //[sNTrueVertices]
   Float_t         sTrueVertexProtonMom[13];   //[sNTrueVertices]
   Float_t         sTrueVertexPionDir[13][3];   //[sNTrueVertices]
   Float_t         sTrueVertexPionMom[13];   //[sNTrueVertices]
   Float_t         sTrueVertexQ2[13];   //[sNTrueVertices]
   Int_t           sTrueVertexReacCode[13];   //[sNTrueVertices]
   Int_t           sTrueVertexRooVtxEntry[13];   //[sNTrueVertices]
   Int_t           sTrueVertexRooVtxIndex[13];   //[sNTrueVertices]
   Float_t         sTrueVertexTargetDir[13][3];   //[sNTrueVertices]
   Float_t         sTrueVertexTargetMom[13];   //[sNTrueVertices]
   Int_t           sTrueVertexTargetPDG[13];   //[sNTrueVertices]
   Int_t           sTrueVertexIsPauliBlocked[13];   //[sNTrueVertices]
   Int_t           sTrueVertexIsCohOnH[13];   //[sNTrueVertices]
   Int_t           sTrueVertexNPrimaryParticles[13][36];   //[sNTrueVertices]
   Int_t           sNTrueTracks;
   Int_t           sTrueTrackPrimaryID[500];   //[sNTrueTracks]
   Int_t           sTrueTrackID[500];   //[sNTrueTracks]
   Int_t           sTrueTrackPDG[500];   //[sNTrueTracks]
   Int_t           sTrueTrackParentPDG[500];   //[sNTrueTracks]
   Int_t           sTrueTrackGParentPDG[500];   //[sNTrueTracks]
   Int_t           sTrueTrackParentID[500];   //[sNTrueTracks]
   Int_t           sTrueTrackGParentID[500];   //[sNTrueTracks]
   Float_t         sTrueTrackMomentum[500];   //[sNTrueTracks]
   Float_t         sTrueTrackCharge[500];   //[sNTrueTracks]
   Float_t         sTrueTrackPosition[500][4];   //[sNTrueTracks]
   Float_t         sTrueTrackPositionEnd[500][4];   //[sNTrueTracks]
   Float_t         sTrueTrackDirection[500][3];   //[sNTrueTracks]
   Int_t           sTrueTrackTruthVertexID[500];   //[sNTrueTracks]
   Int_t           sTrueTrackNDetCrossings[500];   //[sNTrueTracks]
   Int_t           sTrueTrackIsTruePrimaryPi0DecayPhoton[500];   //[sNTrueTracks]
   Int_t           sTrueTrackIsTruePrimaryPi0DecayPhotonChild[500];   //[sNTrueTracks]
   Int_t           sTrueTrackInActive[500][30];   //[sNTrueTracks]
   Int_t           sTrueTrackDetector[500][30];   //[sNTrueTracks]
   Float_t         sTrueTrackEntrancePosition[500][30][4];   //[sNTrueTracks]
   Float_t         sTrueTrackExitPosition[500][30][4];   //[sNTrueTracks]
   Float_t         sTrueTrackEntranceMomentum[500][30][4];   //[sNTrueTracks]
   Float_t         sTrueTrackExitMomentum[500][30][4];   //[sNTrueTracks]
   Float_t         sTrueTrackPurity[500];   //[sNTrueTracks]
   Int_t           sTrueTrackBunch[500];   //[sNTrueTracks]
   Int_t           sTrueTrackNReconTracks[500];   //[sNTrueTracks]
   Int_t           sTrueTrackVertexIndex[500];   //[sNTrueTracks]
   Int_t           sNFgdTimeBins;
   Float_t         sFgdTimeBinMinTime[10];   //[sNFgdTimeBins]
   Int_t           sFgdTimeBinNHits1[10];   //[sNFgdTimeBins]
   Int_t           sFgdTimeBinNHits2[10];   //[sNFgdTimeBins]
   Float_t         sFgdTimeBinRawChargeSum1[10];   //[sNFgdTimeBins]
   Float_t         sFgdTimeBinRawChargeSum2[10];   //[sNFgdTimeBins]
   Int_t           Bunch;
   Float_t         Weight;
   Int_t           NTracks;
   Int_t           trTpcDetector[100][3];   //[NTracks]
   Int_t           trTpcNNodes[100][3];   //[NTracks]
   Int_t           trTpcNHits[100][3];   //[NTracks]
   Int_t           trTpcUniqueID[100][3];   //[NTracks]
   Float_t         trTpcLength[100][3];   //[NTracks]
   Float_t         trTpcDirectionStart[100][3][3];   //[NTracks]
   Float_t         trTpcPositionStart[100][3][4];   //[NTracks]
   Float_t         trTpcPositionEnd[100][3][4];   //[NTracks]
   Float_t         trTpcDirectionEnd[100][3][3];   //[NTracks]
   Float_t         trTpcCharge[100][3];   //[NTracks]
   Float_t         trTpcMomentum[100][3];   //[NTracks]
   Float_t         trTpcMomentumError[100][3];   //[NTracks]
   Float_t         trTpcRefitMomentum[100][3];   //[NTracks]
   Float_t         trTpcEFieldRefitMomentum[100][3];   //[NTracks]
   Float_t         trTpcdEdxMeas[100][3];   //[NTracks]
   Float_t         trTpcdEdxExpMu[100][3];   //[NTracks]
   Float_t         trTpcdEdxExpEle[100][3];   //[NTracks]
   Float_t         trTpcdEdxExpP[100][3];   //[NTracks]
   Float_t         trTpcdEdxExpPi[100][3];   //[NTracks]
   Float_t         trTpcRawdEdxMeas[100][3];   //[NTracks]
   Float_t         trTpcRawdEdxExpMu[100][3];   //[NTracks]
   Float_t         trTpcRawdEdxExpEle[100][3];   //[NTracks]
   Float_t         trTpcRawdEdxExpP[100][3];   //[NTracks]
   Float_t         trTpcRawdEdxExpPi[100][3];   //[NTracks]
   Float_t         trTpcdEdxSigmaMu[100][3];   //[NTracks]
   Float_t         trTpcdEdxSigmaEle[100][3];   //[NTracks]
   Float_t         trTpcdEdxSigmaP[100][3];   //[NTracks]
   Float_t         trTpcdEdxSigmaPi[100][3];   //[NTracks]
   Int_t           trTpcTrueTrackID[100][3];   //[NTracks]
   Float_t         trTpcBackMomentum[100][3];   //[NTracks]
   Float_t         trTpcPullmu[100][3];   //[NTracks]
   Float_t         trTpcPullele[100][3];   //[NTracks]
   Float_t         trTpcPullp[100][3];   //[NTracks]
   Float_t         trTpcPullpi[100][3];   //[NTracks]
   Float_t         trTpcPullk[100][3];   //[NTracks]
   Float_t         trTpcdEdxExpK[100][3];   //[NTracks]
   Float_t         trTpcRawdEdxExpK[100][3];   //[NTracks]
   Float_t         trTpcdEdxSigmaK[100][3];   //[NTracks]
   Float_t         trTpcPurity[100][3];   //[NTracks]
   Int_t           trFgdDetector[100][2];   //[NTracks]
   Int_t           trFgdNNodes[100][2];   //[NTracks]
   Int_t           trFgdNHits[100][2];   //[NTracks]
   Int_t           trFgdUniqueID[100][2];   //[NTracks]
   Float_t         trFgdLength[100][2];   //[NTracks]
   Float_t         trFgdDirectionStart[100][2][3];   //[NTracks]
   Float_t         trFgdPositionStart[100][2][4];   //[NTracks]
   Float_t         trFgdPositionEnd[100][2][4];   //[NTracks]
   Float_t         trFgdDirectionEnd[100][2][3];   //[NTracks]
   Float_t         trFgdX[100][2];   //[NTracks]
   Float_t         trFgdPullmu[100][2];   //[NTracks]
   Float_t         trFgdPullp[100][2];   //[NTracks]
   Float_t         trFgdPullpi[100][2];   //[NTracks]
   Float_t         trFgdPullno[100][2];   //[NTracks]
   Int_t           trFgdContainment[100][2];   //[NTracks]
   Float_t         trFgdE[100][2];   //[NTracks]
   Float_t         trFgdAvgTime[100][2];   //[NTracks]
   Int_t           trFgdHasFgdVA[100][2];   //[NTracks]
   Int_t           trFgdHasFgdVA_fmode[100][2];   //[NTracks]
   Int_t           trFgdTypeVA[100][2];   //[NTracks]
   Float_t         trFgdVertex1x1[100][2];   //[NTracks]
   Float_t         trFgdVertex3x3[100][2];   //[NTracks]
   Float_t         trFgdVertex5x5[100][2];   //[NTracks]
   Float_t         trFgdVertex7x7[100][2];   //[NTracks]
   Float_t         trFgdVertexLayer[100][2];   //[NTracks]
   Int_t           trFgdHasFgdEA[100][2];   //[NTracks]
   Int_t           trFgdHasFgdEA_fmode[100][2];   //[NTracks]
   Int_t           trFgdTypeEA[100][2];   //[NTracks]
   Float_t         trFgdEnd0x0[100][2];   //[NTracks]
   Float_t         trFgdEnd1x1[100][2];   //[NTracks]
   Float_t         trFgdEnd3x3[100][2];   //[NTracks]
   Float_t         trFgdEnd5x5[100][2];   //[NTracks]
   Float_t         trFgdEnd7x7[100][2];   //[NTracks]
   Int_t           trECALDetector[100][9];   //[NTracks]
   Int_t           trECALNNodes[100][9];   //[NTracks]
   Int_t           trECALNHits[100][9];   //[NTracks]
   Int_t           trECALUniqueID[100][9];   //[NTracks]
   Float_t         trECALLength[100][9];   //[NTracks]
   Float_t         trECALDirectionStart[100][9][3];   //[NTracks]
   Float_t         trECALPositionStart[100][9][4];   //[NTracks]
   Float_t         trECALPositionEnd[100][9][4];   //[NTracks]
   Float_t         trECALDirectionEnd[100][9][3];   //[NTracks]
   Float_t         trECALTrShVal[100][9];   //[NTracks]
   Float_t         trECALPIDMipEm[100][9];   //[NTracks]
   Float_t         trECALPIDEmHip[100][9];   //[NTracks]
   Float_t         trECALEMEnergy[100][9];   //[NTracks]
   Float_t         trECALEDeposit[100][9];   //[NTracks]
   Int_t           trECALIsShowerLike[100][9];   //[NTracks]
   Float_t         trECALAvgTime[100][9];   //[NTracks]
   Int_t           trECALMostUpStreamLayerHit[100][9];   //[NTracks]
   Float_t         trECALShowerPosition[100][9][3];   //[NTracks]
   Float_t         trECALPIDMipPion[100][9];   //[NTracks]
   Float_t         trECALContainment[100][9];   //[NTracks]
   Int_t           trSMRDDetector[100][4];   //[NTracks]
   Int_t           trSMRDNNodes[100][4];   //[NTracks]
   Int_t           trSMRDNHits[100][4];   //[NTracks]
   Int_t           trSMRDUniqueID[100][4];   //[NTracks]
   Float_t         trSMRDLength[100][4];   //[NTracks]
   Float_t         trSMRDDirectionStart[100][4][3];   //[NTracks]
   Float_t         trSMRDPositionStart[100][4][4];   //[NTracks]
   Float_t         trSMRDPositionEnd[100][4][4];   //[NTracks]
   Float_t         trSMRDDirectionEnd[100][4][3];   //[NTracks]
   Float_t         trSMRDEDeposit[100][4];   //[NTracks]
   Float_t         trSMRDAvgTime[100][4];   //[NTracks]
   Int_t           trP0DDetector[100][1];   //[NTracks]
   Int_t           trP0DNNodes[100][1];   //[NTracks]
   Int_t           trP0DNHits[100][1];   //[NTracks]
   Int_t           trP0DUniqueID[100][1];   //[NTracks]
   Float_t         trP0DLength[100][1];   //[NTracks]
   Float_t         trP0DDirectionStart[100][1][3];   //[NTracks]
   Float_t         trP0DPositionStart[100][1][4];   //[NTracks]
   Float_t         trP0DPositionEnd[100][1][4];   //[NTracks]
   Float_t         trP0DDirectionEnd[100][1][3];   //[NTracks]
   Float_t         trP0DAvgTime[100][1];   //[NTracks]
   Float_t         trP0DELoss[100][1];   //[NTracks]
   Int_t           trTRACKERDetector[100][8];   //[NTracks]
   Int_t           trTRACKERNNodes[100][8];   //[NTracks]
   Int_t           trTRACKERNHits[100][8];   //[NTracks]
   Int_t           trTRACKERUniqueID[100][8];   //[NTracks]
   Float_t         trTRACKERLength[100][8];   //[NTracks]
   Float_t         trTRACKERDirectionStart[100][8][3];   //[NTracks]
   Float_t         trTRACKERPositionStart[100][8][4];   //[NTracks]
   Float_t         trTRACKERPositionEnd[100][8][4];   //[NTracks]
   Float_t         trTRACKERDirectionEnd[100][8][3];   //[NTracks]
   Float_t         trTRACKERCharge[100][8];   //[NTracks]
   Float_t         trTRACKERMomentum[100][8];   //[NTracks]
   Float_t         trTRACKERMomentumEnd[100][8];   //[NTracks]
   Int_t           trUniqueID[100];   //[NTracks]
   Int_t           trDetUsed[100][19];   //[NTracks]
   Int_t           trNTPCs[100];   //[NTracks]
   Int_t           trNFGDs[100];   //[NTracks]
   Int_t           trNECALs[100];   //[NTracks]
   Int_t           trNSMRDs[100];   //[NTracks]
   Int_t           trNP0Ds[100];   //[NTracks]
   Int_t           trNHits[100];   //[NTracks]
   Float_t         trCharge[100];   //[NTracks]
   Float_t         trMomentum[100];   //[NTracks]
   Float_t         trToFFGD1_FGD2[100];   //[NTracks]
   Float_t         trToFP0D_FGD1[100];   //[NTracks]
   Float_t         trToFDSECal_FGD1[100];   //[NTracks]
   Float_t         trToFECal_FGD1[100];   //[NTracks]
   Float_t         trToFDSECal_FGD2[100];   //[NTracks]
   Float_t         trToFECal_FGD2[100];   //[NTracks]
   Int_t           trToFFlag_FGD1_FGD2[100];   //[NTracks]
   Int_t           trToFFlag_P0D_FGD1[100];   //[NTracks]
   Int_t           trToFFlag_DSECal_FGD1[100];   //[NTracks]
   Int_t           trToFFlag_ECal_FGD1[100];   //[NTracks]
   Int_t           trToFFlag_DSECal_FGD2[100];   //[NTracks]
   Int_t           trToFFlag_ECal_FGD2[100];   //[NTracks]
   Float_t         trRangeMomentumMuon[100];   //[NTracks]
   Float_t         trDirectionStart[100][3];   //[NTracks]
   Float_t         trDirectionEnd[100][3];   //[NTracks]
   Float_t         trPositionStart[100][4];   //[NTracks]
   Float_t         trPositionEnd[100][4];   //[NTracks]
   Float_t         trMomentumFlip[100];   //[NTracks]
   Float_t         trDirectionStartFlip[100][3];   //[NTracks]
   Int_t           trTrueTrackID[100];   //[NTracks]
   Int_t           trStatus[100];   //[NTracks]
   Int_t           trNNodes[100];   //[NTracks]
   Float_t         trMomentumError[100];   //[NTracks]
   Int_t           trNTRACKERs[100];   //[NTracks]
   Int_t           trNDOF[100];   //[NTracks]
   Float_t         trChi2[100];   //[NTracks]
   Float_t         trMomentumMuon[100];   //[NTracks]
   Float_t         trMomentumErrorMuon[100];   //[NTracks]
   Float_t         trMomentumProton[100];   //[NTracks]
   Float_t         trMomentumErrorProton[100];   //[NTracks]
   Float_t         trMomentumEle[100];   //[NTracks]
   Float_t         trMomentumErrorEle[100];   //[NTracks]
   Float_t         trPositionStartFlip[100][4];   //[NTracks]
   Float_t         trDirectionEndFlip[100][3];   //[NTracks]
   Float_t         trRangeMomentumEle[100];   //[NTracks]
   Float_t         trRangeMomentumProton[100];   //[NTracks]
   Float_t         trRangeMomentumMuonFlip[100];   //[NTracks]
   Float_t         trRangeMomentumProtonFlip[100];   //[NTracks]
   Int_t           trReconPDG[100];   //[NTracks]
   Int_t           trBunch[100];   //[NTracks]
   Int_t           trNReconVertices[100];   //[NTracks]
   Float_t         trMomentumAtVertex[100];   //[NTracks]
   Float_t         trDirectionAtVertex[100][3];   //[NTracks]
   Int_t           trNTimeNodes[100];   //[NTracks]
   Int_t           trTimeNodeDetector[100][10];   //[NTracks]
   Float_t         trTimeNodeTimeStart[100][10];   //[NTracks]
   Float_t         trTimeNodeTimeEnd[100][10];   //[NTracks]
   Int_t           trDetectors[100];   //[NTracks]
   Float_t         trLength[100];   //[NTracks]
   Int_t           tecalReconCounter;
   Float_t         tecalReconAverageHitTime[17];   //[tecalReconCounter]
   Float_t         tecalReconAverageZPosition[17];   //[tecalReconCounter]
   Float_t         tecalReconContainment[17];   //[tecalReconCounter]
   Float_t         tecalReconEFitResult[17];   //[tecalReconCounter]
   Float_t         tecalReconEFitUncertainty[17];   //[tecalReconCounter]
   Int_t           tecalReconFirstLayer[17];   //[tecalReconCounter]
   Int_t           tecalReconIsShowerLike[17];   //[tecalReconCounter]
   Int_t           tecalReconIsTrackLike[17];   //[tecalReconCounter]
   Int_t           tecalReconLastLayer[17];   //[tecalReconCounter]
   Float_t         tecalReconLikeMIPEM[17];   //[tecalReconCounter]
   Float_t         tecalReconLikeMIPPion[17];   //[tecalReconCounter]
   Float_t         tecalReconLikeEMHIP[17];   //[tecalReconCounter]
   Float_t         tecalReconLikeMIPEMLow[17];   //[tecalReconCounter]
   Float_t         tecalReconMatchingLike[17];   //[tecalReconCounter]
   Int_t           tecalReconMichelTagNDelayedCluster[17];   //[tecalReconCounter]
   //std::vector<std::string>  *tecalReconModule;
   Int_t           tecalReconMostDownStreamLayerHit[17];   //[tecalReconCounter]
   Int_t           tecalReconMostUpStreamLayerHit[17];   //[tecalReconCounter]
   Int_t           tecalReconNHits[17];   //[tecalReconCounter]
   Int_t           tecalReconNLayersHit[17];   //[tecalReconCounter]
   Float_t         tecalReconObjectLength[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDAMR[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDAngle[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDAsymmetry[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDCircularity[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDFBR[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDMaxRatio[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDMeanPos[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDShowerAngle[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDShowerWidth[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDTransverseChargeRatio[17];   //[tecalReconCounter]
   Float_t         tecalReconPIDTruncatedMaxRatio[17];   //[tecalReconCounter]
   Float_t         tecalReconPointing[17][3];   //[tecalReconCounter]
   Float_t         tecalReconThrust[17];   //[tecalReconCounter]
   Float_t         tecalReconThrustAxis[17][3];   //[tecalReconCounter]
   Float_t         tecalReconThrustOrigin[17][3];   //[tecalReconCounter]
   Int_t           tecalReconTimeBunch[17];   //[tecalReconCounter]
   Float_t         tecalReconTotalHitCharge[17];   //[tecalReconCounter]
   Int_t           tecalReconTrueID[17];   //[tecalReconCounter]
   Int_t           tecalReconTrueIDPrimary[17];   //[tecalReconCounter]
   Int_t           tecalReconTrueIDRecursive[17];   //[tecalReconCounter]
   Int_t           tecalReconTrueIDSingle[17];   //[tecalReconCounter]
   Int_t           tecalReconUniqueID[17];   //[tecalReconCounter]
   Int_t           tecalReconBunch[17];   //[tecalReconCounter]
   Int_t           tecalUnmatchedCounter;
   Float_t         tecalUnmatchedAverageHitTime[5];   //[tecalUnmatchedCounter]
   Float_t         tecalUnmatchedBackPos[5][3];   //[tecalUnmatchedCounter]
   Float_t         tecalUnmatchedFrontPos[5][3];   //[tecalUnmatchedCounter]
   Int_t           tecalUnmatchedMostDownStreamLayerHit[5];   //[tecalUnmatchedCounter]
   Int_t           tecalUnmatchedMostUpStreamLayerHit[5];   //[tecalUnmatchedCounter]
   Int_t           tecalUnmatchedNHits[5];   //[tecalUnmatchedCounter]
   Float_t         tecalUnmatchedTotalHitCharge[5];   //[tecalUnmatchedCounter]
   Int_t           tecalUnmatchedTrueID[5];   //[tecalUnmatchedCounter]
   Int_t           tecalUnmatchedTrueIDPrimary[5];   //[tecalUnmatchedCounter]
   Int_t           tecalUnmatchedTrueIDRecursive[5];   //[tecalUnmatchedCounter]
   Int_t           tecalUnmatchedTrueIDSingle[5];   //[tecalUnmatchedCounter]
   Int_t           tecalUnmatchedView[5];   //[tecalUnmatchedCounter]
   Int_t           tecalUnmatchedBunch[5];   //[tecalUnmatchedCounter]
   Int_t           NVertices;
   Int_t           vPrimaryIndex[14];   //[NVertices]
   Float_t         vPosition[14][4];   //[NVertices]
   Int_t           vNReconTracks[14];   //[NVertices]
   Int_t           vNTrueVerticesMatch[14];   //[NVertices]
   Int_t           vTracksUniqueID[14][7];   //[NVertices]
   Int_t           vTrueVertexIndex[14][3];   //[NVertices]
   Float_t         vTrueVerticesClean[14][3];   //[NVertices]
   Float_t         vTrueVerticesCompl[14][3];   //[NVertices]

   // List of branches
   TBranch        *b_sNBunches;   //!
   TBranch        *b_sNTotalTrueVertices;   //!
   TBranch        *b_sNTotalTrueTracks;   //!
   TBranch        *b_sIsMC;   //!
   TBranch        *b_sIsSand;   //!
   TBranch        *b_sRun;   //!
   TBranch        *b_sSubRun;   //!
   TBranch        *b_sEvt;   //!
   TBranch        *b_sBeamGoodSpill;   //!
   TBranch        *b_sDQGoodDaq;   //!
   TBranch        *b_sTriggerFGDCosmic;   //!
   TBranch        *b_sTriggerTripTCosmic;   //!
   TBranch        *b_sTriggerCTMTriggerPattern;   //!
   TBranch        *b_sRooVtxEntry;   //!
   TBranch        *b_sInputFileIndex;   //!
   TBranch        *b_sPOTSincePreviousSpill;   //!
   TBranch        *b_sNTrueVertices;   //!
   TBranch        *b_sTrueVertexID;   //!
   TBranch        *b_sTrueVertexNuPDG;   //!
   TBranch        *b_sTrueVertexNuEnergy;   //!
   TBranch        *b_sTrueVertexPosition;   //!
   TBranch        *b_sTrueVertexNTrueTracks;   //!
   TBranch        *b_sTrueVertexBunch;   //!
   TBranch        *b_sTrueVertexDetector;   //!
   TBranch        *b_sTrueVertexLeptonDir;   //!
   TBranch        *b_sTrueVertexLeptonMom;   //!
   TBranch        *b_sTrueVertexLeptonPDG;   //!
   TBranch        *b_sTrueVertexNBaryons;   //!
   TBranch        *b_sTrueVertexNReconTracks;   //!
   TBranch        *b_sTrueVertexNReconVertices;   //!
   TBranch        *b_sTrueVertexNuDir;   //!
   TBranch        *b_sTrueVertexNuParentDecPoint;   //!
   TBranch        *b_sTrueVertexNuParentPDG;   //!
   TBranch        *b_sTrueVertexProtonDir;   //!
   TBranch        *b_sTrueVertexProtonMom;   //!
   TBranch        *b_sTrueVertexPionDir;   //!
   TBranch        *b_sTrueVertexPionMom;   //!
   TBranch        *b_sTrueVertexQ2;   //!
   TBranch        *b_sTrueVertexReacCode;   //!
   TBranch        *b_sTrueVertexRooVtxEntry;   //!
   TBranch        *b_sTrueVertexRooVtxIndex;   //!
   TBranch        *b_sTrueVertexTargetDir;   //!
   TBranch        *b_sTrueVertexTargetMom;   //!
   TBranch        *b_sTrueVertexTargetPDG;   //!
   TBranch        *b_sTrueVertexIsPauliBlocked;   //!
   TBranch        *b_sTrueVertexIsCohOnH;   //!
   TBranch        *b_sTrueVertexNPrimaryParticles;   //!
   TBranch        *b_sNTrueTracks;   //!
   TBranch        *b_sTrueTrackPrimaryID;   //!
   TBranch        *b_sTrueTrackID;   //!
   TBranch        *b_sTrueTrackPDG;   //!
   TBranch        *b_sTrueTrackParentPDG;   //!
   TBranch        *b_sTrueTrackGParentPDG;   //!
   TBranch        *b_sTrueTrackParentID;   //!
   TBranch        *b_sTrueTrackGParentID;   //!
   TBranch        *b_sTrueTrackMomentum;   //!
   TBranch        *b_sTrueTrackCharge;   //!
   TBranch        *b_sTrueTrackPosition;   //!
   TBranch        *b_sTrueTrackPositionEnd;   //!
   TBranch        *b_sTrueTrackDirection;   //!
   TBranch        *b_sTrueTrackTruthVertexID;   //!
   TBranch        *b_sTrueTrackNDetCrossings;   //!
   TBranch        *b_sTrueTrackIsTruePrimaryPi0DecayPhoton;   //!
   TBranch        *b_sTrueTrackIsTruePrimaryPi0DecayPhotonChild;   //!
   TBranch        *b_sTrueTrackInActive;   //!
   TBranch        *b_sTrueTrackDetector;   //!
   TBranch        *b_sTrueTrackEntrancePosition;   //!
   TBranch        *b_sTrueTrackExitPosition;   //!
   TBranch        *b_sTrueTrackEntranceMomentum;   //!
   TBranch        *b_sTrueTrackExitMomentum;   //!
   TBranch        *b_sTrueTrackPurity;   //!
   TBranch        *b_sTrueTrackBunch;   //!
   TBranch        *b_sTrueTrackNReconTracks;   //!
   TBranch        *b_sTrueTrackVertexIndex;   //!
   TBranch        *b_sNFgdTimeBins;   //!
   TBranch        *b_sFgdTimeBinMinTime;   //!
   TBranch        *b_sFgdTimeBinNHits1;   //!
   TBranch        *b_sFgdTimeBinNHits2;   //!
   TBranch        *b_sFgdTimeBinRawChargeSum1;   //!
   TBranch        *b_sFgdTimeBinRawChargeSum2;   //!
   TBranch        *b_Bunch;   //!
   TBranch        *b_Weight;   //!
   TBranch        *b_NTracks;   //!
   TBranch        *b_trTpcDetector;   //!
   TBranch        *b_trTpcNNodes;   //!
   TBranch        *b_trTpcNHits;   //!
   TBranch        *b_trTpcUniqueID;   //!
   TBranch        *b_trTpcLength;   //!
   TBranch        *b_trTpcDirectionStart;   //!
   TBranch        *b_trTpcPositionStart;   //!
   TBranch        *b_trTpcPositionEnd;   //!
   TBranch        *b_trTpcDirectionEnd;   //!
   TBranch        *b_trTpcCharge;   //!
   TBranch        *b_trTpcMomentum;   //!
   TBranch        *b_trTpcMomentumError;   //!
   TBranch        *b_trTpcRefitMomentum;   //!
   TBranch        *b_trTpcEFieldRefitMomentum;   //!
   TBranch        *b_trTpcdEdxMeas;   //!
   TBranch        *b_trTpcdEdxExpMu;   //!
   TBranch        *b_trTpcdEdxExpEle;   //!
   TBranch        *b_trTpcdEdxExpP;   //!
   TBranch        *b_trTpcdEdxExpPi;   //!
   TBranch        *b_trTpcRawdEdxMeas;   //!
   TBranch        *b_trTpcRawdEdxExpMu;   //!
   TBranch        *b_trTpcRawdEdxExpEle;   //!
   TBranch        *b_trTpcRawdEdxExpP;   //!
   TBranch        *b_trTpcRawdEdxExpPi;   //!
   TBranch        *b_trTpcdEdxSigmaMu;   //!
   TBranch        *b_trTpcdEdxSigmaEle;   //!
   TBranch        *b_trTpcdEdxSigmaP;   //!
   TBranch        *b_trTpcdEdxSigmaPi;   //!
   TBranch        *b_trTpcTrueTrackID;   //!
   TBranch        *b_trTpcBackMomentum;   //!
   TBranch        *b_trTpcPullmu;   //!
   TBranch        *b_trTpcPullele;   //!
   TBranch        *b_trTpcPullp;   //!
   TBranch        *b_trTpcPullpi;   //!
   TBranch        *b_trTpcPullk;   //!
   TBranch        *b_trTpcdEdxExpK;   //!
   TBranch        *b_trTpcRawdEdxExpK;   //!
   TBranch        *b_trTpcdEdxSigmaK;   //!
   TBranch        *b_trTpcPurity;   //!
   TBranch        *b_trFgdDetector;   //!
   TBranch        *b_trFgdNNodes;   //!
   TBranch        *b_trFgdNHits;   //!
   TBranch        *b_trFgdUniqueID;   //!
   TBranch        *b_trFgdLength;   //!
   TBranch        *b_trFgdDirectionStart;   //!
   TBranch        *b_trFgdPositionStart;   //!
   TBranch        *b_trFgdPositionEnd;   //!
   TBranch        *b_trFgdDirectionEnd;   //!
   TBranch        *b_trFgdX;   //!
   TBranch        *b_trFgdPullmu;   //!
   TBranch        *b_trFgdPullp;   //!
   TBranch        *b_trFgdPullpi;   //!
   TBranch        *b_trFgdPullno;   //!
   TBranch        *b_trFgdContainment;   //!
   TBranch        *b_trFgdE;   //!
   TBranch        *b_trFgdAvgTime;   //!
   TBranch        *b_trFgdHasFgdVA;   //!
   TBranch        *b_trFgdHasFgdVA_fmode;   //!
   TBranch        *b_trFgdTypeVA;   //!
   TBranch        *b_trFgdVertex1x1;   //!
   TBranch        *b_trFgdVertex3x3;   //!
   TBranch        *b_trFgdVertex5x5;   //!
   TBranch        *b_trFgdVertex7x7;   //!
   TBranch        *b_trFgdVertexLayer;   //!
   TBranch        *b_trFgdHasFgdEA;   //!
   TBranch        *b_trFgdHasFgdEA_fmode;   //!
   TBranch        *b_trFgdTypeEA;   //!
   TBranch        *b_trFgdEnd0x0;   //!
   TBranch        *b_trFgdEnd1x1;   //!
   TBranch        *b_trFgdEnd3x3;   //!
   TBranch        *b_trFgdEnd5x5;   //!
   TBranch        *b_trFgdEnd7x7;   //!
   TBranch        *b_trECALDetector;   //!
   TBranch        *b_trECALNNodes;   //!
   TBranch        *b_trECALNHits;   //!
   TBranch        *b_trECALUniqueID;   //!
   TBranch        *b_trECALLength;   //!
   TBranch        *b_trECALDirectionStart;   //!
   TBranch        *b_trECALPositionStart;   //!
   TBranch        *b_trECALPositionEnd;   //!
   TBranch        *b_trECALDirectionEnd;   //!
   TBranch        *b_trECALTrShVal;   //!
   TBranch        *b_trECALPIDMipEm;   //!
   TBranch        *b_trECALPIDEmHip;   //!
   TBranch        *b_trECALEMEnergy;   //!
   TBranch        *b_trECALEDeposit;   //!
   TBranch        *b_trECALIsShowerLike;   //!
   TBranch        *b_trECALAvgTime;   //!
   TBranch        *b_trECALMostUpStreamLayerHit;   //!
   TBranch        *b_trECALShowerPosition;   //!
   TBranch        *b_trECALPIDMipPion;   //!
   TBranch        *b_trECALContainment;   //!
   TBranch        *b_trSMRDDetector;   //!
   TBranch        *b_trSMRDNNodes;   //!
   TBranch        *b_trSMRDNHits;   //!
   TBranch        *b_trSMRDUniqueID;   //!
   TBranch        *b_trSMRDLength;   //!
   TBranch        *b_trSMRDDirectionStart;   //!
   TBranch        *b_trSMRDPositionStart;   //!
   TBranch        *b_trSMRDPositionEnd;   //!
   TBranch        *b_trSMRDDirectionEnd;   //!
   TBranch        *b_trSMRDEDeposit;   //!
   TBranch        *b_trSMRDAvgTime;   //!
   TBranch        *b_trP0DDetector;   //!
   TBranch        *b_trP0DNNodes;   //!
   TBranch        *b_trP0DNHits;   //!
   TBranch        *b_trP0DUniqueID;   //!
   TBranch        *b_trP0DLength;   //!
   TBranch        *b_trP0DDirectionStart;   //!
   TBranch        *b_trP0DPositionStart;   //!
   TBranch        *b_trP0DPositionEnd;   //!
   TBranch        *b_trP0DDirectionEnd;   //!
   TBranch        *b_trP0DAvgTime;   //!
   TBranch        *b_trP0DELoss;   //!
   TBranch        *b_trTRACKERDetector;   //!
   TBranch        *b_trTRACKERNNodes;   //!
   TBranch        *b_trTRACKERNHits;   //!
   TBranch        *b_trTRACKERUniqueID;   //!
   TBranch        *b_trTRACKERLength;   //!
   TBranch        *b_trTRACKERDirectionStart;   //!
   TBranch        *b_trTRACKERPositionStart;   //!
   TBranch        *b_trTRACKERPositionEnd;   //!
   TBranch        *b_trTRACKERDirectionEnd;   //!
   TBranch        *b_trTRACKERCharge;   //!
   TBranch        *b_trTRACKERMomentum;   //!
   TBranch        *b_trTRACKERMomentumEnd;   //!
   TBranch        *b_trUniqueID;   //!
   TBranch        *b_trDetUsed;   //!
   TBranch        *b_trNTPCs;   //!
   TBranch        *b_trNFGDs;   //!
   TBranch        *b_trNECALs;   //!
   TBranch        *b_trNSMRDs;   //!
   TBranch        *b_trNP0Ds;   //!
   TBranch        *b_trNHits;   //!
   TBranch        *b_trCharge;   //!
   TBranch        *b_trMomentum;   //!
   TBranch        *b_trToFFGD1_FGD2;   //!
   TBranch        *b_trToFP0D_FGD1;   //!
   TBranch        *b_trToFDSECal_FGD1;   //!
   TBranch        *b_trToFECal_FGD1;   //!
   TBranch        *b_trToFDSECal_FGD2;   //!
   TBranch        *b_trToFECal_FGD2;   //!
   TBranch        *b_trToFFlag_FGD1_FGD2;   //!
   TBranch        *b_trToFFlag_P0D_FGD1;   //!
   TBranch        *b_trToFFlag_DSECal_FGD1;   //!
   TBranch        *b_trToFFlag_ECal_FGD1;   //!
   TBranch        *b_trToFFlag_DSECal_FGD2;   //!
   TBranch        *b_trToFFlag_ECal_FGD2;   //!
   TBranch        *b_trRangeMomentumMuon;   //!
   TBranch        *b_trDirectionStart;   //!
   TBranch        *b_trDirectionEnd;   //!
   TBranch        *b_trPositionStart;   //!
   TBranch        *b_trPositionEnd;   //!
   TBranch        *b_trMomentumFlip;   //!
   TBranch        *b_trDirectionStartFlip;   //!
   TBranch        *b_trTrueTrackID;   //!
   TBranch        *b_trStatus;   //!
   TBranch        *b_trNNodes;   //!
   TBranch        *b_trMomentumError;   //!
   TBranch        *b_trNTRACKERs;   //!
   TBranch        *b_trNDOF;   //!
   TBranch        *b_trChi2;   //!
   TBranch        *b_trMomentumMuon;   //!
   TBranch        *b_trMomentumErrorMuon;   //!
   TBranch        *b_trMomentumProton;   //!
   TBranch        *b_trMomentumErrorProton;   //!
   TBranch        *b_trMomentumEle;   //!
   TBranch        *b_trMomentumErrorEle;   //!
   TBranch        *b_trPositionStartFlip;   //!
   TBranch        *b_trDirectionEndFlip;   //!
   TBranch        *b_trRangeMomentumEle;   //!
   TBranch        *b_trRangeMomentumProton;   //!
   TBranch        *b_trRangeMomentumMuonFlip;   //!
   TBranch        *b_trRangeMomentumProtonFlip;   //!
   TBranch        *b_trReconPDG;   //!
   TBranch        *b_trBunch;   //!
   TBranch        *b_trNReconVertices;   //!
   TBranch        *b_trMomentumAtVertex;   //!
   TBranch        *b_trDirectionAtVertex;   //!
   TBranch        *b_trNTimeNodes;   //!
   TBranch        *b_trTimeNodeDetector;   //!
   TBranch        *b_trTimeNodeTimeStart;   //!
   TBranch        *b_trTimeNodeTimeEnd;   //!
   TBranch        *b_trDetectors;   //!
   TBranch        *b_trLength;   //!
   TBranch        *b_tecalReconCounter;   //!
   TBranch        *b_tecalReconAverageHitTime;   //!
   TBranch        *b_tecalReconAverageZPosition;   //!
   TBranch        *b_tecalReconContainment;   //!
   TBranch        *b_tecalReconEFitResult;   //!
   TBranch        *b_tecalReconEFitUncertainty;   //!
   TBranch        *b_tecalReconFirstLayer;   //!
   TBranch        *b_tecalReconIsShowerLike;   //!
   TBranch        *b_tecalReconIsTrackLike;   //!
   TBranch        *b_tecalReconLastLayer;   //!
   TBranch        *b_tecalReconLikeMIPEM;   //!
   TBranch        *b_tecalReconLikeMIPPion;   //!
   TBranch        *b_tecalReconLikeEMHIP;   //!
   TBranch        *b_tecalReconLikeMIPEMLow;   //!
   TBranch        *b_tecalReconMatchingLike;   //!
   TBranch        *b_tecalReconMichelTagNDelayedCluster;   //!
   //TBranch        *b_tecalReconModule;   //!
   TBranch        *b_tecalReconMostDownStreamLayerHit;   //!
   TBranch        *b_tecalReconMostUpStreamLayerHit;   //!
   TBranch        *b_tecalReconNHits;   //!
   TBranch        *b_tecalReconNLayersHit;   //!
   TBranch        *b_tecalReconObjectLength;   //!
   TBranch        *b_tecalReconPIDAMR;   //!
   TBranch        *b_tecalReconPIDAngle;   //!
   TBranch        *b_tecalReconPIDAsymmetry;   //!
   TBranch        *b_tecalReconPIDCircularity;   //!
   TBranch        *b_tecalReconPIDFBR;   //!
   TBranch        *b_tecalReconPIDMaxRatio;   //!
   TBranch        *b_tecalReconPIDMeanPos;   //!
   TBranch        *b_tecalReconPIDShowerAngle;   //!
   TBranch        *b_tecalReconPIDShowerWidth;   //!
   TBranch        *b_tecalReconPIDTransverseChargeRatio;   //!
   TBranch        *b_tecalReconPIDTruncatedMaxRatio;   //!
   TBranch        *b_tecalReconPointing;   //!
   TBranch        *b_tecalReconThrust;   //!
   TBranch        *b_tecalReconThrustAxis;   //!
   TBranch        *b_tecalReconThrustOrigin;   //!
   TBranch        *b_tecalReconTimeBunch;   //!
   TBranch        *b_tecalReconTotalHitCharge;   //!
   TBranch        *b_tecalReconTrueID;   //!
   TBranch        *b_tecalReconTrueIDPrimary;   //!
   TBranch        *b_tecalReconTrueIDRecursive;   //!
   TBranch        *b_tecalReconTrueIDSingle;   //!
   TBranch        *b_tecalReconUniqueID;   //!
   TBranch        *b_tecalReconBunch;   //!
   TBranch        *b_tecalUnmatchedCounter;   //!
   TBranch        *b_tecalUnmatchedAverageHitTime;   //!
   TBranch        *b_tecalUnmatchedBackPos;   //!
   TBranch        *b_tecalUnmatchedFrontPos;   //!
   TBranch        *b_tecalUnmatchedMostDownStreamLayerHit;   //!
   TBranch        *b_tecalUnmatchedMostUpStreamLayerHit;   //!
   TBranch        *b_tecalUnmatchedNHits;   //!
   TBranch        *b_tecalUnmatchedTotalHitCharge;   //!
   TBranch        *b_tecalUnmatchedTrueID;   //!
   TBranch        *b_tecalUnmatchedTrueIDPrimary;   //!
   TBranch        *b_tecalUnmatchedTrueIDRecursive;   //!
   TBranch        *b_tecalUnmatchedTrueIDSingle;   //!
   TBranch        *b_tecalUnmatchedView;   //!
   TBranch        *b_tecalUnmatchedBunch;   //!
   TBranch        *b_NVertices;   //!
   TBranch        *b_vPrimaryIndex;   //!
   TBranch        *b_vPosition;   //!
   TBranch        *b_vNReconTracks;   //!
   TBranch        *b_vNTrueVerticesMatch;   //!
   TBranch        *b_vTracksUniqueID;   //!
   TBranch        *b_vTrueVertexIndex;   //!
   TBranch        *b_vTrueVerticesClean;   //!
   TBranch        *b_vTrueVerticesCompl;   //!

   MakeClFlatTree(TTree *tree=0);
   virtual ~MakeClFlatTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

   ClassDef(MakeClFlatTree,1)
};

#endif

#ifdef MakeClFlatTree_cxx
MakeClFlatTree::MakeClFlatTree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/physics/data/BANFF/asg2018oa/Splines_testing_psyche_v3r26/MCrun5w_Splines.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("/physics/data/BANFF/asg2018oa/Splines_testing_psyche_v3r26/MCrun5w_Splines.root");
      }
      f->GetObject("flattree",tree);

   }
   Init(tree);
}

MakeClFlatTree::~MakeClFlatTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MakeClFlatTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MakeClFlatTree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void MakeClFlatTree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   //tecalReconModule = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("sNBunches", &sNBunches, &b_sNBunches);
   fChain->SetBranchAddress("sNTotalTrueVertices", &sNTotalTrueVertices, &b_sNTotalTrueVertices);
   fChain->SetBranchAddress("sNTotalTrueTracks", &sNTotalTrueTracks, &b_sNTotalTrueTracks);
   fChain->SetBranchAddress("sIsMC", &sIsMC, &b_sIsMC);
   fChain->SetBranchAddress("sIsSand", &sIsSand, &b_sIsSand);
   fChain->SetBranchAddress("sRun", &sRun, &b_sRun);
   fChain->SetBranchAddress("sSubRun", &sSubRun, &b_sSubRun);
   fChain->SetBranchAddress("sEvt", &sEvt, &b_sEvt);
   fChain->SetBranchAddress("sBeamGoodSpill", &sBeamGoodSpill, &b_sBeamGoodSpill);
   fChain->SetBranchAddress("sDQGoodDaq", &sDQGoodDaq, &b_sDQGoodDaq);
   fChain->SetBranchAddress("sTriggerFGDCosmic", &sTriggerFGDCosmic, &b_sTriggerFGDCosmic);
   fChain->SetBranchAddress("sTriggerTripTCosmic", &sTriggerTripTCosmic, &b_sTriggerTripTCosmic);
   fChain->SetBranchAddress("sTriggerCTMTriggerPattern", sTriggerCTMTriggerPattern, &b_sTriggerCTMTriggerPattern);
   fChain->SetBranchAddress("sRooVtxEntry", &sRooVtxEntry, &b_sRooVtxEntry);
   fChain->SetBranchAddress("sInputFileIndex", &sInputFileIndex, &b_sInputFileIndex);
   fChain->SetBranchAddress("sPOTSincePreviousSpill", &sPOTSincePreviousSpill, &b_sPOTSincePreviousSpill);
   fChain->SetBranchAddress("sNTrueVertices", &sNTrueVertices, &b_sNTrueVertices);
   fChain->SetBranchAddress("sTrueVertexID", sTrueVertexID, &b_sTrueVertexID);
   fChain->SetBranchAddress("sTrueVertexNuPDG", sTrueVertexNuPDG, &b_sTrueVertexNuPDG);
   fChain->SetBranchAddress("sTrueVertexNuEnergy", sTrueVertexNuEnergy, &b_sTrueVertexNuEnergy);
   fChain->SetBranchAddress("sTrueVertexPosition", sTrueVertexPosition, &b_sTrueVertexPosition);
   fChain->SetBranchAddress("sTrueVertexNTrueTracks", sTrueVertexNTrueTracks, &b_sTrueVertexNTrueTracks);
   fChain->SetBranchAddress("sTrueVertexBunch", sTrueVertexBunch, &b_sTrueVertexBunch);
   fChain->SetBranchAddress("sTrueVertexDetector", sTrueVertexDetector, &b_sTrueVertexDetector);
   fChain->SetBranchAddress("sTrueVertexLeptonDir", sTrueVertexLeptonDir, &b_sTrueVertexLeptonDir);
   fChain->SetBranchAddress("sTrueVertexLeptonMom", sTrueVertexLeptonMom, &b_sTrueVertexLeptonMom);
   fChain->SetBranchAddress("sTrueVertexLeptonPDG", sTrueVertexLeptonPDG, &b_sTrueVertexLeptonPDG);
   fChain->SetBranchAddress("sTrueVertexNBaryons", sTrueVertexNBaryons, &b_sTrueVertexNBaryons);
   fChain->SetBranchAddress("sTrueVertexNReconTracks", sTrueVertexNReconTracks, &b_sTrueVertexNReconTracks);
   fChain->SetBranchAddress("sTrueVertexNReconVertices", sTrueVertexNReconVertices, &b_sTrueVertexNReconVertices);
   fChain->SetBranchAddress("sTrueVertexNuDir", sTrueVertexNuDir, &b_sTrueVertexNuDir);
   fChain->SetBranchAddress("sTrueVertexNuParentDecPoint", sTrueVertexNuParentDecPoint, &b_sTrueVertexNuParentDecPoint);
   fChain->SetBranchAddress("sTrueVertexNuParentPDG", sTrueVertexNuParentPDG, &b_sTrueVertexNuParentPDG);
   fChain->SetBranchAddress("sTrueVertexProtonDir", sTrueVertexProtonDir, &b_sTrueVertexProtonDir);
   fChain->SetBranchAddress("sTrueVertexProtonMom", sTrueVertexProtonMom, &b_sTrueVertexProtonMom);
   fChain->SetBranchAddress("sTrueVertexPionDir", sTrueVertexPionDir, &b_sTrueVertexPionDir);
   fChain->SetBranchAddress("sTrueVertexPionMom", sTrueVertexPionMom, &b_sTrueVertexPionMom);
   fChain->SetBranchAddress("sTrueVertexQ2", sTrueVertexQ2, &b_sTrueVertexQ2);
   fChain->SetBranchAddress("sTrueVertexReacCode", sTrueVertexReacCode, &b_sTrueVertexReacCode);
   fChain->SetBranchAddress("sTrueVertexRooVtxEntry", sTrueVertexRooVtxEntry, &b_sTrueVertexRooVtxEntry);
   fChain->SetBranchAddress("sTrueVertexRooVtxIndex", sTrueVertexRooVtxIndex, &b_sTrueVertexRooVtxIndex);
   fChain->SetBranchAddress("sTrueVertexTargetDir", sTrueVertexTargetDir, &b_sTrueVertexTargetDir);
   fChain->SetBranchAddress("sTrueVertexTargetMom", sTrueVertexTargetMom, &b_sTrueVertexTargetMom);
   fChain->SetBranchAddress("sTrueVertexTargetPDG", sTrueVertexTargetPDG, &b_sTrueVertexTargetPDG);
   fChain->SetBranchAddress("sTrueVertexIsPauliBlocked", sTrueVertexIsPauliBlocked, &b_sTrueVertexIsPauliBlocked);
   fChain->SetBranchAddress("sTrueVertexIsCohOnH", sTrueVertexIsCohOnH, &b_sTrueVertexIsCohOnH);
   fChain->SetBranchAddress("sTrueVertexNPrimaryParticles", sTrueVertexNPrimaryParticles, &b_sTrueVertexNPrimaryParticles);
   fChain->SetBranchAddress("sNTrueTracks", &sNTrueTracks, &b_sNTrueTracks);
   fChain->SetBranchAddress("sTrueTrackPrimaryID", sTrueTrackPrimaryID, &b_sTrueTrackPrimaryID);
   fChain->SetBranchAddress("sTrueTrackID", sTrueTrackID, &b_sTrueTrackID);
   fChain->SetBranchAddress("sTrueTrackPDG", sTrueTrackPDG, &b_sTrueTrackPDG);
   fChain->SetBranchAddress("sTrueTrackParentPDG", sTrueTrackParentPDG, &b_sTrueTrackParentPDG);
   fChain->SetBranchAddress("sTrueTrackGParentPDG", sTrueTrackGParentPDG, &b_sTrueTrackGParentPDG);
   fChain->SetBranchAddress("sTrueTrackParentID", sTrueTrackParentID, &b_sTrueTrackParentID);
   fChain->SetBranchAddress("sTrueTrackGParentID", sTrueTrackGParentID, &b_sTrueTrackGParentID);
   fChain->SetBranchAddress("sTrueTrackMomentum", sTrueTrackMomentum, &b_sTrueTrackMomentum);
   fChain->SetBranchAddress("sTrueTrackCharge", sTrueTrackCharge, &b_sTrueTrackCharge);
   fChain->SetBranchAddress("sTrueTrackPosition", sTrueTrackPosition, &b_sTrueTrackPosition);
   fChain->SetBranchAddress("sTrueTrackPositionEnd", sTrueTrackPositionEnd, &b_sTrueTrackPositionEnd);
   fChain->SetBranchAddress("sTrueTrackDirection", sTrueTrackDirection, &b_sTrueTrackDirection);
   fChain->SetBranchAddress("sTrueTrackTruthVertexID", sTrueTrackTruthVertexID, &b_sTrueTrackTruthVertexID);
   fChain->SetBranchAddress("sTrueTrackNDetCrossings", sTrueTrackNDetCrossings, &b_sTrueTrackNDetCrossings);
   fChain->SetBranchAddress("sTrueTrackIsTruePrimaryPi0DecayPhoton", sTrueTrackIsTruePrimaryPi0DecayPhoton, &b_sTrueTrackIsTruePrimaryPi0DecayPhoton);
   fChain->SetBranchAddress("sTrueTrackIsTruePrimaryPi0DecayPhotonChild", sTrueTrackIsTruePrimaryPi0DecayPhotonChild, &b_sTrueTrackIsTruePrimaryPi0DecayPhotonChild);
   fChain->SetBranchAddress("sTrueTrackInActive", sTrueTrackInActive, &b_sTrueTrackInActive);
   fChain->SetBranchAddress("sTrueTrackDetector", sTrueTrackDetector, &b_sTrueTrackDetector);
   fChain->SetBranchAddress("sTrueTrackEntrancePosition", sTrueTrackEntrancePosition, &b_sTrueTrackEntrancePosition);
   fChain->SetBranchAddress("sTrueTrackExitPosition", sTrueTrackExitPosition, &b_sTrueTrackExitPosition);
   fChain->SetBranchAddress("sTrueTrackEntranceMomentum", sTrueTrackEntranceMomentum, &b_sTrueTrackEntranceMomentum);
   fChain->SetBranchAddress("sTrueTrackExitMomentum", sTrueTrackExitMomentum, &b_sTrueTrackExitMomentum);
   fChain->SetBranchAddress("sTrueTrackPurity", sTrueTrackPurity, &b_sTrueTrackPurity);
   fChain->SetBranchAddress("sTrueTrackBunch", sTrueTrackBunch, &b_sTrueTrackBunch);
   fChain->SetBranchAddress("sTrueTrackNReconTracks", sTrueTrackNReconTracks, &b_sTrueTrackNReconTracks);
   fChain->SetBranchAddress("sTrueTrackVertexIndex", sTrueTrackVertexIndex, &b_sTrueTrackVertexIndex);
   fChain->SetBranchAddress("sNFgdTimeBins", &sNFgdTimeBins, &b_sNFgdTimeBins);
   fChain->SetBranchAddress("sFgdTimeBinMinTime", sFgdTimeBinMinTime, &b_sFgdTimeBinMinTime);
   fChain->SetBranchAddress("sFgdTimeBinNHits1", sFgdTimeBinNHits1, &b_sFgdTimeBinNHits1);
   fChain->SetBranchAddress("sFgdTimeBinNHits2", sFgdTimeBinNHits2, &b_sFgdTimeBinNHits2);
   fChain->SetBranchAddress("sFgdTimeBinRawChargeSum1", sFgdTimeBinRawChargeSum1, &b_sFgdTimeBinRawChargeSum1);
   fChain->SetBranchAddress("sFgdTimeBinRawChargeSum2", sFgdTimeBinRawChargeSum2, &b_sFgdTimeBinRawChargeSum2);
   fChain->SetBranchAddress("Bunch", &Bunch, &b_Bunch);
   fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
   fChain->SetBranchAddress("NTracks", &NTracks, &b_NTracks);
   fChain->SetBranchAddress("trTpcDetector", trTpcDetector, &b_trTpcDetector);
   fChain->SetBranchAddress("trTpcNNodes", trTpcNNodes, &b_trTpcNNodes);
   fChain->SetBranchAddress("trTpcNHits", trTpcNHits, &b_trTpcNHits);
   fChain->SetBranchAddress("trTpcUniqueID", trTpcUniqueID, &b_trTpcUniqueID);
   fChain->SetBranchAddress("trTpcLength", trTpcLength, &b_trTpcLength);
   fChain->SetBranchAddress("trTpcDirectionStart", trTpcDirectionStart, &b_trTpcDirectionStart);
   fChain->SetBranchAddress("trTpcPositionStart", trTpcPositionStart, &b_trTpcPositionStart);
   fChain->SetBranchAddress("trTpcPositionEnd", trTpcPositionEnd, &b_trTpcPositionEnd);
   fChain->SetBranchAddress("trTpcDirectionEnd", trTpcDirectionEnd, &b_trTpcDirectionEnd);
   fChain->SetBranchAddress("trTpcCharge", trTpcCharge, &b_trTpcCharge);
   fChain->SetBranchAddress("trTpcMomentum", trTpcMomentum, &b_trTpcMomentum);
   fChain->SetBranchAddress("trTpcMomentumError", trTpcMomentumError, &b_trTpcMomentumError);
   fChain->SetBranchAddress("trTpcRefitMomentum", trTpcRefitMomentum, &b_trTpcRefitMomentum);
   fChain->SetBranchAddress("trTpcEFieldRefitMomentum", trTpcEFieldRefitMomentum, &b_trTpcEFieldRefitMomentum);
   fChain->SetBranchAddress("trTpcdEdxMeas", trTpcdEdxMeas, &b_trTpcdEdxMeas);
   fChain->SetBranchAddress("trTpcdEdxExpMu", trTpcdEdxExpMu, &b_trTpcdEdxExpMu);
   fChain->SetBranchAddress("trTpcdEdxExpEle", trTpcdEdxExpEle, &b_trTpcdEdxExpEle);
   fChain->SetBranchAddress("trTpcdEdxExpP", trTpcdEdxExpP, &b_trTpcdEdxExpP);
   fChain->SetBranchAddress("trTpcdEdxExpPi", trTpcdEdxExpPi, &b_trTpcdEdxExpPi);
   fChain->SetBranchAddress("trTpcRawdEdxMeas", trTpcRawdEdxMeas, &b_trTpcRawdEdxMeas);
   fChain->SetBranchAddress("trTpcRawdEdxExpMu", trTpcRawdEdxExpMu, &b_trTpcRawdEdxExpMu);
   fChain->SetBranchAddress("trTpcRawdEdxExpEle", trTpcRawdEdxExpEle, &b_trTpcRawdEdxExpEle);
   fChain->SetBranchAddress("trTpcRawdEdxExpP", trTpcRawdEdxExpP, &b_trTpcRawdEdxExpP);
   fChain->SetBranchAddress("trTpcRawdEdxExpPi", trTpcRawdEdxExpPi, &b_trTpcRawdEdxExpPi);
   fChain->SetBranchAddress("trTpcdEdxSigmaMu", trTpcdEdxSigmaMu, &b_trTpcdEdxSigmaMu);
   fChain->SetBranchAddress("trTpcdEdxSigmaEle", trTpcdEdxSigmaEle, &b_trTpcdEdxSigmaEle);
   fChain->SetBranchAddress("trTpcdEdxSigmaP", trTpcdEdxSigmaP, &b_trTpcdEdxSigmaP);
   fChain->SetBranchAddress("trTpcdEdxSigmaPi", trTpcdEdxSigmaPi, &b_trTpcdEdxSigmaPi);
   fChain->SetBranchAddress("trTpcTrueTrackID", trTpcTrueTrackID, &b_trTpcTrueTrackID);
   fChain->SetBranchAddress("trTpcBackMomentum", trTpcBackMomentum, &b_trTpcBackMomentum);
   fChain->SetBranchAddress("trTpcPullmu", trTpcPullmu, &b_trTpcPullmu);
   fChain->SetBranchAddress("trTpcPullele", trTpcPullele, &b_trTpcPullele);
   fChain->SetBranchAddress("trTpcPullp", trTpcPullp, &b_trTpcPullp);
   fChain->SetBranchAddress("trTpcPullpi", trTpcPullpi, &b_trTpcPullpi);
   fChain->SetBranchAddress("trTpcPullk", trTpcPullk, &b_trTpcPullk);
   fChain->SetBranchAddress("trTpcdEdxExpK", trTpcdEdxExpK, &b_trTpcdEdxExpK);
   fChain->SetBranchAddress("trTpcRawdEdxExpK", trTpcRawdEdxExpK, &b_trTpcRawdEdxExpK);
   fChain->SetBranchAddress("trTpcdEdxSigmaK", trTpcdEdxSigmaK, &b_trTpcdEdxSigmaK);
   fChain->SetBranchAddress("trTpcPurity", trTpcPurity, &b_trTpcPurity);
   fChain->SetBranchAddress("trFgdDetector", trFgdDetector, &b_trFgdDetector);
   fChain->SetBranchAddress("trFgdNNodes", trFgdNNodes, &b_trFgdNNodes);
   fChain->SetBranchAddress("trFgdNHits", trFgdNHits, &b_trFgdNHits);
   fChain->SetBranchAddress("trFgdUniqueID", trFgdUniqueID, &b_trFgdUniqueID);
   fChain->SetBranchAddress("trFgdLength", trFgdLength, &b_trFgdLength);
   fChain->SetBranchAddress("trFgdDirectionStart", trFgdDirectionStart, &b_trFgdDirectionStart);
   fChain->SetBranchAddress("trFgdPositionStart", trFgdPositionStart, &b_trFgdPositionStart);
   fChain->SetBranchAddress("trFgdPositionEnd", trFgdPositionEnd, &b_trFgdPositionEnd);
   fChain->SetBranchAddress("trFgdDirectionEnd", trFgdDirectionEnd, &b_trFgdDirectionEnd);
   fChain->SetBranchAddress("trFgdX", trFgdX, &b_trFgdX);
   fChain->SetBranchAddress("trFgdPullmu", trFgdPullmu, &b_trFgdPullmu);
   fChain->SetBranchAddress("trFgdPullp", trFgdPullp, &b_trFgdPullp);
   fChain->SetBranchAddress("trFgdPullpi", trFgdPullpi, &b_trFgdPullpi);
   fChain->SetBranchAddress("trFgdPullno", trFgdPullno, &b_trFgdPullno);
   fChain->SetBranchAddress("trFgdContainment", trFgdContainment, &b_trFgdContainment);
   fChain->SetBranchAddress("trFgdE", trFgdE, &b_trFgdE);
   fChain->SetBranchAddress("trFgdAvgTime", trFgdAvgTime, &b_trFgdAvgTime);
   fChain->SetBranchAddress("trFgdHasFgdVA", trFgdHasFgdVA, &b_trFgdHasFgdVA);
   fChain->SetBranchAddress("trFgdHasFgdVA_fmode", trFgdHasFgdVA_fmode, &b_trFgdHasFgdVA_fmode);
   fChain->SetBranchAddress("trFgdTypeVA", trFgdTypeVA, &b_trFgdTypeVA);
   fChain->SetBranchAddress("trFgdVertex1x1", trFgdVertex1x1, &b_trFgdVertex1x1);
   fChain->SetBranchAddress("trFgdVertex3x3", trFgdVertex3x3, &b_trFgdVertex3x3);
   fChain->SetBranchAddress("trFgdVertex5x5", trFgdVertex5x5, &b_trFgdVertex5x5);
   fChain->SetBranchAddress("trFgdVertex7x7", trFgdVertex7x7, &b_trFgdVertex7x7);
   fChain->SetBranchAddress("trFgdVertexLayer", trFgdVertexLayer, &b_trFgdVertexLayer);
   fChain->SetBranchAddress("trFgdHasFgdEA", trFgdHasFgdEA, &b_trFgdHasFgdEA);
   fChain->SetBranchAddress("trFgdHasFgdEA_fmode", trFgdHasFgdEA_fmode, &b_trFgdHasFgdEA_fmode);
   fChain->SetBranchAddress("trFgdTypeEA", trFgdTypeEA, &b_trFgdTypeEA);
   fChain->SetBranchAddress("trFgdEnd0x0", trFgdEnd0x0, &b_trFgdEnd0x0);
   fChain->SetBranchAddress("trFgdEnd1x1", trFgdEnd1x1, &b_trFgdEnd1x1);
   fChain->SetBranchAddress("trFgdEnd3x3", trFgdEnd3x3, &b_trFgdEnd3x3);
   fChain->SetBranchAddress("trFgdEnd5x5", trFgdEnd5x5, &b_trFgdEnd5x5);
   fChain->SetBranchAddress("trFgdEnd7x7", trFgdEnd7x7, &b_trFgdEnd7x7);
   fChain->SetBranchAddress("trECALDetector", trECALDetector, &b_trECALDetector);
   fChain->SetBranchAddress("trECALNNodes", trECALNNodes, &b_trECALNNodes);
   fChain->SetBranchAddress("trECALNHits", trECALNHits, &b_trECALNHits);
   fChain->SetBranchAddress("trECALUniqueID", trECALUniqueID, &b_trECALUniqueID);
   fChain->SetBranchAddress("trECALLength", trECALLength, &b_trECALLength);
   fChain->SetBranchAddress("trECALDirectionStart", trECALDirectionStart, &b_trECALDirectionStart);
   fChain->SetBranchAddress("trECALPositionStart", trECALPositionStart, &b_trECALPositionStart);
   fChain->SetBranchAddress("trECALPositionEnd", trECALPositionEnd, &b_trECALPositionEnd);
   fChain->SetBranchAddress("trECALDirectionEnd", trECALDirectionEnd, &b_trECALDirectionEnd);
   fChain->SetBranchAddress("trECALTrShVal", trECALTrShVal, &b_trECALTrShVal);
   fChain->SetBranchAddress("trECALPIDMipEm", trECALPIDMipEm, &b_trECALPIDMipEm);
   fChain->SetBranchAddress("trECALPIDEmHip", trECALPIDEmHip, &b_trECALPIDEmHip);
   fChain->SetBranchAddress("trECALEMEnergy", trECALEMEnergy, &b_trECALEMEnergy);
   fChain->SetBranchAddress("trECALEDeposit", trECALEDeposit, &b_trECALEDeposit);
   fChain->SetBranchAddress("trECALIsShowerLike", trECALIsShowerLike, &b_trECALIsShowerLike);
   fChain->SetBranchAddress("trECALAvgTime", trECALAvgTime, &b_trECALAvgTime);
   fChain->SetBranchAddress("trECALMostUpStreamLayerHit", trECALMostUpStreamLayerHit, &b_trECALMostUpStreamLayerHit);
   fChain->SetBranchAddress("trECALShowerPosition", trECALShowerPosition, &b_trECALShowerPosition);
   fChain->SetBranchAddress("trECALPIDMipPion", trECALPIDMipPion, &b_trECALPIDMipPion);
   fChain->SetBranchAddress("trECALContainment", trECALContainment, &b_trECALContainment);
   fChain->SetBranchAddress("trSMRDDetector", trSMRDDetector, &b_trSMRDDetector);
   fChain->SetBranchAddress("trSMRDNNodes", trSMRDNNodes, &b_trSMRDNNodes);
   fChain->SetBranchAddress("trSMRDNHits", trSMRDNHits, &b_trSMRDNHits);
   fChain->SetBranchAddress("trSMRDUniqueID", trSMRDUniqueID, &b_trSMRDUniqueID);
   fChain->SetBranchAddress("trSMRDLength", trSMRDLength, &b_trSMRDLength);
   fChain->SetBranchAddress("trSMRDDirectionStart", trSMRDDirectionStart, &b_trSMRDDirectionStart);
   fChain->SetBranchAddress("trSMRDPositionStart", trSMRDPositionStart, &b_trSMRDPositionStart);
   fChain->SetBranchAddress("trSMRDPositionEnd", trSMRDPositionEnd, &b_trSMRDPositionEnd);
   fChain->SetBranchAddress("trSMRDDirectionEnd", trSMRDDirectionEnd, &b_trSMRDDirectionEnd);
   fChain->SetBranchAddress("trSMRDEDeposit", trSMRDEDeposit, &b_trSMRDEDeposit);
   fChain->SetBranchAddress("trSMRDAvgTime", trSMRDAvgTime, &b_trSMRDAvgTime);
   fChain->SetBranchAddress("trP0DDetector", trP0DDetector, &b_trP0DDetector);
   fChain->SetBranchAddress("trP0DNNodes", trP0DNNodes, &b_trP0DNNodes);
   fChain->SetBranchAddress("trP0DNHits", trP0DNHits, &b_trP0DNHits);
   fChain->SetBranchAddress("trP0DUniqueID", trP0DUniqueID, &b_trP0DUniqueID);
   fChain->SetBranchAddress("trP0DLength", trP0DLength, &b_trP0DLength);
   fChain->SetBranchAddress("trP0DDirectionStart", trP0DDirectionStart, &b_trP0DDirectionStart);
   fChain->SetBranchAddress("trP0DPositionStart", trP0DPositionStart, &b_trP0DPositionStart);
   fChain->SetBranchAddress("trP0DPositionEnd", trP0DPositionEnd, &b_trP0DPositionEnd);
   fChain->SetBranchAddress("trP0DDirectionEnd", trP0DDirectionEnd, &b_trP0DDirectionEnd);
   fChain->SetBranchAddress("trP0DAvgTime", trP0DAvgTime, &b_trP0DAvgTime);
   fChain->SetBranchAddress("trP0DELoss", trP0DELoss, &b_trP0DELoss);
   fChain->SetBranchAddress("trTRACKERDetector", trTRACKERDetector, &b_trTRACKERDetector);
   fChain->SetBranchAddress("trTRACKERNNodes", trTRACKERNNodes, &b_trTRACKERNNodes);
   fChain->SetBranchAddress("trTRACKERNHits", trTRACKERNHits, &b_trTRACKERNHits);
   fChain->SetBranchAddress("trTRACKERUniqueID", trTRACKERUniqueID, &b_trTRACKERUniqueID);
   fChain->SetBranchAddress("trTRACKERLength", trTRACKERLength, &b_trTRACKERLength);
   fChain->SetBranchAddress("trTRACKERDirectionStart", trTRACKERDirectionStart, &b_trTRACKERDirectionStart);
   fChain->SetBranchAddress("trTRACKERPositionStart", trTRACKERPositionStart, &b_trTRACKERPositionStart);
   fChain->SetBranchAddress("trTRACKERPositionEnd", trTRACKERPositionEnd, &b_trTRACKERPositionEnd);
   fChain->SetBranchAddress("trTRACKERDirectionEnd", trTRACKERDirectionEnd, &b_trTRACKERDirectionEnd);
   fChain->SetBranchAddress("trTRACKERCharge", trTRACKERCharge, &b_trTRACKERCharge);
   fChain->SetBranchAddress("trTRACKERMomentum", trTRACKERMomentum, &b_trTRACKERMomentum);
   fChain->SetBranchAddress("trTRACKERMomentumEnd", trTRACKERMomentumEnd, &b_trTRACKERMomentumEnd);
   fChain->SetBranchAddress("trUniqueID", trUniqueID, &b_trUniqueID);
   fChain->SetBranchAddress("trDetUsed", trDetUsed, &b_trDetUsed);
   fChain->SetBranchAddress("trNTPCs", trNTPCs, &b_trNTPCs);
   fChain->SetBranchAddress("trNFGDs", trNFGDs, &b_trNFGDs);
   fChain->SetBranchAddress("trNECALs", trNECALs, &b_trNECALs);
   fChain->SetBranchAddress("trNSMRDs", trNSMRDs, &b_trNSMRDs);
   fChain->SetBranchAddress("trNP0Ds", trNP0Ds, &b_trNP0Ds);
   fChain->SetBranchAddress("trNHits", trNHits, &b_trNHits);
   fChain->SetBranchAddress("trCharge", trCharge, &b_trCharge);
   fChain->SetBranchAddress("trMomentum", trMomentum, &b_trMomentum);
   fChain->SetBranchAddress("trToFFGD1_FGD2", trToFFGD1_FGD2, &b_trToFFGD1_FGD2);
   fChain->SetBranchAddress("trToFP0D_FGD1", trToFP0D_FGD1, &b_trToFP0D_FGD1);
   fChain->SetBranchAddress("trToFDSECal_FGD1", trToFDSECal_FGD1, &b_trToFDSECal_FGD1);
   fChain->SetBranchAddress("trToFECal_FGD1", trToFECal_FGD1, &b_trToFECal_FGD1);
   fChain->SetBranchAddress("trToFDSECal_FGD2", trToFDSECal_FGD2, &b_trToFDSECal_FGD2);
   fChain->SetBranchAddress("trToFECal_FGD2", trToFECal_FGD2, &b_trToFECal_FGD2);
   fChain->SetBranchAddress("trToFFlag_FGD1_FGD2", trToFFlag_FGD1_FGD2, &b_trToFFlag_FGD1_FGD2);
   fChain->SetBranchAddress("trToFFlag_P0D_FGD1", trToFFlag_P0D_FGD1, &b_trToFFlag_P0D_FGD1);
   fChain->SetBranchAddress("trToFFlag_DSECal_FGD1", trToFFlag_DSECal_FGD1, &b_trToFFlag_DSECal_FGD1);
   fChain->SetBranchAddress("trToFFlag_ECal_FGD1", trToFFlag_ECal_FGD1, &b_trToFFlag_ECal_FGD1);
   fChain->SetBranchAddress("trToFFlag_DSECal_FGD2", trToFFlag_DSECal_FGD2, &b_trToFFlag_DSECal_FGD2);
   fChain->SetBranchAddress("trToFFlag_ECal_FGD2", trToFFlag_ECal_FGD2, &b_trToFFlag_ECal_FGD2);
   fChain->SetBranchAddress("trRangeMomentumMuon", trRangeMomentumMuon, &b_trRangeMomentumMuon);
   fChain->SetBranchAddress("trDirectionStart", trDirectionStart, &b_trDirectionStart);
   fChain->SetBranchAddress("trDirectionEnd", trDirectionEnd, &b_trDirectionEnd);
   fChain->SetBranchAddress("trPositionStart", trPositionStart, &b_trPositionStart);
   fChain->SetBranchAddress("trPositionEnd", trPositionEnd, &b_trPositionEnd);
   fChain->SetBranchAddress("trMomentumFlip", trMomentumFlip, &b_trMomentumFlip);
   fChain->SetBranchAddress("trDirectionStartFlip", trDirectionStartFlip, &b_trDirectionStartFlip);
   fChain->SetBranchAddress("trTrueTrackID", trTrueTrackID, &b_trTrueTrackID);
   fChain->SetBranchAddress("trStatus", trStatus, &b_trStatus);
   fChain->SetBranchAddress("trNNodes", trNNodes, &b_trNNodes);
   fChain->SetBranchAddress("trMomentumError", trMomentumError, &b_trMomentumError);
   fChain->SetBranchAddress("trNTRACKERs", trNTRACKERs, &b_trNTRACKERs);
   fChain->SetBranchAddress("trNDOF", trNDOF, &b_trNDOF);
   fChain->SetBranchAddress("trChi2", trChi2, &b_trChi2);
   fChain->SetBranchAddress("trMomentumMuon", trMomentumMuon, &b_trMomentumMuon);
   fChain->SetBranchAddress("trMomentumErrorMuon", trMomentumErrorMuon, &b_trMomentumErrorMuon);
   fChain->SetBranchAddress("trMomentumProton", trMomentumProton, &b_trMomentumProton);
   fChain->SetBranchAddress("trMomentumErrorProton", trMomentumErrorProton, &b_trMomentumErrorProton);
   fChain->SetBranchAddress("trMomentumEle", trMomentumEle, &b_trMomentumEle);
   fChain->SetBranchAddress("trMomentumErrorEle", trMomentumErrorEle, &b_trMomentumErrorEle);
   fChain->SetBranchAddress("trPositionStartFlip", trPositionStartFlip, &b_trPositionStartFlip);
   fChain->SetBranchAddress("trDirectionEndFlip", trDirectionEndFlip, &b_trDirectionEndFlip);
   fChain->SetBranchAddress("trRangeMomentumEle", trRangeMomentumEle, &b_trRangeMomentumEle);
   fChain->SetBranchAddress("trRangeMomentumProton", trRangeMomentumProton, &b_trRangeMomentumProton);
   fChain->SetBranchAddress("trRangeMomentumMuonFlip", trRangeMomentumMuonFlip, &b_trRangeMomentumMuonFlip);
   fChain->SetBranchAddress("trRangeMomentumProtonFlip", trRangeMomentumProtonFlip, &b_trRangeMomentumProtonFlip);
   fChain->SetBranchAddress("trReconPDG", trReconPDG, &b_trReconPDG);
   fChain->SetBranchAddress("trBunch", trBunch, &b_trBunch);
   fChain->SetBranchAddress("trNReconVertices", trNReconVertices, &b_trNReconVertices);
   fChain->SetBranchAddress("trMomentumAtVertex", trMomentumAtVertex, &b_trMomentumAtVertex);
   fChain->SetBranchAddress("trDirectionAtVertex", trDirectionAtVertex, &b_trDirectionAtVertex);
   fChain->SetBranchAddress("trNTimeNodes", trNTimeNodes, &b_trNTimeNodes);
   fChain->SetBranchAddress("trTimeNodeDetector", trTimeNodeDetector, &b_trTimeNodeDetector);
   fChain->SetBranchAddress("trTimeNodeTimeStart", trTimeNodeTimeStart, &b_trTimeNodeTimeStart);
   fChain->SetBranchAddress("trTimeNodeTimeEnd", trTimeNodeTimeEnd, &b_trTimeNodeTimeEnd);
   fChain->SetBranchAddress("trDetectors", trDetectors, &b_trDetectors);
   fChain->SetBranchAddress("trLength", trLength, &b_trLength);
   fChain->SetBranchAddress("tecalReconCounter", &tecalReconCounter, &b_tecalReconCounter);
   fChain->SetBranchAddress("tecalReconAverageHitTime", tecalReconAverageHitTime, &b_tecalReconAverageHitTime);
   fChain->SetBranchAddress("tecalReconAverageZPosition", tecalReconAverageZPosition, &b_tecalReconAverageZPosition);
   fChain->SetBranchAddress("tecalReconContainment", tecalReconContainment, &b_tecalReconContainment);
   fChain->SetBranchAddress("tecalReconEFitResult", tecalReconEFitResult, &b_tecalReconEFitResult);
   fChain->SetBranchAddress("tecalReconEFitUncertainty", tecalReconEFitUncertainty, &b_tecalReconEFitUncertainty);
   fChain->SetBranchAddress("tecalReconFirstLayer", tecalReconFirstLayer, &b_tecalReconFirstLayer);
   fChain->SetBranchAddress("tecalReconIsShowerLike", tecalReconIsShowerLike, &b_tecalReconIsShowerLike);
   fChain->SetBranchAddress("tecalReconIsTrackLike", tecalReconIsTrackLike, &b_tecalReconIsTrackLike);
   fChain->SetBranchAddress("tecalReconLastLayer", tecalReconLastLayer, &b_tecalReconLastLayer);
   fChain->SetBranchAddress("tecalReconLikeMIPEM", tecalReconLikeMIPEM, &b_tecalReconLikeMIPEM);
   fChain->SetBranchAddress("tecalReconLikeMIPPion", tecalReconLikeMIPPion, &b_tecalReconLikeMIPPion);
   fChain->SetBranchAddress("tecalReconLikeEMHIP", tecalReconLikeEMHIP, &b_tecalReconLikeEMHIP);
   fChain->SetBranchAddress("tecalReconLikeMIPEMLow", tecalReconLikeMIPEMLow, &b_tecalReconLikeMIPEMLow);
   fChain->SetBranchAddress("tecalReconMatchingLike", tecalReconMatchingLike, &b_tecalReconMatchingLike);
   fChain->SetBranchAddress("tecalReconMichelTagNDelayedCluster", tecalReconMichelTagNDelayedCluster, &b_tecalReconMichelTagNDelayedCluster);
   //fChain->SetBranchAddress("tecalReconModule", &tecalReconModule, &b_tecalReconModule);
   fChain->SetBranchAddress("tecalReconMostDownStreamLayerHit", tecalReconMostDownStreamLayerHit, &b_tecalReconMostDownStreamLayerHit);
   fChain->SetBranchAddress("tecalReconMostUpStreamLayerHit", tecalReconMostUpStreamLayerHit, &b_tecalReconMostUpStreamLayerHit);
   fChain->SetBranchAddress("tecalReconNHits", tecalReconNHits, &b_tecalReconNHits);
   fChain->SetBranchAddress("tecalReconNLayersHit", tecalReconNLayersHit, &b_tecalReconNLayersHit);
   fChain->SetBranchAddress("tecalReconObjectLength", tecalReconObjectLength, &b_tecalReconObjectLength);
   fChain->SetBranchAddress("tecalReconPIDAMR", tecalReconPIDAMR, &b_tecalReconPIDAMR);
   fChain->SetBranchAddress("tecalReconPIDAngle", tecalReconPIDAngle, &b_tecalReconPIDAngle);
   fChain->SetBranchAddress("tecalReconPIDAsymmetry", tecalReconPIDAsymmetry, &b_tecalReconPIDAsymmetry);
   fChain->SetBranchAddress("tecalReconPIDCircularity", tecalReconPIDCircularity, &b_tecalReconPIDCircularity);
   fChain->SetBranchAddress("tecalReconPIDFBR", tecalReconPIDFBR, &b_tecalReconPIDFBR);
   fChain->SetBranchAddress("tecalReconPIDMaxRatio", tecalReconPIDMaxRatio, &b_tecalReconPIDMaxRatio);
   fChain->SetBranchAddress("tecalReconPIDMeanPos", tecalReconPIDMeanPos, &b_tecalReconPIDMeanPos);
   fChain->SetBranchAddress("tecalReconPIDShowerAngle", tecalReconPIDShowerAngle, &b_tecalReconPIDShowerAngle);
   fChain->SetBranchAddress("tecalReconPIDShowerWidth", tecalReconPIDShowerWidth, &b_tecalReconPIDShowerWidth);
   fChain->SetBranchAddress("tecalReconPIDTransverseChargeRatio", tecalReconPIDTransverseChargeRatio, &b_tecalReconPIDTransverseChargeRatio);
   fChain->SetBranchAddress("tecalReconPIDTruncatedMaxRatio", tecalReconPIDTruncatedMaxRatio, &b_tecalReconPIDTruncatedMaxRatio);
   fChain->SetBranchAddress("tecalReconPointing", tecalReconPointing, &b_tecalReconPointing);
   fChain->SetBranchAddress("tecalReconThrust", tecalReconThrust, &b_tecalReconThrust);
   fChain->SetBranchAddress("tecalReconThrustAxis", tecalReconThrustAxis, &b_tecalReconThrustAxis);
   fChain->SetBranchAddress("tecalReconThrustOrigin", tecalReconThrustOrigin, &b_tecalReconThrustOrigin);
   fChain->SetBranchAddress("tecalReconTimeBunch", tecalReconTimeBunch, &b_tecalReconTimeBunch);
   fChain->SetBranchAddress("tecalReconTotalHitCharge", tecalReconTotalHitCharge, &b_tecalReconTotalHitCharge);
   fChain->SetBranchAddress("tecalReconTrueID", tecalReconTrueID, &b_tecalReconTrueID);
   fChain->SetBranchAddress("tecalReconTrueIDPrimary", tecalReconTrueIDPrimary, &b_tecalReconTrueIDPrimary);
   fChain->SetBranchAddress("tecalReconTrueIDRecursive", tecalReconTrueIDRecursive, &b_tecalReconTrueIDRecursive);
   fChain->SetBranchAddress("tecalReconTrueIDSingle", tecalReconTrueIDSingle, &b_tecalReconTrueIDSingle);
   fChain->SetBranchAddress("tecalReconUniqueID", tecalReconUniqueID, &b_tecalReconUniqueID);
   fChain->SetBranchAddress("tecalReconBunch", tecalReconBunch, &b_tecalReconBunch);
   fChain->SetBranchAddress("tecalUnmatchedCounter", &tecalUnmatchedCounter, &b_tecalUnmatchedCounter);
   fChain->SetBranchAddress("tecalUnmatchedAverageHitTime", tecalUnmatchedAverageHitTime, &b_tecalUnmatchedAverageHitTime);
   fChain->SetBranchAddress("tecalUnmatchedBackPos", tecalUnmatchedBackPos, &b_tecalUnmatchedBackPos);
   fChain->SetBranchAddress("tecalUnmatchedFrontPos", tecalUnmatchedFrontPos, &b_tecalUnmatchedFrontPos);
   fChain->SetBranchAddress("tecalUnmatchedMostDownStreamLayerHit", tecalUnmatchedMostDownStreamLayerHit, &b_tecalUnmatchedMostDownStreamLayerHit);
   fChain->SetBranchAddress("tecalUnmatchedMostUpStreamLayerHit", tecalUnmatchedMostUpStreamLayerHit, &b_tecalUnmatchedMostUpStreamLayerHit);
   fChain->SetBranchAddress("tecalUnmatchedNHits", tecalUnmatchedNHits, &b_tecalUnmatchedNHits);
   fChain->SetBranchAddress("tecalUnmatchedTotalHitCharge", tecalUnmatchedTotalHitCharge, &b_tecalUnmatchedTotalHitCharge);
   fChain->SetBranchAddress("tecalUnmatchedTrueID", tecalUnmatchedTrueID, &b_tecalUnmatchedTrueID);
   fChain->SetBranchAddress("tecalUnmatchedTrueIDPrimary", tecalUnmatchedTrueIDPrimary, &b_tecalUnmatchedTrueIDPrimary);
   fChain->SetBranchAddress("tecalUnmatchedTrueIDRecursive", tecalUnmatchedTrueIDRecursive, &b_tecalUnmatchedTrueIDRecursive);
   fChain->SetBranchAddress("tecalUnmatchedTrueIDSingle", tecalUnmatchedTrueIDSingle, &b_tecalUnmatchedTrueIDSingle);
   fChain->SetBranchAddress("tecalUnmatchedView", tecalUnmatchedView, &b_tecalUnmatchedView);
   fChain->SetBranchAddress("tecalUnmatchedBunch", tecalUnmatchedBunch, &b_tecalUnmatchedBunch);
   fChain->SetBranchAddress("NVertices", &NVertices, &b_NVertices);
   fChain->SetBranchAddress("vPrimaryIndex", vPrimaryIndex, &b_vPrimaryIndex);
   fChain->SetBranchAddress("vPosition", vPosition, &b_vPosition);
   fChain->SetBranchAddress("vNReconTracks", vNReconTracks, &b_vNReconTracks);
   fChain->SetBranchAddress("vNTrueVerticesMatch", vNTrueVerticesMatch, &b_vNTrueVerticesMatch);
   fChain->SetBranchAddress("vTracksUniqueID", vTracksUniqueID, &b_vTracksUniqueID);
   fChain->SetBranchAddress("vTrueVertexIndex", vTrueVertexIndex, &b_vTrueVertexIndex);
   fChain->SetBranchAddress("vTrueVerticesClean", vTrueVerticesClean, &b_vTrueVerticesClean);
   fChain->SetBranchAddress("vTrueVerticesCompl", vTrueVerticesCompl, &b_vTrueVerticesCompl);
   Notify();
}

Bool_t MakeClFlatTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MakeClFlatTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MakeClFlatTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MakeClFlatTree_cxx
