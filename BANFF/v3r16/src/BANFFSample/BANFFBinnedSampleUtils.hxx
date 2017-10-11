#ifndef BANFFBINNEDSAMPLEUTILS_HXX
#define BANFFBINNEDSAMPLEUTILS_HXX
#include "THn.h"

///This namespace is primarily for containing functions to deal with THn
///histograms in more convenient way.
namespace BANFFBinnedSampleUtils{

  ///Returns the number of bins in the histogram, omitting overflow and
  ///underflow bins.
  int GetNBins(THn* hist);

  ///Returns an array with the bin numbers of the bins in the histogram that
  ///are not overflow or underflow bins.
  int* GetBins(THn* hist);

}

#endif
