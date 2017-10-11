




void ComparePsycheHighland(){

  /// This macro makes a comparison of a highland / psyche selection
  /// Use RunSyst_New.exe and the highland analysis to create the file
  /// Update the root file path
  /// Then, modify the accum_level and the selection Id to plot the muon momentum and angle
  
  
  TFile* highlandFile = new TFile("/data/lasorak/psyche/psyche_v3r26_2/psyche/psycheSteering/v3r18/src/outputH2.root", "READ");
  TFile* psycheFile   = new TFile("/data/lasorak/psyche/psyche_v3r26_2/psyche/psycheSteering/v3r18/src/output.root", "READ");
  TTree* Tree[2];
  Tree[0] = (TTree*)highlandFile->Get("default");
  Tree[1] = (TTree*)psycheFile  ->Get("all_syst");
  std::cout << "higlandTree tree has " << Tree[0]->GetEntries() << " entries." << std::endl;
  std::cout << "psycheTree  tree has " << Tree[1]->GetEntries() << " entries." << std::endl;

  TH1D *MuonMomentum[2];
  TH1D *MuonCosTheta[2];
  MuonCosTheta[0] = new TH1D("HighlandMuonCosTheta", ";#mu cos(#theta);n events", 10, -1, 1);
  MuonCosTheta[1] = new TH1D("PsycheMuonCosTheta",   ";#mu cos(#theta);n events", 10, -1, 1);
  MuonMomentum[0] = new TH1D("HighlandMuonMomentum", ";#mu momentum [MeV];n events", 10, 0, 5000);
  MuonMomentum[1] = new TH1D("PsycheMuonMomentum",   ";#mu momentum [MeV];n events", 10, 0, 5000);

  SelectionPsyche = new TH1D("SelectionPsyche",   "SelectionPsyche;SampleID;n events",   110, -10+0.5, 100+0.5);
  Tree[1]->Project("SelectionPsyche",   "SelectionNom", "");
  
  AccumLevelHighland = new TH1D("AccumLevelHighland",   "AccumLevelHighland;Accum level;n events",   10, 0.5, 10+0.5);
  Tree[0]->Project("AccumLevelHighland",   "accum_level", "");
  //std::string cutH = "(accum_level[0][0]>7) || (accum_level[0][1]>7) || (accum_level[0][2]>6)";
  std::string cutH = "(accum_level>0)";
  std::string cutP = "SelectionNom!=0";
  
  Tree[0]->Project("HighlandMuonMomentum", "selmu_mom",    cutH.c_str());
  Tree[1]->Project("PsycheMuonMomentum",   "LeptonMomNom", cutP.c_str());

  Tree[0]->Project("HighlandMuonCosTheta", "selmu_costheta", cutH.c_str());
  Tree[1]->Project("PsycheMuonCosTheta",   "LeptonCosNom",   cutP.c_str());
  
  MuonCosTheta[0]->SetLineColor(kBlue);
  MuonCosTheta[1]->SetLineColor(kRed);
  MuonMomentum[0]->SetLineColor(kBlue);
  MuonMomentum[1]->SetLineColor(kRed);

  MuonCosTheta[0]->SetLineWidth(2);
  MuonCosTheta[1]->SetLineWidth(2);
  MuonMomentum[0]->SetLineWidth(2);
  MuonMomentum[1]->SetLineWidth(2);
  gStyle->SetOptStat(11);
  
  double minX = 0.5;
  double maxX = 0.7;
  double minY = 0.7;
  double maxY = 0.9;
  
  TCanvas* c0 = new TCanvas();
  TLegend* leg0 = new TLegend(minX, minY, maxX, maxY);
  leg0->AddEntry(MuonCosTheta[0], "Highland", "L");
  leg0->AddEntry(MuonCosTheta[1], "Psyche"  , "L");
  leg0->SetFillColor(0);
  MuonCosTheta[0]->Draw();
  c0->Modified(); c0->Update();
  TPaveStats *stats =(TPaveStats*)c0->GetPrimitive("stats");
  stats->SetName("h1stats");
  stats->SetY1NDC(.8);
  stats->SetY2NDC(.9);
  stats->SetX1NDC(.1);
  stats->SetX2NDC(.3);
  stats->SetTextColor(MuonCosTheta[0]->GetLineColor());
  MuonCosTheta[1]->Draw("SAMES");
  c0->Update();
  TPaveStats *stats2 = (TPaveStats*)c0->GetPrimitive("stats");
  stats2->SetName("h2stats");
  stats2->SetY1NDC(.7);
  stats2->SetY2NDC(.8);
  stats2->SetX1NDC(.1);
  stats2->SetX2NDC(.3);
  stats2->SetTextColor(MuonCosTheta[1]->GetLineColor());
  leg0->Draw();
  c0->SaveAs("CosTheta.png");

  TCanvas* c1 = new TCanvas();
  TLegend* leg1 = new TLegend(minX, minY, maxX, maxY);
  leg1->AddEntry(MuonCosTheta[0], "Highland", "L");
  leg1->AddEntry(MuonCosTheta[1], "Psyche"  , "L");
  leg1->SetFillColor(0);
  MuonMomentum[0]->Draw();
  c1->Modified(); c1->Update();
  TPaveStats *stats3 =(TPaveStats*)c1->GetPrimitive("stats");
  stats3->SetName("h1stats");
  stats3->SetY1NDC(.8);
  stats3->SetY2NDC(.9);
  stats3->SetX1NDC(.7);
  stats3->SetX2NDC(.9);
  stats3->SetTextColor(MuonCosTheta[0]->GetLineColor());
  MuonMomentum[1]->Draw("SAMES");
  c1->Update();
  TPaveStats *stats4 = (TPaveStats*)c1->GetPrimitive("stats");
  stats4->SetName("h2stats");
  stats4->SetY1NDC(.7);
  stats4->SetY2NDC(.8);
  stats4->SetX1NDC(.7);
  stats4->SetX2NDC(.9);
  stats4->SetTextColor(MuonCosTheta[1]->GetLineColor());
  leg0->Draw();
  leg1->Draw();
  c1->SaveAs("Momentum.png");

  TCanvas* c2 = new TCanvas();
  SelectionPsyche->Draw();
  c2->SaveAs("Selection.png");

  TCanvas* c3 = new TCanvas();
  AccumLevelHighland->Draw();
  c3->SaveAs("AccumLevelHighland.png");
}
