//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr  1 14:23:40 2011 by ROOT version 5.28/00
// from TChain SK__h1/
//
// SK 11a file modified for T2KReWeight by P. de Perio
//
//////////////////////////////////////////////////////////

#include <TROOT.h>
#include <TTree.h>

#ifndef SK__h1_h
#define SK__h1_h

namespace SK {
  class SK__h1;
} // end of namespace

namespace SK {
class SK__h1 {

public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           nring;
   //UInt_t          nrun;
   Int_t           nev;
   //Int_t           nsub;
   //UInt_t          cate;
   //Float_t         potot;
   //UInt_t          nhit;
   //Float_t         pomax;
   //Float_t         potota;
   //UInt_t          nhita;
   UInt_t          nhitac;
   //Float_t         pomaxa;
   Float_t         wall;
   Float_t         evis;
   //Float_t         rtsum;
   //Float_t         rtmax;
   //Float_t         wlen;
   UInt_t          ip[10];   //[nring]
   //Float_t         pos[3];
   Float_t         dir[10][3];   //[nring]
   //Float_t         dirtot[3];
   //Float_t         ang[10];   //[nring]
   //Float_t         rtot[10];   //[nring]
   Float_t         amom[10];   //[nring]
   //Float_t         rtote[10];   //[nring]
   Float_t         amome[10];   //[nring]
   //Float_t         rtotm[10];   //[nring]
   Float_t         amomm[10];   //[nring]
   //UInt_t          nsube;
   //UInt_t          ndcy;
   //UInt_t          ngate;
   //UInt_t          nbye;
   //Float_t         probms[10][6];   //[nring]
   //Float_t         prmslg[10][6];   //[nring]
   //Int_t           date[3];
   //Int_t           time[4];
   //Float_t         elpsday;
   //Int_t           numpo[10];   //[nring]
   //Float_t         apos[3];
   //Float_t         adir[3];
   //Float_t         aang;
   //Float_t         agood;
   //Float_t         wgain;
   //Int_t           nbad;
   //Int_t           nbada;
   //Float_t         msdir[10][3][6];   //[nring]
   //Float_t         amomp[10];
   //Float_t         ange[10];
   //Float_t         angm[10];
   //Float_t         angp[10];
   //Int_t           ntot[10];
   //Float_t         probth[10][6];
   //Float_t         probpt[10][6];
   Float_t         pi0like[2];
   Float_t         pi0_e[2][2];
   Float_t         pi0_dir[2][2][3];
   Float_t         pi0mass[2];
   //Float_t         evisold;
   //Float_t         evisoldxe;
   //Float_t         evisnew;
   Int_t           nmue;
   //UInt_t          etype[10];   //[nmue]
   //Float_t         etime[10];   //[nmue]
   //Float_t         epos[10][3];   //[nmue]
   //Float_t         edir[10][3];   //[nmue]
   //Float_t         egood[10];   //[nmue]
   //Float_t         ehit[10];   //[nmue]
   //Float_t         mueprob[2];
   //Int_t           Rnring;
   //Float_t         Rdir[30][3];   //[Rnring]
   //Float_t         Rang[30];   //[Rnring]
   //UInt_t          Riring;
   //Float_t         Rtwout[30];   //[Rnring]
   //Float_t         Rtwith[30];   //[Rnring]
   //Float_t         Alwout;
   //Float_t         Alwith;
   //Float_t         Qsmi;
   //Float_t         Qsmo;
   //Float_t         Qexi;
   //Float_t         Qexo;
   //Float_t         Pe5d;
   //Float_t         En5d;
   //Float_t         Eh5d;
   //Float_t         Pe5do;
   //Float_t         En5do;
   //Float_t         Eh5do;
   //Float_t         Rtadd;
   //Float_t         Pdgeta;
   //Float_t         Pd5d;
   //Float_t         Pdthre;
   //Float_t         Pd5do;
   //Float_t         Delpd;
   //Float_t         Ropena[30];   //[Rnring]
   //Int_t           Maxth;
   //Float_t         Pkang;
   //Float_t         Qrfct;
   //Float_t         Pdfct;
   //Float_t         Pkfct;
   //Float_t         Agfct;
   //Float_t         Dlfct;
   //Int_t           Iflag;
   //Float_t         Pmfct;
   //Float_t         Imfct;
   //Float_t         Rilike;
   //Int_t           ri_ver;
   //Float_t         ri_pid;
   //Int_t           ri_nring;
   //Float_t         ri_flag[10];   //[ri_nring]
   //Float_t         ri_dlfct[10];   //[ri_nring]
   //Float_t         ri_pdfct[10];   //[ri_nring]
   //Float_t         ri_pkfct[10];   //[ri_nring]
   //Float_t         ri_vafct[10];   //[ri_nring]
   //Float_t         ri_total[10];   //[ri_nring]
   //Float_t         ri_dir[10][3];   //[ri_nring]
   //Float_t         ri_imfct[10];   //[ri_nring]
   //Float_t         ri_pmfct[10];   //[ri_nring]
   Int_t           npar;
   Float_t         wallv;
   UInt_t          ipv[50];   //[npar]
   Float_t         posv[3];
   Float_t         dirv[50][3];   //[npar]
   Float_t         pmomv[50];   //[npar]  MeV/c
   //Int_t           light_flag[50];   //[npar]
   //Int_t           npar2;
   //Float_t         wallv2[50];   //[npar2]
   //UInt_t          ipv2[50];   //[npar2]
   //UInt_t          iorg[50];   //[npar2]
   //Float_t         posv2[50][3];   //[npar2]
   //Float_t         dirv2[50][3];   //[npar2]
   //Float_t         pmomv2[50];   //[npar2]
   Int_t           numnu;
   Int_t           mode;
   Int_t           ipnu[50];   //[numnu]
   Float_t         pnu[50];   //[numnu]  GeV/c
   Float_t         dirnu[50][3];   //[numnu]
   //Int_t           scan[2];
   //Int_t           nuceff_version;
   //Int_t           charge_exchange;
   //Int_t           absorbed;
   //Int_t           multipi_gen;
   //Int_t           scattering;
   //Int_t           piless_dcy;
   Int_t           nscndprt;
   Int_t           itrkscnd[200];   //[nscndprt]
   Float_t         vtxscnd[200][3];   //[nscndprt]
   Float_t         pscnd[200][3];   //[nscndprt]
   Int_t           iprtscnd[200];   //[nscndprt]
   Float_t         tscnd[200];   //[nscndprt]
   Int_t           iprntprt[200];   //[nscndprt]
   Int_t           lmecscnd[200];   //[nscndprt]
   Int_t           iprnttrk[200];   //[nscndprt]
   Int_t           iorgprt[200];   //[nscndprt]
   Int_t           iflgscnd[200];   //[nscndprt]
   Float_t         pprnt[200][3];   //[nscndprt]
   //Int_t           Neutmode;
   //Int_t           Npne;
   //Int_t           Ipne[100];   //[Npne]
   //Float_t         Abspne[100];   //[Npne]
   //Float_t         Pvecne[100][3];   //[Npne]
   Int_t           Npvc;
   Int_t           Ipvc[100];   //[Npvc]
   Int_t           Ichvc[100];   //[Npvc]
   Int_t           Iorgvc[100];   //[Npvc]
   Int_t           Iflvc[100];   //[Npvc]
   Float_t         Abspvc[100];   //[Npvc]
   Float_t         Pvc[100][3];   //[Npvc]
   Float_t         Crsx;
   Float_t         Crsy;
   Float_t         Crsz;
   Float_t         Crsphi;
   Int_t           Nvert;
   Float_t         Posvert[150][3];   //[Nvert]
   Int_t           Iflgvert[150];   //[Nvert]
   Int_t           Nvcvert;
   Float_t         Dirvert[900][3];   //[Nvcvert]
   Float_t         Abspvert[900];   //[Nvcvert]
   Float_t         Abstpvert[900];   //[Nvcvert]
   Int_t           Ipvert[900];   //[Nvcvert]
   Int_t           Iverti[900];   //[Nvcvert]
   Int_t           Ivertf[900];   //[Nvcvert]
   Float_t         Fsiprob;

