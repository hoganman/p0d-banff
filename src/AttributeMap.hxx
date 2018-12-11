#ifndef ATTRIBUTEMAP_HXX
#define ATTRIBUTEMAP_HXX

#include "TObject.h"
#include "TString.h"
#include <map>
#ifdef __CINT__
#pragma link C++ nestedclasses;
#pragma link C++ nestedtypedefs;
#pragma link C++ class std::map<TString,TString>+;
#pragma link C++ class std::map<TString,TString>::*;
#pragma link C++ operators std::map<TString,TString>::iterator;
#pragma link C++ operators std::map<TString,TString>::const_iterator;
#pragma link C++ operators std::map<TString,TString>::reverse_iterator;
#endif

class AttributeMap : public TObject
{
public:

    AttributeMap(){}

    virtual ~AttributeMap(){}

    TString GetAttrib(const TString attribKey = "value") const;

    Bool_t GetAttribBool(const TString attribKey = "value") const;

    Int_t GetAttribInt(const TString attribKey = "value") const;

    Double_t GetAttribDouble(const TString attribKey = "value") const;

    void AddAttribute(const TString attribKey, const TString attribValue);

    UInt_t GetSize() const { return fMap.size(); }

    typedef std::map<TString, TString> map_t;

    map_t fMap;

    map_t::const_iterator GetFirstAttribute() {fMapCIter = fMap.begin(); return fMapCIter;}

    map_t::const_iterator GetNextAttribute() {if(fMapCIter != fMap.end()) ++fMapCIter; return fMapCIter;}

    map_t::const_iterator GetEndIterator() {return fMap.end();}

    map_t::const_iterator fMapCIter;

ClassDef(AttributeMap, 1)
};

#endif
