#include "THn.h"
#include "TFile.h"
#include "TObjString.h"
#include "TH1F.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include <iostream>
#include "TKey.h"
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"


///Takes in a toy file and makes projections of data, nominal MC, and mean MC
///in each dimension.  The error on the meanMC histogram is equal to the
///standard deviation of the toys.
///Usage:ProcessToys.exe inputFile outputFile
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

    //Set up arrays to hold the mean, nominal, and data histograms.
    THnD** mean = new THnD*[nSamples];
    THnD** nom = new THnD*[nSamples];
    THnD** data = new THnD*[nSamples];

    //Load the MC predictions for each toy into memory.
    THnD*** predMC = new THnD**[nSamples];

    //For each sample, add the first toy to the mean histogram.
    //While we're looping through samples, might as well get the nominal histogram
    //too.  Also, data.
    for(int i = 0; i < nSamples; i++){

        TString histoName = ((TObjString*)(sampleNames->At(i)))->GetString() + "_predMC_";
        histoName += 0;

        mean[i] = (THnD*)(inputFile->Get(histoName.Data()));

        //Create the array of THnD* of length nToys for this sample.
        predMC[i] = new THnD*[nToys];

        //Need to get a fresh copy of the first toy to put in here.
        predMC[i][0] = (THnD*)(inputFile->Get(histoName.Data()));


        histoName = ((TObjString*)(sampleNames->At(i)))->GetString() + "_nomMC";
        nom[i] = (THnD*)(inputFile->Get(histoName.Data()));

        histoName = ((TObjString*)(sampleNames->At(i)))->GetString() + "_data";
        data[i] = (THnD*)(inputFile->Get(histoName.Data()));

    }
    
    //Now, loop though the remaining toys and all samples, and add the histogram for that toy to the mean
    //histogram.
    for(int ti = 1; ti < nToys; ti++){

        //std::cout << "Now loading toy: " << ti << std::endl;
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
        
    }
    

    //Create the output file and cd to it.
    TFile* outputFile = new TFile(argv[2],"RECREATE");
    outputFile->cd();

    //Set up arrays to store the histograms we're saving.
    TH1D*** dataProj = new TH1D**[nSamples];
    TH1D*** nomProj = new TH1D**[nSamples];
    TH1D*** meanProj = new TH1D**[nSamples];

    for(int si = 0; si < nSamples; si++){

        int nObs = mean[si]->GetNdimensions();
        dataProj[si] = new TH1D*[nObs];
        nomProj[si] = new TH1D*[nObs];
        meanProj[si] = new TH1D*[nObs];

        for(int oi = 0; oi < nObs; oi++){

            dataProj[si][oi] = data[si]->Projection(oi);
            dataProj[si][oi]->SetName(Form("data%d%d",si,oi));
            
            nomProj[si][oi] = nom[si]->Projection(oi);
            nomProj[si][oi]->SetName(Form("nomMC%d%d",si,oi));

            meanProj[si][oi] = mean[si]->Projection(oi);
            meanProj[si][oi]->SetName(Form("meanMC%d%d",si,oi));

            //Initialize all bin errors on meanProj to zero, so we can use it
            //to compute the standard deviation from the toys.
            for(int bi = 1; bi < meanProj[si][oi]->GetNbinsX() + 1; bi++){

                meanProj[si][oi]->SetBinError(bi,0.0);
            }
                
            //Now loop through all the toys and use the bin error to
            //compute the standard deviation.
            for(int ti = 0; ti < nToys; ti++){

                TH1D* tmpPredProj = predMC[si][ti]->Projection(oi);

                for(int bi = 1; bi < meanProj[si][oi]->GetNbinsX() + 1; bi++){

                    double predBin = tmpPredProj->GetBinContent(bi);
                    double meanBin = meanProj[si][oi]->GetBinContent(bi);


                    meanProj[si][oi]->SetBinError(bi,meanProj[si][oi]->GetBinError(bi) + pow(predBin-meanBin,2));
                }

                //Now that we're done with this toy, delete the projected
                //histogram.
                tmpPredProj->Delete();
            }

            //Once all toys have been looped through, divide the BinError by
            //the number of toys and take the square root in order to make it
            //the standard deviation of the toys.
             for(int bi = 1; bi < meanProj[si][oi]->GetNbinsX() + 1; bi++){

                meanProj[si][oi]->SetBinError(bi,TMath::Sqrt((meanProj[si][oi]->GetBinError(bi))/nToys));
            }
        }//End loop over observables.

    }//End loop over samples.


    //All the graphs are now made.  Write out the root file.
    outputFile->Write();
    outputFile->Close();
}
