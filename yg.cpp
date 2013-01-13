
#include <boost/program_options.hpp>
#include "yg.hpp"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char** argv) {

  double Mmw, MFe, perC, perH, perN, perF;

  bool norm=true;

  try {
    po::options_description general("General options");
    general.add_options()
      ("help", "produce help message")
      ("test", "Run with a test dataset");
    
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
    if(vm.count("test")) {
      Mmw=24.3; // Mg
      MFe=2; // M-Fe ratio
      perC=0.6;
      perH=1.5;
      perN=0;
      perF=42.8;
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
      norm=false;
    }
  } catch(exception& e) {
    cerr << "error: " << e.what() << "\n";
    return 1;
  }
  catch(...) {
    cerr << "Exception of unknown type!\n";
  }

  EA y(Mmw, MFe, perC, perH, perN, perF, norm);
  y.solve();
  y.printCoefs();
  return(0);
}