   //Float_t         t2kposc[2][6];

   Int_t           Numbndn;
   Int_t           Numbndp;
   Int_t           Numfrep;
   Int_t           Numatom;
   Int_t           Ibound;

   Int_t           Nvertsk;
   Float_t         Posvertsk[300][3];   //[Nvertsk]
   Int_t           Iflgvertsk[300];   //[Nvertsk]
   Int_t           Nvcvertsk;
   Float_t         Dirvertsk[900][3];   //[Nvcvertsk]
   Float_t         Abspvertsk[900];   //[Nvcvertsk]
   Float_t         Abstpvertsk[900];   //[Nvcvertsk]
   Int_t           Ipvertsk[900];   //[Nvcvertsk]
   Int_t           Ivertisk[900];   //[Nvcvertsk]
   Int_t           Ivertfsk[900];   //[Nvcvertsk]
   Float_t         Fsiprobsk;

   // Added for T2KReWeight (not in original ntuple)
   Int_t           filetype;  // 0: non-signal nue, 1: signal nue

   // End of tree variables

   Float_t         fRecEnu;

   Float_t         coslep();
   Float_t         enurec();  // GeV
   Float_t         recpi0mass(Int_t ir1,Int_t ir2);  // MeV
   Float_t         recpi0mom(Int_t ir1,Int_t ir2);   // MeV

