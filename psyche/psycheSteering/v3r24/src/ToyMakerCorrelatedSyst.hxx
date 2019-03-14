#ifndef TOYMAKERCORRELATEDSYST
#define TOYMAKERCORRELATEDSYST

#include <vector>

#include "TXMLEngine.h"
#include "TRandom3.h"
#include "TMatrixTSym.h"
#include "TH2.h"
#include "TDecompChol.h"

#include "ToyExperiment.hxx"
#include "SystematicManager.hxx"
#include "ToyMaker.hxx"
#include "SystematicBase.hxx"
#include "SystematicManager.hxx"
#include "SystId.hxx"


// A small class which contains the systematic id and the parameter number in that systematic
class SystematicParameter{

private:
  SystId::SystEnum _syst;
  UInt_t _param;

  SystematicParameter() {
    _syst  = SystId::kBFieldDist;
    _param = 0;
  };

public:
  SystematicParameter(SystId::SystEnum syst ,
                      UInt_t           param):
                      _syst(syst),_param(param){
  };
  
  bool operator<(SystematicParameter const& other) const{
    if     ((int)_syst <  (int)other._syst) return true;
    else if((int)_syst >  (int)other._syst) return false;
    else if((int)_syst == (int)other._syst){
      if     (_param < other._param) return true;
      return false;
    } 
    return false;
  }

  SystId::SystEnum GetParameter() { return _syst ; };
  UInt_t           GetSystId   () { return _param; };

  void SetParameter(SystId::SystEnum syst ) { _syst  = syst ; };
  void SetSystId   (UInt_t           param) { _param = param; };
};

class ToyMakerCorrelatedSyst:public ToyMaker{
public:
  ToyMakerCorrelatedSyst(unsigned int seed, SystematicManager& systs);
  ~ToyMakerCorrelatedSyst();

  void FillToyExperiment(ToyExperiment& toy);  
  
  void CorrelateAllParametersInSystematic(SystematicBase* syst);
  void CorrelateDiagonalOfSystematic     (SystematicBase* syst1, SystematicBase* syst2);
  void CorrelateSystematic               (SystematicBase* syst1, SystematicBase* syst2);
  void CorrelateParametersSystematic     (SystematicBase* syst1, UInt_t param1, SystematicBase* syst2, UInt_t param2);
  void UnCorrelateParametersSystematic   (SystematicBase* syst1, UInt_t param1, SystematicBase* syst2, UInt_t param2);
  void CorrelateParametersBlockSystematic(SystematicBase* syst1, SystematicBase* syst2,
                                          UInt_t size, UInt_t param1, UInt_t param2);
  void CorrelateParametersBlockToParamSystematic(SystematicBase* syst1, SystematicBase* syst2,
                                                 UInt_t size, UInt_t param1, UInt_t param2);

  
  void SetXMLFile(std::string filename) {_file_name = filename; };  
  std::string GeXMLFile() const { return _file_name; };
  UInt_t GetSeed() const { return _seed; };
  void SetSeed(unsigned int seed) { _seed = seed; };
  void SaveCorrelationMatrixTH2D(std::string filename="correlation.root");
  void CreateInputXMLParameterFileFromCov();
  virtual void CreateCorrelationMatrix(){}; // so that the user can inherit and change that
  void ParseInputXMLFileAndCreateCorrelation();
  void CreateInputXMLParameterFile();

  void DeCorrelateInMatrix    (UInt_t, UInt_t);
  void FillCorrelationInMatrix(UInt_t, UInt_t);  
  void InvertMatrix();
  void Usage();
  std::vector<UInt_t> ParseCommaSeparatedValueString(std::string str);

  TMatrixTSym<Double_t>* GetCorrelationMatrix(){return _correlation_matrix;}

private:

  /// The random seed used
  UInt_t _seed;

  /// A random generator that can be used to generate throws.
  TRandom3* _RandomGenerator;

  UInt_t _mat_size;
  TMatrixTSym<Double_t>* _correlation_matrix;
  TMatrixT<Double_t>* _inverted_correlation_matrix;
  std::map<SystematicParameter, UInt_t> _map_systematic_param;
    
  std::string _file_name;
  TXMLEngine* _xml_engine;
  XMLDocPointer_t _xml_input_file;

  std::vector<SystematicBase*> _systematic_vector;
  std::map<SystId::SystEnum, SystematicBase*> _map_systematic;

};
#endif
