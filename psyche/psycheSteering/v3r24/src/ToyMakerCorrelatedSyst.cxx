#include "ToyMakerCorrelatedSyst.hxx"

// This is the precision the correlation matrix, with the current number of systematics parameter (~500),
// this leads to a precision of 3 per mil...
// Note that you can't put zero here, otherwise the matrix becomes whatever and the computer can't invert it.
const double kSmallNumber = 0.00000001;


//********************************************
ToyMakerCorrelatedSyst::ToyMakerCorrelatedSyst(unsigned int seed, SystematicManager& systs)
    :ToyMaker(),
    _seed(seed),
    _RandomGenerator(new TRandom3(_seed)),
    _mat_size(0),
    _correlation_matrix(NULL),
    _inverted_correlation_matrix(NULL),
    _file_name(""),
    _xml_engine(new TXMLEngine()),
    _xml_input_file(NULL){
//********************************************

  _systematic_vector = systs.GetEnabledSystematics();
  _mat_size = 0;
  UInt_t index=0;
  for(std::vector<SystematicBase*>::iterator it = _systematic_vector.begin();
      it != _systematic_vector.end(); ++it){
    _mat_size = _mat_size + (*it)->GetNParameters();
    _map_systematic[(SystId::SystEnum)(*it)->GetIndex()] = (*it);
    std::cout << "Systematic Id " << (*it)->GetIndex() << "  " << (*it)->Name() << std::endl;
    for(UInt_t i=0; i<(*it)->GetNParameters(); ++i){
      _map_systematic_param[SystematicParameter((SystId::SystEnum)(*it)->GetIndex(),i)] = index;
      index++;
    }
  }

  _correlation_matrix = new TMatrixTSym<Double_t>(_mat_size);

  for(size_t i = 0; i < _mat_size; ++i){
    for(size_t j = 0; j < _mat_size; ++j){
      (*_correlation_matrix)[i][j] = 0.;
      (*_correlation_matrix)[i][i] = 1.+kSmallNumber;
    }
  }


  for(size_t ind = 0; ind < _mat_size; ++ind){
    for(std::vector<SystematicBase*>::iterator it = _systematic_vector.begin();
        it != _systematic_vector.end(); ++it){
      for(UInt_t i=0; i<(*it)->GetNParameters(); ++i){
        //    if(_map_systematic_param[SystematicParameter((SystId::SystEnum)(*it)->GetIndex(),i)] == ind)
        //      std::cout << "Index " << ind << ": " << SystId::ConvertSystematic((SystId::SystEnum)(*it)->GetIndex()) << "  param " << i << std::endl;
      }
    }
  }
};


//********************************************
ToyMakerCorrelatedSyst::~ToyMakerCorrelatedSyst(){
//********************************************

  if(_correlation_matrix)          delete _correlation_matrix;
  if(_inverted_correlation_matrix) delete _inverted_correlation_matrix;
  //if(_xml_input_file)              delete _xml_input_file;
  if(_xml_engine)                  delete _xml_engine;

  _correlation_matrix          = NULL;
  _inverted_correlation_matrix = NULL;
  //_xml_input_file              = NULL;
  _xml_engine                  = NULL;

};



//********************************************
void ToyMakerCorrelatedSyst::InvertMatrix(){
//********************************************

  TMatrixTSym<double> *smallercorr = new TMatrixTSym<double>(_correlation_matrix->GetNrows());

  for(int i = 0; i < _correlation_matrix->GetNrows(); i++)
    for(int j = 0; j < _correlation_matrix->GetNrows(); j++)
      (*smallercorr)(i, j) = (*_correlation_matrix)(i, j);

  TDecompChol *chdcmp = new TDecompChol(*_correlation_matrix);

  int chuck = 0;
  while(!chdcmp->Decompose()){
    if(chuck == 1){
      SaveCorrelationMatrixTH2D("FailedMatrix.root");
    }
    std::cerr << "ERROR: Cholesky decomposition failed!" << std::endl;
    std::cerr << "ERROR: Check the file FailedMatrix.root!" << std::endl;

    if(smallercorr)
      delete smallercorr;

    smallercorr = new TMatrixTSym<double>(_correlation_matrix->GetNrows() - chuck);
    for(int i = 0; i < _correlation_matrix->GetNrows() - chuck; i++)
      for(int j = 0; j < _correlation_matrix->GetNrows() - chuck; j++)
        (*smallercorr)(i, j) = (*_correlation_matrix)(i, j);

    delete chdcmp;
    chdcmp = new TDecompChol(*smallercorr);
    chuck++;
  }

  std::cout << "Decomposed after attempt " << chuck << std::endl;

  if(chuck != 0){
    std::cerr << "ERROR: Check the line/column " << _correlation_matrix->GetNrows() - chuck << " there is probably something wrong there in the correlation matrix as the Cholesky decomposition failed." << std::endl;
    std::cerr << "ERROR: For now, exit..." << std::endl;
    throw;
  }

  //get upper triangular matrix - want lower so transpose
  _inverted_correlation_matrix = new TMatrixT<Double_t>(chdcmp->GetU());
  _inverted_correlation_matrix->Transpose(*_inverted_correlation_matrix);
  if(_inverted_correlation_matrix->GetNcols() != _correlation_matrix->GetNcols()){
    std::cerr << "Problem in the matrix inversion" << std::endl;
    throw;
  }

  delete chdcmp;

};


