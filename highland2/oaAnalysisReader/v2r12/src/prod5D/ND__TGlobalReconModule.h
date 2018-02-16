//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Nov  4 11:44:16 2016 by ROOT version 5.34/34)
//      from the StreamerInfo in file /hep/T2K/DataDir/valid/oa_nt_beam_90410000-0000_h252u2jujozw_anal_000_prod005magnet201011waterc-bsdv01.root
//////////////////////////////////////////////////////////


#ifndef ND__TGlobalReconModule_h
#define ND__TGlobalReconModule_h
namespace ND {
class TGlobalReconModule;
} // end of namespace.

#include "ND__TAnalysisReconModuleBase.h"
#include "Riostream.h"
#include <vector>
#include "TClonesArray.h"
#include "ND__TGlobalReconModule.h"
#include "TObject.h"
#include <string>
#include "TLorentzVector.h"
#include "ND__TTrueVertex.h"
#include "TVector3.h"
#include "ND__TTrueParticle.h"
#include "ND__TSubBaseObject.h"
#include "ND__TSubBaseShowerObject.h"

namespace ND {
class TGlobalReconModule : public ND::TAnalysisReconModuleBase {

public:
// Nested classes forward declaration.
class TGlobalVertex;
class TVertexConstituent;
class TFgdCluster;
class TFgdTimeBin;
class TOutermostHits;
class TGlobalHit;
class TGlobalPID;
class TTPCObject;
class TFGDObject;
class TECALObject;
class TP0DObject;
class TSMRDObject;
class TTrackerObject;
class TGlobalPIDAlternate;
class TTPCOtherObject;
class TSMRDHit;

public:
// Nested classes declaration.
class TSMRDHit : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   int         Wall;        //
   int         Yoke;        //
   int         Layer;       //
   int         Tower;       //
   int         Counter;     //
   double      Charge;      //
   double      Time;        //
   TVector3    Position;    //
   TVector3    PositionError;    //
   int         Type;             //

   TSMRDHit();
   TSMRDHit(const TSMRDHit & );
   virtual ~TSMRDHit();

   ClassDef(TSMRDHit,2); // Generated by MakeProject.
};
class TTPCOtherObject : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      Charge;      //
   int         Detector;    //
   int         NHits;       //
   double      Chi2;        //
   double      EDeposit;    //
   TLorentzVector FrontPosition;    //
   TLorentzVector BackPosition;     //
   TVector3       FrontDirection;    //
   TVector3       BackDirection;     //
   double         Momentum;          //
   ND::TTrueParticle TrueParticle;      //

   TTPCOtherObject();
   TTPCOtherObject(const TTPCOtherObject & );
   virtual ~TTPCOtherObject();

