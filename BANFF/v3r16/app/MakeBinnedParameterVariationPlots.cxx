///An app that takes the output produced from varying each systematic
///individually and creates a PDF document, 1 page per systematic, showing each 
///variation on a histogram with each bins content, and as a fraction of the
///nominal bin content, for all sigma variations performed.

//Usage: MakeBinnedParameterVariationPlots inputFile outputFile
#include "THn.h"
#include "TFile.h"
#include "TObjString.h"
#include "TH1D.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include <iostream>
#include "BANFFSample/BANFFBinnedSampleUtils.hxx"
#include "TCanvas.h"
#include "TPad.h"
#include "TPaveText.h"
#include "THStack.h"
#include "TLegend.h"
#include <sstream>

int main(int argc, char** argv){

    if(argc != 3){

        std::cout << "Warning: Unexpected number of arguments." << std::endl;

    }

    TFile* inputFile = new TFile(argv[1]);

    //We need to know the number of samples and parameters, both of which we
    //can get from the name arrays.
    TObjArray* sampleNames = (TObjArray*)(inputFile->Get("sampleNames"));
    Int_t nSamples = sampleNames->GetEntries();

    TObjArray* paramNames = (TObjArray*)(inputFile->Get("parameterNames"));
    
    //Note: We want to include NULL entries here due to parameters on the end being 
    //skipped otherwise (i.e. we need the full length of the TObjArray, not
    //just the number of non-NULL entries).  The NULL entries are accounted for elsewhere.
    Int_t nParams = paramNames->GetEntriesFast();
    
    //Also pull out the TVectorD of systematic variations.
    TVectorD* sigmaValues = (TVectorD*)(inputFile->Get("sigmaValues"));
    Int_t nSigmaValues = sigmaValues->GetNrows();

    //Each sample is considered individually. We need a histogram to store the
    //nominal histogram in.
    THnD* nominalHist;

    //We also need to pull out the current histogram under consideration.
    THnD* currentHist;

    //We're putting the information in TH1Ds, which we need to have one for
    //each sigma value for bin content and fraction of nominal for each sigma variation.
    TH1D* nominalBinContent;
    TH1D* nominalBinFraction;

    TH1D** variedBinContent = new TH1D*[nSigmaValues];
    TH1D** variedBinFraction = new TH1D*[nSigmaValues];
    
    //Only need to create the canvas and pads once, can fill as needed before
    //printing.
    //Create a TCanvas that is 8.5x11 in size in pixels at 300 dpi
    //resolution, i.e. 2550x3300
    TCanvas* c1 = new TCanvas("canvas", "canvas", 2550, 3300);

    //Divide the canvas manually.
    //Top pad is to be 16% of the y 
    TPad* topPad = new TPad("topPad","topPad",0.005,0.84,0.995,0.995);

    c1->cd();
    //Middle pad goes from 0.42 to 0.84.
    TPad* middlePad = new TPad("middlePad","middlePad",0.005,0.42,0.995,0.84);

    c1->cd();
    //Bottom pad goes from bottom to 0.42
    TPad* bottomPad = new TPad("bottomPad","bottomPad",0.005,0.005,0.995,0.42);

    //The top one we want to divide in two, to put title information on
    //the top left, and the legend on the top right.
    topPad->Divide(2,0);

    topPad->Draw();
    middlePad->Draw();
    bottomPad->Draw();


    //TODO: Choosing colours typically isn't simple.  If there's only 4 sigma
    //values (i.e. the normal use case, perhaps the only one), just go with the same colour scheme Asher used.
    //See https://root.cern.ch/root/html/TColor.html
    //Otherwise print an error and exit.
    int* plotColours = new int[nSigmaValues];

    if(nSigmaValues == 4){

        //-3 sigma is red.
        plotColours[0] = 2;

        //-1 sigma is magenta.
        plotColours[1] = 6;

        //1 sigma is cyan.
        plotColours[2] = 7;

        //3 sigma is blue;
        plotColours[3] = 4;


    }

    else{

        std::cout << "Colours not defined for this number of variations." << std::endl;
        exit(1);

    }

    //Loop over the parameters.
    for(int ip = 0; ip < nParams; ip++){

        //For each parameter, loop over each sample.
        for(int is = 0; is < nSamples; is++){

            //Get various names we'll need.
            std::string sampleName(((TObjString*)(sampleNames->At(is)))->GetString());
            std::string nominalPlotName = sampleName + "_nomMC";
            std::string predMCNameBase = sampleName + "_predMC_";

            //Check for NULL, as this is what it will be if the parameter was
            //skipped for some reason.  Skip this index is this is the case.)
            if(paramNames->At(ip) == NULL){
                continue;
            }

            std::string paramName(((TObjString*)(paramNames->At(ip)))->GetString());

            //Deal with everything related to nominal for this sample, since
            //that doesn't change.
            nominalHist = (THnD*)(inputFile->Get(nominalPlotName.c_str()));

            //The number of bins is the same for all of this sample's
            //histograms, as is the bin get provided by GetBins.
            Int_t nBins = BANFFBinnedSampleUtils::GetNBins(nominalHist);
            int* bins = BANFFBinnedSampleUtils::GetBins(nominalHist);

            nominalBinContent = new TH1D("nominalBin","nominalBin", nBins, 0.0, (double)nBins);
            nominalBinContent->SetLineColor(kBlack);

            nominalBinFraction = new TH1D("nominalFrac","nominalFrac", nBins, 0.0, (double)nBins);
            nominalBinFraction->SetLineColor(kBlack);

            for(int ib = 0; ib < nBins; ib++){

                nominalBinContent->SetBinContent(ib + 1, nominalHist->GetBinContent(bins[ib]));
                nominalBinFraction->SetBinContent(ib + 1, 1.0);
            }


            //Get the various elements we're drawing ready to go.
            //On the top left put the title information.
            TPaveText* pt = new TPaveText(0.05,0.05,0.95,0.95);
            pt->SetFillColor(kWhite);
            std::stringstream paramLine;
            paramLine << "Parameter " << ip << ":";
            pt->AddText(paramLine.str().c_str());
            pt->AddText(paramName.c_str());
            pt->AddText((sampleName + " sample").c_str()); 
            topPad->cd(1);
            pt->Draw();

            //Set up the stacks that will hold the histograms.
            THStack* stackBinContent = new THStack("sbc",";Bin number;Bin content");
            THStack* stackBinFraction = new THStack("sbf",";Bin number; Fraction of nominal bin content");

            //Put nominal into both of these.
            stackBinContent->Add(nominalBinContent);
            stackBinFraction->Add(nominalBinFraction);

            //Set up the legend.
            topPad->cd(2);
            TLegend* leg = new TLegend(0.05,0.05,0.95,0.95);
            leg->SetFillColor(kWhite);
            //Put nominal in.
            leg->AddEntry(nominalBinContent,"nominal");

            //Now take care of the variation histograms that need to for each sample.
            for(int iv = 0; iv < nSigmaValues; iv++){

                //Set up the name for the variation we're considering.
                std::stringstream currentName;
                currentName << predMCNameBase << paramName << "_" << (*sigmaValues)(iv) << "sigma";

                currentHist = (THnD*)(inputFile->Get(currentName.str().c_str()));
                variedBinContent[iv] = new TH1D((currentName.str()+"Content").c_str(),
                        (currentName.str()+"Content").c_str(), nBins, 0.0, (double)nBins);
                variedBinFraction[iv] = new TH1D((currentName.str()+"Frac").c_str(),
                        (currentName.str()+"Frac").c_str(), nBins, 0.0, (double)nBins);

                //Now that the histograms are set up, loop through the bins and
                //fill them.
                for(int ib = 0; ib < nBins; ib++){

                    variedBinContent[iv]->SetBinContent(ib + 1, currentHist->GetBinContent(bins[ib]));
                    variedBinFraction[iv]->SetBinContent(ib + 1, 
                            variedBinContent[iv]->GetBinContent(ib+1)/nominalBinContent->GetBinContent(ib+1));
                }


                //Set the line colours, then add to the relevant stack and
                //legend.
                variedBinContent[iv]->SetLineColor(plotColours[iv]);
                stackBinContent->Add(variedBinContent[iv]);

                variedBinFraction[iv]->SetLineColor(plotColours[iv]);
                stackBinFraction->Add(variedBinFraction[iv]);

                std::stringstream sigmaString;
                std::string theSign("");
                if((*sigmaValues)(iv) > 0.0){

                    theSign = "+";

                }
                sigmaString << theSign << (*sigmaValues)(iv) << " #sigma";

                leg->AddEntry(variedBinContent[iv],sigmaString.str().c_str());

            }

        
            //Now that everything has been assembled, put it in the right spot
            //on the canvas and then print it out to file.
            
            //The legend goes on the right hand side of topPad.
            topPad->cd(2);
            leg->Draw();

            //The varied bin content plot goes on the next pad below.
            //"nostack" superimposes them instead of drawing a stack.
            middlePad->cd();
            middlePad->SetTicks(1,1);

            stackBinContent->Draw("nostack");

            //The fraction of nominal goes in the very bottom pad.
            bottomPad->cd();
            bottomPad->SetTicks(1,1);
            stackBinFraction->Draw("nostack");
            stackBinFraction->SetMinimum(stackBinFraction->GetMinimum("nostack") - TMath::Abs(0.05*(1.0 - stackBinFraction->GetMinimum("nostack"))));
            stackBinFraction->SetMaximum(stackBinFraction->GetMaximum("nostack") + TMath::Abs(0.05*(stackBinFraction->GetMaximum("nostack") - 1.0)));
            stackBinFraction->Draw("nostack");

            //With the canvas drawn, it is time to print. The exact statement
            //is dependent on whether this is the first or last canvas to
            //print, or something in between.
            c1->Modified();
            c1->Update();

            std::string outputFileName(argv[2]);

            //First canvas.
            if((ip == 0) && (is == 0)){

                c1->Print((outputFileName+"(").c_str());
            }

            else if((ip == (nParams - 1)) && (is == (nSamples - 1))){

                c1->Print((outputFileName+")").c_str());

            }

            else{

                c1->Print(outputFileName.c_str());
            }


            //With everything saved, delete all pointers that are going to be
            //recreated on the next pass.
            delete nominalHist;
            delete nominalBinContent;
            delete nominalBinFraction;
            delete currentHist;
            delete pt;
            delete stackBinContent;
            delete stackBinFraction;
            delete leg;

            //Clear the pads so they don't keep trying to draw more and more
            //things on top of each other at each iteration, slowing things
            //down.
            topPad->GetPad(1)->Clear();
            topPad->GetPad(2)->Clear();
            middlePad->Clear();
            bottomPad->Clear();

            //Loop through the histogram arrays and delete their entries.
            for(int ihist = 0; ihist < nSigmaValues; ihist++){
                delete variedBinContent[ihist];
                delete variedBinFraction[ihist];

            }


            std::cout << sampleName << " " << paramName <<  std::endl;
        } //End loop over samples.
    } //End loop over parameters.

}//End main method.
