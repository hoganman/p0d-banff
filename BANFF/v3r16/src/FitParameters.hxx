#ifndef FitParameters_h
#define FitParameters_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <stdlib.h>
#include <iostream>
#include <limits>
#include <string.h>
#include <vector>
#include <TAxis.h>
#include <TMatrixDSym.h>
#include <TRandom3.h>
#include "ThrowParms.hxx"
#include "EigenDecomp.hxx"
#include "WeightFile.hxx"

//Forward declaration of BANFFEventBase (both classes depend on each other.)
class BANFFEventBase;
class BANFFSampleBase;

//This is the only thing from SystematicVar.h that we need here:
enum PDFcode{ PDFGaus = 0, PDFFlat, PDFBino  };


enum ParamType { FLUX = 0,
    XSECRESP = 1, 
    XSECNORM = 2, 
    DET = 3,
    OBSNORM = 4,
	XSECFUNC = 5 };


///A base class to describe a parameter.  Holds information common to all
//parameter types.
class FitParameter{

    public:

        FitParameter(){
            enable = true;
            stepSize = -1;
            xMin = -1;
            xMax = -1;
            nStep = -1;
        };

        virtual ~FitParameter(){};


        ///When given a pointer to an event, records its parameter index in the
        ///event if it is a match to the event.  Does nothing otherwise.
        ///Must be implemented by each parameter derived class.
        virtual void MatchToEvent(BANFFEventBase* /*event*/){};

        ///Prints requirements specific to this parameter that determine how it
        ///is calculated and what events it applies to.  Used for validation
        ///purposes.  Implement in derived classes.
        virtual void PrintParameterRequirements(){};
        
            
        ///The prior value of the parameter.  If eigen decomposition of the parameter has taken place,
        ///this would be the value post-decomposition.
        double priorValue;

        ///The prior value of the parameter prior to eigen decomposition.
        double priorValueUD;

        ///The current value of the parameter (as thrown or varied by the fit.)
        ///If this parameter has been eigen-decomposed, this is the current value in
        ///the basis of the eigenvectors of the covariance matrix.
        double currentValue;

        ///The current value of the parameter (as thrown or varied by the fit),
        ///undecomposed.
        double currentValueUD;

        ///An enum defining the parameter type.  Can be one of:
        /// FLUX: 0
        /// XSECRESP: 1
        /// XSECNORM: 2
        /// DET: 3
        /// OBSNORM: 4
        /// XSECFUNC: 5
        //TODO: Actually, the idea here may be to do away with this, and then
        //have a base "HandleParameter" method that does things properly for
        //the different types, instead of having if statements all over the
        //place to have to check this and deal with it.
        //May still find it useful for picking out a subset (all FLUX for
        //example)
        ParamType paramType;


        ///An integer indicating which detector this parameter applies to.  Can
        ///mean different things depending on the parameter type (e.g. FLUX:
        ///ND280 or SK, DET: Which ND280 subdetector) 
        int detector;

        //The maximum neutrino energy to apply this parameter for.  Typically
        ///only used for FLUX and XSEC parameters. 
        double emax;

        ///The minimum neutrino energy to apply this parameter for.  Typically
        ///only used for FLUX and XSec parameters.
        double emin;

        ///The lower bound of this parameter used in the fit.
        double lowBound;

        ///The upper bound of this parameter used in the fit.
        double upBound;

        ///Indicates whether this parameter has bounds for any parameter scan
        ///or not.
        bool hasScanBounds;

        ///The lower bound for a parameter scan.  If there is none, set to some
        ///default value.
        double lowBoundScan;

        ///The upper bound for a parameter scan.  If there is none, set to some
        ///default value.
        double upBoundScan;

        ///This parameter's index within the array of FitParameters.
        int index;

        //Indexes the parameter by its type.  Used to count parameters within
        //their type.
        int typeIndex;

        ///Whether this parameter is to be varied in the fit. 
        bool vary;

        ///Whether this parameter requires decomposition or not:
        ///0: No decomposition required.
        ///1 or higher: Decomposition required.
        ///(higher might mean additional things.  TODO maybe want to switch out
        //for a bool since those additional things never seem to happen.
        int decompose;

