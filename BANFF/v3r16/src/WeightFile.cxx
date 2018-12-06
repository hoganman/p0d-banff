#define WeightFile_cxx
#include "WeightFile.hxx"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>



XsecSysts ParamNameToXsecSysts(std::string paramName){

  //Start with 2013.
  if(paramName == "MAQE") return MAQE;
  else if(paramName == "MARES") return MARES;
  else if(paramName == "DISMPISHP") return DISMPISHP;
  else if(paramName == "SF") return SF;
  else if(paramName == "EB" ) return EB;
  else if((paramName == "PF") || (paramName == "pF") ) return PF;
  else if(paramName == "PDD") return PDD;
  else if((paramName == "FSI_PI_ABS") || (paramName == "FEFABS")) return FSI_PI_ABS;
  else if((paramName == "FSI_PI_PROD") || (paramName == "FEFINEL")) return FSI_PI_PROD;
  else if((paramName == "FSI_INEL_LO") || (paramName == "FEFQE") || (paramName == "FSI_INEL_LO_E")) return FSI_INEL_LO;
  else if((paramName == "FSI_INEL_HI") || (paramName == "FEFQEH") || (paramName == "FSI_INEL_HI_E")) return FSI_INEL_HI;
  else if((paramName == "FSI_CEX_LO") || (paramName == "FEFCX")  || (paramName == "FSI_CEX_LO_E")) return FSI_CEX_LO;
  else if((paramName == "FSI_CEX_HI") || (paramName == "FEFCXH") || (paramName == "FSI_CEX_HI_E")) return FSI_CEX_HI;
  else if(paramName == "PDD_MEC") return PDD_MEC;
  else if(paramName == "MEC") return MEC;

  //Now those added for 2014.
  else if(paramName == "MEC_C") return MEC_C;
  else if(paramName == "pF_C") return PF_C;
  else if(paramName == "EB_C") return EB_C;
  else if(paramName == "MEC_O") return MEC_O;
  else if(paramName == "pF_O") return PF_O;
  else if(paramName == "EB_O") return EB_O;
  else if(paramName == "CA5") return CA5;
  else if(paramName == "MANFFRES") return MARES;
  else if(paramName == "BgRES" ) return BgSclRes;
  else if(paramName == "SCCV") return SCCV;
  else if(paramName == "SCCA") return SCCA;
  else if((paramName == "RPA_C") || (paramName == "RPA_O")) return RPA;  //TODO: This probably needs to be changed to separate these two... either that or handle them properly elsewhere.
  else if(paramName == "SF_RFG") return SF_RFG;
  else if(paramName == "CCNUE_0") return CCNuE;
   
  // Kendall's parameter
  else if(paramName == "COH_BS") return COH_BS;

	
  //Now those for 2016 with some additions and some names that have changed
  else if(paramName == "2p2h_shape_C") return MEC_shape_C;
  else if(paramName == "2p2h_shape_O") return MEC_shape_O;
  else if(paramName == "CC_DIS") return DISMPISHP;
  else if(paramName == "ISO_BKG") return BgSclRes;
	
  //If it is not in the above list, it's a normalization parameter, so return
  //NORM, i.e. -1.
  else return NORM;
}

