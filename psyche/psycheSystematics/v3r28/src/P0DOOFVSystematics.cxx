#define P0DOOFVSystematics_cxx

#include "P0DOOFVSystematics.hxx"
#include "ND280AnalysisUtils.hxx"
#include "BasicUtils.hxx"
#include "SubDetId.hxx"
#include "FiducialVolumeDefinition.hxx"
#include "SystematicUtils.hxx"
#include "ToyBoxTracker.hxx"

//********************************************************************
P0DOOFVSystematics::P0DOOFVSystematics() : EventWeightBase(1)
//********************************************************************
{

  _p0d = new BinnedParams("P0DOOFV_reco", BinnedParams::k1D_SYMMETRIC);

  _rate = new BinnedParams("P0DOOFV_rate", BinnedParams::k2D_SYMMETRIC);

  SetNParameters(_p0d->GetNBins() + _rate->GetNBins());

  for (UInt_t i = 0; i < NMAXP0DOOFVSYSTEMATICSBINS; i++)
  {
    if (!_rate->GetBinValues(i, _rate_corr[i], _rate_error[i], _rate_index[i]))
      _rate_index[i] = -1;

    if (_rate_index[i] >= 0)
      _rate_index[i] += _p0d->GetNBins();
  }

}

//********************************************************************
void P0DOOFVSystematics::Initialize()
//********************************************************************
{
    systUtils::AddBinnedParamsOffsetToSystematic(*this, *_rate, _rate->GetNBins());
    systUtils::AddBinnedParamsOffsetToSystematic(*this, *_p0d, _p0d->GetNBins());
}

//********************************************************************
Int_t P0DOOFVSystematics::GetBeamNumber(const Int_t& runperiod, AnaTrackB* maInt_track) const
//********************************************************************
{
  // FHC
  if(!anaUtils::IsRHC(runperiod))
    return 0;
  // RHC
  else
  {
    // NUMU BKG
    if(maInt_track->Charge < 0)
      return 1;
    // ANTINUMU
    else
      return 2;
  }
}

//********************************************************************
Int_t P0DOOFVSystematics::GetDetNumber(const SubDetId::SubDetEnum& det) const {
//********************************************************************

  if(SubDetId::IsP0DDetector(det))
    return 0;
  else if(SubDetId::IsFGDDetector(det))
    return 1;
  else if(SubDetId::IsECALDetector(det))
    return 2;
  else if(SubDetId::IsTECALDetector(det))
    return 2;
  else if(SubDetId::IsPECALDetector(det))
    return 2;
  else if(SubDetId::IsSMRDDetector(det))
    return 3;
  else if(SubDetId::IsTPCDetector(det))
    return 4;
  else
    return 5;

}

//********************************************************************
Weight_h P0DOOFVSystematics::ComputeWeight(const ToyExperiment& toy, const AnaEventC& eventC, const ToyBoxB& boxB)
//********************************************************************
{
    // Cast the ToyBox to the appropriate type
    const ToyBoxTracker& box = *static_cast<const ToyBoxTracker*>(&boxB);

    Weight_h eventWeight=1;
    if (!box.MainTrack)
        return eventWeight; // HMN track should exist
    if (!box.MainTrack->GetTrueParticle())
        return eventWeight; // True track associated to HMN track should exist
    if (!box.MainTrack->GetTrueParticle()->TrueVertex)
        return eventWeight; // True vertex associated to HMN track should exist

    // Get the true vertex position
    const Float_t* const tvertex = box.MainTrack->GetTrueParticle()->TrueVertex->Position;

    // if the true vertex is inside the P0D FV this is not OOFV (RETURN EVENTWEIGHT=1)
    if(anaUtils::InFiducialVolume(static_cast<SubDetId::SubDetEnum>(box.DetectorFV), tvertex))
        return eventWeight;

    //const SubDetId::SubDetEnum detector = static_cast<const SubDetId::SubDetEnum>(anaUtils::GetDetector(tvertex));
    const SubDetId::SubDetEnum detector = anaUtils::GetDetector(tvertex);

    Int_t categ =-1;
    if(SubDetId::IsP0DDetector(detector))
    {
        //In P0D CEcal.
        if(tvertex[2] > DetDef::p0dmax[2] - FVDef::FVdefmaxP0D[2])
            categ = 0;
        //In P0D US ECal
        else if(tvertex[2] < DetDef::p0dmin[2] + FVDef::FVdefminP0D[2])
            categ = 1;
        //In P0D WT
        else
            categ = 2;
    }
    else if (SubDetId::IsFGDDetector(detector))
      categ = 3;
    else if (SubDetId::IsECALDetector(detector))
      categ = 4;
    else if (SubDetId::IsPECALDetector(detector))
      categ = 4;
    else if (SubDetId::IsTECALDetector(detector))
      categ = 4;
    else if (SubDetId::IsSMRDDetector(detector))
      categ = 5;
    else if (SubDetId::IsTPCDetector(detector))
      categ = 6;

    if(categ >= 0)
    {

        const AnaEventB& event = *static_cast<const AnaEventB*>(&eventC);
        if (!_p0d->GetBinValues(GetBeamNumber(anaUtils::GetRunPeriod(event.EventInfo.Run), box.MainTrack),
                                GetDetNumber(detector),
                                _reco_corr, _reco_error, _reco_index)
           )
        {
            _reco_index=-1;
        }

        // eval the other detector rate OOFV contribution
        if (_rate_index[categ] >= 0)
        {
          eventWeight.Systematic *= 1 + _rate_corr[categ] + (_rate_error[categ] * toy.GetToyVariations(_index)->Variations[_rate_index[categ]]);
          eventWeight.Correction *= 1 + _rate_corr[categ];
        }

        // eval the P0D reco OOFV contribution
        if (_reco_index >= 0)
        {
          eventWeight.Systematic *= 1 + _reco_corr + _reco_error * (toy.GetToyVariations(_index)->Variations[_reco_index]);
          eventWeight.Correction *= 1 + _reco_corr;
        }
    }

    if (eventWeight.Systematic < 0)
        eventWeight.Systematic = 0;

    return eventWeight;
}
