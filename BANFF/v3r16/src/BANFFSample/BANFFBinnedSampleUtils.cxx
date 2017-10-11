#include "BANFFBinnedSampleUtils.hxx"
#include <iostream>

int BANFFBinnedSampleUtils::GetNBins(THn* hist){


    //Loop through, multiplying the number of bins in each axis together, to
    //get the total number of bins in the n dimensional histogram.
    int nDim = hist->GetNdimensions();
    int nBins = 1;
    for(int i = 0; i < nDim; i++){

        nBins *= hist->GetAxis(i)->GetNbins();

    }

    //With all axes looped over, have the full bin count.  Return the result.
    return nBins;

}

int* BANFFBinnedSampleUtils::GetBins(THn* hist){


    //Extract the axes from the histogram, some information we will find useful
    //later, and information for the first bin we will consider.
    int nDim = hist->GetNdimensions();
    TAxis** axes = new TAxis*[nDim];
    int* axesNBins = new int[nDim];
    int* currentBin = new int[nDim];

    int* bins = new int[GetNBins(hist)];
    int binsIter = 0;

    for(int i = 0; i < nDim; i++){

        axes[i] = hist->GetAxis(i);
        currentBin[i] = 1;
        axesNBins[i] = axes[i]->GetNbins();
    }


    //Follow a similar procedure used to load in the observable normalization
    //parameters.
    //For each axis, bin zero is underflow, and bin Nbins+1 is overflow.
    bool firstBin = true;
    while(currentBin[0] <= axesNBins[0]){

        //Loop backward over each dimension, determining whether to increment.
        for(int k = nDim - 1; k >=0; k--){

            //If it's our first pass through, do nothing, since we started with
            //the first bin of all of the axes.
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

        if(lastBin){

            break;
        }


    }

    //Delete the various arrays that aren't being returned from memory.
    //NB: We want to delete the axes array, but not the axes themselves, so
    //just call the array delete and not loop through it deleting each pointer.
    delete[] axes;
    delete[] axesNBins;
    delete[] currentBin;

    return bins;
}
