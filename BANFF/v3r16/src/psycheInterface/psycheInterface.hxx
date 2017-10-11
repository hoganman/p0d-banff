#ifndef PSYCHEINTERFACE_HXX
#define PSYCHEINTERFACE_HXX

#include "BANFFInterfaceBase.hxx"
#include "AnalysisManager.hxx"
#include "SystematicBase.hxx"
#include "ToyMaker.hxx"
#include "ToyMakerExample.hxx"
#include "WeightType.hxx"

class psycheInterface
	: public BANFFInterfaceBase
{
    public:

        ///Constructor for the psyche interface.  Give it the samples and fit
        ///parameters, and specify whether it should load detector parameters
        ///from psyche, and whether it should vary the detector parameters.
        psycheInterface(int nSamp = 0,
			BANFFSampleBase** samp = NULL,
			FitParameters* fitParms = NULL,
			bool loadDetParms = false,
			bool varyDetParms = false,
			std::string diagnostic = "");
        ~psycheInterface();

        ///The psyche analysis manager.  Holds data and MC events together, as
        ///loaded by the Experiment class.
        AnalysisManager* _man;
        ToyMaker* _toyMaker;
  
        ///psyche specific implementation of virtual LoadEvents method in base
        ///class.  Takes the mcFiles, dataFiles, and run periods and uses
        ///psyche's Experiment class to get them into the analysis manager.
        ///Includes initial processing and calls to the needed summary tree
        ///loading functions from the base class. 
        void LoadEvents();

        ///Passes the FitParameters values to psyche and re-processes the MC.
        ///Perform any actions necessary as a result of the reprocessing.
        void ReProcessMC();

        ///Converts the psyche PDFEnum to a BANFF PDFcode.  Ostensibly
        ///unnecessary, but perhaps useful to keep things clear.
        PDFcode GetPDFcode(SystematicBase::PDFEnum pdfEnum);

        ///Uses the information in psyche to create the detector parameters.
        ///Specify whether to vary the detector parameters or not.
        void LoadDetectorParameters(bool vary);
};

#endif
