void numuBkgMultiPiDrawRelativeError(){

  //==============================================================================================================================================================
  //=== Initialize DrawingTools
  DrawingTools* draw = new DrawingTools("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD1/MC/numubkgmultipi_run5cmc_500toys_fgd1.root",4);
  draw->SetStackFillStyle(3254);

  //=== Initialize Experiment class 
  Experiment exp_fgd1("nd280");
 
  //=== Magnet MC samples
  DataSample* magnet_mc_run5c = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD1/MC/numubkgmultipi_run5cmc_500toys_fgd1.root");
  DataSample* magnet_mc_run6  = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD1/MC/numubkgmultipi_run6mc_500toys_fgd1.root");
 
  //=== Run5c sample
  SampleGroup run5c("run5c");
  run5c.AddMCSample("magnet",magnet_mc_run5c);
  
  //=== Run6 sample
  SampleGroup run6("run6");
  run6.AddMCSample("magnet",magnet_mc_run6);
  
  //=== Sand MC samples
  DataSample* sand_mc_run5 = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD1/MC/numubkgmultipi_sandmuon_500toys_fgd1.root");
  DataSample* sand_mc_run6 = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD1/MC/numubkgmultipi_sandmuon_500toys_fgd1.root");
  run5c.AddMCSample("sand",sand_mc_run5);
  run6.AddMCSample("sand",sand_mc_run6);

  exp_fgd1.AddSampleGroup( "run5c" , run5c );
  exp_fgd1.AddSampleGroup( "run6"  , run6  );

  //=== Initialize Experiment class 
  Experiment exp_fgd2("nd280");
 
  //=== Magnet MC samples
  DataSample* magnet_mc_run5c_fgd2 = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD2/MC/numubkgmultipi_run5cmc_500toys_fgd2.root");
  DataSample* magnet_mc_run6_fgd2  = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD2/MC/numubkgmultipi_run6mc_500toys_fgd2.root");
 
  //=== Run5c sample
  SampleGroup run5c_fgd2("run5c");
  run5c_fgd2.AddMCSample("magnet",magnet_mc_run5c_fgd2);
  
  //=== Run6 sample
  SampleGroup run6_fgd2("run6");
  run6_fgd2.AddMCSample("magnet",magnet_mc_run6_fgd2);
  
  //=== Sand MC samples
  DataSample* sand_mc_run5_fgd2 = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD2/MC/numubkgmultipi_sandmuon_500toys_fgd2.root");
  DataSample* sand_mc_run6_fgd2 = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD2/MC/numubkgmultipi_sandmuon_500toys_fgd2.root");
  
  run5c_fgd2.AddMCSample("sand",sand_mc_run5_fgd2);
  run6_fgd2.AddMCSample("sand",sand_mc_run6_fgd2);  

  exp_fgd2.AddSampleGroup( "run5c" , run5c_fgd2 );
  exp_fgd2.AddSampleGroup( "run6"  , run6_fgd2  );

  const int NSamples = 3;
  string cut_sample[NSamples]  = { "accum_level[][0]>8" , "accum_level[][1]>8" , "accum_level[][2]>7" };
 
  string sample_name[NSamples] = {    "CC-0Pi"      ,      "CC-1PiPlus"    ,       "CC-Other"     };

  string opt[]       = { "SSYS" , "SSYS" , "SSYS" , "SSYS" , "SSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS", "SYS NWS11 WC11"};

  string syst_name[] = { "bfield_syst" , "momresol_syst" , "momscale_syst" , "tpcpid_syst"  , "fgdpid_syst" , "chargeideff_syst" , "tpctrackeff_syst" , "tpcfgdmatcheff_syst" , "tpcclustereff_syst" , "michel_syst" , "fgdhybridtrackeff_syst" , "pileup_syst" ,  "fgdmass_syst" , "oofv_syst" , "sipion_syst" , "siproton_syst" , "sandmu_syst" , "all_syst"};

  const int NSyst = sizeof(syst_name)/sizeof(string);

  //======================================================================================================================================

  
  //======================================================================================================================================
  string _sample_name;
  string _syst_name;
  string _cut;

  for(int isy=0; isy<NSyst; isy++){
    _syst_name=syst_name[isy];
    if(_syst_name!="sandmu_syst"){
      exp_fgd1.GetMCSample("run5c" , "magnet")->SetCurrentTree(syst_name[isy]);
      exp_fgd1.GetMCSample("run6"  , "magnet")->SetCurrentTree(syst_name[isy]);
      exp_fgd2.GetMCSample("run5c" , "magnet")->SetCurrentTree(syst_name[isy]);
      exp_fgd2.GetMCSample("run6"  , "magnet")->SetCurrentTree(syst_name[isy]);
    }
    else{
      exp_fgd1.GetMCSample("run5c" ,  "sand")->SetCurrentTree("sandmu_syst");
      exp_fgd1.GetMCSample("run6"  ,  "sand")->SetCurrentTree("sandmu_syst");
      exp_fgd2.GetMCSample("run5c" ,  "sand")->SetCurrentTree("sandmu_syst");
      exp_fgd2.GetMCSample("run6"  ,  "sand")->SetCurrentTree("sandmu_syst");
    }

    string _opt;
    _opt=opt[isy];
    for (int is=0; is<NSamples; is++){
      _sample_name=sample_name[is];
      _cut=cut_sample[is];
      
      if(isy==NSyst-1)
	draw->DrawRelativeErrors(exp_fgd1,"selmu_mom/1000",20,0,5,(_cut+" && weight_syst_total<100").c_str(),"",(_opt+" NODRAW").c_str());
      else 
	draw->DrawRelativeErrors(exp_fgd1,"selmu_mom/1000",20,0,5,_cut.c_str(),"",(_opt+" NODRAW").c_str());
      
      TH1D* hFGD1 = draw->GetLastHisto();            
      
      if(isy==NSyst-1)
	draw->DrawRelativeErrors(exp_fgd2,"selmu_mom/1000",20,0,5,(_cut+" && weight_syst_total<100").c_str(),"",(_opt+" NODRAW").c_str()); 
      else 
	draw->DrawRelativeErrors(exp_fgd2,"selmu_mom/1000",20,0,5,_cut.c_str(),"",(_opt+" NODRAW").c_str());      
      
      TH1D* hFGD2 = draw->GetLastHisto();
      
      float vmax0 = TMath::Max(-999.,hFGD1->GetMaximum());
      float vmax1 = TMath::Max(vmax0,hFGD2->GetMaximum());
      float vmax  = TMath::Max(vmax0,vmax1);
      
      hFGD1->GetYaxis()->SetRangeUser(0.,vmax*1.1);
      hFGD2->GetYaxis()->SetRangeUser(0.,vmax*1.1);
      
      TLegend *leg = new TLegend(0.35,0.90,0.86,0.98);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetFillColor(0);
      leg->SetLineColor(1);
      leg->SetNColumns(2);
      leg->AddEntry(hFGD1,"FGD1 sample","lp");
      leg->AddEntry(hFGD2,"FGD2 sample","l");
      
      hFGD1->SetMarkerStyle(21);
      hFGD1->SetMarkerColor(2);
      hFGD1->SetLineColor(kBlue);
      hFGD1->GetXaxis()->SetTitle("Muon Momentum [GeV/c]");
      hFGD1->GetYaxis()->SetTitle("Relative Error");
      hFGD1->Draw("HP");
      hFGD2->SetLineColor(kCyan);
      hFGD2->GetXaxis()->SetTitle("Muon Momentum [GeV/c]");
      hFGD2->GetYaxis()->SetTitle("Relative Error");
      hFGD2->Draw("SAME");
      leg->Draw("SAME");
      
      gPad->Print(("plots/RelativeError_"+_sample_name+"_"+_syst_name+"_momentum.pdf").c_str());
      
    }
  }
}

