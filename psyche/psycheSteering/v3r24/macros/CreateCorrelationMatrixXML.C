#include "TXMLEngine.h"
#include <vector>



void CodeInPileUp(TXMLEngine* xml, XMLNodePointer_t NodeP){
   xml->NewAttr(NodeP, 0, "Block",      "10,0,0"  ); // FHC FGD1
   xml->NewAttr(NodeP, 0, "Block",      "7,10,10" ); // RHC FGD1
   xml->NewAttr(NodeP, 0, "Block",      "10,17,17"); // FHC FGD2
   xml->NewAttr(NodeP, 0, "Block",      "7,27,27" ); // RHC FGD2
   xml->NewAttr(NodeP, 0, "Block",      "10,0,17" ); // FHC FGD1/2
   xml->NewAttr(NodeP, 0, "Block",      "7,10,27" ); // RHC FGD1/2
   xml->NewAttr(NodeP, 0, "Block",      "10,17,0" ); // FHC FGD2/1
   xml->NewAttr(NodeP, 0, "Block",      "7,27,10" ); // RHC FGD2/1
}


void CreateCorrelationMatrixXML(const char* filename = "correlations.xml"){

   // First create engine
   TXMLEngine* xml = new TXMLEngine;
   // Create main node of document tree
   XMLNodePointer_t mainnode = xml->NewChild(0, 0, "main");

   std::vector<XMLNodePointer_t> SelfCorr;
   SelfCorr.push_back(xml->NewChild(mainnode, 0, "MomResol"      ));
   SelfCorr.push_back(xml->NewChild(mainnode, 0, "ChargeIDEff"   ));
   SelfCorr.push_back(xml->NewChild(mainnode, 0, "TpcClusterEff" ));
   SelfCorr.push_back(xml->NewChild(mainnode, 0, "TpcTrackEff"   ));
   SelfCorr.push_back(xml->NewChild(mainnode, 0, "TpcFgdMatchEff"));
   SelfCorr.push_back(xml->NewChild(mainnode, 0, "MichelEleEff"  ));

   for(std::vector<XMLNodePointer_t>::iterator it = SelfCorr.begin();
       it != SelfCorr.end(); ++it){
     xml->NewChild((*it), 0, "WithItself", "");
   }

   XMLNodePointer_t TPCPID     = xml->NewChild(mainnode, 0, "TpcPid");
   XMLNodePointer_t TPCPIDCorr = xml->NewChild(TPCPID,   0, "WithItself");
   xml->NewAttr(TPCPIDCorr, 0, "Block", "24,0,0"  ); // Muon Sigma
   xml->NewAttr(TPCPIDCorr, 0, "Block", "5,24,24" ); // Elec Sigma
   xml->NewAttr(TPCPIDCorr, 0, "Block", "16,29,29"); // Prot Sigma
   xml->NewAttr(TPCPIDCorr, 0, "Block", "24,45,45"); // Muon Mean
   xml->NewAttr(TPCPIDCorr, 0, "Block", "5,69,69" ); // Elec Mean
   xml->NewAttr(TPCPIDCorr, 0, "Block", "16,74,74"); // Prot Mean
   xml->NewAttr(TPCPIDCorr, 0, "Block", "24,0,45" ); // Muon Mean/Sigma
   xml->NewAttr(TPCPIDCorr, 0, "Block", "5,24,69" ); // Elec Mean/Sigma
   xml->NewAttr(TPCPIDCorr, 0, "Block", "16,29,74"); // Prot Mean/Sigma

   XMLNodePointer_t FGDPID     = xml->NewChild(mainnode, 0, "FgdPid");
   XMLNodePointer_t FGDPIDCorr = xml->NewChild(FGDPID,   0, "WithItself");
   xml->NewAttr(FGDPIDCorr, 0, "Param", "0,2"); // Muon Mean and Sigma FGD1
   xml->NewAttr(FGDPIDCorr, 0, "Param", "4,6"); // Muon Mean and Sigma FGD2
   xml->NewAttr(FGDPIDCorr, 0, "Param", "0,4"); // Muon Mean and Sigma FGD1/FGD2
   xml->NewAttr(FGDPIDCorr, 0, "Param", "0,6"); // Muon Mean and Sigma FGD1/FGD2
   xml->NewAttr(FGDPIDCorr, 0, "Param", "2,4"); // Muon Mean and Sigma FGD1/FGD2
   xml->NewAttr(FGDPIDCorr, 0, "Param", "2,6"); // Muon Mean and Sigma FGD1/FGD2
   xml->NewAttr(FGDPIDCorr, 0, "Param", "1,3"); // Prot Mean and Sigma FGD1     
   xml->NewAttr(FGDPIDCorr, 0, "Param", "5,7"); // Prot Mean and Sigma FGD2     
   xml->NewAttr(FGDPIDCorr, 0, "Param", "1,5"); // Prot Mean and Sigma FGD1/FGD2
   xml->NewAttr(FGDPIDCorr, 0, "Param", "1,7"); // Prot Mean and Sigma FGD1/FGD2
   xml->NewAttr(FGDPIDCorr, 0, "Param", "3,5"); // Prot Mean and Sigma FGD1/FGD2
   xml->NewAttr(FGDPIDCorr, 0, "Param", "3,7"); // Prot Mean and Sigma FGD1/FGD2

   XMLNodePointer_t OOFV        = xml->NewChild(mainnode, 0, "OOFV");
   XMLNodePointer_t NueOOFVCorr = xml->NewChild(OOFV,     0, "NuEOOFV");
   xml->NewAttr(NueOOFVCorr, 0, "Param", "0,0");
   xml->NewAttr(NueOOFVCorr, 0, "Param", "1,1");
   xml->NewAttr(NueOOFVCorr, 0, "Param", "2,2");
   xml->NewAttr(NueOOFVCorr, 0, "Param", "3,3");
   xml->NewAttr(NueOOFVCorr, 0, "Param", "4,4");


   // Quite tricky here you need to make sure that everything is correlated with everything, otherwise the matrix can't be decomposed
   XMLNodePointer_t PileUp = xml->NewChild(mainnode, 0, "PileUp");
   XMLNodePointer_t PileUpCorr        = xml->NewChild(PileUp, 0, "WithItself"   );
   XMLNodePointer_t NuETPCPileUpCorr  = xml->NewChild(PileUp, 0, "NuETPCPileUp" );
   XMLNodePointer_t NuEP0DPileUpCorr  = xml->NewChild(PileUp, 0, "NuEP0DPileUp" );
   XMLNodePointer_t NuEECalPileUpCorr = xml->NewChild(PileUp, 0, "NuEECalPileUp");
   CodeInPileUp(xml,PileUpCorr       );
   CodeInPileUp(xml,NuETPCPileUpCorr );
   CodeInPileUp(xml,NuEP0DPileUpCorr );
   CodeInPileUp(xml,NuEECalPileUpCorr);

   XMLNodePointer_t NuETPCPileUp  = xml->NewChild(mainnode, 0, "NuETPCPileUp");
   XMLNodePointer_t NuETPCPileUpCorr2  = xml->NewChild(NuETPCPileUp, 0, "WithItself");
   XMLNodePointer_t NuEP0DPileUpCorr3  = xml->NewChild(NuETPCPileUp, 0, "NuEP0DPileUp");
   XMLNodePointer_t NuEECalPileUpCorr3 = xml->NewChild(NuETPCPileUp, 0, "NuEECalPileUp");
   CodeInPileUp(xml,NuETPCPileUpCorr2 );
   CodeInPileUp(xml,NuEP0DPileUpCorr3 );
   CodeInPileUp(xml,NuEECalPileUpCorr3);

   XMLNodePointer_t NuEP0DPileUp = xml->NewChild(mainnode, 0, "NuEP0DPileUp");
   XMLNodePointer_t NuEP0DPileUpCorr2  = xml->NewChild(NuEP0DPileUp, 0, "WithItself");
   XMLNodePointer_t NuEECalPileUpCorr4 = xml->NewChild(NuEP0DPileUp, 0, "NuEECalPileUp");
   CodeInPileUp(xml,NuEP0DPileUpCorr2 );
   CodeInPileUp(xml,NuEECalPileUpCorr4);

   XMLNodePointer_t NuEECalPileUp = xml->NewChild(mainnode, 0, "NuEECalPileUp");
   XMLNodePointer_t NuEECalPileUpCorr2 = xml->NewChild(NuEECalPileUp, 0, "WithItself");
   CodeInPileUp(xml,NuEECalPileUpCorr2);

   XMLNodePointer_t Sand     = xml->NewChild(mainnode, 0, "SandMu");
   XMLNodePointer_t SandCorr = xml->NewChild(Sand,     0, "WithItself");
   xml->NewAttr(SandCorr, 0, "Param", "1,2"); // FHC SAND
   
   // now create document and assign main node of document
   XMLDocPointer_t xmldoc = xml->NewDoc();
   xml->DocSetRootElement(xmldoc, mainnode);
   // Save document to file
   xml->SaveDoc(xmldoc, filename);
   // Release memory before exit
   xml->FreeDoc(xmldoc);
   delete xml;
  
}


