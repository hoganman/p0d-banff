void numuBkgMultiPiPionTaggingStudy(TString run5c_data, TString run5c_MC, TString run6_data, TString run6_MC, TString sandMC){

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

  draw->SetOptStat("i");

  TCanvas *c1 = new TCanvas("c1","c1",1);
  draw->SetTitleY("Events");
  draw->SetTitleX("Number of pion-like tracks");
  draw->Draw(exp,"NPiFGD",4,0,4,"topology","accum_level[][0]>6");
  c1->Print("plots/npions_fgd1.pdf");
    
  TCanvas *c2 = new TCanvas("c2","c2",1);
  draw->SetTitleY("Events");
  draw->SetTitleX("Number isolated FGD1 pion-like tracks");
  draw->Draw(exp,"NFGDPi",4,0,4,"topology","accum_level[][0]>6");
  c2->Print("plots/isoFGD1pionliketracks.pdf");

  TCanvas *c3 = new TCanvas("c3","c3",1);
  draw->SetTitleY("Events");
  draw->SetTitleX("Number isolated FGD1 pion-like tracks");
  draw->Draw(exp,"NFGDPi"3,1,4,"topology","accum_level[][0]>6");
  c3->Print("plots/isoFGD1pionliketracks_zoom.pdf");
	    
  TCanvas *c4 = new TCanvas("c4","c4",1);
  draw->SetTitleY("Events");
  draw->SetTitleX("Number of Michel electrons");
  draw->Draw(exp,"NFGDPi",4,0,4,"topology","accum_level[][0]>6");
  c4->Print("plots/MichelElectrons_fgd1.pdf");

  TCanvas *c5 = new TCanvas("c5","c5",1);
  draw->SetTitleY("Events");
  draw->SetTitleX("Number of Michel electrons");
  draw->Draw(exp,"NFGDPi",3,1,4,"topology","accum_level[][0]>6");
  c5->Print("plots/MichelElectrons_zoomfgd1.pdf");


  //==============================================================================================================================================================
  //====== Print Purities 

  std::cout<< "---------------------------------------------------------------------"<<std::endl;
  std::cout<< "---------------------------------------------------------------------"<<std::endl;
  std::cout<< "-------------------------- Print Purities ---------------------------"<<std::endl;
  std::cout<< "---------------------------------------------------------------------"<<std::endl;
  std::cout<< "---------------------------------------------------------------------"<<std::endl;

  std::cout<< "---------------------- NME==0 && NPosPion==0 ------------------------"<<std::endl;
  draw->PrintPurities(exp,"topology","accum_level[][0]>6 && NME==0 && NPosPion==0");
  std::cout<< "---------------------------------------------------------------------"<<std::endl;

  std::cout<< "---------------------- NME==0 && NPosPion==1 ------------------------"<<std::endl;
  draw->PrintPurities(exp,"topology","accum_level[][0]>6 && NME==0 && NPosPion==1");
  std::cout<< "---------------------------------------------------------------------"<<std::endl;

  std::cout<< "---------------------- NME==1 && NPosPion==1 ------------------------"<<std::endl;
  draw->PrintPurities(exp,"topology","accum_level[][0]>6 && NME==1 && NPosPion==1");
  std::cout<< "---------------------------------------------------------------------"<<std::endl;


  std::cout<< "---------------------- NME==1 && NPosPion==0 ------------------------"<<std::endl;
  draw->PrintPurities(exp,"topology","accum_level[][0]>6 && NME==1 && NPosPion==0");
  std::cout<< "---------------------------------------------------------------------"<<std::endl;


  std::cout<< "---------------------------------------------------------------------"<<std::endl;
  std::cout<< "---------------------------------------------------------------------"<<std::endl;
  std::cout<< "---------------------------- The end --------------------------------"<<std::endl;
  std::cout<< "---------------------------------------------------------------------"<<std::endl;
  std::cout<< "---------------------------------------------------------------------"<<std::endl;

  //==============================================================================================================================================================
  
  //==============================================================================================================================================================

}
