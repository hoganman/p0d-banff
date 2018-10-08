
#include "TString.h"
#include "TCut.h"

class PlottingSelectionInfo : public TObject
{
public:

    PlottingSelectionInfo() : name(""), cuts(""), legendLabel("")
    {}

    virtual ~PlottingSelectionInfo(){}

    PlottingSelectionInfo(const TString& input_name, const TCut &input_cuts,
            const TString &legend_label);

    TString name;
    TCut cuts;
    TString legendLabel;

    ClassDef(PlottingSelectionInfo, 1)

};


