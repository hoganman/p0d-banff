#include "Experiment.hxx"
#include "Parameters.hxx"
#include "BANFFInterfaceBase.hxx"

#include <iostream>
#include <iomanip>

int main(){

  ND::params().SetReadParamOverrideFilePointPassed();
  BANFFInterfaceBase* bib  = new BANFFInterfaceBase(0, NULL, NULL, "");
  Experiment* exp = new Experiment("nd280");

  //Double check that the same number of data and mc run periods exist.
  if(bib->dataFiles.size() != bib->mcFiles.size()) {
    for(std::map<std::string,std::vector<std::string> >::iterator it = bib->dataFiles.begin(); it != bib->dataFiles.end(); it++)
      std::cout << "dataFiles[" << "] " << it->first << std::endl;
    for(std::map<std::string, std::vector<std::pair<std::string, std::string> > >::iterator it = bib->mcFiles.begin(); it != bib->mcFiles.end(); it++)
      std::cout << "mcFiles  [" << "] " << it->first << std::endl;

    std::cout << "dataFiles.size() " << bib->dataFiles.size() << std::endl;
    std::cout << "mcFiles.size()   " << bib->mcFiles.size()   << std::endl;
    std::cout << "Error: Data and MC run periods do not match." << std::endl;
    exit(1);
  }

  //So, we have the same number of run periods between data and MC.  Create
  //an array of SampleGroups, one for each from the input files.
  SampleGroup** sampleGroups = new SampleGroup*[bib->dataFiles.size()];
  double FHCTotalPOTData     = 0;
  double FHCTotalPOTMagnetMC = 0;
  double FHCTotalPOTSandMC   = 0;
  double RHCTotalPOTData     = 0;
  double RHCTotalPOTMagnetMC = 0;
  double RHCTotalPOTSandMC   = 0;
  
  //Iterate through the maps, adding the data samples and the MC samples.
  //sgi is an iterator over the sample groups.
  int sgi = 0;
  ofstream fileout;
  fileout.open("POTTable.tex");
  fileout << "\\begin{table}[ht]\n";
  fileout << "\\begin{adjustbox}{center}\n";
  fileout << "\\begin{tabular}{llll}\n";
  fileout << "\\toprule\n";  
  fileout << "\\multirow{2}{*}{Runs} & \\multicolumn{3}{c}{POT}\\\\\n";
  fileout << " & Data & Magnet MC (ratio) & Sand MC (ratio) \\\\\n";
  fileout << "\\midrule\n";  
  fileout.precision(3);
  for(std::map<std::string,std::vector<std::string> >::iterator it = bib->dataFiles.begin();
      it != bib->dataFiles.end(); it++) {
    bool isFHC = (it->first.find("2") != std::string::npos ||
                  it->first.find("3") != std::string::npos ||
                  it->first.find("4") != std::string::npos ||
                  it->first.find("8") != std::string::npos);
    
    //Set up the new SampleGroup corresponding to this run period.
    sampleGroups[sgi] = new SampleGroup(it->first);

    //Add all data files.
    for( unsigned int i = 0; i < (it->second).size(); i++ ) {
      sampleGroups[sgi]->AddDataSample(new DataSample((it->second)[i]));
    }
        
    //Now, for the same run period, add all the MC files.
    for ( unsigned int i = 0; i < bib->mcFiles[it->first].size(); i++ ) {
      bool isSand = (((bib->mcFiles[it->first])[i]).first.find("SAND")!=std::string::npos ||
                     ((bib->mcFiles[it->first])[i]).first.find("sand")!=std::string::npos ||
                     ((bib->mcFiles[it->first])[i]).first.find("Sand")!=std::string::npos);
      if(isSand){
        std::cout << "FOUND SAND!! " << (bib->mcFiles[it->first])[i].first<< std::endl;
        sampleGroups[sgi]->AddMCSample(((bib->mcFiles[it->first])[i]).first,
                                       new DataSample(((bib->mcFiles[it->first])[i]).second, 1));
        
      }else{
        sampleGroups[sgi]->AddMCSample(((bib->mcFiles[it->first])[i]).first,
                                       new DataSample(((bib->mcFiles[it->first])[i]).second));
      }
      
    }

    //Add this sample group to the experiment.
    exp->AddSampleGroup(it->first, *sampleGroups[sgi]);

    Float_t POTdata=0, POTmc=0, POTsand=0;
    sampleGroups[sgi]->GetPOT(POTdata, POTmc, POTsand);
    if(isFHC){
      FHCTotalPOTData     = FHCTotalPOTData     + POTdata;
      FHCTotalPOTMagnetMC = FHCTotalPOTMagnetMC + POTmc;
      FHCTotalPOTSandMC   = FHCTotalPOTSandMC   + POTsand;
      fileout << it->first << "(FHC) & "
              << POTdata << " & "
              << POTmc   << "(" << POTdata/POTmc   << ") & "
              << POTsand << "(" << POTdata/POTsand << ") \\\\\n";
    }else{
      RHCTotalPOTData     = RHCTotalPOTData     + POTdata;
      RHCTotalPOTMagnetMC = RHCTotalPOTMagnetMC + POTmc;
      RHCTotalPOTSandMC   = RHCTotalPOTSandMC   + POTsand;
      fileout << it->first << "(RHC) & "
              << POTdata << " & "
              << POTmc   << "(" << POTdata/POTmc   << ") & "
              << POTsand << "(" << POTdata/POTsand << ") \\\\\n";
    }
   
    //Since we're done with this sample group, increment the counter.
    sgi++;
  }
  fileout << "\\midrule\n";  
  fileout << "Total FHC & "
          << FHCTotalPOTData     << " & "
          << FHCTotalPOTMagnetMC << "(" << FHCTotalPOTData / FHCTotalPOTMagnetMC << ") & "
          << FHCTotalPOTSandMC   << "(" << FHCTotalPOTData / FHCTotalPOTSandMC   << ") \\\\ \n"; 
  fileout << "Total RHC & "
          << RHCTotalPOTData     << " & "
          << RHCTotalPOTMagnetMC << "(" << RHCTotalPOTData / RHCTotalPOTMagnetMC << ") & "
          << RHCTotalPOTSandMC   << "(" << RHCTotalPOTData / RHCTotalPOTSandMC   << ") \\\\ \n"; 
  fileout << "\\bottomrule\n";
  fileout << "\\end{tabular}\n";
  fileout << "\\end{adjustbox}\n";
  fileout << "\\end{table}\n";

  std::map<std::string, SampleGroup> SampleGroups = exp->GetSampleGroups();
  for (std::map<std::string, SampleGroup>::iterator sit = SampleGroups.begin();
       sit != SampleGroups.end(); ++sit) {
    // Get the current sample group
    SampleGroup TempGroup = (*sit).second;

    // Get the data
    std::vector<DataSample*> DataSamples = TempGroup.GetDataSamples();
    std::cout << "-------------------------" << std::endl;
    std::cout << "Sample group " << sit->first << std::endl;
    std::cout << "Data:" << std::endl;
    for (std::vector<DataSample*>::iterator it = DataSamples.begin(); it != DataSamples.end(); ++it) {
      std::cout << (*it)->GetFilePath() << " POT:" << std::endl;
      (*it)->DumpPOT();
    }

    // Get the MC
    std::map<std::string, DataSample*> MCSampleGroups = TempGroup.GetMCSamples();
    std::cout << "MC:" << std::endl;
    for (std::map<std::string, DataSample*>::iterator it = MCSampleGroups.begin(); it != MCSampleGroups.end(); ++it) {
      std::cout << (*it).first << " from " << (*it).second->GetFilePath() << " POT:" << std::endl;
      (*it).second->DumpPOT();
    }
  }
  return 1;
}
