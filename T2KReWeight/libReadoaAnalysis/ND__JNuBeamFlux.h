//////////////////////////////////////////////////////////
//   This class has been generated by TFile::MakeProject
//     (Tue Jun 27 21:29:22 2017 by ROOT version 5.34/09)
//      from the StreamerInfo in file /physics/data/MC_Productions/GRID/mcp6_Spin_B/neut/run4-water/anal/oa_nt_beam_90410000-0023_wjojmfzpy6p4_anal_001_prod6amagnet201011waterc-bsdv01_2.root
//////////////////////////////////////////////////////////


#ifndef ND__JNuBeamFlux_h
#define ND__JNuBeamFlux_h
namespace ND {
class JNuBeamFlux;
} // end of namespace.

#include "ND__RooTrackerVtxBase.h"
#include "TObjString.h"

namespace ND {
class JNuBeamFlux : public ND::RooTrackerVtxBase {

public:
// Nested classes declaration.

public:
// Data Members.
   long        NuFluxEntry;    ///< corresponding entry in orig flux file
   TObjString* NuFileName;     ///< orig flux file name
   int         NuParentPdg;    ///< parent hadron pdg code
   int         NuParentDecMode;    ///< parent hadron decay mode
   double      NuParentDecP4[4];    ///< parent hadron 4-momentum at decay
   double      NuParentDecX4[4];    ///< parent hadron 4-position at decay 
   float       NuCospibm;           ///< parent hadron cos(theta) relative to beam at decay
   float       NuNorm;              ///< Normalization weight 
   double      NuParentProP4[4];    ///< parent hadron 4-momentum at production 
   double      NuParentProX4[4];    ///< parent hadron 4-position at production  
   float       NuCospi0bm;          ///< parent hadron cos(theta) relative to beam at production
   float       NuRnu;               ///< neutrino r position at ND5/6 plane 
   float       NuXnu[2];            ///< neutrino (x,y) position at ND5/6 plane 
   int         NuIdfd;              ///< Detector ID
   int         NuGipart;            ///< primary particle ID  
   float       NuGpos0[3];          ///< primary particle starting point 
   float       NuGvec0[3];          ///< primary particle direction at starting point 
   float       NuGamom0;            ///< momentum of the primary particle at the starting point 
   int         NuNg;                ///< Number of interaction steps 
   float       NuGp[12][3];         ///< Momentum of ancestor particle 
   float       NuGcosbm[12];        ///< Ancestor particle cos(theta) relative to beam
   float       NuGv[12][3];         ///< Position of ancestor particle 
   int         NuGpid[12];          ///< PDG ancestor particle ID 
   int         NuGmec[12];          ///< particle production mechanism of ancestor particle
   float       NuEnusk;             ///< Neutrino Energy at SK 
   float       NuNormsk;            ///< Normalization weight at SK 
   float       NuAnorm;             ///< ND Acceptance Weight 
   int         NuGmat[12];          ///< material in which the particle originates
   float       NuGdistc[12];        ///< distance traveled through carbon
   float       NuGdistal[12];       ///< distance traveled through aluminum
   float       NuGdistti[12];       ///< distance traveled through titanium
   float       NuGdistfe[12];       ///< distance traveled through iron
   float       NuVersion;           ///< jnubeam flux version 
   int         NuTuneid;            ///< beam tune ID # 
   int         NuNtrig;             ///< Number of triggers
   int         NuPint;              ///< Interaction model ID 
   float       NuBpos[2];           ///< Beam center position 
   float       NuBtilt[2];          ///< Beam angle  
   float       NuBrms[2];           ///< Beam RMS width 
   float       NuEmit[2];           ///< Beam Emittance 
   float       NuAlpha[2];          ///< Beam Alpha 
   float       NuHcur[3];           ///< Horn currents 
   int         NuRand;              ///< Random seed

   JNuBeamFlux();
   JNuBeamFlux(const JNuBeamFlux & );
   virtual ~JNuBeamFlux();

   ClassDef(JNuBeamFlux,2); // Generated by MakeProject.
};
} // namespace
#endif
