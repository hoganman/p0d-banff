#ifndef P0DBANFFInterface_hxx
#define P0DBANFFInterface_hxx

#include"ROOTUtils.hxx"

class P0DBANFFInterface : public TObject {
public:
    P0DBANFFInterface();
    virtual ~P0DBANFFInterface();
    void LoadColorBlindPalette() const;
    void CheckFile(const TString& fileName) const;
    void RandomSleep(const Int_t& nSeconds) const;

    ClassDef(P0DBANFFInterface,1)
};

#endif
