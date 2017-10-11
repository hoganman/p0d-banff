void numuBkgMultiPiPrintRatio(TString run5c_data, TString run5c_MC, TString run6_data, TString run6_MC, TString sandMC){

/*
Info on the current configuration

#: name                      pdf             

0: ChargeIDEff               gaussian        
1: TpcClusterEff             gaussian        
2: TpcTrackEff               gaussian        
3: TpcFgdMatchEff            gaussian        
4: FgdHybridTrackEff         gaussian        
5: MichelEleEff              gaussian        
6: PileUp                    gaussian        
7: FgdMass                   gaussian        
8: OOFV                      gaussian        
9: SIPion                    gaussian        
10: SIProton                  gaussian       
11: FluxWeight                gaussian     
  
*/

  //==============================================================================================================================================================
  //=== Initialize DrawingTools
  //                                                T2K style index      
  //                                                     / 
  DrawingTools* draw = new DrawingTools(run5c_MC.Data(),4);

  draw->SetStackFillStyle(3254);

  //=== Initialize Experiment class 
  Experiment exp("nd280");
  
  //=== Data samples
  DataSample* data_run5c = new DataSample(run5c_data.Data());
  DataSample* data_run6  = new DataSample(run6_data.Data());

  //=== Magnet MC samples
  DataSample* magnet_mc_run5c = new DataSample(run5c_MC.Data());
  DataSample* magnet_mc_run6  = new DataSample(run6_MC.Data());
  
  //=== Sand MC samples
  DataSample* sand_mc_run5 = new DataSample(sandMC.Data());
  DataSample* sand_mc_run6 = new DataSample(sandMC.Data());
  
  //=== Run5c sample
  SampleGroup run5c("run5c");
  run5c.AddDataSample(data_run5c);
  run5c.AddMCSample("magnet",magnet_mc_run5c);
  run5c.AddMCSample("sand",sand_mc_run5);
  
  //=== Run6 sample
  SampleGroup run6("run6");
  run6.AddDataSample(data_run6);
  run6.AddMCSample("magnet",magnet_mc_run6);
  run6.AddMCSample("sand",sand_mc_run6);
  
  exp.AddSampleGroup( "run5c" , run5c );
  exp.AddSampleGroup( "run6"  , run6  );

  exp.SetCurrentTree("default");

  //==============================================================================================================================================================
  
  
  //==============================================================================================================================================================
  //=== Cuts for different sample, i.e. CC-Inclusive, CC-0pi etc 
  const Int_t   Nsamples = 3;
  
  TString cut_sample[Nsamples]  = { "accum_level[][0]>8" , "accum_level[][1]>8" , "accum_level[][2]>7" };
  TString sample_name[Nsamples] = {      "CC-0Pion"      ,      "CC-1PiPlus"    ,       "CC-Other"     };  

  //==============================================================================================================================================================
  

  //==============================================================================================================================================================
  cout<< " ======  Evaluate DATA/MC Ratio for CC-0Pion " <<endl;

  cout<< " ====== NOW " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW NOW");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== ChargeIDEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== TpcClusterEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== TpcTrackEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== TpcFgdMatchEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== FgdHybridTrackEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== MichelEleEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== PileUp " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== FgdMass " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== OOFV " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== SIPion " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8 WC9");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== SIProton " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8 WC9 WC10");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== Flux " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][0]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8 WC9 WC10 WC11");
  cout<< " ==============================================  " <<endl;

  cout<< " ==============================================  " <<endl;



  cout<< " ======  Evaluate DATA/MC Ratio for CC-1PositivePion " <<endl;

  cout<< " ====== NOW " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW NOW");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== ChargeIDEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== TpcClusterEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== TpcTrackEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== TpcFgdMatchEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== FgdHybridTrackEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== MichelEleEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== PileUp " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== FgdMass " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== OOFV " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== SIPion " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8 WC9");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== SIProton " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8 WC9 WC10");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== Flux " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8 WC9 WC10 WC11");
  cout<< " ==============================================  " <<endl;

  cout<< " ==============================================  " <<endl;


  cout<< " ======  Evaluate DATA/MC Ratio for CC-Other " <<endl;


  cout<< " ====== NOW " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW NOW");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== ChargeIDEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== TpcClusterEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== TpcTrackEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1 WC2");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== TpcFgdMatchEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][1]>8","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== FgdHybridTrackEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== MichelEleEff " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== PileUp " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== FgdMass " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== OOFV " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== SIPion " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8 WC9");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== SIProton " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8 WC9 WC10");
  cout<< " ==============================================  " <<endl;

  cout<< " ====== Flux " <<endl;
  draw->DrawRatio(exp,"0.",1,-1,1,"accum_level[][2]>7","","DUMP NODRAW WCORR WC0 WC1 WC2 WC3 WC4 WC5 WC6 WC7 WC8 WC9 WC10 WC11");
  cout<< " ==============================================  " <<endl;

}
