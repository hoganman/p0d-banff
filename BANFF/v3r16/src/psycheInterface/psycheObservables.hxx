#ifndef PSYCHEOBSERVABLES_HXX
#define PSYCHEOBSERVABLES_HXX

#include "BANFFObservableBase.hxx"
#include "BANFFEventBase.hxx"

class psycheLeptonCandidateMomentum: public BANFFObservableBase{



    public:

        psycheLeptonCandidateMomentum();
        virtual ~psycheLeptonCandidateMomentum(){};


        float GetCurrentValue(BANFFEventBase* event);

};

class psycheLeptonCandidateCosTheta: public BANFFObservableBase{

    public:

        psycheLeptonCandidateCosTheta();
        virtual ~psycheLeptonCandidateCosTheta(){};

        float GetCurrentValue(BANFFEventBase* event);


};


#endif
