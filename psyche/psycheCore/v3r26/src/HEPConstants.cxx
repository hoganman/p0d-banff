#include "HEPConstants.hxx"
#include "TDatabasePDG.h"

namespace units{

  TDatabasePDG* GetPDGBase(){
    return TDatabasePDG::Instance();  
  }
    
  // From ROOT TDatabasePDG
  // nuclons
  const double mass_proton       = 0.9382719 * units::GeV;
  const double mass_neutron      = 0.9395650 * units::GeV;
            
  // leptons
  const double mass_electron     = 0.000511 * units::GeV;
  const double mass_muon         = 0.105658 * units::GeV;
 
   // mesons
  const double mass_pion_zero    = 0.134977 * units::GeV;
  const double mass_pion_charged = 0.139570 * units::GeV;
  const double mass_kaon_zero    = 0.497614 * units::GeV;
  const double mass_kaon_charged = 0.493677 * units::GeV;

  // baryons
  const double mass_sigma_plus   = 1.189370 * units::GeV;
  const double mass_sigma_minus  = 1.197440 * units::GeV;
  const double mass_sigma_zero   = 1.192642 * units::GeV;

  
  
}
