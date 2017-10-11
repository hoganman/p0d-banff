#ifndef MINUIT2LIKELIHOODFITFCN_HXX
#define MINUIT2LIKELIHOODFITFCN_HXX
#include "Minuit2/FCNBase.h"
#include "TMatrixDSym.h"
#include "TVectorD.h"
#include "BANFFInterfaceBase.hxx"

class Minuit2LikelihoodFitFCN : public ROOT::Minuit2::FCNBase{
    public:

        /// Constructor
        Minuit2LikelihoodFitFCN(bool type=false);

        /// Destructor
        ~Minuit2LikelihoodFitFCN(){};

        void AssignInterface(BANFFInterfaceBase* inter);

        // Minimum chi2 fit (delta chi2 = 1 for 1 sigma error)
        double Up() const{ return 1.; }

        // Returns Chi square
        double operator() (const std::vector<double>& x) const;

    private:

        int counter;

        ///A pointer to the BANFFInterface class that contains all the
        ///information required to perform the fit.
        BANFFInterfaceBase* interface;

        void increment(){ counter++; };

};

#endif
