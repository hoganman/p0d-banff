#ifndef P0DDATACLASSES_HXX
#define P0DDATACLASSES_HXX

#include "DataClasses.hxx"

class AnaP0DTrackerTrack : public AnaTrack{
  public:
    AnaP0DTrackerTrack();
    virtual ~AnaP0DTrackerTrack(){}
    
      /// Clone this object.
    virtual AnaP0DTrackerTrack* Clone() {
        return new AnaP0DTrackerTrack(*this);
    }
      
    /// Dump the object to screen.
    virtual void Print() const;
      
    float TrackerMomentum;
    float TrackerDirection[3];
    float TrackerPosition[4];

  protected:
    AnaP0DTrackerTrack(const AnaP0DTrackerTrack& track);

};

class EventBoxP0D : public EventBoxB{
public:
  // Just call the base class versions for now
  EventBoxP0D();
  ~EventBoxP0D(){}

  enum GroupEnum{
    kUnassigned=0,
    kTracksWithTPCinP0DFV,
    kTracksWithGoodQualityTPCinP0DFV,
    kTracksWithP0D,
    kTracksWithP0DinFV,
    kTracksWithTPCorP0D,
    kTracksWithTPCorP0DinFV,
    kTracksWithP0DandNoTPC,
    kP0DPIDTracks,
    kTrueTracksChargedInP0DInBunch,
    kTrueTracksChargedInP0DAndTPCInBunch,
    kTrueTracksChargedInP0DAndNoTPCInBunch
  };

  ///-------------  Michel Electron candidates ----------------------------------
  Int_t nP0DMichelElectrons;
};


#endif
