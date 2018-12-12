#define PLOTTINGSELECTIONINFO_CXX

#include"PlottingSelectionInfo.hxx"
ClassImp(PlottingSelectionInfo)

PlottingSelectionInfo::PlottingSelectionInfo(const TString& input_name,
         const SampleId::SampleEnum &inputID, const TCut &input_cuts,
         const TString &legend_label)
{
    name = input_name;
    sampleID = inputID;
    cuts = input_cuts;
    legendLabel = legend_label;
}
