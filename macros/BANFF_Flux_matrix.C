void BANFF_Flux_matrix()
{
    TFile* _file1 = TFile::Open("$BANFFROOT/inputs/flux_covariance_banff_13av2.root");
    TMatrixT<double>* total_flux_cov = total_flux_cov->Clone("total_flux_cov");
    _file1->Close();
    TCanvas* c = new TCanvas("c", "", 1000, 800);
    c->SetLogz(1);
    c->cd();
    total_flux_cov->Draw("COLZ");
    TH2D* cov_th2d = c->GetPrimitive("TMatrixDBase")->Clone("cov_th2d");
    for(int i = 1; i<=100; ++i){ cov_th2d->GetXaxis()->SetBinLabel(i, ""); cov_th2d->GetYaxis()->SetBinLabel(i, "");}
    cov_th2d->GetXaxis()->SetBinLabel(1,  "ND FHC #nu_{#mu}");       cov_th2d->GetYaxis()->SetBinLabel(1,  "ND FHC #nu_{#mu}");
    cov_th2d->GetXaxis()->SetBinLabel(12, "ND FHC #bar{#nu}_{#mu}"); cov_th2d->GetYaxis()->SetBinLabel(12, "ND FHC #bar{#nu}_{#mu}");
    cov_th2d->GetXaxis()->SetBinLabel(17, "ND FHC #nu_{e}");         cov_th2d->GetYaxis()->SetBinLabel(17, "ND FHC #nu_{e}");
    cov_th2d->GetXaxis()->SetBinLabel(24, "ND FHC #bar{#nu}_{e}");   cov_th2d->GetYaxis()->SetBinLabel(24, "ND FHC #bar{#nu}_{e}");
    cov_th2d->GetXaxis()->SetBinLabel(26, "ND RHC #nu_{#mu}");       cov_th2d->GetYaxis()->SetBinLabel(26, "ND RHC #nu_{#mu}");
    cov_th2d->GetXaxis()->SetBinLabel(31, "ND RHC #bar{#nu}_{#mu}"); cov_th2d->GetYaxis()->SetBinLabel(31, "ND RHC #bar{#nu}_{#mu}");
    cov_th2d->GetXaxis()->SetBinLabel(44, "ND RHC #bar{#nu}_{e}");   cov_th2d->GetYaxis()->SetBinLabel(44, "ND RHC #bar{#nu}_{e}");
    cov_th2d->GetXaxis()->SetBinLabel(42, "ND RHC #nu_{e}");         cov_th2d->GetYaxis()->SetBinLabel(42, "ND RHC #nu_{e}");
    cov_th2d->GetXaxis()->SetBinLabel(44, "ND RHC #bar{#nu}_{e}");   cov_th2d->GetYaxis()->SetBinLabel(44, "ND RHC #bar{#nu}_{e}");
    cov_th2d->GetXaxis()->SetBinLabel(50+1,  "SK FHC #nu_{#mu}");       cov_th2d->GetYaxis()->SetBinLabel(50+1,  "SK FHC #nu_{#mu}");
    cov_th2d->GetXaxis()->SetBinLabel(50+12, "SK FHC #bar{#nu}_{#mu}"); cov_th2d->GetYaxis()->SetBinLabel(50+12, "SK FHC #bar{#nu}_{#mu}");
    cov_th2d->GetXaxis()->SetBinLabel(50+17, "SK FHC #nu_{e}");         cov_th2d->GetYaxis()->SetBinLabel(50+17, "SK FHC #nu_{e}");
    cov_th2d->GetXaxis()->SetBinLabel(50+24, "SK FHC #bar{#nu}_{e}");   cov_th2d->GetYaxis()->SetBinLabel(50+24, "SK FHC #bar{#nu}_{e}");
    cov_th2d->GetXaxis()->SetBinLabel(50+26, "SK RHC #nu_{#mu}");       cov_th2d->GetYaxis()->SetBinLabel(50+26, "SK RHC #nu_{#mu}");
    cov_th2d->GetXaxis()->SetBinLabel(50+31, "SK RHC #bar{#nu}_{#mu}"); cov_th2d->GetYaxis()->SetBinLabel(50+31, "SK RHC #bar{#nu}_{#mu}");
    cov_th2d->GetXaxis()->SetBinLabel(50+44, "SK RHC #bar{#nu}_{e}");   cov_th2d->GetYaxis()->SetBinLabel(50+44, "SK RHC #bar{#nu}_{e}");
    cov_th2d->GetXaxis()->SetBinLabel(50+42, "SK RHC #nu_{e}");         cov_th2d->GetYaxis()->SetBinLabel(50+42, "SK RHC #nu_{e}");
    cov_th2d->GetXaxis()->SetBinLabel(50+44, "SK RHC #bar{#nu}_{e}");   cov_th2d->GetYaxis()->SetBinLabel(50+44, "SK RHC #bar{#nu}_{e}");
    cov_th2d->Draw("COLZ");
    interface.SaveCanvasAs(c, "BANFF_Flux_matrix");
}
