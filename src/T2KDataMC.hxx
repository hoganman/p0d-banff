#ifndef T2KDATAMC_HXX
#define T2KDATAMC_HXX

#include "RunName.hxx"
#include "TChain.h"

class T2KDataMC : public TObject
{

public:

    T2KDataMC(){Init();}

    virtual ~T2KDataMC(){}

    RunName RUN2W;
    RunName RUN2A;
    RunName RUN3B;
    RunName RUN3C;
    RunName RUN4W;
    RunName RUN4A;
    RunName RUN5C;
    RunName RUN6B;
    RunName RUN6C;
    RunName RUN6D;
    RunName RUN6E;
    RunName RUN7B;
    RunName SANDFHC;
    RunName SANDRHC;
    RunName RUN2WDATA;
    RunName RUN2ADATA;
    RunName RUN3BDATA;
    RunName RUN3CDATA;
    RunName RUN4WDATA;
    RunName RUN4ADATA;
    RunName RUN5CDATA;
    RunName RUN6BDATA;
    RunName RUN6CDATA;
    RunName RUN6DDATA;
    RunName RUN6EDATA;
    RunName RUN7BDATA;

    TString NEUT6B;
    TString NEUT6L;
    TString DATA6M;
    TString DATA6N;
    TString SAND6B;

protected:

    void Init();

ClassDef(T2KDataMC, 1)

};

#endif
