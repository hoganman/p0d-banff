void numuBkgMultiPiEvalSyst(TString run5c_MC, TString run6_MC, TString sandMC){

  //==============================================================================================================================================================
  //=== Initialize DrawingTools
  //                                                T2K style index      
  //                                                     / 
  DrawingTools* draw = new DrawingTools(run5c_MC.Data(),4);
  draw->SetStackFillStyle(3254);

  //=== Initialize Experiment class 
  Experiment exp("nd280");
 
  //=== Magnet MC samples
  DataSample* magnet_mc_run5c = new DataSample(run5c_MC.Data());
  DataSample* magnet_mc_run6  = new DataSample(run6_MC.Data());
 
  //=== Sand MC samples
  DataSample* sand_mc_run5 = new DataSample(sandMC.Data());
  DataSample* sand_mc_run6 = new DataSample(sandMC.Data());
  
  //=== Run5c sample
  SampleGroup run5c("run5c");
  run5c.AddMCSample("magnet",magnet_mc_run5c);
  run5c.AddMCSample("sand",sand_mc_run5);
  
  //=== Run6 sample
  SampleGroup run6("run6");
  run6.AddMCSample("magnet",magnet_mc_run6);
  run6.AddMCSample("sand",sand_mc_run6);

  exp.AddSampleGroup( "run5c" , run5c );
  exp.AddSampleGroup( "run6"  , run6  );
            
  const int NSamples = 3;
  string cut_sample[NSamples]  = { "accum_level[][0]>8" , "accum_level[][1]>8" , "accum_level[][2]>7" };
 
  string sample_name[NSamples] = {    "CC-0Pi"      ,      "CC-1PiPlus"    ,       "CC-Other"     };

  string opt[]       = { "SSYS" , "SSYS" , "SSYS" , "SSYS" , "SSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" , "WSYS" ,"WSYS" , "SYS NWS11 WC11"};

  string syst_name[] = { "bfield_syst" , "momresol_syst" , "momscale_syst" , "tpcpid_syst"  , "fgdpid_syst" , "chargeideff_syst" , "tpctrackeff_syst" , "tpcfgdmatcheff_syst" , "tpcclustereff_syst" , "michel_syst" , "fgdhybridtrackeff_syst" , "pileup_syst" ,  "fgdmass_syst" , "oofv_syst" , "sipion_syst" , "siproton_syst" ,"sandmu_syst", "all_syst"};

  const int NSyst = sizeof(syst_name)/sizeof(string);

  streambuf *psbuf, *backup;
  ofstream file_output;
  file_output.open("plots/FGD1_Syst.txt");
  backup = cout.rdbuf();       // back up cout's streambuf
  psbuf = file_output.rdbuf(); // get file's streambuf
  cout.rdbuf(psbuf);           // assign streambuf to cout
  
  Float_t Ntoys = 500.;
  double tot[NSamples],tot_corr[NSamples],tot_err[NSamples];
  double entries[NSamples],entries_corr[NSamples];

  for(int isy=0; isy<NSyst; isy++){
    if(syst_name[isy]!="sandmu_syst"){
      exp.GetMCSample("run5c" , "magnet")->SetCurrentTree(syst_name[isy]);
      exp.GetMCSample("run6"  , "magnet")->SetCurrentTree(syst_name[isy]);
    }
    else{
      exp.GetMCSample("run5c" ,  "sand")->SetCurrentTree("sandmu_syst");
      exp.GetMCSample("run6"  ,  "sand")->SetCurrentTree("sandmu_syst");
    }
    
    string _opt;
    _opt=opt[isy];

    for(int is=0; is<NSamples; is++){
      
      draw->Draw(exp,"0.",1,-1,1,"all",cut_sample[is],"",(_opt+" NODRAW").c_str());
      tot[is]     = draw->GetLastStackTotal()->Integral("");
      entries[is] = draw->GetLastStackTotal()->GetEntries()/Ntoys;
    
      if(_opt=="SSYS"){

	draw->DrawRelativeErrors(exp,"0.",1,-1,1,cut_sample[is],"",(_opt+" NODRAW").c_str());
	tot_err[is] = 100*draw->GetLastHisto()->Integral("");

      }
      else{
	if(syst_name[isy]=="all_syst"){//in order to prevent big weight
	  if(is==0){
	    draw->DrawRelativeErrors(exp,"selmu_mom/1000",20,0,10,"accum_level[][0]>8 && weight_syst_total<100","",(_opt+" NODRAW").c_str());
	    tot_err[is] = 100*draw->GetLastHisto()->Integral("")/20;
	  }
	  if(is==1){
	    draw->DrawRelativeErrors(exp,"selmu_mom/1000",20,0,10,"accum_level[][1]>8 && weight_syst_total<100","",(_opt+" NODRAW").c_str());
	    tot_err[is] = 100*draw->GetLastHisto()->Integral("")/20;
	  }
	  if(is==2){
	    draw->DrawRelativeErrors(exp,"selmu_mom/1000",20,0,10,"accum_level[][2]>7 && weight_syst_total<100","",(_opt+" NODRAW").c_str());
	    tot_err[is] = 100*draw->GetLastHisto()->Integral("")/20;
	  }
	}
	else{ 
	  draw->DrawRelativeErrors(exp,"0.",1,-1,1,cut_sample[is],"",(_opt+" NODRAW").c_str());
	  tot_err[is] = 100*draw->GetLastHisto()->Integral("");
	}
	
	draw->Draw(exp,"0.",1,-1,1,"all",cut_sample[is],"","WCORR NODRAW");
	tot_corr[is]     = draw->GetLastStackTotal()->Integral("");
	entries_corr[is] = draw->GetLastStackTotal()->GetEntries()/Ntoys;
      
      }
    }
    cout<<fixed; 
    cout<<setprecision(7);
    cout<< "  ===============================================================================  "<<endl;
    cout<< "                 "+sample_name[0]+"&" << "  "+sample_name[1]+" &" << "  "+sample_name[2]+" &" <<"  "<<endl;
    cout<< syst_name[isy]+" " << tot_err[0] << "       " <<  tot_err[1] << "       " <<  tot_err[2] << "       " <<endl;
    for (int is=0; is<NSamples; is++){
      cout<< "     Total               "<< tot[is] << " Total entries (Mean)            " << entries[is] <<endl;
      if(_opt=="WSYS" || _opt=="SYS NWS11 WC11") cout<< "     Total correction   " << tot_corr[is] << " Total entries correction (Mean) " << entries_corr[is] <<endl;
    }
    cout<< "  ===============================================================================  "<<endl;
  }
  
  cout.rdbuf(backup);// restore cout's original streambuf
  file_output.close();
}
  
