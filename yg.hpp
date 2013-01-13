#ifndef _YG_HPP_
#define _YG_HPP_

#include <iostream>
#include <Eigen/Dense>
#include <string>
using namespace Eigen;
using namespace std;

#define EA_COEF_LEN 6

enum Melement {
  Mg, // assigned 0
  Ca, // assigned 1
  Sr, // assigned 2
  Ba, // assigned 3
};

static const string MelementStrings[]={"Mg", "Ca", "Sr", "Ba"};
static const double MelementMmw[]={24.305, 40.08,87.62, 137.328};

class EA {
  double _Mmw, _MFe, _perC, _perH, _perN, _perF;
  Melement _me;
  bool _norm;
  string _Mname;
  vector<string> _molNames;
  vector<double> _coefs;
public:
  EA();
  EA(double, double, 
     double, double, double, double, bool);
  EA(Melement, double, 
     double, double, double, double, bool);
  void setValues(double, double, 
     double, double, double, double, bool);
  void setValues(Melement, double, 
     double, double, double, double, bool);
  void setMname(const string);
  void solve();
  vector<double> coefs();
  vector<string> molNames();
  void printCoefs();
  void htmlPrint();
};

void EA::setValues(double Mmw, double MFe,
       double perC, double perH, double perN, double perF,
       bool norm) {
  _Mmw=Mmw;
  setMname("M");
  _MFe=MFe;
  _perC=perC;
  _perH=perH;
  _perN=perN;
  _perF=perF;
  _norm=norm;
}

void EA::setValues(Melement m, double MFe,
       double perC, double perH, double perN, double perF,
       bool norm) {
  _Mmw=MelementMmw[m];
  setMname(MelementStrings[m]);
  _MFe=MFe;
  _perC=perC;
  _perH=perH;
  _perN=perN;
  _perF=perF;
  _norm=norm;
}

EA::EA() {}

EA::EA(double Mmw, double MFe,
       double perC, double perH, double perN, double perF,
       bool norm) {
  setValues(Mmw, MFe, perC, perH, perN, perF, norm);
}

EA::EA(Melement m, double MFe,
       double perC, double perH, double perN, double perF,
       bool norm) {
  setValues(m, MFe, perC, perH, perN, perF, norm);
}

void EA::setMname(const string m) {
  if(_molNames.size()!=EA_COEF_LEN)
    _molNames.resize(EA_COEF_LEN);
  _Mname=m;
  _molNames.at(0)="FeF3";
  _molNames.at(1)="Fe(OH)3";
  _molNames.at(2)="Fe(NO3)3";
  _molNames.at(3)=_Mname + "F2";
  _molNames.at(4)=_Mname + "(OAc)2";
  _molNames.at(5)="H2O";
}

void EA::solve() {
  MatrixXf A(EA_COEF_LEN,EA_COEF_LEN);
  VectorXf b(EA_COEF_LEN);

  A << 112.8, 106.8, 241.8, 38+_Mmw, 118+_Mmw, 18.0,	\
    0.0, 0.0, 0.0, 0.0, 48.0, 0.0,			\
    0.0, 3.0, 0.0, 0.0, 6.0, 2.0,			\
    0.0, 0.0, 42.0, 0.0, 0.0, 0.0,			\
    57.0, 0.0, 0.0, 38.0, 0.0, 0.0,			\
    _MFe, _MFe, _MFe, -1.0, -1.0, 0.0;
  b << 100, _perC, _perH, _perN, _perF, 0;
  VectorXf x(EA_COEF_LEN);
  x=A.colPivHouseholderQr().solve(b);
  if(_norm) {
    double fesum=x[0]+x[1]+x[2];
    x = x / fesum;
  }
  _coefs.resize(EA_COEF_LEN);
  /* Question: is there a way to save the memory copying? */
  for(unsigned int i=0;i<EA_COEF_LEN;i++)
    _coefs.at(i)=x[i];
}
vector<double> EA::coefs() {
  return(_coefs);
}

vector<string> EA::molNames() {
  return(_molNames);
}

void EA::printCoefs() {
  for(unsigned int i=0; i< EA_COEF_LEN; i++)
    cout << _molNames.at(i) << "\t" << _coefs.at(i) << endl;
}

void EA::htmlPrint() {
  cout << "<table border=\"1\">" << endl;
  for(unsigned int i=0; i<EA_COEF_LEN; i++) {
    cout << "<tr>" << endl;
    cout << "<td>" <<  _molNames.at(i) << "</td>" << endl;
    cout << "<td>" << _coefs.at(i) << "</td>" << endl;
    cout << "</tr>" << endl;
  }
  cout << "</table>" << endl;
}
#endif
