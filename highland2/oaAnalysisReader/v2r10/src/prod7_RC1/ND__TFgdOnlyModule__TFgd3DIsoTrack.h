//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Jan 20 12:54:43 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file test_ev_anal.root
//////////////////////////////////////////////////////////


#ifndef ND__TFgdOnlyModule__TFgd3DIsoTrack_h
#define ND__TFgdOnlyModule__TFgd3DIsoTrack_h
namespace ND {
namespace TFgdOnlyModule {
class TFgd3DIsoTrack;
} // end of namespace.
} // end of namespace.

#include "TObject.h"
#include "Riostream.h"
#include <string>
#include "TLorentzVector.h"
#include "TVector3.h"
#include <vector>

namespace ND {
namespace TFgdOnlyModule {
class TFgd3DIsoTrack : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   string      AlgorithmName;    //
   int         NHits;            //
   double      SumCharge;        //
   double      Range;            //
   TLorentzVector Origin;           //
   TLorentzVector OriginVariance;    //
   TVector3       Direction;         //
   double         muonPull;          //
   double         pionPull;          //
   double         protonPull;        //
   vector<TVector3*> XZHitPositions;    //
   vector<TVector3*> YZHitPositions;    //
   int               TrajId;            //
   double            Completeness;      //
   double            Cleanliness;       //

   TFgd3DIsoTrack();
   TFgd3DIsoTrack(const TFgd3DIsoTrack & );
   virtual ~TFgd3DIsoTrack();

   ClassDef(TFgd3DIsoTrack,3); // Generated by MakeProject.
};
} // namespace
} // namespace
#endif
