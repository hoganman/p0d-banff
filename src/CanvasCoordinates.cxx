#define CANVASCOORDINATES_CXX

#include "CanvasCoordinates.hxx"
ClassImp(CanvasCoordinates)

void CanvasCoordinates::Init()
{

    Legend_RHS_X1 = 0.550;  // 0.642;
    Legend_RHS_Y1 = 0.500;  // 0.606;
    Legend_RHS_X2 = 0.894;
    Legend_RHS_Y2 = 0.892;

    Legend_LHS_X1 = 0.110;
    Legend_LHS_Y1 = 0.550;
    Legend_LHS_X2 = 0.454;
    Legend_LHS_Y2 = 0.892;

    PaveStats_Data_X1 = 0.70;
    PaveStats_Data_Y1 = 0.90;
    PaveStats_Data_X2 = 0.90;
    PaveStats_Data_Y2 = 0.95;

    PaveStats_MC_X1 = 0.50;
    PaveStats_MC_Y1 = 0.90;
    PaveStats_MC_X2 = 0.70;
    PaveStats_MC_Y2 = 0.95;
}
