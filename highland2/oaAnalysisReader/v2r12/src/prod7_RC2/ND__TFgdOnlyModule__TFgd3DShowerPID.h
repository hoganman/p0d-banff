//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Sep 15 17:37:36 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file ../../../../../Files/Prod7/oa_nt_beam_80730000-0000_ngg4oj3wpuhl_anal_000_nu-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TFgdOnlyModule__TFgd3DShowerPID_h
#define ND__TFgdOnlyModule__TFgd3DShowerPID_h
namespace ND {
namespace TFgdOnlyModule {
class TFgd3DShowerPID;
} // end of namespace.
} // end of namespace.

#include "TObject.h"
#include "ND__TFgdOnlyModule__TFgd3DShowerHyp.h"
#include "TVector3.h"
#include "Riostream.h"
#include <map>

namespace ND {
namespace TFgdOnlyModule {
class TFgd3DShowerPID : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   ND::TFgdOnlyModule::TFgd3DShowerHyp Hyp1;        ///< Information assuming the particle is forward-going.
   ND::TFgdOnlyModule::TFgd3DShowerHyp Hyp2;        ///< Information assuming the particle is backward-going.
   TVector3                            PCAEigenValues;    ///< The three PCA eigenvalues.
   int                                 NumHits;           ///< The number of hits contributing to this cluster.
   double                              MatchingLikelihood3D;    //MVA likelihood value when matching 2D
   double                              Circularity;             //Dan Scully's Circularity variable, adapted for FGDs.
   std::map<int,int>                   Trajectories;            ///< Map of true trajectory IDs to the
   int                                 NDOF;                    ///< The number of degrees of freedom in the reconstruction.
   double                              Quality;                 ///< The goodness of fit for the reconstruction.
   int                                 Status;                  ///< The fit status.

   TFgd3DShowerPID();
   TFgd3DShowerPID(const TFgd3DShowerPID & );
   virtual ~TFgd3DShowerPID();

   ClassDef(TFgd3DShowerPID,2); // Generated by MakeProject.
};
} // namespace
} // namespace
#endif
