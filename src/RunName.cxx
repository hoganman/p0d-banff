#define RUNNAME_CXX

#include"RunName.hxx"
ClassImp(RunName)
#include "P0DBANFFInterface.hxx"
#include<iostream>

//**************************************************
TChain* RunName::GetAllChainsFrom(const TString &ChainName,
        const TString &FileNamePath, Int_t firstNum, Int_t lastNum) const
//**************************************************
{
    TChain* chain = new TChain(ChainName.Data());
    const TString formatBuffer = iter_name(FileNamePath);
    TString testFileName = TString::Format(formatBuffer, lastNum);
    while(!P0DBANFFInterface::IsGoodFile(testFileName))
    {
        lastNum -= 1;
        testFileName = TString::Format(formatBuffer, lastNum);
        if (lastNum < firstNum)
        {
            std::cout << "RUNNAME ERROR: Unable to find files for " << properName.Data() << std::endl;
            return chain;
        }
    }
    for(Int_t iterIndex = firstNum; iterIndex <= lastNum; ++iterIndex)
    {
        testFileName = TString::Format(formatBuffer, iterIndex);
        chain->Add(testFileName.Data());
    }
    return chain;
}
