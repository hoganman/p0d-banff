//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Sep 15 17:37:36 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file ../../../../../Files/Prod7/oa_nt_beam_80730000-0000_ngg4oj3wpuhl_anal_000_nu-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TGlobalReconModule__TFgdTimeBin_h
#define ND__TGlobalReconModule__TFgdTimeBin_h
namespace ND {
namespace TGlobalReconModule {
class TFgdTimeBin;
} // end of namespace.
} // end of namespace.

#include "TObject.h"
#include "TVector3.h"
#include "ND__TGlobalReconModule__TOutermostHits.h"
#include "Riostream.h"
#include <vector>
#include "TClonesArray.h"
#include "ND__TGlobalReconModule__TGlobalHit.h"

namespace ND {
namespace TGlobalReconModule {
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
   vector<int>                            FGD1HitLayer;             //
   vector<float>                          FGD1HitPosition;          //
   vector<float>                          FGD1HitQ;                 //
   vector<float>                          FGD1HitT;                 //
   vector<int>                            FGD1HitTrajID;            //
   vector<int>                            FGD2HitLayer;             //
   vector<float>                          FGD2HitPosition;          //
   vector<float>                          FGD2HitQ;                 //
   vector<float>                          FGD2HitT;                 //
   vector<int>                            FGD2HitTrajID;            //
   Int_t                                  NFGD1Unused;              //
   TClonesArray*                          FGD1Unused;               //
   Int_t                                  NFGD2Unused;              //
   TClonesArray*                          FGD2Unused;               //
   int                                    g4ID;                     //
   ND::TGlobalReconModule::TGlobalHit     hackFGDUnused;            //

   TFgdTimeBin();
   TFgdTimeBin(const TFgdTimeBin & );
   virtual ~TFgdTimeBin();

   ClassDef(TFgdTimeBin,2); // Generated by MakeProject.
};
} // namespace
} // namespace
#endif
