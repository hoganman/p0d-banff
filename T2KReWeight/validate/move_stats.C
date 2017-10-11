#include "move_stats.h"

#include <iostream>
#include <sstream>
#include <string>

#include "TObject.h"
#include "TROOT.h"
#include "TString.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TIterator.h"
#include "TList.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TTree.h"
#include "TPaveStats.h"
#include "TDirectory.h"
#include "TAttLine.h"
#include "TAttFill.h"
#include "TAttMarker.h"
#include "TKey.h"
#include "TGraph.h"
#include "TMultiGraph.h"
#include "TMath.h"
#include "TProfile.h"
#include "TMinuit.h"
#include "TStyle.h"

using namespace std;

void write_all_cans(const char* prefix, const char* type){
   if(!gPad) return;
//   TVirtualPad* save = gPad;
   
   // loop on all canvases
   TIter canit(gROOT->GetListOfCanvases());
   TObject* obj;
   while((obj=canit.Next())){
      if(obj->InheritsFrom("TCanvas") ){
	 TCanvas* can = static_cast<TCanvas*>(obj);
	 can->cd();
	 TString name=prefix; name+=can->GetName();
	 name+="."; name+=type;
	 can->Print(name.Data(), type);
      }
   }
}

void fix_maximum_all_cans(){
   if(!gPad) return;
   TVirtualPad* save = gPad;
   
   // loop on all canvases
   TIter canit(gROOT->GetListOfCanvases());
   TObject* obj;
   while((obj=canit.Next())){
      if(obj->InheritsFrom("TCanvas") ){
	 TCanvas* can = static_cast<TCanvas*>(obj);
	 can->cd();
	 TIter it(can->GetListOfPrimitives());
	 while((obj=it.Next())){
	    if(obj->InheritsFrom("TPad") ){
	       TPad* pad = static_cast<TPad*>( obj);
	       pad->cd();
	       fix_maximum();
	    }
	 }
      }
   }
   save->cd();
}

void move_stats_all_cans(const char* opt, float x, float y){
   if(!gPad) return;
   TVirtualPad* save = gPad;
   
   // loop on all canvases
   TIter canit(gROOT->GetListOfCanvases());
   TObject* obj;
   while((obj=canit.Next())){
      if(obj->InheritsFrom("TCanvas") ){
	 TCanvas* can = static_cast<TCanvas*>(obj);
	 can->cd();
	 TIter it(can->GetListOfPrimitives());
	 while((obj=it.Next())){
	    if(obj->InheritsFrom("TPad") ){
	       TPad* pad = static_cast<TPad*>( obj);
	       pad->cd();
	       move_stats(opt,x,y);
	    }
	 }
      }
   }
   save->cd();
}
void fix_maximum(){

    // sets the maximum in the current pad so that all histograms are on scale
    gPad->Modified();
    gPad->Update();

    int nps=0;
    float max=0.0;
    // iterate over primitives looking for histograms
    TH1* hvec[50];
    TIter it(gPad->GetListOfPrimitives());
    TObject* obj;
    while((obj=it.Next())) {
        if(obj->InheritsFrom("TH1") ) {
            //	     && !obj->InheritsFrom("TH2") 
            //	     && !obj->InheritsFrom("TH3"))
            // is a 1d histogram
            // try to get the stats box
            TH1* h= static_cast<TH1*>(obj);
            hvec[nps]=h;

            if(h->GetMaximum()>max) max=h->GetMaximum();

            nps++;
        }
        else if(obj->InheritsFrom("TPad")){
            // make it recursive
            TVirtualPad* spad = gPad;
            TPad* npad = static_cast<TPad*>(obj);
            npad->cd();
            fix_maximum();
            spad->cd();
        }
    }
    // include an additional bit of whitespace at the top of the figure
    if(gPad->GetLogy()) max*=2.3;
    else max*=1.2;
    for(int i=0; i<nps; i++){
        TH1* h = hvec[i];
        h->SetMaximum(max);
    }
    gPad->Modified();
    gPad->Update();
}

