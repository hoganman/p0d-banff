#ifndef THROWPARAMS_HXX
#define THROWPARAMS_HXX

#include "TMatrixTSym.h"
#include "TVectorT.h"
#include "TDecompChol.h"
#include "TRandom3.h"
#include "TF1.h"
#include <vector>

class ThrowParams : public TObject
{

public:
    ThrowParams();

    virtual ~ThrowParams();

    ThrowParams(TVectorT<double> &params, TMatrixTSym<double> &covm);

    void SetSeed(int seed = 0);

    int GetSize() const {return npars;}

    void ThrowSet(std::vector<double> &params);

private:

    void CheloskyDecomp(TMatrixT<double> &chel_mat);

    void StdNormRand(double *z);

    TVectorD    *pvals;
    TMatrixDSym *covar;
    TMatrixD    *chel_dec;
    int         npars;
    TRandom3    rand;
    TF1         *gauss;

ClassDef(ThrowParams,1)

};

#endif
