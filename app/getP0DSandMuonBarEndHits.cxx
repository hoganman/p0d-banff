/// Application to calculate bar-to-bar equalization calibration constants in the P0D using reconstructed through going (sand) muons.
#define GETP0DSANDMUONBARENDHITS_CXX

#include <iostream>
#include <algorithm>
#include <sstream>

#include "TTree.h"
#include "TFile.h"
#include "TH2F.h"

#include <nd280EventLoop.hxx>
#include <TGeomInfo.hxx>
#include <TReconTrack.hxx>
#include <TTFBChannelId.hxx>

inline void PrintTVector3(TVector3 vec, std::string name)
{
    std::cout << name.c_str() << ": "; vec.Print();
}

//Comment this to hide/run DEBUGPRINT
//#define DEBUG
#define DEBUGPRINT(x) std::cout << #x << " = " << x << std::endl;

/// Since standard C++ doesn't let static strings be used inside class definitions,
/// Use this namespace
namespace SandMuonCuts
{
    /// The Kalman filter cut
    static const TString kKalmanFilterCut = "Kalman";

    /// Number of tracks in this analysis
    static const UInt_t kNumberOfTracksCut = 1;

    /// ~36 degress, anything less than this is cut
    static const Double_t kCosThetaCut = 0.8;

    /// There are 40 P0Dules.
    /// N.B. they are indexed from 0 to 39
    /// so use (last - first) + 1
    static const Int_t kNP0DulesCut = 40;

    /// Currently only interested in single track events
    static const Int_t kTrackCut = 1;

    ///If you want to accept accept single hits as well
    static const bool kAllowSingleton = false;
};

///These hard coded numbers are useful as TTree branches defaults
///if an analyzer is using cuts
namespace Resets
{
    /// The default value for a double
    static const Double_t kDouble = -9999.;

    /// The default value for a integer
    static const Int_t kInt = -1;

    /// The default value for a string/char*
    static const std::string kString = "";

    /// The default value for a bool
    static const Bool_t kBool = false;

};


class TTrackInBar
///A container for various track properites inside a bar
{
public:
    TTrackInBar() :
        StartEndLength(Resets::kDouble), OverEstimatedLength(Resets::kDouble),
        CorrectedLength(Resets::kDouble), IsSingleton(Resets::kBool),
        DistanceToClosestBaseVertex(Resets::kDouble), Hit(NULL)
    {}
    TTrackInBar(const ND::THandle<ND::TSingleHit>& hit) :
        StartEndLength(Resets::kDouble), OverEstimatedLength(Resets::kDouble),
        CorrectedLength(Resets::kDouble), IsSingleton(Resets::kBool),
        DistanceToClosestBaseVertex(Resets::kDouble)
    {
            Hit = hit;
    }
    TTrackInBar(const Double_t& correctedLength,
        const Double_t& overEstimatedLength, const ND::THandle<ND::TSingleHit>& hit):
        IsSingleton(Resets::kBool)
    {
            OverEstimatedLength = overEstimatedLength;
            CorrectedLength = correctedLength;
            CalculateStartEndLength();
            Hit = hit;
    }
    virtual ~TTrackInBar(){}

protected:

    Double_t StartEndLength;

    /// The amount of StartEndLength that is inside the fiber hole volume
    Double_t OverEstimatedLength;

    /// The difference between StartEndLength and OverEstimatedLength
    Double_t CorrectedLength;

    Bool_t IsSingleton;

    ///The attenuation for the hit
    Double_t Attentuation;

    Double_t DistanceToClosestBaseVertex;

    ND::THandle<ND::TSingleHit> Hit;

public:

    void CalculateAttentuation(const ND::THandle<ND::TReconNode>& node)
    {
        const Double_t distsensor = Hit->IsXHit() ? (node->GetState()->GetPosition() - Hit->GetPosition()).X()
                                                  : (node->GetState()->GetPosition() - Hit->GetPosition()).Y();
        const Double_t fiberlength = Hit->IsXHit() ? XBarLength : YBarLength;
        const Double_t promptResponse = LongFraction*exp(-distsensor/LongComponent) + (1.0-LongFraction)*exp(-distsensor/fShortComponent);
        const Double_t reflectResponse = (LongFraction*exp(-(2*fiberlength - distsensor)/LongComponent) + (1.0-LongFraction)*exp(-(2*fiberlength - distsensor)/ShortComponent))*Relectivity;
        Attentuation = promptResponse + reflectResponse;
    }

    Double_t GetStartEndLength() const {return StartEndLength;}
    Double_t GetOverEstimatedLength() const {return OverEstimatedLength;}
    Double_t GetCorrectedLength() const {return CorrectedLength;}
    Double_t GetDistanceToClosestBaseVertex() const {return DistanceToClosestBaseVertex;}
    Bool_t GetIsSingleton() const {return IsSingleton;}
    ND::THandle<ND::TSingleHit> GetHit() const {return Hit;}

    void SetCorrectedLength(const Double_t& length){CorrectedLength = length;}
    void SetOverEstimatedLength(const Double_t& length){OverEstimatedLength = length;}
    void CalculateStartEndLength(){ StartEndLength = OverEstimatedLength + CorrectedLength;}
    void SetIsSingleton(const Bool_t& state = true) { IsSingleton = state; }
    void SetDistanceToClosestBaseVertex(const Double_t& length) {DistanceToClosestBaseVertex = length;}

    //P0D Fiber constants taken from NCE code https://repo.nd280.org/viewvc/ND280/p0dAnalysis/macros/NCE/TP0DGeom.H?revision=1.7&view=markup
    static const Double_t LongFraction = 0.69;
    static const Double_t LongComponent = 508.1;
    static const Double_t ShortComponent = 51.8;
    static const Double_t Reflectivity = 0.80;
    static const Double_t XBarLength = 2209.;
    static const Double_t YBarLength = 2370.;

};

class P0DBarEndLoopFunction: public ND::TND280EventLoopFunction {
///User code for the event loop.
///This can be copied and used as a skeleton for generic user loop programs.

public:

    P0DBarEndLoopFunction(TH2F* barResponseMap = NULL) : ND::TND280EventLoopFunction()
    {
        p0dBarResponseMap = barResponseMap;
    }
    virtual ~P0DBarEndLoopFunction()
    {
        //if(chain) delete chain;
    }

    void Usage(void)
    {
        std::cout << "Usage:" << std::endl;
        std::cout << "     genP0DBarEndLoopFunction.exe -o output.root input.reco.root" << std::endl;
    }

    void Initialize(void);

    bool operator () (ND::TND280Event&);

    using TND280EventLoopFunction::Finalize;
    virtual void Finalize(ND::TND280Output*);

    /// Select a 3D track
    static const UInt_t k3Dimensions = 3;

    /// The number of cycles the electronic loop over. There are 8 spill cycles
    static const UInt_t kNP0DCycles = 24;

    /// The triangular bar is 17 mm high (excluding 0.1 mm added from paint
    /// and TiO2 coating)
    static const Double_t kBarHeight = 17.;

    /// The WS fiber is situated halfway in the bar height (in mm)
    static const Double_t kBarHalfHeight = 8.5;

    /// The WS fiber is situated halfway in the bar width as well (in mm)
    static const Double_t kBarHalfWidth = 16.5;

    /// units: mm
    static const Double_t kFiberHoleRadius = 1.3;

