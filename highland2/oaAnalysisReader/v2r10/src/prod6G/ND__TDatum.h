//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Thu Nov 17 16:54:42 2016 by ROOT version 5.34/34)
//      from the StreamerInfo in file /hep/T2K/DataDir/PROD6/oa_nd_spl_00011000-0000_4stg6pqnornm_anal_001_v11r31p5-wg-bugaboo-bsdv01_2_prod6G.root
//////////////////////////////////////////////////////////


#ifndef ND__TDatum_h
#define ND__TDatum_h
namespace ND {
class TDatum;
} // end of namespace.

#include "TNamed.h"
#include "ND__TDatum.h"

namespace ND {
class TDatum : public TNamed {

public:
// Nested classes declaration.

public:
// Data Members.
   ND::TDatum* fParent;     //

   TDatum();
   TDatum(const TDatum & );
   virtual ~TDatum();

   ClassDef(TDatum,3); // Generated by MakeProject.
};
} // namespace
#endif
