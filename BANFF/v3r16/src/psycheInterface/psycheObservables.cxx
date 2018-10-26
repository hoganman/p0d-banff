#include "psycheObservables.hxx"
#include "psycheBANFFEvent.hxx"

psycheLeptonCandidateMomentum::psycheLeptonCandidateMomentum(){


  name = "p";
  ApplyCoulombShift = false;
  ShiftMuCarbon  = 0.;
  ShiftAMuCarbon = 0.;
  ShiftMuOxygen  = 0.;
  ShiftAMuOxygen = 0.;

}


float psycheLeptonCandidateMomentum::GetCurrentValue(BANFFEventBase* event){

    psycheBANFFEvent* pEvent = static_cast<psycheBANFFEvent*>(event);
    AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(pEvent->detEvent->Event->Summary);
    // Get the selection this event falls into
    SampleId::SampleEnum index = summary->EventSample;
    AnaParticleMomB* leptonCandidate = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[index]);
    if(ApplyCoulombShift){
      
      if(!leptonCandidate->GetTrueParticle()) return leptonCandidate->Momentum;

      bool isCarbon = (event->GetTargetMaterial() == 12);
      bool isOxygen = (event->GetTargetMaterial() == 16);
      //bool isCC = TMath::Abs(event->GetReactionCode()) < 31;
      int  LepPDG  = leptonCandidate->GetTrueParticle()->PDG;
      /// only for cc
      //if(!isCC) return leptonCandidate->Momentum;
      /// chuck out all the non lepton selection
      //if(TMath::Abs(pdg) != 11 && TMath::Abs(pdg) != 13 && TMath::Abs(pdg) != 15) return leptonCandidate->Momentum;

      // Get the neutrino PDG
      AnaTrueVertexB *vtx = summary->TrueVertex[index];
      int NuPDG = vtx->NuPDG;

      // Check the lepton candidate matches the neutrino PDG
      // If anti-neutrino interaction, check that NuPDG 
      if (NuPDG < 0) {
	if (NuPDG != LepPDG-1) return leptonCandidate->Momentum;
      }
      else {
	if (NuPDG != LepPDG+1) return leptonCandidate->Momentum;
      }

      if(LepPDG > 0){
        //
        if     (isCarbon) return leptonCandidate->Momentum + ShiftMuCarbon;
        else if(isOxygen) return leptonCandidate->Momentum + ShiftMuOxygen;
      }
      else{
        if     (isCarbon) return leptonCandidate->Momentum + ShiftAMuCarbon;
        else if(isOxygen) return leptonCandidate->Momentum + ShiftAMuOxygen;
      }
    }
    return leptonCandidate->Momentum;
}


psycheLeptonCandidateCosTheta::psycheLeptonCandidateCosTheta(){


    name = "th";


}


float psycheLeptonCandidateCosTheta::GetCurrentValue(BANFFEventBase* event){

    psycheBANFFEvent* pEvent = static_cast<psycheBANFFEvent*>(event);
    AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(pEvent->detEvent->Event->Summary);
    AnaParticleMomB* leptonCandidate = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);
    return leptonCandidate->DirectionStart[2];
}