        ///Whether to throw this parameter or not.
        //Set to the same value as vary in FLUX.  Do we just not need this?
        bool throwParm;


        ///An enum giving the prior distribution of the parameter.
        PDFcode priorType; 


        ///The full name of this parameter.
        std::string paramName;

        ///The short name of this parameter.  Abbreviates its type combined
        ///with its typeIndex.
        std::string shortName;

        ///A flag for if the parameter is discrete (true) or not (false).
        bool discrete;


        ///A bool that indicates whether this parameter should be used to
        ///weight events or not. (defaults to enabled)
        ///NB: Only consulted for slow, high memory weight calculations, i.e. NOT IN
        //THE FIT.
        bool enable;

        ///A double to hold the step size between spline knots - this lets us evaluate
        ///the spline much more quickly than ROOT does
        double stepSize;

        double xMax;
        double xMin;
        int nStep;

};

///A class for flux parameters.
class FluxParameter: public FitParameter{

    public:
        FluxParameter(){};
        virtual ~FluxParameter(){};


        ///The neutrino flavour associated with this flux parameter.
        //TODO: Note: This replaces "physicsCode" from the old FitParameter
        //struct, which held the flavour for flux parameters.
        int flavour;

        ///The beam mode, +1 for neutrino, -1 for antineutrino.
        int beamMode;

        ///Returns the neutrino flavour that this flux parameter applies to.
        int GetFlavour() {return flavour;};

        ///If this flux parameter is the flux parameter that applies to this
        ///event, note its index inside the event structure.
        void MatchToEvent(BANFFEventBase* event);

        ///Print out the energy range, neutrino flavor, and beam mode stored
        //for this parameter.
        void PrintParameterRequirements();
};

///A class for cross section response parameters.
class XSecRespParameter: public FitParameter{

    public:

        XSecRespParameter(){};
        virtual ~XSecRespParameter(){};

        ///The reaction code that this cross section parameter applies to.
        int reactionCode;

        //An enum value indicating which cross section parameter this parameter
        //is.
        XsecSysts xsecID;

        ///Whether this cross section response parameter should have a spline
        //produced or use a Graph. 
        bool spline;

        ///Whether this cross section is applied as a weight or not
        bool weight;

        ///Which material this parameter corresponds to.  This is an integer
        ///code which is read from the input file, and has the following key:
        ///-1: Does not care about the material.
        ///12: Carbon only.
        ///16: Oxygen only.
        ///Any other atomic weights as needed, for which parameters are defined
        ///in the parameter input file.
        //On second thought, this is unnecessary for XSec response parameters,
        //since they are divided by material in the parameter file, and if the
        //material of the specific event has no response, the spline would just
        //be 1 here.
        int* targetMaterial;

        ///There can be more than 1 target material, so store the number of
        //applicable ones here.
        int nTargetMaterials;

        ///XSecResponse parameters do not require matching, so this function
        ///does nothing.
        void MatchToEvent(BANFFEventBase* /*event*/){};

        ///Print out information showing which spline this parameter is
        //attached to. 
        void PrintParameterRequirements();

        double GetDiscreteWeight(std::string param);


};

///A class for cross section functionnal parameters.
class XSecFuncParameter: public FitParameter{

    public:

        XSecFuncParameter(){};
        virtual ~XSecFuncParameter(){};

        ///The number of reaction codes that this parameter applies to.
        int nReactionCodes;
		
        ///The reaction codes that this cross section parameter applies to.
        int* reactionCodes;

        ///The number of nupdg that this parameter applies to.
        int nNupdg;

        ///An array of nupdg that this parameter applies to.
        int* Nupdg;
		
        ///Which material this parameter corresponds to.  This is an integer
        ///code which is read from the input file, and has the following key:
        ///-1: Does not care about the material.
        ///12: Carbon only.
        ///16: Oxygen only.
        ///Any other atomic weights as needed, for which parameters are defined
        ///in the parameter input file.
        //On second thought, this is unnecessary for XSec response parameters,
        //since they are divided by material in the parameter file, and if the
        //material of the specific event has no response, the spline would just
        //be 1 here.
        int* targetMaterial;

        ///There can be more than 1 target material, so store the number of
        //applicable ones here.
        int nTargetMaterials;

