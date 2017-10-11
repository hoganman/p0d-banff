///An app that reads in a file full of toys thrown with binned samples and produces
///a file which the code can read in and create observable normalization
///parameters from.  Modelled after make_covariance_v2.cc in the BANFFv2 inputs
///directory, altered for the specifics of the BANFFBinnedSample format and
//BANFFInterfaceBase output specific features.
#include "THn.h"
#include "TFile.h"
#include "TObjString.h"
#include "TH1F.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include <iostream>
#include "TKey.h"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"

///Specify the name of the toy file as the first argument, and the desired name
///of the output file as the second argument.
int main(int argc, char** argv){

    if(argc != 3){

        std::cout << "Warning: Unexpected number of arguments." << std::endl;
    }

    TFile* inputFile = new TFile(argv[1]);


    //Extract the number of toys from the input file.
    TObject* numberOfToys = (TObject*)(inputFile->Get("nToys"));
    Int_t nToys = numberOfToys->GetUniqueID();

    //Next extract the sample information from the input file.
    TObjArray* sampleNames = (TObjArray*)(inputFile->Get("sampleNames"));
    Int_t nSamples = sampleNames->GetEntries();

    //Set up arrays to hold the mean and nominal histograms.
    THnD** mean = new THnD*[nSamples];
    THnD** nom = new THnD*[nSamples];
   
    //Load the MC predictions for each toy into memory.
    THnD*** predMC = new THnD**[nSamples];

    //For each sample, add the first toy to the mean histogram.
    //While we're looping through samples, might as well get the nominal histogram
    //too.
    for(int i = 0; i < nSamples; i++){

        TString histoName = ((TObjString*)(sampleNames->At(i)))->GetString() + "_predMC_";
        histoName += 0;

        mean[i] = (THnD*)(inputFile->Get(histoName.Data()));

        histoName = ((TObjString*)(sampleNames->At(i)))->GetString() + "_nomMC";
        nom[i] = (THnD*)(inputFile->Get(histoName.Data()));

        //Create the array of THnD* of length nToys for this sample.
        predMC[i] = new THnD*[nToys];

        //Need to get a fresh copy of the first toy to put in here.
        predMC[i][0] = (THnD*)(inputFile->Get(histoName.Data()));

    }
    
    //Now, loop though the remaining toys and all samples, and add the histogram for that toy to the mean
    //histogram.
    for(int ti = 1; ti < nToys; ti++){

        for(int si = 0; si < nSamples; si++){

            TString histoName = ((TObjString*)(sampleNames->At(si)))->GetString() + "_predMC_";
            histoName += ti;

            THnD* tmp = (THnD*)(inputFile->Get(histoName.Data()));
            predMC[si][ti] = tmp;
            mean[si]->Add(tmp);


        }

    }

    //With the histograms from all toys added in, divide each sample's mean
    //histogram by the number of toys in order to have the mean of the toys.
    //Also set up the covariance matrix for the Obs Norm parameters.  The
    //dimension is equal to the sum of the number of bins of each sample's
    //histogram (since the Obs Norm parameters will have the same binning used
    //for the toy throwing.)
    Int_t nBins = 0;
    for(int i = 0; i < nSamples; i++){

        mean[i]->Scale(1/(Double_t)nToys);
        nBins += BANFFBinnedSampleUtils::GetNBins(mean[i]);
        //std::cout << "Number of bins in sample: " << BANFFBinnedSampleUtils::GetNBins(mean[i]) << std::endl;
    }

    //The covariance matrix.
    TMatrixDSym* cov = new TMatrixDSym(nBins);

    //A 1D histogram of the weights that weight nominal MC to the mean of the
    //toys.  This is for easy visualization of the TVectorD contents as a
    //group.
    TH1F* hist_weights = new TH1F("hist_weights","",50,0.5,1.5);

    //A vector of all the weights that will be used as the prior values for the
    //ObsNorm parameters.
    TVectorD* weights = new TVectorD(cov->GetNrows());

    //Loop over all the toys.
    for(int ti = 0; ti < nToys; ti++){

        //For each toy we need to count all the bins across all samples.  Do
        //that with this variable.
        int iter1 = 0;

        //Loop over the samples.
        for(int si1 = 0; si1 < nSamples; si1++){

            //Get this sample's predMC histogram.
            //TString histoName1 = ((TObjString*)(sampleNames->At(si1)))->GetString() + "_predMC_";
            //histoName1 += ti;
            //THnD* tmp1 = (THnD*)(inputFile->Get(histoName1.Data()));

            //Loop over all the bins in this sample's predMC histogram that are
            //not overflow or underflow.
            //Get the array of bin indices first.
            int* bins1 = BANFFBinnedSampleUtils::GetBins(predMC[si1][ti]);
            for(int bi1 = 0; bi1 < BANFFBinnedSampleUtils::GetNBins(predMC[si1][ti]); bi1++){

                double delta1 = (1.0 - (predMC[si1][ti]->GetBinContent(bins1[bi1])/mean[si1]->GetBinContent(bins1[bi1])));

                //If the bin content we just tried to divide by was zero, print
                //out the information for the bin.
                if(mean[si1]->GetBinContent(bins1[bi1]) == 0){

                    std::cout << "Sample " + ((TObjString*)(sampleNames->At(si1)))->GetString() 
                        + " bin " << bins1[bi1] << " has zero content!" << std::endl;

                }

                //We also need to find the weight to go from nominal MC to the
                //mean of the predicted MC.
                //This is where it is important that psycheSteering be
                //configured the same for the toy throwing and the fit, because
                //nominal MC needs to have its weights applied the same way for
                //the toy throw and the fit, in case any of the systematics
                //incorporates a "correction" component at its nominal throw
                //value.
                //Only need to do this once, so do it on toy 0.
                if(ti == 0){

                    (*weights)(iter1) = mean[si1]->GetBinContent(bins1[bi1])/nom[si1]->GetBinContent(bins1[bi1]);

                    //std::cout << (*weights)(iter1) << std::endl;
                    hist_weights->Fill((*weights)(iter1));
                }


                //Now we need to loop through all samples and bins to match
                //with this one.
                int iter2 = 0;
                for(int si2 = 0; si2 < nSamples; si2++){

                    //Get this sample's predMC histogram.
                    //TString histoName2 = ((TObjString*)(sampleNames->At(si2)))->GetString() + "_predMC_";
                    //histoName2 += ti;
                    //THnD* tmp2 = (THnD*)(inputFile->Get(histoName2.Data()));

                    //Loop over all the bins in this sample's predMC histogram.
                    //Loop over the special array that only contains the
                    //indices of bins that are not underflow or overflow.
                    int* bins2 = BANFFBinnedSampleUtils::GetBins(predMC[si2][ti]);
                    for(int bi2 = 0; bi2 < BANFFBinnedSampleUtils::GetNBins(predMC[si2][ti]); bi2++){


                        double delta2 = (1.0 - (predMC[si2][ti]->GetBinContent(bins2[bi2])/mean[si2]->GetBinContent(bins2[bi2])));

                        //Compute the corresponding covariance matrix entry.
                        (*cov)(iter1,iter2) += delta1*delta2/((Double_t)nToys);

                        iter2++;

                    } //Close 2nd loop over bins.

                    //Since we're done with this histogram now, delete the
                    //pointer to the bins array.
                    delete[] bins2;


                } //Close 2nd loop over samples.

                iter1++; 

            } //Close 1st loop over bins.

            //Since we're done with the histogram bins now, delete the pointer
            //to the bins array.
            delete[] bins1;

        } //Close 1st loop over samples.


    } //Close loop over toys.

    //Having now created everything that needs to be saved, create the output file.
    TFile* outputFile = new TFile(argv[2],"RECREATE");
    cov->Write("obsNorm_cov");
    weights->Write("obsNorm_weights");
    hist_weights->Write();

    //Loop through the input file and look for any keys with "axis" in the
    //name.  Copy them over to the output file.
    TList* keys = inputFile->GetListOfKeys();
    Int_t nKeys = inputFile->GetNkeys();


    //Check for bad entries in the covariance matrix.
    for(int i = 0; i < nBins; i++){


        for(int j = 0; j < nBins; j++){

            if((*cov)(i,j) != (*cov)(i,j)){

                std::cout << "Entry NAN: " << i << " " << j << std::endl;

            }

        }

    }


    for(int i = 0; i < nKeys; i++){

        TKey* tmpKey = (TKey*)(keys->At(i));

        //The object is accessed via the key's name.
        std::string keyName(tmpKey->GetName());

        //If "_axis" is in the key name, retrieve it from the input file and then
        //save it to the output file.
        if(keyName.find("_axis") != std::string::npos){

            TAxis* tmpAxis = (TAxis*)inputFile->Get(keyName.c_str());
            outputFile->cd();
            tmpAxis->Write(keyName.c_str());
        }
    }

    //Now that we're done with the output file, close it.
    outputFile->Close();

    return 0;
}
