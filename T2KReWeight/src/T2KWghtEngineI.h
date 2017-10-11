//____________________________________________________________________________
/*!

\class    t2krew::T2KWghtEngineI

\brief    Event reweighting engine ABC. Adapted from previous version T2KReWeight. 
          This class defines the interface required when adding a wrapper 
          to an external reweighting library. By requiring all external 
          weighting libraries to be wrapped in concrete implementations of 
          this ABC they can be treated in a generic way by T2KReWeight making 
          it possible to provide the user with a single interface to many 
          different reweighting libraries.
          
\author   Jim Dobson <j.dobson07 \at imperial.ac.uk>
          Imperial College London

          Costas Andreopoulos <costas.andreopoulos \at stfc.ac.uk> 
          STFC, Rutherford Appleton Laboratory

\created  Nov 1, 2010

 @ June 5, 2011 - JD
   Renamed class from T2KReWeightI to more appropriate T2KWghtEngineI. 

 @ June 5, 2011 - JD
   Added methods to vary systematic parameters in terms of absolute fractional
   errors and also to override the generator defaults for the fractional 
   errors. 

*/
//____________________________________________________________________________

#ifndef _T2KRW_REWEIGHT_ABC_H_
#define _T2KRW_REWEIGHT_ABC_H_

#include "TTree.h"

#include "T2KSyst.h"
#include "T2KSystSet.h"

#ifdef __T2KRW_OAANALYSIS_ENABLED__
#include "ND__JNuBeamFlux.h"
#endif

#ifdef __T2KRW_NEUT_ENABLED__
#include "neutvect.h"
#endif

#include "SK__h1.h"
#include "JnuBeamRead.h"

namespace t2krew {

 class T2KWghtEngineI 
 {
 public:
  virtual ~T2KWghtEngineI() { } 

  //
  // Define the T2KWghtEngineI interface
  //

  // Update the current tweaking dial value for the specified nuisance param.
  // Should result in tweaked value for syst:
  // tweaked = nominal*(1.0 + fractional_error*val)
  // where fractional_error is defined by generator. 
  virtual void SetSystTwkDial (T2KSyst_t syst, double val) = 0; 

  // Override default +/- 1 sigma fractional_error defined by generator.
  // Setting absolute to true should be equivalent to setting fractional_error
  // to 1.0 whilst also telling reweighting engine that fractional_error is not 
  // meaningful when calculating chisq etc... Tweaked value is then:
  // tweaked = nominal*(1.0 + val)
  virtual void SetSystFracErr (T2KSyst_t syst, double val, bool absolute = false) = 0;

  // Set all nuisance parameters to default values
  virtual void Reset (void) = 0;            

  // Propagate updated nuisance parameter values to actual MC, etc
  virtual void Reconfigure (void) = 0;            
  
  // Calculate a weight for the input event using the current nuisance param values
#ifdef __T2KRW_OAANALYSIS_ENABLED__
  virtual double CalcWeight (const ND::RooTrackerVtxBase * event) = 0;  
#endif
  virtual double CalcWeight (const SK::SK__h1 * sktree) = 0;  

  virtual double CalcWeight (const JnuBeamRead * jnutree) = 0;

  virtual double CalcWeight (TTree * intree, int entry) = 0;  

#ifdef __T2KRW_NEUT_ENABLED__
  virtual double CalcWeight (NeutVect *nvect) = 0;
#endif

  // calculate penalty factors
  virtual double CalcChisq (void) = 0;        

  // allow each calculator to define its own print output 
  virtual void Print (void) = 0;

 protected:
   T2KWghtEngineI() { }
 };
}

#endif