    /// The step size in which the fiber hole intersection is calculated
    /// units: mm
    static const Double_t kStepSize = 0.1;

    /// units: PEU (photoelectron units) per MIP per mm for doublet hits. See P0D NIM paper
    static const Double_t kDoubletBarMIPLightYield = 3.79;

    /// units: PEU (photoelectron units) per MIP per mm for singletons
    //static const Double_t kSingleBarMIPLightYield = 1.895;

    /// A rotation of 3Pi/2
    //static const Double_t kRotation = 4.712389;

protected:

    TH2F* p0dBarResponseMap;

    //output TTree
    TTree* chain;

    // These are the branches
    /**************************************/
    std::string AsString;
    Int_t RunNumber;
    Int_t SubRunNumber;
    Int_t EventNumber;
    Int_t Cycle;

    // units: PEU
    Double_t Charge;

    // units: mm
    Double_t StartEndLength;
    Double_t OverEstimatedLength;
    Double_t CorrectedLength;

    //Is the StartEndLength physically possible
    Int_t ValidLength;

    // X = Position[0], Y = Position[1], Z = Position[2]
    Double_t Position[k3Dimensions];
    Int_t IsSingleton;
    Int_t P0Dule;

    // IsX == !IsY
    Int_t IsX;
    Int_t IsY;

    // units: PEU / MIP / mm
    Double_t LightYieldPerMIPPerLength;

    // ONLY APPLIES TO CALIBRATED EVENTS
    // LYPMPL = LightYieldPerMIPPerLength
    // units: 1
    Double_t LYPMPLRatio;

    // only applies to doublets
    Double_t Position_Node[k3Dimensions];
    Double_t Direction_Node[k3Dimensions];
    Double_t CosTheta_Node;
    Double_t LightYieldPerMIPPerLength_Node;
    Double_t LYPMPLRatio_Node;
    Double_t Length_Node;

    // While I try to stick to using ints and doubles,
    // since the TFB info are given as unsigned integers,
    // and it is safe to set integers to -1 for safety purposes
    // using a long long perserves this ideology
    Long64_t TFB;
    Long64_t RMM;
    Long64_t Channel;
    Long64_t TripTId;
    Long64_t Capacitor;
    Long64_t TripChip;
    Long64_t ID;
    /**************************************/

protected:

    /// For each branch in the chain, reset the variables after each fill
    void ResetChainBranches();

    /// Sets the run number, sub-run number, etc
    void SetEventLevelInfo(const ND::TND280Event& event, const UInt_t& cycleNumber);

    /// Sets the hit ID, charge, length, etc
    void SetHitLevelInfo(const TTrackInBar* const trkBar);

    /// This method assumes that the first intersection point with the bar
    /// is the base
    void GetPathLengthEnterBase(const TVector3& prevNodePos,
            const TVector3& curNodePos, const TVector3& direction,
            const TVector3& trackDirection, const ND::THandle<ND::TSingleHit>& hit,
            TTrackInBar *trkBar, const Bool_t verbose=true) const;

    /// This method assumes that the last intersection point with the bar
    /// is the base
    void GetPathLengthExitBase(const TVector3& prevNodePos,
            const TVector3& curNodePos, const TVector3& direction,
            const TVector3& trackDirection, const ND::THandle<ND::TSingleHit>& hit,
            TTrackInBar *trkBar, const Bool_t verbose=true) const;

    /// Iteratively steps through the track and checks if it intersects the
    /// fiber hole
    Double_t GetLengthInFiberHole(const TVector3& startPosition,
            const TVector3& direction, const Double_t& totalLength,
            const TVector3& hitPosition, const Bool_t& isXHit) const;

    /// Since the layer is rectangular (trigangular bars stacked on
    /// one-another), the total length inside is the total height over the
    /// cos(angle)
    inline Double_t GetTotalLengthInLayer(const TVector3& direction) const
    {
            return kBarHeight / direction.CosTheta();
    }

    /// The energy deposition inside the scintillator should be 3.79 PEU /MIP/mm
    /// Thie function calculates what the MPPC measured
    inline Double_t GetLightYieldPerMIPPerLength(const Double_t& charge,
            const Double_t& length) const
    {
            return charge / length;
    }

    /// If you need to compare a list "array" with some elements
    /// If you need to compare with a vector called "var", use &var[0] as the array address
    template<class T>
    inline Bool_t IsElementInArray(const T& element, const T* array, const Long64_t& size) const
    {
        return std::find(array, array + size, element) != array + size;
    }

    /// Get the P0Dule number using the geometry
    inline Int_t GetP0Dule(const ND::THandle<ND::TSingleHit>& hit)
    {
        return ND::TGeomInfo::P0D().GetBar(hit->GetGeomId()).GetP0Dule();
    }

    Double_t GetLengthInDeadMaterial(const TVector3& startPositionInBar,
            const TVector3& direction, const Double_t& totalLength,
            const TVector3& hitPosition, const Bool_t& isXHit) const;


    Bool_t IsValidPathLengthInBar(const Double_t &lengthInBar) const;

    /// A dump method for debugging purposes
    void DumpHit(const ND::THandle<ND::TSingleHit>& hit) const;

};

//****************************************************************************
void P0DBarEndLoopFunction::ResetChainBranches()
//****************************************************************************
{
    using namespace Resets;
    AsString = kString;

    RunNumber = kInt;
    SubRunNumber = kInt;
    EventNumber = kInt;
    Cycle = kInt;
    RMM = kInt;
    TFB = kInt;
    P0Dule = kInt;
    Channel = kInt;
    TripChip = kInt;
    TripTId = kInt;
    IsSingleton = kInt;
    IsX = kInt;
    IsY = kInt;
    ID = kInt;
    ValidLength = kInt;

    StartEndLength = kDouble;
    OverEstimatedLength = kDouble;
    CorrectedLength = kDouble;
    Length_Node = kDouble;
    LightYieldPerMIPPerLength = kDouble;
    LightYieldPerMIPPerLength_Node = kDouble;
    LYPMPLRatio = kDouble;
    LYPMPLRatio_Node = kDouble;
    Charge = kDouble;

    for(UInt_t index = 0; index < k3Dimensions; ++index)
    {
        Position[index] = kDouble;
    }
}