//void move_stats(){
void move_stats(const char* opt, float x, float y){

    // Moves and aligns the stats boxes for histograms in the current pad
    // 
    // author: mike kordosky
    // kordosky@fnal.gov
    //
 
     gPad->Modified();
     gPad->Update();

     std::string opts = opt;
     
     TPaveStats* ps_array[50]={0}; // Limitation: No more than 50 histograms in pad
     int nps=0;
     // iterate over primitives looking for histograms
     TIter it(gPad->GetListOfPrimitives());
     TObject* obj;
     int first=1;
     int blow=0;
     int bhigh=1;
     while((obj=it.Next())){
	  if(obj->InheritsFrom("TH1") ){
	       // is a 1d histogram
	       // try to get the stats box
	       TH1* h= static_cast<TH1*>(obj);


	       // this sordid busines resets the axis limits based on
	       // the first histogram it finds in the pad
	       // the reason is that this will make the stats box display
	       // consistent
	       if(first){
		  blow=h->GetXaxis()->GetFirst();
		  bhigh=h->GetXaxis()->GetLast();
		  first=0;
	       }
	       else{
		  h->GetXaxis()->SetRange(blow,bhigh);
	       }

	       TObject* ps_obj = h->FindObject("stats");
	       if(ps_obj&&ps_obj->InheritsFrom("TPaveStats")){
		    TPaveStats* ps = static_cast<TPaveStats*>(ps_obj);
		    // rename stats box
		    TString s = h->GetName();
		    s+="_stats";

		    ps->SetName(s.Data());
		    ps->SetTextColor(h->GetLineColor());
		    // store in array
		    ps_array[nps]=ps;
		    nps++;
	       }	       
	  }
	  else if(obj->InheritsFrom("TGraph") ){
	       // is a 1d histogram
	       // try to get the stats box
	       TGraph* gr= static_cast<TGraph*>(obj);


	       // this sordid busines resets the axis limits based on
	       // the first histogram it finds in the pad
	       // the reason is that this will make the stats box display
	       // consistent
	       if(first){
		  blow=gr->GetXaxis()->GetFirst();
		  bhigh=gr->GetXaxis()->GetLast();
		  first=0;
	       }
	       else{
		  gr->GetXaxis()->SetRange(blow,bhigh);
	       }

	       TObject* ps_obj = gr->FindObject("stats");
	       if(ps_obj&&ps_obj->InheritsFrom("TPaveStats")){
		    TPaveStats* ps = static_cast<TPaveStats*>(ps_obj);
		    // rename stats box
		    TString s = gr->GetName();
		    s+="_stats";

		    ps->SetName(s.Data());
		    ps->SetTextColor(gr->GetLineColor());
		    // store in array
		    ps_array[nps]=ps;
		    nps++;
	       }	       
	  }
	  else if(obj->InheritsFrom("TPad") ){
	     // make it recursive
	     TVirtualPad* spad = gPad;
	     TPad* npad = static_cast<TPad*>(obj);
	     npad->cd();
	     move_stats(opt, x, y);
	     spad->cd();
	  }
     }
     if(nps<=0) return;
     // configuration parameters
     //     double tot_h = 0.6;
     double tot_h = gPad->GetTopMargin()+gPad->GetBottomMargin() 
       + gStyle->GetLabelSize("X")+gStyle->GetLabelOffset("X")
	 // this line from TGAxis::PaintAxis(): title offset is a scale factor
       + gStyle->GetTitleOffset("X")*1.3*gStyle->GetTitleSize("X");
     tot_h=1-tot_h;
     double stats_h = tot_h/nps;
     double start_h=gStyle->GetStatX()-gStyle->GetStatW();
     double start_v=gStyle->GetStatY();
     bool vertical=true;
     if((opts.find("h")!=std::string::npos)
	|| (opts.find("H")!=std::string::npos)){
       vertical=false;
       start_h=gStyle->GetStatX();
       start_v=gStyle->GetStatY()-gStyle->GetStatH();
       tot_h = gPad->GetLeftMargin()+gPad->GetRightMargin()
	 + gStyle->GetLabelSize("Y") + gStyle->GetLabelOffset("Y")
	 // this line from TGAxis::PaintAxis(): title offset is a scale factor
	 + gStyle->GetTitleOffset("Y")*1.6*gStyle->GetTitleSize("Y");
       tot_h=1-tot_h;
     }
     
     // a fudge factor accounting for space taken up by axis

     if(y>=0){
       start_v=y;
     }
     if(x>=0){
       start_h=x;
     }

     double stats_v = stats_h*0.6;
     for(int i=0; i<nps; i++){
	  TPaveStats* p = ps_array[i];
//	  p->Print();

//	  p->SetY2NDC(start_h-i*(stats_h+0.01));
//	  p->SetY1NDC(p->GetY2NDC()-stats_h);
	  if(vertical){
	    p->SetY2NDC(start_v-i*(stats_v+0.01));
	    p->SetY1NDC(p->GetY2NDC()-stats_v);
	    p->SetX2NDC(start_h+gStyle->GetStatW());
	    p->SetX1NDC(start_h);
	  }
	  else {
	    p->SetX2NDC(start_h-i*(stats_h+0.01));
	    p->SetX1NDC(p->GetX2NDC()-stats_h);
	    p->SetY2NDC(start_v+gStyle->GetStatH());
	    p->SetY1NDC(start_v);
	  }
     }

     gPad->Modified();
     gPad->Update();
}

