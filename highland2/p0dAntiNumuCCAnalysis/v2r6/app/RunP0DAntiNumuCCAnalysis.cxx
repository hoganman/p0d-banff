#include "p0dAntiNumuCCAnalysis.hxx"
#include "AnalysisLoop.hxx"

int main(int argc, char *argv[]){
  p0dAntiNumuCCAnalysis* ana = new p0dAntiNumuCCAnalysis();
  AnalysisLoop loop(ana, argc, argv); 
  loop.Execute();
}
