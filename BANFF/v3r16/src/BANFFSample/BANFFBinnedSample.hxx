#ifndef BANFFBINNEDSAMPLE_HXX
#define BANFFBINNEDSAMPLE_HXX

#include "BANFFSample/BANFFSampleBase.hxx"
#include "BANFFObservableBase.hxx"
#include "THn.h"
#include "TH1D.h"

//Forward declare BANFFObservableBase
class BANFFObservableBase;


class BANFFBinnedSample: public BANFFSampleBase{

    public:

        BANFFBinnedSample(SampleId::SampleEnum sampleIDInput, int nObsInput, BANFFObservableBase** obsInput, bool ithrowMCStat = false, bool ithrowStat = false, bool ithrowMCStatPoisson = false);
        BANFFBinnedSample(std::string nameInput, int sampleIDInput, int nObsInput, BANFFObservableBase** obsInput, TAxis** axesInput, bool ithrowMCStat = false, bool ithrowStat = false, bool ithrowMCStatPoisson = false);
        virtual ~BANFFBinnedSample(){};


        ///An array holding the axes for the histograms, with one axis for each
        ///observable.
        TAxis** axes;


        ///Stores the number of histogram bins (excluding underflow and
        ///overflow)
        int nBins;

        ///Stores the indices of the bins in the histograms that are not
        //underflow or overflow bins.
        int* bins;

        ///The data histogram.  The dimensionality is the number of
        ///observables.
        THnD* data;

        ///The nominal MC histogram. The dimensionality is the number of
        ///observables.
        THnD* nomMC;

        ///The predicted MC histogram. The dimensionality is the number of
        ///observables.
        THnD* predMC;

        ///Some histograms defined for diagnostic purposes.
        
        ///Nominal MC as loaded in, with no weights of any kind applied.
        ///NB: If detector systematics are enabled, and there are any
        //migrations at nominal, they will be included here.
        THnD* nomMCNoWeights;

        ///The nominal MC with only POT weights applied.
        THnD* nomMCPOTWeights;

        ///POT weighted nominal MC with flux tuning weights applied.
        THnD* nomMCNomFluxWeights;

        ///A histogram of these flux weights.
        TH1D* nominalFluxWeights;

        ///POT weighted nominal MC with XSec nominal weights applied.
        THnD* nomMCNomXSecWeights;

        ///A histogram of these xsec weights.
        TH1D* nominalXSecWeights;

        ///POT weighted nominal MC with nominal detector systematic weights applied.
        THnD* nomMCNomDetWeights;

        ///A histogram of the nominal det weights.
        TH1D* nominalDetWeights;

        ///POT weighted nominal MC with "covariance" weights applied.
        THnD* nomMCNomCovWeights;

        ///A histogram of the  "covariance"  weights.
        TH1D* nominalCovWeights;
        
        ///POT weighted nominal MC with flux and xsec weights.
        THnD* nomMCNomFluxAndXSecWeights;

        ///POT weighted nominal MC with flux and det weights.
        THnD* nomMCNomFluxAndDetWeights;


        ///An array of THnD for storing reaction code breakdowns of the MC
        ///prediction.
        ///This is an array of length 200, with reaction code x being stored at
        ///index 100 + x
        THnD** rxnPredMC; 

        ///A bool for indicating whether this sample's
        ///SaveObservableInformation() function has been called, because this
        ///affects how histograms are filled and must be processed when saved
        //to file.
        bool observableInformationSaved;

        ///For handling the data.
        void AddDataEvent(BANFFEventBase* event);

        ///Loads the data from a file instead of using BANFFEventBase objects.
        ///Useful for validation purposes if have generated some fake data from
        ///MC with known parameter values to try to fit.
        ///Supply at pointer to a TFile, and the name of the TObject (in this
        //case a THnD) the data.  As a basic sanity check, If the number of bins doesn't match the
        //binning already present, exits instead of proceeding.
        void LoadDataFromFile(TFile* dataFile, std::string dataName);

        ///Copies the nominal MC
        ///histograms from nomMC into data, so they will be used as "data" to
        ///fit against.  NB: Must call the interface's "BuildNominalMC()"
        ///before calling this.
        void UseNominalMCAsData();
        

        ///For saving the data to an output file.
        ///Supply a name for the histogram.
        void SaveData(std::string dataName);

        ///For handling the nominal MC.
        void AddNominalMCEvent(BANFFEventBase* event);

        ///For saving the MC prediction to an output file, with the name
        ///supplied for the histogram.
        void SaveNominalMC(std::string nomMCName);

        ///For handling the MC prediction.
        void AddPredictedMCEvent(BANFFEventBase* event);

        ///For performing any final actions on the MC prediction as a whole.
        ///In the case of this binned sample, setting the bin errors and doing
        ///any modifications required if throwing MC statistical errors or
        ///statistical errors is needed.
        void FinalizeMCPrediction();

        ///For saving the MC prediction to an output file, with the name
        ///supplied for the histogram.
        void SaveMCPrediction(std::string predMCName);

        ///Reset the MC prediction.
        void ResetMCPrediction();

        ///Calculates this sample's contirbution to the -2*log likelihood ratio
        ///that is minimized by the BANFF fit.  In the case of these binned
        //samples, this is a delta chi squared. 
        double CalcLLRContrib();

        ///Saves the observable bin number that the event falls into in this
        ///sample.
        void SaveObservableInformation(BANFFEventBase* event);

        ///Add an event to the reaction code breakdown.  This method will also
        ///create the histogram if it doesn't exist.
        void AddMCEventToReactionCodeBreakdown(BANFFEventBase* event);
        
        ///Saves the events into histograms broken down by their reaction code.
        ///Supply naming information that will have "rxn_<code>_" prepended to
        ///it.
        void SaveReactionCodeBreakdown(std::string nameBase); 


};
#endif