        ///If the xsec functionnal parameter match the event
        void MatchToEvent(BANFFEventBase* event);

};

///A class for cross section normalization parameters.
class XSecNormParameter: public FitParameter{

    public:

        XSecNormParameter(){};
        virtual ~XSecNormParameter(){};

        ///The number of reaction codes that this parameter applies to.
        int nReactionCodes;

        ///An array of reaction codes that this parameter applies to.
        int* reactionCodes;
		
        ///The number of nupdg that this parameter applies to.
        int nNupdg;

        ///An array of nupdg that this parameter applies to.
        int* Nupdg;

        ///An enum value indicating which cross section parameter this parameter
        ///is.  Should always be -1 for NORM parameter.
        XsecSysts xsecID;

        ///Which material this parameter corresponds to.  This is an integer
        ///code which is read from the input file, and has the following key:
        ///-1: Does not care about the material.
        ///12: Carbon only.
        ///16: Oxygen only.
        ///Any other atomic weights as needed, for which parameters are defined
        ///in the parameter input file.
        int* targetMaterial;

        ///There can be more than 1 target material, so store the number of
        //applicable ones here.
        int nTargetMaterials;

        ///If this xsec norm parameter is the xsec norm parameter that applies to this
        ///event, note its index inside the event structure.
        void MatchToEvent(BANFFEventBase* event);

        //Print out the energy range of validity, as well as the reaction codes
        //that this parameter applies to, and the target material.
        void PrintParameterRequirements();
};

///A class for detector systematic parameters.
class DetParameter: public FitParameter{

    public:

        DetParameter(){};
        virtual ~DetParameter(){};

        ///Detector parameters do not require matching, so this function
        ///does nothing.
        void MatchToEvent(BANFFEventBase* /*event*/){};


        ///Do nothing, as there is no extra information here to verify.
        void PrintParameterRequirements(){};
};

///A class for observable normalization parameters.
class ObsNormParameter: public FitParameter{

    public:

        ObsNormParameter(){};
        virtual ~ObsNormParameter(){};

        ///A vector which for each relevant observable contains the minimum value that
        ///this observable normalization parameter applies to. 
        std::vector<double> obsMin;

        ///A vector which for each relevant observable contains the maximum
        ///value that this observable normalization parameter applies to.
        std::vector<double> obsMax;

        ///An integer identifying the sample that this observable normalization
        ///parameter applies to.
        int sample;


        ///If this observable normalization parameter is the observable normalization parameter that applies to this
        ///event, note its index inside the event structure.
        void MatchToEvent(BANFFEventBase* event);

        ///Print the observable ranges and sample that this parameter applues
        ///to.
        void PrintParameterRequirements();
};

///A class to store all the parameters and information affecting them as a
//whole (e.g. covariance).
class FitParameters {

    public :

        FitParameters();
        ~FitParameters();


        ///The total number of fit parameters, for fast access.
        int numbSystsFast;

        ///An array to copy the fit parameters into for fast access.
        FitParameter** fitParametersFast;

        ///A boolean flag to indicate whether the continuous parameters have
        ///been computed for these parameter values.  Used for fast parameter
        ///computation when have multiple discrete parameter sets in the fit.
        bool continuousParamsComputed;

        ///Takes the information stored inside FitParameters and copies it to
        ///the "Fast" variables.
        void FillFastVariables();

    private :

        ///A vector of pointers to all the FitParameters to consider.  They may or may not
        ///be varied in the fit.
        std::vector<FitParameter*> fitParameters;

        ///The active covariance matrix of the fit parameters.  Can be
        ///undecomposed or decomposed, depending on the point in the
        ///processing.
        TMatrixDSym *covariance;

        ///The inverse of the covariance matrix.
        TMatrixDSym *covarianceI;

        ///The undecomposed covariance matrix.
        TMatrixDSym *covarianceUD;

        ///The inverse of the undecomposed covariance matrix.
        TMatrixDSym *covarianceUDI;

        ///The covariance matrix output from the fitter.
        TMatrixDSym *covarianceFit;

        ///The current number of each of the parameter types.  If a new
        ///parameter of a given type is created, it is given the value of this
        ///variable, then this variable is incremented.
        int fluxIter, xsecRespIter,xsecFuncIter, xsecNormIter, detIter, obsNormIter;

