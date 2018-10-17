#include "p0dDataClasses.hxx"

AnaP0DTrackerTrack::AnaP0DTrackerTrack() : AnaTrack()
{
  TrackerMomentum = -999;
  for (int i = 0; i < 3; i++) TrackerDirection[i] = -999;
  for (int i = 0; i < 4; i++) TrackerPosition[i] = -999;

}

AnaP0DTrackerTrack::AnaP0DTrackerTrack(const AnaP0DTrackerTrack& track) : AnaTrack(track)
{
  TrackerMomentum = track.TrackerMomentum;
  for (int i = 0; i < 3; i++) TrackerDirection[i] = track.TrackerDirection[i];
  for (int i = 0; i < 4; i++) TrackerPosition[i] = track.TrackerPosition[i];
}

void
AnaP0DTrackerTrack::Print() const
{
  AnaTrack::Print();
  std::cout <<"MomentumentumAtFirstDetEntrance: "<<TrackerMomentum<<std::endl;
  std::cout <<"DirectionectionAtFirstDetEntrance: "<<TrackerDirection[0]<<" "<<TrackerDirection[1]<<" "<<TrackerDirection[2]<<std::endl;
  std::cout <<"PositionitionAtFirstDetEntrance: "<<TrackerPosition[0]<<" "<<TrackerPosition[1]<<" "<<TrackerPosition[2]<<" "<<TrackerPosition[3]<<std::endl;
}


// EventBoxP0D
EventBoxP0D::EventBoxP0D() {
  nP0DMichelElectrons = -999;
}