void WeightFile::MakeSplines(int entry)
{
  if (fChain == 0) return;
  Long64_t nentries = fChain->GetEntriesFast();
  if(entry>=nentries){
    std::cerr << "Entry too high " << entry << std::endl;
    exit(0);
  }

  LoadTree(entry);
  fChain->GetEntry(entry);

  //Start by making sure the splines are all clear.
  for(int i = 0; i < SampleId::kNSamples; i++){


    //Delete the individual splines.
    if(MAQESpline!=NULL){ 
      if(MAQESpline[i] != NULL){
        delete MAQESpline[i];
      }
    }

    if(MARESSpline!=NULL){
      if(MARESSpline[i] != NULL){
        delete MARESSpline[i];
      }
    }

    if(DISMPISHPSpline!=NULL){
      if(DISMPISHPSpline[i] != NULL){
        delete DISMPISHPSpline[i];
      }
    }

    if(SFSpline!=NULL){
      if(SFSpline[i] != NULL){
        delete SFSpline[i];
      }
    }
    if(EBSpline!=NULL){
      if(EBSpline[i] != NULL){
        delete EBSpline[i];
      }
    }

    if(PFSpline!=NULL){
      if(PFSpline[i] != NULL){
        delete PFSpline[i];
      }
    }

    if(PDDSpline!=NULL){
      if(PDDSpline[i] != NULL){
        delete PDDSpline[i];
      }
    }

    if(FSI_PI_ABSSpline!=NULL){
      if(FSI_PI_ABSSpline[i] != NULL){
        delete FSI_PI_ABSSpline[i];
      }
    }

    if(FSI_CEX_LOSpline!=NULL){
      if(FSI_CEX_LOSpline[i] != NULL){
        delete FSI_CEX_LOSpline[i];
      }
    }

    if(FSI_INEL_LOSpline!=NULL){
      if(FSI_INEL_LOSpline[i] != NULL){
        delete FSI_INEL_LOSpline[i];
      }
    }

    if(FSI_PI_PRODSpline!=NULL){
      if(FSI_PI_PRODSpline[i] != NULL){
        delete FSI_PI_PRODSpline[i];
      }
    }

    if(FSI_CEX_HISpline!=NULL){
      if(FSI_CEX_HISpline[i] != NULL){
        delete FSI_CEX_HISpline[i];
      }
    }

    if(FSI_INEL_HISpline!=NULL){
      if(FSI_INEL_HISpline[i] != NULL){
        delete FSI_INEL_HISpline[i];
      }
    }

    if(PDD_MECSpline!=NULL){
      if(PDD_MECSpline[i] != NULL){
        delete PDD_MECSpline[i];
      }
    }

    if(MECSpline!=NULL){
      if(MECSpline[i] != NULL){
        delete MECSpline[i];
      }
    }

    if(MEC_CSpline != NULL){ 
      if(MEC_CSpline[i] != NULL){
        delete MEC_CSpline[i];
      }
    }

    if(PF_CSpline != NULL){
      if(PF_CSpline[i] != NULL){
        delete PF_CSpline[i];
      }
    }

    if(EB_CSpline != NULL){
      if(EB_CSpline[i] != NULL){
        delete EB_CSpline[i];
      }
    }

    if(MEC_OSpline != NULL){
      if(MEC_OSpline[i] != NULL){
        delete MEC_OSpline[i];
      }
    }

    if(PF_OSpline != NULL){
      if(PF_OSpline[i] != NULL){
        delete PF_OSpline[i];
      }
    }

    if(EB_OSpline != NULL){ 
      if(EB_OSpline[i] != NULL){
        delete EB_OSpline[i];
      }
    }

    if(CA5Spline != NULL){ 
      if(CA5Spline[i] != NULL){
        delete CA5Spline[i];
      }
    }

    if(BgSclResSpline != NULL){
      if(BgSclResSpline[i] != NULL){
        delete BgSclResSpline[i];
      }
    }

    if(SCCVSpline != NULL){ 
      if(SCCVSpline[i] != NULL){
        delete SCCVSpline[i];
      }
    }

    if(SCCASpline != NULL){
      if(SCCASpline[i] != NULL){
        delete SCCASpline[i];
      }
    }

    if(SF_RFGSpline != NULL){
      if(SF_RFGSpline[i] != NULL){
        delete SF_RFGSpline[i];
      }
    }
    if(RPASpline != NULL){
      if(RPASpline[i] != NULL){
        delete RPASpline[i];
      }
    }
    if(CCNuESpline != NULL){
      if(CCNuESpline[i] != NULL){
        delete CCNuESpline[i];
      }
    }
		
    if(PDD_CSpline != NULL){
      if(PDD_CSpline[i] != NULL){
        delete PDD_CSpline[i];
      }
    }
		
    if(PDD_OSpline != NULL){
      if(PDD_OSpline[i] != NULL){
        delete PDD_OSpline[i];
      }
    }
  }

  //Delete the pointers to the arrays.
  //The individual elements have already been deleted, so use delete
  //instead of delete[].
  if(MAQESpline!=NULL) delete[] MAQESpline;
  if(MARESSpline!=NULL) delete[] MARESSpline;
  if(DISMPISHPSpline!=NULL) delete[] DISMPISHPSpline;
  if(SFSpline!=NULL) delete[] SFSpline;
  if(EBSpline!=NULL) delete[] EBSpline;
  if(PFSpline!=NULL) delete[] PFSpline;
  if(PDDSpline!=NULL) delete[] PDDSpline;
  if(FSI_PI_ABSSpline!=NULL) delete[] FSI_PI_ABSSpline;
  if(FSI_CEX_LOSpline!=NULL) delete[] FSI_CEX_LOSpline;
  if(FSI_INEL_LOSpline!=NULL) delete[] FSI_INEL_LOSpline;
  if(FSI_PI_PRODSpline!=NULL) delete[] FSI_PI_PRODSpline;
  if(FSI_CEX_HISpline!=NULL) delete[] FSI_CEX_HISpline;
  if(FSI_INEL_HISpline!=NULL) delete[] FSI_INEL_HISpline;
  if(PDD_MECSpline!=NULL) delete[] PDD_MECSpline;
  if(MECSpline!=NULL) delete[] MECSpline;

  if(MEC_CSpline != NULL) delete[] MEC_CSpline;
  if(PF_CSpline != NULL) delete[] PF_CSpline;
  if(EB_CSpline != NULL) delete[] EB_CSpline;

  if(MEC_OSpline != NULL) delete[] MEC_OSpline;
  if(PF_OSpline != NULL) delete[] PF_OSpline;
  if(EB_OSpline != NULL) delete[] EB_OSpline;

  if(CA5Spline != NULL) delete[] CA5Spline;
  if(BgSclResSpline != NULL) delete[] BgSclResSpline;
  if(SCCVSpline != NULL) delete[] SCCVSpline;
  if(SCCASpline != NULL) delete[] SCCASpline;
  if(SF_RFGSpline != NULL) delete[] SF_RFGSpline;
  if(RPASpline != NULL) delete[] RPASpline;
  if(CCNuESpline != NULL) delete[] CCNuESpline;

  if(PDD_CSpline != NULL) delete[] PDD_CSpline;
  if(PDD_OSpline != NULL) delete[] PDD_OSpline;

  //Set up the spline arrays.
  //Here we use NSamples as read in from the tree.
  MAQESpline = new TSpline3*[NSamples];
  MARESSpline = new TSpline3*[NSamples];
  DISMPISHPSpline = new TSpline3*[NSamples];
  SFSpline = new TSpline3*[NSamples];
  EBSpline = new TSpline3*[NSamples];
  PFSpline = new TSpline3*[NSamples];
  PDDSpline = new TSpline3*[NSamples];
  MECSpline = new TSpline3*[NSamples];
  FSI_PI_ABSSpline = new TSpline3*[NSamples];
  FSI_PI_PRODSpline = new TSpline3*[NSamples];
  FSI_CEX_LOSpline = new TSpline3*[NSamples];
  FSI_CEX_HISpline = new TSpline3*[NSamples];
  FSI_INEL_LOSpline = new TSpline3*[NSamples];
  FSI_INEL_HISpline = new TSpline3*[NSamples];

  MEC_CSpline = new TSpline3*[NSamples];
  PF_CSpline = new TSpline3*[NSamples];
  EB_CSpline = new TSpline3*[NSamples];

  MEC_OSpline = new TSpline3*[NSamples];
  PF_OSpline = new TSpline3*[NSamples];
  EB_OSpline = new TSpline3*[NSamples];

  CA5Spline = new TSpline3*[NSamples];
  BgSclResSpline = new TSpline3*[NSamples];
  SCCVSpline = new TSpline3*[NSamples];
  SCCASpline = new TSpline3*[NSamples];

  SF_RFGSpline = new TSpline3*[NSamples];
  RPASpline = new TSpline3*[NSamples];
  CCNuESpline = new TSpline3*[NSamples];

  PDD_CSpline = new TSpline3*[NSamples];
  PDD_OSpline = new TSpline3*[NSamples];

  //First, generate splines common to both filetypes, for each sample.
  for(int i = 0; i < NSamples; i++){

    //Start by setting the element to NULL for each spline.  If something else can happen
    //to it, this will happen below.
    MAQESpline[i] = NULL;
    MARESSpline[i] = NULL;
    DISMPISHPSpline[i] = NULL;
    SFSpline[i] = NULL;
    EBSpline[i] = NULL;
    PFSpline[i] = NULL;
    PDDSpline[i] = NULL;
    MECSpline[i] = NULL;
    FSI_PI_ABSSpline[i] = NULL;
    FSI_PI_PRODSpline[i] = NULL;
    FSI_CEX_LOSpline[i] = NULL;
    FSI_CEX_HISpline[i] = NULL;
    FSI_INEL_LOSpline[i] = NULL;
    FSI_INEL_HISpline[i] = NULL;

    MEC_CSpline[i] = NULL;
    PF_CSpline[i] = NULL;
    EB_CSpline[i] = NULL;

    MEC_OSpline[i] = NULL;
    PF_OSpline[i] = NULL;
    EB_OSpline[i] = NULL;

    CA5Spline[i] = NULL;
    BgSclResSpline[i] = NULL;
    SCCVSpline[i] = NULL;
    SCCASpline[i] = NULL;
    SF_RFGSpline[i] = NULL;
    RPASpline[i] = NULL;
    CCNuESpline[i] = NULL;

    PDD_CSpline[i] = NULL;
    PDD_OSpline[i] = NULL;

    if(MAQEGraph->At(i) != NULL){
      if(((TGraph*)(MAQEGraph->At(i)))->GetN()>1) MAQESpline[i] = new TSpline3("MAQESpline",(TGraph*)(MAQEGraph->At(i)));
    }

    if(MARESGraph->At(i) != NULL){
      if(((TGraph*)(MARESGraph->At(i)))->GetN()>1) MARESSpline[i] = new TSpline3("MARESSpline",(TGraph*)(MARESGraph->At(i)));
    }

    if(DISMPISHPGraph->At(i) != NULL){
      if(((TGraph*)(DISMPISHPGraph->At(i)))->GetN()>1) DISMPISHPSpline[i] = new TSpline3("DISMPISHPSpline",(TGraph*)(DISMPISHPGraph->At(i)));
    }

    if(FSI_PI_ABSGraph->At(i) != NULL){
      if(((TGraph*)(FSI_PI_ABSGraph->At(i)))->GetN()>1) FSI_PI_ABSSpline[i] = new TSpline3("FSI_PI_ABSSpline",(TGraph*)(FSI_PI_ABSGraph->At(i)));
    }

    if(FSI_PI_PRODGraph->At(i) != NULL){
      if(((TGraph*)(FSI_PI_PRODGraph->At(i)))->GetN()>1) FSI_PI_PRODSpline[i] = new TSpline3("FSI_PI_PRODSpline",(TGraph*)(FSI_PI_PRODGraph->At(i)));
    }

    if(FSI_CEX_LOGraph->At(i) != NULL){
      if(((TGraph*)(FSI_CEX_LOGraph->At(i)))->GetN()>1) FSI_CEX_LOSpline[i] = new TSpline3("FSI_CEX_LOSpline",(TGraph*)(FSI_CEX_LOGraph->At(i)));
    }

    if(FSI_CEX_HIGraph->At(i) != NULL){
      if(((TGraph*)(FSI_CEX_HIGraph->At(i)))->GetN()>1) FSI_CEX_HISpline[i] = new TSpline3("FSI_CEX_HISpline",(TGraph*)(FSI_CEX_HIGraph->At(i)));
    }

    if(FSI_INEL_LOGraph->At(i) != NULL){
      if(((TGraph*)(FSI_INEL_LOGraph->At(i)))->GetN()>1) FSI_INEL_LOSpline[i] = new TSpline3("FSI_INEL_LOSpline",(TGraph*)(FSI_INEL_LOGraph->At(i)));
    }

    if(FSI_INEL_HIGraph->At(i) != NULL){
      if(((TGraph*)(FSI_INEL_HIGraph->At(i)))->GetN()>1) FSI_INEL_HISpline[i] = new TSpline3("FSI_INEL_HISpline",(TGraph*)(FSI_INEL_HIGraph->At(i)));
    }
		        
    //Next, if we're dealing with a 2013 style file, make those splines.
    if(fileType == 0){

      if(SFGraph->At(i) != NULL){
        if(((TGraph*)(SFGraph->At(i)))->GetN()>1) SFSpline[i] = new TSpline3("SFSpline",(TGraph*)(SFGraph->At(i)));
      }

      if(EBGraph->At(i) != NULL){
        if(((TGraph*)(EBGraph->At(i)))->GetN()>1) EBSpline[i] = new TSpline3("EBSpline",(TGraph*)(EBGraph->At(i)));
      }

      if(PFGraph->At(i) != NULL){
        if(((TGraph*)(PFGraph->At(i)))->GetN()>1) PFSpline[i] = new TSpline3("PFSpline",(TGraph*)(PFGraph->At(i)));
      }

      if(PDDGraph->At(i) != NULL){
        if(((TGraph*)(PDDGraph->At(i)))->GetN()>1) PDDSpline[i] = new TSpline3("PDDSpline",(TGraph*)(PDDGraph->At(i)));
      }

      if(MECGraph->At(i) != NULL){
        if(((TGraph*)(MECGraph->At(i)))->GetN()>1) MECSpline[i] = new TSpline3("MECSpline",(TGraph*)(MECGraph->At(i)));
      }
    }

    //If it's a 2014 or 2015 style file, make those splines.
    if(fileType == 1 || fileType == 2){

      if(MEC_CGraph->At(i) != NULL){
        if(((TGraph*)(MEC_CGraph->At(i)))->GetN()>1) MEC_CSpline[i] = new TSpline3("MEC_CSpline",(TGraph*)(MEC_CGraph->At(i)));
      }

      if(PF_CGraph->At(i) != NULL){
        if(((TGraph*)(PF_CGraph->At(i)))->GetN()>1) PF_CSpline[i] = new TSpline3("PF_CSpline",(TGraph*)(PF_CGraph->At(i)));
      }

      if(EB_CGraph->At(i) != NULL){
        if(((TGraph*)(EB_CGraph->At(i)))->GetN()>1) EB_CSpline[i] = new TSpline3("EB_CSpline",(TGraph*)(EB_CGraph->At(i)));
      }

      if(MEC_OGraph->At(i) != NULL){
        if(((TGraph*)(MEC_OGraph->At(i)))->GetN()>1) MEC_OSpline[i] = new TSpline3("MEC_OSpline",(TGraph*)(MEC_OGraph->At(i)));
      }

      if(PF_OGraph->At(i) != NULL){
        if(((TGraph*)(PF_OGraph->At(i)))->GetN()>1) PF_OSpline[i] = new TSpline3("PF_OSpline",(TGraph*)(PF_OGraph->At(i)));
      }

      if(EB_OGraph->At(i) != NULL){
        if(((TGraph*)(EB_OGraph->At(i)))->GetN()>1) EB_OSpline[i] = new TSpline3("EB_OSpline",(TGraph*)(EB_OGraph->At(i)));
      }

      if(CA5Graph->At(i) != NULL){
        if(((TGraph*)(CA5Graph->At(i)))->GetN()>1) CA5Spline[i] = new TSpline3("CA5Spline",(TGraph*)(CA5Graph->At(i)));
      }

      if(BgSclResGraph->At(i) != NULL){
        if(((TGraph*)(BgSclResGraph->At(i)))->GetN()>1) BgSclResSpline[i] = new TSpline3("BgSclResSpline",(TGraph*)(BgSclResGraph->At(i)));
      }

      if(SCCVGraph->At(i) != NULL){
        if(((TGraph*)(SCCVGraph->At(i)))->GetN()>1) SCCVSpline[i] = new TSpline3("SCCVSpline",(TGraph*)(SCCVGraph->At(i)));
      }

      if(SCCAGraph->At(i) != NULL){
        if(((TGraph*)(SCCAGraph->At(i)))->GetN()>1) SCCASpline[i] = new TSpline3("SCCASpline",(TGraph*)(SCCAGraph->At(i)));
      }

      //We do not want splines for SF_RFG and RPA.  So just leave these
      //entries NULL.
            
      if(CCNuEGraph->At(i) != NULL){
        if(((TGraph*)(CCNuEGraph->At(i)))->GetN()>1) CCNuESpline[i] = new TSpline3("CCNuESpline",(TGraph*)(CCNuEGraph->At(i)));
      }

    }
    //Finally, if it's a 2016 style file, make those splines
    if(fileType == 3){

      if(MEC_CGraph->At(i) != NULL){
        if(((TGraph*)(MEC_CGraph->At(i)))->GetN()>1) MEC_CSpline[i] = new TSpline3("MEC_CSpline",(TGraph*)(MEC_CGraph->At(i)));
      }

      if(PF_CGraph->At(i) != NULL){
        if(((TGraph*)(PF_CGraph->At(i)))->GetN()>1) PF_CSpline[i] = new TSpline3("PF_CSpline",(TGraph*)(PF_CGraph->At(i)));
      }

      if(MEC_OGraph->At(i) != NULL){
        if(((TGraph*)(MEC_OGraph->At(i)))->GetN()>1) MEC_OSpline[i] = new TSpline3("MEC_OSpline",(TGraph*)(MEC_OGraph->At(i)));
      }

      if(PF_OGraph->At(i) != NULL){
        if(((TGraph*)(PF_OGraph->At(i)))->GetN()>1) PF_OSpline[i] = new TSpline3("PF_OSpline",(TGraph*)(PF_OGraph->At(i)));
      }

      if(CA5Graph->At(i) != NULL){
        if(((TGraph*)(CA5Graph->At(i)))->GetN()>1) CA5Spline[i] = new TSpline3("CA5Spline",(TGraph*)(CA5Graph->At(i)));
      }

      if(BgSclResGraph->At(i) != NULL){
        if(((TGraph*)(BgSclResGraph->At(i)))->GetN()>1) BgSclResSpline[i] = new TSpline3("BgSclResSpline",(TGraph*)(BgSclResGraph->At(i)));
      }

      if(SCCVGraph->At(i) != NULL){
        if(((TGraph*)(SCCVGraph->At(i)))->GetN()>1) SCCVSpline[i] = new TSpline3("SCCVSpline",(TGraph*)(SCCVGraph->At(i)));
      }

      if(SCCAGraph->At(i) != NULL){
        if(((TGraph*)(SCCAGraph->At(i)))->GetN()>1) SCCASpline[i] = new TSpline3("SCCASpline",(TGraph*)(SCCAGraph->At(i)));
      }

      //We do not want splines for SF_RFG and RPA.  So just leave these
      //entries NULL.
            
      if(CCNuEGraph->At(i) != NULL){
        if(((TGraph*)(CCNuEGraph->At(i)))->GetN()>1) CCNuESpline[i] = new TSpline3("CCNuESpline",(TGraph*)(CCNuEGraph->At(i)));
      }
			
      if(PDD_CGraph->At(i) != NULL){
        if(((TGraph*)(PDD_CGraph->At(i)))->GetN()>1) PDD_CSpline[i] = new TSpline3("PDD_CSpline",(TGraph*)(PDD_CGraph->At(i)));
      }
			
      if(PDD_OGraph->At(i) != NULL){
        if(((TGraph*)(PDD_OGraph->At(i)))->GetN()>1) PDD_OSpline[i] = new TSpline3("PDD_OSpline",(TGraph*)(PDD_OGraph->At(i)));
      }

    }

  }
}