//********************************************
void ToyMakerCorrelatedSyst::Usage() {
//********************************************

  std::cout << "To use the ToyMakerCorrelatedSyst, you have to call (at least)!" << std::endl;
  std::cout << "ToyMakerCorrelatedSyst tm(seed)" << std::endl;
  std::cout << "tm.InvertMatrix()" << std::endl;
  std::cout << "tm.CreateToyExperiments(ntoys, systematics)" << std::endl;

};


//********************************************
std::vector<UInt_t> ToyMakerCorrelatedSyst::ParseCommaSeparatedValueString(std::string str){
//********************************************

  std::vector<UInt_t> vect;
  std::stringstream ss(str);

  UInt_t i;
  while (ss >> i){
    vect.push_back(i);

    if (ss.peek() == ',' || ss.peek() == ' ' || ss.peek() == ';')
      ss.ignore();
  }
  return vect;

}


//********************************************
void ToyMakerCorrelatedSyst::ParseInputXMLFileAndCreateCorrelation() {
//********************************************

  if(_file_name == ""){
    std::cerr << "ERROR: Can't parse the file because _file_name isn't specified!" << std::endl;
    std::cerr << "ERROR: Exiting " << std::endl;
    throw;
  }

  if(!_correlation_matrix){
    std::cerr << "ERROR: Can't parse the file because the correlation matrix isn't initialised!" << std::endl;
    std::cerr << "ERROR: Exiting " << std::endl;
    throw;
  }

  _xml_input_file = _xml_engine->ParseFile(_file_name.c_str());

  if (_xml_input_file == NULL) {
    delete _xml_engine;
    return;
  }

  // take access to main node
  XMLNodePointer_t mainnode = _xml_engine->DocGetRootElement(_xml_input_file);

  // Now parse the heck out of this
  std::cout << "Using the XML parser!" << std::endl;

  XMLNodePointer_t SystematicNode = _xml_engine->GetChild(mainnode);

  while(SystematicNode != 0){ // First loop is over the systematic
    std::string SystematicName1(_xml_engine->GetNodeName(SystematicNode));

    std::cout << "Parsing info for systematic " << SystematicName1 << std::endl;
    SystId::SystEnum syst1 = SystId::GetSystematic(SystematicName1);

    if(syst1 == SystId::kNotASyst){
      std::cerr << SystematicName1 << " doesn't exist?!?" << std::endl;
      throw;
    }

    XMLNodePointer_t TypeOfCorrNode = _xml_engine->GetChild(SystematicNode);

    while(TypeOfCorrNode != 0){
      std::string TypeOfCorr(_xml_engine->GetNodeName(TypeOfCorrNode));

      SystId::SystEnum syst2 = SystId::kNotASyst;

      if(TypeOfCorr == "WithItself"){
        std::cout << "Correlating " << SystematicName1 << " with itself" << std::endl;
        syst2 = syst1;
      }
      else{
        syst2 = SystId::GetSystematic(TypeOfCorr);
        std::cout << "Correlating " << SystematicName1 << " with " << TypeOfCorr <<  "(" << SystId::ConvertSystematic(syst2) << ")" << std::endl;
      }


      // Check that the systematic is switched on
      std::map<SystId::SystEnum, SystematicBase*>::iterator it1 = _map_systematic.find(syst1);
      std::map<SystId::SystEnum, SystematicBase*>::iterator it2 = _map_systematic.find(syst2);
      if(it1 == _map_systematic.end() || it2 == _map_systematic.end()){
        std::cout << "Systematic ID = " << syst1 << " or " << syst2 << " is not enabled, so skip correlations for that systematic" << std::endl;
        TypeOfCorrNode = _xml_engine->GetNext(TypeOfCorrNode);
        continue;
      }


      XMLAttrPointer_t attr = _xml_engine->GetFirstAttr(TypeOfCorrNode);
      //std::cout << "Before loop " << std::endl;
      while(attr != NULL){
        std::string TheCorr  = _xml_engine->GetAttrName(attr);

        if(TheCorr == "AllParams" || TheCorr == ""){
          std::cout << "Correlating all systematics " << syst1 << std::endl;
          CorrelateSystematic(_map_systematic[syst1],_map_systematic[syst2]);

        }
        else if(TheCorr == "Diag"){
          CorrelateDiagonalOfSystematic(_map_systematic[syst1], _map_systematic[syst2]);

        }
        else{ //these ones need attribute values!
          std::string ValueStr = _xml_engine->GetAttrValue(attr);
          std::vector<UInt_t> Values = ParseCommaSeparatedValueString(ValueStr);

          if(TheCorr == "Param"){
            if(Values.size() != 2){
              std::cerr << "you need 2 values for correlating 2 param in the systematic (param1, param2)" << std::endl;
              throw;
            }
            CorrelateParametersSystematic(_map_systematic[syst1], Values[0], _map_systematic[syst2], Values[1]);
          }
          else if(TheCorr == "UncorFromSystParam"){
            if(Values.size() != 2){
              std::cerr << "you need 2 values for uncorrelating a param in the systematic (param1, param2)" << std::endl;
              throw;
            }
            UnCorrelateParametersSystematic(_map_systematic[syst1], Values[0], _map_systematic[syst2], Values[1]);
          }
          else if(TheCorr == "Block"){
            if(Values.size() != 3){
              std::cerr << "you need 3 values for correlating a block in the systematic (size, position1, position2)" << std::endl;
              throw;
            }
            CorrelateParametersBlockSystematic(_map_systematic[syst1], _map_systematic[syst2], Values[0], Values[1], Values[2]);
          }
          else if(TheCorr == "BlockParam"){
            if(Values.size() != 3){
              std::cerr << "you need 3 values for correlating a block to a parameter in the systematic (size, position1, param)" << std::endl;
              throw;
            }
            CorrelateParametersBlockToParamSystematic(_map_systematic[syst1], _map_systematic[syst2], Values[0], Values[1], Values[2]);
          }
        }
        attr = _xml_engine->GetNextAttr(attr);
       }

      TypeOfCorrNode = _xml_engine->GetNext(TypeOfCorrNode);

    }

    SystematicNode = _xml_engine->GetNext(SystematicNode);
  }

  // Release memory before exit
  _xml_engine->FreeDoc(_xml_input_file);
  delete _xml_engine;

};

