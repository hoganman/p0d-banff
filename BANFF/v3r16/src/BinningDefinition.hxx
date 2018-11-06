#include "SampleId.hxx"
#include "Parameters.hxx"

#include "TAxis.h"

#include <map>
#include <algorithm>

namespace BANFF
{
    class TAxis2D
    {
        public:
            TAxis2D(int nbin1, double* bins1,int nbin2, double* bins2);
            ~TAxis2D();
            TAxis* operator[] (const unsigned int &d);
        private:
            TAxis* FirstDim;
            TAxis* SecondDim;
    };


    class BinningDefinition
    {

    private:
        BinningDefinition();
        BinningDefinition(const BinningDefinition& binning);
        static BinningDefinition* fBinningDefinition;

    public:
        ~BinningDefinition();

    private:

        typedef std::map<SampleId::SampleEnum, TAxis*>::const_iterator SampleTAxisCIterator;
        typedef std::map<SampleId::SampleEnum, TAxis*>::iterator SampleTAxisIterator;

        std::map<SampleId::SampleEnum, bool> ActiveSample;
        std::map<SampleId::SampleEnum, TAxis*>   Axis_Mom;
        std::map<SampleId::SampleEnum, TAxis*>   Axis_Mom_Det;
        std::map<SampleId::SampleEnum, TAxis*>   Axis_Cos;
        std::map<SampleId::SampleEnum, TAxis*>   Axis_Cos_Det;
        std::map<SampleId::SampleEnum, TAxis2D*> bothAxis;
        std::map<SampleId::SampleEnum, TAxis2D*> bothAxis_Det;

        std::map<int, int> FullToReduMap;

        bool Do4PiFHC;
        bool DoNue;
        bool DoOnlyNue;
        bool DoMultiPiRHC;

        bool Do1DCheckMom;
        bool Do1DCheckCos;
        bool UseFlatNuMuBinning;

    public:

        bool IsActiveSample(const SampleId::SampleEnum &sample) const;
        bool IsActiveSample(const int &sample) const;
        int GetNbins_Mom(const SampleId::SampleEnum &sample) const;
        int GetNbins_Mom_Det(const SampleId::SampleEnum &sample) const;
        int GetNbins_Cos(const SampleId::SampleEnum &sample) const;
        int GetNbins_Cos_Det(const SampleId::SampleEnum &sample) const;
        int GetGlobalBinMatrix_Det(const SampleId::SampleEnum &sample) const;
        int GetGlobalBinMatrix(const SampleId::SampleEnum &sample) const;
        void GetLocalBinMatrixMomCos(const int &GlobalBin, SampleId::SampleEnum& Sample, int& MomBin, int& CosBin);
        void GetLocalBinMatrixMomCos_Det(const int &GlobalBin, SampleId::SampleEnum& Sample, int& MomBin, int& CosBin);
        int GetNbins_Det() const;
        int GetNbins() const;
        int GetNbins_Det(const SampleId::SampleEnum &sample) const;
        int GetNbins(const SampleId::SampleEnum &sample) const;
        int GetGlobalBinMatrixMomCos_Det(const SampleId::SampleEnum &sample, const double Mom, const double Cos);
        int GetGlobalBinMatrixMomCos(const SampleId::SampleEnum &sample, const double Mom, const double Cos);
        TAxis* GetBinning_Mom(const SampleId::SampleEnum &sample) const;
        TAxis* GetBinning_Mom_Det(const SampleId::SampleEnum &sample) const;
        TAxis* GetBinning_Cos(const SampleId::SampleEnum &sample) const;
        TAxis* GetBinning_Cos_Det(const SampleId::SampleEnum &sample) const;
        TAxis2D* GetBinningArray(const SampleId::SampleEnum &sample) const;
        SampleId::SampleEnum GetSampleFromIndex(const int &index) const;
        SampleId::SampleEnum GetSampleFromIndex_Det(const int &index) const;
        std::vector<std::string> GetListOfBins();
        std::vector<std::string> GetListOfBins_Det();
        std::map<int,int> GetFullToReduMap();
        static BANFF::BinningDefinition& Get();

    };
}
