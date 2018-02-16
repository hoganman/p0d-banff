//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Fri Jan 20 12:54:43 2017 by ROOT version 5.34/34)
//      from the StreamerInfo in file test_ev_anal.root
//////////////////////////////////////////////////////////


#ifndef ND__TP0DECALReconModule__TP0DECALLayerCharge_h
#define ND__TP0DECALReconModule__TP0DECALLayerCharge_h
namespace ND {
namespace TP0DECALReconModule {
class TP0DECALLayerCharge;
} // end of namespace.
} // end of namespace.

#include "TObject.h"
#include <TString.h>

namespace ND {
namespace TP0DECALReconModule {
class TP0DECALLayerCharge : public TObject {

public:
// Nested classes declaration.

public:
// Data Members.
   double      FirstTime;    ///< The earliest hit time of the relevant cluster.
   double      LastTime;     ///< The latest hit time of the relevant cluster.
   double      Charge[6];    //
   TString     module_name;    //

   TP0DECALLayerCharge();
   TP0DECALLayerCharge(const TP0DECALLayerCharge & );
   virtual ~TP0DECALLayerCharge();

   ClassDef(TP0DECALLayerCharge,2); // Generated by MakeProject.
};
} // namespace
} // namespace
#endif
