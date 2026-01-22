#include <TMB.hpp>

using namespace tmbutils;

array<int> asIArray(SEXP x)
{
  if(!Rf_isArray(x))Rf_error("NOT AN ARRAY!");
  SEXP dim=Rf_getAttrib(x,R_DimSymbol);
  vector<int> d=asVector<int,int>(INTEGER(dim), LENGTH(dim));
  vector<int> y=asVector<int,int>(INTEGER(x), LENGTH(x));
  return array<int>(y,d);
}


// From kaskr https://github.com/kaskr/adcomp/commit/5a6df30625f2e3c0579e983c4829b381cfd1df96
/** \brief Helper class to handle NA integers passed from R */
struct Rint {
  int i;
  inline Rint () { }
  inline Rint (double x) : i(IntegerFromReal(x)) { }
  inline operator int() const { return i; }
  // From src/main/coerce.c
  inline int IntegerFromReal(double x) {
    using std::isnan;
    if (ISNAN(x))
      return NA_INTEGER;
    else if (x >= INT_MAX+1. || x <= INT_MIN ) {
      return NA_INTEGER;
    }
    return (int) x;
  }
};

template<>
inline vector<int> asVector(SEXP x) {
  return asVector<Rint>(x).cast<int>();
}

template<>
inline matrix<int> asMatrix(SEXP x) {
  return asMatrix<Rint>(x).cast<int>();
}

namespace tmbutils {
template<>
inline array<int> asArray(SEXP x) {
  array<Rint> tmp = asArray<Rint>(x);
  return array<int>(tmp.vectorcopy.cast<int>(), tmp.dim);
}
}

#define DATA_INTEGER2(name) int name(asVector<Rint>(     \
getListElement(TMB_OBJECTIVE_PTR -> data,               \
#name, &isNumericScalar))[0]);
////



template<class Type>
Type objective_function<Type>::operator() ()
{
  DATA_INTEGER(a);
  Rcout << a << "\n";
  DATA_INTEGER2(a2);
  DATA_IVECTOR(b);
  Rcout << b(0) << "\n";
  DATA_IMATRIX(d);
  Rcout << d(0,0) << "\n";
  DATA_IARRAY(e);
  Rcout << e[0] << "\n";
  PARAMETER(x);
  return x;
};

extern "C" {

  double RealFromInteger(int x) {
    return (x == NA_INTEGER) ? NA_REAL : x;
  }

  
  SEXP testValues(SEXP x){

    // Test1
    // array<int> test_v_1(asArray<int>(getListElement(x, "vec", &Rf_isReal   )));
    // Rcout << test_v_1(0,0) << "\n";
    // array<int> test_v_2(asArray<int>(getListElement(x, "vec", &Rf_isInteger   )));
    // Rcout << test_v_2(0,0) << "\n";
    Rcout << INTEGER(getListElement(x, "vec", &Rf_isNumeric   ))[0] << "\n";  
    Rcout << REAL(Rf_coerceVector(getListElement(x, "vec", &Rf_isNumeric   ),REALSXP))[0] << "\n";
    Rcout << (int)REAL(Rf_coerceVector(getListElement(x, "vec", &Rf_isNumeric   ),REALSXP))[0] << "\n";
    Rcout << RealFromInteger(NA_INTEGER) << ", " << RealFromInteger(INTEGER(getListElement(x, "vec", &Rf_isNumeric   ))[0]) << "\n";
    //array<int> test_v_3(asArray<int>(Rf_coerceVector(getListElement(x, "vec", &Rf_isNumeric   ),REALSXP)));
    //Rcout << test_v_3(0,0) << "\n";
    // array<int> test_v_4(asArray<int>(getListElement(x, "vec", &Rf_isNumeric   )));
    // Rcout << test_v_4(0,0) << "\n";

    //array<int> test_v_5(asIArray(getListElement(x, "vec", &Rf_isInteger   )));

    //Rcout << test_v_5(0,0) << "\n";

    Rcout << CppAD::Integer(NA_REAL) << "\n";

    Rcout << NA_INTEGER << ", " << (double)NA_INTEGER << ", " << (int)((double)NA_INTEGER) << "\n";
    Rcout << NA_REAL << ", " << (int)NA_REAL << ", " << (double)((int)NA_REAL) << "\n";

int test98 = asVector<int>(getListElement(x,"vec", &isNumeric))[0];
    Rcout << test98 << "\n";
    
int test99 = asVector<Rint>(getListElement(x,"vec", &isNumeric))[0];
    Rcout << test99 << "\n";
    
    return R_NilValue;
    
  }
  
  
#define CALLDEF(name,n) {#name, (DL_FUNC) &name, n}

  static const R_CallMethodDef CallEntries[] = {
    TMB_CALLDEFS,
    CALLDEF(testValues,1),
    {NULL, NULL, 0}
  };

#define CALLABLE(name) R_RegisterCCallable("testIntNa", #name, (DL_FUNC) &name)

  void R_init_testIntNa(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    TMB_CCALLABLES("testIntNa");
  }

}
