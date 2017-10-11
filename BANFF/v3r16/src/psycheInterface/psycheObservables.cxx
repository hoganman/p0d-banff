#include "psycheObservables.hxx"
#include "psycheBANFFEvent.hxx"

psycheLeptonCandidateMomentum::psycheLeptonCandidateMomentum(){


    name = "p";


}


float psycheLeptonCandidateMomentum::GetCurrentValue(BANFFEventBase* event){

    psycheBANFFEvent* pEvent = static_cast<psycheBANFFEvent*>(event);
    AnaEventSummaryB* summary = static_cast<AnaEventSummaryB*>(pEvent->detEvent->Event->Summary);
    AnaParticleMomB* leptonCandidate = static_cast<AnaParticleMomB*>(summary->LeptonCandidate[summary->EventSample]);
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
