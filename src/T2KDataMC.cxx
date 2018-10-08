#define T2KDATAMC_CXX
#include"T2KDataMC.hxx"
ClassImp(T2KDataMC)
#include <iostream>

//**************************************************
void T2KDataMC::Init()
//**************************************************
{
    NEUT6B = "mcp6_Spin_B/neut";
    NEUT6L = "mcp6_Spin_L/neut";
    DATA6M = "rdp6_Spin_M";
    DATA6N = "rdp6_Spin_N";

    RUN2W = RunName("run2-water", "Run2_Water", NEUT6B, kTRUE);
    RUN2A = RunName("run2-air", "Run2_Air", NEUT6B, kTRUE);
    RUN3B = RunName("run3b-air", "Run3b_Air", NEUT6B, kTRUE);
    RUN3C = RunName("run3c-air", "Run3c_Air", NEUT6B, kTRUE);
    RUN4W = RunName("run4-water", "Run4_Water", NEUT6B, kTRUE);
    RUN4A = RunName("run4-air", "Run4_Air", NEUT6B, kTRUE);
    RUN5C = RunName("run5c-water", "Run5c_Water", NEUT6B, kTRUE);
    RUN6B = RunName("run6b-air", "Run6b_Air", NEUT6B, kTRUE);
    RUN6C = RunName("run6c-air", "Run6c_Air", NEUT6B, kTRUE);
    RUN6D = RunName("run6d-air", "Run6d_Air", NEUT6B, kTRUE);
    RUN6E = RunName("run6e-air", "Run6e_Air", NEUT6B, kTRUE);
    RUN7B = RunName("run7b-water", "Run7b_Water", NEUT6B, kTRUE);
    SANDFHC = RunName("run3-air_FHC", "Run3_Air_FHC", NEUT6B, kTRUE);
    SANDRHC = RunName("run3-air_RHC", "Run3_Air_RHC", NEUT6L, kTRUE);

    RUN2WDATA = RunName("run2-water", "Run2_Water_data", DATA6M, kFALSE);
    RUN2ADATA = RunName("run2-air", "Run2_Air_data", DATA6M, kFALSE);
    RUN3BDATA = RunName("run3b-air", "Run3b_Air_data", DATA6M, kFALSE);
    RUN3CDATA = RunName("run3c-air", "Run3c_Air_data", DATA6M, kFALSE);
    RUN4WDATA = RunName("run4-water", "Run4_Water_data", DATA6M, kFALSE);
    RUN4ADATA = RunName("run4-air", "Run4_Air_data", DATA6M, kFALSE);
    RUN5CDATA = RunName("run5c-water", "Run5c_Water_data", DATA6M, kFALSE);
    RUN6BDATA = RunName("run6b-air", "Run6b_Air_data", DATA6M, kFALSE);
    RUN6CDATA = RunName("run6c-air", "Run6c_Air_data", DATA6M, kFALSE);
    RUN6DDATA = RunName("run6d-air", "Run6d_Air_data", DATA6M, kFALSE);
    RUN6EDATA = RunName("run6e-air", "Run6e_Air_data", DATA6M, kFALSE);
    RUN7BDATA = RunName("run7b-water", "Run7b_Water_data", DATA6N, kFALSE);

}
