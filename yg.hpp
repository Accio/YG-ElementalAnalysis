#ifndef _YG_HPP_
#define _YG_HPP_

#include <iostream>
#include <Eigen/Dense>
#include <string>
using namespace Eigen;
using namespace std;

#define EA_COEF_LEN 6
#define WOFE_EA_COEF_LEN 3

enum Melement {
  Mg, // assigned 0
  Ca, // assigned 1
  Sr, // assigned 2
  Ba, // assigned 3
  Zn, // assigned 4
};

static const string MelementStrings[]={"Mg", "Ca", "Sr", "Ba", "Zn"};
static const double MelementMmw[]={24.305, 40.08,87.62, 137.328, 65.382};

class EA {
  double _Mmw, _MFe, _perC, _perH, _perN, _perF;
  Melement _me;
  bool _norm;
  bool _hasFe;
  string _Mname;
  vector<string> _molNames;
  vector<double> _coefs;
public:
  EA();
  EA(Melement, double, 
     double, double, double, double, bool);
  EA(Melement, double, double, bool);

  void setValues(Melement, double, 
     double, double, double, double, bool);
  void setValues(Melement, double, double, bool);

  void setMname(const string);
  void solve();
  vector<double> coefs();
  vector<string> molNames();
  void print();
  void printCoefs();
  void htmlPrint();
};

void EA::setValues(Melement m, double MFe,
       double perC, double perH, double perN, double perF,
       bool norm) {
  _Mmw=MelementMmw[m];
  _hasFe=true;
  _MFe=MFe;
  _perC=perC;
  _perH=perH;
  _perN=perN;
  _perF=perF;
  _norm=norm;
  setMname(MelementStrings[m]);
}

void EA::setValues(Melement m,  double perC, double perF, bool norm) {
  _Mmw=MelementMmw[m];
  _hasFe=false;
  _MFe=0;
  _perC=perC;
  _perH=0;
  _perN=0;
  _perF=perF;
  _norm=norm;
  setMname(MelementStrings[m]);
}

EA::EA() {}

EA::EA(Melement m, double MFe,
       double perC, double perH, double perN, double perF,
       bool norm) {
  setValues(m, MFe, perC, perH, perN, perF, norm);
}

EA::EA(Melement m,double perC, double perF, bool norm) {
  setValues(m, perC, perF, norm);
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

  unsigned int i;
  _coefs.resize(EA_COEF_LEN);
  MatrixXf A(EA_COEF_LEN,EA_COEF_LEN);
  VectorXf b(EA_COEF_LEN);
  VectorXf x(EA_COEF_LEN);

  if(_hasFe) {
    A << 112.8, 106.8, 241.8, 38+_Mmw, 118+_Mmw, 18.0,	\
      0.0, 0.0, 0.0, 0.0, 48.0, 0.0,			\
      0.0, 3.0, 0.0, 0.0, 6.0, 2.0,			\
      0.0, 0.0, 42.0, 0.0, 0.0, 0.0,			\
      57.0, 0.0, 0.0, 38.0, 0.0, 0.0,			\
      _MFe, _MFe, _MFe, -1.0, -1.0, 0.0;
    b << 100, _perC, _perH, _perN, _perF, 0;
  } else {
    A.resize(3, 3);
    b.resize(3);
    x.resize(3);
    A << 38+_Mmw, 118+_Mmw, 18.0,	\
      0.0, 48.0, 0.0,			\
      38.0, 0.0, 0.0;
    b << 100,_perC, _perF;
  } 

  x=A.colPivHouseholderQr().solve(b);


  if(_norm) {
    double fesum = _hasFe ? x[0]+x[1]+x[2] : x[0]+x[1];
    x = x / fesum;
  }

  /* Question: is there a way to save the memory copying? */
  if(_hasFe) {
    for(i=0;i<EA_COEF_LEN;i++)
      _coefs.at(i)=x[i];
  } else {
    for(i=0; i<3; i++) _coefs.at(i)=0;
    for(i=3; i<6; i++)
      _coefs.at(i)=x[i-3];
  }
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

void EA::print() {
  cout << "M=" << _Mname << endl
       << "Has Fe=" << _hasFe << endl
       << "M/Fe=" << _MFe << endl
       << "perC=" << _perC << endl
       << "perH=" << _perH << endl
       << "perN=" << _perN << endl
       << "perF=" << _perF << endl;
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
