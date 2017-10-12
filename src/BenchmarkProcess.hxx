#ifndef BenchmarkProcess_hxx
#define BenchmarkProcess_hxx

#include"TObject.h"
#include"TBenchmark.h"
#include"TString.h"

#include"P0DBANFFTypes.hxx"

///Credit to this class goes to S. Assylbelkov and T. Wachala
class BenchmarkProcess: public TObject{
public:
    BenchmarkProcess(TBenchmark *const bench, const TString& name);
    virtual ~BenchmarkProcess();

    void SetPrintAll(bool printOption)
    {
      fprintAll = printOption;
    }
    void StartTimer(const TString& name)
    {
      fProcessName = name;
      fBenchmark->Start(name);
    }
    void EndTimer()
    {
      fBenchmark->Stop(fProcessName);
    }
    int PrintBenchmarkInfo();

protected:
    void SetDefaults();
    TBenchmark *fBenchmark;
    TString fProcessName;
    bool fprintAll;

    ClassDef(BenchmarkProcess,1);
};

#endif
