#include "FGDPIDVariation.hxx"

//#define DEBUG

//**************************************************
void FGDPIDVariation::ApplyVariation(AnaTrackB* track, const ToyExperiment& exp){
  //**************************************************


#ifdef DEBUG
  std::cout << " FGDPIDVariation::ApplyVariation(): start " << std::endl;  
#endif

  // Check track validity
  if (!track) return;

  // Do not do anything if a track does not use an FGD
  if (track->nFGDSegments == 0) return;
  
  // Retrieve the PDG
  if (!track->TrueObject) return;
  Int_t pdg;
  
  if (abs(track->GetTrueParticle()->PDG) == 211 || abs(track->GetTrueParticle()->PDG) == 13)
    pdg = 13;
  else if (abs(track->GetTrueParticle()->PDG) == 2212)
    pdg = 2212;
  else
    return;

  // Loop over FGD segments
  for (int k = 0; k < track->nFGDSegments; k++) {

    if (!track->FGDSegments[k]) continue;

    // The segment to be modified
    AnaFGDParticleB* fgdTrack = track->FGDSegments[k];

#ifdef DEBUG
    std::cout << " FGDPIDVariation::ApplyVariation(): fgdSegment " << std::endl; 
    fgdTrack->Print();
#endif

    // The un-corrected TPC track  
    const AnaFGDParticleB* original = static_cast<const AnaFGDParticleB*>(fgdTrack->Original);
    
    if (!original) continue; 

    Float_t pull_mean, mean_var, sigma_var;

    if (!GetVariation(*track, pull_mean, mean_var, sigma_var, exp)) continue; // for the moment depends on true info of the global track, not the TPC segment

#ifdef DEBUG
    std::cout << " FGDPIDVariation::ApplyVariation(): variation params: " << std::endl; 
    std::cout << " pull_mean " << pull_mean << std::endl;
    std::cout << " mean_var "  << mean_var  << std::endl;
    std::cout << " sigma_var " << sigma_var << std::endl;
#endif
     
    // Apply the actual variation#ifdef DEBUG

#ifdef DEBUG
    // Get the pulls prior to variation
    Float_t pullpi0 = fgdTrack->Pullpi;
    Float_t pullmu0 = fgdTrack->Pullmu;
    Float_t pullp0  = fgdTrack->Pullp;
#endif
      
    // Depends on the PDG
     if (pdg == 13){
        // Vary based on mean
        fgdTrack->Pullpi =  fgdTrack->Pullpi + mean_var; 
        fgdTrack->Pullmu =  fgdTrack->Pullmu + mean_var; 

        // Vary based on sigma ratio
        fgdTrack->Pullpi =  pull_mean + (fgdTrack->Pullpi - pull_mean) * sigma_var; 
        fgdTrack->Pullmu =  pull_mean + (fgdTrack->Pullpi - pull_mean) * sigma_var; 
      }
      else if (pdg == 2212){
       
        // Vary based on mean
        fgdTrack->Pullp =  fgdTrack->Pullp + mean_var;
        
        // Vary based on sigma ratio
        fgdTrack->Pullp =  pull_mean + (fgdTrack->Pullp - pull_mean) * sigma_var; 
      }

#ifdef DEBUG

    std::cout << " FGDPIDVariation::ApplyVariation(): Results: " << std::endl;  
    std::cout << " PDG:\t " << track->GetTrueParticle()->PDG << std::endl;  
    std::cout << pullpi0 << " -->\t " <<  fgdTrack->Pullpi  << std::endl; 
    std::cout << pullmu0 << " -->\t " <<  fgdTrack->Pullmu  << std::endl; 
    std::cout << pullp0  << " -->\t " <<  fgdTrack->Pullp   << std::endl;
#endif
  
  }

#ifdef DEBUG
  std::cout << " FGDPIDVariation::ApplyVariation(): end \n " << std::endl;  
#endif

}



