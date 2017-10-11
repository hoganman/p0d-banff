//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 10 20:02:56 2010 by ROOT version 5.18/00
// from TTree h3002/Near
// found on file: all_near_mr31_numu.root
//////////////////////////////////////////////////////////

#ifndef JnuBeamRead_h
#define JnuBeamRead_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class JnuBeamRead {
public :
   TChain          *fChain;   //!pointer to the analyzed TTree or TChain
   TChain          *fChain_geom;
   TChain          *fChain_tag;
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         Enu;
   Int_t           ppid;
   Int_t           mode;
   Float_t         ppi;
   Float_t         xpi[3];
   Float_t         npi[3];
   Float_t         cospibm;
   Float_t         norm;
   Float_t         anorm;
   Float_t         ppi0;
   Float_t         xpi0[3];
   Float_t         npi0[3];
   Float_t         cospi0bm;
   Float_t         xnu;
   Float_t         ynu;
   Float_t         nnu[3];
   Int_t           idfd;
   UChar_t         gipart;
   Float_t         gpos0[3];
   Float_t         gvec0[3];
   Float_t         gamom0;
   Int_t           ng;
   Float_t         gpx[20];   //[ng]
   Float_t         gpy[20];   //[ng]
   Float_t         gpz[20];   //[ng]
   Float_t         gcosbm[20];   //[ng]
   Float_t         gvx[20];   //[ng]
   Float_t         gvy[20];   //[ng]
   Float_t         gvz[20];   //[ng]
   Int_t           gpid[20];   //[ng]
   Int_t           gmec[20];   //[ng]
   Int_t           gmat[20];   //[ng]
   Float_t         gdistc[20];   //[ng]
   Float_t         gdistal[20];   //[ng]
   Float_t         gdistti[20];   //[ng]
   Float_t         gdistfe[20];   //[ng] 
   Float_t         Enusk;
   Float_t         normsk;

   Float_t         Hfd[20];
   Float_t         Vfd[20];
   Float_t         Xfd[20];
   Float_t         Yfd[20];
   Float_t         Zfd[20];

   Int_t           ntrig;
   Float_t         bpos[2];
   Float_t         btilt[2];
   Float_t         brms[2];
   Float_t         emit[2];
   Float_t         alpha[2];
   Float_t         hcur[3];

   // List of branches
   TBranch        *b_Enu;   //!
   TBranch        *b_ppid;   //!
   TBranch        *b_mode;   //!
   TBranch        *b_ppi;   //!
   TBranch        *b_xpi;   //!
   TBranch        *b_npi;   //! 
   TBranch        *b_cospibm;   //!
   TBranch        *b_norm;   //!
   TBranch        *b_anorm;   //!
   TBranch        *b_ppi0;   //!
   TBranch        *b_xpi0;   //!
   TBranch        *b_npi0;   //! 
   TBranch        *b_cospi0bm;   //!
   TBranch        *b_xnu;   //!
   TBranch        *b_ynu;   //!
   TBranch        *b_nnu;   //!
   TBranch        *b_idfd;   //!
   TBranch        *b_gipart;   //!
   TBranch        *b_gpos0;   //!
   TBranch        *b_gvec0;   //!
   TBranch        *b_gamom0;   //!
   TBranch        *b_ng;   //!
   TBranch        *b_gpx;   //!
   TBranch        *b_gpy;   //!
   TBranch        *b_gpz;   //!
   TBranch        *b_gcosbm;   //!
   TBranch        *b_gvx;   //!
   TBranch        *b_gvy;   //!
   TBranch        *b_gvz;   //!
   TBranch        *b_gpid;   //!
   TBranch        *b_gmec;   //!
   TBranch        *b_gmat;   //!
   TBranch        *b_gdistc;   //!
   TBranch        *b_gdistal;   //!
   TBranch        *b_gdistti;   //!
   TBranch        *b_gdistfe;   //!
   TBranch        *b_Enusk;   //!
   TBranch        *b_normsk;   //!

   TBranch        *b_Hfd;
   TBranch        *b_Vfd;
   TBranch        *b_Xfd;
   TBranch        *b_Yfd;
   TBranch        *b_Zfd;

   TBranch        *b_ntrig;
   TBranch        *b_bpos;
   TBranch        *b_btilt;
   TBranch        *b_brms;
   TBranch        *b_emit;
   TBranch        *b_alpha;
   TBranch        *b_hcur;
   bool do_near;

   JnuBeamRead(const char *filename,const char *release, bool near);
   virtual ~JnuBeamRead();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(int rel);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   Int_t GetEntries();
   void AddFile(const char *filename);
   int INGRIDModule(int det, double xpos, double ypos);
  
};

#endif

#ifdef JnuBeamRead_cxx
JnuBeamRead::JnuBeamRead(const char *filename,const char *release, bool near=true)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   do_near = near;
   char tree_name[50];
   if(near) sprintf(tree_name,"h3002");
   else sprintf(tree_name,"h2000");
   fChain = new TChain(tree_name);
   fChain->Add(filename);
   int rel = 0;
   if(strcmp(release,"10a")==0) rel = 1;
   else if(strcmp(release,"10b")==0) rel = 2;
   else if(strcmp(release,"10c")==0) rel = 3;
   else if(strcmp(release,"10d")==0) rel = 4;
   else if(strcmp(release,"11a")==0) rel = 5;

   fChain_geom = new TChain("h3000");
   if(near) fChain_geom->Add(filename);

   if (rel > 3) {
     fChain_tag = new TChain("h1000");
     fChain_tag->Add(filename);
   }

   Init(rel);

}

