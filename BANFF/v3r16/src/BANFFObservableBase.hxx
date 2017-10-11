#ifndef BANFFOBSERVABLEBASE_HXX
#define BANFFOBSERVABLEBASE_HXX

#include <string>
#include "BANFFEventBase.hxx"

class BANFFObservableBase{


    public:

        BANFFObservableBase(){};
        virtual ~BANFFObservableBase(){};

        //The name of this observable;
        std::string name; 

        ///Returns the current value of the given observable when passed an
        ///event.
        virtual float GetCurrentValue(BANFFEventBase* /*event*/){ return 0.0; };
};
#endif
