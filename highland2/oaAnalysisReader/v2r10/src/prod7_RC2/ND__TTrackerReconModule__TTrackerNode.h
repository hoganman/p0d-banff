//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Sep 15 17:37:36 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file ../../../../../Files/Prod7/oa_nt_beam_80730000-0000_ngg4oj3wpuhl_anal_000_nu-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TTrackerReconModule__TTrackerNode_h
#define ND__TTrackerReconModule__TTrackerNode_h
namespace ND {
namespace TTrackerReconModule {
class TTrackerNode;
} // end of namespace.
} // end of namespace.

#include "TObject.h"
#include "TLorentzVector.h"
#include "TVector3.h"

namespace ND {
namespace TTrackerReconModule {
class TTrackerNode : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      Charge;      ///< The Charge (+-1)
   double      EDeposit;    ///< The Energy Deposit (number of pe's)
   TLorentzVector Position;    ///< Position 4-vector (at node) x,y,z,t in mm, ns
   TLorentzVector Variance;    ///< Position variance 4-vector (at node)
   TVector3       Direction;    ///< Direction vector (at node)
   TVector3       DirectionVariance;    ///< Direction variance vector (at node)
   double         Momentum;             ///< Track Momentum (at node) in MeV/c
   double         MomentumError;        ///< Track Momentum uncertainty (at node) in MeV/c

   TTrackerNode();
   TTrackerNode(const TTrackerNode & );
   virtual ~TTrackerNode();

   ClassDef(TTrackerNode,2); // Generated by MakeProject.
};
} // namespace
} // namespace
#endif
