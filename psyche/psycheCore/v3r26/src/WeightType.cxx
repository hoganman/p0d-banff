#include "WeightType.hxx"

WeightType operator -(Float_t a, const WeightType& w){return WeightType(a-w.Correction, a-w.Systematic);}
WeightType operator *(Float_t a, const WeightType& w){return WeightType(a*w.Correction, a*w.Systematic);}

std::ostream& operator<<(std::ostream &os, const WeightType& w){
  size_t p = os.precision(9);
  os << "(" << w.Correction << ", " << w.Systematic << ")";
  os.precision(p);
  return os;
}
