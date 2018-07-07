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
std::map<TString, XMLTools::AttributeMap> XMLTools::GetAllNodes()
//**************************************************
{
    std::map<TString, AttributeMap> attribs;
    if(!fRootNode || !fDoc)
    {
        std::cout << "No fRootNode or no fDoc" << std::endl;
        return attribs;
    }
    XMLNodePointer_t node = fxml->GetChild(fRootNode);
    XMLNodePointer_t child = fxml->GetChild(node);

    while(child)
    {
        TString nodeName = fxml->GetNodeName(child);
        AttributeMap a_attrib = GetAllChildAttributesFromNode(nodeName);
        attribs[nodeName] = a_attrib;
        child = fxml->GetNext(child);
    }

    /*
    std::cout << attribs.size() << std::endl;
    for(std::map<TString, AttributeMap>::iterator it0 = attribs.begin(); it0!=attribs.end(); ++it0)
    {
        std::cout << it0->first << std::endl;
        AttributeMap tmp = it0->second;
        std::cout << tmp.size() << std::endl;
        for(AttributeMap::iterator it = tmp.begin(); it!=tmp.end(); ++it)
        {
            std::cout << it->first << std::endl;
            std::cout << it->second << std::endl;
        }
    }
    */
    return attribs;
}

//**************************************************
std::vector<TString> XMLTools::SplitString(TString theOpt,
    Char_t separator)
//**************************************************
{
   // splits the option string at 'separator' and fills the list
   // 'splitV' with the primitive strings
   std::vector<TString> splitV;
   TString splitOpt(theOpt);
   splitOpt.ReplaceAll("\n"," ");
   splitOpt = splitOpt.Strip(TString::kBoth,separator);
   while (splitOpt.Length()>0) {
      if ( !splitOpt.Contains(separator) ) {
         splitV.push_back(splitOpt);
         break;
      }
      else {
         TString toSave = splitOpt(0,splitOpt.First(separator));
         splitV.push_back(toSave);
         splitOpt = splitOpt(splitOpt.First(separator),splitOpt.Length());
      }
      splitOpt = splitOpt.Strip(TString::kLeading,separator);
   }
   return splitV;
}
