#ifndef MOVESTATS_H
#define MOVESTATS_H
#include <vector>
#include <Rtypes.h>

class TH1;
//class TH1F;
//class TH1D;
class TProfile;
class TH2;
class TGraph;
class TMultiGraph;
class TTree;

//#pragma link C++ class vector<TH1*>;


///////////////////////////////////////////////////////////////////////////////
// functions in this file
///////////////////////////////////////////////////////////////////////////////

// set line style for all hists and graphs in memory in gDirectory
void set_ls(Int_t v, Int_t lw=-1);
// set line style for all hists and graphs in gDirectory (opens files on disk)
void set_ls_file(Int_t v, Int_t lw=-1);
// set line color for all hists and graphs in memory in gDirectory
void set_lc(Int_t v);
// set line color for all hists and graphs in gDirectory (opens files on disk)
void set_lc_file(Int_t v);
// set fill style for all hists and graphs in memory in gDirectory
void set_fs(Int_t v);
// set fill style for all hists and graphs in gDirectory (opens files on disk)
void set_fs_file(Int_t v);
// set fill color for all hists and graphs in memory in gDirectory
void set_fc(Int_t v);
// set fill color for all hists and graphs in gDirectory (opens files on disk)
void set_fc_file(Int_t v);

// set marker style for all hists and graphs in memory in gDirectory
void set_ms(Int_t v);
// set marker style for all hists and graphs in gDirectory(opens files on disk)
void set_ms_file(Int_t v);

// set marker color for all hists and graphs in memory in gDirectory
void set_mc(Int_t v);
// set marker color for all hists and graphs in gDirectory(opens files on disk)
void set_mc_file(Int_t v);


// rearrange and color multiple stats boxes in gPad
// boxes are colored according to the histogram or graph line color
// opt = "v" arrange boxes vertically
//       "h" arrange boxes horizontally
// x,y = NDC coordinates of block of stats boxes
//       allows you to move them around left and right, up and down
//       if x,y=-1  use gStyle->GetStatsX,Y() for x and y
void move_stats(const char* opt="v", float x=-1, float y=-1);

// move stats on all pads of all canvases
void move_stats_all_cans(const char* opt="v", float x=-1, float y=-1);

// fix figure maximum on gPad so all histograms fit vertically
void fix_maximum();

// fix maximum on all pads of all canvases
void fix_maximum_all_cans();

// call print on all open canvases
void write_all_cans(const char* prefix="", const char* type="eps");

// center axis titles
void center_titles(const char* opt="XY");
void center_titles_all_cans(const char* opt="XY");

// find histograms, stick them in a vector
int get_histos(std::vector<TH1*>& v, const char* names);

// get things from a file, close the file (get_t is an exception)
// histograms are normally owned by the file but this function
// removes that ownership (so the file may be closed)
// get_t gets a tree but can't close the file
TH1* get_h1(const char* hname, const char* fname);
TH2* get_h2(const char* hname, const char* fname);
TGraph* get_g(const char* hname, const char* fname);
TTree* get_t(const char* hname, const char* fname);

//  Histogram with logarithmic x-axis scale
// sets bin sizes such that when gPad->Logx,y() is called
// the bin widths look uniform.
// This is similar to filling a histogram with log10(quantity)
// but the axis labels will look like 10^N rather than N
TH1* log10_h1(const char* hname, const char* tit, 
	    int nbins, float low, float high);
TProfile* log10_prof(const char* hname, const char* tit, 
	    int nbins, float low, float high);

// natural logs, unfortunately the bin sizes will
// not look uniform when gPad->Logx,y() is called since
// that's a log10 scale. You can't do a Ln scale in root. 
// They ought to add it.
TH1* log_h1(const char* hname, const char* tit, 
	    int nbins, float low, float high);

TProfile* log_prof(const char* hname, const char* tit, 
	    int nbins, float low, float high);

// couldn't make this templated version work with root 4.02.00
// due to a bug in CINT (found by Brett actually)
//template<class T> T* log_h1(const char* hname, const char* tit, 
//			    int nbins, float low, float high);


// histogram operations
// vanilla root doesn't include methods two divide two histograms
// without getting a pointer to one of them... this is annoying
// if "new" is included in the option then a new histogram will be returned
// otherwise the histogram referenced by n1 will modified and returned
//
// divide two histograms, opt is passed to the TH1::Divide() call
//TH1* dhist(const char* n1, const char* n2, const char* opt=0);
//TH1* dhist(const char* n1, const char* n2, const char* opt, 
//	   Double_t c1=1, Double_t c2=1,);
// multiply two histograms, opt is passed to the TH1::Multiply() call
//TH1* mhist(const char* n1, const char* n2, const char* opt);
// add two histograms
///////////////////////////////////////////////////////////////////////////////

///////////////// integrate a histogram to the right /////////////////////////
TH1* sum_right(TH1* h);

//////////////  set axes ranges //////////////////////////////////
void set_axis_range(TH1* h, float nsig, float quant);
void same_axis_range(TH1* h1, TH1* h2);
void set_axis_range2(TH1* h, float nsigup, float nsigdn, float quant);

///////////// fitting utilities //////////////////////////////////

/// The following functions use gMinuit and assume you've just done a fit

// perform a chi2 scan of the recently fit function in par[par_num]
// scan over the range [ (par-epar*nsig_down) , (par+epar*nsig_up) ]
// in ndiv steps, both up and down from the best fit point
// the graph shows delta chi2 vs. the parameter value
//TGraph* chisquare_scan(Int_t par_num, Int_t nsig_up=3, Int_t nsig_down=3, Int_t ndiv=10);
// do the scan and also draw (convenience function)
TGraph* draw_chisquare_scan(Int_t par_num, Int_t nsig_up=3, Int_t nsig_down=3, Int_t ndiv=10);
// get ncont  two-dimensional fit contours,
// starting at sig_start  (in std. deviations), stepping by sig_step
/// p1,p2 are function parameter numbers --> map to x,y on graphs
TMultiGraph* get_fit_contours(Int_t p1, Int_t p2, Int_t ncont=3, Int_t sig_start=1, Int_t sig_step=1, Int_t npts=30);
// get the contours and draw them (convenience function)
TMultiGraph* draw_fit_contours(Int_t p1, Int_t p2, Int_t ncont=3, Int_t sig_start=1, Int_t sig_step=1, Int_t npts=30);

// these functions are used by the ones above
// they should work fine when called alone but the author
// hasn't used them that way
TGraph* best_fit_point(Int_t p1, Int_t p2);
//TGraph* two_dim_contour(Int_t p1, Int_t p2, Double_t sig=1.0, Int_t npts=30);

/*
///// templated stuff
// couldn't make this work with 4.02.00... see above
template<class T> T* log_h1(const char* hname, const char* tit, int nbins, float low, float high){
  // xbins construction taken from rootalk : P.Sizun
  
  Double_t logxmin = TMath::Log(xmin);
  Double_t logxmax = TMath::Log(xmax);
  Double_t binwidth = (logxmax-logxmin)/nbins;
  Double_t xbins[nbins+1];
  xbins[0] = xmin;
  for (Int_t i=1;i<=nbins;i++) {
    //    xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidth);
    xbins[i] = xmin + exp(logxmin+i*binwidth);
  }
    
  T* h = new T(hname, tit, nbins,xbins);  
  return h;
}
#pragma link C++ function log_h1<TProfile>(const char* , const char*, int, float, float);
#pragma link C++ function log_h1<TH1F>(const char* , const char*, int, float, float);
#pragma link C++ function log_h1<TH1D>(const char* , const char*, int, float, float);
*/


#endif
