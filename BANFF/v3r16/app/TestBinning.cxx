#include "BinningDefinition.hxx"


#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"


int main(){
  ND::params().SetReadParamOverrideFilePointPassed();
  BANFF::BinningDefinition& bd = BANFF::BinningDefinition::Get();

  int ibinMom=0;
  int cbinMom=0;

  int ibinCos=0;
  int cbinCos=0;

  int ibinMomCos=0;
  int cbinMomCos=0;

  bool Do1DCheckMom = (bool)ND::params().GetParameterI("BANFF.Do1DCheckMom");
  bool Do1DCheckCos = (bool)ND::params().GetParameterI("BANFF.Do1DCheckCos");
  
  
  int nBinsRedu = bd.GetNbins_Det();
  int nBinsFull = bd.GetNbins();
  
  for(int j = 0; j < nBinsFull; ++j){
    int binmom;
    int bincos;
    SampleId::SampleEnum sample;
    bd.GetLocalBinMatrixMomCos(j, sample, binmom, bincos);
    double ph = bd.GetBinning_Mom(sample)->GetBinUpEdge (binmom+1);
    double pl = bd.GetBinning_Mom(sample)->GetBinLowEdge(binmom+1);
    double ch = bd.GetBinning_Cos(sample)->GetBinUpEdge (bincos+1);
    double cl = bd.GetBinning_Cos(sample)->GetBinLowEdge(bincos+1);

    if(j != bd.GetGlobalBinMatrixMomCos(sample, 0.5*(ph+pl), 0.5*(ch+cl))){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << j << std::endl;
      std::cout << "Should be identical to bd.GetGlobalBinMatrixMomCos(" << sample << ", " << "0.5*("<<ph<<"+"<<pl<<"), " << "0.5*("<<ch<<"+"<<cl<<")): " <<  bd.GetGlobalBinMatrixMomCos(sample, 0.5*(ph+pl), 0.5*(ch+cl)) << std::endl;
      throw;
    }
    if(j != bd.GetGlobalBinMatrix(sample)+bincos*bd.GetNbins_Mom(sample)+binmom){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << j << std::endl;
      std::cout << "Should be identical to bd.GetGlobalBinMatrix(" << sample << ")" <<  bd.GetGlobalBinMatrix(sample) << " + bincos*bd.GetNbins_Mom(sample)+binmom" << std::endl;
      throw;
    }
    double epsilon = 0.0001; // Make sure this is smaller that the cos binning!!
    int bin_det  = bd.GetGlobalBinMatrixMomCos_Det(sample, ph-epsilon, ch-epsilon);
    int bin2_det = bd.GetGlobalBinMatrixMomCos_Det(sample, pl+epsilon, cl+epsilon);
    SampleId::SampleEnum sample_det;
    int binmom_det, bincos_det;
    bd.GetLocalBinMatrixMomCos_Det(bin_det, sample_det, binmom_det, bincos_det);
    
    if(bin_det != bin2_det){
      std::cerr << "The fine binning is more coarse than the detector binning!!!" << std::endl;
      std::cerr << "Maybe epilson is too large for the Cos binning?" << std::endl;
      
      std::cerr << "First detector cov bin corresponds to (low edge of the fine binning): Global bin " << bin_det << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinLowEdge("<<binmom_det<<") " << bd.GetBinning_Mom_Det(sample_det)->GetBinLowEdge(binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinUpEdge ("<<binmom_det<<") " << bd.GetBinning_Mom_Det(sample_det)->GetBinUpEdge (binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinLowEdge("<<bincos_det<<") " << bd.GetBinning_Cos_Det(sample_det)->GetBinLowEdge(bincos_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinUpEdge ("<<bincos_det<<") " << bd.GetBinning_Cos_Det(sample_det)->GetBinUpEdge (bincos_det+1) << std::endl;

      bd.GetLocalBinMatrixMomCos_Det(bin2_det, sample_det, binmom_det, bincos_det);
      std::cerr << "Second detector cov bin corresponds to (high edge of the fine binning): Global bin " << bin2_det << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinLowEdge("<<binmom_det<<") " << bd.GetBinning_Mom_Det(sample_det)->GetBinLowEdge(binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Mom Binning->GetBinUpEdge ("<<binmom_det<<") " << bd.GetBinning_Mom_Det(sample_det)->GetBinUpEdge (binmom_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinLowEdge("<<bincos_det<<") " << bd.GetBinning_Cos_Det(sample_det)->GetBinLowEdge(bincos_det+1) << std::endl;
      std::cerr << "DET: Sample "<<SampleId::ConvertSample(sample_det)<<"Cos Binning->GetBinUpEdge ("<<bincos_det<<") " << bd.GetBinning_Cos_Det(sample_det)->GetBinUpEdge (bincos_det+1) << std::endl;

      std::cerr << "These 2 should be the same!!" << std::endl;
      throw;
    }

    double ph_det = bd.GetBinning_Mom_Det(sample_det)->GetBinUpEdge (binmom_det+1);
    double pl_det = bd.GetBinning_Mom_Det(sample_det)->GetBinLowEdge(binmom_det+1);
    double ch_det = bd.GetBinning_Cos_Det(sample_det)->GetBinUpEdge (bincos_det+1);
    double cl_det = bd.GetBinning_Cos_Det(sample_det)->GetBinLowEdge(bincos_det+1);
    if(bin_det != bd.GetGlobalBinMatrixMomCos_Det(sample_det, 0.5*(ph_det+pl_det), 0.5*(ch_det+cl_det))){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << bin_det << std::endl;
      std::cout << "Should be identical to bd.GetGlobalBinMatrixMomCos_Det(" << sample_det << ", 0.5*(ph+pl), 0.5*(ch+cl))" <<  bd.GetGlobalBinMatrixMomCos_Det(sample_det, 0.5*(ph_det+pl_det), 0.5*(ch_det+cl_det)) << std::endl;
      throw;
    }
    if(bin_det != bd.GetGlobalBinMatrix_Det(sample_det)+bincos_det*bd.GetNbins_Mom_Det(sample_det)+binmom_det){
      std::cout << "Closure test failed!! " << std::endl;
      std::cout << "Global Bin: " << bin_det << std::endl;
      std::cout << "Should be identical to bd.GetGlobalBinMatrix(" << sample_det << ")" <<  bd.GetGlobalBinMatrix(sample_det) << " + bincos_det*bd.GetNbins_Mom_Det(sample_det)+binmom_det" << std::endl;
      throw;
    }
    
    //if(j == 20)throw;
    
  }      
  
  std::cout << "?*?*?*?*?*?*?*?*?*?" << std::endl;
  std::cout << "?*?* Finished ?*?*?" << std::endl;
  std::cout << "?*?*?*?*?*?*?*?*?*?" << std::endl;
  
  throw;
  
  
  
  for(int i = SampleId::kUnassigned;
      i < SampleId::kNSamples; ++i){
    if(bd.IsActiveSample(i)){

      SampleId::SampleEnum sample = (SampleId::SampleEnum)i;
      std::string sampleName = SampleId::ConvertSample(sample);

      if(Do1DCheckMom)
        for(double jMom = 0; jMom <= 30000; jMom = jMom+25){
          ibinMom = bd.GetGlobalBinMatrixMomCos_Det(sample, jMom, 0);
          if(ibinMom >  cbinMom+1){
            std::cout << "binMom Fucked up here sample:" <<  sampleName << " bin: " << ibinMom << " skipping the bin cbin: " << cbinMom << std::endl;
            throw;
          }
          if(ibinMom != cbinMom)   std::cout << "binMom Sample:" << sampleName << " bin: " << ibinMom << std::endl;
          cbinMom = ibinMom;
        }
      
      if(Do1DCheckCos)
        for(double jCos = -1; jCos <= 1; jCos = jCos+0.005){
          ibinCos = bd.GetGlobalBinMatrixMomCos_Det(sample,0,jCos);
          if(ibinCos >  cbinCos+1){
            std::cout << "binCos Fucked up here sample:" <<  sampleName << " bin: " << ibinCos << " skipping the bin cbin: " << cbinCos << std::endl;
            throw;
          }
          if(ibinCos != cbinCos)   std::cout << "binCos Sample:" << sampleName << " bin: " << ibinCos << std::endl;
          cbinCos = ibinCos;
        }
      if(!Do1DCheckCos && !Do1DCheckMom)
        for(double jCos = -1; jCos <= 1; jCos = jCos+0.005){
          for(double jMom = 0; jMom <= 30000; jMom = jMom+25){
            ibinMomCos = bd.GetGlobalBinMatrixMomCos_Det(sample,jMom,jCos);
            if(ibinMomCos >  cbinMomCos+1){
              std::cout << "binMomCos Fucked up here sample:" <<  sampleName << " bin: " << ibinMomCos << " skipping the bin cbin: " << cbinMomCos << std::endl;
              throw;
            }
            if(ibinMomCos != cbinMomCos)   std::cout << "binMomCos Sample:" << sampleName << " bin: " << ibinMomCos << std::endl;
            cbinMomCos = ibinMomCos;
          }
        }

      TCanvas* can = new TCanvas();
      TH1D* MomFull = new TH1D(std::string("MomFull_"+sampleName).c_str(),std::string("MomFull_"+sampleName).c_str(),1,0,1);
      TH1D* CosFull = new TH1D(std::string("CosFull_"+sampleName).c_str(),std::string("CosFull_"+sampleName).c_str(),1,0,1);
      TH1D* MomRedu = new TH1D(std::string("MomRedu_"+sampleName).c_str(),std::string("MomRedu_"+sampleName).c_str(),1,0,1);
      TH1D* CosRedu = new TH1D(std::string("CosRedu_"+sampleName).c_str(),std::string("CosRedu_"+sampleName).c_str(),1,0,1);
      TH2D* Full2D  = new TH2D(std::string("Full2D_" +sampleName).c_str(),std::string("Full2D_" +sampleName).c_str(),1,0,1,1,0,1);
      TH2D* Redu2D  = new TH2D(std::string("Redu2D_" +sampleName).c_str(),std::string("Redu2D_" +sampleName).c_str(),1,0,1,1,0,1);

      MomFull->GetXaxis()->Set(bd.GetBinning_Mom    (sample)->GetNbins(), bd.GetBinning_Mom    (sample)->GetXbins()->GetArray());
      CosFull->GetXaxis()->Set(bd.GetBinning_Cos    (sample)->GetNbins(), bd.GetBinning_Cos    (sample)->GetXbins()->GetArray());
      MomRedu->GetXaxis()->Set(bd.GetBinning_Mom_Det(sample)->GetNbins(), bd.GetBinning_Mom_Det(sample)->GetXbins()->GetArray());
      CosRedu->GetXaxis()->Set(bd.GetBinning_Cos_Det(sample)->GetNbins(), bd.GetBinning_Cos_Det(sample)->GetXbins()->GetArray());
      Full2D ->GetXaxis()->Set(bd.GetBinning_Mom_Det(sample)->GetNbins(), bd.GetBinning_Mom_Det(sample)->GetXbins()->GetArray());
      Full2D ->GetYaxis()->Set(bd.GetBinning_Cos_Det(sample)->GetNbins(), bd.GetBinning_Cos_Det(sample)->GetXbins()->GetArray());
      Redu2D ->GetXaxis()->Set(bd.GetBinning_Mom    (sample)->GetNbins(), bd.GetBinning_Mom    (sample)->GetXbins()->GetArray());
      Redu2D ->GetYaxis()->Set(bd.GetBinning_Cos    (sample)->GetNbins(), bd.GetBinning_Cos    (sample)->GetXbins()->GetArray());

      // MomFull->Draw(); can->SaveAs(std::string(sampleName+"_MomFull.png").c_str());
      // CosFull->Draw(); can->SaveAs(std::string(sampleName+"_CosFull.png").c_str());
      // MomRedu->Draw(); can->SaveAs(std::string(sampleName+"_MomRedu.png").c_str());
      // CosRedu->Draw(); can->SaveAs(std::string(sampleName+"_CosRedu.png").c_str());
      // Full2D ->Draw(); can->SaveAs(std::string(sampleName+"_Full2D .png").c_str());
      // Full2D ->Draw(); can->SaveAs(std::string(sampleName+"_Full2D .png").c_str());
      // Redu2D ->Draw(); can->SaveAs(std::string(sampleName+"_Redu2D .png").c_str());
      // Redu2D ->Draw(); can->SaveAs(std::string(sampleName+"_Redu2D .png").c_str());
     
    }
  }

  return 1;

}