TClonesArray* WeightFile::GetResponseGraph(XsecSysts systPar){
  if(systPar==MAQE) return MAQEGraph;
  else if(systPar==MARES) return MARESGraph;
  else if(systPar==DISMPISHP) return DISMPISHPGraph;
  else if(systPar==SF) return SFGraph;
  else if(systPar==EB) return EBGraph;
  else if(systPar==PF) return PFGraph;
  else if(systPar==PDD) return PDDGraph;
  else if(systPar==FSI_PI_ABS) return FSI_PI_ABSGraph;
  else if(systPar==FSI_PI_PROD) return FSI_PI_PRODGraph;
  else if(systPar==FSI_CEX_LO) return FSI_CEX_LOGraph;
  else if(systPar==FSI_CEX_HI) return FSI_CEX_HIGraph;
  else if(systPar==FSI_INEL_LO) return FSI_INEL_LOGraph;
  else if(systPar==FSI_INEL_HI) return FSI_INEL_HIGraph;
  else if(systPar==MEC) return MECGraph;

  //2014 additions.
  else if(systPar == MEC_C) return MEC_CGraph;
  else if(systPar == PF_C) return PF_CGraph;
  else if(systPar == EB_C) return EB_CGraph;
  else if(systPar == MEC_O) return MEC_OGraph;
  else if(systPar == PF_O) return PF_OGraph;
  else if(systPar == EB_O) return EB_OGraph;
  else if(systPar == CA5) return CA5Graph;
  else if(systPar == BgSclRes) return BgSclResGraph;
  else if(systPar == SCCV) return SCCVGraph;
  else if(systPar == SCCA) return SCCAGraph;
  else if(systPar == SF_RFG) return SF_RFGGraph;
  else if(systPar == RPA) return RPAGraph;
  else if(systPar == CCNuE) return CCNuEGraph;
	
  //2016 winter additions
  else if(systPar == MEC_shape_C) return PDD_CGraph;
  else if(systPar == MEC_shape_O) return PDD_OGraph;
	
  else return NULL;
}

