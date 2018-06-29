#include "psycheInterface/psycheInterface.hxx"
#include "BANFFSample/BANFFBinnedSample.hxx"
#include "psycheInterface/psycheObservables.hxx"
#include "BANFFObservableBase.hxx"
#include "FitParameters.hxx"
#include "Parameters.hxx"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include "Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx"
#include <sstream>

//Set up the samples and observables with proper binning, then load the
//parameters and perform the fit.
int main(int argc, char** argv){

    if(argc > 2){

        std::cout << "Too many command line arguments!" << std::endl;
    }

    //If there is 1 command line argument, it is specifying a parameter
    //override file.  Read this in now.
    //According to the documentation for ReadParamOverrideFile, the code will
    //definitely use these values of the parameters, ignoring the contents of
    //any other parameters file.
    else if(argc == 2){

        std::string paramFile(argv[1]);
        ND::params().ReadParamOverrideFile(paramFile);
    }

    ND::params().SetReadParamOverrideFilePointPassed();

    //Create the observables we will use.
    psycheLeptonCandidateMomentum* pmu  = new psycheLeptonCandidateMomentum();
    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;
    bool Do4PiFHC = (bool)ND::params().GetParameterI("BANFF.Do4PiFHC");


    //For each observable and sample set up the binning used in the fit.

    //Set the binning for the CC0pi sample.
    Int_t npbins_0pi = 14;
    Double_t pbins_0pi[15] = {0., 300., 400., 500., 600., 700., 800., 900., 1000., 1250., 1500., 2000., 3000., 5000., 30000.};
    Int_t nctbins_0pi = 11;
    Double_t ctbins_0pi[25];
    if(!Do4PiFHC){
      nctbins_0pi = 11;
      ctbins_0pi[ 0] = -1.0;
      ctbins_0pi[ 1] =  0.6;
      ctbins_0pi[ 2] =  0.7;
      ctbins_0pi[ 3] =  0.8;
      ctbins_0pi[ 4] =  0.85;
      ctbins_0pi[ 5] =  0.90;
      ctbins_0pi[ 6] =  0.92;
      ctbins_0pi[ 7] =  0.94;
      ctbins_0pi[ 8] =  0.96;
      ctbins_0pi[ 9] =  0.98;
      ctbins_0pi[10] =  0.99;
      ctbins_0pi[11] =  1.0;
    }else{
      nctbins_0pi = 19;
      ctbins_0pi[ 0] = -1.0;
      ctbins_0pi[ 1] = -0.6;
      ctbins_0pi[ 2] = -0.4;
      ctbins_0pi[ 3] = -0.2;
      ctbins_0pi[ 4] = -0.1;
      ctbins_0pi[ 5] =  0.;
      ctbins_0pi[ 6] =  0.1;
      ctbins_0pi[ 7] =  0.2;
      ctbins_0pi[ 8] =  0.4;
      ctbins_0pi[ 9] =  0.6;
      ctbins_0pi[10] =  0.7;
      ctbins_0pi[11] =  0.8;
      ctbins_0pi[12] =  0.85;
      ctbins_0pi[13] =  0.90;
      ctbins_0pi[14] =  0.92;
      ctbins_0pi[15] =  0.94;
      ctbins_0pi[16] =  0.96;
      ctbins_0pi[17] =  0.98;
      ctbins_0pi[18] =  0.99;
      ctbins_0pi[19] =  1.0;
    }

    TAxis* pAxis_0pi = new TAxis(npbins_0pi, pbins_0pi);
    TAxis* thAxis_0pi = new TAxis(nctbins_0pi, ctbins_0pi);
    TAxis** cc0pi_axes = new TAxis*[2];
    cc0pi_axes[0] = pAxis_0pi;
    cc0pi_axes[1] = thAxis_0pi;


    //Set the binning for the CC1pi sample.
    Int_t npbins_1pi = 13;
    Double_t pbins_1pi[14] = {0., 300., 400., 500., 600., 700., 800., 900., 1000., 1250., 1500., 2000., 5000., 30000.};
    Int_t nctbins_1pi;
    Double_t ctbins_1pi[25];
    if(!Do4PiFHC){
      nctbins_1pi = 11;
      ctbins_1pi[ 0] = -1.0;
      ctbins_1pi[ 1] =  0.6;
      ctbins_1pi[ 2] =  0.7;
      ctbins_1pi[ 3] =  0.8;
      ctbins_1pi[ 4] =  0.85;
      ctbins_1pi[ 5] =  0.90;
      ctbins_1pi[ 6] =  0.92;
      ctbins_1pi[ 7] =  0.94;
      ctbins_1pi[ 8] =  0.96;
      ctbins_1pi[ 9] =  0.98;
      ctbins_1pi[10] =  0.99;
      ctbins_1pi[11] =  1.0;
    }else{
      nctbins_1pi = 19;
      ctbins_1pi[ 0] = -1.0;
      ctbins_1pi[ 1] = -0.6;
      ctbins_1pi[ 2] = -0.4;
      ctbins_1pi[ 3] = -0.2;
      ctbins_1pi[ 4] = -0.1;
      ctbins_1pi[ 5] =  0.;
      ctbins_1pi[ 6] =  0.1;
      ctbins_1pi[ 7] =  0.2;
      ctbins_1pi[ 8] =  0.4;
      ctbins_1pi[ 9] =  0.6;
      ctbins_1pi[10] =  0.7;
      ctbins_1pi[11] =  0.8;
      ctbins_1pi[12] =  0.85;
      ctbins_1pi[13] =  0.90;
      ctbins_1pi[14] =  0.92;
      ctbins_1pi[15] =  0.94;
      ctbins_1pi[16] =  0.96;
      ctbins_1pi[17] =  0.98;
      ctbins_1pi[18] =  0.99;
      ctbins_1pi[19] =  1.0;
    }

    TAxis* pAxis_1pi = new TAxis(npbins_1pi,pbins_1pi);
    TAxis* thAxis_1pi = new TAxis(nctbins_1pi, ctbins_1pi);
    TAxis** cc1pi_axes = new TAxis*[2];
    cc1pi_axes[0] = pAxis_1pi;
    cc1pi_axes[1] = thAxis_1pi;


    //Set the binning for the CC-Other sample.
    Int_t npbins_npi = 14;
    Double_t pbins_npi[15] = {0., 300., 400., 500., 600., 700., 800., 900., 1000., 1250., 1500., 2000., 3000., 5000., 30000.};
    Int_t nctbins_npi;
    Double_t ctbins_npi[25];
    if(!Do4PiFHC){
      nctbins_npi = 11;
      ctbins_npi[ 0] = -1.0;
      ctbins_npi[ 1] =  0.6;
      ctbins_npi[ 2] =  0.7;
      ctbins_npi[ 3] =  0.8;
      ctbins_npi[ 4] =  0.85;
      ctbins_npi[ 5] =  0.90;
      ctbins_npi[ 6] =  0.92;
      ctbins_npi[ 7] =  0.94;
      ctbins_npi[ 8] =  0.96;
      ctbins_npi[ 9] =  0.98;
      ctbins_npi[10] =  0.99;
      ctbins_npi[11] =  1.0;
    }else{
      nctbins_npi = 19;
      ctbins_npi[ 0] = -1.0;
      ctbins_npi[ 1] = -0.6;
      ctbins_npi[ 2] = -0.4;
      ctbins_npi[ 3] = -0.2;
      ctbins_npi[ 4] = -0.1;
      ctbins_npi[ 5] =  0.;
      ctbins_npi[ 6] =  0.1;
      ctbins_npi[ 7] =  0.2;
      ctbins_npi[ 8] =  0.4;
      ctbins_npi[ 9] =  0.6;
      ctbins_npi[10] =  0.7;
      ctbins_npi[11] =  0.8;
      ctbins_npi[12] =  0.85;
      ctbins_npi[13] =  0.90;
      ctbins_npi[14] =  0.92;
      ctbins_npi[15] =  0.94;
      ctbins_npi[16] =  0.96;
      ctbins_npi[17] =  0.98;
      ctbins_npi[18] =  0.99;
      ctbins_npi[19] =  1.0;
    }

    TAxis* pAxis_npi = new TAxis(npbins_npi,pbins_npi);
    TAxis* thAxis_npi = new TAxis(nctbins_npi, ctbins_npi);
    TAxis** ccnpi_axes = new TAxis*[2];
    ccnpi_axes[0] = pAxis_npi;
    ccnpi_axes[1] = thAxis_npi;

    //RHC anumu CC0pi
    Int_t npbins_anucc0pi = 13;
    Double_t pbins_anucc0pi[14] = {0., 300., 400., 500., 600., 700., 800., 900., 1000., 1250., 1500., 2000., 4000., 30000.};
    Int_t nctbins_anucc0pi = 10;
    Double_t ctbins_anucc0pi[11] = {-1., 0.7, 0.8, 0.85, 0.9, 0.94, 0.96, 0.98, 0.99, 0.995, 1.};

    TAxis* pAxis_anucc0pi = new TAxis(npbins_anucc0pi, pbins_anucc0pi);
    TAxis* thAxis_anucc0pi = new TAxis(nctbins_anucc0pi, ctbins_anucc0pi);
    TAxis** anucc0pi_axes = new TAxis*[2];
    anucc0pi_axes[0] = pAxis_anucc0pi;
    anucc0pi_axes[1] = thAxis_anucc0pi;

    //RHC anumu CC1pi
    Int_t npbins_anucc1pi = 8;
    Double_t pbins_anucc1pi[9] = {0., 400., 600., 800., 1000., 1250., 1500., 2500., 30000.};
    Int_t nctbins_anucc1pi = 4;
    Double_t ctbins_anucc1pi[5] = {-1, 0.8, 0.9, 0.96, 1};

    TAxis* pAxis_anucc1pi = new TAxis(npbins_anucc1pi, pbins_anucc1pi);
    TAxis* thAxis_anucc1pi = new TAxis(nctbins_anucc1pi, ctbins_anucc1pi);
    TAxis** anucc1pi_axes = new TAxis*[2];
    anucc1pi_axes[0] = pAxis_anucc1pi;
    anucc1pi_axes[1] = thAxis_anucc1pi;

    //RHC anumu CCOth
    Int_t npbins_anuccOth = 12;
    Double_t pbins_anuccOth[13] = {0., 350., 500., 600., 700., 800., 900., 1000., 1250., 1500., 2000., 4000., 30000.};
    Int_t nctbins_anuccOth = 5;
    Double_t ctbins_anuccOth[6] = {-1., 0.8, 0.9, 0.95, 0.98, 1.};

    TAxis* pAxis_anuccOth = new TAxis(npbins_anuccOth, pbins_anuccOth);
    TAxis* thAxis_anuccOth = new TAxis(nctbins_anuccOth, ctbins_anuccOth);
    TAxis** anuccOth_axes = new TAxis*[2];
    anuccOth_axes[0] = pAxis_anuccOth;
    anuccOth_axes[1] = thAxis_anuccOth;

    //RHC numu CC0pi
    Int_t npbins_nuccbkg0pi = 11;
    Double_t pbins_nuccbkg0pi[12] = {0., 350., 500., 650., 800., 900., 1000., 1250., 1500., 2000., 4000., 30000.};
    Int_t nctbins_nuccbkg0pi = 8;
    Double_t ctbins_nuccbkg0pi[9] = {-1, 0.75, 0.85, 0.9, 0.92, 0.94, 0.96, 0.98, 1};

    TAxis* pAxis_nuccbkg0pi = new TAxis(npbins_nuccbkg0pi, pbins_nuccbkg0pi);
    TAxis* thAxis_nuccbkg0pi = new TAxis(nctbins_nuccbkg0pi, ctbins_nuccbkg0pi);
    TAxis** nuccbkg0pi_axes = new TAxis*[2];
    nuccbkg0pi_axes[0] = pAxis_nuccbkg0pi;
    nuccbkg0pi_axes[1] = thAxis_nuccbkg0pi;

    //RHC numu CC1pi
    Int_t npbins_nuccbkg1pi =  8;
    Double_t pbins_nuccbkg1pi[9] = {0., 350., 500., 650., 800., 1000., 1250., 2000., 30000.};
    Int_t nctbins_nuccbkg1pi = 4;
    Double_t ctbins_nuccbkg1pi[5] = {-1, 0.8, 0.9, 0.95, 1.};

    TAxis* pAxis_nuccbkg1pi = new TAxis(npbins_nuccbkg1pi, pbins_nuccbkg1pi);
    TAxis* thAxis_nuccbkg1pi = new TAxis(nctbins_nuccbkg1pi, ctbins_nuccbkg1pi);
    TAxis** nuccbkg1pi_axes = new TAxis*[2];
    nuccbkg1pi_axes[0] = pAxis_nuccbkg1pi;
    nuccbkg1pi_axes[1] = thAxis_nuccbkg1pi;

    //RHC numu CCOth
    Int_t npbins_nuccbkgOth =  10;
    Double_t pbins_nuccbkgOth[11] = {0., 300., 450., 600., 800., 1000., 1250., 1500., 2000., 5000., 30000.};
    Int_t nctbins_nuccbkgOth = 8;
    Double_t ctbins_nuccbkgOth[9] = {-1., 0.7, 0.8, 0.86, 0.92, 0.94, 0.96, 0.98, 1.};

    TAxis* pAxis_nuccbkgOth = new TAxis(npbins_nuccbkgOth, pbins_nuccbkgOth);
    TAxis* thAxis_nuccbkgOth = new TAxis(nctbins_nuccbkgOth, ctbins_nuccbkgOth);
    TAxis** nuccbkgOth_axes = new TAxis*[2];
    nuccbkgOth_axes[0] = pAxis_nuccbkgOth;
    nuccbkgOth_axes[1] = thAxis_nuccbkgOth;

    //Anti-numu CCQE selection in anti-neutrino beam mode.
    Int_t npbins_anuccqe = 10;
    Double_t pbins_anuccqe[11] = {0., 400., 500., 600., 700., 800., 900., 1100., 1400., 2000., 10000.};

    Int_t nctbins_anuccqe = 13;
    Double_t ctbins_anuccqe[14] = {-1., 0.6, 0.7, 0.8, 0.85, 0.88, 0.91, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};

    TAxis* pAxis_anuccqe = new TAxis(npbins_anuccqe,pbins_anuccqe);
    TAxis* thAxis_anuccqe = new TAxis(nctbins_anuccqe, ctbins_anuccqe);
    TAxis** anuccqe_axes = new TAxis*[2];
    anuccqe_axes[0] = pAxis_anuccqe;
    anuccqe_axes[1] = thAxis_anuccqe;

    //Anti-numu CCnQE selection in anti-neutrino beam mode.
    Int_t npbins_anuccnqe = 7;
    Double_t pbins_anuccnqe[8] = {0., 700., 950., 1200., 1500., 2000., 3000., 10000.};

    Int_t nctbins_anuccnqe = 11;
    Double_t ctbins_anuccnqe[12] = {-1., 0.75, 0.85, 0.88, 0.91, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};

    TAxis* pAxis_anuccnqe = new TAxis(npbins_anuccnqe,pbins_anuccnqe);
    TAxis* thAxis_anuccnqe = new TAxis(nctbins_anuccnqe, ctbins_anuccnqe);
    TAxis** anuccnqe_axes = new TAxis*[2];
    anuccnqe_axes[0] = pAxis_anuccnqe;
    anuccnqe_axes[1] = thAxis_anuccnqe;

    //Numu CCQE selection in anti-neutrino beam mode.
    Int_t npbins_nuccqe = 6;
    Double_t pbins_nuccqe[7] = {0., 400., 600., 800., 1100., 2000., 10000.};

    Int_t nctbins_nuccqe = 11;
    Double_t ctbins_nuccqe[12] = {-1., 0.7, 0.8, 0.85, 0.90, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};

    TAxis* pAxis_nuccqe = new TAxis(npbins_nuccqe,pbins_nuccqe);
    TAxis* thAxis_nuccqe = new TAxis(nctbins_nuccqe, ctbins_nuccqe);
    TAxis** nuccqe_axes = new TAxis*[2];
    nuccqe_axes[0] = pAxis_nuccqe;
    nuccqe_axes[1] = thAxis_nuccqe;

    //Numu CCnQE selection in anti-neutrino beam mode.
    Int_t npbins_nuccnqe = 8;
    Double_t pbins_nuccnqe[9] = {0., 500., 700., 1000., 1250., 1500., 2000., 3000., 10000.};

    Int_t nctbins_nuccnqe = 11;
    Double_t ctbins_nuccnqe[12] = {-1., 0.7, 0.8, 0.85, 0.90, 0.93, 0.95, 0.96, 0.97, 0.98, 0.99, 1.00};

    TAxis* pAxis_nuccnqe = new TAxis(npbins_nuccnqe,pbins_nuccnqe);
    TAxis* thAxis_nuccnqe = new TAxis(nctbins_nuccnqe, ctbins_nuccnqe);
    TAxis** nuccnqe_axes = new TAxis*[2];
    nuccnqe_axes[0] = pAxis_nuccnqe;
    nuccnqe_axes[1] = thAxis_nuccnqe;

    //FHC nue
    int npbins_nue = 6;
    double pbins_nue[7] = {200., 400., 600., 800., 1000., 1500., 30000.};
    int nctbins_nue = 4;
    double ctbins_nue[5] = {-1., 0.8, 0.9, 0.95, 1.};

    TAxis* pAxis_nue = new TAxis(npbins_nue, pbins_nue);
    TAxis* thAxis_nue = new TAxis(nctbins_nue, ctbins_nue);
    TAxis** nue_axes = new TAxis*[2];
    nue_axes[0] = pAxis_nue;
    nue_axes[1] = thAxis_nue;

    //RHC nue
    int npbins_nuebkg = 6;
    double pbins_nuebkg[7] = {200., 400., 600., 800., 1000., 2000., 30000.};
    int nctbins_nuebkg = 2;
    double ctbins_nuebkg[3] = {-1., 0.9, 1.};

    TAxis* pAxis_nuebkg = new TAxis(npbins_nuebkg, pbins_nuebkg);
    TAxis* thAxis_nuebkg = new TAxis(nctbins_nuebkg, ctbins_nuebkg);
    TAxis** nuebkg_axes = new TAxis*[2];
    nuebkg_axes[0] = pAxis_nuebkg;
    nuebkg_axes[1] = thAxis_nuebkg;

    //RHC anue
    int npbins_anue = 3;
    double pbins_anue[4] = {200., 500., 1000., 30000.};
    int nctbins_anue = 2;
    double ctbins_anue[3] = {-1., 0.9, 1.};

    TAxis* pAxis_anue = new TAxis(npbins_anue, pbins_anue);
    TAxis* thAxis_anue = new TAxis(nctbins_anue, ctbins_anue);
    TAxis** anue_axes = new TAxis*[2];
    anue_axes[0] = pAxis_anue;
    anue_axes[1] = thAxis_anue;

    //FHC anue
    int npbins_anuebkg = 4;
    double pbins_anuebkg[5] = {200., 300., 400., 1000., 30000.};
    int nctbins_anuebkg = 1;
    double ctbins_anuebkg[2] = {-1., 1.};

    TAxis* pAxis_anuebkg = new TAxis(npbins_anuebkg, pbins_anuebkg);
    TAxis* thAxis_anuebkg = new TAxis(nctbins_anuebkg, ctbins_anuebkg);
    TAxis** anuebkg_axes = new TAxis*[2];
    anuebkg_axes[0] = pAxis_anuebkg;
    anuebkg_axes[1] = thAxis_anuebkg;


    //FHC gamma
    int npbins_gamma_fhc = 5;
    double pbins_gamma_fhc[6] = {200., 300., 400., 600., 1000., 30000.};
    int nctbins_gamma_fhc = 4;
    double ctbins_gamma_fhc[5] = {-1., 0.8, 0.95, 0.975, 1.};

    TAxis* pAxis_gamma_fhc = new TAxis(npbins_gamma_fhc, pbins_gamma_fhc);
    TAxis* thAxis_gamma_fhc = new TAxis(nctbins_gamma_fhc, ctbins_gamma_fhc);
    TAxis** gamma_fhc_axes = new TAxis*[2];
    gamma_fhc_axes[0] = pAxis_gamma_fhc;
    gamma_fhc_axes[1] = thAxis_gamma_fhc;


    //RHC gamma
    int npbins_gamma_rhc = 5;
    double pbins_gamma_rhc[6] = {200., 400., 600., 800., 1000., 30000.};
    int nctbins_gamma_rhc = 3;
    double ctbins_gamma_rhc[4] = {-1., 0.9, 0.95, 1.};

    TAxis* pAxis_gamma_rhc = new TAxis(npbins_gamma_rhc, pbins_gamma_rhc);
    TAxis* thAxis_gamma_rhc = new TAxis(nctbins_gamma_rhc, ctbins_gamma_rhc);
    TAxis** gamma_rhc_axes = new TAxis*[2];
    gamma_rhc_axes[0] = pAxis_gamma_rhc;
    gamma_rhc_axes[1] = thAxis_gamma_rhc;

    //Create the samples.
    //Supply the:
    //name
    //sampleID
    //Number of observables.
    //Array of observables.
    //Axes for the histograms.
    //Whether to throw MC statistical errors.
    //Whether to throw statistical errors
    bool throwMCStat = ND::params().GetParameterI("BANFF.RunFit.ThrowMCStat");
    bool throwStat = ND::params().GetParameterI("BANFF.RunFit.ThrowStat");

    // // FHC Numu selections
    BANFFBinnedSample* fhc_fgd1_numucc0pi;
    BANFFBinnedSample* fhc_fgd1_numucc1pi;
    BANFFBinnedSample* fhc_fgd1_numuccOth;
    BANFFBinnedSample* fhc_fgd2_numucc0pi;
    BANFFBinnedSample* fhc_fgd2_numucc1pi;
    BANFFBinnedSample* fhc_fgd2_numuccOth;

    // FHC Nue selections
    BANFFBinnedSample* fhc_fgd1_nuecc;
    BANFFBinnedSample* fhc_fgd1_anuecc;
    BANFFBinnedSample* fhc_fgd1_gamma;

    BANFFBinnedSample* fhc_fgd2_nuecc;
    BANFFBinnedSample* fhc_fgd2_anuecc;
    BANFFBinnedSample* fhc_fgd2_gamma;

    // RHC Nue selections
    BANFFBinnedSample* rhc_fgd1_nuecc;
    BANFFBinnedSample* rhc_fgd1_anuecc;
    BANFFBinnedSample* rhc_fgd1_gamma;

    BANFFBinnedSample* rhc_fgd2_nuecc;
    BANFFBinnedSample* rhc_fgd2_anuecc;
    BANFFBinnedSample* rhc_fgd2_gamma;

    // RHC Numu Multi Pi selections
    BANFFBinnedSample* rhc_fgd1_numucc0pi;
    BANFFBinnedSample* rhc_fgd1_numucc1pi;
    BANFFBinnedSample* rhc_fgd1_numuccOth;

    BANFFBinnedSample* rhc_fgd2_numucc0pi;
    BANFFBinnedSample* rhc_fgd2_numucc1pi;
    BANFFBinnedSample* rhc_fgd2_numuccOth;

    // RHC ANumu Multi Pi selections
    BANFFBinnedSample* rhc_fgd1_anumucc0pi;
    BANFFBinnedSample* rhc_fgd1_anumucc1pi;
    BANFFBinnedSample* rhc_fgd1_anumuccOth;

    BANFFBinnedSample* rhc_fgd2_anumucc0pi;
    BANFFBinnedSample* rhc_fgd2_anumucc1pi;
    BANFFBinnedSample* rhc_fgd2_anumuccOth;

    // RHC ANumu Multi Track selections
    BANFFBinnedSample* rhc_fgd1_anuccqe;
    BANFFBinnedSample* rhc_fgd1_anuccnqe;

    BANFFBinnedSample* rhc_fgd2_anuccqe;
    BANFFBinnedSample* rhc_fgd2_anuccnqe;

    // RHC Numu Multi Track selections
    BANFFBinnedSample* rhc_fgd1_nuccqe;
    BANFFBinnedSample* rhc_fgd1_nuccnqe;

    BANFFBinnedSample* rhc_fgd2_nuccqe;
    BANFFBinnedSample* rhc_fgd2_nuccnqe;

    int nSample = 6;
    bool DoNue        = (bool)ND::params().GetParameterI("BANFF.DoNueSelections");
    bool DoOnlyNue    = (bool)ND::params().GetParameterI("BANFF.DoOnlyNueSelections");
    bool DoMultiPiRHC = (bool)ND::params().GetParameterI("BANFF.DoMultiPiRHC");

    if(!Do4PiFHC && !DoOnlyNue)
    {
      std::cout << "---------------------------------" << std::endl;
      std::cout << "Applying FHC CCMultiPI selections" << std::endl;
      std::cout << "---------------------------------" << std::endl;

      // Some idiot proof check...
      if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPi") == 0 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPiFGD2") == 0)
      {
          std::cout << "FHC MultiPi is not activated in psycheSteering!" << std::endl;
          exit(1);
      }
      if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPi") == 1 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPiFGD2") == 1)
      {
          std::cout << "FHC 4Pi is activated in psycheSteering!" << std::endl;
          exit(1);
      }

      fhc_fgd1_numucc0pi= new BANFFBinnedSample("fhc_fgd1_numucc0pi", SampleId::kFGD1NuMuCC0Pi,   2, observables, cc0pi_axes, throwMCStat, throwStat);
      fhc_fgd1_numucc1pi= new BANFFBinnedSample("fhc_fgd1_numucc1pi", SampleId::kFGD1NuMuCC1Pi,   2, observables, cc1pi_axes, throwMCStat, throwStat);
      fhc_fgd1_numuccOth= new BANFFBinnedSample("fhc_fgd1_numuccOth", SampleId::kFGD1NuMuCCOther, 2, observables, ccnpi_axes, throwMCStat, throwStat);

      fhc_fgd2_numucc0pi= new BANFFBinnedSample("fhc_fgd2_numucc0pi", SampleId::kFGD2NuMuCC0Pi,   2, observables, cc0pi_axes, throwMCStat, throwStat);
      fhc_fgd2_numucc1pi= new BANFFBinnedSample("fhc_fgd2_numucc1pi", SampleId::kFGD2NuMuCC1Pi,   2, observables, cc1pi_axes, throwMCStat, throwStat);
      fhc_fgd2_numuccOth= new BANFFBinnedSample("fhc_fgd2_numuccOth", SampleId::kFGD2NuMuCCOther, 2, observables, ccnpi_axes, throwMCStat, throwStat);


    }
    else if(!DoOnlyNue)
    {
      std::cout << "-----------------------------" << std::endl;
      std::cout << "Applying CC4Pi FHC selections" << std::endl;
      std::cout << "-----------------------------" << std::endl;

      // Some idiot proof check...
      if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPi") == 1 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPiFGD2") == 1)
      {
          std::cout << "FHC MultiPi is activated in psycheSteering!" << std::endl;
          exit(1);
      }
      if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPi") == 0 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPiFGD2") == 0)
      {
          std::cout << "FHC 4Pi is not activated in psycheSteering!" << std::endl;
          exit(1);
      }
      fhc_fgd1_numucc0pi= new BANFFBinnedSample("fhc_fgd1_numucc0pi", SampleId::kFGD1NuMuCC0Pi,   2, observables, cc0pi_axes, throwMCStat, throwStat);
      fhc_fgd1_numucc1pi= new BANFFBinnedSample("fhc_fgd1_numucc1pi", SampleId::kFGD1NuMuCC1Pi,   2, observables, cc1pi_axes, throwMCStat, throwStat);
      fhc_fgd1_numuccOth= new BANFFBinnedSample("fhc_fgd1_numuccOth", SampleId::kFGD1NuMuCCOther, 2, observables, ccnpi_axes, throwMCStat, throwStat);

      fhc_fgd2_numucc0pi= new BANFFBinnedSample("fhc_fgd2_numucc0pi", SampleId::kFGD2NuMuCC0Pi,   2, observables, cc0pi_axes, throwMCStat, throwStat);
      fhc_fgd2_numucc1pi= new BANFFBinnedSample("fhc_fgd2_numucc1pi", SampleId::kFGD2NuMuCC1Pi,   2, observables, cc1pi_axes, throwMCStat, throwStat);
      fhc_fgd2_numuccOth= new BANFFBinnedSample("fhc_fgd2_numuccOth", SampleId::kFGD2NuMuCCOther, 2, observables, ccnpi_axes, throwMCStat, throwStat);
    }

    if(DoNue)
    {
      std::cout << "-----------------------" << std::endl;
      std::cout << "Applying nue selections" << std::endl;
      std::cout << "-----------------------" << std::endl;

      fhc_fgd1_nuecc  = new BANFFBinnedSample("fhc_fgd1_nuecc",  SampleId::kFGD1NuECC,                      2, observables, nue_axes,        throwMCStat, throwStat);
      fhc_fgd1_anuecc = new BANFFBinnedSample("fhc_fgd1_anuecc", SampleId::kFGD1AntiNuEBkgInNuModeCC,       2, observables, anuebkg_axes,    throwMCStat, throwStat);
      fhc_fgd1_gamma  = new BANFFBinnedSample("fhc_fgd1_gamma",  SampleId::kFGD1Gamma,                      2, observables, gamma_fhc_axes,  throwMCStat, throwStat);

      fhc_fgd2_nuecc  = new BANFFBinnedSample("fhc_fgd2_nuecc",  SampleId::kFGD2NuECC,                      2, observables, nue_axes,        throwMCStat, throwStat);
      fhc_fgd2_anuecc = new BANFFBinnedSample("fhc_fgd2_anuecc", SampleId::kFGD2AntiNuEBkgInNuModeCC,       2, observables, anuebkg_axes,    throwMCStat, throwStat);
      fhc_fgd2_gamma  = new BANFFBinnedSample("fhc_fgd2_gamma",  SampleId::kFGD2Gamma,                      2, observables, gamma_fhc_axes,  throwMCStat, throwStat);

      rhc_fgd1_nuecc  = new BANFFBinnedSample("rhc_fgd1_nuecc",  SampleId::kFGD1NuEBkgInAntiNuModeCC,       2, observables, nuebkg_axes,     throwMCStat, throwStat);
      rhc_fgd1_anuecc = new BANFFBinnedSample("rhc_fgd1_anuecc", SampleId::kFGD1AntiNuECC,                  2, observables, anue_axes,       throwMCStat, throwStat);
      rhc_fgd1_gamma  = new BANFFBinnedSample("rhc_fgd1_gamma",  SampleId::kFGD1GammaInAntiNuMode,          2, observables, gamma_rhc_axes,  throwMCStat, throwStat);

      rhc_fgd2_nuecc  = new BANFFBinnedSample("rhc_fgd2_nuecc",  SampleId::kFGD2NuEBkgInAntiNuModeCC,       2, observables, nuebkg_axes,     throwMCStat, throwStat);
      rhc_fgd2_anuecc = new BANFFBinnedSample("rhc_fgd2_anuecc", SampleId::kFGD2AntiNuECC,                  2, observables, anue_axes,       throwMCStat, throwStat);
      rhc_fgd2_gamma  = new BANFFBinnedSample("rhc_fgd2_gamma",  SampleId::kFGD2GammaInAntiNuMode,          2, observables, gamma_rhc_axes,  throwMCStat, throwStat);

      nSample=nSample+12;

      // Some idiot proof check...
      if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECC")         == 0 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECC")     == 0 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGamma")         == 0 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECCFGD2")     == 0 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECCFGD2") == 0 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGammaFGD2")     == 0)
      {
          std::cout << "Nue not activated in psycheSteering!" << std::endl;
          exit(1);
      }
    }
    else
    {
      // Some idiot proof check...
      if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECC")         == 1 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECC")     == 1 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGamma")         == 1 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNuECCFGD2")     == 1 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNuECCFGD2") == 1 ||
         ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerGammaFGD2")     == 1)
      {
          std::cout << "Nue is activated in psycheSteering!" << std::endl;
          exit(1);
      }
    }
    if(!DoOnlyNue)
    {
        if(DoMultiPiRHC)
        {
            std::cout << "------------------------------" << std::endl;
            std::cout << "Applying RHCMultiPi selections" << std::endl;
            std::cout << "------------------------------" << std::endl;

            rhc_fgd1_numucc0pi  = new BANFFBinnedSample("rhc_fgd1_numucc0pi",  SampleId::kFGD1NuMuBkgInAntiNuModeCC0Pi,   2, observables, nuccbkg0pi_axes, throwMCStat, throwStat);
            rhc_fgd1_numucc1pi  = new BANFFBinnedSample("rhc_fgd1_numucc1pi",  SampleId::kFGD1NuMuBkgInAntiNuModeCC1Pi,   2, observables, nuccbkg1pi_axes, throwMCStat, throwStat);
            rhc_fgd1_numuccOth  = new BANFFBinnedSample("rhc_fgd1_numuccOth",  SampleId::kFGD1NuMuBkgInAntiNuModeCCOther, 2, observables, nuccbkgOth_axes, throwMCStat, throwStat);

            rhc_fgd1_anumucc0pi = new BANFFBinnedSample("rhc_fgd1_anumucc0pi", SampleId::kFGD1AntiNuMuCC0Pi,              2, observables, anucc0pi_axes,   throwMCStat, throwStat);
            rhc_fgd1_anumucc1pi = new BANFFBinnedSample("rhc_fgd1_anumucc1pi", SampleId::kFGD1AntiNuMuCC1Pi,              2, observables, anucc1pi_axes,   throwMCStat, throwStat);
            rhc_fgd1_anumuccOth = new BANFFBinnedSample("rhc_fgd1_anumuccOth", SampleId::kFGD1AntiNuMuCCOther,            2, observables, anuccOth_axes,   throwMCStat, throwStat);

            rhc_fgd2_numucc0pi  = new BANFFBinnedSample("rhc_fgd2_numucc0pi",  SampleId::kFGD2NuMuBkgInAntiNuModeCC0Pi,   2, observables, nuccbkg0pi_axes, throwMCStat, throwStat);
            rhc_fgd2_numucc1pi  = new BANFFBinnedSample("rhc_fgd2_numucc1pi",  SampleId::kFGD2NuMuBkgInAntiNuModeCC1Pi,   2, observables, nuccbkg1pi_axes, throwMCStat, throwStat);
            rhc_fgd2_numuccOth  = new BANFFBinnedSample("rhc_fgd2_numuccOth",  SampleId::kFGD2NuMuBkgInAntiNuModeCCOther, 2, observables, nuccbkgOth_axes, throwMCStat, throwStat);

            rhc_fgd2_anumucc0pi = new BANFFBinnedSample("rhc_fgd2_anumucc0pi", SampleId::kFGD2AntiNuMuCC0Pi,              2, observables, anucc0pi_axes,   throwMCStat, throwStat);
            rhc_fgd2_anumucc1pi = new BANFFBinnedSample("rhc_fgd2_anumucc1pi", SampleId::kFGD2AntiNuMuCC1Pi,              2, observables, anucc1pi_axes,   throwMCStat, throwStat);
            rhc_fgd2_anumuccOth = new BANFFBinnedSample("rhc_fgd2_anumuccOth", SampleId::kFGD2AntiNuMuCCOther,            2, observables, anuccOth_axes,   throwMCStat, throwStat);

            nSample=nSample+12;

            // Some idiot proof check...
            if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPi")             == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPi")     == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPiFGD2")         == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPiFGD2") == 0)
            {
                std::cout << "MultiPi RHC not activated in psycheSteering!" << std::endl;
                exit(1);
            }
            // Some idiot proof check...
            if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrack")             == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrack")     == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrackFGD2")         == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrackFGD2") == 1)
            {
                std::cout << "MultiTrack RHC is activated in psycheSteering!" << std::endl;
                exit(1);
            }
        }
        else
        {
            std::cout << "--------------------------------" << std::endl;
            std::cout << "Applying RHCMultiTrack selection" << std::endl;
            std::cout << "--------------------------------" << std::endl;

            rhc_fgd1_anuccqe  = new BANFFBinnedSample("rhc_fgd1_anucc1tr", SampleId::kFGD1AntiNuMuCC1Track,             2, observables, anuccqe_axes,  throwMCStat, throwStat);
            rhc_fgd1_anuccnqe = new BANFFBinnedSample("rhc_fgd1_anuccntr", SampleId::kFGD1AntiNuMuCCNTracks,            2, observables, anuccnqe_axes, throwMCStat, throwStat);

            rhc_fgd1_nuccqe   = new BANFFBinnedSample("rhc_fgd1_nucc1tr",  SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track,  2, observables, nuccqe_axes,   throwMCStat, throwStat);
            rhc_fgd1_nuccnqe  = new BANFFBinnedSample("rhc_fgd1_nuccntr",  SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks, 2, observables, nuccnqe_axes,  throwMCStat, throwStat);

            rhc_fgd2_anuccqe  = new BANFFBinnedSample("rhc_fgd2_anucc1tr", SampleId::kFGD2AntiNuMuCC1Track,             2, observables, anuccqe_axes,  throwMCStat, throwStat);
            rhc_fgd2_anuccnqe = new BANFFBinnedSample("rhc_fgd2_anuccntr", SampleId::kFGD2AntiNuMuCCNTracks,            2, observables, anuccnqe_axes, throwMCStat, throwStat);

            rhc_fgd2_nuccqe   = new BANFFBinnedSample("rhc_fgd2_nucc1tr",  SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track,  2, observables, nuccqe_axes,   throwMCStat, throwStat);
            rhc_fgd2_nuccnqe  = new BANFFBinnedSample("rhc_fgd2_nuccntr",  SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks, 2, observables, nuccnqe_axes,  throwMCStat, throwStat);

            nSample=nSample+8;

            // Some idiot proof check...
            if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPi")             == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPi")     == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPiFGD2")         == 1 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPiFGD2") == 1)
            {
                std::cout << "MultiPi RHC is activated in psycheSteering!" << std::endl;
                exit(1);
            }
            // Some idiot proof check...
            if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrack")             == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrack")     == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrackFGD2")         == 0 ||
               ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrackFGD2") == 0)
            {
                std::cout << "MultiTrack RHC not activated in psycheSteering!" << std::endl;
                exit(1);
            }
        }
    }
    // ONLY NUE SELECTIONS
    else
    {
        nSample=12;
        // Some idiot proof check...
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPi")             == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPi")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiPiFGD2")         == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiPiFGD2") == 1)
        {
            std::cout << "MultiPi RHC is activated in psycheSteering!" << std::endl;
            exit(1);
        }
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrack")             == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrack")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerAntiNumuCCMultiTrackFGD2")         == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuInAntiNuModeCCMultiTrackFGD2") == 1)
        {
            std::cout << "MultiTrack RHC is activated in psycheSteering!" << std::endl;
            exit(1);
        }
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPi")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCCMultiPiFGD2") == 1)
        {
            std::cout << "FHC MultiPi is activated in psycheSteering!" << std::endl;
            exit(1);
        }
        if(ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPi")     == 1 ||
           ND::params().GetParameterI("psycheSteering.Selections.EnableTrackerNumuCC4piMultiPiFGD2") == 1)
        {
            std::cout << "FHC CC4pi multiPi is activated in psycheSteering!" << std::endl;
            exit(1);
        }
    }

    BANFFSampleBase** samples = new BANFFSampleBase*[nSample];
    int iSample = 0;
    if(!DoOnlyNue)
    {
        samples[iSample++] = fhc_fgd1_numucc0pi;
        samples[iSample++] = fhc_fgd1_numucc1pi;
        samples[iSample++] = fhc_fgd1_numuccOth;

        samples[iSample++] = fhc_fgd2_numucc0pi;
        samples[iSample++] = fhc_fgd2_numucc1pi;
        samples[iSample++] = fhc_fgd2_numuccOth;
    }

    if(DoNue)
    {
        samples[iSample++] = fhc_fgd1_nuecc;
        samples[iSample++] = fhc_fgd1_anuecc;
        samples[iSample++] = fhc_fgd1_gamma;

        samples[iSample++] = rhc_fgd1_nuecc;
        samples[iSample++] = rhc_fgd1_anuecc;
        samples[iSample++] = rhc_fgd1_gamma;

        samples[iSample++] = fhc_fgd2_nuecc;
        samples[iSample++] = fhc_fgd2_anuecc;
        samples[iSample++] = fhc_fgd2_gamma;

        samples[iSample++] = rhc_fgd2_nuecc;
        samples[iSample++] = rhc_fgd2_anuecc;
        samples[iSample++] = rhc_fgd2_gamma;
    }
    if(!DoOnlyNue)
    {
        if(DoMultiPiRHC)
        {
            samples[iSample++] = rhc_fgd1_numucc0pi;
            samples[iSample++] = rhc_fgd1_numucc1pi;
            samples[iSample++] = rhc_fgd1_numuccOth;

            samples[iSample++] = rhc_fgd1_anumucc0pi;
            samples[iSample++] = rhc_fgd1_anumucc1pi;
            samples[iSample++] = rhc_fgd1_anumuccOth;

            samples[iSample++] = rhc_fgd2_numucc0pi;
            samples[iSample++] = rhc_fgd2_numucc1pi;
            samples[iSample++] = rhc_fgd2_numuccOth;

            samples[iSample++] = rhc_fgd2_anumucc0pi;
            samples[iSample++] = rhc_fgd2_anumucc1pi;
            samples[iSample++] = rhc_fgd2_anumuccOth;

        }
        else
        {
            samples[iSample++] = rhc_fgd1_anuccqe;
            samples[iSample++] = rhc_fgd1_anuccnqe;

            samples[iSample++] = rhc_fgd1_nuccqe;
            samples[iSample++] = rhc_fgd1_nuccnqe;

            samples[iSample++] = rhc_fgd2_anuccqe;
            samples[iSample++] = rhc_fgd2_anuccnqe;

            samples[iSample++] = rhc_fgd2_nuccqe;
            samples[iSample++] = rhc_fgd2_nuccnqe;
        }
    }

    //With the samples ready, now define the parameters that are going to be
    //considered.
    //TODO: Again, maybe some of this can be moved to a backend?
    FitParameters* fitParameters = new FitParameters();

    //Load the Flux Parameters.
    //Specify to fit them.
    if(ND::params().GetParameterI("BANFF.RunFit.LoadFluxParams"))
    {
        fitParameters->LoadFluxParametersFromFile(
                ND::params().GetParameterS("BANFF.FluxInputFile"),
                ND::params().GetParameterI("BANFF.RunFit.LoadNuModeFluxParams"),
                ND::params().GetParameterI("BANFF.RunFit.LoadAntiNuModeFluxParams"),
                ND::params().GetParameterI("BANFF.RunFit.FitFluxParams"));
    }

    //Load the XSec Parameters and specify to fit them
    if(ND::params().GetParameterI("BANFF.RunFit.LoadXSecParams"))
    {
        fitParameters->LoadXSecParametersFromFile(
                ND::params().GetParameterS("BANFF.XSecInputFile"),
                ND::params().GetParameterI("BANFF.RunFit.FitXSecParams"));
    }

    //Load the ObsNorm parameters, and specify to fit them.
    if(ND::params().GetParameterI("BANFF.RunFit.LoadObsNormParams"))
    {
        fitParameters->LoadObsNormParametersFromFile(
                ND::params().GetParameterS("BANFF.ObsNormInputFile"),
                nSample, samples,
                ND::params().GetParameterI("BANFF.RunFit.FitObsNormParams"));
    }

    //We can save pre-fit information to a file here, since everything that has
    //been loaded will be.
    //Just make it iteration 0, save the names, and don't access the
    //undecomposed variable.

    std::string outputName = ND::params().GetParameterS("BANFF.RunFit.OutputFile");
    std::string PsycheVersion = anaUtils::GetSoftwareVersionFromPath((std::string)getenv("ND280PSYCHEROOT"));
    outputName = PsycheVersion+"_"+outputName;

    if(Do4PiFHC)
        outputName = "CC4PiFHC_"+outputName;
    if(DoNue)
        outputName = "NuE_"+outputName;
    if(!DoOnlyNue)
    {
        if(DoMultiPiRHC)
            outputName = "MultiPiRHC_"+outputName;
        else
            outputName = "MultiTrackRHC_"+outputName;
    }
    else
        outputName = "ONLY"+outputName;

    if(ND::params().GetParameterI("BANFF.SavePrefitParamsAndExit"))
    {
        TFile* preFitFile = new TFile(("prefit_"+outputName).c_str(),"RECREATE");
        preFitFile->cd();
        fitParameters->WriteResults(0, true, false);
        preFitFile->Close();
        exit(0);
    }

    //Now that all parameters are added, flag the flux and FSI parameters for
    //decomposition.
    for(int i = 0; i < fitParameters->GetNumbSysts(); i++)
    {

        if(fitParameters->GetParamType(i)==FLUX)
            fitParameters->SetDecompose(i,1);

        if(fitParameters->GetParamName(i).find("FSI")!=std::string::npos)
        {
            std::cout << "Decompose " << fitParameters->GetParamName(i) << std::endl;
            fitParameters->SetDecompose(i,1);
        }
    }

    //Create the psyche interface.  Do not load in or vary the detector
    //parameters from psyche.
    //NB: Although there are program parameters for this, it is something that
    //would never be done.

    std::string diagnostic_name = "diagnostic_" + outputName;

    psycheInterface* interface = new psycheInterface(nSample, samples, fitParameters, false, false, diagnostic_name);

    //Create the fitter that we want, and pass it the interface and the name of
    //the output file.
    Minuit2LikelihoodFit* fitter = new Minuit2LikelihoodFit(interface, outputName);

    //If requested in the program parameters file, fit to the data loaded in by
    //psyche.
    if(ND::params().GetParameterI("BANFF.RunFit.FitData"))
    {

        fitter->outputFile->cd();
        interface->BuildData();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++)
        {
            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName   = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);
        }

        if(ND::params().GetParameterI("BANFF.SavePrefitAndExit"))
            exit(0);

        fitter->InitializeParameters();
        fitter->DoFit();
    }

    //Otherwise, if the nominal MC flag is set, build the nominal MC then use
    //it as the data to fit.
    else if(ND::params().GetParameterI("BANFF.RunFit.FitNominalMC"))
    {

        fitter->outputFile->cd();
        interface->BuildNominalMC();

        for(int si = 0; si < interface->nSamples; si++)
        {
            samples[si]->UseNominalMCAsData();
            std::string dataName = interface->samples[si]->name + "_data";
            std::string mcName   = interface->samples[si]->name + "_prefit";
            samples[si]->SaveData(dataName);
            samples[si]->SaveNominalMC(mcName);
        }

        if(ND::params().GetParameterI("BANFF.SavePrefitAndExit"))
            exit(0);

        fitter->InitializeParameters();
        fitter->DoFit();
    }

    //Otherwise, fit to a number of toy MC histgorams in a file specified in
    //the program parameters file.
    //Taking advantage of this allows performing multiple validation fits
    //without having to spend time reloading the MC.
    else
    {
        //Set up the file containing the data.
        TFile* toyFile = new TFile((ND::params().GetParameterS("BANFF.RunFit.ToyFile")).c_str());

        //Loop over the toys.
        for(int i = ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit");
                i < ND::params().GetParameterI("BANFF.RunFit.FirstToyToFit") + ND::params().GetParameterI("BANFF.RunFit.NToysToFit");
                i++)
        {

            fitter->outputFile->cd();
            interface->BuildNominalMC();

            //Loop over the samples.
            for(int si = 0; si < interface->nSamples; si++)
            {
                std::stringstream histoStream;
                histoStream << samples[si]->name << "_predMC_" << i;

                //Load the predicted MC histogram in for this sample's data
                //histogram.
                samples[si]->LoadDataFromFile(toyFile,histoStream.str());

                std::string dataName = interface->samples[si]->name + "_data";
                std::string mcName = interface->samples[si]->name + "_prefit";
                samples[si]->SaveData(dataName);
                samples[si]->SaveNominalMC(mcName);
            }
            if(ND::params().GetParameterI("BANFF.SavePrefitAndExit"))
                exit(0);
            fitter->InitializeParameters();

            //DoFit has an internal counter that will automatically iterate the
            //names of the fit output to fit multiple toys and save the output
            //to the same file..
            fitter->DoFit();
        }

        toyFile->Close();
    }
}
