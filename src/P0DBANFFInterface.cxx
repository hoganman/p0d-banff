#define P0DBANFFInterface_cxx

#include"P0DBANFFInterface.hxx"
ClassImp(P0DBANFFInterface)

#include"ColorBlindFriendly.hxx"

P0DBANFFInterface::P0DBANFFInterface() 
{

}

P0DBANFFInterface::~P0DBANFFInterface()
{

}

void P0DBANFFInterface::RandomSleep(const Int_t& nSeconds) const
{
    ROOTRandomSleep(nSeconds); 
}

void P0DBANFFInterface::CheckFile(const TString& fileName) const
{
    ROOTCheckFile(fileName);
}


void P0DBANFFInterface::LoadColorBlindPalette() const
{
    ColorBlindColors::SetCBFColorBar();
}