TSpline3** WeightFile::GetResponseSpline(XsecSysts systPar){
  if(systPar==MAQE) return MAQESpline;
  else if(systPar==MARES) return MARESSpline;
  else if(systPar==DISMPISHP) return DISMPISHPSpline;
  else if(systPar==SF) return SFSpline;
  else if(systPar==EB) return EBSpline;
  else if(systPar==PF) return PFSpline;
  else if(systPar==PDD) return PDDSpline;
  else if(systPar==FSI_PI_ABS) return FSI_PI_ABSSpline;
  else if(systPar==FSI_PI_PROD) return FSI_PI_PRODSpline;
  else if(systPar==FSI_CEX_LO) return FSI_CEX_LOSpline;
  else if(systPar==FSI_CEX_HI) return FSI_CEX_HISpline;
  else if(systPar==FSI_INEL_LO) return FSI_INEL_LOSpline;
  else if(systPar==FSI_INEL_HI) return FSI_INEL_HISpline;
  else if(systPar==PDD_MEC) return PDD_MECSpline;
  else if(systPar==MEC) return MECSpline;

  //2014 additions.
  else if(systPar == MEC_C) return MEC_CSpline;
  else if(systPar == PF_C) return PF_CSpline;
  else if(systPar == EB_C) return EB_CSpline;
  else if(systPar == MEC_O) return MEC_OSpline;
  else if(systPar == PF_O) return PF_OSpline;
  else if(systPar == EB_O) return EB_OSpline;
  else if(systPar == CA5) return CA5Spline;
  else if(systPar == BgSclRes) return BgSclResSpline;
  else if(systPar == SCCV) return SCCVSpline;
  else if(systPar == SCCA) return SCCASpline;
   
  //Even though they are full of NULL entries, return the SF_RFGSpline and
  //RPASpline arrays to preserve structure.
  else if(systPar == SF_RFG) return SF_RFGSpline;
  else if (systPar == RPA) return RPASpline;

  else if (systPar == CCNuE) return CCNuESpline;
    
  //2016 additions
  else if(systPar == MEC_shape_C) return PDD_CSpline;
  else if(systPar == MEC_shape_O) return PDD_OSpline;
	
  else return NULL;
}

double* WeightFile::GetXSecWeightArray(XsecSysts systPar){

  if(systPar == COH_BS) return CohTuningWeight;

  return NULL;

}


