void numuBkgMultiPiDrawFinalPlots(TString run5c_data, TString run5c_MC, TString run6_data, TString run6_MC, TString sandMC){

  //==============================================================================================================================================================
  //=== Initialize DrawingTools
  //                                                T2K style index      
  //                                                     / 
  DrawingTools* draw = new DrawingTools(run5c_MC.Data(),4);
  //  draw->SetStackFillStyle(3254);

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

  exp.GetMCSample("run5c" ,  "magnet")->SetCurrentTree("all_syst");
  exp.GetMCSample("run6" ,   "magnet")->SetCurrentTree("all_syst");
  exp.GetMCSample("run5c" ,  "sand")->SetCurrentTree("sandmu_syst");
  exp.GetMCSample("run6" ,   "sand")->SetCurrentTree("sandmu_syst");
 
  //==============================================================================================================================================================
  
  
  //==============================================================================================================================================================
  //=== Cuts for different sample, i.e. CC-Inclusive, CC-0pi etc 
  const Int_t   Nsamples = 3;

  TString cut_sample[Nsamples]  = { "accum_level[][0]>8 && weight_syst_total<100" , "accum_level[][1]>8 && weight_syst_total<100" , "accum_level[][2]>7 && weight_syst_total<100" };
  TString sample_name[Nsamples] = {      "CC-0Pion"      ,      "CC-1PiPlus"    ,       "CC-Other"     };  

  //==============================================================================================================================================================
  
  //==============================================================================================================================================================
  //====== Draw muon momentum, cosine and angle distributions
  TString reco_variable[] = { "selmu_mom/1000" ,"selmu_costheta"};
  Int_t   Nvar = sizeof(reco_variable)/sizeof(TString);

  TString variable_name[] = { "mom" , "costheta" };
  
  TCanvas *c1[Nsamples];
  TString mom_plot_TitleY[Nsamples] = {"Events/(0.2 GeV/c)","Events/(0.25 GeV/c)","Events/(0.25 GeV/c)"};

  for(int is=0; is<Nsamples; is++){
    for(int iv=0; iv<Nvar; iv++){
      c1[is]=new TCanvas(Form("c1_%s_%s",sample_name[is].Data(),reco_variable[iv].Data()),Form("c1_%s_%s",sample_name[is].Data(),reco_variable[iv].Data()),1);
      draw->SetOptStat("i");
      if(iv==0){
	draw->SetLegendPos("tr");
	draw->SetTitleY(Form("%s",mom_plot_TitleY[is].Data()));
	draw->SetTitleX("Muon Momentum [GeV/c]");
      }
      if(iv==1){
	draw->SetLegendPos("tl");
	draw->SetTitleY("Events");
	draw->SetTitleX("Muon cos#theta");
      }
      draw->SetAllMCStatLabel("MC stat. error");
      draw->SetAllMCLabel("MC stat. + det. syst. error");
      //draw->SetMCErrorColor(kAzure);
      draw->SetMCErrorColor(7);
      if(iv==0){
	if(is<1) draw->Draw(exp,Form("%s",reco_variable[iv].Data()),25,0,5,"all",Form("%s",cut_sample[is].Data()),"","NOAUTOLABELS SYS NWS11 WC11 E2");
	else draw->Draw(exp,Form("%s",reco_variable[iv].Data()),20,0,5,"all",Form("%s",cut_sample[is].Data()),"","NOAUTOLABELS SYS NWS11 WC11 E2");
      }
      if(iv==1){
	draw->Draw(exp,Form("%s",reco_variable[iv].Data()),30,-1,1,"all",Form("%s",cut_sample[is].Data()),"","NOAUTOLABELS SYS NWS11 WC11 E2");
      }
      c1[is]->Print(Form("plots/Final_%s_%s.pdf",sample_name[is].Data(),variable_name[iv].Data()));
    }
  }
}
