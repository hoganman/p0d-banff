//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Sep 15 17:37:36 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file ../../../../../Files/Prod7/oa_nt_beam_80730000-0000_ngg4oj3wpuhl_anal_000_nu-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TTrackerReconModule__TTrackerConstituent_h
#define ND__TTrackerReconModule__TTrackerConstituent_h
namespace ND {
namespace TTrackerReconModule {
class TTrackerConstituent;
} // end of namespace.
} // end of namespace.

#include "TObject.h"
#include "Riostream.h"
#include <string>
#include "TLorentzVector.h"
#include "TVector3.h"

namespace ND {
namespace TTrackerReconModule {
class TTrackerConstituent : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   string      AlgorithmName;    ///< algorithm that created this object.
   int         Detectors;        ///< Detectors used
   unsigned long Status;           ///< The status for the fit.
   double        Quality;          ///< The quality of the fit.(probability)
   int           NDOF;             ///< The number of degrees of freedom.
   double        Chi2;             ///< The chi2 of the fit.
   int           NNodes;           ///< The number of nodes
   int           NHits;            ///< The number of hits.
   Int_t         NConstituents;    ///< The number of constituents this constituent is
   Int_t         ConstitIdx[2];    ///< Index into Constituents in
   bool          isForward;        ///< Sense of track
   double        Charge;           ///< The Charge of this constituent (+-1)
   double        EDeposit;         ///< The deposited charge for the constituent object
   TLorentzVector FrontPosition;    ///< The 4-vector position at the front of
   TLorentzVector BackPosition;     ///< The 4-vector position of the back of the
   TLorentzVector FrontVariance;    ///< The 4-vector position variance at the
   TLorentzVector BackVariance;     ///< The 4-vector position variance at the
   TVector3       FrontDirection;    ///< The direction vector at the front of the track
   TVector3       BackDirection;     ///< The direction vector at the back of the track
   double         FrontMomentum;     ///< the momentum at the front of the track in MeV/c
   double         BackMomentum;      ///< the momentum at the back of the track iin MeV/c
   TLorentzVector Position;          ///< position 4-vector (x,y,z,t) in mm, ns
   TLorentzVector Variance;          ///< position variance 4-vector
   TVector3       Direction;         ///< direction vector
   TVector3       DirectionVariance;    ///< direction variance vector
   double         Momentum;             ///< momentum MeV/c
   double         MomentumError;        ///< uncertainty in momentum MeV/c

   TTrackerConstituent();
   TTrackerConstituent(const TTrackerConstituent & );
   virtual ~TTrackerConstituent();

   ClassDef(TTrackerConstituent,2); // Generated by MakeProject.
};
} // namespace
} // namespace
#endif