//********************************************
void ToyMakerCorrelatedSyst::CreateInputXMLParameterFileFromCov(){
//********************************************

  std::cerr << "This functionality hasn't been implemented yet!" << std::endl;
  throw;

};

//********************************************
void ToyMakerCorrelatedSyst::CorrelateParametersBlockSystematic(SystematicBase* syst1, SystematicBase* syst2,
    UInt_t size, UInt_t param1, UInt_t param2){
//********************************************
  if(size + param1 > syst1->GetNParameters() ||
      size + param2 > syst2->GetNParameters()){
    std::cerr << "ERROR: Systematics size " << syst1->Name() << " and " << syst2->Name()
      << " (" << syst1->GetNParameters()<<", "<< syst2->GetNParameters()
      << ") is smaller than the block you want ot correlate (" << size+param1 << ", " << size+param2 << ")" << std::endl;
    std::cerr << "ERROR: Exiting" << std::endl;
    throw;
  }
  for(UInt_t i=0; i<size; ++i){
    for(UInt_t j=0; j<size; ++j){
      UInt_t index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),i+param1)];
      UInt_t index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),j+param2)];
      FillCorrelationInMatrix(index1, index2);
    }
  }
}

//********************************************
void ToyMakerCorrelatedSyst::CorrelateParametersBlockToParamSystematic(SystematicBase* syst1, SystematicBase* syst2,
    UInt_t size, UInt_t param1, UInt_t param2){
//********************************************

  if(size + param1 >  syst1->GetNParameters() ||
      param2 > syst2->GetNParameters()){
    std::cerr << "ERROR: Systematics size " << syst1->Name() << " and " << syst2->Name()
      << " (" << syst1->GetNParameters()<<", "<< syst2->GetNParameters()
      << ") is smaller than the block you want ot correlate (" << size+param1 << ", " << size+param2 << ")" << std::endl;
    std::cerr << "ERROR: Exiting" << std::endl;
    throw;
  }
  // You first need to make sure that the block is already correlated within itself
  CorrelateParametersBlockSystematic(syst1, syst1, size, param1, param1);

  // You first need to make sure that the block is already correlated within itself
  CorrelateParametersSystematic(syst1, param2, syst2, param2);

  // now draws a line in the correlation matrix
  for(UInt_t i=0; i<size; ++i){
    UInt_t index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),i+param1)];
    UInt_t index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),  param2)];
    FillCorrelationInMatrix(index1, index2);
  }
  for(UInt_t i=0; i<size; ++i){
    UInt_t index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),  param2)];
    UInt_t index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),i+param1)];
    FillCorrelationInMatrix(index1, index2);
  }

}


