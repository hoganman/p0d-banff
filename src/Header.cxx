#include "Header.hxx"
#include "TFile.h"
#include <stdio.h>
#include <iostream>
ClassImp(Header)

//********************************************************************
Header::Header(){
//********************************************************************
  Reset();
}

//********************************************************************
void Header::Reset() {
//********************************************************************

  _POT_CountedPerFile = 0;
  _POT_NoCut = 0;
  _POT_BadBeam = 0;
  _POT_BadND280 = 0;
  _POT_GoodBeamGoodND280 = 0;
  _POT_0KA = 0;
  _POT_200KA = 0;
  _POT_250KA = 0;
  _POT_m250KA = 0;
  _POT_OtherKA = 0;
  _Spill_NoCut = 0;
  _Spill_BadBeam = 0;
  _Spill_BadND280 = 0;
  _Spill_GoodBeamGoodND280 = 0;

  _SoftwareVersion="";
  _IsMC = false;
  _isFirstFile=true;
}

//********************************************************************
bool Header::AddHeader(TTree* tree, bool bySpillPOT){
//********************************************************************

  if (!tree)
  {
    std::cout << "TTree in Header class does NOT exist!" << std::endl;
    return false;
  }

  double tmp_POT_NoCut, tmp_POT_BadBeam, tmp_POT_BadND280;
  int tmp_Spill_NoCut, tmp_Spill_BadBeam, tmp_Spill_BadND280;
  double tmp_POT_GoodBeamGoodND280;
  int tmp_Spill_GoodBeamGoodND280;
  double tmp_POT_0KA, tmp_POT_200KA, tmp_POT_250KA, tmp_POT_m250KA, tmp_POT_OtherKA;
  char tmp_SoftwareVersion[100];
  TBranch* b_tmp_SoftwareVersion = nullptr;
  TBranch* b_tmp_POT_NoCut = nullptr;
  TBranch* b_tmp_POT_BadBeam = nullptr;
  TBranch* b_tmp_POT_BadND280 = nullptr;
  TBranch* b_tmp_POT_GoodBeamGoodND280 = nullptr;
  TBranch* b_tmp_POT_0KA = nullptr;
  TBranch* b_tmp_POT_200KA = nullptr;
  TBranch* b_tmp_POT_250KA = nullptr;
  TBranch* b_tmp_POT_m250KA = nullptr;
  TBranch* b_tmp_POT_OtherKA = nullptr;
  TBranch* b_tmp_Spill_NoCut = nullptr;
  TBranch* b_tmp_Spill_BadBeam = nullptr;
  TBranch* b_tmp_Spill_BadND280 = nullptr;
  TBranch* b_tmp_Spill_GoodBeamGoodND280 = nullptr;

  if (tree->FindLeaf("POT_NoCut")){
    tree->SetBranchAddress("SoftwareVersion",         tmp_SoftwareVersion,          &b_tmp_SoftwareVersion);
    tree->SetBranchAddress("POT_NoCut",               &tmp_POT_NoCut,               &b_tmp_POT_NoCut);
    tree->SetBranchAddress("POT_BadBeam",             &tmp_POT_BadBeam,             &b_tmp_POT_BadBeam);
    tree->SetBranchAddress("POT_BadND280",            &tmp_POT_BadND280,            &b_tmp_POT_BadND280);
    tree->SetBranchAddress("POT_GoodBeamGoodND280",   &tmp_POT_GoodBeamGoodND280,   &b_tmp_POT_GoodBeamGoodND280);
    tree->SetBranchAddress("POT_0KA",                 &tmp_POT_0KA,                 &b_tmp_POT_0KA);
    tree->SetBranchAddress("POT_200KA",               &tmp_POT_200KA,               &b_tmp_POT_200KA);
    tree->SetBranchAddress("POT_250KA",               &tmp_POT_250KA,               &b_tmp_POT_250KA);
    tree->SetBranchAddress("POT_m250KA",              &tmp_POT_m250KA,              &b_tmp_POT_m250KA);
    tree->SetBranchAddress("POT_OtherKA",             &tmp_POT_OtherKA,             &b_tmp_POT_OtherKA);
    tree->SetBranchAddress("Spill_NoCut",             &tmp_Spill_NoCut,             &b_tmp_Spill_NoCut);
    tree->SetBranchAddress("Spill_BadBeam",           &tmp_Spill_BadBeam,           &b_tmp_Spill_BadBeam);
    tree->SetBranchAddress("Spill_BadND280",          &tmp_Spill_BadND280,          &b_tmp_Spill_BadND280);
    tree->SetBranchAddress("Spill_GoodBeamGoodND280", &tmp_Spill_GoodBeamGoodND280, &b_tmp_Spill_GoodBeamGoodND280);
    // There can be multiple entries in the "header" tree if the analyser has
    // used hadd to merge multiple output trees. We sum the total POT in all
    // entries.
    for (int i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);
      _POT_CountedPerFile += tmp_POT_NoCut;
      _POT_NoCut += tmp_POT_NoCut;
      _POT_BadBeam += tmp_POT_BadBeam;
      _POT_BadND280 += tmp_POT_BadND280;
      _POT_GoodBeamGoodND280 += tmp_POT_GoodBeamGoodND280;
      _POT_0KA += tmp_POT_0KA;
      _POT_200KA += tmp_POT_200KA;
      _POT_250KA += tmp_POT_250KA;
      _POT_m250KA += tmp_POT_m250KA;
      _POT_OtherKA += tmp_POT_OtherKA;
      _Spill_NoCut += tmp_Spill_NoCut;
      _Spill_BadBeam += tmp_Spill_BadBeam;
      _Spill_BadND280 += tmp_Spill_BadND280;
      _Spill_GoodBeamGoodND280 += tmp_Spill_GoodBeamGoodND280;
    }
  }
  else{
    Header *header=0;
    TBranch* b_header = tree->GetBranch("POTInfo");
    b_header->SetAddress(&header);

    // There can be multiple entries in the "header" tree if the analyser has
    // used hadd to merge multiple output trees. We sum the total POT in all
    // entries.
    for (int i = 0; i < tree->GetEntries(); i++) {
      tree->GetEntry(i);

      _POT_CountedPerFile      += header->GetPOTCountedPerFile();
      if (!bySpillPOT){
        _POT_NoCut               += header->GetPOTNoCut();
        _POT_BadBeam             += header->GetPOTBadBeam();
        _POT_BadND280            += header->GetPOTBadND280();
        _POT_GoodBeamGoodND280   += header->GetPOTGoodBeamGoodND280();
        _POT_0KA                 += header->GetPOT0KA();
        _POT_200KA               += header->GetPOT200KA();
        _POT_250KA               += header->GetPOT250KA();
        _POT_m250KA              += header->GetPOTMinus250KA();
        _POT_OtherKA             += header->GetPOTOtherKA();
        _Spill_NoCut             += header->GetSpillNoCut();
        _Spill_BadBeam           += header->GetSpillBadBeam();
        _Spill_BadND280          += header->GetSpillBadND280();
        _Spill_GoodBeamGoodND280 += header->GetSpillGoodBeamGoodND280();
      }
    }
  }

  return true;
}

