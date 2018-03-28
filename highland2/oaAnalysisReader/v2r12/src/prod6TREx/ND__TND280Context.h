//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Wed Nov  9 15:39:20 2016 by ROOT version 5.34/09)
//      from the StreamerInfo in file /Users/izmaylov/Downloads/oa_nt_beam_90210000-0000_2e7lhce5kvor_anal_000_prod6amagnet201011waterb-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__TND280Context_h
#define ND__TND280Context_h
namespace ND {
class TND280Context;
} // end of namespace.

#include "TNamed.h"

namespace ND {
class TND280Context {

public:
// Nested classes forward declaration.
enum Time { kDefault_Time };

public:
// Nested classes declaration.

public:
// Data Members.
   int         fPartition;    //
   int         fRun;          //
   int         fSubRun;       //
   int         fEvent;        //
   int         fSpill;        //
   UInt_t      fTimeStamp;    //

   TND280Context();
   TND280Context(const TND280Context & );
   virtual ~TND280Context();

};
} // namespace
#endif