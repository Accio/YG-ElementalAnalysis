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
  
  double Mmw, MFe, perC, perH, perN, perF;


  int verbose=0;
  int norm=1;

  try {
    po::options_description general("General options");
    general.add_options()
      ("help", "produce help message")
      ("test", "Run with a test dataset")
      ("verbose", "verbose");
    
    po::options_description essential("Essential parameters");
    essential.add_options()
      ("m", po::value<double>(), "Molecular weight of M")
      ("r", po::value<double>(),"M-Fe Ratio")
      ("c", po::value<double>(),"Percentage of C")
      ("h", po::value<double>(),"Percentage of H")
      ("n", po::value<double>(),"Percentage of N")
      ("f", po::value<double>(),"Percentage of F")
      ;
    
    po::options_description optional("Optional");
    optional.add_options()
      ("noNorm", "If set, the sum of Fe is *NOT* normalised to 1");

    po::options_description desc("Allowed options");
    desc.add(general).add(essential).add(optional);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if(vm.count("help") || argc==1) {
      cout << desc << "\n";
      return 0;
    }
    if(vm.count("verbose")) {
      verbose=1;
    }
    if(vm.count("test")) {
      Mmw=24.3; // Mg
      MFe=2; // M-Fe ratio
      perC=0.2;
      perH=1.6;
      perN=1.2;
      perF=38.3;
    } else {
      if(vm.count("m")) {
	Mmw=vm["m"].as<double>();
      } else {
	cerr << "Use --m to set the molecular weight of m" << endl;
	return 1;
      }
      if(vm.count("r")) {
	MFe=vm["r"].as<double>();
      } else {
	cerr << "Use --r to set the ratio of M:Fe" << endl;
	return 1;
      }
      if(vm.count("c")) {
	perC=vm["c"].as<double>();
      } else {
	cerr << "Use --c to set the percentage of C (C%)" << endl;
	return 1;
      }
      if(vm.count("h")) {
	perH=vm["h"].as<double>();
      } else {
	cerr << "Use --h to set the percentage of H (H%)" << endl;
	return 1;
      }
      if(vm.count("n")) {
	perN=vm["n"].as<double>();
      } else {
	cerr << "Use --n to set the percentage of N (N%)" << endl;
	return 1;
      }
      if(vm.count("f")) {
	perF=vm["f"].as<double>();
      } else {
	cerr << "Use --f to set the percentage of F (F%)" << endl;
	return 1;
      }
    }
    if(vm.count("noNorm")) {
      norm=0;
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
  if(norm) {
    double fesum=x[0]+x[1]+x[2];
    x = x / fesum;
  }
  cout << "FeF3" << "\t" << x[0] << endl;
  cout << "Fe(OH)3" << "\t" << x[1] << endl;
  cout << "Fe(NO3)3" << "\t" << x[2] << endl;
  cout << "MF2" << "\t" << x[3] << endl;
  cout << "M(OAc)2" << "\t" << x[4] << endl;
  cout << "H2O" << "\t" << x[5] << endl;
  return(0);
}