void set_ls(Int_t v, Int_t lw){
   TIter it(gDirectory->GetList()); // list of objects in memory
   TObject* obj=0;
   while((obj=it.Next())){
      if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	 TAttLine* al = dynamic_cast<TAttLine*>(obj);
	 al->SetLineStyle(v);
	 if(lw>=0) al->SetLineWidth(lw);
      }
   }
}

void set_ls_file(Int_t v, Int_t lw){
   TIter it(gDirectory->GetListOfKeys());
   TObject* obj=0;
   TKey* key=0;
   while((key=static_cast<TKey*>(it.Next()))){
      obj=key->ReadObj();
      if(!obj) continue;
      if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	 TAttLine* al = dynamic_cast<TAttLine*>(obj);
	 al->SetLineStyle(v);
	 if(lw>=0) al->SetLineWidth(lw);	   
      }
   }
}

void set_lc(Int_t v){
   TIter it(gDirectory->GetList()); // list of objects in memory
   TObject* obj=0;
   while((obj=it.Next())){
      if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	 TAttLine* al = dynamic_cast<TAttLine*>(obj);
	 al->SetLineColor(v);
	}
   }
}

void set_lc_file(Int_t v){
     TIter it(gDirectory->GetListOfKeys());
     TObject* obj=0;
     TKey* key=0;
     while((key=static_cast<TKey*>(it.Next()))){
	obj=key->ReadObj();
	if(!obj) continue;
	if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	   TAttLine* al = dynamic_cast<TAttLine*>(obj);
	   al->SetLineColor(v);
	}
     }
}

void center_titles(const char* opt){
     // iterate over primitives looking for histograms
     TIter it(gPad->GetListOfPrimitives());
     TObject* obj;
     TString sopt(opt);
     sopt.ToLower();

     while((obj=it.Next())){

	if(obj->InheritsFrom("TH1") ){
	   // is a 1d histogram
	   // try to get the stats box
	   TH1* h= static_cast<TH1*>(obj);
	   if(sopt.Contains("x")){
	      h->GetXaxis()->CenterTitle();
	   }
	   if(sopt.Contains("y")){
	      h->GetYaxis()->CenterTitle();
	   }
	}
	else if(obj->InheritsFrom("TGraph")){
	   // is a 1d histogram
	   // try to get the stats box
	   TGraph* g = static_cast<TGraph*>(obj);
	   TH1* h= g->GetHistogram();
	   if(sopt.Contains("x")){
	      h->GetXaxis()->CenterTitle();
	   }
	   if(sopt.Contains("y")){
	      h->GetYaxis()->CenterTitle();
	   }
	}
	else if(obj->InheritsFrom("TMultiGraph")){
	   // is a 1d histogram
	   // try to get the stats box
	   TMultiGraph* g = static_cast<TMultiGraph*>(obj);
	   TH1* h= g->GetHistogram();
	   if(sopt.Contains("x")){
	      h->GetXaxis()->CenterTitle();
	   }
	   if(sopt.Contains("y")){
	      h->GetYaxis()->CenterTitle();
	   }
	}
	else if(obj->InheritsFrom("TPad")){
	   // is a 1d histogram
	   // try to get the stats box
	   TPad* pp = static_cast<TPad*>(obj);
	   TVirtualPad* sp = gPad;
	   pp->cd();
	   center_titles();
	   sp->cd();
	}

     }
     gPad->Modified();
     gPad->Update();
}

