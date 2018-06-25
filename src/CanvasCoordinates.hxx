#ifndef CANVASCOORDINATES_HXX
#define CANVASCOORDINATES_HXX

#include "TObject.h"

class CanvasCoordinates: public TObject
{

public:
    CanvasCoordinates(){Init();}
    virtual ~CanvasCoordinates(){}

    void Init();

    Double_t Legend_RHS_X1;
    Double_t Legend_RHS_Y1;
    Double_t Legend_RHS_X2;
    Double_t Legend_RHS_Y2;

    Double_t Legend_LHS_X1;
    Double_t Legend_LHS_Y1;
    Double_t Legend_LHS_X2;
    Double_t Legend_LHS_Y2;

    Double_t PaveStats_Data_X1;
    Double_t PaveStats_Data_Y1;
    Double_t PaveStats_Data_X2;
    Double_t PaveStats_Data_Y2;

    Double_t PaveStats_MC_X1;
    Double_t PaveStats_MC_Y1;
    Double_t PaveStats_MC_X2;
    Double_t PaveStats_MC_Y2;

    ClassDef(CanvasCoordinates, 1)
};

#endif
