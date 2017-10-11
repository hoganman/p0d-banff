#include "TFile.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include <iostream>
#include "TObjArray.h"
#include "TObjString.h"
#include "TH2D.h"
#include <stdlib.h>
#include "TDecompChol.h"

///Load in the old NIWG covariance file, and alter the contents of the CCQE
//parameters with a new covariance and new central values for the carbon
//parameters.
//Usage: ChangeXSecCovarianceFile.exe oldCovarianceFile newCCQECovariance newCovarianceFile removeRPA useq3=0.9FittedPriorValues
int main(int argc, char** argv){

    if(argc != 6){
        std::cout << "Warning: Unexpected number of arguments." << std::endl;
    }


    TFile* inputFile = new TFile(argv[1]);
    TFile* newCCQECov = new TFile(argv[2]);
    TFile* outputFile = new TFile(argv[3], "RECREATE");
    bool removeRPA = atoi(argv[4]);
    bool useAlternateq3 = atoi(argv[5]);

    //Pull everything from the existing file.
    TMatrixDSym* xsec_cov = (TMatrixDSym*)(inputFile->Get("xsec_cov"));
    TVectorD* xsec_param_prior = (TVectorD*)(inputFile->Get("xsec_param_prior"));
    TVectorD* xsec_param_nom = (TVectorD*)(inputFile->Get("xsec_param_nom"));
    TVectorD* xsec_param_prior_unnorm = (TVectorD*)(inputFile->Get("xsec_param_prior_unnorm"));
    TVectorD* xsec_param_nom_unnorm = (TVectorD*)(inputFile->Get("xsec_param_nom_unnorm"));
    TVectorD* xsec_param_lb = (TVectorD*)(inputFile->Get("xsec_param_lb"));
    TVectorD* xsec_param_ub = (TVectorD*)(inputFile->Get("xsec_param_ub"));
    TObjArray* xsec_param_names = (TObjArray*)(inputFile->Get("xsec_param_names"));


    //First order of business:  The CCQE covariance is a TH2D.  It follows the
    //same indexing in bins 1-7 as the full covariance from entries 0-6. 
    //The CCQE covariance is called CCQE_covariance.
    TH2D* ccqeCov = (TH2D*)(newCCQECov->Get("CCQE_covariance"));

    for(int i = 0; i < 7; i++){

        for(int j = 0; j < 7; j++){

            (*xsec_cov)(i,j) = ccqeCov->GetBinContent(i + 1, j + 1);

        }
    }

    //Make sure the matrix is invertible by calculating a determinant and
    //verifying that it is non-zero.
    TMatrixDSym* test_cov = (TMatrixDSym*)(xsec_cov->Clone());
    double determinant = test_cov->Determinant();
    std::cout << "The determinant is: " << determinant << std::endl;

    //Check that the matrix is symmetric and positive definite by trying to
    //perform a Cholesky Decomposition.  (A matrix is symmetric positive
    //definite if and only if it has a Cholesky Decomposition.)
    TDecompChol* chol = new TDecompChol(*test_cov);

    if(!(chol->Decompose())){

        std::cout << "Cholesky Decomposition failed!  Matrix invalid." << std::endl;
    }

    //With the new covariance entries written in, we must make the following
    //modifications to the prior values.
    //MAQE = 1.15
    //MEC_C = 27%  NB: Keeping with the past file, this is saved as 0.27.
    //pF_C = 223
    //MAQE is index 0
    //pF_C is index 1
    //MEC_C is index 2
    //None of the upper and lower bounds change, just the prior and the
    //prior_unnorm.
    //These values just go straight into xsec_param_prior_unnorm at the correct
    //indices.
    (*xsec_param_prior_unnorm)(0) = 1.15; //MAQE = 1.15
    (*xsec_param_prior_unnorm)(1) = 223.0; //PF_C = 223
    (*xsec_param_prior_unnorm)(2) = 0.27; //MEC_C = 27% = 0.27

    //Callum's presentation gives the following alternate values for relRPA and
    //q3=0.9
    if(useAlternateq3){
        (*xsec_param_prior_unnorm)(0) = 1.17; //MAQE = 1.17
        (*xsec_param_prior_unnorm)(1) = 224.0; //PF_C = 224
        (*xsec_param_prior_unnorm)(2) = 0.38; //MEC_C = 38% = 0.38
    }

    //Now, divide these by NEUT nominal to put in the "prior" vector.
    for(int i = 0; i < 3; i++){

        (*xsec_param_prior)(i) = (*xsec_param_prior_unnorm)(i)/(*xsec_param_nom_unnorm)(i); 

    }


    //If we're not removing RPA, just save everything to file.
    outputFile->cd();

    if(!removeRPA){
        xsec_cov->Write("xsec_cov");
        xsec_param_prior->Write("xsec_param_prior");
        xsec_param_nom->Write("xsec_param_nom");
        xsec_param_prior_unnorm->Write("xsec_param_prior_unnorm");
        xsec_param_nom_unnorm->Write("xsec_param_nom_unnorm");
        xsec_param_lb->Write("xsec_param_lb");
        xsec_param_ub->Write("xsec_param_ub");
        xsec_param_names->Write("xsec_param_names",TObject::kSingleKey);

    }


    //Otherwise, we're removing RPA.  Make new containers with size
    //originalsize-2, and fill them with the original contents until they're
    //full, leaving out the last two entries.
    else{
    
        int newlen = xsec_cov->GetNrows() - 2;
        TMatrixDSym* xsec_covNoRPA = new TMatrixDSym(newlen);
        TVectorD* xsec_param_priorNoRPA = new TVectorD(newlen);
        TVectorD* xsec_param_nomNoRPA = new TVectorD(newlen);
        TVectorD* xsec_param_prior_unnormNoRPA = new TVectorD(newlen);
        TVectorD* xsec_param_nom_unnormNoRPA = new TVectorD(newlen);
        TVectorD* xsec_param_lbNoRPA = new TVectorD(newlen);
        TVectorD* xsec_param_ubNoRPA = new TVectorD(newlen);
        TObjArray* xsec_param_namesNoRPA = new TObjArray(newlen);

        for(int i = 0; i < newlen; i++){
            (*xsec_param_priorNoRPA)(i) = (*xsec_param_prior)(i);
            (*xsec_param_nomNoRPA)(i) = (*xsec_param_nom)(i);
            (*xsec_param_prior_unnormNoRPA)(i) = (*xsec_param_prior_unnorm)(i); 
            (*xsec_param_nom_unnormNoRPA)(i) =(*xsec_param_nom_unnorm)(i); 
            (*xsec_param_lbNoRPA)(i) = (*xsec_param_lb)(i); 
            (*xsec_param_ubNoRPA)(i) = (*xsec_param_ub)(i); 
            xsec_param_namesNoRPA->AddAt(xsec_param_names->At(i),i); 


            //Need to loop over j to copy the covariance over too.
            for(int j = 0; j < newlen; j++){
                (*xsec_covNoRPA)(i,j) = (*xsec_cov)(i,j);
            }
        }

        xsec_covNoRPA->Write("xsec_cov");
        xsec_param_priorNoRPA->Write("xsec_param_prior");
        xsec_param_nomNoRPA->Write("xsec_param_nom");
        xsec_param_prior_unnormNoRPA->Write("xsec_param_prior_unnorm");
        xsec_param_nom_unnormNoRPA->Write("xsec_param_nom_unnorm");
        xsec_param_lbNoRPA->Write("xsec_param_lb");
        xsec_param_ubNoRPA->Write("xsec_param_ub");
        xsec_param_namesNoRPA->Write("xsec_param_names",TObject::kSingleKey);


    }

    outputFile->Close();
}
