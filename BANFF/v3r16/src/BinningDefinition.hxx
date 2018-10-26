#include <map>

#include "TAxis.h"

#include "SampleId.hxx"
#include "Parameters.hxx"


namespace BANFF{

  class TAxis2D{
  private:
    TAxis* FirstDim;
    TAxis* SecondDim;
  public:
    TAxis2D(int nbin1, double* bins1,int nbin2, double* bins2){
      FirstDim  = new TAxis(nbin1, bins1);
      SecondDim = new TAxis(nbin2, bins2);
    };
    
    ~TAxis2D(){
      delete FirstDim;
      delete SecondDim;        
    };

    TAxis* operator[] (const unsigned int d){
      if     (d == 0) return FirstDim;
      else if(d == 1) return SecondDim;
      else{
        std::cerr << "Can't acces more than 2D" << std::endl;
        throw;
      }
    };
  };

  
  class BinningDefinition{
 
  private:
    BinningDefinition();
    BinningDefinition(const BinningDefinition& binning);
    static BinningDefinition* fBinningDefinition;

    std::map<SampleId::SampleEnum, bool> ActiveSample;
    
    std::map<SampleId::SampleEnum, TAxis*> Axis_Mom;
    std::map<SampleId::SampleEnum, TAxis*> Axis_Mom_Det;
    std::map<SampleId::SampleEnum, TAxis*> Axis_Cos;
    std::map<SampleId::SampleEnum, TAxis*> Axis_Cos_Det;
    std::map<SampleId::SampleEnum, TAxis2D*> bothAxis;
    std::map<SampleId::SampleEnum, TAxis2D*> bothAxis_Det;

    std::map<int, int> FullToReduMap;

    bool Do4PiFHC    ;
    bool DoNue       ;
    bool DoOnlyNue   ;
    bool DoMultiPiRHC;
    
    bool Do1DCheckMom;
    bool Do1DCheckCos;
    bool UseFlatNuMuBinning;
    
  public:
    ~BinningDefinition(){
      for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Mom.begin(); it!=Axis_Mom.end(); ++it){
        if(it->second) delete it->second;
        Axis_Mom.erase(it);
      }

      for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Mom_Det.begin(); it!=Axis_Mom_Det.end(); ++it){
        if(it->second) delete it->second;
        Axis_Mom_Det.erase(it);
      }
      for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Cos.begin(); it!=Axis_Cos.end(); ++it){
        if(it->second) delete it->second;
        Axis_Cos.erase(it);
      }

      for (std::map<SampleId::SampleEnum, TAxis*>::iterator it=Axis_Cos_Det.begin(); it!=Axis_Cos_Det.end(); ++it){
        if(it->second) delete it->second;
        Axis_Cos_Det.erase(it);
      }
    };

    int GetNbins_Mom    (const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return Axis_Mom    [sample]->GetNbins();
      else return 0;
    };
    int GetNbins_Mom_Det(const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return Axis_Mom_Det[sample]->GetNbins();
      else return 0;
    };
    int GetNbins_Cos    (const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return Axis_Cos    [sample]->GetNbins();
      else return 0;
    };
    int GetNbins_Cos_Det(const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return Axis_Cos_Det[sample]->GetNbins();
      else return 0;
    };
    
    int GetGlobalBinMatrix_Det(const SampleId::SampleEnum sample);
    int GetGlobalBinMatrix    (const SampleId::SampleEnum sample);
    void GetLocalBinMatrixMomCos    (const int GlobalBin, SampleId::SampleEnum& Sample, int& MomBin, int& CosBin);
    void GetLocalBinMatrixMomCos_Det(const int GlobalBin, SampleId::SampleEnum& Sample, int& MomBin, int& CosBin);

    int GetNbins_Det();
    int GetNbins    ();
    
    int GetNbins_Det(const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return GetNbins_Mom_Det(sample) * GetNbins_Cos_Det(sample);
      else return 0;
    };
    int GetNbins    (const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return GetNbins_Mom    (sample) * GetNbins_Cos    (sample);
      else return 0;
    };

    int GetGlobalBinMatrixMomCos_Det(const SampleId::SampleEnum sample, const double Mom, const double Cos);
    int GetGlobalBinMatrixMomCos    (const SampleId::SampleEnum sample, const double Mom, const double Cos);

    TAxis* GetBinning_Mom    (const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return Axis_Mom    [sample];
      else{
        std::cerr << "No momentum binning for the sample " << ConvertSample(sample) << std::endl;
        throw;
        return NULL;
      }
    };
    TAxis* GetBinning_Mom_Det(const  SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return Axis_Mom_Det[sample];
      else{
        std::cerr << "No momentum (det cov) binning for the sample " << ConvertSample(sample) << std::endl;
        throw;
        return NULL;
      }
    };
    TAxis* GetBinning_Cos    (const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return Axis_Cos    [sample];
      else{
        std::cerr << "No costheta binning for the sample " << ConvertSample(sample) << std::endl;
        throw;
        return NULL;
      }
    };
    TAxis* GetBinning_Cos_Det(const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return Axis_Cos_Det[sample];
      else{
        std::cerr << "No costheta (det cov) binning for the sample " << ConvertSample(sample) << std::endl;
        throw;
        return NULL;
      }
    };

    TAxis2D* GetBinningArray(const SampleId::SampleEnum sample){
      if(ActiveSample[sample]) return bothAxis[sample];
      else{
        std::cerr << "No costheta (det cov) binning for the sample " << ConvertSample(sample) << std::endl;
        throw;
        return NULL;
      }
    };

    SampleId::SampleEnum GetSampleFromIndex    (const int index);
    SampleId::SampleEnum GetSampleFromIndex_Det(const int index);

    bool IsActiveSample(const SampleId::SampleEnum sample){
      return ActiveSample[sample];
    };
    bool IsActiveSample(const int sample){
      if(sample >= SampleId::kUnassigned && sample < SampleId::kNSamples){
        SampleId::SampleEnum s = static_cast<SampleId::SampleEnum>(sample);
        return ActiveSample[s];
      }else return false;
    };
    std::vector<std::string> GetListOfBins();
    std::vector<std::string> GetListOfBins_Det();
    std::map<int,int> GetFullToReduMap();

    
    static BANFF::BinningDefinition& Get() {
      if (!fBinningDefinition) fBinningDefinition = new BANFF::BinningDefinition;
      return *(fBinningDefinition);
    };
    
  };


}
