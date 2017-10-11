#ifndef BANFFEVENTBASE_HXX
#define BANFFEVENTBASE_HXX


#include <stdlib.h>
#include <iostream>
#include <vector>
#include <TGraph.h>
#include <TSpline.h>
#include "WeightFile.hxx"
//#include "FitParameters.hxx"
//#include "BANFFSample/BANFFSampleBase.hxx"

//Need to forward declare BANFFSampleBase, because both classes need to know
//about each other, but only one can actually come first in the compilation.
class BANFFSampleBase;
class FitParameters;

///A Base class for holding information about an event that the BANFF code is
///considering.  Is used for both data and MC, with MC variables just not being
///filled  for data.
class BANFFEventBase{

    public:


        //Empty constructor, useful for data.
        BANFFEventBase();

        //Constructor with values needed for systematics.  Use for MC.
        BANFFEventBase(int numbSysts, int number, bool conserveMemory);


        virtual ~BANFFEventBase();

        ///A pointer to the sample that this event gets selected into.
        BANFFSampleBase* sample;


        ///An array of length fitParameters->nDiscreteParameterCombinations,
        ///which stores the weight that is applied to this event due to the
        ///given discrete parameter combination. (Can compute once and never
        //have to recalculate.)
        double* discreteParameterWeights;

        ///If doing a binned likelihood, the observable bin that this event
        ///goes into would be stored here.
        int observableBin;


        //NB: The following are for storing TGraphs and TSpline3 pointers more
        //efficiently when the sample cannot change.  They heavily reduce the
        //amount of space required in RAM by replacing arrays of null pointers
        //with arrays that contain exclusively the information we need.
        
        ///The number of continuous XSECRESP fit parameters relevant to this
        //event that have TGraphs stored for their response functions.
        int nContXSRTGraphs;

        ///The fit parameter indices of continuous XSECRESP fit parameters relevant to this event
        ///that have TGraphs stored for their response functions.
        int* contXSRTGraphIndices;

        ///The array of TGraphs for continuous XSECRESP fit parameters relevant
        ///to this event.
        TGraph** contXSRTGraphs;

        ///The number of continuous XSECRESP fit parameters relevant to this
        //event that have TGraphs stored for their response functions.
        int nContXSRTSpline3s;

        ///The fit parameter indices of continuous XSECRESP fit parameters relevant to this event
        ///that have TGraphs stored for their response functions.
        int* contXSRTSpline3Indices;

        ///The array of TGraphs for continuous XSECRESP fit parameters relevant
        ///to this event.
        TSpline3** contXSRTSpline3s;

        ///The number of discrete XSECRESP fit parameters relevant to this
        //event that have TGraphs stored for their response functions.
        int nDiscXSRTGraphs;

        ///The fit parameter indices of discrete XSECRESP fit parameters relevant to this event
        ///that have TGraphs stored for their response functions.
        int* discXSRTGraphIndices;

        ///The array of TGraphs for continuous XSECRESP fit parameters relevant
        ///to this event.
        TGraph** discXSRTGraphs;

        ///The number of discrete XSECRESP fit parameters relevant to this
        //event that have TWeights stored for their response functions.
        int nDiscXSRTWeights;

        ///The fit parameter indices of discrete XSECRESP fit parameters relevant to this event
        ///that have TWeights stored for their response functions.
        int* discXSRTWeightIndices;

        ///The array of TWeights for continuous XSECRESP fit parameters relevant
        ///to this event.
        double* discXSRTWeights;

        ///Get method to access weight outside of class
        double GetXSecWeight(int index, int sample){return xsecWeights[index][sample];}

        ///Get method to access splines outside of class
        TSpline3* GetXSecSpline(int index, int sample){return xsecSplines[index][sample];}

        ///Get method to access TGraphs outside of class
        TGraph* GetXSecGraph(int index, int sample){return xsecGraphs[index][sample];}



    protected:

        //The variables used for MC.

        //The number of samples that truth information is stored for.
        int nSamples;

        ///The true neutrino energy in GeV, stored as an array for all possible
        ///selection results.
        Double_t* nuEnergy;
        Double_t* q2;
        Int_t* nuFlavor;
        Int_t* reactionCode;
        int numbXsecSysts;

        ///The material that the neutrino interaction occurred on.
        Int_t* targetMaterial;


        ///The beam mode that that this event corresponds to.
        ///+1 for neutrino beam mode.
        ///-1 for antineutrino beam mode.
        int beamMode;

        ///The index of the flux parameter that applies to this event.
        int fluxParameter;

        ///The index of the cross section functionnal parameter that applies
        ///to this event.
        int xsecFuncParameter;
		
        ///The index of the cross section normalization parameter that applies
        ///to this event.
        int xsecNormParameter;

        ///The index of the observable normalization parameter that applies to
        ///this event.
        int obsNormParameter;

        ///The enum identifying the sample that this event belongs to.
        int sampleIndex;

        TSpline3 ***xsecSplines;
        TGraph ***xsecGraphs;
        double **xsecWeights;
        float *twkDials;

        ///The cross section weight for this event, calculated from XSecNorm
        ///and XSecResp parameter values.  Defaults to 1.0.
        double xsecWeight;

