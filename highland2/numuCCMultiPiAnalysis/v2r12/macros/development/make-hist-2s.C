/// run in a root session
/// .L makeh.C
/// makeh();
/// 2*20bins version
void makeh(){

// input: output from highland2 selection
    DrawingTools draw("/lustrehome/magaletti/SummerAnalysis_oa_Run5c_Data_FGD1_AntiNuMu.root");
//    MultiAnalysisTools data("/lustrehome/magaletti/results/DATA/Run6_6C_highland/data_run6_new2.root");
//    MultiAnalysisTools old("/lustrehome/magaletti/results/DATA/Run5c_6C_highland/data_run5c.root");
    //DataSample data("/lustrehome/magaletti/results/DATA/Run6_6C_antinu_FlatTree_highland2/prova_run6_rdp.root");//FGD1
    //DataSample old("/lustrehome/magaletti/results/DATA/Run5_6C_antinu_FlatTree_highland2/prova_run5c_rdp.root");//FGD1
    DataSample data("/lustrehome/magaletti/SummerAnalysis_oa_Run6b_6c_6d_Data_FGD1_AntiNuMu.root");//FGD2
    DataSample old("/lustrehome/magaletti/SummerAnalysis_oa_Run5c_Data_FGD1_AntiNuMu.root");//FGD2


    TFile * f_output = new TFile("hist2.root","RECREATE");

// binning as in old BANFF fit 
    Float_t thetabins[5] = {-1., 0.84, 0.9, 0.94, 1.};
    Float_t mombins[6] = {0., 400., 500., 700., 900., 200000.};
    Float_t mombinsplot[6] = {0., 400., 500., 700., 900., 2000.};

    TH2F *hrun5ccqe = new TH2F("hrun5ccqe","run5 ccqe;momentum [MeV];costheta",5,mombins,4,thetabins);
    TH2F *hrun5ccnqe = new TH2F("hrun5ccnqe","run5 ccnqe;momentum [MeV];costheta",5,mombins,4,thetabins);
    TH2F *hrun6ccqe = new TH2F("hrun6ccqe","run6 ccqe;momentum [MeV];costheta",5,mombins,4,thetabins);
    TH2F *hrun6ccnqe = new TH2F("hrun6ccnqe","run6 ccnqe;momentum [MeV];costheta",5,mombins,4,thetabins);
    TH1F *hrun5ccqemom = new TH1F("hrun5ccqemom","momentum run5 ccqe",5,mombins);
    TH1F *hrun5ccnqemom = new TH1F("hrun5ccnqemom","momentum run5 ccnqe",5,mombins);
    TH1F *hrun6ccqemom = new TH1F("hrun6ccqemom","momentum run6 ccqe",5,mombins);
    TH1F *hrun6ccnqemom = new TH1F("hrun6ccnqemom","momentum run6 ccnqe",5,mombins);
    TH1F *hrun5ccqeang = new TH1F("hrun5ccqeang","angle run5 ccqe",4,thetabins);
    TH1F *hrun5ccnqeang = new TH1F("hrun5ccnqeang","angle run5 ccnqe",4,thetabins);
    TH1F *hrun6ccqeang = new TH1F("hrun6ccqeang","angle run6 ccqe",4,thetabins);
    TH1F *hrun6ccnqeang = new TH1F("hrun6ccnqeang","angle run6 ccnqe",4,thetabins);
//    data.GetTree()->Draw("selmu_costheta:selmu_mom","accum_level>5 &&cut6");

    data.GetTree()->Project("hrun6ccqe","selmu_costheta:selmu_mom","accum_level[][0]>7");
    data.GetTree()->Project("hrun6ccnqe","selmu_costheta:selmu_mom","accum_level[][1]>7");
    old.GetTree()->Project("hrun5ccqe","selmu_costheta:selmu_mom","accum_level[][0]>7");
    old.GetTree()->Project("hrun5ccnqe","selmu_costheta:selmu_mom","accum_level[][1]>7");

    data.GetTree()->Project("hrun6ccqemom","selmu_mom","accum_level[][0]>7");
    data.GetTree()->Project("hrun6ccnqemom","selmu_mom","accum_level[][1]>7");
    old.GetTree()->Project("hrun5ccqemom","selmu_mom","accum_level[][0]>7");
    old.GetTree()->Project("hrun5ccnqemom","selmu_mom","accum_level[][1]>7");

    data.GetTree()->Project("hrun6ccqeang","selmu_costheta","accum_level[][0]>7");
    data.GetTree()->Project("hrun6ccnqeang","selmu_costheta","accum_level[][1]>7");
    old.GetTree()->Project("hrun5ccqeang","selmu_costheta","accum_level[][0]>7");
    old.GetTree()->Project("hrun5ccnqeang","selmu_costheta","accum_level[][1]>7");

    TH2F *h_run5ccqe = new TH2F("hprun5ccqe","run5 ccqe;momentum [MeV];costheta",5,mombinsplot,4,thetabins);
    TH2F *h_run5ccnqe = new TH2F("hprun5ccnqe","run5 ccnqe;momentum [MeV];costheta",5,mombinsplot,4,thetabins);
    TH2F *h_run6ccqe = new TH2F("hprun6ccqe","run6 ccqe;momentum [MeV];costheta",5,mombinsplot,4,thetabins);
    TH2F *h_run6ccnqe = new TH2F("hprun6ccnqe","run6 ccnqe;momentum [MeV];costheta",5,mombinsplot,4,thetabins);

    h_run5ccqe->Add(hrun5ccqe);
    h_run5ccnqe->Add(hrun5ccnqe);
    h_run6ccqe->Add(hrun6ccqe);
    h_run6ccnqe->Add(hrun6ccnqe);

// check for over/underflows
    cout << "Run5 ccqe: momentum under: " << hrun5ccqemom->GetBinContent(0) << " over: " << hrun5ccqemom->GetBinContent(6) << " ; angle under: " << hrun5ccqeang->GetBinContent(0) << " over: " << hrun5ccqeang->GetBinContent(5) << endl;
    cout << "Run5 ccnqe: momentum under: " << hrun5ccnqemom->GetBinContent(0) << " over: " << hrun5ccnqemom->GetBinContent(6) << " ; angle under: " << hrun5ccnqeang->GetBinContent(0) << " over: " << hrun5ccnqeang->GetBinContent(5) << endl;
    cout << "Run6 ccqe: momentum under: " << hrun6ccqemom->GetBinContent(0) << " over: " << hrun6ccqemom->GetBinContent(6) << " ; angle under: " << hrun6ccqeang->GetBinContent(0) << " over: " << hrun6ccqeang->GetBinContent(5) << endl;
    cout << "Run6 ccnqe: momentum under: " << hrun6ccnqemom->GetBinContent(0) << " over: " << hrun6ccnqemom->GetBinContent(6) << " ; angle under: " << hrun6ccnqeang->GetBinContent(0) << " over: " << hrun6ccnqeang->GetBinContent(5) << endl;

// output histograms for likelihood 

    TH1F* hrun5 = new TH1F("run5","bins for run5",40,0.,40.);
    TH1F* hrun6 = new TH1F("run6","bins for run6",40,0.,40.);

    for (Int_t imom=1; imom<6; imom++){
	for (Int_t itheta=1; itheta<5; itheta++){
	    Int_t ibin = (imom-1)*4+itheta;
	    hrun5->SetBinContent(ibin,hrun5ccqe->GetBinContent(imom,itheta));
	    hrun6->SetBinContent(ibin,hrun6ccqe->GetBinContent(imom,itheta));
	    //cout << ibin << " imom " << imom << " itheta " << itheta << " content5 " << hrun5ccqe->GetBinContent(imom,itheta)<< " 6 " << hrun6ccqe->GetBinContent(imom,itheta) << endl;
	}
    }

    for (Int_t imom=1; imom<6; imom++){
	for (Int_t itheta=1; itheta<5; itheta++){
	    Int_t ibin = (imom+4)*4+itheta;
	    hrun5->SetBinContent(ibin,hrun5ccnqe->GetBinContent(imom,itheta));
	    hrun6->SetBinContent(ibin,hrun6ccnqe->GetBinContent(imom,itheta));
	    //cout << ibin << " imom " << imom << " itheta " << itheta << " content5 " << hrun5ccnqe->GetBinContent(imom,itheta)<< " 6 " << hrun6ccnqe->GetBinContent(imom,itheta) << endl;
	}
    }

    double goodPOTrun5 = draw.GetGoodPOT(old);
    double goodPOTrun6 = draw.GetGoodPOT(data);
    double POTratio = goodPOTrun6/goodPOTrun5;
    std::cout<<"Good POT run5 "<<goodPOTrun5<<std::endl;
    std::cout<<"Good POT run6 "<<goodPOTrun6<<std::endl;
    TH1F* hrun5_w = new TH1F(*hrun5);
    hrun5_w->SetNameTitle("run5w","bins for run5, weighted");
    hrun5_w->Scale(POTratio);
    for (int i=1; i<41; i++) {
	hrun5_w->SetBinError(i,POTratio*sqrt(hrun5->GetBinContent(i)));
    }

f_output->Write();
}
