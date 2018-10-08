#ifndef RUNNAME_HXX
#define RUNNAME_HXX

#include "TObject.h"
#include "TString.h"

class RunName : public TObject
{

public:
    RunName() : lowercaseName(""), uppercaseName(""), properName("")
    {}

    virtual ~RunName(){}

    RunName(const TString &lowercase, const TString &uppercase,
            const TString &proper = "")
    {
        lowercaseName = lowercase;
        uppercaseName = uppercaseName;
        properName = proper;
    }

    TString lowercaseName;
    TString uppercaseName;
    TString properName;

    TString LowerUpperPathJoin(const TString &prepend = "") const
    {
        if(prepend.Length() > 0)
            return prepend + "/" + lowercaseName + "/" + uppercaseName;
        return lowercaseName + "/" + uppercaseName;
    }

    inline TString iter_name(const TString &prepend = "") const
    {
        return LowerUpperPathJoin(prepend) + "_%d.root";
    }

    ClassDef(RunName, 1)

};

#endif
