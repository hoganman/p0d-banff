//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr  1 14:23:40 2011 by ROOT version 5.28/00
// from TChain SK__h1/
//
// SK 11a file modified for T2KReWeight by P. de Perio
//
//////////////////////////////////////////////////////////
#include <iostream>
#include <math.h>

#include "SK__h1.h"

using namespace SK;
using std::cout;
using std::endl;

SK__h1::SK__h1(TTree *tree, bool a_sk)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
     
     cout << "Error: SK__h1 constructed with NULL tree" << endl;
     
   }
   f_sk=a_sk;

   Init(tree);

   filetype=0; // non-signal nue file by default
}

SK__h1::~SK__h1()
{
   if (!fChain) return;
}

Int_t SK__h1::GetEntries()
{
   if (!fChain) return 0;
   return fChain->GetEntries();
}
Int_t SK__h1::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;

  Int_t bytes = fChain->GetEntry(entry);

  fRecEnu = enurec();

  // Default to water if target info not stored
  if (!b_Numbndn || !b_Numbndp || 
      !b_Numatom || !b_Numfrep ) {
    Numbndn = 8;
    Numbndp = 8;
    Numfrep = 2;
    Numatom = 16;
    Ibound = 1;
  }
  if (!b_Ibound) Ibound = 1;

  return bytes;
}



Float_t SK__h1::coslep(){
  Int_t i;
	
  Float_t costmp = 0;

  Float_t nudir[3] = {0.669764, -0.742179, 0.0242228};

  if ((numnu==0)&&(npar==0)){
    for ( i = 0 ; i < 3 ; i++ ){
      dirnu[0][i]=nudir[i];
    }
  }else{
    for ( i = 0 ; i < 3 ; i++ ){
      if (fabs(dirnu[0][i] - nudir[i])>0.000001){
	cout << "dirnu[0]=(" << dirnu[0][0]
	     << "," << dirnu[0][1]
	     << "," << dirnu[0][2] << ")" << endl;
	cout << "nudir   =(" << nudir[0]
	     << "," << nudir[1]
	     << "," << nudir[2] << ")" << endl;
      }
    }
  }
  
  Float_t dir2 = pow(dir[0][0],2)+pow(dir[0][1],2)+pow(dir[0][2],2);
  if (fabs(dir2-1.)>0.0001){
    cout << "SK__h1::coslep() Error: abs(dir) = " << dir2 << " is not 1\n" << endl;
  }
  for ( i = 0 ; i < 3 ; i++){
    costmp += dirnu[0][i]*dir[0][i];
  }
  return costmp;

}


Float_t SK__h1::enurec(){

  // Only for SK sample with nring
  if (!b_nring) return -1;

  Float_t erec;
  Float_t xmlep;
  const Float_t xmneutr= 0.93956536;
  const Float_t xmproto= 0.93827203;
  const Float_t xmelec = 0.000510999;
  const Float_t xmmu   = 0.10566;
  const Float_t vnuc   = 0.027;
	
  Float_t plep   = 0.;
  Float_t elep   = 0.;

  Double_t costh;

  if (ip[0] == 2){
    xmlep = xmelec;
    plep  = amome[0]/1000.;
  }else{
    xmlep = xmmu;
    plep  = amomm[0]/1000.;
  }

  elep = sqrt( xmlep * xmlep + plep * plep );

  costh = coslep();

  erec = (xmneutr-vnuc)*elep - ( xmlep*xmlep/2.0 ) +
    (xmneutr*vnuc)      - ( vnuc*vnuc/2.0 ) +
    ( (xmproto*xmproto - xmneutr*xmneutr) /2.0 ) ;
  erec = erec / (xmneutr - vnuc - elep + plep*costh );
	
  /*
    erec
    = (2 * xmneutr * elep - xmlep * xmlep)
    / (2 * xmneutr - 2 * elep + 2 * costh * plep );
  */
	  
  /*
    cout << "IP = " << ip[0]
    << " / ipnu[2] = " << ipnu[2] 
    << " / PTRUE = " << pnu[2]
    << " / Prec  = " << amomm[0]
    << " / ETRUE = " << pnu[0]
    << " / Erec  = " << erec[0]
    << " / Erec2 = " << erec2[0]
    << endl;
  */
  return erec;

}

Float_t SK__h1::recpi0mass(Int_t ir1,Int_t ir2){
  Float_t e;
  Float_t p;
	
  e = amome[ir1] + amome[ir2];
  p = recpi0mom(ir1,ir2);

  return ( sqrt(e * e - p * p) );
}

