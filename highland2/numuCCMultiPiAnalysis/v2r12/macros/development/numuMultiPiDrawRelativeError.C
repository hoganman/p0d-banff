void numuMultiPiDrawRelativeError(){

  //==============================================================================================================================================================
  //=== Initialize DrawingTools
  DrawingTools* draw = new DrawingTools("/Users/alexander/Science/T2K/Software/ANALYSIS_HIGHLAND/Files/Prod6/MCP/Syst_Lorenzo_2017_10_25/NuMu_run4a_FHC_MultiPi_FGD1_single_syst.root",4);
  draw->SetStackFillStyle(3254);

  //=== Initialize Experiment class 
  Experiment exp_fgd1("nd280");

  //=== Magnet MC samples
  DataSample* magnet_mc_run4a = new DataSample("/Users/alexander/Science/T2K/Software/ANALYSIS_HIGHLAND/Files/Prod6/MCP/Syst_Lorenzo_2017_10_25/NuMu_run4a_FHC_MultiPi_FGD1_single_syst.root");
  DataSample* magnet_mc_run4w = new DataSample("/Users/alexander/Science/T2K/Software/ANALYSIS_HIGHLAND/Files/Prod6/MCP/Syst_Lorenzo_2017_10_25/NuMu_run4w_FHC_MultiPi_FGD1_single_syst.root");


  //=== Run4w sample
  SampleGroup run4w("run4w");
  run4w.AddMCSample("magnet",magnet_mc_run4w);

  //=== Run4a sample
  SampleGroup run4a("run4a");
  run4a.AddMCSample("magnet",magnet_mc_run4a);

  //=== Sand MC samples
  //DataSample* sand_mc_run5 = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD1/MC/numubkgmultipi_sandmuon_500toys_fgd1.root");
  //DataSample* sand_mc_run6 = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD1/MC/numubkgmultipi_sandmuon_500toys_fgd1.root");
  //run5c.AddMCSample("sand",sand_mc_run5);
  //run6.AddMCSample("sand",sand_mc_run6);

  exp_fgd1.AddSampleGroup( "run4w" , run4w );
  exp_fgd1.AddSampleGroup( "run4a" , run4a );

  //=== Initialize Experiment class 
  Experiment exp_fgd2("nd280");

  //=== Magnet MC samples
  DataSample* magnet_mc_run4a_fgd2 = new DataSample("/Users/alexander/Science/T2K/Software/ANALYSIS_HIGHLAND/Files/Prod6/MCP/Syst_Lorenzo_2017_10_25/NuMu_run4a_FHC_MultiPi_FGD2_single_syst.root");
  DataSample* magnet_mc_run4w_fgd2 = new DataSample("/Users/alexander/Science/T2K/Software/ANALYSIS_HIGHLAND/Files/Prod6/MCP/Syst_Lorenzo_2017_10_25/NuMu_run4w_FHC_MultiPi_FGD2_single_syst.root");


  //=== Run4w sample
  SampleGroup run4w_fgd2("run4w");
  run4w_fgd2.AddMCSample("magnet",magnet_mc_run4w_fgd2);

  //=== Run2a sample
  SampleGroup run4a_fgd2("run4a");
  run4a_fgd2.AddMCSample("magnet",magnet_mc_run4a_fgd2);

  //=== Sand MC samples
  //DataSample* sand_mc_run5 = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD1/MC/numubkgmultipi_sandmuon_500toys_fgd1.root");
  //DataSample* sand_mc_run6 = new DataSample("/lustrehome/riccioc/hl2_output/numuBkgMultiPi/FGD1/MC/numubkgmultipi_sandmuon_500toys_fgd1.root");
  //run5c.AddMCSample("sand",sand_mc_run5);
  //run6.AddMCSample("sand",sand_mc_run6);

  exp_fgd2.AddSampleGroup( "run4w" , run4w_fgd2 );
  exp_fgd2.AddSampleGroup( "run4a" , run4a_fgd2 );

  const int NSamples = 3;
  string cut_sample[NSamples]  = { "accum_level[][0]>7" , "accum_level[][1]>7" , "accum_level[][2]>6" };

  string sample_name[NSamples] = {    "CC-0Pi"      ,      "CC-1PiPlus"    ,       "CC-Other"     };

  string opt[]       = { "SSYS" , "SSYS" , "SSYS" , "SSYS" , "SSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS", "SYS NWS11 WC11"};

  string syst_name[] = { "bfield_syst" , "momresol_syst" , "momscale_syst" , "tpcpid_syst"  , "fgdpid_syst" , "chargeideff_syst" , "tpctrackeff_syst" , "tpcfgdmatcheff_syst" , "tpcclustereff_syst" , "michel_syst" , "fgdhybridtrackeff_syst" , "pileup_syst" ,  "fgdmass_syst" , "oofv_syst" , "sipion_syst" , "siproton_syst" , "sandmu_syst" , "all_syst"};

  const int NSyst = sizeof(syst_name)/sizeof(string);
  
  std::cout << " ***** Total systs ***** " << NSyst << " ***** " << std::endl;
   
  std::string wcut = " && weight_syst_total<10 && weight_syst_total>=0";

  //======================================================================================================================================


  //======================================================================================================================================
  string _sample_name;
  string _syst_name;
  string _cut;

  TCanvas* c2 = new TCanvas("c2", "c2");

  c2->Print("plots/RelativeErrors_momentum.pdf["); 
  c2->Divide(NSamples/2+1, NSamples/2+1);

  for(int isy=0; isy<NSyst; isy++){
    
    _syst_name = syst_name[isy];
    std::cout << " syst " << _syst_name << std::endl;
    
  
    if (_syst_name == "sandmu_syst"){
      //exp_fgd1.GetMCSample("run5c" ,  "sand")->SetCurrentTree("sandmu_syst");
      //exp_fgd1.GetMCSample("run6"  ,  "sand")->SetCurrentTree("sandmu_syst");
      //exp_fgd2.GetMCSample("run5c" ,  "sand")->SetCurrentTree("sandmu_syst");
      //exp_fgd2.GetMCSample("run6"  ,  "sand")->SetCurrentTree("sandmu_syst");
      continue;
    }
    else {
      exp_fgd1.GetMCSample("run4w" , "magnet")->SetCurrentTree(syst_name[isy]);
      exp_fgd1.GetMCSample("run4a"  ,"magnet")->SetCurrentTree(syst_name[isy]);
      exp_fgd2.GetMCSample("run4w" , "magnet")->SetCurrentTree(syst_name[isy]);
      exp_fgd2.GetMCSample("run4a"  ,"magnet")->SetCurrentTree(syst_name[isy]);
    }


    string _opt;
    _opt=opt[isy];
    c2->Clear("D");
    c2->Draw();
    for (int is=0; is<NSamples; is++){

      c2->cd(is+1);

      _sample_name=sample_name[is];
      _cut=cut_sample[is];
      std::string _wcut == "";
      if (_opt.find("WS") != std::string::npos)
        _wcut = wcut;

      draw->DrawRelativeErrors(exp_fgd1,"selmu_mom/1000",20,0,5,(_cut + _wcut).c_str(),"",(_opt+" NODRAW").c_str());

      TH1* hFGD1 = draw->GetLastHisto();            

      draw->DrawRelativeErrors(exp_fgd2,"selmu_mom/1000",20,0,5,(_cut + _wcut).c_str(),"",(_opt+" NODRAW").c_str());

      TH1* hFGD2 = draw->GetLastHisto();

      float vmax0 = TMath::Max(-999.,hFGD1->GetMaximum());
      float vmax1 = TMath::Max(vmax0,hFGD2->GetMaximum());
      float vmax  = TMath::Max(vmax0,vmax1);

      hFGD1->GetYaxis()->SetRangeUser(0.,vmax*1.1);
      hFGD2->GetYaxis()->SetRangeUser(0.,vmax*1.1);

      TLegend *leg = new TLegend(0.45,0.90,0.96,0.98);
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
      hFGD1->SetTitle((_sample_name+"_"+_syst_name).c_str());
      hFGD1->Draw("HP");
      hFGD2->SetLineColor(kCyan);
      hFGD2->GetXaxis()->SetTitle("Muon Momentum [GeV/c]");
      hFGD2->GetYaxis()->SetTitle("Relative Error"); 
      hFGD2->SetTitle((_sample_name+"_"+_syst_name).c_str());
      hFGD2->Draw("SAME");
      leg->Draw("SAME");
      TText *t = new TText();
      t-> SetNDC();
      t -> SetTextFont(22);
      t -> SetTextColor(1);
      t -> SetTextSize(0.04);
      t -> SetTextAlign(22);
      t -> SetTextAngle(0);
      t -> DrawText(0.2, 0.94, (_sample_name+"_"+_syst_name).c_str());

      gPad->Modified();
      gPad->Update();
    }  
    c2->Print("plots/RelativeErrors_momentum.pdf"); 
  }

  c2->Print("plots/RelativeErrors_momentum.pdf]"); 
}