        ///Interface to the parameter throwing code.  This includes the code
        ///for performing the Cholesky Decomposition.
        ThrowParms *throwParms;

        ///The random number generator.
        TRandom3 random;

        ///Interface to the code that diagonalizes the covariance matrix (i.e.
        ///performs the eigen decomposition.)
        EigenDecomp *eigenDecomp;

        ///For each throw, stores the value of the Chi^2 that compares the
        ///thrown values of the parameters to their prior values.
        std::vector<double> thrownChi2;

        /// An int to hold the number of threads we want to run on
        int nCores;

    public :


        ///The current discrete parameter combination being considered.
        int currentDiscreteParameterCombinationIndex;

        ///The total number of discrete parameters.
        int nDiscreteParameters;

        //The number of discrete parameter combinations that are possible, as
        //read in from the parameter file.
        int nDiscreteParameterCombinations;

        ///A 2D array storing the correspondence between the number of allowed
        ///discrete parameter combinations, the parameter number, and its value
        ///for this combination. i.e.
        ///parameterCombination[combinationNumber][parameterIndex] = allowed
        ///value for this parameter for this combination.
        double** discreteParameterCombinations;


        ///A variable that stores whether to perform eigendecomposition of the
        ///parameters for the fit.  Defaults to false, but should be set in the
        //fit code.
        bool eigenDecompositionRequired;

        ///Loops through the fit parameters and checks if any have their
        //decomposition flag set, and sets the eigenDecompositionRequired
        //variable accordingly.
        void ComputeEigenDecompositionRequired();

        //********************************
        //For handling flux parameters
        //********************************

        ///The number of flux parameters.
        int nFluxParms;

        ///The file containing the flux inputs.
        TFile *fluxFile;

        ///The binning for the near detector numu flux in neutrino beam mode.
        TAxis *nd_numode_numu_bins;

        ///The binning for the near detector numu flux in anti-neutrino beam mode.
        TAxis *nd_anumode_numu_bins;

        ///The binning for the near detector numubar flux in neutrino beam mode.
        TAxis *nd_numode_numub_bins;

        ///The binning for the near detector numubar flux in anti-neutrino beam mode.
        TAxis *nd_anumode_numub_bins;

        ///The binning for the near detector nue flux in neutrino beam mode.
        TAxis *nd_numode_nue_bins;

        ///The binning for the near detector nue flux in anti-neutrino beam mode.
        TAxis *nd_anumode_nue_bins;

        ///The binning for the near detector nuebar flux in neutrino beam mode.
        TAxis *nd_numode_nueb_bins;

        ///The binning for the near detector nuebar flux in anti-neutrino beam mode.
        TAxis *nd_anumode_nueb_bins;

        ///The binning for the SK numu flux in neutrino beam mode.
        TAxis *sk_numode_numu_bins;

        ///The binning for the SK numu flux in anti-neutrino beam mode.
        TAxis *sk_anumode_numu_bins;

        ///The binning for the SK numubar flux in neutrino beam mode.
        TAxis *sk_numode_numub_bins;

        ///The binning for the SK numubar flux in anti-neutrino beam mode.
        TAxis *sk_anumode_numub_bins;

        ///The binning for the SK nue flux in neutrino beam mode.
        TAxis *sk_numode_nue_bins;

        ///The binning for the SK nue flux in anti-neutrino beam mode.
        TAxis *sk_anumode_nue_bins;

        ///The binning for the SK nuebar flux in neutrino beam mode.
        TAxis *sk_numode_nueb_bins;

        ///The binning for the SK nuebar flux in anti-neutrino beam mode.
        TAxis *sk_anumode_nueb_bins;

        ///The flux covariance matrix.
        TMatrixDSym *flux_cov;

        //**********************************
        //For handling XSec parameters
        //**********************************

        ///The number of XSec parameters.
        int nXSecParms;

        ///The file containing the XSec inputs.
        TFile *fxsec;

        ///The covariance matrix for the XSec parameters.
        TMatrixDSym *xsec_cov;

        ///The nominal values for the XSec parameters.
        TVectorD *xsec_param_nom;

        ///The prior values for the XSec parameters.
        TVectorD *xsec_param_prior;

