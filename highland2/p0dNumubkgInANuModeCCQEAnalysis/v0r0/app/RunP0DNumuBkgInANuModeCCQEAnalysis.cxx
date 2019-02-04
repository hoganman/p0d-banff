#include "p0dNumuBkgInANuModeCCQEAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  p0dNumuBkgInANuModeCCQEAnalysis* ana = new p0dNumuBkgInANuModeCCQEAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
