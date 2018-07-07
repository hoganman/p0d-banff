#ifndef XMLTOOLS_HXX
#define XMLTOOLS_HXX
///A wrapper class for the TXMLEngine

#include "TXMLEngine.h"
#include "TH1D.h"
#include "TString.h"
#include <vector>
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

class XMLTools : public TObject {

public:
    ///The constructor
    XMLTools();

    ///The constructor
    XMLTools(TString fileName);

    ///The destructor
    virtual ~XMLTools();

    ///Get the engine
    TXMLEngine* Engine();

    ///Sets the XML file to read
    void SetFile(TString file);

    ///Returns the XML file name
    TString GetXMLFile() const {return fXMLFile;}

    ///Recursively searches for a node with "name"
    XMLNodePointer_t GetXMLNode(TString name);

    ///A useful container for all contents in a XML file
    typedef std::map<TString, TString> AttributeMap;

    ///Get all the attributes for the input node name
    ///Key: attribute name
    ///Value: attribute value
    XMLTools::AttributeMap GetAllChildAttributesFromNode(TString name);

    std::map<TString, XMLTools::AttributeMap> GetAllNodes();

    ///Get from a child node the attribute with name "attrName"
    TString GetChildAttributeFromNode(TString motherNodeName, TString attrName);

    ///For a binning config file like config/Binning.xml, get a TH1D* with bin edges
    ///specified in the binningName node
    ///If you want to use the last bin as overflow, see AnalysisBins
    TH1D* GetTH1DWithBinning(TString binningName);


protected:

    TString fXMLFile;
    XMLDocPointer_t fDoc; //!
    XMLNodePointer_t fRootNode; //!
    TXMLEngine* fxml; //!

    ///Once a file has been set, fill the required pointers
    void SetXMLDocAndRootNode();

    ///Recursively search for a node
    XMLNodePointer_t GetXMLNode(TString name, XMLNodePointer_t node);
    ClassDef(XMLTools, 1)

};

#endif
