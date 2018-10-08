#define PLOTTINGSELECTIONINFO_CXX

#include"PlottingSelectionInfo.hxx"
ClassImp(PlottingSelectionInfo)

PlottingSelectionInfo::PlottingSelectionInfo(const TString& input_name,
        const TCut &input_cuts, const TString &legend_label)
{
    name = input_name;
    cuts = input_cuts;
    legendLabel = legend_label;
}
