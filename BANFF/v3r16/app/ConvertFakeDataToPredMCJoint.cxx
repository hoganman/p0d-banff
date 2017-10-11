#include "psycheInterface/psycheInterface.hxx"
#include "BANFFSample/BANFFBinnedSample.hxx"
#include "psycheInterface/psycheObservables.hxx"
#include "BANFFObservableBase.hxx"
#include "FitParameters.hxx"
#include "Parameters.hxx"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include "Minuit2LikelihoodFit/Minuit2LikelihoodFit.cxx"
#include <sstream>
#include "TH2F.h"

//Set up the samples and observables with proper binning, then load the
//parameters and perform the fit.
//argv[1] is the input file.
//argv[2] is the output file.
int main(int argc, char** argv){

    //Open the file with the fake data in it.
    TFile* fakeDataFile = new TFile(argv[1]);

    //Create the output file.
    TFile* outputFile = new TFile(argv[2],"RECREATE");

    outputFile->cd();
    //Create the observables we will use.
    psycheLeptonCandidateMomentum* pmu = new psycheLeptonCandidateMomentum();
    psycheLeptonCandidateCosTheta* thmu = new psycheLeptonCandidateCosTheta();

    BANFFObservableBase** observables = new BANFFObservableBase*[2];
    observables[0] = pmu;
    observables[1] = thmu;


    //For each observable and sample set up the binning used in the fit.

    //Set the binning for the CC0pi sample.
    Int_t npbins_0pi = 14;
    Double_t pbins_0pi[15] = {0.,300.,400.,500.,600.,700.,800.,900.,1000.,1250.,1500.,2000.,3000.,5000.,30000.};
    Int_t nctbins_0pi = 11;
    Double_t ctbins_0pi[12] = {-1.0,0.6,0.7,0.8,0.85,0.90,0.92,0.94,0.96,0.98,0.99,1.0};

    TAxis* pAxis_0pi = new TAxis(npbins_0pi,pbins_0pi);
    TAxis* thAxis_0pi = new TAxis(nctbins_0pi, ctbins_0pi);
    TAxis** cc0pi_axes = new TAxis*[2];
    cc0pi_axes[0] = pAxis_0pi;
    cc0pi_axes[1] = thAxis_0pi;


    //Set the binning for the CC1pi sample.
    Int_t npbins_1pi = 13;
    Double_t pbins_1pi[14] = {0.,300.,400.,500.,600.,700.,800.,900.,1000.,1250.,1500.,2000.,5000.,30000.};
    Int_t nctbins_1pi = 11;
    Double_t ctbins_1pi[12] = {-1.0,0.6,0.7,0.8,0.85,0.90,0.92,0.94,0.96,0.98,0.99,1.0}; 

    TAxis* pAxis_1pi = new TAxis(npbins_1pi,pbins_1pi);
    TAxis* thAxis_1pi = new TAxis(nctbins_1pi, ctbins_1pi);
    TAxis** cc1pi_axes = new TAxis*[2];
    cc1pi_axes[0] = pAxis_1pi;
    cc1pi_axes[1] = thAxis_1pi;


    //Set the binning for the CC-Other sample.
    Int_t npbins_npi = 14;
    Double_t pbins_npi[15] = {0.,300.,400.,500.,600.,700.,800.,900.,1000.,1250.,1500.,2000.,3000.,5000.,30000.};
    Int_t nctbins_npi = 11;
    Double_t ctbins_npi[12] = {-1.0,0.6,0.7,0.8,0.85,0.90,0.92,0.94,0.96,0.98,0.99,1.0};

    TAxis* pAxis_npi = new TAxis(npbins_npi,pbins_npi);
    TAxis* thAxis_npi = new TAxis(nctbins_npi, ctbins_npi);
    TAxis** ccnpi_axes = new TAxis*[2];
    ccnpi_axes[0] = pAxis_npi;
    ccnpi_axes[1] = thAxis_npi;

    //Anti-numu CCQE selection in anti-neutrino beam mode.
    Int_t npbins_anuccqe = 5;
    Double_t pbins_anuccqe[6] = {0., 500., 900., 1200., 2000., 10000.};

    Int_t nctbins_anuccqe = 4;
    Double_t ctbins_anuccqe[5] = {-1.0, 0.8, 0.92, 0.98, 1.0};

    TAxis* pAxis_anuccqe = new TAxis(npbins_anuccqe,pbins_anuccqe);
    TAxis* thAxis_anuccqe = new TAxis(nctbins_anuccqe, ctbins_anuccqe);
    TAxis** anuccqe_axes = new TAxis*[2];
    anuccqe_axes[0] = pAxis_anuccqe;
    anuccqe_axes[1] = thAxis_anuccqe;

    //Anti-numu CCnQE selection in anti-neutrino beam mode.
    Int_t npbins_anuccnqe = 5;
    Double_t pbins_anuccnqe[6] = {0., 600., 1000., 1500., 2200., 10000.};

    Int_t nctbins_anuccnqe = 4;
    Double_t ctbins_anuccnqe[5] = {-1.0, 0.8, 0.9, 0.97, 1.0};

    TAxis* pAxis_anuccnqe = new TAxis(npbins_anuccnqe,pbins_anuccnqe);
    TAxis* thAxis_anuccnqe = new TAxis(nctbins_anuccnqe, ctbins_anuccnqe);
    TAxis** anuccnqe_axes = new TAxis*[2];
    anuccnqe_axes[0] = pAxis_anuccnqe;
    anuccnqe_axes[1] = thAxis_anuccnqe;

    //Numu CCQE selection in anti-neutrino beam mode.
    Int_t npbins_nuccqe = 5;
    Double_t pbins_nuccqe[6] = {0., 500., 900., 1200., 2000., 10000.};

    Int_t nctbins_nuccqe = 4;
    Double_t ctbins_nuccqe[5] = {-1.0, 0.8, 0.92, 0.98, 1.0};

    TAxis* pAxis_nuccqe = new TAxis(npbins_nuccqe,pbins_nuccqe);
    TAxis* thAxis_nuccqe = new TAxis(nctbins_nuccqe, ctbins_nuccqe);
    TAxis** nuccqe_axes = new TAxis*[2];
    nuccqe_axes[0] = pAxis_nuccqe;
    nuccqe_axes[1] = thAxis_nuccqe;

    //Numu CCnQE selection in anti-neutrino beam mode.
    Int_t npbins_nuccnqe = 5;
    Double_t pbins_nuccnqe[6] = {0., 600., 1000., 1500., 2200., 10000.};

    Int_t nctbins_nuccnqe = 4;
    Double_t ctbins_nuccnqe[5] = {-1.0, 0.8, 0.9, 0.97, 1.0};

    TAxis* pAxis_nuccnqe = new TAxis(npbins_nuccnqe,pbins_nuccnqe);
    TAxis* thAxis_nuccnqe = new TAxis(nctbins_nuccnqe, ctbins_nuccnqe);
    TAxis** nuccnqe_axes = new TAxis*[2];
    nuccnqe_axes[0] = pAxis_nuccnqe;
    nuccnqe_axes[1] = thAxis_nuccnqe;

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
    BANFFBinnedSample* fgd1_cc0pi = new BANFFBinnedSample("fgd1cc0pi", SampleId::kFGD1NuMuCC0Pi, 2, observables, cc0pi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd1_cc1pi = new BANFFBinnedSample("fgd1cc1pi", SampleId::kFGD1NuMuCC1Pi, 2, observables, cc1pi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd1_ccOth = new BANFFBinnedSample("fgd1ccOth", SampleId::kFGD1NuMuCCOther, 2, observables, ccnpi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd1_anuccqe = new BANFFBinnedSample("fgd1anucc1tr", SampleId::kFGD1AntiNuMuCC1Track, 2, observables, anuccqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd1_anuccnqe = new BANFFBinnedSample("fgd1anuccntr", SampleId::kFGD1AntiNuMuCCNTracks, 2, observables, anuccnqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd1_nuccqe = new BANFFBinnedSample("fgd1nucc1tr", SampleId::kFGD1NuMuBkgInAntiNuModeCC1Track, 2, observables, nuccqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd1_nuccnqe = new BANFFBinnedSample("fgd1nuccntr", SampleId::kFGD1NuMuBkgInAntiNuModeCCNTracks, 2, observables, nuccnqe_axes, throwMCStat, throwStat);
    
    BANFFBinnedSample* fgd2_cc0pi = new BANFFBinnedSample("fgd2cc0pi", SampleId::kFGD2NuMuCC0Pi, 2, observables, cc0pi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd2_cc1pi = new BANFFBinnedSample("fgd2cc1pi", SampleId::kFGD2NuMuCC1Pi, 2, observables, cc1pi_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd2_ccOth = new BANFFBinnedSample("fgd2ccOth", SampleId::kFGD2NuMuCCOther, 2, observables, ccnpi_axes, throwMCStat, throwStat);	
    BANFFBinnedSample* fgd2_anuccqe = new BANFFBinnedSample("fgd2anucc1tr", SampleId::kFGD2AntiNuMuCC1Track, 2, observables, anuccqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd2_anuccnqe = new BANFFBinnedSample("fgd2anuccntr", SampleId::kFGD2AntiNuMuCCNTracks, 2, observables, anuccnqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd2_nuccqe = new BANFFBinnedSample("fgd2nucc1tr", SampleId::kFGD2NuMuBkgInAntiNuModeCC1Track, 2, observables, nuccqe_axes, throwMCStat, throwStat);
    BANFFBinnedSample* fgd2_nuccnqe = new BANFFBinnedSample("fgd2nuccntr", SampleId::kFGD2NuMuBkgInAntiNuModeCCNTracks, 2, observables, nuccnqe_axes, throwMCStat, throwStat);
    

    BANFFBinnedSample** samples = new BANFFBinnedSample*[14];
    samples[0] = fgd1_cc0pi;
    samples[1] = fgd1_cc1pi;
    samples[2] = fgd1_ccOth;
    samples[3] = fgd1_anuccqe;
    samples[4] = fgd1_anuccnqe;
    samples[5] = fgd1_nuccqe;
    samples[6] = fgd1_nuccnqe;
    samples[7] = fgd2_cc0pi;
    samples[8] = fgd2_cc1pi;
    samples[9] = fgd2_ccOth;	
    samples[10] = fgd2_anuccqe;
    samples[11] = fgd2_anuccnqe;
    samples[12] = fgd2_nuccqe;
    samples[13] = fgd2_nuccnqe;


    //Fake Data histogram array.
    TH2F** fakeData = new TH2F*[14];
    fakeData[0] = (TH2F*)(fakeDataFile->Get("fgd1_tuned_cc0pi"));
    fakeData[1] = (TH2F*)(fakeDataFile->Get("fgd1_tuned_cc1pi"));
    fakeData[2] = (TH2F*)(fakeDataFile->Get("fgd1_tuned_ccNpi"));
    fakeData[3] = (TH2F*)(fakeDataFile->Get("fgd1_tuned_acc1tr"));
    fakeData[4] = (TH2F*)(fakeDataFile->Get("fgd1_tuned_accNtr"));
    fakeData[5] = (TH2F*)(fakeDataFile->Get("fgd1_tuned_cc1tr"));
    fakeData[6] = (TH2F*)(fakeDataFile->Get("fgd1_tuned_ccNtr"));
    fakeData[7] = (TH2F*)(fakeDataFile->Get("fgd2_tuned_cc0pi"));
    fakeData[8] = (TH2F*)(fakeDataFile->Get("fgd2_tuned_cc1pi"));
    fakeData[9] = (TH2F*)(fakeDataFile->Get("fgd2_tuned_ccNpi"));
    fakeData[10] = (TH2F*)(fakeDataFile->Get("fgd2_tuned_acc1tr"));
    fakeData[11] = (TH2F*)(fakeDataFile->Get("fgd2_tuned_accNtr"));
    fakeData[12] = (TH2F*)(fakeDataFile->Get("fgd2_tuned_cc1tr"));
    fakeData[13] = (TH2F*)(fakeDataFile->Get("fgd2_tuned_ccNtr")); 
 
    //We're including overflow and underflow.
    Double_t binCentre[2];

    //Loop over all the samples.
    for(int si = 0; si < 14; si++){

        //Loop over the X bins of the TH2Fs.
        for(int i = 0; i < fakeData[si]->GetNbinsX() + 1; i++){

            //Loop over the Y bins of the TH2Fs.
            for(int j = 0; j < fakeData[si]->GetNbinsY() + 1; j++){


                binCentre[0] = fakeData[si]->GetXaxis()->GetBinCenter(i);
                binCentre[1] = fakeData[si]->GetYaxis()->GetBinCenter(j);

                Long64_t bin = samples[si]->data->GetBin(binCentre);
                samples[si]->data->SetBinContent(bin,fakeData[si]->GetBinContent(i,j));
            }
        }

        //Now that all the bins have been looped over, set the overall number
        //of entries for the THnD to equal that of the TH2F.
        samples[si]->data->SetEntries(fakeData[si]->GetEntries());

        //Perform a check on the number of events in each sample.  Will do a
        //visible comparison later as well.
        if(fakeData[si]->GetSumOfWeights() != samples[si]->data->Projection(1,0)->GetSumOfWeights()){
            std::cout << "ERROR: Fake data content mismatch" << std::endl;
            std::cout << "Fake data input: " << fakeData[si]->GetSumOfWeights() << std::endl;
            std::cout << "Fake data output: " << samples[si]->data->Projection(1,0)->GetSumOfWeights() << std::endl;
        }
    }

    //Write out the completed histograms.
    outputFile->cd();
    samples[0]->data->Write("fgd1cc0pi_predMC_0");
    samples[1]->data->Write("fgd1cc1pi_predMC_0");
    samples[2]->data->Write("fgd1ccOth_predMC_0");
    samples[3]->data->Write("fgd1anucc1tr_predMC_0");
    samples[4]->data->Write("fgd1anuccntr_predMC_0");
    samples[5]->data->Write("fgd1nucc1tr_predMC_0");
    samples[6]->data->Write("fgd1nuccntr_predMC_0");
    samples[7]->data->Write("fgd2cc0pi_predMC_0");
    samples[8]->data->Write("fgd2cc1pi_predMC_0");
    samples[9]->data->Write("fgd2ccOth_predMC_0");
    samples[10]->data->Write("fgd2anucc1tr_predMC_0");
    samples[11]->data->Write("fgd2anuccntr_predMC_0");
    samples[12]->data->Write("fgd2nucc1tr_predMC_0");
    samples[13]->data->Write("fgd2nuccntr_predMC_0");    

    //Having finished the task at hand, close the output file.
    outputFile->Close();

}
