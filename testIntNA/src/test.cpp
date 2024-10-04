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

template<class Type>
Type objective_function<Type>::operator() ()
{
  return 0;
};

extern "C" {

  SEXP testValues(SEXP x){

    // Test1
    // array<int> test_v_1(asArray<int>(getListElement(x, "vec", &Rf_isReal   )));
    // Rcout << test_v_1(0,0) << "\n";
    // array<int> test_v_2(asArray<int>(getListElement(x, "vec", &Rf_isInteger   )));
    // Rcout << test_v_2(0,0) << "\n";
    Rcout << INTEGER(getListElement(x, "vec", &Rf_isNumeric   ))[0] << "\n";
    Rcout << REAL(Rf_coerceVector(getListElement(x, "vec", &Rf_isNumeric   ),REALSXP))[0] << "\n";
    array<int> test_v_3(asArray<int>(Rf_coerceVector(getListElement(x, "vec", &Rf_isNumeric   ),REALSXP)));
    Rcout << test_v_3(0,0) << "\n";
    // array<int> test_v_4(asArray<int>(getListElement(x, "vec", &Rf_isNumeric   )));
    // Rcout << test_v_4(0,0) << "\n";

    array<int> test_v_5(asIArray(getListElement(x, "vec", &Rf_isInteger   )));

    Rcout << test_v_5(0,0) << "\n";

    Rcout << NA_INTEGER << ", " << (double)NA_INTEGER << ", " << (int)((double)NA_INTEGER) << "\n";

    
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
