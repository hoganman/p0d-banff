//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Tue Jun 27 21:29:21 2017 by ROOT version 5.34/09)
//      from the StreamerInfo in file /physics/data/MC_Productions/GRID/mcp6_Spin_B/neut/run4-water/anal/oa_nt_beam_90410000-0023_wjojmfzpy6p4_anal_001_prod6amagnet201011waterc-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TAlignmentId_h
#define ND__TAlignmentId_h
namespace ND {
class TAlignmentId;
} // end of namespace.

#include "ND__TSHAHashValue.h"
#include "Riostream.h"
#include <string>

namespace ND {
class TAlignmentId : public ND::TSHAHashValue {

public:
// Nested classes declaration.

public:
// Data Members.
   string      fDocString;    //

   TAlignmentId();
   TAlignmentId(const TAlignmentId & );
   virtual ~TAlignmentId();

};
} // namespace
#endif