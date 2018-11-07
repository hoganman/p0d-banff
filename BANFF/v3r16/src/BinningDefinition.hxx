#ifndef BINNINGDEFINITION_HXX
#define BINNINGDEFINITION_HXX

#include "SampleId.hxx"
#include "Parameters.hxx"

#include "TAxis.h"

#include <map>
#include <algorithm>

namespace BANFF
{
    class TAxis2D
    {
    ///Store Momentum and Costheta TAxis*. You can think of this as a array of
    ///TAxis*, of size 2
    public:
        ///Constructor
        TAxis2D(int nbin1, double* bins1,int nbin2, double* bins2);

        ///Destructor
        virtual ~TAxis2D();

        ///[] operator, 0 returns FirstDim, 1 returns SecondDim, else throw
        TAxis* operator[] (const unsigned int &d);

    private:
        ///Usually Momentum TAxis
        TAxis* FirstDim;

        ///Usually Costheta TAxis
        TAxis* SecondDim;

        ///Size array for storing axes
        TAxis** Array;

        ///Defining the number of elements in Array
        static const UInt_t ArraySize = 2;
    };


    class BinningDefinition
    {
    ///Store a mapping between SampleId::SampleEnum and p-theta binning
    ///for bin correlations and BANFF fitting

    private:
        ///Constructor
        BinningDefinition();

        ///Copy constructor
        BinningDefinition(const BinningDefinition& binning);

        ///Singleton member
        static BinningDefinition* fBinningDefinition;

    public:
        ///Destructor
        ~BinningDefinition();

        ///Singleton Get method
        static BANFF::BinningDefinition& Get();

    public:
        ///Return the product of Momentum x Costheta Det bins
        int  GetNbins_Det() const;

        ///Return the product of Momentum x Costheta bins
        int  GetNbins() const;

        ///Get the SampleId corresponding to sub-matrix index
        SampleId::SampleEnum GetSampleFromIndex(const int &index) const;

        ///Get the SampleId corresponding to Det sub-matrix index
        SampleId::SampleEnum GetSampleFromIndex_Det(const int &index) const;

        ///Is the SampleId set to active in BANFF.parameters
        bool IsActiveSample(const int &sample) const;

        ///Is the SampleId set to active in BANFF.parameters
        bool IsActiveSample(const SampleId::SampleEnum &sample) const;

        ///Return the product of Momentum x Costheta bins for a given SampleId
        int  GetNbins(const SampleId::SampleEnum &sample) const;

        ///Return the product of Momentum Det x Costheta Det bins for a given
        ///SampleId
        int  GetNbins_Det(const SampleId::SampleEnum &sample) const;

        ///Return the number of Momentum bins for a given SampleId
        int  GetNbins_Mom(const SampleId::SampleEnum &sample) const;

        ///Return the number of Momentum Det bins for a given SampleId
        int  GetNbins_Mom_Det(const SampleId::SampleEnum &sample) const;

        ///Return the number of Costheta bins for a given SampleId
        int  GetNbins_Cos(const SampleId::SampleEnum &sample) const;

        ///Return the number of Costheta Det bins for a given SampleId
        int  GetNbins_Cos_Det(const SampleId::SampleEnum &sample) const;

        ///Get the momentum TAxis for a given SampleId
        TAxis* GetBinning_Mom(const SampleId::SampleEnum &sample) const;

        ///Get the momentum Det TAxis for a given SampleId
        TAxis* GetBinning_Mom_Det(const SampleId::SampleEnum &sample) const;

        ///Get the Costheta TAxis for a given SampleId
        TAxis* GetBinning_Cos(const SampleId::SampleEnum &sample) const;

        ///Get the Costheta Det TAxis for a given SampleId
        TAxis* GetBinning_Cos_Det(const SampleId::SampleEnum &sample) const;

        ///Get the Momentum+Costheta TAxis2D* for a given SampleId
        TAxis2D* GetBinningArray(const SampleId::SampleEnum &sample) const;

