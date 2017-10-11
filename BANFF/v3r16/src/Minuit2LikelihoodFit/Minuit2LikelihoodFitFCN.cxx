#include "Minuit2LikelihoodFitFCN.hxx"
#include "math.h"
#include "stdlib.h"
#include "TMath.h"
#include <iostream>
#include <omp.h>
#include <cmath>
#include "FitParameters.hxx"

Minuit2LikelihoodFitFCN::Minuit2LikelihoodFitFCN(bool type){

    (void) type;
    interface = NULL;
    counter = 0;

}

void Minuit2LikelihoodFitFCN::AssignInterface(BANFFInterfaceBase* inter){

    interface = inter;

}

double Minuit2LikelihoodFitFCN::operator() (const std::vector<double>& x) const {


    return interface->ApplyParamsAndCalcLLR(x);


}