        ///The cross section weight with no discrete parameters for this event.
        double xsecWeightNoDiscreteParams;

        ///The flux weight for this event based off of the flux tuning applied.
        ///Since this depends on which true vertex is being used, this is an
        ///array over all the samples that could have been selected into.
        Double_t* fluxWeight;

        ///The weight applied to this event to normalize to data POT.  Defaults
        ///to 1.0 unless set in the interface to detector code.
        ///This is sample independent.
        double POTWeight;

        ///The weight provided by the interface to detector code for detector
        ///systematics.
        ///Although this is probably sample dependent, it gets filled after the
        ///selection is performed, so an array of them does not need to be
        ///stored.
        double detWeight;

        ///The product of all of the above weights for whatever sample the
        ///event is in when the calculation is performed.  Its calculation includes
        ///setting it to 0 if it goes negative.
        double totalWeight;

        ///The total weight including all parameters but any discrete
        //parameters.
        double totalWeightNoDiscreteParams;


        ///The entry in the summary tree that this event corresponds to.
        int evtNumb;

        //TODO:  No longer needed, so delete?
        std::vector<float> thrownWeights;
        std::vector<char> thrownSamples;
        std::vector<short> thrownBins;


    public:

        int GetNSamples(){ return nSamples; };
        double GetNuEnergy();
        double GetQ2();
        int GetNuFlavor();
        int GetReactionCode();
        int GetBeamMode(){ return beamMode; };
        int GetFluxParameter(){ return fluxParameter; };
        int GetXsecNormParameter(){ return xsecNormParameter; };
        int GetXsecFuncParameter(){ return xsecFuncParameter; };
        int GetObsNormParameter(){ return obsNormParameter; };
        int GetObservableBin(){ return observableBin; };
        int GetSampleIndex(){ return sampleIndex; };
        void SetThrow(float weight, char sample, short bin);   
        void SetThrow(float weight, char sample, short bin, int throwIndex);   
        float GetThrownWeight(int index){ return thrownWeights[index]; };
        char GetThrownSample(int index){ return thrownSamples[index]; };
        unsigned int GetNThrownBins(){return thrownBins.size();};
        short GetThrownBin(int index){return thrownBins[index]; };

        ///Return the index of the summary tree that contains information
        ///corresponding to this event.
        int GetSumTreeIndex(){ return evtNumb;};

        void SetNSamples(int x) { nSamples = x; };
        void SetNuEnergy(Double_t* x);
        void SetQ2(Double_t* x);
        void SetNuFlavor(Int_t* x);
        void SetReactionCode(Int_t* x);
        void SetFluxParameter(int x){ fluxParameter = x; };
        void SetPOTWeight(double weight){ POTWeight = weight;}
        void SetDetWeight(double weight){detWeight = weight;};
        void SetFluxWeight(Double_t* weight);
        void SetXsecFuncParameter(int x){ xsecFuncParameter = x; };
        void SetXsecNormParameter(int x){ xsecNormParameter = x; };
        void SetObsNormParameter(int x){ obsNormParameter = x; };
        void SetObservableBin(int x){ observableBin = x; };
        void SetSampleIndex(int x){ sampleIndex = x; };
        void SetTargetMaterial(Int_t* x);

        void AddXsecResponse( int index , TClonesArray *graph, TSpline3 **spline );
        void AddXsecWeight( int index, double* weight_arr);

        void SetXsecTwk( XsecSysts index, float tweak );
        double GetXsecWeight();
        double GetXsecWeight(std::vector<float> &twkDialsv);
        double GetXsecWeight(FitParameters &fitParameters);
        void SaveTotalWeight(FitParameters* fitParameters, double Q2, int NuFlavor);
        double GetTotalWeight(){ return totalWeight; };
        double GetFluxWeight();
        double GetPOTWeight(){ return POTWeight;};
        double GetDetWeight(){ return detWeight;};
        int GetTargetMaterial();

        void SaveTotalWeightFast(FitParameters* fitParameters, double Q2, int NuFlavor);
        void SaveTotalWeightFastLowMem(FitParameters* fitParameters, double Q2, int NuFlavor);

        ///Evaluates the TGraphs for the discrete parameter combinations and
        ///saves them for quicker access in the fit.
        void SaveDiscreteParameterWeights(FitParameters* fitParameters);

        ///Evaluates the TGraphs for the discrete parameter combinations and
        ///saves them for quicker access in the fit, but using less memory.
        void SaveDiscreteParameterWeightsLowMem(FitParameters* fitParameters);


        ///Apply an additional weight to the total weight.  Useful for when need
        ///to make a change to the saved value without going through the full weight
        ///calculation procedure (e.g. when dealing with discrete parameters.)
        void WeightTotalWeight(double weight){totalWeight = weight*totalWeight;};

        ///Copies summary tree information from one selection's entries to all
        ///of them.  Only needs to be used if want to allow for migrations with
        ///splines that only have information saved for one selection result.
        void CopySummaryTreeInfo();
		
		// Calculates the absolute eRPA for given Q2, BERNSTEIN STYLE	
		double const calcRPA(double Q2, double A, double B, double D, double E, double U);
		
};
#endif
