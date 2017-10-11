#include "ECalPiZeroVetoPileUpSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "ToyBoxTracker.hxx"
#include "MultiPiBox.hxx"
#include "SystematicUtils.hxx"

//#define DEBUG

//********************************************************************
ECalPiZeroVetoPileUpSystematics::ECalPiZeroVetoPileUpSystematics():BinnedParams("ECalPiZeroVetoPileUp",k1D_EFF_SYMMETRIC){
//********************************************************************
   SetNParameters(GetNBins());
}

//********************************************************************
Weight_h ECalPiZeroVetoPileUpSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventBB, const ToyBoxB& boxB){
//********************************************************************

  const AnaEventB& event = *static_cast<const AnaEventB*>(&eventBB); 
  
  // Cast the ToyBox to the appropriate type
  const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB); 

  Weight_h eventWeight = 1;
  
  // Get the multi-pion box
  const multipi::MultiPiBox* pionBox = box.GetMultiPiBox();
  
  if (!pionBox)
    return eventWeight;

  BinnedParamsParams params;
  int index;

  if (!GetBinValues(anaUtils::GetRunPeriod(event.EventInfo.Run),params, index)) 
    return eventWeight;

  // Consider two cases
  // 1. ECal candidate is fake -- use "efficiency"
  // 2. No fake ECal candidate -- use "inefficiency"
 
  bool found = false;
  
  if (!pionBox->ECalPi0Photon)
    found = false;
  
  if (pionBox->ECalPi0Photon){
    // Get true vertex
    if (box.MainTrack){
      if (box.MainTrack->GetTrueParticle()){
        if (box.MainTrack->GetTrueParticle()->TrueVertex){
          // Get the true vertex position
          Float_t* pos = box.MainTrack->GetTrueParticle()->TrueVertex->Position;
          // Check that the truth vertex is in the FV
         if (anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(box.DetectorFV), pos)){
           AnaTrueParticleB* primary = anaUtils::GetTrueParticleFromTrueVertex(*box.MainTrack->GetTrueParticle()->TrueVertex, 
               pionBox->ECalPi0Photon->GetTrueParticle()->PrimaryID);
           found = (!primary && pionBox->ECalPi0Photon->GetTrueParticle()->PrimaryID != -999);
           
           // If everything is fine (correct ECal canidate from the FGD vertex), then do nothing
           if (primary)
             return eventWeight;
         }  
        }
      }
    }
  }
  

  eventWeight *= systUtils::ComputeEffLikeWeight(found, toy, GetIndex(), index, params);

#ifdef DEBUG
  std::cout << "Run: " << event.EventInfo.Run  
    << " index " << index << " variation " << toy.GetToyVariations(_index)->Variations[index] 
    << " weight " << eventWeight << std::endl;
#endif

  return eventWeight;

}