//********************************************
void ToyMakerCorrelatedSyst::FillCorrelationInMatrix(unsigned int i, unsigned int j){
//********************************************

  (*_correlation_matrix)[i][j] = 1.;
  (*_correlation_matrix)[j][i] = 1.;
  if(i == j)(*_correlation_matrix)[i][j] = 1. + kSmallNumber;

};

//********************************************
void ToyMakerCorrelatedSyst::DeCorrelateInMatrix(unsigned int i, unsigned int j){
//********************************************

  if(i == j)(*_correlation_matrix)[i][j] = 1. + kSmallNumber;
  else{
    (*_correlation_matrix)[i][j] = 0.;
    (*_correlation_matrix)[j][i] = 0.;
  }

};


//********************************************
void ToyMakerCorrelatedSyst::CorrelateSystematic(SystematicBase* syst1, SystematicBase* syst2){
//********************************************

  if(syst1->GetNParameters() != syst2->GetNParameters()){
    std::cerr << "WARNING: you are trying to correlate " <<syst1->Name() << " to " << syst2->Name()
      << " which don't have the same number of parameters (" << syst1->GetNParameters() << " and" << syst2->GetNParameters() << ")" << std::endl;
    std::cerr << "WARNING: To get a invertible matrix, all the parameters of each systematic need to be correlated together." << std::endl;
    std::cerr << "WARNING: Correlating all the parameters of " << syst1->Name() << " and " << syst2->Name() << std::endl;
    CorrelateAllParametersInSystematic(syst1);
    CorrelateAllParametersInSystematic(syst2);
    for(UInt_t i=0; i<syst1->GetNParameters(); ++i){
      for(UInt_t j=0; j<syst2->GetNParameters(); ++j){
        UInt_t index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),i)];
        UInt_t index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),j)];
        FillCorrelationInMatrix(index1, index2);
      }
    }
  }else{
    for(UInt_t i=0; i<syst1->GetNParameters(); ++i){
      for(UInt_t j=0; j<syst2->GetNParameters(); ++j){
        UInt_t index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),i)];
        UInt_t index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),j)];
        FillCorrelationInMatrix(index1, index2);
      }
    }
  }
};


//********************************************
void ToyMakerCorrelatedSyst::CorrelateDiagonalOfSystematic(SystematicBase* syst1, SystematicBase* syst2){
//********************************************

  if(syst1->GetNParameters() != syst2->GetNParameters()){
    std::cerr << "ERROR: you can't correlate the diagonal of 2 systematics if they don't have the same number of parameters:" << std::endl;
    std::cerr << "ERROR: Systematics " << syst1->Name() << " and " << syst2->Name() << " don't have the same number of parameters (" << syst1->GetNParameters()<<", "<< syst2->GetNParameters() << ")" << std::endl;
    std::cerr << "ERROR: Exiting" << std::endl;
    throw;
  }else{
    for(UInt_t i=0; i<syst1->GetNParameters(); ++i){
      UInt_t index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),i)];
      UInt_t index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),i)];
      FillCorrelationInMatrix(index1, index2);
    }

  }

};


