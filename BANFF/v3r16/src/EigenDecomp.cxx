#include <iostream>
#include <assert.h>
#include <algorithm>
#include <math.h>

#include "EigenDecomp.hxx"

EigenDecomp::EigenDecomp(TVectorD &params, TMatrixDSym &cov)
{  
    npars = params.GetNrows();
    orig_params = new TVectorD(params);
    orig_cov = new TMatrixDSym(cov);
    beta_nom = new TVectorD(params);
    eigen_vectors = new TMatrixD(npars,npars);
    eigen_vectorsT = new TMatrixD(npars,npars);
    eigen_cov = new TMatrixDSym(npars);

    Decompose();

}

EigenDecomp::~EigenDecomp()
{
    if(orig_cov!=NULL) orig_cov->Delete();
    if(eigen_vectors!=NULL) eigen_vectors->Delete();
    if(eigen_vectorsT!=NULL) eigen_vectorsT->Delete();
    if(eigen_cov!=NULL) eigen_cov->Delete();
    if(beta_nom!=NULL) beta_nom->Delete();
}

void EigenDecomp::GetDecomposition(TVectorD &decomp_params, TMatrixDSym &decomp_cov)
{
    if(beta_nom==NULL || eigen_cov==NULL){
        std::cerr << "No decomposition available" << std::endl;
        return;
    }

    //The decomposed parameters are the projection of the original parameters
    //into the eigenspace.
    decomp_params.ResizeTo(beta_nom->GetNrows());
    for(int i=0; i<beta_nom->GetNrows(); i++){
        decomp_params(i) = (*beta_nom)(i);
    }

    //The decomposed covariance submatrix is the diagonal matrix of the
    //eigenvalues.
    decomp_cov.ResizeTo(eigen_cov->GetNrows(),eigen_cov->GetNrows());
    for(int i=0; i<eigen_cov->GetNrows(); i++){

        for(int j=0; j<eigen_cov->GetNcols(); j++){

            decomp_cov(i,j) = (*eigen_cov)(i,j);

        }

    }

    return;
}

void EigenDecomp::Decompose(){

    //Get the eigenvalues and eigenvectors
    //Note: These are guaranteed to exist since real symmetric matrices (like a
    //covariance matrix) are always diagonalizable.
    //TMatrixDSym::EigenVectors returns a TMatrixD containing the eigen-vectors
    //ordered by descending eigen-values (and puts the eigenvalues in ev). 
    //Furthermore, TMatrixDSym calls TMatrixDSymEigen for this calculation, and
    //its documentation explains how the eigenvector matrix is orthogonal.
    TVectorD ev(npars);
    *eigen_vectors = orig_cov->EigenVectors(ev);
    *eigen_vectorsT = orig_cov->EigenVectors(ev);

    //Make the covariance with eigenvalues on the diagonal
    //eigen_cov was empty up until this point.  Now, put the eigenvalues on the
    //diagonal, and zero everywhere else.
    for(int i=0; i<eigen_cov->GetNrows(); i++){
        for(int j=0; j<eigen_cov->GetNcols(); j++){

            if(i==j){
                (*eigen_cov)(i,j) = ev(i);
            }

            else (*eigen_cov)(i,j) = 0.;
        }
    }

    //Make eigen_vectorsT the transpose of the eigenvector matrix supplied by
    //TMatrixDSym::EigenVectors().
    eigen_vectorsT->T();

    //Prior to this operation, beta_nom was just the TVectorD of prior values
    //for those parameters that have correlations with other parameters.
    //Since the eigenvector matrix is orthogonal, this has the function of
    //projecting the prior value vector into the eigenspace.
    (*beta_nom) *= (*eigen_vectorsT);
}

//Projects the vector of parameters from the eigenbasis of the covariance
//submatrix back to the basis of the original covariance submatrix.
//(Projecting from the original basis to the eigenbasis was done by multiplying
//by eigen_vectors^{T}.  Since eigen_vectors and eigen_vectors^{T} are
//orthogonal (because the covariance matrix is real and symmetric, and ROOT
//produces the orthogonal set of eigenvectors), the inverse operation
//(projection back to the original basis) is accomplished by multiplying the
//parameter vector by the inverse of eigen_vectors^{T}, which is eigen_vectors.
void EigenDecomp::GetOriginalParams(TVectorD &params_in, TVectorD &params_out, int start_param, int nparams)
{

    //If nparams is zero, just multiply the whole thing.
    if(nparams==0){
        params_out = (*eigen_vectors)*params_in;
        return;


    }

    //Otherwise, go through and only work with the vector elements requested.
    else{
        TVectorD tmp(nparams);
        for(int i=0; i<nparams; i++){
            tmp(i) = params_in(start_param+i);
        }

        tmp = (*eigen_vectors)*tmp;

        for(int i=0; i<params_in.GetNrows(); i++){

            //if the i value is between start parameter and the total number of
            //parameters, set params_out(i) to tmp(i-start_param).  Otherwise,
            //set it to the same valye as params_in(i). 
            params_out(i) = ( (i >= start_param && i < start_param + nparams) ? tmp(i - start_param) : params_in(i));
        }

        return;
    }
} 



//Transforms the fitted covariance (or a submatrix of it dictated by
//start_param and nparams) back into the original parameter basis by using the
//matrix of eigenvectors of the original covariance.  (i.e. Sub the new matrix
//in for D in C = PDP^{-1} = PDP^{T}
//Note: cov_in may not necessarily be diagonal, but it does not need to be, as this is how the change of
//basis is done.
//i.e. If have a vector x in the same basis as C, P^{T} transforms to the basis
//of D, D acts on it, then P transforms it back to the basis of C.
void EigenDecomp::GetOriginalCovariance(TMatrixDSym &cov_in, TMatrixDSym &cov_out, int start_param, int nparams)
{


    TMatrixD tmp1(cov_in.GetNrows(),cov_in.GetNcols());
    TMatrixD tmp2(cov_in.GetNrows(),cov_in.GetNcols());

    if(nparams==0){
        tmp1.Mult(cov_in,*eigen_vectorsT);
        tmp2.Mult(*eigen_vectors,tmp1);
        for(int i=0; i<cov_out.GetNrows(); i++){

            for(int j=0; j<cov_out.GetNcols(); j++){

                cov_out(i,j) = tmp2(i,j);
            }
        }
        return;
    }else{
        TMatrixD egvec(cov_in.GetNrows(),cov_in.GetNcols());
        TMatrixD egvecT(cov_in.GetNrows(),cov_in.GetNcols());
        for(int i=0; i<egvec.GetNrows(); i++)
            for(int j=0; j<egvec.GetNrows(); j++){
                if(i>=start_param && i<(start_param+nparams) &&
                        j>=start_param && j<(start_param+nparams) ){
                    egvec(i,j) = (*eigen_vectors)(i-start_param,j-start_param);
                    egvecT(i,j) = (*eigen_vectors)(i-start_param,j-start_param);
                } else {
                    egvec(i,j) = (i==j ? 1.0 : 0.0);
                    egvecT(i,j) = (i==j ? 1.0 : 0.0);
                } 
            }  
        egvecT.T();
        tmp1.Mult(cov_in,egvecT);
        tmp2.Mult(egvec,tmp1);
        for(int i=0; i<cov_out.GetNrows(); i++)
            for(int j=0; j<cov_out.GetNcols(); j++) cov_out(i,j) = tmp2(i,j);
        return;
    }

} 


