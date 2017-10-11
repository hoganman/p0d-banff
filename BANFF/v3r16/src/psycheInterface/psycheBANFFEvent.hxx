#ifndef PSYCHEBANFFEVENT_HXX
#define PSYCHEBANFFEVENT_HXX

#include "BANFFEventBase.hxx"
#include "AnalysisManager.hxx"
#include "ND280AnalysisUtils.hxx"

///Extends the BANFFEventBase class to include a pointer to an AnaSuperEventB and a vector of
///weights. 
class psycheBANFFEvent: public BANFFEventBase{


    public:
        
        ///Constructor for use with data events.
        psycheBANFFEvent(AnaSuperEventB* event){
            detEvent = event;
            AnaEventB* aeb = dynamic_cast<AnaEventB*>(event->Event);
            if(anaUtils::IsRHC(aeb->EventInfo.Run)) beamMode =  1;
            else                                    beamMode = -1;
            
        };

        //Constructor for use with MC events.
        psycheBANFFEvent(AnaSuperEventB* event, int numbSysts, int number, bool lowMem) : BANFFEventBase(numbSysts,number,lowMem){ 
            detEvent = event;
            POTWeight = event->POTWeight;
            AnaEventB* aeb = dynamic_cast<AnaEventB*> (event->Event);
            if(anaUtils::IsRHC(aeb->EventInfo.Run)) beamMode =  1;
            else                                    beamMode = -1;
        };

        virtual ~psycheBANFFEvent(){};


        ///The event from psyche.
        AnaSuperEventB* detEvent;

        ///The vector of weights returned by ProcessEvent in AnalysisManager.
        ///Might include a correction weight for data events.
        std::vector<float> detWeights;
};

#endif
