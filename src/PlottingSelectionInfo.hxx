#ifndef PLOTTINGSELECTIONINFO_HXX
#define PLOTTINGSELECTIONINFO_HXX

#include "SampleId.hxx"
#include "TString.h"
#include "TCut.h"

class PlottingSelectionInfo : public TObject
{
public:

    PlottingSelectionInfo() : name(""), sampleID(SampleId::kUnassigned), cuts(""), legendLabel("")
    {}

    virtual ~PlottingSelectionInfo(){}

    PlottingSelectionInfo(const TString& input_name, const SampleId::SampleEnum &inputID, const TCut &input_cuts,
            const TString &legend_label);

    PlottingSelectionInfo(const TString& input_name, const TCut &input_cuts,
            const TString &legend_label)
    {
        PlottingSelectionInfo(input_name, SampleId::kUnassigned, input_cuts, legend_label);
    }

    TString name;
    SampleId::SampleEnum sampleID;
    TCut cuts;
    TString legendLabel;

    ClassDef(PlottingSelectionInfo, 1)

};

#endif
