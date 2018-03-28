//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Jan 20 12:54:43 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file test_ev_anal.root
//////////////////////////////////////////////////////////


#ifndef ND__TP0DReconModule__TP0DAlgoRes_h
#define ND__TP0DReconModule__TP0DAlgoRes_h
namespace ND {
namespace TP0DReconModule {
class TP0DAlgoRes;
} // end of namespace.
} // end of namespace.

#include "TObject.h"
#include "Riostream.h"
#include <string>
#include <vector>

namespace ND {
namespace TP0DReconModule {
class TP0DAlgoRes : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   string      AlgorithmName;    //
   short       Cycle;            ///< The cycle number, based on the first hit.
   vector<short> Vertices;         //
   vector<short> Particles;        //
   vector<short> Tracks;           //
   vector<short> Showers;          //
   vector<short> Clusters;         //
   vector<short> Nodes;            //
   vector<short> Hits;             //
   short         NHits;            ///< A count of the Hits associated with this object.
   UInt_t        UniqueID;         //
   string        FullName;         //
   vector<short> AlgoResults;      //
   short         Parent;           //
   short         UsedHitCluster;    //
   short         UnusedHitCluster;    //

   TP0DAlgoRes();
   TP0DAlgoRes(const TP0DAlgoRes & );
   virtual ~TP0DAlgoRes();

   ClassDef(TP0DAlgoRes,6); // Generated by MakeProject.
};
} // namespace
} // namespace
#endif