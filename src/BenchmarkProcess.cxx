#define BenchmarkProcess_cxx

#include"BenchmarkProcess.hxx"
#include<iostream>

#include"TROOT.h"
#include"TSystem.h"

ClassImp(BenchmarkProcess)

BenchmarkProcess::BenchmarkProcess(TBenchmark *const bench, const TString& name)
{
    SetDefaults();
    if(!bench){
        fBenchmark = new TBenchmark();
    }
    fBenchmark->Start(fProcessName.Data());
}

BenchmarkProcess::~BenchmarkProcess()
{
    delete fBenchmark;
}

void BenchmarkProcess::SetDefaults()
{
    fProcessName = "Benchmark information";
    fBenchmark = nullptr;
    fprintAll = false;
}


int BenchmarkProcess::PrintBenchmarkInfo()
{
    using namespace std;
    cout << "\n#Showing benchmark information" << endl;
    fBenchmark->Show(fProcessName.Data());
    cout << endl;
    ProcInfo_t* procInfo = new ProcInfo_t();
    int inf1 = gSystem->GetProcInfo(procInfo);
    if (inf1 == -1)
        cout << "Unable to get ram and swap memory usage info" << endl;
    else
    {
        cout << "System time used by this process - " << procInfo->fCpuSys << " seconds " << endl;
        cout << "User time used by this process - " << procInfo->fCpuUser << " seconds " << endl;
        cout << "Resident memory used by this process - " << (procInfo->fMemResident) / 1024 << " MB" << endl;
        cout << "Virtual memory used by this process - " << (procInfo->fMemVirtual) / 1024 << " MB" << endl;
    }
    if (fprintAll)
    {
        MemInfo_t* memInfo = new MemInfo_t();
        gSystem->GetMemInfo(memInfo);
        cout << endl;
        cout << "Free RAM - " << memInfo->fMemFree << " MB" << endl;
        cout << "Total RAM - " << memInfo->fMemTotal << " MB" << endl;
        cout << "Used RAM - " << memInfo->fMemUsed << " MB" << endl;
        cout << "Free swap - " << memInfo->fSwapFree << " MB" << endl;
        cout << "Total swap - " << memInfo->fSwapTotal << " MB" << endl;
        cout << "Used swap - " << memInfo->fSwapUsed<< " MB" << endl;
    }
    return P0DBANFFInterface::GoodStatus;
}