        ///The lower bounds for the XSec parameters.
        TVectorD *xsec_param_lb;

        ///The upper bounds for the XSec parameters.
        TVectorD *xsec_param_ub;

        ///The ID for each XSec parameter.
        //TODO Explain more.
        TVectorD *xsec_param_id;

        ///Whether the XSec parameter is thrown or not.
        TVectorD *xsec_param_throw;

        ///Whether the XSec parameter is fit or not.
        TVectorD *xsec_param_fit;

        ///TODO
        TVectorD *xsec_param_constrain;

        ///The neutrino interaction mode this XSec parameter applies for.
        TVectorD *xsec_param_mode;

        ///Whether this XSec parameter has a spline or not. TODO check.
        TVectorD *xsec_param_spline;

        ///Whether this XSec parameter is a weight or not. TODO check.
        TVectorD *xsec_param_weight;

        ///The name of the TAxis that stores the binning used by this
        ///parameter.
        TObjArray *xsec_param_binsname;

        ///Whether the parameter is discrete (1.0) or not (0.0)
        TVectorD* xsec_param_discrete;

        ///The modes to which this parameter applies (2016)
        TObjArray *xsec_norm_modes;
		
        ///The target elements of the parameter (2016)
        TObjArray *xsec_norm_elements;
		
        ///The neutrino PDG for the parameter (2016)
        TObjArray *xsec_norm_nupdg;
		
        ///The names of the discrete parameters.
        TObjArray* xsec_param_discreteNames;

        ///An array containing each allowed combination of the discrete
        //parameters, a TVectorD at each entry, with nrows equal to the number
        //of discrete parameters, indexed like the names TObjArray.
        TObjArray* xsec_param_discreteCombinations;

        ///The binning for the CCQE normalization parameters.
        ///Only used for 2013 style xsec input.
        TAxis *ccqe_bins;

        ///The binning for the CC1pi normalization parameters.
        ///Only used for 2013 style xsec input.
        TAxis *cc1pi_bins;

        ///The binning for the CCCoh normalization parameters.
        ///Only used for 2013 style xsec input.
        TAxis *cccoh_bins;

        ///The binning for the NCOther normalization parameters.
        ///Only used for 2013 style xsec input.
        TAxis *ncother_bins;

        ///The binning for the NC1pi0 normalization parameters.
        ///Only used for 2013 style xsec input.
        TAxis *nc1pi0_bins;

        ///The name of the xsec parameter stored in the xsec parameter input
        ///file.  The TObjArray contains TObjString objects for each parameter.
        TObjArray* xsec_param_name;

        ///The target material that this xsec parameter applies to.  Specified
        ///by the atomic mass as an integer.
        ///-1 for "applies to all"
        ///12 for Carbon
        ///16 for Oxygen
        ///etc.
        TVectorD* xsec_param_target;

        ///The bin in a TAxis  for a given xsec parameter stored in the input file.
        ///Is set to -1 for XSecResp parameters (which are unbinned), but set
        //to a specific bin number for each XSecNorm parameter, corresponding
        //to the bin in the saved TAxis that this parameter corresponds to.
        TVectorD* xsec_param_bin;

        //*****************************************************
        //For setting up parameters when want to do throws
        //based on the fit results.
        //*****************************************************

        ///The file with the fit results.
        TFile *ffit;

        ///The covariance matrix output from the fit.
        TMatrixDSym *fit_cov;

        ///The parameters from the fit file.
        TVectorD *fit_params;


        //*****************************************************
        //For setting up detector parameters if they are taken from an external
        //analysis interface.
        //*****************************************************

        ///The number of detector parameters.
        int nDetParms;

        ///Whether any of the detector parameters are to be varied.
        bool varyDetParms;

        ///The covariance matrix for detector parameters.
        TMatrixDSym *det_cov;


        //***********************************
        //For setting up observable normalization parameters (the approach
        //typically used for detector parameters, though possibly useful for
        //other applications.)
        //***********************************


        int nObsNormParms;

        ///The file with the detector parameter information and covariance
        ///matrix.
        TFile *obsNormFile;

        ///The covariance matrix for the observable normalization parameters.
        TMatrixDSym *obsNorm_cov;