void JnuBeamRead::AddFile(const char *filename)
{
   fChain->Add(filename);
}


JnuBeamRead::~JnuBeamRead()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t JnuBeamRead::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t JnuBeamRead::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void JnuBeamRead::Init(int rel)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   //if (!tree) return;
   //fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Enu", &Enu, &b_Enu);
   fChain->SetBranchAddress("ppid", &ppid, &b_ppid);
   fChain->SetBranchAddress("mode", &mode, &b_mode);
   fChain->SetBranchAddress("ppi", &ppi, &b_ppi);
   fChain->SetBranchAddress("xpi", xpi, &b_xpi);
   fChain->SetBranchAddress("npi", npi, &b_npi);
   fChain->SetBranchAddress("cospibm", &cospibm, &b_cospibm);
   fChain->SetBranchAddress("norm", &norm, &b_norm);
   if(do_near) 
     fChain->SetBranchAddress("anorm", &anorm, &b_anorm);
   fChain->SetBranchAddress("ppi0", &ppi0, &b_ppi0);
   fChain->SetBranchAddress("xpi0", xpi0, &b_xpi0);
   fChain->SetBranchAddress("npi0", npi0, &b_npi0);
   fChain->SetBranchAddress("cospi0bm", &cospi0bm, &b_cospi0bm);
   if(do_near){ 
     fChain->SetBranchAddress("xnu", &xnu, &b_xnu);
     fChain->SetBranchAddress("ynu", &ynu, &b_ynu);
     fChain->SetBranchAddress("nnu", nnu, &b_nnu);
     fChain->SetBranchAddress("idfd", &idfd, &b_idfd);
   } 
   fChain->SetBranchAddress("gipart", &gipart, &b_gipart);
   fChain->SetBranchAddress("gpos0", gpos0, &b_gpos0);
   fChain->SetBranchAddress("gvec0", gvec0, &b_gvec0);
   fChain->SetBranchAddress("gamom0", &gamom0, &b_gamom0);
   fChain->SetBranchAddress("ng", &ng, &b_ng);
   fChain->SetBranchAddress("gpx", gpx, &b_gpx);
   fChain->SetBranchAddress("gpy", gpy, &b_gpy);
   fChain->SetBranchAddress("gpz", gpz, &b_gpz);
   fChain->SetBranchAddress("gcosbm", gcosbm, &b_gcosbm);
   fChain->SetBranchAddress("gvx", gvx, &b_gvx);
   fChain->SetBranchAddress("gvy", gvy, &b_gvy);
   fChain->SetBranchAddress("gvz", gvz, &b_gvz);
   fChain->SetBranchAddress("gpid", gpid, &b_gpid);
   fChain->SetBranchAddress("gmec", gmec, &b_gmec);
   if(rel>=5){
     fChain->SetBranchAddress("gdistc", gdistc, &b_gdistc);
     fChain->SetBranchAddress("gdistal", gdistal, &b_gdistal);
     fChain->SetBranchAddress("gdistti", gdistti, &b_gdistti);
     fChain->SetBranchAddress("gdistfe", gdistfe, &b_gdistfe);
     fChain->SetBranchAddress("gmat", gmat, &b_gmat);
   }
   if(do_near){
     fChain->SetBranchAddress("Enusk", &Enusk, &b_Enusk);
     fChain->SetBranchAddress("normsk", &normsk, &b_normsk);
   }

   if(do_near){
    fChain_geom->SetBranchAddress("Hfd",Hfd,&b_Hfd);
    fChain_geom->SetBranchAddress("Vfd",Vfd,&b_Vfd);
    fChain_geom->SetBranchAddress("Xfd",Xfd,&b_Xfd);
    fChain_geom->SetBranchAddress("Yfd",Yfd,&b_Yfd);
    fChain_geom->SetBranchAddress("Zfd",Zfd,&b_Zfd);
    fChain_geom->GetEntry(0);
   }

   if (rel>3) {
     fChain_tag->SetBranchAddress("ntrig",&ntrig,&b_ntrig);
     fChain_tag->SetBranchAddress("bpos",bpos,&b_bpos);
     fChain_tag->SetBranchAddress("btilt",btilt,&b_btilt);
     fChain_tag->SetBranchAddress("brms",brms,&b_brms);
     fChain_tag->SetBranchAddress("emit",emit,&b_emit);
     fChain_tag->SetBranchAddress("alpha",alpha,&b_alpha);
     fChain_tag->SetBranchAddress("hcur",hcur,&b_hcur);
     
     fChain_tag->GetEntry(0);
   }

   Notify();
}

Bool_t JnuBeamRead::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void JnuBeamRead::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t JnuBeamRead::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
Int_t JnuBeamRead::GetEntries()
{  
  return fChain->GetEntries();
}  

#endif // #ifdef JnuBeamRead_cxx
