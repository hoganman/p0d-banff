//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Sep 15 17:37:36 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file ../../../../../Files/Prod7/oa_nt_beam_80730000-0000_ngg4oj3wpuhl_anal_000_nu-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TGlobalReconModule__TGlobalHit_h
#define ND__TGlobalReconModule__TGlobalHit_h
namespace ND {
namespace TGlobalReconModule {
class TGlobalHit;
} // end of namespace.
} // end of namespace.

#include "TObject.h"
#include "TVector3.h"

namespace ND {
namespace TGlobalReconModule {
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
} // namespace
} // namespace
#endif
