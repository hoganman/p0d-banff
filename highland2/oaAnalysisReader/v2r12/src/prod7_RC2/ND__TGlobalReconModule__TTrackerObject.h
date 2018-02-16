//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Sep 15 17:37:36 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file ../../../../../Files/Prod7/oa_nt_beam_80730000-0000_ngg4oj3wpuhl_anal_000_nu-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TGlobalReconModule__TTrackerObject_h
#define ND__TGlobalReconModule__TTrackerObject_h
namespace ND {
namespace TGlobalReconModule {
class TTrackerObject;
} // end of namespace.
} // end of namespace.

#include "ND__TSubBaseObject.h"

namespace ND {
namespace TGlobalReconModule {
class TTrackerObject : public ND::TSubBaseObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      Charge;      //
   int         matchingFailure_flag;    //

   TTrackerObject();
   TTrackerObject(const TTrackerObject & );
   virtual ~TTrackerObject();

   ClassDef(TTrackerObject,2); // Generated by MakeProject.
};
} // namespace
} // namespace
#endif
