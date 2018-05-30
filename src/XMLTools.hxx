#ifndef XMLTOOLS_HXX
#define XMLTOOLS_HXX
///A wrapper class for the TXMLEngine

#include "TXMLEngine.h"
#include "TString.h"

class XMLTools : public TObject {

public:
    ///The constructor
    XMLTools();

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

    ///Get from a child node the attribute with name "attrName"
    TString GetChildAttributeFromNode(TString motherNodeName, TString attrName);

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