        ///A vector of the axes for each observable normalization parameter.
        std::vector<TAxis*> obs_axes;

        ///A vector of the axis names needed to pull the axes from the file.
        std::vector<std::string> obs_axes_names;

        ///The weights for the detector parameters.
        TVectorD* obsNorm_weights;

        ///Loops through all the fitParameters and applies their MatchToEvent()
        ///method.
        void MatchToEvent(BANFFEventBase* event);


        ///Calculates the Chi^2 comparing the thrown values of the parameters
        ///to their prior values.
        double CalcChi2();

        ///Works like CalcChi2(), but limits to either the FLUX, XSEC (put
        ///XSECRESP as input) or OBSNORM parameters.
        double CalcChi2OfType(ParamType parameterType);

        ///Returns, for a given throw, the Chi^2 comparing the thrown values of the parameters
        ///to their prior values.
        double GetCalcChi2(int throwIndex){ return thrownChi2[throwIndex]; };


        ///Load in the file with the specified file name and add flux
        ///parameters according to its contents.
        ///Indicate whether to vary the parameters or not.
        ///This function accepts either a 2013 style flux file or a 2014 style
        ///flux file, and recognizes the type automatically.
        ///You must also specify which of neutrino and antineutrino modes you
        ///care about.  If you do something that doesn't make sense, the
        ///program will print an error and exit.
        void LoadFluxParametersFromFile(std::string fileName, bool loadNuMode, bool loadAntiNuMode, bool vary);

        ///Add a set of flux parameters according to the given binning, the
        ///detector, and the flavour, specifying whether they are to be varied
        ///or not.
        ///Also specify the beam mode that these flux parameters apply to.
        void AddFluxParameters(TAxis *bins, int detector, int flavor, int mode, bool vary = true);

        ////Load in the file with the specified file name and add xsec
        ///parameters according to its contents.
        ///Indicate whether to vary the parameters or not.
        void LoadXSecParametersFromFile(std::string fileName, bool vary);

        ///Add a cross section response parameter.
        void AddXsecRespParameter(std::string paramName, double value, XsecSysts parID, int reactioncode, int nTgtMat, int* tgtMatArray, double lowerbound, double upperbound, bool doSpline=true, bool doWeight=false, bool vary=true, bool constrain = true, bool isDiscrete = false);

        ///Add a cross section functionnal parameter.
        void AddXsecFuncParameter(std::string paramName, double value, TVectorD* reacCodes, TVectorD* nuPDG, int nTgtMat, int* tgtMatArray, double lowerbound, double upperbound, bool vary=true, bool constrain = true, bool isDiscrete = false);

        ///When passed a pointer to the summary tree (with the current event
        ///entry loaded), load the cross section responses from the summary
        ///tree for this event.
        void LoadXSecResponses(BANFFEventBase* event, WeightFile* sumTree);

        ///A version of LoadXSecResponses that makes use of BANFFEventBase
        ///arrays that are only as big as they need to be, to cut down on space
        ///wasted on huge arrays of NULL pointers.
        void LoadXSecResponsesLowMem(BANFFEventBase* event, WeightFile* sumTree);

        //Add a cross section normalization parameter.
        //void AddXsecNormParameter(std::string nameBase, double value, int reactioncode, int tgtMat, TAxis *norm_bins, bool vary = true);

        ///Add a cross section normalization parameter.
        void AddXsecNormParameter(std::string nameBase, double value, TVectorD* reacCodes, int nTgtMat, int* tgtMatArray, TAxis *norm_bins, int bin, bool vary = true);

        ///Add a cross section normalization parameter.
        void AddXsecNormParameter2016(std::string nameBase, double value, TVectorD* reacCodes, TVectorD* nuPDG, int nTgtMat, int* tgtMatArray, TAxis *norm_bins, int bin, bool vary = true);

        ///Load in the file with the specified file name and add observable
        //normalization
        ///parameters according to its contents.
        ///Need to know about the samples here (as the parameters are added
        ///separately for each sample), so pass in the number of samples and
        ///the array of BANFFSampleBase pointers.
        ///Indicate whether to vary the parameters or not.
        void LoadObsNormParametersFromFile(std::string fileName, int nSamples, BANFFSampleBase** samples, bool vary);