   ClassDef(TTPCOtherObject,2); // Generated by MakeProject.
};
class TGlobalPIDAlternate : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   unsigned long Detectors;    //
   bool          DetectorUsed[19];    //
   unsigned long Status;              //
   int           NDOF;                //
   double        Chi2;                //
   double        Charge;              //
   double        Length;              //
   int           ParticleId;          //
   double        PIDWeight;           //
   TLorentzVector FrontPosition;       //
   TLorentzVector BackPosition;        //
   TLorentzVector FrontPositionVar;    //
   TLorentzVector BackPositionVar;     //
   TVector3       FrontDirection;      //
   TVector3       BackDirection;       //
   TVector3       FrontDirectionVar;    //
   TVector3       BackDirectionVar;     //
   double         FrontMomentum;        //
   double         BackMomentum;         //
   double         FrontMomentumError;    //
   double         BackMomentumError;     //
   TLorentzVector PositionAtTrueVertex;    //
   TLorentzVector PositionVarAtTrueVertex;    //
   TVector3       DirectionAtTrueVertex;      //
   TVector3       DirectionVarAtTrueVertex;    //
   double         MomentumAtTrueVertex;        //
   double         MomentumErrorAtTrueVertex;    //

   TGlobalPIDAlternate();
   TGlobalPIDAlternate(const TGlobalPIDAlternate & );
   virtual ~TGlobalPIDAlternate();

   ClassDef(TGlobalPIDAlternate,2); // Generated by MakeProject.
};
class TTrackerObject : public ND::TSubBaseObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      Charge;      //

   TTrackerObject();
   TTrackerObject(const TTrackerObject & );
   virtual ~TTrackerObject();

   ClassDef(TTrackerObject,2); // Generated by MakeProject.
};
class TSMRDObject : public ND::TSubBaseObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      avgtime;     //

   TSMRDObject();
   TSMRDObject(const TSMRDObject & );
   virtual ~TSMRDObject();

   ClassDef(TSMRDObject,2); // Generated by MakeProject.
};
class TP0DObject : public ND::TSubBaseShowerObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      Width;       //
   vector<int> ParticleId;    //
   vector<double> PIDWeight;     //

   TP0DObject();
   TP0DObject(const TP0DObject & );
   virtual ~TP0DObject();

   ClassDef(TP0DObject,2); // Generated by MakeProject.
};
class TECALObject : public ND::TSubBaseShowerObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      TrShVal;     //
   double      EMHadVal;    //
   TVector3    Width;       //
   double      AverageHitTime;    //
   double      EMEnergy;          //
   double      EMEnergyError;     //
   double      KFParameter;       //
   int         KFNNodes;          //
   double      KFParameterNodes;    //
   double      KFQuality;           //
   int         KFNDOF;              //
   double      KFHitQuality;        //
   double      MElectronTag_NDelayedCluster;    //
   vector<double> MElectronTag_NDelayedHits;       //
   vector<double> MElectronTag_DeltaX;             //
   vector<double> MElectronTag_DeltaY;             //
   vector<double> MElectronTag_DeltaZ;             //
   vector<double> MElectronTag_DeltaT;             //
   vector<double> MElectronTag_EDep;               //
   vector<double> MElectronTag_TrackEnd;           //
   double         EMEnergy_Likelihood_energyGrad;    //
   double         EMEnergy_Likelihood_energy_qsumGrad;    //
   double         EMEnergy_Likelihood_qsum_like;          //
   double         EMEnergy_Para_QSum;                     //
   double         EMEnergy_Para_QMean;                    //
   double         EMEnergy_Para_QRMS;                     //
   double         EMEnergy_Para_QSkew;                    //
   double         EMEnergy_Para_QMax;                     //
   int            IsShowerLike;                           //
   int            IsTrackLike;                            //
   double         PID_AMR;                                //
   double         PID_Angle;                              //
   double         PID_Max_Ratio;                          //
   double         PID_ShowerAngle;                        //
   double         PID_Asymmetry;                          //
   double         PID_MeanPosition;                       //
   double         PID_ShowerWidth;                        //
   int            mostUpStreamLayerHit;                   //
   int            mostDownStreamLayerHit;                 //

   TECALObject();
   TECALObject(const TECALObject & );
   virtual ~TECALObject();

   ClassDef(TECALObject,2); // Generated by MakeProject.
};
class TFGDObject : public ND::TSubBaseObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      avgtime;     //
   float       chargePerLayer[30];    //
   float       chargePerLayerAttenCorr[30];    //
   int         fgdContainment;                 //
   double      E;                              //
   double      x;                              //
   double      E_exp_muon;                     //
   double      E_exp_pion;                     //
   double      E_exp_proton;                   //
   double      sigmaE_muon;                    //
   double      sigmaE_pion;                    //
   double      sigmaE_proton;                  //
   double      PullMuon;                       //
   double      PullPion;                       //
   double      PullProton;                     //
   double      PullNotSet;                     //
   bool        isFgdVA;                        //
   bool        isFgdVA_flag;                   //
   double      fgdVA_upMinZ;                   //
   double      fgdVA_downMaxZ;                 //
   double      fgdVA_upCosTheta;               //
   double      fgdVA_downCosTheta;             //
   double      fgdVA_otherUpQ;                 //
   double      fgdVA_otherDownQ;               //
   double      fgdVA_verQ;                     //
   double      fgdVA_verLayQ;                  //
   double      fgdVA_verNearQ;                 //
   double      fgdVA_verNextNearQ;             //
   double      fgdVA_verNextNextNearQ;         //
   double      fgdVA_totalQ;                   //
   double      fgdVA_totalCorrE;               //

   TFGDObject();
   TFGDObject(const TFGDObject & );
   virtual ~TFGDObject();

   ClassDef(TFGDObject,2); // Generated by MakeProject.
};
class TTPCObject : public ND::TSubBaseObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      Charge;      //
   double      NTrun;       //70% of the number of cluster
   double      Ccorr;       //
   double      PullEle;     //
   double      PullMuon;    //
   double      PullPion;    //
   double      PullKaon;    //
   double      PullProton;    //
   double      dEdxexpEle;    //
   double      dEdxexpMuon;    //
   double      dEdxexpPion;    //
   double      dEdxexpKaon;    //
   double      dEdxexpProton;    //
   double      SigmaEle;         //
   double      SigmaMuon;        //
   double      SigmaPion;        //
   double      SigmaKaon;        //
   double      SigmaProton;      //
   TLorentzVector RefitPosition;    //
   TVector3       RefitDirection;    //
   double         RefitMomentum;     //
   double         RefitCharge;       //

   TTPCObject();
   TTPCObject(const TTPCObject & );
   virtual ~TTPCObject();

   ClassDef(TTPCObject,3); // Generated by MakeProject.
};
class TGlobalPID : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   UInt_t      UniqueID;    //
   vector<UInt_t> BrokenUniqueIDs;    //
   string         AlgorithmName;      //
   unsigned long  Detectors;          //
   bool           DetectorUsed[19];    //
   unsigned long  Status;              //
   int            NDOF;                //
   double         Chi2;                //
   int            NNodes;              //
   int            NHits;               //
   int            NConstituents;       //
   bool           isForward;           //
   bool           SenseOK;             //
   double         Charge;              //
   double         EDeposit;            //
   TVector3       Cone;                //
   double         Width;               //
   double         Length;              //
   vector<int>    ParticleIds;         //
   vector<double> PIDWeights;          //
   TLorentzVector FrontPosition;       //
   TLorentzVector BackPosition;        //
   TLorentzVector FrontPositionVar;    //
   TLorentzVector BackPositionVar;     //
   TVector3       FrontDirection;      //
   TVector3       BackDirection;       //
   TVector3       FrontDirectionVar;    //
   TVector3       BackDirectionVar;     //
   double         FrontMomentum;        //
   double         BackMomentum;         //
   double         FrontMomentumError;    //
   double         BackMomentumError;     //
   TLorentzVector PositionAtTrueVertex;    //
   TLorentzVector PositionVarAtTrueVertex;    //
   TVector3       DirectionAtTrueVertex;      //
   TVector3       DirectionVarAtTrueVertex;    //
   double         MomentumAtTrueVertex;        //
   double         MomentumErrorAtTrueVertex;    //
   int            EntranceOK[25];               //
   int            ExitOK[25];                   //
   TLorentzVector EntrancePosition[25];         //
   TLorentzVector ExitPosition[25];             //
   TVector3       EntranceDirection[25];        //
   TVector3       ExitDirection[25];            //
   double         EntranceMomentum[25];         //
   double         ExitMomentum[25];             //
   TLorentzVector EntrancePositionErr[25];      //
   TLorentzVector ExitPositionErr[25];          //
   TVector3       EntranceDirectionErr[25];     //
   TVector3       ExitDirectionErr[25];         //
   double         EntranceMomentumErr[25];      //
   double         ExitMomentumErr[25];          //
   Int_t          NHitsSaved;                   //
   TClonesArray*  HitsSaved;                    //
   ND::TTrueParticle TrueParticle;                 //
   Int_t             NTRACKERs;                    //
   TClonesArray*     TRACKER;                      //
   Int_t             NTPCs;                        //
   TClonesArray*     TPC;                          //
   Int_t             NFGDs;                        //
   TClonesArray*     FGD;                          //
   Int_t             NECALs;                       //
   Int_t             NDsECALs;                     //
   Int_t             NTrECALs;                     //
   TClonesArray*     ECAL;                         //
   Int_t             NP0DECALs;                    //
   TClonesArray*     P0DECAL;                      //
   Int_t             NP0Ds;                        //
   TClonesArray*     P0D;                          //
   Int_t             NSMRDs;                       //
   TClonesArray*     SMRD;                         //
   Int_t             NAlternates;                  //
   TClonesArray*     Alternates;                   //
   ND::TGlobalReconModule::TTPCObject hackTPCObject;                //
   ND::TGlobalReconModule::TFGDObject hackFGDObject;                //
   ND::TGlobalReconModule::TECALObject hackECALObject;               //
   ND::TGlobalReconModule::TP0DObject  hackP0DObject;                //
   ND::TGlobalReconModule::TSMRDObject hackSMRDObject;               //
   ND::TGlobalReconModule::TTrackerObject hackTrackerObject;            //
   ND::TGlobalReconModule::TGlobalPIDAlternate hackGlobalPIDAlternate;       //

   TGlobalPID();
   TGlobalPID(const TGlobalPID & );
   virtual ~TGlobalPID();

   ClassDef(TGlobalPID,2); // Generated by MakeProject.
};
class TGlobalHit : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      Charge;      //
   double      Time;        //
   TVector3    Position;    //
   TVector3    PositionError;    //
   int         Type;             //

   TGlobalHit();
   TGlobalHit(const TGlobalHit & );
   virtual ~TGlobalHit();

   ClassDef(TGlobalHit,2); // Generated by MakeProject.
};
class TOutermostHits : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   ND::TGlobalReconModule::TGlobalHit hitMinX;     //
   ND::TGlobalReconModule::TGlobalHit hitMaxX;     //
   ND::TGlobalReconModule::TGlobalHit hitMinY;     //
   ND::TGlobalReconModule::TGlobalHit hitMaxY;     //
   ND::TGlobalReconModule::TGlobalHit hitMinZ;     //
   ND::TGlobalReconModule::TGlobalHit hitMaxZ;     //

   TOutermostHits();
   TOutermostHits(const TOutermostHits & );
   virtual ~TOutermostHits();

   ClassDef(TOutermostHits,2); // Generated by MakeProject.
};
class TFgdTimeBin : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      minTime;     //
   double      maxTime;     //
   int         nHits[2];    //
   double      rawChargeSum[2];    //
   float       chargePerLayer[2][30];    //
   TVector3    chargeWeightedPos[2];     //
   ND::TGlobalReconModule::TOutermostHits FGD1OutermostHits;        //
   ND::TGlobalReconModule::TOutermostHits FGD2OutermostHits;        //
   Int_t                                  NFGD1Unused;              //Number of hits unused in FGD1   
   TClonesArray*                          FGD1Unused;               //The vector unused hits in FGD1
   Int_t                                  NFGD2Unused;              //Number of hits unused in FGD2 
   TClonesArray*                          FGD2Unused;               //The vector unused hits in FGD2  
   int                                    g4ID;                     //
   ND::TGlobalReconModule::TGlobalHit     hackFGDUnused;            //

   TFgdTimeBin();
   TFgdTimeBin(const TFgdTimeBin & );
   virtual ~TFgdTimeBin();

   ClassDef(TFgdTimeBin,2); // Generated by MakeProject.
};
class TFgdCluster : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      TotalCharge;    //
   double      TimeDisp;       //time dispersion of the cluster                          
   double      SpatialDisp;    //spatial dispersion of the cluster (distance between the two most far hits in the cluster)
   int         NHits;          //number of hits in the cluster                         
   TLorentzVector PosRMS;         //rms of the cluster position and time          
   TLorentzVector Position;       //
   TLorentzVector Variance;       //

   TFgdCluster();
   TFgdCluster(const TFgdCluster & );
   virtual ~TFgdCluster();

   ClassDef(TFgdCluster,2); // Generated by MakeProject.
};
class TVertexConstituent : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   int         Charge;      //
   double      Quality;     //
   TVector3    Momentum;    //
   int         PID;         //

   TVertexConstituent();
   TVertexConstituent(const TVertexConstituent & );
   virtual ~TVertexConstituent();

   ClassDef(TVertexConstituent,2); // Generated by MakeProject.
};
class TGlobalVertex : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   int         PrimaryIndex;    //
   string      AlgorithmName;    //
   int         Status;           //
   double      Quality;          //
   int         NDOF;             //
   TLorentzVector Position;         //
   TLorentzVector Variance;         //
   Int_t          NTrueVertices;    //
   TClonesArray*  TrueVertices;     //
   Int_t          NConstituents;    //
   TClonesArray*  Constituents;     //
   ND::TTrueVertex hackTrueVertexObject;    //
   ND::TGlobalReconModule::TVertexConstituent hackVertexConstituentObject;    //

   TGlobalVertex();
   TGlobalVertex(const TGlobalVertex & );
   virtual ~TGlobalVertex();

   ClassDef(TGlobalVertex,2); // Generated by MakeProject.
};

public:
// Data Members.
   bool        fRecPackInitialized;    //
   vector<string> fALLMODULES;            //
   bool           fPassedDetector[25];    //
   bool           fTestTPCInfo;           //
   Int_t          fNTPCPIDs;              //
   TClonesArray*  fTPCPIDs;               //
   Int_t          fNSMRDBottomUnused;     //
   Int_t          fNSMRDLeftUnused;       //
   Int_t          fNSMRDRightUnused;      //
   ND::TGlobalReconModule::TOutermostHits fP0DOutermostHits;      //

   TGlobalReconModule();
   TGlobalReconModule(const TGlobalReconModule & );
   virtual ~TGlobalReconModule();

   ClassDef(TGlobalReconModule,4); // Generated by MakeProject.
};
} // namespace
#endif