//********************************************
void ToyMakerCorrelatedSyst::CorrelateAllParametersInSystematic(SystematicBase* syst){
//********************************************

  for(UInt_t i=0; i<syst->GetNParameters(); ++i){
    for(UInt_t j=0; j<syst->GetNParameters(); ++j){
      UInt_t index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst->GetIndex(),i)];
      UInt_t index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst->GetIndex(),j)];
      FillCorrelationInMatrix(index1, index2);
    }
  }

};


//********************************************
void ToyMakerCorrelatedSyst::CorrelateParametersSystematic(SystematicBase* syst1, UInt_t param1, SystematicBase* syst2, UInt_t param2){
//********************************************

  UInt_t index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),param1)];
  UInt_t index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),param2)];
  FillCorrelationInMatrix(index1, index2);

  index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),param2)];
  index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),param1)];
  FillCorrelationInMatrix(index1, index2);
};

//********************************************
void ToyMakerCorrelatedSyst::UnCorrelateParametersSystematic(SystematicBase* syst1, UInt_t param1, SystematicBase* syst2, UInt_t param2){
//********************************************

  UInt_t index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),param1)];
  UInt_t index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),param2)];
  DeCorrelateInMatrix(index1, index2);
  index1 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst1->GetIndex(),param2)];
  index2 = _map_systematic_param[SystematicParameter((SystId::SystEnum)syst2->GetIndex(),param1)];
  DeCorrelateInMatrix(index1, index2);

};


//********************************************
void ToyMakerCorrelatedSyst::FillToyExperiment(ToyExperiment& toy){
//********************************************

  TVectorT<double> vec(_correlation_matrix->GetNcols());

  for(Int_t isystpar = 0; isystpar < _correlation_matrix->GetNcols(); ++isystpar){
    vec[isystpar] = _RandomGenerator->Gaus(0.,1.);
  }

  vec *= (*_inverted_correlation_matrix);

  Float_t weight = 1.;

  for (UInt_t isyst = 0; isyst < NMAXSYSTEMATICS; isyst++){
    SystematicBase* syst = _systematics[isyst];
    if (!syst) continue;

    // Create the proper structure for the ToyExperiment adding each of the systematics
    toy.AddToyVariation(syst->GetIndex(), syst->GetNParameters());

    // Loop over parameters for this systematic
    for (UInt_t ipar = 0; ipar < syst->GetNParameters(); ipar++){
      Float_t var = 0;
      SystematicParameter s((SystId::SystEnum)syst->GetIndex(),ipar);
      UInt_t globalpar = _map_systematic_param[s];
      if(globalpar < 0 || globalpar >  _correlation_matrix->GetNcols()){
        std::cerr << "out of range!!"  << std::endl;
        throw;
      }
      // When the option _zero_var is enabled all variations are 0
      if   (syst->PDF() == SystematicBase::kGaussian) var = vec[globalpar];
      else{
        // ignore the correlation for the uniform or any other form of distirbution
        if(syst->PDF() == SystematicBase::kUniform) var = _RandomGenerator->Uniform(0.,1.);
        else{
          std::cerr << "Systematic " <<  syst->Name() << " has a pdf that isn't supported by this ToyMakerCorrelatedSyst, you should code it in!" << std::endl;
          throw;
        }
      }
      toy.SetToyVariation(syst->GetIndex(), ipar, var, weight);
    }
  }

};


//********************************************
void ToyMakerCorrelatedSyst::SaveCorrelationMatrixTH2D(std::string filename){
//********************************************

  TFile* file = new TFile(filename.c_str(), "RECREATE");
  TH2D* corr = NULL;
  TH2D* inv  = NULL;
  file->cd();

  if(_correlation_matrix){
    corr = new TH2D("corr", "corr", _mat_size, 0, _mat_size, _mat_size, 0, _mat_size);
    for(size_t i = 0; i < _mat_size; ++i){
      for(size_t j = 0; j < _mat_size; ++j){
        corr->SetBinContent(i+1, j+1, (*_correlation_matrix)[i][j]);
      }
    }
    corr->Write();
  }

  if(_inverted_correlation_matrix){
    inv  = new TH2D("inv",  "inv", _mat_size, 0, _mat_size, _mat_size, 0, _mat_size);
    for(size_t i = 0; i < _mat_size; ++i){
      for(size_t j = 0; j < _mat_size; ++j){
        inv->SetBinContent(i+1, j+1, (*_inverted_correlation_matrix)[i][j]);
      }
    }
    inv->Write();
  }

  file->Close();
  // This already happens when you close the file??
  //  if(corr) delete corr;
  //  if(inv ) delete inv ;
}