        //Add an observable normalization parameter.
        void AddObsNormParameter(double value, std::vector<double> obsMin, std::vector<double> obsMax, int det, int sample, bool vary = true);

        ///Add in a detector parameter.  The code to do this must be written in
        ///class that interfaces with the detector code.
        void AddDetParameter(double value, int det, PDFcode prior, double lBound, double uBound, std::string &name, bool vary=false);

        ///Effectively adds rows and columns of zeroes to the current
        ///covariance matrix, and copies this covariance matrix appropriately
        ///into the larger matrix after the original covariance matrix entries.  
        void AppendCovariance(TMatrixDSym *cov);

        ///Assuming that the fit parameters were the same and ordered in the
        ///same way as the current parameter set, load in the fitted parameter
        //values and their covariance as the nominal/prior values.
        void LoadFittedParameters(std::string fileName, int fitNumber = 0);

        ///Replace the covariance matrix stored in this FitParameters object
        //with one being supplied here.
        void SubstituteCovariance(TMatrixDSym* newCov);

        ///Takes the MACH3 interaction mode and gives back a TVectorD with the NEUT reacCodes
        TVectorD* MACH3toNEUTReacCode(TVectorD* MACH3, int isneut);
	
   	 	///Takes the nuPDG to which this parameter is applied and check if it's neutrino or antineutrino or both
  		int IsNeutrinoorAntiNeutrino(TVectorD* nuPDG);
		
        //TODO: Gets the current value of DET parameters and returns them.
        //Needs updating for the new format if do away with paramType, but I
        //don't think we will, as it is useful in some cases.
        std::vector<double> GetDetectorVector();

        ///Takes a BANFFEventBase pointer and loops through the parameters,
        ///matching to those that are suitable.
        void MatchParameters(BANFFEventBase* event);


        ///Sets up the ThrowParms object with a random seed and passes it a
        ///vector of prior values, along with the covariance matrix.
        void InitializeThrows(int seed=1987);

        ///Gets a vector of throws for the parameters from ThrowParms and sets
        ///the values of each of the parameters appropriately according to
        //those throws.
        void DoThrow(bool saveChi2=false);

        ///An alternate DoThrow method that rethrows the parameters if one is
        ///out of bounds, instead of just putting any out of bounds parameters
        ///at the boundary.  This is done to make sure that correlations in the
        ///throws are properly observed.
        void DoThrowRTOOB(bool saveChi2=false);

        //TODO: Handle this better, for all parameters.
        int GetNumbDetSysts() {return detIter;};

        int GetNumbSysts() {return (int)(fitParameters.size());};

        int GetNumbXsecRespSysts(){ return xsecRespIter; };

        ///Return the current value of parameter i.  Indicate whether to
        ///return the undecomposed parameter or not (default false).
        double GetCurrentValue(int i, bool UD=false);

        ///Return the prior value of parameter i.  Indicate whether to
        ///return the undecomposed parameter or not (default false).
        double GetPriorValue(int i, bool UD=false);

        ///Return the maximum neutrino energy parameter i applies to.
        double GetEmax(int i) { return fitParameters[i]->emax; };

        ///Return the minimum neutrino energy parameter i applies to.
        double GetEmin(int i) { return fitParameters[i]->emin; };

        ///Return whether this parameters has bounds for a parameter value
        ///scan.
        bool HasScanBounds(int i){return fitParameters[i]->hasScanBounds;};

        ///Return the lower bound for the parameter scan for parameter i.
        double GetScanLowerBound(int i){return fitParameters[i]->lowBoundScan;};

        ///Return the upper bound the the parameter scan for parameter i.
        double GetScanUpperBound(int i){return fitParameters[i]->upBoundScan;};

        ///Return the lower bound of parameter i.  If it does not have a
        ///lower bound, this returns a default value.
        double GetLowerBound(int i) { return fitParameters[i]->lowBound; };

        ///Return the upper bound of parameter i. If it does not
        ///have an upper bound, this returns a default value.
        double GetUpperBound(int i) { return fitParameters[i]->upBound; };

        ///Return the type of parameter this parameter is
        ParamType GetParamType(int i) { return fitParameters[i]->paramType; };