void center_titles_all_cans(const char* opt){
   if(!gPad) return;
   TVirtualPad* save = gPad;
   
   // loop on all canvases
   TIter canit(gROOT->GetListOfCanvases());
   TObject* obj;
   while((obj=canit.Next())){

      if(obj->InheritsFrom("TCanvas") ){
	 TCanvas* can = static_cast<TCanvas*>(obj);
	 can->cd();
	 TIter it(can->GetListOfPrimitives());
	 while((obj=it.Next())){
	    if(obj->InheritsFrom("TPad") ){
	       TPad* pad = static_cast<TPad*>( obj);
	       pad->cd();
	       center_titles(opt);
	    }
	 }
      }

   }
   save->cd();
}

TH1* sum_right(TH1* h){
   if(!h) return 0;
   TString snew=h->GetName();
   snew+="_sumr";
   TH1* hnew;
   if(h->InheritsFrom("TProfile")){
      TProfile* hprof = dynamic_cast<TProfile*>(h);
      hnew = hprof->ProjectionX("_sumr");
   }
   else hnew = (TH1*) h->Clone(snew.Data());
   TAttLine* hal = dynamic_cast<TAttLine*>(h);
   hal->Copy(dynamic_cast<TAttLine&>(*hnew));
   TAttFill* haf = dynamic_cast<TAttFill*>(h);
   haf->Copy(dynamic_cast<TAttFill&>(*hnew));
   TAttMarker* ham = dynamic_cast<TAttMarker*>(h);
   ham->Copy(dynamic_cast<TAttMarker&>(*hnew));

   for(int i=2; i<=hnew->GetNbinsX(); i++){
      std::cout<<" bin["<<i-1<<"]="<<hnew->GetBinContent(i-1)
	       <<"  bin["<<i<<"]="<<hnew->GetBinContent(i)<<std::endl;
      double bc=hnew->GetBinContent(i-1) + hnew->GetBinContent(i);
      double be=sqrt(hnew->GetBinError(i-1)*hnew->GetBinError(i-1)
		     + hnew->GetBinError(i)*hnew->GetBinError(i));
      std::cout<<bc<<"  "<<be<<std::endl;
      hnew->SetBinContent(i,bc);
//      hnew->SetBinError(i,be);
   }
   return hnew;
}

void set_fs(Int_t v){
   TIter it(gDirectory->GetList()); // list of objects in memory
   TObject* obj=0;
   while((obj=it.Next())){
      if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	 TAttFill* al = dynamic_cast<TAttFill*>(obj);
	 al->SetFillStyle(v);
	}
   }
}

void set_fs_file(Int_t v){
     TIter it(gDirectory->GetListOfKeys());
     TObject* obj=0;
     TKey* key=0;
     while((key=static_cast<TKey*>(it.Next()))){
	obj=key->ReadObj();
	if(!obj) continue;
	if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	   TAttFill* al = dynamic_cast<TAttFill*>(obj);
	   al->SetFillStyle(v);
	}
     }
}


void set_fc(Int_t v){
   TIter it(gDirectory->GetList()); // list of objects in memory
   TObject* obj=0;
   while((obj=it.Next())){
      if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	 TAttFill* al = dynamic_cast<TAttFill*>(obj);
	 al->SetFillColor(v);
	}
   }
}

void set_fc_file(Int_t v){
     TIter it(gDirectory->GetListOfKeys());
     TObject* obj=0;
     TKey* key=0;
     while((key=static_cast<TKey*>(it.Next()))){
	obj=key->ReadObj();
	if(!obj) continue;
	if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	   TAttFill* al = dynamic_cast<TAttFill*>(obj);
	   al->SetFillColor(v);
	}
     }
}


void set_ms(Int_t v){
   TIter it(gDirectory->GetList()); // list of objects in memory
   TObject* obj=0;
   while((obj=it.Next())){
      if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	 TAttMarker* al = dynamic_cast<TAttMarker*>(obj);
	 al->SetMarkerStyle(v);
	}
   }
}

void set_ms_file(Int_t v){
     TIter it(gDirectory->GetListOfKeys());
     TObject* obj=0;
     TKey* key=0;
     while((key=static_cast<TKey*>(it.Next()))){
	obj=key->ReadObj();
	if(!obj) continue;
	if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	   TAttMarker* al = dynamic_cast<TAttMarker*>(obj);
	   al->SetMarkerStyle(v);
	}
     }
}


void set_mc(Int_t v){
   TIter it(gDirectory->GetList()); // list of objects in memory
   TObject* obj=0;
   while((obj=it.Next())){
      if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	 TAttMarker* al = dynamic_cast<TAttMarker*>(obj);
	 al->SetMarkerColor(v);
	}
   }
}