Float_t SK__h1::recpi0mom(Int_t ir1,Int_t ir2){
  Int_t i;
  Float_t p = 0;

  for ( i = 0 ; i < 3 ; i++ ){
    p = p 
      + ( ( amome[ir1]*dir[ir1][i]+amome[ir2]*dir[ir2][i])
	  *(amome[ir1]*dir[ir1][i]+amome[ir2]*dir[ir2][i]));
  }

  return sqrt(p);
}












void SK__h1::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   b_nring = NULL;
   b_Numbndn = NULL;
   b_Numbndp = NULL;
   b_Numfrep = NULL;
   b_Numatom = NULL;
   b_Ibound = NULL;

   if (f_sk) {
     fChain->SetBranchAddress("nring", &nring, &b_nring);
     //fChain->SetBranchAddress("nrun", &nrun, &b_nrun);
     fChain->SetBranchAddress("nev", &nev, &b_nev);
     //fChain->SetBranchAddress("nsub", &nsub, &b_nsub);
     //fChain->SetBranchAddress("cate", &cate, &b_cate);
     //fChain->SetBranchAddress("potot", &potot, &b_potot);
     //fChain->SetBranchAddress("nhit", &nhit, &b_nhit);
     //fChain->SetBranchAddress("pomax", &pomax, &b_pomax);
     //fChain->SetBranchAddress("potota", &potota, &b_potota);
     //fChain->SetBranchAddress("nhita", &nhita, &b_nhita);
     fChain->SetBranchAddress("nhitac", &nhitac, &b_nhitac);
     //fChain->SetBranchAddress("pomaxa", &pomaxa, &b_pomaxa);
     fChain->SetBranchAddress("wall", &wall, &b_wall);
     fChain->SetBranchAddress("evis", &evis, &b_evis);
     //fChain->SetBranchAddress("rtsum", &rtsum, &b_rtsum);
     //fChain->SetBranchAddress("rtmax", &rtmax, &b_rtmax);
     //fChain->SetBranchAddress("wlen", &wlen, &b_wlen);
     fChain->SetBranchAddress("ip", ip, &b_ip);
     //fChain->SetBranchAddress("pos", pos, &b_pos);
     fChain->SetBranchAddress("dir", dir, &b_dir);
     //fChain->SetBranchAddress("dirtot", dirtot, &b_dirtot);
     //fChain->SetBranchAddress("ang", ang, &b_ang);
     //fChain->SetBranchAddress("rtot", rtot, &b_rtot);
     fChain->SetBranchAddress("amom", amom, &b_amom);
     //fChain->SetBranchAddress("rtote", rtote, &b_rtote);
     fChain->SetBranchAddress("amome", amome, &b_amome);
     //fChain->SetBranchAddress("rtotm", rtotm, &b_rtotm);
     fChain->SetBranchAddress("amomm", amomm, &b_amomm);
     //fChain->SetBranchAddress("nsube", &nsube, &b_nsube);
     //fChain->SetBranchAddress("ndcy", &ndcy, &b_ndcy);
     //fChain->SetBranchAddress("ngate", &ngate, &b_ngate);
     //fChain->SetBranchAddress("nbye", &nbye, &b_nbye);
     //fChain->SetBranchAddress("probms", probms, &b_probms);
     //fChain->SetBranchAddress("prmslg", prmslg, &b_prmslg);
     //fChain->SetBranchAddress("date", date, &b_date);
     //fChain->SetBranchAddress("time", time, &b_time);
     //fChain->SetBranchAddress("elpsday", &elpsday, &b_elpsday);
     //fChain->SetBranchAddress("numpo", numpo, &b_numpo);
     //fChain->SetBranchAddress("apos", apos, &b_apos);
     //fChain->SetBranchAddress("adir", adir, &b_adir);
     //fChain->SetBranchAddress("aang", &aang, &b_aang);
     //fChain->SetBranchAddress("agood", &agood, &b_agood);
     //fChain->SetBranchAddress("wgain", &wgain, &b_wgain);
     //fChain->SetBranchAddress("nbad", &nbad, &b_nbad);
     //fChain->SetBranchAddress("nbada", &nbada, &b_nbada);
     //fChain->SetBranchAddress("msdir", msdir, &b_msdir);
     //fChain->SetBranchAddress("amomp", amomp, &b_amomp);
     //fChain->SetBranchAddress("ange", ange, &b_ange);
     //fChain->SetBranchAddress("angm", angm, &b_angm);
     //fChain->SetBranchAddress("angp", angp, &b_angp);
     //fChain->SetBranchAddress("ntot", ntot, &b_ntot);
     //fChain->SetBranchAddress("probth", probth, &b_probth);
     //fChain->SetBranchAddress("probpt", probpt, &b_probpt);
     fChain->SetBranchAddress("pi0like", pi0like, &b_pi0like);
     fChain->SetBranchAddress("pi0_e", pi0_e, &b_pi0_e);
     fChain->SetBranchAddress("pi0_dir", pi0_dir, &b_pi0_dir);
     fChain->SetBranchAddress("pi0mass", pi0mass, &b_pi0mass);
     //fChain->SetBranchAddress("evisold", &evisold, &b_evisold);
     //fChain->SetBranchAddress("evisoldxe", &evisoldxe, &b_evisoldxe);
     //fChain->SetBranchAddress("evisnew", &evisnew, &b_evisnew);
     fChain->SetBranchAddress("nmue", &nmue, &b_nmue);
     //fChain->SetBranchAddress("etype", etype, &b_etype);
     //fChain->SetBranchAddress("etime", etime, &b_etime);
     //fChain->SetBranchAddress("epos", epos, &b_epos);
     //fChain->SetBranchAddress("edir", edir, &b_edir);
     //fChain->SetBranchAddress("egood", egood, &b_egood);
     //fChain->SetBranchAddress("ehit", ehit, &b_ehit);
     //fChain->SetBranchAddress("mueprob", mueprob, &b_mueprob);
     //fChain->SetBranchAddress("Rnring", &Rnring, &b_Rnring);
     //fChain->SetBranchAddress("Rdir", Rdir, &b_Rdir);
     //fChain->SetBranchAddress("Rang", Rang, &b_Rang);
     //fChain->SetBranchAddress("Riring", &Riring, &b_Riring);
     //fChain->SetBranchAddress("Rtwout", Rtwout, &b_Rtwout);
     //fChain->SetBranchAddress("Rtwith", Rtwith, &b_Rtwith);
     //fChain->SetBranchAddress("Alwout", &Alwout, &b_Alwout);
     //fChain->SetBranchAddress("Alwith", &Alwith, &b_Alwith);
     //fChain->SetBranchAddress("Qsmi", &Qsmi, &b_Qsmi);
     //fChain->SetBranchAddress("Qsmo", &Qsmo, &b_Qsmo);
     //fChain->SetBranchAddress("Qexi", &Qexi, &b_Qexi);
     //fChain->SetBranchAddress("Qexo", &Qexo, &b_Qexo);
     //fChain->SetBranchAddress("Pe5d", &Pe5d, &b_Pe5d);
     //fChain->SetBranchAddress("En5d", &En5d, &b_En5d);
     //fChain->SetBranchAddress("Eh5d", &Eh5d, &b_Eh5d);
     //fChain->SetBranchAddress("Pe5do", &Pe5do, &b_Pe5do);
     //fChain->SetBranchAddress("En5do", &En5do, &b_En5do);
     //fChain->SetBranchAddress("Eh5do", &Eh5do, &b_Eh5do);
     //fChain->SetBranchAddress("Rtadd", &Rtadd, &b_Rtadd);
     //fChain->SetBranchAddress("Pdgeta", &Pdgeta, &b_Pdgeta);
     //fChain->SetBranchAddress("Pd5d", &Pd5d, &b_Pd5d);
     //fChain->SetBranchAddress("Pdthre", &Pdthre, &b_Pdthre);
     //fChain->SetBranchAddress("Pd5do", &Pd5do, &b_Pd5do);
     //fChain->SetBranchAddress("Delpd", &Delpd, &b_Delpd);
     //fChain->SetBranchAddress("Ropena", Ropena, &b_Ropena);
     //fChain->SetBranchAddress("Maxth", &Maxth, &b_Maxth);
     //fChain->SetBranchAddress("Pkang", &Pkang, &b_Pkang);
     //fChain->SetBranchAddress("Qrfct", &Qrfct, &b_Qrfct);
     //fChain->SetBranchAddress("Pdfct", &Pdfct, &b_Pdfct);
     //fChain->SetBranchAddress("Pkfct", &Pkfct, &b_Pkfct);
     //fChain->SetBranchAddress("Agfct", &Agfct, &b_Agfct);
     //fChain->SetBranchAddress("Dlfct", &Dlfct, &b_Dlfct);
     //fChain->SetBranchAddress("Iflag", &Iflag, &b_Iflag);
     //fChain->SetBranchAddress("Pmfct", &Pmfct, &b_Pmfct);
     //fChain->SetBranchAddress("Imfct", &Imfct, &b_Imfct);
     //fChain->SetBranchAddress("Rilike", &Rilike, &b_Rilike);
     //fChain->SetBranchAddress("ri_ver", &ri_ver, &b_ri_ver);
     //fChain->SetBranchAddress("ri_pid", &ri_pid, &b_ri_pid);
     //fChain->SetBranchAddress("ri_nring", &ri_nring, &b_ri_nring);
     //fChain->SetBranchAddress("ri_flag", ri_flag, &b_ri_flag);
     //fChain->SetBranchAddress("ri_dlfct", ri_dlfct, &b_ri_dlfct);
     //fChain->SetBranchAddress("ri_pdfct", ri_pdfct, &b_ri_pdfct);
     //fChain->SetBranchAddress("ri_pkfct", ri_pkfct, &b_ri_pkfct);
     //fChain->SetBranchAddress("ri_vafct", ri_vafct, &b_ri_vafct);
     //fChain->SetBranchAddress("ri_total", ri_total, &b_ri_total);
     //fChain->SetBranchAddress("ri_dir", ri_dir, &b_ri_dir);
     //fChain->SetBranchAddress("ri_imfct", ri_imfct, &b_ri_imfct);
     //fChain->SetBranchAddress("ri_pmfct", ri_pmfct, &b_ri_pmfct);
     fChain->SetBranchAddress("npar", &npar, &b_npar);
     fChain->SetBranchAddress("wallv", &wallv, &b_wallv);
     fChain->SetBranchAddress("ipv", ipv, &b_ipv);
     fChain->SetBranchAddress("posv", posv, &b_posv);
     fChain->SetBranchAddress("dirv", dirv, &b_dirv);
     fChain->SetBranchAddress("pmomv", pmomv, &b_pmomv);
     //fChain->SetBranchAddress("light_flag", light_flag, &b_light_flag);
     //fChain->SetBranchAddress("npar2", &npar2, &b_npar2);
     //fChain->SetBranchAddress("wallv2", wallv2, &b_wallv2);
     //fChain->SetBranchAddress("ipv2", ipv2, &b_ipv2);
     //fChain->SetBranchAddress("iorg", iorg, &b_iorg);
     //fChain->SetBranchAddress("posv2", posv2, &b_posv2);
     //fChain->SetBranchAddress("dirv2", dirv2, &b_dirv2);
     //fChain->SetBranchAddress("pmomv2", pmomv2, &b_pmomv2);
     fChain->SetBranchAddress("numnu", &numnu, &b_numnu);
     fChain->SetBranchAddress("mode", &mode, &b_mode);
     fChain->SetBranchAddress("ipnu", ipnu, &b_ipnu);
     fChain->SetBranchAddress("pnu", pnu, &b_pnu);
     fChain->SetBranchAddress("dirnu", dirnu, &b_dirnu);
     //fChain->SetBranchAddress("scan", scan, &b_scan);
     //fChain->SetBranchAddress("nuceff_version", &nuceff_version, &b_nuceff_version);
     //fChain->SetBranchAddress("charge_exchange", &charge_exchange, &b_charge_exchange);
     //fChain->SetBranchAddress("absorbed", &absorbed, &b_absorbed);
     //fChain->SetBranchAddress("multipi_gen", &multipi_gen, &b_multipi_gen);
     //fChain->SetBranchAddress("scattering", &scattering, &b_scattering);
     //fChain->SetBranchAddress("piless_dcy", &piless_dcy, &b_piless_dcy);
     fChain->SetBranchAddress("nscndprt", &nscndprt, &b_nscndprt);
     fChain->SetBranchAddress("itrkscnd", itrkscnd, &b_itrkscnd);
     fChain->SetBranchAddress("vtxscnd", vtxscnd, &b_vtxscnd);
     fChain->SetBranchAddress("pscnd", pscnd, &b_pscnd);
     fChain->SetBranchAddress("iprtscnd", iprtscnd, &b_iprtscnd);
     fChain->SetBranchAddress("tscnd", tscnd, &b_tscnd);
     fChain->SetBranchAddress("iprntprt", iprntprt, &b_iprntprt);
     fChain->SetBranchAddress("lmecscnd", lmecscnd, &b_lmecscnd);
     fChain->SetBranchAddress("iprnttrk", iprnttrk, &b_iprnttrk);
     fChain->SetBranchAddress("iorgprt", iorgprt, &b_iorgprt);
     fChain->SetBranchAddress("iflgscnd", iflgscnd, &b_iflgscnd);
     fChain->SetBranchAddress("pprnt", pprnt, &b_pprnt);
     //fChain->SetBranchAddress("Neutmode", &Neutmode, &b_Neutmode);
     //fChain->SetBranchAddress("Npne", &Npne, &b_Npne);
     //fChain->SetBranchAddress("Ipne", Ipne, &b_Ipne);
     //fChain->SetBranchAddress("Abspne", Abspne, &b_Abspne);
     //fChain->SetBranchAddress("Pvecne", Pvecne, &b_Pvecne);

     fChain->SetBranchAddress("Crsx", &Crsx, &b_Crsx);
     fChain->SetBranchAddress("Crsy", &Crsy, &b_Crsy);
     fChain->SetBranchAddress("Crsz", &Crsz, &b_Crsz);
     fChain->SetBranchAddress("Crsphi", &Crsphi, &b_Crsphi);
   }
   fChain->SetBranchAddress("Npvc", &Npvc, &b_Npvc);
   fChain->SetBranchAddress("Ipvc", Ipvc, &b_Ipvc);
   fChain->SetBranchAddress("Ichvc", Ichvc, &b_Ichvc);
   fChain->SetBranchAddress("Iorgvc", Iorgvc, &b_Iorgvc);
   fChain->SetBranchAddress("Iflvc", Iflvc, &b_Iflvc);
   fChain->SetBranchAddress("Abspvc", Abspvc, &b_Abspvc);
   fChain->SetBranchAddress("Pvc", Pvc, &b_Pvc);
   fChain->SetBranchAddress("Nvert", &Nvert, &b_Nvert);
   fChain->SetBranchAddress("Posvert", Posvert, &b_Posvert);
   fChain->SetBranchAddress("Iflgvert", Iflgvert, &b_Iflgvert);
   fChain->SetBranchAddress("Nvcvert", &Nvcvert, &b_Nvcvert);
   fChain->SetBranchAddress("Dirvert", Dirvert, &b_Dirvert);
   fChain->SetBranchAddress("Abspvert", Abspvert, &b_Abspvert);
   fChain->SetBranchAddress("Abstpvert", Abstpvert, &b_Abstpvert);
   fChain->SetBranchAddress("Ipvert", Ipvert, &b_Ipvert);
   fChain->SetBranchAddress("Iverti", Iverti, &b_Iverti);
   fChain->SetBranchAddress("Ivertf", Ivertf, &b_Ivertf);
   fChain->SetBranchAddress("Fsiprob", &Fsiprob, &b_Fsiprob);

   //fChain->SetBranchAddress("t2kposc", t2kposc, &b_t2kposc);

   fChain->SetBranchAddress("Numbndn", &Numbndn, &b_Numbndn);
   fChain->SetBranchAddress("Numbndp", &Numbndp, &b_Numbndp);
   fChain->SetBranchAddress("Numfrep", &Numfrep, &b_Numfrep);
   fChain->SetBranchAddress("Numatom", &Numatom, &b_Numatom);
   fChain->SetBranchAddress("Ibound", &Ibound, &b_Ibound);

   // Branches for NEUT cascade variables within SKDETSIM hadronic interactions.
   // Uncomment when using specially generated SK files with the NEUT option turned on
//   fChain->SetBranchAddress("Nvertsk", &Nvertsk, &b_Nvertsk);
//   fChain->SetBranchAddress("Posvertsk", Posvertsk, &b_Posvertsk);
//   fChain->SetBranchAddress("Iflgvertsk", Iflgvertsk, &b_Iflgvertsk);
//   fChain->SetBranchAddress("Nvcvertsk", &Nvcvertsk, &b_Nvcvertsk);
//   fChain->SetBranchAddress("Dirvertsk", Dirvertsk, &b_Dirvertsk);
//   fChain->SetBranchAddress("Abspvertsk", Abspvertsk, &b_Abspvertsk);
//   fChain->SetBranchAddress("Abstpvertsk", Abstpvertsk, &b_Abstpvertsk);
//   fChain->SetBranchAddress("Ipvertsk", Ipvertsk, &b_Ipvertsk);
//   fChain->SetBranchAddress("Ivertisk", Ivertisk, &b_Ivertisk);
//   fChain->SetBranchAddress("Ivertfsk", Ivertfsk, &b_Ivertfsk);
//   fChain->SetBranchAddress("Fsiprobsk", &Fsiprobsk, &b_Fsiprobsk);

   Notify();
}

Bool_t SK__h1::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}
