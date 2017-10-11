void numuBkgMultiPiDrawEff(TString run5c_MC, TString run6_MC){
  //==============================================================================================================================================================
  //=== Initialize DrawingTools‰
  //                                                T2K style index      
  //                                                     / 
  DrawingTools* draw = new DrawingTools(run5c_MC.Data(),4);

  //=== Initialize Experiment class 
  Experiment exp("nd280");
  
  //=== Magnet MC samples
  DataSample* magnet_mc_run5c = new DataSample(run5c_MC.Data());
  DataSample* magnet_mc_run6  = new DataSample(run6_MC.Data());
    
  //=== Run5c sample
  SampleGroup run5c("run5c");
  run5c.AddMCSample("magnet",magnet_mc_run5c);
  
  //=== Run6 sample
  SampleGroup run6("run6");
  run6.AddMCSample("magnet",magnet_mc_run6);
  
  exp.AddSampleGroup( "run5c" , run5c );
  exp.AddSampleGroup( "run6"  , run6  );

  //==============================================================================================================================================================
  
  
  //==============================================================================================================================================================
  //====== Draw Efficiency

  //=== CC-0Pion sample
  TCanvas *c4 = new TCanvas("c4_eff_cc0pi_mom","c4_eff_cc0pi_mom",1); 
  draw->SetLegendPos("rt");
  draw->SetTitleY("Efficiency");
  draw->SetTitleX("True Muon Momentum [GeV/c]");
  draw->DrawEff(exp,false,"truelepton_mom/1000",25,0,5,"accum_level[0]>8","topology==0","","","");
  c4->Print("plots/Eff_CC-0Pion_mom.pdf");

  TCanvas *c5 = new TCanvas("c5_eff_cc0pi_cos","c5_eff_cc0pi_cos",1); 
  draw->SetLegendPos("lt"); 
  draw->SetTitleY("Efficiency");
  draw->SetTitleX("True Muon cos#theta");
  draw->DrawEff(exp,false,"truelepton_dir[2]",50,-1,1,"accum_level[0]>8","topology==0","","","");
  c5->Print("plots/Eff_CC-0Pion_cos.pdf");
  //================

  //=== Draw Efficiency for CC-1PiPlus sample
  TCanvas *c6 = new TCanvas("c6_eff_cc1piplus_mom","c6_eff_cc1piplus_mom",1); 
  draw->SetLegendPos("rt");
  draw->SetTitleY("Efficiency");
  draw->SetTitleX("True Muon Momentum [GeV/c]");
  draw->DrawEff(exp,false,"truelepton_mom/1000",20,0,5,"accum_level[1]>8","topology==1","","","");
  c6->Print("plots/Eff_CC-1PiPlus_mom.pdf");

  TCanvas *c7 = new TCanvas("c7_eff_cc1piplus_cos","c7_eff_cc1piplus_cos",1); 
  draw->SetLegendPos("lt"); 
  draw->SetTitleY("Efficiency");
  draw->SetTitleX("True Muon cos#theta");
  draw->DrawEff(exp,false,"truelepton_dir[2]",50,-1,1,"accum_level[1]>8","topology==1","","","");
  c7->Print("plots/Eff_CC-1PiPlus_cos.pdf");
  //================

  //=== Draw Efficiency for CC-Other sample
  TCanvas *c8 = new TCanvas("c8_eff_ccoth_mom","c8_eff_ccoth_mom",1); 
  draw->SetLegendPos("rt");
  draw->SetTitleY("Efficiency");
  draw->SetTitleX("True Muon Momentum [GeV/c]");
  draw->DrawEff(exp,false,"truelepton_mom/1000",20,0,5,"accum_level[2]>7","topology==2","","","");
  c8->Print("plots/Eff_CC-Other_mom.pdf");

  TCanvas *c9 = new TCanvas("c9_eff_ccoth_cos","c9_eff_oth_cos",1); 
  draw->SetLegendPos("lt"); 
  draw->SetTitleY("Efficiency");
  draw->SetTitleX("True Muon cos#theta");
  draw->DrawEff(exp,false,"truelepton_dir[2]",50,-1,1,"accum_level[2]>7","topology==2","","","");
  c9->Print("plots/Eff_CC-Other_cos.pdf");
  //==============================================================================================================================================================
  
  //==============================================================================================================================================================

}
