#ifndef BANFFINTERFACEBASE_HXX
#define BANFFINTERFACEBASE_HXX

#include "BANFFSample/BANFFSampleBase.hxx"
#include "BANFFEventBase.hxx"
#include "FitParameters.hxx"
#include <string>
#include <vector>
#include <map>
#include <sys/time.h>

///A class to interface between the fitter and the samples, data/mc, and the
///parameters being fit.  Additional code needed to interface with specific
///other software packages must be coded in a derived class.
class BANFFInterfaceBase{

    public:

       BANFFInterfaceBase(int nSamp = 0,
			  BANFFSampleBase** samp = NULL,
			  FitParameters* fitParms = NULL,
			  std::string diagnostic = "");
        virtual ~BANFFInterfaceBase();

        ///A variable to store the total POT of all data loaded.
        double totalDataPOT;

        ///A variable to store the total POT of all MC loaded.
        double totalMCPOT;

        timeval tim;
        double t0;
        double t1;

        /// nCores we want to run on - to avoid setting this each iteration
        int nCores;

        ///The name for the diagnostic file.
        std::string diagnostic_name;

        ///The number of samples being used in the fit.
        int nSamples;

        ///The array of samples being used in the fit.
        BANFFSampleBase** samples;

        ///The number of data events.
        int nDataEvents;

        ///The array of pointers to the data events.
        BANFFEventBase** dataEvents;

        ///The number of MC Events.
        int nMCEvents;

        ///The array of pointers to the MC events.
        BANFFEventBase** mcEvents;

        ///The parameters being considered.
        FitParameters* fitParameters;

        ///The list of lists of files.
        std::string file_list;
        
        ///A map that stores a vector of (string) file paths to data files, with the run
        ///identifier parameterizing the map.
        std::map<std::string, std::vector<std::string> > dataFiles;

        ///A map that stores a vector of (string) file paths to data files, with the run
        ///identified parameterizing the map.  MC is further broken down into
        ///the type (magnet, basket, mec, sand) within each run.
        ///The first element of the pair is the type, the 2nd is the file path.
        std::map<std::string, std::vector<std::pair<std::string, std::string> > > mcFiles;


        ///A variable to facilitate searching through the summary tree for
        ///matching to an event.
        int sumTreeCurrentSearchIndex;

        //The summary tree loaded from the input file. (With RooTracker
        //information and splines.)
        WeightFile* sumTree;


        ///Stores the TruthVertexID value from the summary tree of the last
        ///event that passed selection.  Used to verify spline matching.
        int sumTreeTruthVtx;

        ///Stores the neutrino PDG value from the summary tree of the last
        //event that passed selection.  Used to verify spline matching.
        int sumTreeNuPDG;

        ///Stores the neutrino energy from the summary tree of the last event
        ///that passed selection.  Used to verify spline matching.
        double sumTreeEnu;

        ///A counter to help with assessing fit timing by counting the number
        ///of times the fit function is called.
        int countApplyParamsAndCalcLLR;

        ///An array to store values related to the likelihood calculation for
        ///marginalizing over discrete parameter combinations.
        double* like_array;

        ///A bool to store the value of the "UseLowMemoryVersion" parameter, so the
        ///code can know whether to use the low memory option.  It's only
        ///compatible with running the fit.
        bool useLowMem;


        ///Consult text input files and load the file paths and run periods
        ///into the dataFile and mcFiles vectors.
        void ParseInputFiles();

        ///Load the data and MC events from the external software package.
        ///Data and nominal MC processing is coded at this step.
        virtual void LoadEvents(){};

        ///Loops through the events and computes the each event total weight given the
        ///current values of the fit parameters. 
        void ComputeEventWeights();

        ///Reprocess the MC to make a prediction with new FitParameters values.
        virtual void ReProcessMC(){};

        ///Load information from the summary tree into this BANFFEventBase.  Meant to be called
        ///in the derived class LoadEvents() in order to bring in information
        ///from the summary tree, including the splines generated in
        ///T2KReWeight, for this event.
        void LoadSummaryTreeInfo(BANFFEventBase* event);

        ///Loop through the data events and samples and perform sample specific
        ///actions to handle them.  Dealing with base classes is sufficient to
        ///handle this here.
        void BuildData();

        ///Loop through the MC events and the samples and perform sample
        ///specific actions to handle them for nominal MC.  Dealing with base classes is
        ///sufficient to handle this here.
        ///Calls ComputeEventWeights() in order to compute a nominal event
        //weight (flux plus any corrections.)
        void BuildNominalMC();

        ///Loop through the MC events and the samples and perform sample
        ///specific actions to handle them for predicted MC.  Dealing with base classes is
        ///sufficient to handle this here.
        ///Calls ComputeEventWeights before looping over the events so that the
        ///weights are ready before taking sample specific actions.
        void BuildPredictedMC();

        ///A method that throws toys with the information stored in the
        ///interface.
        //Calls methods in FitParameters for actually throwing the parameters,
        //but operates as a convenience function for looping through samples
        //and saving things as needed.
        void ThrowToys(int nToys, std::string fileName);

        ///Given a vector of doubles (representing parameter values), calculate
        ///the -2*log likelihood ratio that the BANFF fit minimizes.
        double ApplyParamsAndCalcLLR(const std::vector<double>& x);

        ///Given the the various number of sigma values to vary the parameters
        ///by, goes through each parameter one by one, and while holding the
        ///others at nominal, produces the usual output in a root file showing how the MC is affected by
        ///each variation.
        void VaryParametersByNSigma(int nSigmaValues, double* sigmaValues, std::string fileName);

        ///Loops through the events and builds the reaction code breakdowns.
        void BuildReactionCodeBreakdowns();    

        ///Condenses the building, saving, and resetting of the MC prediction
        ///into one method, instead of copying and pasting several lines of
        ///code when want to do this. 
        void BuildSaveAndResetMCPrediction(std::string nameSuffix);
};

#endif
