//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Tue Nov  8 13:36:45 2016 by ROOT version 5.34/34)
//      from the StreamerInfo in file /hep/T2K/DataDir/valid/oa_nt_beam_90410000-0002_g6rrt6tsqxr2_anal_000_prod005magnet201011waterc-bsdv01.root
//////////////////////////////////////////////////////////


#ifndef ND__TDataVector_h
#define ND__TDataVector_h
namespace ND {
class TDataVector;
} // end of namespace.

#include "ND__TData.h"
#include "Riostream.h"
#include <vector>
#include "ND__TDatum.h"

namespace ND {
class TDataVector : public ND::TData {

public:
// Nested classes declaration.

public:
// Data Members.
   vector<ND::TDatum*> fVector;     //

   TDataVector();
   TDataVector(const TDataVector & );
   virtual ~TDataVector();

   ClassDef(TDataVector,5); // Generated by MakeProject.
};
} // namespace
#endif