//********************************************************************
void Header::ReadHeader(TTree* tree){
//********************************************************************

  if (!tree) return;
  Reset();
  AddHeader(tree);
}

//********************************************************************
bool Header::AddHeader(const std::string& file, bool bySpillPOT){
//********************************************************************

  TFile* inFile = TFile::Open(file.c_str());
  if(!inFile->Get("header"))
  {
    std::cout << "ERROR: Unable to get TTree header from " << file.c_str() << std::endl;
    inFile->Close();
    return false;
  }
  inFile->Close();
  TChain chain("header");
  chain.AddFile(file.c_str());
  return AddHeader(&chain, bySpillPOT);
}

//********************************************************************
void Header::ReadHeader(const std::string& file){
//********************************************************************

  TFile* inFile = TFile::Open(file.c_str());
  if(!inFile->Get("header"))
  {
    std::cout << "ERROR: Unable to get TTree header from " << file.c_str() << std::endl;
    inFile->Close();
    return;
  }
  TChain chain("header");
  chain.AddFile(file.c_str());
  ReadHeader(&chain);
}

//********************************************************************
void Header::DumpPOT() {
//********************************************************************
  std::cout << "Initial POT (per File).. " << _POT_CountedPerFile << std::endl;
  std::cout << "Initial POT............. " << _POT_NoCut << " Spills " << _Spill_NoCut  << std::endl;
  std::cout << "|-- Bad Beam............ " << _POT_BadBeam << " Spills " << _Spill_BadBeam  << std::endl;
  std::cout << "|-- Bad ND280........... " << _POT_BadND280 << " Spills " << _Spill_BadND280  << std::endl;
  std::cout << "|-- Total Good POT...... " << _POT_GoodBeamGoodND280 << " Spills " << _Spill_GoodBeamGoodND280  << std::endl;
  std::cout << "  |-- @  0KA............ " << _POT_0KA << std::endl;
  std::cout << "  |-- @  200KA.......... " << _POT_200KA << std::endl;
  std::cout << "  |-- @  250KA.......... " << _POT_250KA << std::endl;
  std::cout << "  |-- @ -250KA.......... " << _POT_m250KA << std::endl;
  std::cout << "  |-- @  Other power.... " << _POT_OtherKA << std::endl;
}