void set_mc_file(Int_t v){
     TIter it(gDirectory->GetListOfKeys());
     TObject* obj=0;
     TKey* key=0;
     while((key=static_cast<TKey*>(it.Next()))){
	obj=key->ReadObj();
	if(!obj) continue;
	if((obj->InheritsFrom("TH1")) || (obj->InheritsFrom("TGraph")) ){
	   TAttMarker* al = dynamic_cast<TAttMarker*>(obj);
	   al->SetMarkerColor(v);
	}
     }
}


int get_histos(std::vector<TH1*>& v, const char* names){
   std::istringstream iss(names);
   int cntr=0;
   while(!iss.eof()){
      std::string hn;
      iss>>hn;
      std::cout<<hn<<std::endl;
      TH1* h = dynamic_cast<TH1*>(gDirectory->Get(hn.c_str()));
      if(h){
	 v.push_back(h);
	 cntr++;
      }
      else{
	 std::cout<<"Could not locate "<<hn<<" or it's not a TH1."<<std::endl;
      }
   }

   return cntr;
}

void set_axis_range(TH1* h, float nsig, float quant){
     float mean=h->GetMean();
     float rms=h->GetRMS();
     float upper=mean+rms*nsig;
     upper= upper - fmod(upper,quant) + quant;
     h->GetXaxis()->SetRangeUser(0,upper);
}

void set_axis_range2(TH1* h, float nsigup, float nsigdn, float quant){
     float mean=h->GetMean();
     float rms=h->GetRMS();
     float upper=mean+rms*nsigup;
     float lower=mean-rms*nsigdn;
     upper= upper - fmod(upper,quant) + quant;
     lower= lower - fmod(lower,quant) + quant;

     h->GetXaxis()->SetRangeUser(lower,upper);
}

void same_axis_range(TH1* h1, TH1* h2){
     h2->GetXaxis()->SetRange(h1->GetXaxis()->GetFirst(), h1->GetXaxis()->GetLast());
}

TH1* get_h1(const char* hname, const char* fname){
   TFile f(fname);
   if(!f.IsOpen()) return 0;
   TH1* h = dynamic_cast<TH1*>(f.Get(hname));
   if(!h) return 0;
   else h->SetDirectory(0);
   return h;
}

TH2* get_h2(const char* hname, const char* fname){
   TFile f(fname);
   if(!f.IsOpen()) return 0;
   TH2* h = dynamic_cast<TH2*>(f.Get(hname));
   if(!h) return 0;
   else h->SetDirectory(0);
   return h;
}

TGraph* get_g(const char* hname, const char* fname){
   TFile f(fname);
   if(!f.IsOpen()) return 0;
   TGraph* h = dynamic_cast<TGraph*>(f.Get(hname));
   if(!h) return 0;
   return h;
}

