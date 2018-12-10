#ifndef XMLTOOLS_HXX
#define XMLTOOLS_HXX
///A wrapper class for the TXMLEngine

#include "TXMLEngine.h"
#include "TH1D.h"
#include "TString.h"
#include "AttributeMap.hxx"
#include <vector>
//#include <map>


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

    ///Get all the attributes for the input node name
    ///Key: attribute name
    ///Value: attribute value
    AttributeMap GetAllChildAttributesFromNode(TString name);

    TString GetChildAttributeValueFromNode(TString name)
    { return GetAllChildAttributesFromNode(name).GetAttrib("value"); }

    //this is a special mode of the AttributeMap class where all the values
    //from the configuration files have been extracted
    AttributeMap GetAllNodeValues();

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

    void GetAllNodes();
    std::map<TString, AttributeMap> fAllNodes;

    ///Once a file has been set, fill the required pointers
    void SetXMLDocAndRootNode();

    ///Recursively search for a node
    XMLNodePointer_t GetXMLNode(TString name, XMLNodePointer_t node);

public:
    ClassDef(XMLTools, 1)

};

#endif
