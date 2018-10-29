#ifndef BANFFSAMPLEBASE_HXX
#define BANFFSAMPLEBASE_HXX

#include "BANFFObservableBase.hxx"
#include "BANFFEventBase.hxx"
#include <string>

//Forward declarations..
class BANFFObservableBase;
class BANFFEventBase;


class BANFFSampleBase{

    public:
    
        ///The user must supply an integer to identify the sample, the number
        ///of observables to consider with this sample, and an array of
        ///pointers to BANFFObservableBase objects that define the observable.
        BANFFSampleBase(std::string nameInput, int sampleIDInput, int nObsInput, BANFFObservableBase** obsInput, bool  ithrowMCStat = false, bool ithrowStat = false, bool ithrowMCStatPoisson = false){
         
            name = nameInput;
            sampleID = sampleIDInput;
            nObs = nObsInput;
            obs = obsInput;
            throwMCStat = ithrowMCStat;
            throwStat = ithrowStat;
            throwMCStatPoisson = ithrowMCStatPoisson;

            if((throwMCStat == true) && (throwMCStatPoisson == true)){

                std::cout << "ERROR: Throwing MC statistical errors twice." << std::endl;
                exit(0);

            }
            
        };

        virtual ~BANFFSampleBase(){};

        ///A string with the name of the sample.
        std::string name;

        ///An integer code for identifying this sample.
        int sampleID;

        ///The number of observables this sample has.
        int nObs;

        ///The array of observables that are measured for this sample.
        BANFFObservableBase** obs;

        ///Whether to throw MC statistical errors for this sample.
        bool throwMCStat;

        ///Whether to throw statistical errors for this sample.
        bool throwStat;

        ///Whether to throw MC statistical errors for this sample using a
        ///Poisson throw for each event, instead of a Gaussian throw on the
        ///total bin content.  Only one of throwMCStat
        bool throwMCStatPoisson;

        ///For handling the data.
        virtual void AddDataEvent(BANFFEventBase* /*event*/){};

        ///Loads the data from a file instead of using BANFFEventBase objects.
        ///Useful for validation purposes if have generated some fake data from
        ///MC with known parameter values to try to fit.
        ///Supply at pointer to a TFile, and the name of the TObject storing
        //the data.
        virtual void LoadDataFromFile(TFile* /*dataFile*/, std::string /*dataName*/){};

        ///Set's the "data" to be nominal MC, so can perform a fit to nominal
        ///MC for validation purposes.
        virtual void UseNominalMCAsData(){};

        ///For saving the data to an output file.
        ///Supply a name for whatever form the data is in to save it as in the
        ///output file.
        virtual void SaveData(std::string /*dataName*/){};

        ///For handling the nominal MC.
        virtual void AddNominalMCEvent(BANFFEventBase* /*event*/){};

        ///For saving the MC prediction to an output file, with the name
        ///supplied for whatever form that is in.
        virtual void SaveNominalMC(std::string /*nomMCName*/){};

        ///For handling the MC prediction.
        virtual void AddPredictedMCEvent(BANFFEventBase* /*event*/){};

        ///For performing any final actions on the MC prediction as a whole.
        virtual void FinalizeMCPrediction(){};

        ///For saving the MC prediction to an output file, with the name
        ///supplied for whatever form that is in.
        virtual void SaveMCPrediction(std::string /*predMCName*/){};

        ///Reset the MC prediction.
        virtual void ResetMCPrediction(){};

        ///Calculates this sample's contribution to the -2*log likelihood ratio
        ///that is the quantity minimized by the BANFF fit.
        virtual double CalcLLRContrib(){return 0.0;};

        ///Pre-saves observable information to this event that is useful to
        ///this sample, in order to speed up histogram filling in the fit.
        virtual void SaveObservableInformation(BANFFEventBase* /*event*/){};

        ///Add an event to the reaction code breakdown.  This method will also
        ///create the histogram if it doesn't exist.
        virtual void AddMCEventToReactionCodeBreakdown(BANFFEventBase* /*event*/){};
        
        ///Add an event to the reaction code true energy breakdown.  This method will also
        ///create the histogram if it doesn't exist.
        virtual void AddMCEventToReactionCodeTrueEnergyBreakdown(BANFFEventBase* /*event*/){};
        
        ///Saves the events into histograms broken down by their reaction code.
        ///Supply naming information that will have "rxn_<code>_" prepended to
        ///it.
        virtual void SaveReactionCodeBreakdown(std::string /*nameBase*/){}; 

};

#endif