//****************************************************************************
void P0DBarEndLoopFunction::Initialize(void)
//****************************************************************************
{
    chain = new TTree("P0DSandMuonHits", "");
    ResetChainBranches();

    chain->Branch("AsString", &AsString);
    chain->Branch("RunNumber", &RunNumber, "RunNumber/I");
    chain->Branch("SubRunNumber", &SubRunNumber, "SubRunNumber/I");
    chain->Branch("EventNumber", &EventNumber, "EventNumber/I");
    chain->Branch("Cycle", &Cycle, "Cycle/I");

    chain->Branch("RMM", &RMM, "RMM/L");
    chain->Branch("TFB", &TFB, "TFB/L");
    //chain->Branch("Channel", &Channel, "Channel/L");
    //chain->Branch("Capacitor", &Capacitor, "Capacitor/L");
    //chain->Branch("TripChip", &TripChip, "TripChip/L");
    //chain->Branch("TripTId", &TripTId, "TripTId/L");
    chain->Branch("P0Dule", &P0Dule, "P0Dule/L");

    //chain->Branch("StartEndLength", &StartEndLength, "StartEndLength/D");
    //chain->Branch("ValidLength", &ValidLength, "ValidLength/I");
    //chain->Branch("OverEstimatedLength", &OverEstimatedLength, "OverEstimatedLength/D");
    //chain->Branch("CorrectedLength", &CorrectedLength, "CorrectedLength/D");

    //chain->Branch("ID", &ID, "ID/L");
    //chain->Branch("Charge", &Charge, "Charge/D");

    //chain->Branch("Position", Position, TString::Format("Position[%d]/D", k3Dimensions));
    //chain->Branch("IsSingleton", &IsSingleton, "IsSingleton/I");

    //chain->Branch("IsX", &IsX, "IsX/I");
    //chain->Branch("IsY", &IsY, "IsY/I");
    //chain->Branch("LightYieldPerMIPPerLength", &LightYieldPerMIPPerLength, "LightYieldPerMIPPerLength/D");
    //chain->Branch("LYPMPLRatio", &LYPMPLRatio, "LYPMPLRatio/D");

    chain->Branch("NodePosition")
    chain->Branch("Length_Node", &Length_Node, "Length_Node/D");
    chain->Branch("LightYieldPerMIPPerLength_Node", &LightYieldPerMIPPerLength_Node, "LightYieldPerMIPPerLength_Node/D");
    chain->Branch("LYPMPLRatio_Node", &LYPMPLRatio_Node, "LYPMPLRatio_Node/D");

}

//****************************************************************************
void P0DBarEndLoopFunction::SetEventLevelInfo(const ND::TND280Event& event, const UInt_t& cycleNumber)
//****************************************************************************
{
    RunNumber = event.GetContext().GetRun();
    SubRunNumber = event.GetContext().GetSubRun();
    EventNumber = event.GetContext().GetEvent();
    Cycle = cycleNumber;
}

//****************************************************************************
void P0DBarEndLoopFunction::SetHitLevelInfo(const TTrackInBar* const trkBar)
//****************************************************************************
{
    const ND::THandle<ND::TSingleHit> hit = trkBar->GetHit();
    const ND::TTFBChannelId tfbChannel = (ND::TTFBChannelId)(hit->GetChannelId());
    IsSingleton = trkBar->GetIsSingleton();

    const TVector3 hitPosition = hit->GetPosition();
    Position[0] = hitPosition.X();
    Position[1] = hitPosition.Y();
    Position[2] = hitPosition.Z();

    Charge = hit->GetCharge();
    IsX = hit->IsXHit();
    IsY = hit->IsYHit();

    RMM = tfbChannel.GetRMM();
    Channel = tfbChannel.GetChannel();
    TFB = tfbChannel.GetTFB();
    TripChip = tfbChannel.GetTripChip();
    Capacitor = tfbChannel.GetCapacitor();
    TripTId = tfbChannel.GetTripTId();
    AsString = tfbChannel.AsString();
    ID = tfbChannel.GetCableId();

    StartEndLength = trkBar->GetStartEndLength();
    OverEstimatedLength = trkBar->GetOverEstimatedLength();
    CorrectedLength = trkBar->GetCorrectedLength();
    ValidLength = IsValidPathLengthInBar(StartEndLength);
    LightYieldPerMIPPerLength = GetLightYieldPerMIPPerLength(Charge, CorrectedLength);
    LYPMPLRatio = LightYieldPerMIPPerLength / kSingleBarMIPLightYield;

#ifdef DEBUG
    if (CorrectedLength < 0 && CorrectedLength > -9900 ) {
      std::cout << "warning: negative non-default CorrectedLength" << std::endl;
      DEBUGPRINT(StartEndLength)
      DEBUGPRINT(OverEstimatedLength)
    }
#endif
}

