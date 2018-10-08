#ifndef RUNNAME_HXX
#define RUNNAME_HXX

#include "TChain.h"
#include "TString.h"
#include <iostream>

class RunName : public TObject
{

public:
    RunName() : lowercaseName(""), uppercaseName(""), properName(""),
                productionPath(""), isMC(kFALSE)
    {}

    virtual ~RunName(){}

    RunName(TString lowercase, TString uppercase,
            TString production_path, Bool_t MC)
    {
        lowercaseName = lowercase;
        uppercaseName = uppercase;
        properName = uppercaseName.Copy().ReplaceAll("_", " ");
        productionPath = production_path;
        isMC = MC;
    }

    TString LowerUpperPathJoin(const TString &prepend = "") const
    {
        TString retString = lowercaseName + "/" + uppercaseName;
        if(prepend.Length() > 0)
            retString.Prepend(prepend + "/");
        return retString;
    }

    TString iter_name(const TString &prependProduction = "",
            const TString &prependLowerUpperPath = "") const
    {
        TString retString = LowerUpperPathJoin(prependLowerUpperPath) + "_%d.root";
        retString.Prepend(productionPath + "/");
        if (prependProduction.Length() > 0)
            retString.Prepend(prependProduction + "/");
        return retString;
    }

    inline TChain* GetAllDataChainsFrom(const TString &DataTreeName, const TString &path) const
    {
        return GetAllChainsFrom(DataTreeName, path, 1, 10);
    }

    inline TChain* GetAllMCChainsFrom(const TString &MCTreeName, const TString &path) const
    {
        return GetAllChainsFrom(MCTreeName, path, 1, 100);
    }

    TChain* GetAllChainsFrom(const TString &ChainName,
            const TString &FileNamePath, Int_t firstNum,
            Int_t lastNum) const;

    inline TChain* GetAllChainsFrom(const TString &ChainName,
            const TString &FileNamePath) const
    {
        return isMC ? GetAllMCChainsFrom(ChainName, FileNamePath) : GetAllDataChainsFrom(ChainName, FileNamePath);
    }

    TString GetLowerCaseName() const {return lowercaseName;}
    TString GetUpperCaseName() const {return uppercaseName;}
    TString GetProperName() const {return properName;}
    TString GetProductionPath() const {return productionPath;;}
    Bool_t GetIsMC() const {return isMC;}

protected:

    TString lowercaseName;
    TString uppercaseName;
    TString properName;
    TString productionPath;
    Bool_t isMC;

    ClassDef(RunName, 1)


};

#endif