   // List of branches
   TBranch        *b_nring;   //!
   //TBranch        *b_nrun;   //!
   TBranch        *b_nev;   //!
   //TBranch        *b_nsub;   //!
   //TBranch        *b_cate;   //!
   //TBranch        *b_potot;   //!
   //TBranch        *b_nhit;   //!
   //TBranch        *b_pomax;   //!
   //TBranch        *b_potota;   //!
   //TBranch        *b_nhita;   //!
   TBranch        *b_nhitac;   //!
   //TBranch        *b_pomaxa;   //!
   TBranch        *b_wall;   //!
   TBranch        *b_evis;   //!
   //TBranch        *b_rtsum;   //!
   //TBranch        *b_rtmax;   //!
   //TBranch        *b_wlen;   //!
   TBranch        *b_ip;   //!
   //TBranch        *b_pos;   //!
   TBranch        *b_dir;   //!
   //TBranch        *b_dirtot;   //!
   //TBranch        *b_ang;   //!
   //TBranch        *b_rtot;   //!
   TBranch        *b_amom;   //!
   //TBranch        *b_rtote;   //!
   TBranch        *b_amome;   //!
   //TBranch        *b_rtotm;   //!
   TBranch        *b_amomm;   //!
   //TBranch        *b_nsube;   //!
   //TBranch        *b_ndcy;   //!
   //TBranch        *b_ngate;   //!
   //TBranch        *b_nbye;   //!
   //TBranch        *b_probms;   //!
   //TBranch        *b_prmslg;   //!
   //TBranch        *b_date;   //!
   //TBranch        *b_time;   //!
   //TBranch        *b_elpsday;   //!
   //TBranch        *b_numpo;   //!
   //TBranch        *b_apos;   //!
   //TBranch        *b_adir;   //!
   //TBranch        *b_aang;   //!
   //TBranch        *b_agood;   //!
   //TBranch        *b_wgain;   //!
   //TBranch        *b_nbad;   //!
   //TBranch        *b_nbada;   //!
   //TBranch        *b_msdir;   //!
   //TBranch        *b_amomp;   //!
   //TBranch        *b_ange;   //!
   //TBranch        *b_angm;   //!
   //TBranch        *b_angp;   //!
   //TBranch        *b_ntot;   //!
   //TBranch        *b_probth;   //!
   //TBranch        *b_probpt;   //!
   TBranch        *b_pi0like;   //!
   TBranch        *b_pi0_e;   //!
   TBranch        *b_pi0_dir;   //!
   TBranch        *b_pi0mass;   //!
   //TBranch        *b_evisold;   //!
   //TBranch        *b_evisoldxe;   //!
   //TBranch        *b_evisnew;   //!
   TBranch        *b_nmue;   //!
   //TBranch        *b_etype;   //!
   //TBranch        *b_etime;   //!
   //TBranch        *b_epos;   //!
   //TBranch        *b_edir;   //!
   //TBranch        *b_egood;   //!
   //TBranch        *b_ehit;   //!
   //TBranch        *b_mueprob;   //!
   //TBranch        *b_Rnring;   //!
   //TBranch        *b_Rdir;   //!
   //TBranch        *b_Rang;   //!
   //TBranch        *b_Riring;   //!
   //TBranch        *b_Rtwout;   //!
   //TBranch        *b_Rtwith;   //!
   //TBranch        *b_Alwout;   //!
   //TBranch        *b_Alwith;   //!
   //TBranch        *b_Qsmi;   //!
   //TBranch        *b_Qsmo;   //!
   //TBranch        *b_Qexi;   //!
   //TBranch        *b_Qexo;   //!
   //TBranch        *b_Pe5d;   //!
   //TBranch        *b_En5d;   //!
   //TBranch        *b_Eh5d;   //!
   //TBranch        *b_Pe5do;   //!
   //TBranch        *b_En5do;   //!
   //TBranch        *b_Eh5do;   //!
   //TBranch        *b_Rtadd;   //!
   //TBranch        *b_Pdgeta;   //!
   //TBranch        *b_Pd5d;   //!
   //TBranch        *b_Pdthre;   //!
   //TBranch        *b_Pd5do;   //!
   //TBranch        *b_Delpd;   //!
   //TBranch        *b_Ropena;   //!
   //TBranch        *b_Maxth;   //!
   //TBranch        *b_Pkang;   //!
   //TBranch        *b_Qrfct;   //!
   //TBranch        *b_Pdfct;   //!
   //TBranch        *b_Pkfct;   //!
   //TBranch        *b_Agfct;   //!
   //TBranch        *b_Dlfct;   //!
   //TBranch        *b_Iflag;   //!
   //TBranch        *b_Pmfct;   //!
   //TBranch        *b_Imfct;   //!
   //TBranch        *b_Rilike;   //!
   //TBranch        *b_ri_ver;   //!
   //TBranch        *b_ri_pid;   //!
   //TBranch        *b_ri_nring;   //!
   //TBranch        *b_ri_flag;   //!
   //TBranch        *b_ri_dlfct;   //!
   //TBranch        *b_ri_pdfct;   //!
   //TBranch        *b_ri_pkfct;   //!
   //TBranch        *b_ri_vafct;   //!
   //TBranch        *b_ri_total;   //!
   //TBranch        *b_ri_dir;   //!
   //TBranch        *b_ri_imfct;   //!
   //TBranch        *b_ri_pmfct;   //!
   TBranch        *b_npar;   //!
   TBranch        *b_wallv;   //!
   TBranch        *b_ipv;   //!
   TBranch        *b_posv;   //!
   TBranch        *b_dirv;   //!
   TBranch        *b_pmomv;   //!
   //TBranch        *b_light_flag;   //!
   //TBranch        *b_npar2;   //!
   //TBranch        *b_wallv2;   //!
   //TBranch        *b_ipv2;   //!
   //TBranch        *b_iorg;   //!
   //TBranch        *b_posv2;   //!
   //TBranch        *b_dirv2;   //!
   //TBranch        *b_pmomv2;   //!
   TBranch        *b_numnu;   //!
   TBranch        *b_mode;   //!
   TBranch        *b_ipnu;   //!
   TBranch        *b_pnu;   //!
   TBranch        *b_dirnu;   //!
   //TBranch        *b_scan;   //!
   //TBranch        *b_nuceff_version;   //!
   //TBranch        *b_charge_exchange;   //!
   //TBranch        *b_absorbed;   //!
   //TBranch        *b_multipi_gen;   //!
   //TBranch        *b_scattering;   //!
   //TBranch        *b_piless_dcy;   //!
   TBranch        *b_nscndprt;   //!
   TBranch        *b_itrkscnd;   //!
   TBranch        *b_vtxscnd;   //!
   TBranch        *b_pscnd;   //!
   TBranch        *b_iprtscnd;   //!
   TBranch        *b_tscnd;   //!
   TBranch        *b_iprntprt;   //!
   TBranch        *b_lmecscnd;   //!
   TBranch        *b_iprnttrk;   //!
   TBranch        *b_iorgprt;   //!
   TBranch        *b_pprnt;   //!
   TBranch        *b_iflgscnd;   //!
   //TBranch        *b_Neutmode;   //!
   //TBranch        *b_Npne;   //!
   //TBranch        *b_Ipne;   //!
   //TBranch        *b_Abspne;   //!
   //TBranch        *b_Pvecne;   //!
   TBranch        *b_Npvc;   //!
   TBranch        *b_Ipvc;   //!
   TBranch        *b_Ichvc;   //!
   TBranch        *b_Iorgvc;   //!
   TBranch        *b_Iflvc;   //!
   TBranch        *b_Abspvc;   //!
   TBranch        *b_Pvc;   //!
   TBranch        *b_Crsx;   //!
   TBranch        *b_Crsy;   //!
   TBranch        *b_Crsz;   //!
   TBranch        *b_Crsphi;   //!
   TBranch        *b_Nvert;   //!
   TBranch        *b_Posvert;   //!
   TBranch        *b_Iflgvert;   //!
   TBranch        *b_Nvcvert;   //!
   TBranch        *b_Dirvert;   //!
   TBranch        *b_Abspvert;   //!
   TBranch        *b_Abstpvert;   //!
   TBranch        *b_Ipvert;   //!
   TBranch        *b_Iverti;   //!
   TBranch        *b_Ivertf;   //!
   TBranch        *b_Fsiprob;   //!

   TBranch        *b_t2kposc;   //!

   TBranch        *b_Numbndn;   //!
   TBranch        *b_Numbndp;   //!
   TBranch        *b_Numfrep;   //!
   TBranch        *b_Numatom;   //!  
   TBranch        *b_Ibound;   //!  

   TBranch        *b_Nvertsk;   //!
   TBranch        *b_Posvertsk;   //!
   TBranch        *b_Iflgvertsk;   //!
   TBranch        *b_Nvcvertsk;   //!
   TBranch        *b_Dirvertsk;   //!
   TBranch        *b_Abspvertsk;   //!
   TBranch        *b_Abstpvertsk;   //!
   TBranch        *b_Ipvertsk;   //!
   TBranch        *b_Ivertisk;   //!
   TBranch        *b_Ivertfsk;   //!
   TBranch        *b_Fsiprobsk;   //!

   SK__h1(TTree *tree=0, bool a_sk=1);
   virtual ~SK__h1();
   virtual Int_t    GetEntries();
   virtual Int_t    GetEntry(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual Bool_t   Notify();

   bool f_sk;
};


}

#endif

