#include "TFile.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include "TAxis.h"

int main(){

    //Load in the BANFFv2 fit_input file.
    TFile* inputFile = new TFile("fit_input_2013_v4.root");

    //Load in the parts we need to rename.
    TAxis* obs1_axis =  (TAxis*)(inputFile->Get("obs1_axis"));
    TAxis* obs2_axis = (TAxis*)(inputFile->Get("obs2_axis"));
    TMatrixDSym* det_cov = (TMatrixDSym*)(inputFile->Get("det_cov"));
    TVectorD* det_weights = (TVectorD*)(inputFile->Get("det_weights"));

    //Create the output file.
    TFile* outputFile = new TFile("obsNorm_2013_v4.root","RECREATE");

    //Now copy everything that need.
    TAxis* cc0pi_p_axis = (TAxis*)(obs1_axis->Clone());
    TAxis* cc0pi_th_axis = (TAxis*)(obs2_axis->Clone());

    TAxis* cc1pi_p_axis = (TAxis*)(obs1_axis->Clone());
    TAxis* cc1pi_th_axis = (TAxis*)(obs2_axis->Clone());

    TAxis* ccOth_p_axis = (TAxis*)(obs1_axis->Clone());
    TAxis* ccOth_th_axis = (TAxis*)(obs2_axis->Clone());

    TMatrixDSym* obsNorm_cov = (TMatrixDSym*)(det_cov->Clone());
    TVectorD* obsNorm_weights = (TVectorD*)(det_weights->Clone());

    //Now save the new ones to the output file.
    obsNorm_cov->Write("obsNorm_cov");
    obsNorm_weights->Write("obsNorm_weights");
    
    cc0pi_p_axis->Write("cc0pi_p_axis");
    cc0pi_th_axis->Write("cc0pi_th_axis");

    cc1pi_p_axis->Write("cc1pi_p_axis");
    cc1pi_th_axis->Write("cc1pi_th_axis");

    ccOth_p_axis->Write("ccOth_p_axis");
    ccOth_th_axis->Write("ccOth_th_axis");

    //With all the information we need written out, close the file.
    outputFile->Close();
    inputFile->Close();

}
