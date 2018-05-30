#define XMLTOOLS_CXX

#include "XMLTools.hxx"
#include <iostream>
ClassImp(XMLTools)

//**************************************************
XMLTools::XMLTools()
//**************************************************
{
    fxml = new TXMLEngine();
    fDoc = NULL;
    fRootNode = NULL;
}

//**************************************************
XMLTools::~XMLTools()
//**************************************************
{
    if(fDoc)
    {
        fxml->FreeDoc(fDoc);
    }
    delete fxml;
}

//**************************************************
void XMLTools::SetFile(TString name)
//**************************************************
{
    fXMLFile = name;
    SetXMLDocAndRootNode();
}

//**************************************************
void XMLTools::SetXMLDocAndRootNode()
//**************************************************
{
    if(fDoc)
    {
        fxml->FreeDoc(fDoc);
    }
    fDoc      = fxml->ParseFile(fXMLFile);
    fRootNode = fxml->DocGetRootElement(fDoc);
}

//**************************************************
XMLNodePointer_t XMLTools::GetXMLNode(TString name)
//**************************************************
{
    if(!fRootNode || !fDoc)
    {
        std::cout << "No fRootNode or no fDoc" << std::endl;
        return NULL;
    }
    XMLNodePointer_t root_child = fxml->GetChild(fRootNode);
//std::cout << name.Data() << std::endl;
    return GetXMLNode(name, root_child);
}

//**************************************************
XMLNodePointer_t XMLTools::GetXMLNode(TString name, XMLNodePointer_t node)
//**************************************************
{
    if(node==0)
        return NULL;
    TString nodeName = fxml->GetNodeName(node);
//std::cout << nodeName.Data() << std::endl;
    if(nodeName.Contains(name))
    {
//std::cout << "RETURNED!" << std::endl;
        return node;
    }
    else
    {
        XMLNodePointer_t child = fxml->GetChild(node);
        while(child!=0)
        {
            XMLNodePointer_t test = GetXMLNode(name, child);
            if(test!=0)
                return test;
            child = fxml->GetNext(child);
        }
    }
    return NULL;
}

//**************************************************
TString XMLTools::GetChildAttributeFromNode(TString motherNodeName, TString attrName)
//**************************************************
{
    XMLNodePointer_t node = GetXMLNode(motherNodeName);
    if(!node)
    {
        std::cout << "ERROR: Unable to find node with name " << motherNodeName.Data() << std::endl;
        return "";
    }
    XMLNodePointer_t child = fxml->GetChild(node);
    while(child)
    {
        TString child_name = fxml->GetNodeName(child);
//std::cout << fxml->GetNodeName(child) << std::endl;
        XMLAttrPointer_t attr = fxml->GetFirstAttr(child);
//std::cout << attr << std::endl;
        while (attr!=0) {
            TString this_attrName = fxml->GetAttrName(attr);
            TString this_attrValue = fxml->GetAttrValue(attr);
//std::cout << this_attrName.Data() << std::endl;
//std::cout << this_attrValue.Data() << std::endl;
            if(child_name.Contains(attrName))
            {
                return this_attrValue;
            }
            attr = fxml->GetNextAttr(attr);
        }
        child = fxml->GetNext(child);
    }
    return "";

}

