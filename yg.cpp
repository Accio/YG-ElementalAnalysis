#include <iostream>
#include <Eigen/Dense>
#include <string>
#include <boost/program_options.hpp>

namespace po = boost::program_options;
using namespace std;
using namespace Eigen;

VectorXf calculate(MatrixXf A, VectorXf b) {
  VectorXf x(6);

  x=A.colPivHouseholderQr().solve(b);
  return(x);
}

int main(int argc, char** argv) {
  MatrixXf A(6,6);
  VectorXf b(6);
  VectorXf x(6);

  double Mmw=24.3; // Mg
  double MFe=2; // M-Fe ratio
  double perC=0.2;
  double perH=1.6;
  double perN=1.2;
  double perF=38.3;
  char* optarg;
  int verbose=0;

  try {
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help", "produce help message")
      ("verbose", "verbose")
      ("m", po::value<double>(), "Molecular weight of M")
      ("r", po::value<double>(),"M-Fe Ratio")
      ("c", po::value<double>(),"Percentage of C")
      ("h", po::value<double>(),"Percentage of H")
      ("n", po::value<double>(),"Percentage of N")
      ("f", po::value<double>(),"Percentage of F")
      ;
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if(vm.count("help")) {
      cout << desc << "\n";
      return 0;
    }
    if(vm.count("verbose")) {
      verbose=1;
    }
    if(vm.count("m")) {
      MFe=vm["m"].as<double>();
    }
    if(vm.count("r")) {
      MFe=vm["r"].as<double>();
    }
    if(vm.count("c")) {
      perC=vm["c"].as<double>();
    }
    if(vm.count("h")) {
      perH=vm["h"].as<double>();
    }
    if(vm.count("n")) {
      perN=vm["n"].as<double>();
    }
    if(vm.count("f")) {
      perF=vm["f"].as<double>();
    }
  } catch(exception& e) {
    cerr << "error: " << e.what() << "\n";
    return 1;
  }
  catch(...) {
    cerr << "Exception of unknown type!\n";
  }

  if(verbose) {
    cout<<"==================== Input ====================" << endl;
    cout << "Molecular weight of M is set to " << Mmw << ".\n";
    cout << "M-Fe ratio is set to " << MFe << ".\n";
    cout << "C% set to " << perC << ".\n";
    cout << "H% set to " << perH << ".\n";
    cout << "N% set to " << perN << ".\n";
    cout << "F% set to " << perF << ".\n";
  }
  A << 112.8, 106.8, 241.8, 38+Mmw, 118+Mmw, 18.0,\
    0.0, 0.0, 0.0, 0.0, 48.0, 0.0,\
    0.0, 3.0, 0.0, 0.0, 6.0, 2.0,\
    0.0, 0.0, 42.0, 0.0, 0.0, 0.0,\
    57.0, 0.0, 0.0, 38.0, 0.0, 0.0,\
    MFe, MFe, MFe, -1.0, -1.0, 0.0;
  b << 100, perC, perH, perN, perF, 0;
  if(verbose) {
    cout<<"==================== Model ====================" << endl;
    cout << "Here is the matrix A:\n" << A << endl;
    cout << "Here is the vector b:\n" << b << endl;
    cout<<"==================== Output ====================" << endl;
  }
  x=calculate(A,b);
  cout << "FeF3" << "\t" << x[0] << endl;
  cout << "Fe(OH)3" << "\t" << x[1] << endl;
  cout << "Fe(NO3)3" << "\t" << x[2] << endl;
  cout << "MgF2" << "\t" << x[3] << endl;
  cout << "Mg(OAc)2" << "\t" << x[4] << endl;
  cout << "H2O" << "\t" << x[5] << endl;
  return(0);
}
