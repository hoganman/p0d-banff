#define XMLTOOLS_CXX

#include "XMLTools.hxx"
#include "P0DBANFFInterface.hxx"
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
XMLTools::XMLTools(TString fileName)
//**************************************************
{
    fxml = new TXMLEngine();
    fDoc = NULL;
    fRootNode = NULL;
    SetFile(fileName);
    SetXMLDocAndRootNode();
}


//**************************************************
XMLTools::~XMLTools()
//**************************************************
{
//std::cout << "XMLTools destructor" << std::endl;
    if(fDoc)
    {
        fxml->FreeDoc(fDoc);
    }
    if(fxml) fxml->Delete();
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
XMLTools::AttributeMap XMLTools::GetAllChildAttributesFromNode(TString name)
//**************************************************
{
    XMLTools::AttributeMap attribMap;
    XMLNodePointer_t node = GetXMLNode(name);
    XMLNodePointer_t child = fxml->GetChild(node);
    while(child != 0)
    {
        TString child_name = fxml->GetNodeName(child);
//std::cout << "nodename = " << fxml->GetNodeName(child) << std::endl;
        XMLAttrPointer_t attr = fxml->GetFirstAttr(child);
//std::cout << attr << std::endl;
        while (attr != 0) {
            TString this_attrValue = fxml->GetAttrValue(attr);
//std::cout << "child_name = " << child_name.Data() << std::endl;
//std::cout << "attrValue = " << this_attrValue.Data() << std::endl;
            attribMap[child_name] = this_attrValue;
            attr = fxml->GetNextAttr(attr);
        }
        child = fxml->GetNext(child);
    }
    return attribMap;
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
    if(nodeName.EqualTo(name))
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
    return GetAllChildAttributesFromNode(motherNodeName)[attrName];
}

//**************************************************
TH1D* XMLTools::GetTH1DWithBinning(TString binningName)
//**************************************************
{

    AttributeMap attribs = GetAllChildAttributesFromNode(binningName);
    TString raw_nBinEdges = attribs["nBinEdges"];
    TString raw_binEdges = attribs["binEdges"];
    if(raw_nBinEdges.Length() == 0 || raw_binEdges.Length() == 0)
    {
        std::cout << "Unable to get " << binningName.Data() << std::endl;
        return NULL;
    }
    Int_t nBinEdges = raw_nBinEdges.Atoi();
    Int_t nBins = nBinEdges - 1;
    std::vector<TString> binEdges_vect = P0DBANFFInterface::SplitString(raw_binEdges, ',');
    if(static_cast<Int_t>(binEdges_vect.size()) != nBinEdges)
    {
        std::cout << "ERROR in " << binningName.Data() << std::endl;
        std::cout << "    The number of stated bin entries for these bins does" << std::endl;
        std::cout << "    match the number of found entries" << std::endl;
        return NULL;
    }
    std::vector<Double_t> binEdges(nBinEdges);
    for(Int_t index = 0; index < nBinEdges; ++index)
    {
        binEdges[index] = binEdges_vect.at(index).Atof();
    }
    TH1D* hist = new TH1D(binningName.Data(), "", nBins, &binEdges[0]);
    return hist;
}
