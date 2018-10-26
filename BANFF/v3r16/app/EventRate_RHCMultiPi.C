#include <iostream>
#include <vector>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "THn.h"
#include <iostream>
#include <iomanip>

int GetNBins(THn* hist){

    //Loop through, multiplying the number of bins in each axis together, to
    //get the total number of bins in the n dimensional histogram.
    int nDim  = hist->GetNdimensions();
    int nBins = 1;

    for(int i = 0; i < nDim; i++){
        nBins *= hist->GetAxis(i)->GetNbins();
    }

    //With all axes looped over, have the full bin count.  Return the result.
    return nBins;
}

int* GetBins(THn* hist){

    //Extract the axes from the histogram, some information we will find useful
    //later, and information for the first bin we will consider.
    int     nDim       = hist->GetNdimensions();
    TAxis** axes       = new TAxis*[nDim];
    int*    axesNBins  = new int[nDim];
    int*    currentBin = new int[nDim];

    int* bins     = new int[GetNBins(hist)];
    int  binsIter = 0;

    for(int i = 0; i < nDim; i++){
        axes[i]       = hist->GetAxis(i);
        currentBin[i] = 1;
        axesNBins[i]  = axes[i]->GetNbins();
    }

    //Follow a similar procedure used to load in 
    //  the observable normalization parameters.
    //For each axis, bin zero is underflow, and bin Nbins+1 is overflow.
    bool firstBin = true;

    while(currentBin[0] <= axesNBins[0]){

        //Loop backward over each dimension, determining whether to increment.
        for(int k = nDim - 1; k >=0; k--){
            //If it's our first pass through, do nothing, 
            //  since we started with the first bin of all of the axes.
            if(firstBin){
                firstBin = false;
                break;
            }

            //If the value we were previously considering for this bin was the
            //last bin, reset it to the first bin, then the loop will continue
            //around to consider the next axis.
            if(currentBin[k] == axesNBins[k]){
                currentBin[k] = 1;
            }

            //Otherwise, consider the next bin for this axis, and break.
            else{
                currentBin[k]++;
                break;
            }
        }

        //Check whether this bin is the last bin (all entries equal to last
        //axis entry) and if so, break out of the while loop.
        bool lastBin = true;

        for(int i = 0; i < nDim; i++){
            lastBin = lastBin && (currentBin[i] == axesNBins[i]);
        }

        //Now, we've assembled a multidimensional bin identifier. Use this to
        //get the linear bin index in the list.  Save this to the array we're
        //returning.
        bins[binsIter] = hist->GetBin(currentBin);
        binsIter++;

        if(lastBin){ break; }
    }

    //Delete the various arrays that aren't being returned from memory.
    //NB: We want to delete the axes array, but not the axes themselves, so
    //just call the array delete and not loop through it deleting each pointer.
    delete[] axes;
    delete[] axesNBins;
    delete[] currentBin;

    return bins;
}

void EventRate_RHCMultiPi(string filename){

    TFile* file = new TFile(filename.c_str(), "READ");

    int nType = 10;
    std::string type[10] = {"data",
                            "prefit",
                            "prefit_withNoWeights",
                            "prefit_withPOTWeights",
                            "prefit_withNomFluxWeights",
                            "prefit_withNomXSecWeights",
                            "prefit_withNomDetWeights",
                            "prefit_withNomCovWeights",
                            "prefit_withNomFluxAndXSecWeights",
                            "prefit_withNomFluxAndDetWeights"};

    int nName = 18;
    std::string name[18] = {"FGD1_numuCC_0pi",
                            "FGD1_numuCC_1pi",
                            "FGD1_numuCC_other",
                            "FGD2_numuCC_0pi",
                            "FGD2_numuCC_1pi",
                            "FGD2_numuCC_other",
                            "FGD1_anti-numuCC_0pi",
                            "FGD1_anti-numuCC_1pi",
                            "FGD1_anti-numuCC_other",
                            "FGD2_anti-numuCC_0pi",
                            "FGD2_anti-numuCC_1pi",
                            "FGD2_anti-numuCC_other",
                            "FGD1_NuMuBkg_CC0pi_in_AntiNu_Mode",
                            "FGD1_NuMuBkg_CC1pi_in_AntiNu_Mode",
                            "FGD1_NuMuBkg_CCother_in_AntiNu_Mode",
                            "FGD2_NuMuBkg_CC0pi_in_AntiNu_Mode",
                            "FGD2_NuMuBkg_CC1pi_in_AntiNu_Mode",
                            "FGD2_NuMuBkg_CCother_in_AntiNu_Mode"};

    for(int iName = 0; iName < nName; iName++){
        std::cout << std::endl;
        std::cout << std::setw(35) << name[iName] << ":"
                  << std::setw(10) << "Entries"
                  << std::setw(10) << "Binned"
                  << std::setw(10) << "Unbinned" << std::endl;

        double totalMCEvents = 0.;
        for(int iType = 0; iType < nType; ++iType) {

            std::string histoName = name[iName] + "_" + type[iType];

            THnD* histo = (THnD*) file->Get(histoName.c_str());
            double MCcounter         = 0.;
            double MCcounterUnbinned = 0.;

            if(histo){
                int* bins = GetBins(histo);
                for(int iBins = 0; iBins < GetNBins(histo); iBins++){
                    //MCcounter = MCcounter + histo->GetBinContent(bins[iBins]);
                    MCcounter += histo->GetBinContent(bins[iBins]);
                }
                for(int iBins = 0; iBins < histo->GetNbins(); iBins++){
                    //MCcounterUnbinned = MCcounterUnbinned + histo->GetBinContent(iBins);
                    MCcounterUnbinned += histo->GetBinContent(iBins);
                }
                std::cout << std::setw(35) << type[iType]
                          << std::setw(10) << histo->GetEntries()
                          << std::setw(10) << MCcounter
                          << std::setw(10) << MCcounterUnbinned << std::endl;

                totalMCEvents += histo->GetEntries();
                //delete histo, histo1, histo2, histo2D;
            }
        }
        //std::cout << "The total number of events is: " << totalMCEvents <<std::endl;

    }

    file->Close();
}
