#define XMLTOOLS_CXX

#include "XMLTools.hxx"
ClassImp(XMLTools)
#include "P0DBANFFInterface.hxx"
#include <iostream>
#include <cstdlib>

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
AttributeMap XMLTools::GetAllChildAttributesFromNode(TString name)
//**************************************************
{
    AttributeMap attribMap;
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
            attribMap.AddAttribute(child_name, this_attrValue);
            attr = fxml->GetNextAttr(attr);
        }
        child = fxml->GetNext(child);
    }
    //printf("Got map for %s\n", name.Data());
    //printf("Size of map: %ld\n", attribMap.size());
    //for(XMLTools::AttributeMap::const_iterator it = attribMap.begin(); it != attribMap.end(); ++it)
    //{
    //    printf("%s: %s\n", it->first.Data(), it->second.Data());
    //}
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
        P0DBANFFInterface::Error(this, TString::Format("In %s. The number of stated bin entries for these bins does NOT match the number of found entries.", binningName.Data()));
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

//**************************************************
void XMLTools::GetAllNodes()
//**************************************************
{
    if(!fRootNode || !fDoc)
    {
        std::cout << "No fRootNode or no fDoc" << std::endl;
        return;
    }
    XMLNodePointer_t node = fxml->GetChild(fRootNode);
    XMLNodePointer_t child = fxml->GetChild(node);

    while(child)
    {
        TString nodeName = fxml->GetNodeName(child);
        AttributeMap a_attrib = GetAllChildAttributesFromNode(nodeName);
        fAllNodes[nodeName] = a_attrib;
        child = fxml->GetNext(child);
    }

    /*
    std::cout << attribs.size() << std::endl;
    for(std::map<TString, AttributeMap>::iterator it0 = attribs.begin(); it0!=attribs.end(); ++it0)
    {
        std::cout << it0->first << std::endl;
        AttributeMap tmp = it0->second;
        std::cout << tmp.size() << std::endl;
        for(AttributeMap::const_iterator it = tmp.begin(); it!=tmp.end(); ++it)
        {
            std::cout << it->first << std::endl;
            std::cout << it->second << std::endl;
        }
    }
    */
}

//**************************************************
AttributeMap XMLTools::GetAllNodeValues()
//**************************************************
{
    if(fAllNodes.size() == 0)
        GetAllNodes();
    AttributeMap nodeValues;
    std::map<TString, AttributeMap>::const_iterator it;
    for(it = fAllNodes.begin(); it != fAllNodes.end(); ++it)
    {
        if( it->second.GetAttrib("value").Length() == 0)
            continue;
        //printf("it->first = %s\n", it->first.Data());
        //printf("it->second[\"value\"] = %s\n", it->second["value"].Data());
        nodeValues.AddAttribute(it->first.Data(), it->second["value"]);
    }
    return nodeValues;
}