        ///Return the global Det bin index for a given SampleId
        int GetGlobalBinMatrix_Det(const SampleId::SampleEnum &sample) const;

        ///Return the global bin index for a given SampleId
        int GetGlobalBinMatrix(const SampleId::SampleEnum &sample) const;

        ///Return the global Det bin index for a given SampleId, momentum,
        ///and Costheta
        int GetGlobalBinMatrixMomCos_Det(const SampleId::SampleEnum &sample,
                const double &Mom, const double &Cos);

        ///Return the global bin index for a given SampleId, momentum, and
        ///Costheta
        int GetGlobalBinMatrixMomCos(const SampleId::SampleEnum &sample,
                const double &Mom, const double &Cos);

        ///All the bins and SampleIds in the matrix
        std::vector<std::string> GetListOfBins() const;

        ///All the bins and SampleIds in the Det matrix
        std::vector<std::string> GetListOfBins_Det() const;

        ///Return a mapping between the full matrix to the reduced Det matrix
        std::map<int,int> GetFullToReduMap();

        ///For a global bin index, extract the SampleId with Momentum bin and
        ///Costheta bin indices
        void GetLocalBinMatrixMomCos(const int &GlobalBin,
                SampleId::SampleEnum& Sample, int& MomBin, int& CosBin);

        ///For a global Det bin index, extract the SampleId with Momentum bin
        ///and Costheta bin indices
        void GetLocalBinMatrixMomCos_Det(const int &GlobalBin,
                SampleId::SampleEnum& Sample, int& MomBin, int& CosBin);

    private:

        ///Keeps a record of which samples are enabled
        std::map<SampleId::SampleEnum, bool> ActiveSample;

        ///Stores the Momentum axis for each sample
        std::map<SampleId::SampleEnum, TAxis*>   Axis_Mom;

        ///Stores the Momentum Det axis for each sample
        std::map<SampleId::SampleEnum, TAxis*>   Axis_Mom_Det;

        ///Stores the Costheta axis for each sample
        std::map<SampleId::SampleEnum, TAxis*>   Axis_Cos;

        ///Stores the Costheta Det axis for each sample
        std::map<SampleId::SampleEnum, TAxis*>   Axis_Cos_Det;

        ///Stores both the Momentum and Costheta axes for each sample
        std::map<SampleId::SampleEnum, TAxis2D*> bothAxis;

        ///Stores both the Momentum Det and Costheta Det axes for each sample
        std::map<SampleId::SampleEnum, TAxis2D*> bothAxis_Det;

        ///Typedefing since it is tedious to type/read the full expression
        typedef std::map<SampleId::SampleEnum, TAxis*> SampleEnumToTAxisMap_t;

        ///A mapping between the full matrix to the reduced Det matrix
        std::map<int, int> FullToReduMap;

        ///Parameter defined in BANFF/parameters/BANFF.parameters.dat or
        ///override file to run on 4 Pi, FHC samples
        bool Do4PiFHC;

        ///Parameter defined in BANFF/parameters/BANFF.parameters.dat or
        ///override file to run NuE samples
        bool DoNue;

        ///Parameter defined in BANFF/parameters/BANFF.parameters.dat
        ///override file to run ONLY NuE samples
        bool DoOnlyNue;

        ///Parameter defined in BANFF/parameters/BANFF.parameters.dat or
        ///override file to run on Multi-pi in RHC
        bool DoMultiPiRHC;

        ///Parameter defined in BANFF/parameters/BANFF.parameters.dat or
        ///override file to run diagnostic check on Momentum
        bool Do1DCheckMom;

        ///Parameter defined in BANFF/parameters/BANFF.parameters.dat or
        ///override file to run diagnostic check on Costheta
        bool Do1DCheckCos;

        ///Parameter defined in BANFF/parameters/BANFF.parameters.dat or
        ///override file to run in p-theta configuration where Momentum
        ///is evenly spaced (250 MeV/c) and a single Costheta bin (-1,1)
        bool UseFlatNuMuBinning;

    };
}

#endif