        //TODO Move to parameter derived class where this means something.
        //XsecSysts GetXsecID(int i) { return fitParameters[i]->xsecID; };

        //TODO rename and move to parameter derived classes where this means
        //something.
        //int GetPhysicsCode(int i) { return fitParameters[i]->physicsCode; };

        //TODO Move?
        //int GetDetector(int i) { return fitParameters[i]->detector; };

        //TODO Move?
        //int GetSample(int i) { return fitParameters[i]->sample; };


        //TODO Move
        //double GetObsMin(int i, int j){return fitParameters[i]->obsMin[j]; };
        //double GetObsMax(int i, int j){return fitParameters[i]->obsMax[j]; };

        ///Return the index of this parameter within is parameter type.
        int GetTypeIndex(int i) { return fitParameters[i]->typeIndex; };

        //TODO Move?
        //bool GetDoSpline(int i) { return fitParameters[i]->spline; };

        ///Return whether this parameter is to be varied in the fit or not.
        bool GetDoVary(int i){ return fitParameters[i]->vary; };

        ///Change whether this parameter is varied or not.
        void SetDoVary(int i, bool doVary){fitParameters[i]->vary = doVary;}

        ///Return whether this parameter is discrete.
        bool GetIsDiscrete(int i){ return fitParameters[i]->discrete; };

        ///Return the parameter's name.
        std::string GetParamName(int i){return fitParameters[i]->paramName; };


        ///Return the parameter error (diagonal entry in the covariance matrix
        ///for this parameter.  Indicate whether want the fitted value or not
        ///(default false.)
        double GetParameterError(int i, bool fitted=false);

        ///
        void SetParameterValue(int i, double value) { fitParameters[i]->currentValue = value; };
        void SetPriorParameterValue(int i, double value) { fitParameters[i]->priorValue = value; };
        void SetParameterName(int i, std::string &name) {fitParameters[i]->paramName = name; };
        void SetDecompose(int i, int decomp) { fitParameters[i]->decompose = decomp; };
        void SetThrowParameter(int i, bool throwparm){ fitParameters[i]->throwParm = throwparm; };
        double GetCurrentValueUD(int i){ return fitParameters[i]->currentValueUD; };




        //TODO: Add some methods for better access to the covariance matrix
        //(perhaps by parameter name, instead of trying to find the index, if,
        //for example, wanted to add a correlation by hand.


        ///Return the short name of parameter i.
        std::string GetShortName(int i){ return fitParameters[i]->shortName; };

        ///Return the step size to use for this parameter in a parameter scan.
        ///With the exception of the first 4 flux parameters, this is just a
        //function of the parameter error.
        double GetScanStepSize(int i);


        void SetFitCovarianceMatrixElement(int i, int j, float value) { (*covarianceFit)(i,j) = value; };
        void PrintCurrentValues();

        void ResetValues();
        void DecomposeParameters();
        void UnDecomposeParameters(bool fitted=true, bool skipCovariance=false);

        void WriteResults(int iteration, bool names, bool UD=true);
        void PrintAllParameterInfo();

        ///Return the index of a parameter with the supplied name.
        ///If that name is not in the parameter list, return -1.
        int GetParameterIndexFromName(std::string thisParamName);

        ///Enables a parameter for inclusion in the event weight.
        ///NB: Only implemented for slow, large memory weight calculations,
        ///i.e. NOT THE FIT.
        void EnableParameter(int i){fitParameters[i]->enable = true;};

        ///Enables a parameter for inclusion in the event weight.
        ///NB: Only implemented for slow, large memory weight calculations,
        ///i.e. NOT THE FIT.
        void EnableParameterByName(std::string paramToEnable);

        ///Disables a parameter, stopping it from being included in the event weight.
        ///NB: Only implemented for slow, large memory weight calculations,
        ///i.e. NOT THE FIT.
        void DisableParameter(int i){fitParameters[i]->enable = false;};


        ///Disables a parameter, stopping it from being included in the event weight.
        ///NB: Only implemented for slow, large memory weight calculations,
        ///i.e. NOT THE FIT.
        void DisableParameterByName(std::string paramToDisable);

        bool IsEnabled(int i){return fitParameters[i]->enable;};

        int fitIter;
};

#endif

