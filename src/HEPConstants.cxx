#define HEPCONSTANTS_CXX
#include "HEPConstants.hxx"
#include <algorithm>
ClassImp(HEPConstants)

Double_t HEPConstants::Convert(TString unit) const {
    std::map<TString, Double_t>::const_iterator iter = conversionMap.find(unit);
    if(iter != conversionMap.end())
        return iter->second;
    return 1.0;

}
