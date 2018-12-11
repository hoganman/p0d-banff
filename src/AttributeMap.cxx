#define ATTRIBUTEMAP_CXX

#include "AttributeMap.hxx"
ClassImp(AttributeMap)
#include "P0DBANFFInterface.hxx"


TString AttributeMap::GetAttrib(const TString attribKey) const
{
    map_t::const_iterator it = fMap.find(attribKey);
    if(it != fMap.end())
        return it->second;
    return TString("");
}

Bool_t AttributeMap::GetAttribBool(const TString attribKey) const
{
    return GetAttribInt(attribKey) == 1;
}

Int_t AttributeMap::GetAttribInt(const TString attribKey) const
{
    return GetAttrib(attribKey).Atoi();
}

Double_t AttributeMap::GetAttribDouble(const TString attribKey) const
{
    return GetAttrib(attribKey).Atof();
}

void AttributeMap::AddAttribute(const TString attribKey, const TString attribValue)
{
    map_t::const_iterator it = fMap.find(attribKey);
    if(it != fMap.end())
    {
        const TString warnMsg = TString::Format("Attribute key %s with value %s is being overritten with %s", it->first.Data(), it->second.Data(), attribValue.Data());
        P0DBANFFInterface::Warning(this, warnMsg);
    }
    //printf("%s : %s\n", attribKey.Data(), attribValue.Data());
    fMap.insert(pair_t(attribKey.Data(), attribValue.Data()));
}
