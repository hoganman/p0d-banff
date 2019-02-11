void CreateEffPur(const char* defaultTreeFilename)
{
    TFile* _file0 = TFile::Open(defaultTreeFilename);
    TTree* truth = static_cast<TTree*>(_file0->Get("truth"));
    TTree* default = static_cast<TTree*>(_file0->Get("default"));
    TCanvas* canvas = new TCanvas("canvas", "", 800, 600);
    canvas->cd();
    canvas->SetGridx();
    canvas->SetGridy();
    DrawingTools draw(defaultTreeFilename);

    draw.SetTitleX("True muon momentum [GeV/c]");
    draw.SetTitleY("#nu_{#mu} CC 1-track Eff/Purity");
    draw.DrawEff(truth, "truelepton_mom*1e-3", 50, 0, 5, "accum_level[][0]>4", "reaction==0", "", "", "#nu_{#mu}CCQE Efficiency");
    draw.DrawEff(default, "truelepton_mom*1e-3", 50, 0, 5, "reaction==0", "accum_level[][0]>4", "SAME", "", "#nu_{#mu}CCQE Purity");
    draw.DrawEff(truth, "truelepton_mom*1e-3", 50, 0, 5, "accum_level[][0]>4", "topology_no1pi==0", "SAME", "", "#nu_{#mu}CC-0#pi Efficiency");
    draw.DrawEff(default, "truelepton_mom*1e-3", 50, 0, 5, "topology_no1pi==0", "accum_level[][0]>4", "SAME", "", "#nu_{#mu}CC-0#pi Purity");

    canvas->SaveAs("numuWaterOutFHCCC1track_recoP_effpur_reactionCCQE.root");
    canvas->SaveAs("numuWaterOutFHCCC1track_recoP_effpur_reactionCCQE.png");

    draw.SetTitleX("True muon momentum [GeV/c]");
    draw.SetTitleY("#nu_{#mu} CC N-tracks Eff/Purity");
    draw.DrawEff(truth, "truelepton_mom*1e-3", 50, 0, 5, "accum_level[][1]>4", "reaction>0&&reaction<=3", "", "", "#nu_{#mu}CCnQE Efficiency");
    draw.DrawEff(default, "truelepton_mom*1e-3", 50, 0, 5, "reaction>0&&reaction<=3", "accum_level[][1]>4", "SAME", "", "#nu_{#mu}CCnQE Purity");
    draw.DrawEff(truth, "truelepton_mom*1e-3", 50, 0, 5, "accum_level[][1]>4", "topology_no1pi==1", "SAME", "", "#nu_{#mu}CC-N#pi Efficiency");
    draw.DrawEff(default, "truelepton_mom*1e-3", 50, 0, 5, "topology_no1pi==1", "accum_level[][1]>4", "SAME", "", "#nu_{#mu}CC-N#pi Purity");

    canvas->SaveAs("numuWaterOutFHCCCNtrack_recoP_effpur_reactionCCnQE.root");
    canvas->SaveAs("numuWaterOutFHCCCNtrack_recoP_effpur_reactionCCnQE.png");

    draw.SetTitleX("True muon (1-cos#theta)");
    draw.SetTitleY("#nu_{#mu} CC 1-track Eff/Purity");
    draw.DrawEff(truth, "1-truelepton_costheta", 50, 0, 1, "accum_level[][0]>4", "reaction==0", "", "", "#nu_{#mu}CCQE Efficiency");
    draw.DrawEff(default, "1-truelepton_costheta", 50, 0, 1, "reaction==0", "accum_level[][0]>4", "SAME", "", "#nu_{#mu}CCQE Purity");
    draw.DrawEff(truth, "1-truelepton_costheta", 50, 0, 1, "accum_level[][0]>4", "topology_no1pi==0", "SAME", "", "#nu_{#mu}CC-0#pi Efficiency");
    draw.DrawEff(default, "1-truelepton_costheta", 50, 0, 1, "topology_no1pi==0", "accum_level[][0]>4", "SAME", "", "#nu_{#mu}CC-0#pi Purity");

    canvas->SaveAs("numuWaterOutFHCCC1track_recocostheta_effpur_reactionCCQE.root");
    canvas->SaveAs("numuWaterOutFHCCC1track_recocostheta_effpur_reactionCCQE.png");

    draw.SetTitleX("True muon (1-cos#theta)");
    draw.SetTitleY("#nu_{#mu} CC N-tracks Eff/Purity");
    draw.DrawEff(truth, "1-truelepton_costheta", 50, 0, 1, "accum_level[][1]>4", "reaction>0&&reaction<=3", "", "", "#nu_{#mu}CCnQE Efficiency");
    draw.DrawEff(default, "1-truelepton_costheta", 50, 0, 1, "reaction>0&&reaction<=3", "accum_level[][1]>4", "SAME", "", "#nu_{#mu}CCnQE Purity");
    draw.DrawEff(truth, "1-truelepton_costheta", 50, 0, 1, "accum_level[][1]>4", "topology_no1pi==1", "SAME", "", "#nu_{#mu}CC-N#pi Efficiency");
    draw.DrawEff(default, "1-truelepton_costheta", 50, 0, 1, "topology_no1pi==1", "accum_level[][1]>4", "SAME", "", "#nu_{#mu}CC-N#pi Purity");

    canvas->SaveAs("numuWaterOutFHCCCNtrack_recocostheta_effpur_reactionCCnQE.root");
    canvas->SaveAs("numuWaterOutFHCCCNtrack_recocostheta_effpur_reactionCCnQE.png");

    _file->Close();

}
