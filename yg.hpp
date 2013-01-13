#ifndef _YG_HPP_
#define _YG_HPP_

#include <iostream>
#include <Eigen/Dense>
using namespace Eigen;
using namespace std;

class EA {
  double _Mmw, _MFe, _perC, _perH, _perN, _perF;
  bool _norm;
  double _coefs[5];
 public:
  EA(double, double, 
     double, double, double, double, bool);
  void solve();
  double* coefs();
  void printCoefs();
};

EA::EA(double Mmw, double MFe,
       double perC, double perH, double perN, double perF,
       bool norm) {
  _Mmw=Mmw;
  _MFe=MFe;
  _perC=perC;
  _perH=perH;
  _perN=perN;
  _perF=perF;
  _norm=norm;
}

void EA::solve() {
  MatrixXf A(6,6);
  VectorXf b(6);

  A << 112.8, 106.8, 241.8, 38+_Mmw, 118+_Mmw, 18.0,	\
    0.0, 0.0, 0.0, 0.0, 48.0, 0.0,			\
    0.0, 3.0, 0.0, 0.0, 6.0, 2.0,			\
    0.0, 0.0, 42.0, 0.0, 0.0, 0.0,			\
    57.0, 0.0, 0.0, 38.0, 0.0, 0.0,			\
    _MFe, _MFe, _MFe, -1.0, -1.0, 0.0;
  b << 100, _perC, _perH, _perN, _perF, 0;
  VectorXf x(6);
  x=A.colPivHouseholderQr().solve(b);
  if(_norm) {
    double fesum=x[0]+x[1]+x[2];
    x = x / fesum;
  }
  for(int i=0;i<6;i++)
    _coefs[i]=x[i];
}
double* EA::coefs() {
  return(_coefs);
}

void EA::printCoefs() {
  cout << "FeF3" << "\t" << _coefs[0] << endl;
  cout << "Fe(OH)3" << "\t" << _coefs[1] << endl;
  cout << "Fe(NO3)3" << "\t" << _coefs[2] << endl;
  cout << "MF2" << "\t" << _coefs[3] << endl;
  cout << "M(OAc)2" << "\t" << _coefs[4] << endl;
  cout << "H2O" << "\t" << _coefs[5] << endl;
}

#endif
