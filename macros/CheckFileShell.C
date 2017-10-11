#define CheckFileShell_C_seen
#include<cstdlib>
#include"CheckFileROOT.C"

void CheckFileShell(TString fileName = ""){
  const int status = CheckFileROOT(fileName);
  exit(status);
}
