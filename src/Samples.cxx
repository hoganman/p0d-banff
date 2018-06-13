#define SAMPLES_CXX

#include "Samples.hxx"
#include "P0DBANFFInterface.hxx"
#include "XMLTools.hxx"
#include <iostream>
ClassImp(Samples)

//**************************************************
Samples::Samples()
//**************************************************
{
    nMomentumBinEdges = 0;
    nCosThetaBinEdges = 0;
    momentumBins = NULL;
    cosThetaBins = NULL;
}

//**************************************************
Samples::Samples(TString name, TString configFile)
//**************************************************
{
    XMLTools* xml = new XMLTools();
    xml->SetFile(configFile);
    XMLNodePointer_t ptr = xml->GetXMLNode(name);
    if(!ptr)
    {
        std::cout << "ERROR: Unable to get " << name.Data() << " from " << configFile.Data() << std::endl;
        return;
    }

    //get the raw strings
    TString raw_momentumBinEdges    = xml->GetChildAttributeFromNode(name, "momentumBinEdges");
    TString raw_momentumNumBinEdges = xml->GetChildAttributeFromNode(name, "momentumNumBinEdges");
    TString raw_cosThetaBinEdges    = xml->GetChildAttributeFromNode(name, "cosThetaBinEdges");
    TString raw_cosThetaNumBinEdges = xml->GetChildAttributeFromNode(name, "cosThetaNumBinEdges");

    //convert the strings into their approrprite type
    nMomentumBinEdges = raw_momentumNumBinEdges.Atoi();
    nCosThetaBinEdges = raw_cosThetaNumBinEdges.Atoi();
    //turns the items with ',' into vectors
    std::vector<TString> momentumBinEdges_vect = P0DBANFFInterface::SplitString(raw_momentumBinEdges, ',');
    std::vector<TString> cosThetaBinEdges_vect = P0DBANFFInterface::SplitString(raw_cosThetaBinEdges, ',');

    if(static_cast<Int_t>(momentumBinEdges_vect.size()) != nMomentumBinEdges)
    {
        std::cout << "ERROR in " << name.Data() << std::endl;
        std::cout << "    The number of stated bin entries (" << momentumBinEdges_vect.size() << ") for momentum does" << std::endl;
        std::cout << "    match the number of found entries (" << nMomentumBinEdges << ")" <<  std::endl;
        return;
    }
    if(static_cast<Int_t>(cosThetaBinEdges_vect.size()) != nCosThetaBinEdges)
    {
        std::cout << "ERROR in " << name.Data() << std::endl;
        std::cout << "    The number of stated bin entries (" << cosThetaBinEdges_vect.size() << ") for cosTheta does" << std::endl;
        std::cout << "    match the number of found entries (" << nMomentumBinEdges << ")" <<  std::endl;
        return;
    }
    for(UInt_t index = 0; index < momentumBinEdges_vect.size(); ++index)
    {
        momentumBinsEdges[index] = momentumBinEdges_vect.at(index).Atof();
    }
    for(UInt_t index = 0; index < cosThetaBinEdges_vect.size(); ++index)
    {
        cosThetaBinsEdges[index] = cosThetaBinEdges_vect[index].Atof();
    }
    Char_t buffer1[500];
    Char_t buffer2[500];
    sprintf(buffer1, "%s_momentumBins", name.Data());
    sprintf(buffer2, "%s_cosThetaBins", name.Data());
    momentumBins = new AnalysisBins(buffer1, momentumBinsEdges, nMomentumBinEdges, kTRUE);
    cosThetaBins = new AnalysisBins(buffer2, cosThetaBinsEdges, nCosThetaBinEdges);
}

//**************************************************
Samples::~Samples()
//**************************************************
{
    if(momentumBins) delete momentumBins;
    if(cosThetaBins) delete cosThetaBins;
}