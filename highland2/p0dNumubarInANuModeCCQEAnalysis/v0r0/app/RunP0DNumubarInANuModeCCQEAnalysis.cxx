#include "p0dNumubarInANuModeCCQEAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  p0dNumubarInANuModeCCQEAnalysis* ana = new p0dNumubarInANuModeCCQEAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