TTree* get_t(const char* hname, const char* fname){
   TFile* f = new TFile(fname);
   if(!f->IsOpen()) return 0;
   TTree* h = dynamic_cast<TTree*>(f->Get(hname));
   if(!h) return 0;
   return h;
}
/*
TGraph* best_fit_point(Int_t p1, Int_t p2){
  if(!gMinuit) return 0;
  TGraph* g = new TGraph(1);
  g->SetMarkerStyle(kMultiply);
  Double_t x, ex,y,ey;
  gMinuit->GetParameter(p1,x,ex);
  gMinuit->GetParameter(p2,y,ey);
  g->SetPoint(0,x,y);
  return g;
}

TGraph* two_dim_contour(Int_t p1, Int_t p2, Double_t sig, Int_t npts){
  if(!gMinuit) return 0;
  gMinuit->SetErrorDef(sig*sig);
  TGraph* g = static_cast<TGraph*>(gMinuit->Contour(npts,p1,p2));
  g->SetLineWidth(gStyle->GetHistLineWidth()); // TGraph() uses gStyle->GetLineWidth
  int N = g->GetN();
  g->SetPoint(N, g->GetX()[0],g->GetY()[0]); // make closed contour
  return g;
}

TMultiGraph* get_fit_contours(Int_t p1, Int_t p2, Int_t ncont, Int_t sig_start, Int_t sig_step, Int_t npts){

  if(!gMinuit) return 0;
  TMultiGraph* a = new TMultiGraph();
  TGraph* bf = best_fit_point(p1,p2);
  a->Add(bf);
  for(int i=0; i<ncont; i++){
    int sig = sig_start + i*sig_step;
    TGraph* g = two_dim_contour(p1,p2,sig,npts);
    a->Add(g);
  }
  return a;
}

TMultiGraph* draw_fit_contours(Int_t p1, Int_t p2, Int_t ncont, Int_t sig_start, Int_t sig_step, Int_t npts){

  TMultiGraph* g = get_fit_contours(p1,p2,ncont,sig_start,sig_step,npts);
  g->Draw("apc");
  return g;
}

TGraph* chisquare_scan(Int_t par_num, Int_t nsig_up, Int_t nsig_down, Int_t ndiv){
  if(!gMinuit) return 0;
  if(par_num<0) return 0;
  Int_t npar = gMinuit->GetNumPars();
  if(npar==0 || par_num>(npar-1)) return 0;
  
  Double_t* pars = new Double_t[npar];
  Double_t* epars = new Double_t[npar];
  
  Double_t p,ep;
  for(int i=0; i<npar; i++){
    gMinuit->GetParameter(i,p,ep);
    pars[i]=p;
    epars[i]=ep;
  }

  TGraph* g = new TGraph(ndiv*2+1);
  g->SetLineWidth(gStyle->GetHistLineWidth());
  
  const Double_t x=pars[par_num];
  const Double_t ex=epars[par_num];
  
  // scan parameter from minimum to best fit
  Double_t w=ex*nsig_down/( (double)ndiv);
  Double_t y=0;
  Int_t N=0;
  for(int i=0; i<ndiv; i++){
    pars[par_num] = x-nsig_down*ex + i*w;
    gMinuit->Eval(npar,0,y,pars,3);
    g->SetPoint(N,pars[par_num],y); N++;
  }
  // best fit
  pars[par_num]=x;  
  gMinuit->Eval(npar,0,y,pars,3);
  g->SetPoint(N,pars[par_num],y); N++;
  Double_t minchi2=y;
  // scan parameter from best fit to maximum
  w=ex*nsig_up/( (double)ndiv);
  y=0;
  for(int i=0; i<ndiv; i++){
    pars[par_num] = x + (i+1)*w;
    gMinuit->Eval(npar,0,y,pars,3);
    g->SetPoint(N,pars[par_num],y); N++;
  }
  // subtract value of minimum chi2 from each point
  for(int i=0; i<N; i++){
    Double_t yy=g->GetY()[i]-minchi2;
    Double_t xx=g->GetX()[i];
    g->SetPoint(i,xx,yy);
  }

  return g;

}

TGraph* draw_chisquare_scan(Int_t par_num, Int_t nsig_up, Int_t nsig_down, Int_t ndiv){
  TGraph* g = chisquare_scan(par_num,nsig_up,nsig_down,ndiv);
  g->Draw("apc");
  return g;
}
*/

TH1* log_h1(const char* hname, const char* tit, int nbins, float xmin, float xmax){
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
    
  TH1* h = new TH1F(hname, tit, nbins,xbins);  
  return h;
}

TProfile* log_prof(const char* hname, const char* tit, int nbins, float xmin, float xmax){
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
    
  TProfile* h = new TProfile(hname, tit, nbins,xbins);  
  return h;
}



TH1* log10_h1(const char* hname, const char* tit, int nbins, float xmin, float xmax){
  // xbins construction taken from rootalk : P.Sizun
  
  Double_t logxmin = TMath::Log10(xmin);
  Double_t logxmax = TMath::Log10(xmax);
  Double_t binwidth = (logxmax-logxmin)/nbins;
  Double_t xbins[nbins+1];
  xbins[0] = xmin;
  for (Int_t i=1;i<=nbins;i++) {
    xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidth);

  }
    
  TH1* h = new TH1F(hname, tit, nbins,xbins);  
  return h;
}

TProfile* log10_prof(const char* hname, const char* tit, int nbins, float xmin, float xmax){
  // xbins construction taken from rootalk : P.Sizun
  
  Double_t logxmin = TMath::Log10(xmin);
  Double_t logxmax = TMath::Log10(xmax);
  Double_t binwidth = (logxmax-logxmin)/nbins;
  Double_t xbins[nbins+1];
  xbins[0] = xmin;
  for (Int_t i=1;i<=nbins;i++) {
    xbins[i] = xmin + TMath::Power(10,logxmin+i*binwidth);

  }
    
  TProfile* h = new TProfile(hname, tit, nbins,xbins);  
  return h;
}
