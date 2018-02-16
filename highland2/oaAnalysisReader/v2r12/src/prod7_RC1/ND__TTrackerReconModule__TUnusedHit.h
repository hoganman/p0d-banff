//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Jan 20 12:54:43 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file test_ev_anal.root
//////////////////////////////////////////////////////////


#ifndef ND__TTrackerReconModule__TUnusedHit_h
#define ND__TTrackerReconModule__TUnusedHit_h
namespace ND {
namespace TTrackerReconModule {
class TUnusedHit;
} // end of namespace.
} // end of namespace.

#include "TObject.h"
#include "TVector3.h"

namespace ND {
namespace TTrackerReconModule {
class TUnusedHit : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      TotalCharge;    ///< Deposited charge (the hit EDeposit)
   TVector3    Position;       ///< The position of the hit component 0=x 1=y 2=z in mm
   TVector3    Variance;       ///< The position variance in mm
   double      Time;           ///< Time of the hit in ns
   double      TimeUnc;        ///< Time Uncertainty of hit in ns

   TUnusedHit();
   TUnusedHit(const TUnusedHit & );
   virtual ~TUnusedHit();

   ClassDef(TUnusedHit,2); // Generated by MakeProject.
};
} // namespace
} // namespace
#endif