//****************************************************************************
bool P0DBarEndLoopFunction::operator () (ND::TND280Event& event) {
//****************************************************************************

    ResetChainBranches();

    // Loop over cycles and find p0d results with tracks
    for(UInt_t cycleNum = 0; cycleNum < kNP0DCycles; ++cycleNum)
    {
        ND::THandle<ND::TAlgorithmResult> p0dRecon = event.GetFit(TString::Format("p0dRecon/p0dCycle%d", cycleNum));
        if( !p0dRecon )
            continue; // skip this cycle

        ND::THandle<ND::TReconObjectContainer> tracks =
        p0dRecon->GetResultsContainer("TP0DTrackRecon/TP0D3DTrackMatching/final"); //Prod-5
        if( !tracks )
            continue; // skip this cycle

        //Np0dTrks = tracks->size();
        if( tracks->size() != SandMuonCuts::kTrackCut )
            continue; // skip this cycle

        ND::THandle<ND::TReconTrack> track = tracks->front();
        if( !track )
            continue; // skip this cycle

        ND::THandle<ND::TTrackState> trackState = track->GetState();
        if( !trackState )
            continue; //skip this cycle

        // make sure it's a 3d track
        const Bool_t isX = !trackState->IsFree(trackState->GetXIndex());
        const Bool_t isY = !trackState->IsFree(trackState->GetYIndex());

        if( !(isX && isY) )
            continue; //skip this cycle

        //store the track direction just in case
        const TVector3 trackDirection = track->GetDirection();

        //only analyze tracks that are very forward going
        if(trackDirection.CosTheta() < SandMuonCuts::kCosThetaCut)
            continue; // skip this cycle

        // Get track nodes
        const ND::TReconNodeContainer &nodes = track->GetNodes();

        const ND::THandle<ND::THitSelection> frontHits = nodes.front()->GetObject()->GetHits();
        const Int_t minP0Dule = GetP0Dule(*frontHits->begin());

        const ND::THandle<ND::THitSelection> backHits = nodes.back()->GetObject()->GetHits();
        const Int_t maxP0Dule = GetP0Dule(*backHits->begin());

        //Length of track in P0Dules
        //skip cycle if not through-going track
        const Int_t nP0DulesTraveled = (maxP0Dule - minP0Dule) + 1;
        if(nP0DulesTraveled != SandMuonCuts::kNP0DulesCut)
            continue; //skip this cycle

        //Start at where the track start position is thought to be
        TVector3 referencePosition = track->GetPosition().Vect();

        //Loop over the nodes and in each node, loop over the hits
        //A Kalman filtered track has a 3D node in each layer
        for(UInt_t nodeIndex = 0; nodeIndex < nodes.size(); ++nodeIndex)
        {
            const ND::THandle<ND::TReconNode> curNode = nodes.at(nodeIndex);
            const ND::THandle<ND::THitSelection> nodeHits = curNode->GetObject()->GetHits();

            //make sure we only use doublets
            if( nodeHits->size() != 2 )
                continue;

            const ND::THandle<ND::TTrackState> nodeState = curNode->GetState();
            const TVector3 nodePosition = nodeState->GetPosition().Vect();
            const ND::TTFBChannelId chanID = (ND::TTFBChannelId)(nodeHits->front()->GetChannelId());

            //make sure that the hits are P0D (to be safe)
            if(!chanID.IsValid() || !chanID.IsTFBChannel() || chanID.GetSubDetector() != ND::TChannelId::kP0D)
                continue;

            TTrackInBar* const trackInBarA = new TTrackInBar(nodeHits->front());
            TTrackInBar* const trackInBarB = new TTrackInBar(nodeHits->back());

            trackInBarA->CalculateAttentuation(curNode);
            trackInBarB->CalculateAttentuation(curNode);

            const TVector3 direction = nodeState->GetDirection();
            Length_Node = kBarHeight / direction.CosTheta();
            LightYieldPerMIPPerLength_Node = (trackInBarA->Hit->GetCharge()/trackInBarA->Attentuation +
                                              trackInBarB->Hit->GetCharge()/trackInBarB->Attentuation) / Length_Node;


            /*
            for(ND::THitSelection::const_iterator hitIter = nodeHits->begin();
                  hitIter != nodeHits->end();
                  ++hitIter)
            {
                //This hit must be valid and can only be an X- or Y-layer
                ND::THandle<ND::TSingleHit> nodeHit = *hitIter;
                if (!nodeHit)
                    continue;
                //std::cout << "nodeHit: " << nodeHit << std::endl;
                //std::cout << "GeomID: " << nodeHit->GetGeomId().AsInt() << std::endl;
                const ND::TTFBChannelId chanID = (ND::TTFBChannelId)(nodeHit->GetChannelId());
                if(!chanID.IsValid() || !chanID.IsTFBChannel() || chanID.GetSubDetector() != ND::TChannelId::kP0D)
                    continue;
                if(nodeHit->IsXHit())
                    xHits.push_back(nodeHit);
                else
                    yHits.push_back(nodeHit);
            }

            //We are at the start of the track, but since P0DRecon sets the
            //the beginning of the track as the first node, step back a bit
            //so that we do not get NaN later on
            if((nodePosition - referencePosition).Mag() < 1)
            {
                const TVector3 direction = nodeState->GetDirection();
                referencePosition = nodePosition - (kBarHalfWidth / direction.CosTheta()) * nodeState->GetDirection();
            }

            //Only one of the vectors has entries
            ND::THitSelection* hits = NULL;
            Bool_t xLayer = false;
            if(xHits.size() > 0)
            {
                 hits = &xHits;
                 xLayer = true;
            }
            else
            {
                 hits = &yHits;
                 xLayer = false;
            }

            // This direction should be the vector pointing between adjecent nodes
            const TVector3 direction = nodeState->GetDirection();
            const Double_t totalLengthInLayer = GetTotalLengthInLayer(direction);
            const UInt_t nHits = hits->size();

            //If we have the singleton case
            if(nHits == 1 && SandMuonCuts::kAllowSingleton)
            {
                const ND::THandle<ND::TSingleHit> nodeHit = hits->front();
                ND::TChannelId hitChannel = nodeHit->GetChannelId(0);

                // Since we do not know a-priori whether the bar enters the base
                // or top side of the bar, test both configurations and idealy
                // we should only get one good answer
                TTrackInBar* const trackInBarEnterBase = new TTrackInBar(nodeHit);
                TTrackInBar* const trackInBarExitBase = new TTrackInBar(nodeHit);
                trackInBarEnterBase->SetIsSingleton();
                trackInBarExitBase->SetIsSingleton();

                GetPathLengthEnterBase(referencePosition, nodePosition, direction, trackDirection, nodeHit, trackInBarEnterBase, false);
                GetPathLengthExitBase(referencePosition, nodePosition, direction, trackDirection, nodeHit, trackInBarExitBase, false);

                const Double_t lengthEnterBase = trackInBarEnterBase->GetCorrectedLength();
                const Double_t lengthExitBase = trackInBarExitBase->GetCorrectedLength();

                // Prevent any nonesene
                if(std::min(lengthEnterBase, lengthExitBase) > totalLengthInLayer ||
                   std::max(lengthEnterBase, lengthExitBase) < 0 ||
                   TMath::IsNaN(lengthEnterBase) || TMath::IsNaN(lengthExitBase))
                {
                    std::cout << "Invalid length calculations!" << std::endl;
                }
                // both answers seem to be valid, chose the one with longest length
                else if(std::max(lengthEnterBase, lengthExitBase) < totalLengthInLayer &&
                        std::min(lengthEnterBase, lengthExitBase) > 0)
                {
                    if(lengthEnterBase > lengthExitBase)
                    {
                        SetEventLevelInfo(event, cycleNum);
                        SetHitLevelInfo(trackInBarEnterBase);
                        chain->Fill();
                        ResetChainBranches();
                    }
                    else
                    {
                        SetEventLevelInfo(event, cycleNum);
                        SetHitLevelInfo(trackInBarExitBase);
                        chain->Fill();
                        ResetChainBranches();
                    }
                }
                // the track seems to have intersected the base of the bar first
                else if(lengthEnterBase < totalLengthInLayer && (lengthExitBase > totalLengthInLayer || lengthExitBase < 0))
                {
                    // This seems to be a failure mode where the length is suspicously small for a
                    // singleton. So let's redo it assuming the length in the bar is the maximum length
                    if(lengthEnterBase < kBarHalfHeight)
                    {
                        std::cout << "The singleton hit is a little short, trying again..." << std::endl;
                        const Double_t deltaZ = std::fabs(nodeHit->GetPosition().Z() - kBarHalfHeight - referencePosition.Z());
                        const TVector3 startPosition = referencePosition + (deltaZ / direction.CosTheta() * direction);
                        const Double_t lengthInHole = GetLengthInFiberHole(startPosition, direction, totalLengthInLayer, nodeHit->GetPosition(), nodeHit->IsXHit());



                        const Double_t lengthInDeadMaterial = GetLengthInDeadMaterial(startPosition, direction, totalLengthInLayer, nodeHit->GetPosition(), nodeHit->IsXHit());
                        //DEBUGPRINT(lengthInDeadMaterial)

                        //trackInBarEnterBase->SetCorrectedLength(totalLengthInLayer-lengthInHole-lengthInDeadMaterial);
                        //trackInBarEnterBase->SetOverEstimatedLength(lengthInHole+lengthInDeadMaterial);
                        trackInBarEnterBase->SetOverEstimatedLength(lengthInHole);
                        trackInBarEnterBase->SetCorrectedLength(totalLengthInLayer-lengthInHole);
                        trackInBarEnterBase->CalculateStartEndLength();
                    }
                    SetEventLevelInfo(event, cycleNum);
                    SetHitLevelInfo(trackInBarEnterBase);
                    chain->Fill();
                    ResetChainBranches();
                }
                // the track seems to have intersected the base of the bar last
                else if(lengthExitBase < totalLengthInLayer && (lengthEnterBase > totalLengthInLayer || lengthEnterBase < 0))
                {
                    // This seems to be a failure mode where the length is suspicously small for a
                    // singleton. So let's redo it assuming the length in the bar is the maximum length
                    if(lengthExitBase < kBarHalfHeight)
                    {
                        std::cout << "The singleton hit is a little short, trying again..." << std::endl;
                        const Double_t deltaZ = std::fabs(nodeHit->GetPosition().Z() + kBarHalfHeight - referencePosition.Z());
                        const TVector3 startPosition = referencePosition + (deltaZ / direction.CosTheta() * direction);
                        const Double_t lengthInHole = GetLengthInFiberHole(startPosition, -1 * direction, totalLengthInLayer, nodeHit->GetPosition(), nodeHit->IsXHit());

                        const Double_t lengthInDeadMaterial = GetLengthInDeadMaterial(startPosition, -1 * direction, totalLengthInLayer, nodeHit->GetPosition(), nodeHit->IsXHit());
                        //DEBUGPRINT(lengthInDeadMaterial)

                        //trackInBarExitBase->SetOverEstimatedLength(lengthInHole+lengthInDeadMaterial);
                        //trackInBarExitBase->SetCorrectedLength(totalLengthInLayer-lengthInHole-lengthInDeadMaterial);
                        trackInBarExitBase->SetOverEstimatedLength(lengthInHole);
                        trackInBarExitBase->SetCorrectedLength(totalLengthInLayer-lengthInHole);
                        trackInBarExitBase->CalculateStartEndLength();
                    }

                    SetEventLevelInfo(event, cycleNum);
                    SetHitLevelInfo(trackInBarExitBase);
                    chain->Fill();
                    ResetChainBranches();
                }
                else
                {
                    std::cout << "Should NOT get here!" << std::endl;
                }

            delete trackInBarEnterBase;
            delete trackInBarExitBase;
            } // end-if for singleton case

            //We have two hits. The theta cut should remove triplets and more
            else if(nHits == 2)
            {
                const ND::THandle<ND::TSingleHit> nodeHitA = hits->at(0);
                const ND::THandle<ND::TSingleHit> nodeHitB = hits->at(1);
                TTrackInBar* const trackInBarA = new TTrackInBar(nodeHitA);
                TTrackInBar* const trackInBarB = new TTrackInBar(nodeHitB);


                // These checks determine which bar is intersected first
                // If going in positive X/Y direction, then lower X/Y bar is hit first
                if((xLayer && direction.X() > 0) || (!xLayer && direction.Y() > 0))
                {
                    //hit "A" is higher than hit "B", so "B" is intersected
                    //first since track has postive X/Y direction
                    if((xLayer && nodeHitA->GetPosition().X() > nodeHitB->GetPosition().X()) ||
                         (!xLayer && nodeHitA->GetPosition().Y() > nodeHitB->GetPosition().Y()))
                    {
                        GetPathLengthEnterBase(referencePosition, nodePosition, direction, trackDirection, nodeHitB, trackInBarB);
                        GetPathLengthExitBase(referencePosition, nodePosition, direction, trackDirection, nodeHitA, trackInBarA);
                    }
                    //hit "A" is lower than hit "B", so "A" is intersected
                    //first since track has postive X/Y direction
                    else
                    {
                        GetPathLengthEnterBase(referencePosition, nodePosition, direction, trackDirection, nodeHitA, trackInBarA);
                        GetPathLengthExitBase(referencePosition, nodePosition, direction, trackDirection, nodeHitB, trackInBarB);
                    }
                }
                // If going in negative X/Y direction, then higher X/Y bar is hit first
                else
                {
                    //hit "A" is higher than hit "B", so "A" is intersected
                    //first since track has negative X/Y direction
                    if((xLayer && nodeHitA->GetPosition().X() > nodeHitB->GetPosition().X()) ||
                        (!xLayer && nodeHitA->GetPosition().Y() > nodeHitB->GetPosition().Y()))
                    {
                        GetPathLengthEnterBase(referencePosition, nodePosition, direction, trackDirection, nodeHitA, trackInBarA);
                        GetPathLengthExitBase(referencePosition, nodePosition, direction, trackDirection, nodeHitB, trackInBarB);
                    }
                    //hit "A" is lower than hit "B", so "B" is intersected
                    //first since track has negative X/Y direction
                    else
                    {
                        GetPathLengthEnterBase(referencePosition, nodePosition, direction, trackDirection, nodeHitB, trackInBarB);
                        GetPathLengthExitBase(referencePosition, nodePosition, direction, trackDirection, nodeHitA, trackInBarA);
                    }
                }
                SetEventLevelInfo(event, cycleNum);
                SetHitLevelInfo(trackInBarA);

                Length_Node = kBarHeight / direction.CosTheta();
                LightYieldPerMIPPerLength_Node = nodeState->GetEDeposit() / Length_Node;
                LYPMPLRatio_Node = LightYieldPerMIPPerLength_Node / kDoubletBarMIPLightYield;
                chain->Fill();
                ResetChainBranches();

                SetEventLevelInfo(event, cycleNum);
                SetHitLevelInfo(trackInBarB);
                Length_Node = kBarHeight / direction.CosTheta();
                LightYieldPerMIPPerLength_Node = nodeState->GetEDeposit() / Length_Node;
                LYPMPLRatio_Node = LightYieldPerMIPPerLength_Node / kDoubletBarMIPLightYield;
                chain->Fill();
                ResetChainBranches();

                delete trackInBarA;
                delete trackInBarB;

            } // end-if for doublet case
            else
            {
#ifdef DEBUG
                std::cout << "There were " << nHits << " hits, ignoring!!!" << std::endl;
                for(ND::THitSelection::const_iterator hitIter = hits->begin();
                        hitIter != hits->end();
                        ++hitIter)
                {
                    const ND::THandle<ND::TSingleHit> hit = *hitIter;
                    DumpHit(hit);
                }
#endif
            } // end-else
            //set the next reference point to the current node position
            referencePosition = nodePosition;

        } // end loop over track nodes
        */
    }// end loop over cycles

    return false; // Don't save events...
}

//****************************************************************************
void P0DBarEndLoopFunction::Finalize(ND::TND280Output* output)
//****************************************************************************
{
    std::cout << "Finalize!" << std::endl;
    //This also saved the ND280Event TTree, but that is not a bad thing.
    //Just makes the file larger
    chain->Write();
}

//****************************************************************************
void P0DBarEndLoopFunction::GetPathLengthEnterBase(const TVector3& prevNodePos,
        const TVector3& curNodePos, const TVector3& direction,
        const TVector3& trackDirection, const ND::THandle<ND::TSingleHit>& hit,
        TTrackInBar *trkBar, const Bool_t verbose) const
//****************************************************************************
{
    TVector3 hitPosition = hit->GetPosition();

    //This actually has little effect on the final result
    //const TVector3 useDirection = trkBar->GetIsSingleton() ? trackDirection : direction;
    const TVector3& useDirection = direction;

    // Base vertex 1 is always lower in X/Y
    TVector3 barVertex1(0, 0, -kBarHalfHeight);
    // Base vertex 2 is always higher in X/Y
    TVector3 barVertex2(0, 0, -kBarHalfHeight);
    // Bar top vertex (number 3) is downstream
    TVector3 barVertex3(0, 0, +kBarHalfHeight);

    TVector3 orthogonalDir;

    //We have a X hit which has no Y info
    if(hit->IsXHit())
    {
        orthogonalDir.SetXYZ(0, 1, 0);
        //hitPosition.SetY(0);
        barVertex1.SetX(-kBarHalfWidth);
        barVertex2.SetX(+kBarHalfWidth);

        //Set the bar local coordinates
        barVertex1 += hitPosition;
        barVertex2 += hitPosition;
        barVertex3 += hitPosition;

        barVertex1.SetY(curNodePos.Y());
        barVertex2.SetY(curNodePos.Y());
        barVertex3.SetY(curNodePos.Y());
    }
    //We have a Y hit which has no X info
    else if(hit->IsYHit())
    {
        orthogonalDir.SetXYZ(1, 0, 0);
        //hitPosition.SetX(0);
        barVertex1.SetY(-kBarHalfWidth);
        barVertex2.SetY(+kBarHalfWidth);

        //Set the bar local coordinates
        barVertex1 += hitPosition;
        barVertex2 += hitPosition;
        barVertex3 += hitPosition;

        barVertex1.SetX(curNodePos.X());
        barVertex2.SetX(curNodePos.X());
        barVertex3.SetX(curNodePos.X());
    }
    else
    {
        std::cout << "This is not a valid track to analyze" << std::endl;
        return;
    }

    const Double_t deltaZ = std::fabs(barVertex1.Z() - prevNodePos.Z());
    // The point where the track intersects the base plane
    TVector3 firstIntersectPoint = prevNodePos + ((deltaZ / useDirection.CosTheta()) * useDirection);

    if(hit->IsXHit())
    {
        //This is a failure mode, where from the previous node, the track
        //useDirection does NOT point into the bar, so we define the
        //useDirection here as vector between the hit position itself and the
        //previous node position
        if(firstIntersectPoint.X() > barVertex2.X() || firstIntersectPoint.X() < barVertex1.X())
        {
#ifdef DEBUG
            std::cout << "WARNING: Track intersection with bar assumption has been broken, will use hit position instead..." << std::endl;
#endif
            const TVector3 newDirection = 1./(hitPosition-prevNodePos).Mag() * (hitPosition-prevNodePos);
            firstIntersectPoint = prevNodePos + ((deltaZ / (newDirection).CosTheta()) * newDirection);
        }
    }
    else if(hit->IsYHit())
    {
        //This is a failure mode, where from the previous node, the track
        //useDirection does NOT point into the bar, so we define the
        //useDirection here as vector between the hit position itself and the
        //previous node position

        if(firstIntersectPoint.Y() > barVertex2.Y() || firstIntersectPoint.Y() < barVertex1.Y())
        {
#ifdef DEBUG
            std::cout << "WARNING: Track intersection with bar assumption has been broken, will use hit position instead..." << std::endl;
#endif
            const TVector3 newDirection = 1./(hitPosition-prevNodePos).Mag() * (hitPosition-prevNodePos);
            firstIntersectPoint = prevNodePos + ((deltaZ / (newDirection).CosTheta()) * newDirection);
        }
    }

    // These vectors are parallel to the bar sides towards the top vertex "3"
    // The convention here is vector_to_from so unitVector31 is a unit vector
    // going to vertex "3" from vertex "1"
    const TVector3 unitVector31 = 1./(barVertex3 - barVertex1).Mag() * (barVertex3 - barVertex1);
    const TVector3 unitVector32 = 1./(barVertex3 - barVertex2).Mag() * (barVertex3 - barVertex2);

    //Define two planes that the track exits towards by their normal vectors
    TVector3 normVector31 = orthogonalDir.Cross(unitVector31);
    TVector3 normVector32 = orthogonalDir.Cross(unitVector32);

    //The algebra assumes that the normal vectors have positive Z components
    if(normVector31.Z() < 0)
          normVector31 = -1 * normVector31;
    if(normVector32.Z() < 0)
          normVector32 = -1 * normVector32;

    // How long would the line need to be to reach the plane?
    // Since we do not know which side of the bar the track exits,
    // test both cases. The correct length is the one where
    // the angle between the plane normal vector and useDirection
    // vector is minimized
    const Double_t lengthInBar31 = (barVertex3 - firstIntersectPoint).Dot(normVector31) / useDirection.Dot(normVector31);
    const Double_t lengthInBar32 = (barVertex3 - firstIntersectPoint).Dot(normVector32) / useDirection.Dot(normVector32);
    const TVector3 futherAlongTrack = firstIntersectPoint + (kBarHeight / useDirection.CosTheta()) * useDirection;
    const Double_t distanceToBarVertex1 = (futherAlongTrack - barVertex1).Mag();
    const Double_t distanceToBarVertex2 = (futherAlongTrack - barVertex2).Mag();
    Double_t lengthInBar;

    trkBar->SetDistanceToClosestBaseVertex(std::min(distanceToBarVertex1, distanceToBarVertex2));

    if(!IsValidPathLengthInBar(lengthInBar31) || !IsValidPathLengthInBar(lengthInBar32))
    {
#ifdef DEBUG
        std::cout << "GetPathLengthEnterBase" << std::endl;
        std::cout << "Some value is NaN or physically impossible" << std::endl;
        std::cout << "hitPostion: "; hitPosition.Print();
        std::cout << "barVertex1: "; barVertex1.Print();
        std::cout << "barVertex2: "; barVertex2.Print();
        std::cout << "barVertex3: "; barVertex3.Print();
        std::cout << "prevNodePos: "; prevNodePos.Print();
        std::cout << "curNodePos: "; curNodePos.Print();
        std::cout << "useDirection: "; useDirection.Print();
        std::cout << "firstIntersectPoint: "; firstIntersectPoint.Print();
        DEBUGPRINT(lengthInBar31)
        DEBUGPRINT(lengthInBar32)
        DEBUGPRINT(distanceToBarVertex1)
        DEBUGPRINT(distanceToBarVertex2)
        std::cout << "normVector31: "; normVector31.Print();
        std::cout << "normVector32: "; normVector32.Print();
        DEBUGPRINT((barVertex3 - firstIntersectPoint).Dot(normVector31))
        DEBUGPRINT((barVertex3 - firstIntersectPoint).Dot(normVector32))
        DEBUGPRINT(useDirection.Dot(normVector31))
        DEBUGPRINT(useDirection.Dot(normVector32))
#endif
        return;
    }
    else if(lengthInBar31 < kBarHeight && lengthInBar32 > kBarHeight)
    {
        lengthInBar = lengthInBar31;
    }
    else if(lengthInBar32 < kBarHeight && lengthInBar31 > kBarHeight)
    {
        lengthInBar = lengthInBar32;
    }
    // Both lengths seem to be reasonable, pick the length with the shortest
    // distance to the other verticies
    else if(distanceToBarVertex1 < distanceToBarVertex2)
    {
        lengthInBar = lengthInBar31;
    }
    else if(distanceToBarVertex2 < distanceToBarVertex1)
    {
        lengthInBar = lengthInBar32;
    }
    else
    {
        std::cout << "Something is very broken (BAD!), returning..." << std::endl;
        return;
    }

    Double_t overEstimatedLength = GetLengthInFiberHole(firstIntersectPoint, useDirection, lengthInBar, hitPosition, hit->IsXHit());
    std::cout << "overEstimatedLength = " << overEstimatedLength << std::endl;

    /*
    // This is not used yet!
    const Double_t lengthInDeadMaterial = GetLengthInDeadMaterial(firstIntersectPoint, useDirection, lengthInBar, hitPosition, hit->IsXHit());
#ifdef DEBUG
    DEBUGPRINT(lengthInDeadMaterial)
#endif
    overEstimatedLength += lengthInDeadMaterial;
    */

    // Subtract off whatever contribution the fiber hole might have given in this calculation
    const Double_t lengthInBar_AfterOverEstimateSubtraction = lengthInBar - overEstimatedLength;

    trkBar->SetCorrectedLength(lengthInBar_AfterOverEstimateSubtraction);
    trkBar->SetOverEstimatedLength(overEstimatedLength);
    trkBar->CalculateStartEndLength();
    return;
}

//****************************************************************************
void P0DBarEndLoopFunction::GetPathLengthExitBase(const TVector3& prevNodePos,
        const TVector3& curNodePos, const TVector3& direction,
        const TVector3& trackDirection, const ND::THandle<ND::TSingleHit>& hit,
        TTrackInBar *trkBar, const Bool_t verbose) const
//****************************************************************************
{
    TVector3 hitPosition = hit->GetPosition();

    //This actually has little effect on the final result
    //const TVector3 useDirection = trkBar->GetIsSingleton() ? -1 * trackDirection : -1 * direction;
    const TVector3 useDirection = -1 * direction;

    TVector3 orthogonalDir;

    // Base vertex 1 is always lower in X/Y
    TVector3 barVertex1(0, 0, +kBarHalfHeight);
    // Base vertex 2 is always higher in X/Y
    TVector3 barVertex2(0, 0, +kBarHalfHeight);
    // Bar top vertex (number 3) is downstream
    TVector3 barVertex3(0, 0, -kBarHalfHeight);


    //We have a X hit which has no Y info
    if(hit->IsXHit())
    {
        orthogonalDir.SetXYZ(0, 1, 0);
        //hitPosition.SetY(0);
        barVertex1.SetX(-kBarHalfWidth);
        barVertex2.SetX(+kBarHalfWidth);

        //Set the bar local coordinates
        barVertex1 += hitPosition;
        barVertex2 += hitPosition;
        barVertex3 += hitPosition;

        barVertex1.SetY(curNodePos.Y());
        barVertex2.SetY(curNodePos.Y());
        barVertex3.SetY(curNodePos.Y());
    }
    //We have a Y hit which has no X info
    else if(hit->IsYHit())
    {
        orthogonalDir.SetXYZ(1, 0, 0);
        //hitPosition.SetX(0);
        barVertex1.SetY(-kBarHalfWidth);
        barVertex2.SetY(+kBarHalfWidth);

        //Set the bar local coordinates
        barVertex1 += hitPosition;
        barVertex2 += hitPosition;
        barVertex3 += hitPosition;

        barVertex1.SetX(curNodePos.X());
        barVertex2.SetX(curNodePos.X());
        barVertex3.SetX(curNodePos.X());
    }
    else
    {
        std::cout << "This is not a valid track to analyze" << std::endl;
        return;
    }

    const Double_t deltaZ = std::fabs(barVertex1.Z() - prevNodePos.Z());
    // The point where the track intersects the base plane
    TVector3 lastIntersectPoint = (prevNodePos + ((deltaZ / useDirection.CosTheta()) * useDirection));

    if(hit->IsXHit())
    {
        //This is a failure mode, where from the previous node, the track
        //useDirection does NOT point into the bar, so we define the
        //useDirection here as vector between the hit position itself and the
        //previous node position. The angle between between the direction and
        //this new direction should be small
        if(lastIntersectPoint.X() > barVertex2.X() || lastIntersectPoint.X() < barVertex1.X())
        {
#ifdef DEBUG
            std::cout << "WARNING: Track intersection with bar assumption has been broken, will use hit position instead..." << std::endl;
#endif
            const TVector3 newDirection = 1./(hitPosition-prevNodePos).Mag() * (hitPosition-prevNodePos);
            lastIntersectPoint = prevNodePos + ((deltaZ / (newDirection).CosTheta()) * newDirection);
        }
    }
    else if(hit->IsYHit())
    {
        //This is a failure mode, where from the previous node, the track
        //useDirection does NOT point into the bar, so we define the
        //useDirection here as vector between the hit position itself and the
        //previous node position. The angle between between the direction and
        //this new direction should be small
        if(lastIntersectPoint.Y() > barVertex2.Y() || lastIntersectPoint.Y() < barVertex1.Y())
        {
#ifdef DEBUG
            std::cout << "WARNING: Track intersection with bar assumption has been broken, will use hit position instead..." << std::endl;
#endif
            const TVector3 newDirection = 1./(hitPosition-prevNodePos).Mag() * (hitPosition-prevNodePos);
            lastIntersectPoint = prevNodePos + ((deltaZ / (newDirection).CosTheta()) * newDirection);
        }
    }

    // These vectors are parallel to the bar sides towards the top vertex "3"
    // The convention here is vector_to_from so unitVector31 is a unit vector
    // going to vertex "3" from vertex "1"
    const TVector3 unitVector31 = 1./(barVertex3 - barVertex1).Mag() * (barVertex3 - barVertex1);
    const TVector3 unitVector32 = 1./(barVertex3 - barVertex2).Mag() * (barVertex3 - barVertex2);

    //Define two planes that the track exits towards by their normal vectors
    TVector3 normVector31 = orthogonalDir.Cross(unitVector31);
    TVector3 normVector32 = orthogonalDir.Cross(unitVector32);

    //The algebra assumes that the normal vectors have negative Z components
    if(normVector31.Z() > 0)
          normVector31 = -1 * normVector31;
    if(normVector32.Z() > 0)
          normVector32 = -1 * normVector32;

    // How long would the line need to be to reach the plane?
    // Since we do not know which side of the bar the track enters,
    // test both cases. The correct length is the one where
    // the angle between the plane normal vector and useDirection
    // vector is minimized
    const Double_t lengthInBar31 = (barVertex3 - lastIntersectPoint).Dot(normVector31) / (useDirection).Dot(normVector31);
    const Double_t lengthInBar32 = (barVertex3 - lastIntersectPoint).Dot(normVector32) / (useDirection).Dot(normVector32);

    // This is the entrace point into the bar
    const TVector3 moreUpstreamOfTrack = lastIntersectPoint + (kBarHeight / direction.CosTheta()) * useDirection;
    const Double_t distanceToBarVertex1 = (moreUpstreamOfTrack - barVertex1).Mag();
    const Double_t distanceToBarVertex2 = (moreUpstreamOfTrack - barVertex2).Mag();
    Double_t lengthInBar;

    trkBar->SetDistanceToClosestBaseVertex(std::min(distanceToBarVertex1, distanceToBarVertex2));
    if(!IsValidPathLengthInBar(lengthInBar31) || !IsValidPathLengthInBar(lengthInBar32))
    {
#ifdef DEBUG
        std::cout << "Some value is NaN or physically impossible" << std::endl;
        std::cout << "GetPathLengthExitBase" << std::endl;
        std::cout << "hitPostion: "; hitPosition.Print();
        std::cout << "barVertex1: "; barVertex1.Print();
        std::cout << "barVertex2: "; barVertex2.Print();
        std::cout << "barVertex3: "; barVertex3.Print();
        std::cout << "prevNodePos: "; prevNodePos.Print();
        std::cout << "curNodePos: "; curNodePos.Print();
        std::cout << "useDirection: "; useDirection.Print();
        std::cout << "lastIntersectPoint: "; lastIntersectPoint.Print();
        DEBUGPRINT(lengthInBar31)
        DEBUGPRINT(lengthInBar32)
        DEBUGPRINT(distanceToBarVertex1)
        DEBUGPRINT(distanceToBarVertex2)
        std::cout << "normVector31: "; normVector31.Print();
        std::cout << "normVector32: "; normVector32.Print();
#endif
        return;
    }
    else if(lengthInBar31 < kBarHeight && lengthInBar32 > kBarHeight)
    {
        lengthInBar = lengthInBar31;
        trkBar->SetDistanceToClosestBaseVertex(distanceToBarVertex1);
    }
    else if(lengthInBar32 < kBarHeight && lengthInBar31 > kBarHeight)
    {
        lengthInBar = lengthInBar32;
        trkBar->SetDistanceToClosestBaseVertex(distanceToBarVertex2);
    }
    // Both lengths seem to be reasonable, pick the length with the shortest
    // distance to the other verticies
    else if(distanceToBarVertex1 < distanceToBarVertex2)
    {
        lengthInBar = lengthInBar31;
    }
    else if(distanceToBarVertex2 < distanceToBarVertex1)
    {
        lengthInBar = lengthInBar32;
    }
    else
    {
        std::cout << "Something is very broken (BAD!), returning..." << std::endl;
        return;
    }

    Double_t overEstimatedLength = GetLengthInFiberHole(lastIntersectPoint, useDirection, lengthInBar, hitPosition, hit->IsXHit());
    std::cout << "overEstimatedLength = " << overEstimatedLength << std::endl;

    /*
    // This is not used yet!
    const Double_t lengthInDeadMaterial = GetLengthInDeadMaterial(lastIntersectPoint, useDirection, lengthInBar, hitPosition, hit->IsXHit());
#ifdef DEBUG
    DEBUGPRINT(lengthInDeadMaterial)
#endif
    overEstimatedLength += lengthInDeadMaterial;
    */

    // Subtract off whatever contribution the fiber hole might have given in this calculation
    const Double_t lengthInBar_AfterOverEstimateSubtraction = lengthInBar - overEstimatedLength;

    trkBar->SetCorrectedLength(lengthInBar_AfterOverEstimateSubtraction);
    trkBar->SetOverEstimatedLength(overEstimatedLength);
    trkBar->CalculateStartEndLength();
    return;
}

//****************************************************************************
void P0DBarEndLoopFunction::DumpHit(const ND::THandle<ND::TSingleHit>& hit) const
//****************************************************************************
{
    std::cout << "******************************************************************************" << std::endl;
    std::cout << "*============================== DUMP HIT ====================================*" << std::endl;
    std::cout << "******************************************************************************" << std::endl;
    DEBUGPRINT(hit->GetChannelId(0).AsUInt())
    DEBUGPRINT(hit->GetCharge())
    std::cout << "******************************************************************************" << std::endl;
}

//****************************************************************************
Double_t P0DBarEndLoopFunction::GetLengthInFiberHole(
        const TVector3& startPosition, const TVector3& direction,
        const Double_t& totalLength, const TVector3& hitPosition,
        const Bool_t& isXHit) const
//****************************************************************************
{
    Double_t summedLength = 0;
    UInt_t inFiberHoleCount = 0;
    TVector3 positionInBar = startPosition;
    //TVector3 norm(0,0,0);
    //isXHit ? norm.SetY(1) : norm.SetX(1);
    while(summedLength < totalLength)
    {
        //If norm is orthogonal direction w.r.t. working 2D plane, ().Dot(norm) is nearly zero
        //hence (() - ().Dot(norm) * norm).Mag() =approx= ().Mag()
        //and therefor we end calculating zero
        //const Double_t distanceBetweenHoleAndPointOnTrackInBar =
          //((positionInBar - hitPosition) - (positionInBar - hitPosition).Dot(norm) * norm).Mag();
        Double_t distanceBetweenHoleAndPointOnTrackInBar = 0;
        const TVector3 delta = positionInBar - hitPosition;
        if (isXHit)
            distanceBetweenHoleAndPointOnTrackInBar = TMath::Sqrt(delta.X()*delta.X()+delta.Z()*delta.Z());
        else
            distanceBetweenHoleAndPointOnTrackInBar = TMath::Sqrt(delta.Y()*delta.Y()+delta.Z()*delta.Z());
        if(distanceBetweenHoleAndPointOnTrackInBar < kFiberHoleRadius)
        {
            ++inFiberHoleCount;
        }

        summedLength += kStepSize;
        positionInBar += kStepSize * direction;
    }
    return inFiberHoleCount * kStepSize;
}

//****************************************************************************
Double_t P0DBarEndLoopFunction::GetLengthInDeadMaterial(
        const TVector3& startPosition, const TVector3& direction,
        const Double_t& totalLength, const TVector3& hitPosition,
        const Bool_t& isXHit) const
//****************************************************************************
{
    TVector3 positionInBar_prime = startPosition - hitPosition;
    //    TVector3 origin_prime = hitPosition;
    //TVector3 start_prime = startPosition;
    TVector3 direction_prime = direction;
    //PrintTVector3(origin_prime, "origin_prime");
    //PrintTVector3(start_prime, "start_prime");
    //PrintTVector3(direction_prime, "direction_prime");
    //origin_prime.RotateX(kRotation);
    //start_prime.RotateX(kRotation);
    positionInBar_prime.RotateX(kRotation);
    direction_prime.RotateX(kRotation);
    if(!isXHit)
    {
      //  origin_prime.RotateY(kRotation);
      //  start_prime.RotateY(kRotation);
        positionInBar_prime.RotateY(kRotation);
        direction_prime.RotateY(kRotation);
    }
    if (direction_prime.Y()<0) {
      positionInBar_prime.RotateZ(2*kRotation);
      direction_prime.RotateZ(2*kRotation);
    }
    //PrintTVector3(origin_prime, "origin_prime");
    //PrintTVector3(start_prime, "start_prime");
#ifdef DEBUG
    PrintTVector3(positionInBar_prime, "positionInBar_prime");
    PrintTVector3(direction_prime, "direction_prime");
#endif
    Double_t summedLength = 0;
    UInt_t zero_content_counts = 0;
    //    TVector3 positionInBar_prime = start_prime - origin_prime;
    while( summedLength < totalLength )
    {
#ifdef DEBUG
        PrintTVector3(positionInBar_prime, "positionInBar_prime");
#endif
        const Double_t content = p0dBarResponseMap->GetBinContent(p0dBarResponseMap->FindBin(positionInBar_prime.X(), positionInBar_prime.Y()));
        if(content <= 0)
        {
            ++zero_content_counts;
        } else if (positionInBar_prime.Perp() < kFiberHoleRadius ){
          // ++ zero_content_counts;
        }
        positionInBar_prime += kStepSize * direction_prime;
        summedLength += kStepSize;
    }

    return (kStepSize * zero_content_counts);
}

//****************************************************************************
Bool_t P0DBarEndLoopFunction::IsValidPathLengthInBar(const Double_t &lengthInBar) const
//****************************************************************************
{
    if(TMath::IsNaN(lengthInBar) || !TMath::Finite(lengthInBar))
        return kFALSE;
    if(lengthInBar < 0 || lengthInBar >= 2.0 * kBarHalfWidth)
        return kFALSE;
    return kTRUE;
}

//****************************************************************************
int main(int argc, char **argv)
//****************************************************************************
{
    if(!getenv("P0DRECONROOT"))
    {
        std::cout << "Unable to find P0DRECONROOT, please source!" << std::endl;
        std::exit(1);
    }
    TFile* _p0dBarResponseSummary = TFile::Open(TString::Format("%s/pdf/p0dBarResponseSummary.root", getenv("P0DRECONROOT")));
    TH2F* p0dBarResponseMap = static_cast<TH2F*>(_p0dBarResponseSummary->Get("p0dBarResponseMap"));
    if(!p0dBarResponseMap)
    {
        std::cout << "Unable to find p0dBarResponseMap in file " << _p0dBarResponseSummary->GetName() << std::endl;
        std::exit(1);
    }
    P0DBarEndLoopFunction generateP0DTFBOffsets(p0dBarResponseMap);
    nd280EventLoop(argc,argv,generateP0DTFBOffsets);
    _p0dBarResponseSummary->Close();
}
