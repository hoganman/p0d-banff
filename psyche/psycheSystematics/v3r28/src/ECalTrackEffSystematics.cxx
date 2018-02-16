#include "ECalTrackEffSystematics.hxx"
#include "CutUtils.hxx"
#include "SystematicUtils.hxx"
#include "SystId.hxx"
#include "EventBoxId.hxx"

//#define DEBUG


//********************************************************************
ECalTrackEffSystematics::ECalTrackEffSystematics(bool comp):BinnedParams("ECalTrackEff",k3D_EFF_ASSYMMETRIC){
  //********************************************************************

  _computecounters = comp;
  SetNParameters(2*GetNBins());
  if(_computecounters)
    InitializeEfficiencyCounter();
}

//********************************************************************
Weight_h ECalTrackEffSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& event, const ToyBoxB& box, const SelectionBase& sel){
  //********************************************************************


  if(_computecounters)
    InitializeEfficiencyCounter();

  (void)event;

  // Get the SystBox for this event, and the appropriate selection and branch
  SystBoxB* SystBox = GetSystBox(event, box.SelectionEnabledIndex, box.SuccessfulBranch);

  Weight_h eventWeight = 1.;

#ifdef DEBUG
  std::cout << " \n ECalTrackEffSystematics::Apply(): " << SystBox->nRelevantTrueObjects<< std::endl; 
#endif

  // Loop over relevant true tracks
  for (Int_t itrk = 0; itrk < SystBox->nRelevantTrueObjects; itrk++){

    AnaTrueParticleB* truePart = static_cast<AnaTrueParticleB*>(SystBox->RelevantTrueObjects[itrk]);
    
    if (!truePart) continue;
    
    // Do fine-tuning of the track relevance via the selection
    if (!sel.IsRelevantTrueObjectForSystematicInToy(event, box, truePart, SystId::kECalTrackEff, box.SuccessfulBranch)) continue;

    AnaTrackB* recoTrack = static_cast<AnaTrackB*>(SystBox->RelevantTrueObjectsReco[itrk]);

    SubDetId::SubDetEnum det[10];
    int ndet = anaUtils::GetECalDetCrossed(truePart, det);
    if (ndet != 1)  continue;

    BinnedParamsParams params;
    int index;
    
    int pdg = abs(truePart->PDG);
    
    // Assume a shower case if not a muon
    if (pdg != 13) pdg = 11;
   

    if (!GetBinValues(pdg, det[0], truePart->Momentum, params, index)) continue;

    // Check the presence of reco track
    // ONE HAS TO USE THE SYSTEMATIC WITH CARE:
    // apply the additional criteria
    // in order to match the efficiency definition used in TN-279
    // for showering/not showering objects in the TPC (basically electron/poistron
    // vs muon) the corresponding ECal object (if found) has to sutisfy additional
    // criterias: be shower/track-like based on the PIDMipEm variable
    // the energy cuts has to be applied as well + the most upstream layer cut
    // (the object is assumed to come from inside!)
    // use IsShower function with the hard coded parameters corresponding to
    // TN-279 values
    
    // Check whether there is a corresponding reco object
    bool found = (recoTrack) ? SubDetId::GetDetectorUsed(recoTrack->Detector, det[0]): false;
    
    // If the reco object is present then apply the PID criteria
    if (found){
      
      for (;;){
        
        // Check whether the true object is a shower
        bool isTrueShower(pdg != 13);
      
     
        // 1. Get the ECal component
        if (recoTrack->nECALSegments < 1){
          found = false;
          break;
        }

        if (!recoTrack->ECALSegments[0]){
          found = false;
          break;
        }

        // 2. Apply EMEnergy cut
        if (recoTrack->ECALSegments[0]->EMEnergy <= 30.){
          found = false;
          break;
        }
        
        // 3. Apply the PID
        bool isECalShower = recoTrack->ECALSegments[0]->PIDMipEm > 0;
        found = (isTrueShower) ? isECalShower : !isECalShower;  
        
        if (!found){
          break;
        }
        
        // 3. Apply MostUpstreamLayerHitCut
        if (recoTrack->ECALSegments[0]->MostUpStreamLayerHit > 6){
          found = false;
          break;
        }
        
        break;
      }
      
    }
    
    // Checked the correspondence,  now get the weight

    Weight_h eventWeight_tmp = 1.;
#if useNewWeights 
    eventWeight_tmp = systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(_index)->Variations[index], params); // New way including data-mc diff
#else
    eventWeight_tmp = systUtils::ComputeEffLikeWeight(found, toy.GetToyVariations(_index)->Variations[2*index],
        toy.GetToyVariations(_index)->Variations[2*index+1], params);
#endif

#ifdef DEBUG
    std::cout << "ecal track found " << found << " eventWeight local " << eventWeight_tmp << std::endl; 
#endif

    // Update the total weight
    eventWeight *= eventWeight_tmp;
  
    if(_computecounters)
      UpdateEfficiencyCounter(index,found);    

  }
  
#ifdef DEBUG
    std::cout << " eventWeight total " << eventWeight << std::endl; 
#endif

  return eventWeight;
}

//********************************************************************
bool ECalTrackEffSystematics::IsRelevantTrueObject(const AnaEventC& event, const AnaTrueObjectC& track) const{
  //********************************************************************

  (void)event;

  //should cross one ECal only and it should be tracker or Ds
  SubDetId::SubDetEnum det[20];
  int ndet = anaUtils::GetECalDetCrossed(static_cast<const AnaTrueParticleB*>(&track), det);
  if (ndet!=1)  return false;

  return (SubDetId::IsTECALDetector(det[0]) || det[0] == SubDetId::kDSECAL);
}

