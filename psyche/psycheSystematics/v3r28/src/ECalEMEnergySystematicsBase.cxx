#include "ECalEMEnergySystematicsBase.hxx"
#include "EventBoxTracker.hxx"
#include "SubDetId.hxx"

//********************************************************************
ECalEMEnergySystematicsBase::ECalEMEnergySystematicsBase(UInt_t npar, const std::string& name, BinnedParams::TypeEnum type):EventVariationBase(npar),BinnedParams(name, type){
  //********************************************************************

  SetNParameters(GetNBins());
}

//********************************************************************
bool ECalEMEnergySystematicsBase::UndoSystematic(AnaEventC& event){
  //********************************************************************

  //return false;
  // Get the SystBox for this event
  SystBoxB* box = GetSystBox(event);
  if(!box) return false;
  if(box->nRelevantRecObjects <= 0) return false;
  if(!box->RelevantRecObjects) return false;
  if(sizeof(box->RelevantRecObjects)/sizeof(AnaRecObjectC*) != box->nRelevantRecObjects) return false;

  // Get the relevant tracks for this systematic
  AnaRecObjectC** tracks = box->RelevantRecObjects;

  for (Int_t itrk=0; itrk<box->nRelevantRecObjects; itrk++){

    // For each track get ecal segment
    if(!tracks[itrk]) continue;
    if(!IsRelevantRecObject(event, *tracks[itrk])) continue;
    AnaRecObjectC* reco_trkC = tracks[itrk];

    AnaTrackB* trk = static_cast<AnaTrackB*>(reco_trkC);

    for (Int_t iecal=0; iecal<trk->nECALSegments; iecal++){

      AnaECALParticleB* ecal = trk->ECALSegments[iecal];

      //make sure we have an ecal track

      //make sure that the ecal track's original particle is also in the ecal
      const AnaParticleB* original = ecal->Original;
      if(!original) continue;
      if(!IsRelevantRecObject(event, static_cast<const AnaRecObjectC&>(*original)))
        continue;

      const AnaECALParticleB* original_ecal = static_cast<const AnaECALParticleB*>(ecal->Original);

      // Go back to the corrected EMEnergy
      Float_t orig_energy = original_ecal->EMEnergy;
      ecal->EMEnergy = orig_energy;
    }
  }
  // Don't reset the spill to corrected
  return false;
}

//********************************************************************
bool ECalEMEnergySystematicsBase::IsRelevantRecObject(const AnaEventC& event, const AnaRecObjectC& recObj) const{
//********************************************************************
  (void)(event);

  //track should use Tracker or DsECal
  if (!SubDetId::GetDetectorUsed(recObj.Detector, SubDetId::kTECAL) &&
      !SubDetId::GetDetectorUsed(recObj.Detector, SubDetId::kDSECAL))
    return false;

  const AnaTrackB& track_ref = static_cast<const AnaTrackB&>(recObj);
  if(sizeof(track_ref.ECALSegments)/sizeof(AnaECALParticleB*) != 1)
      return false;

  // track should have one ECal segment
  if (track_ref.nECALSegments!=1) return false;

  if (!track_ref.ECALSegments[0]) return false;

  return true;
}

