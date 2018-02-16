//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Nov  4 11:44:16 2016 by ROOT version 5.34/34)
//      from the StreamerInfo in file /hep/T2K/DataDir/valid/oa_nt_beam_90410000-0000_h252u2jujozw_anal_000_prod005magnet201011waterc-bsdv01.root
//////////////////////////////////////////////////////////


#ifndef ND__TSubBaseShowerObject_h
#define ND__TSubBaseShowerObject_h
namespace ND {
class TSubBaseShowerObject;
} // end of namespace.

#include "ND__TSubBaseObject.h"
#include "TVector3.h"

namespace ND {
class TSubBaseShowerObject : public ND::TSubBaseObject {

public:
// Nested classes declaration.

public:
// Data Members.
   TVector3    Cone;        //

   TSubBaseShowerObject();
   TSubBaseShowerObject(const TSubBaseShowerObject & );
   virtual ~TSubBaseShowerObject();

   ClassDef(TSubBaseShowerObject,2); // Generated by MakeProject.
};
} // namespace
#endif
