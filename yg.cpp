
#include <boost/program_options.hpp>
#include "yg.hpp"

using namespace std;
namespace po = boost::program_options;

int main(int argc, char** argv) {

  double MFe=0, perC=0, perH=0, perN=0, perF=0;
  Melement element;
  bool nofe=false;
  bool norm=true;
  bool html=false;
  
  // option parsing
  try {
    po::options_description general("General options");
    general.add_options()
      ("help", "produce help message")
      ("test", "Run with a test dataset");

    po::options_description mwm("Specify the Metal (specify only one parameter)");
    mwm.add_options()
      ("e",  po::value<string>(), "M element=Mg, Ca, Sr or Ba");
    
    po::options_description essential("Numeric parameters");
    essential.add_options()
      ("r", po::value<double>(),"M-Fe Ratio")
      ("c", po::value<double>(),"Percentage of C")
      ("h", po::value<double>(),"Percentage of H")
      ("n", po::value<double>(),"Percentage of N")
      ("f", po::value<double>(),"Percentage of F");
    
    po::options_description optional("Optional");
    optional.add_options()
      ("nofe", "No Fe is in the system")
      ("noNorm", "If set, the sum of Fe is *NOT* normalised to 1")
      ("html", "If set, the output will be a HTML table");
    
    po::options_description desc("Allowed options");
    desc.add(general).add(mwm).add(essential).add(optional);

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    if(vm.count("help") || argc==1) {
      cout << desc << "\n";
      return 0;
    }

    if(vm.count("test")) {
      element=Mg; // Mg
      MFe=1.0; // M-Fe ratio
      perC=0.2;
      perH=1.6;
      perN=1.2;
      perF=38.3;
    } else {
      if (vm.count("e")) {
	string es=vm["e"].as<string>();
	if(es.compare("Mg")==0) {
	  element=Mg;
	} else if (es.compare("Ca")==0) {
	  element=Ca;
	} else if (es.compare("Sr")==0) {
	  element=Sr;
	} else if (es.compare("Ba")==0) {
	  element=Ba;
	} else if (es.compare("Zn")==0) {
	  element=Zn;
	} else {
	  cerr << "-e accepts only Mg,Ca,Sr, Ba or Zn" << endl;
	  return 1;
	}
      }
      
      if(vm.count("nofe")) {
	nofe=true;
      }
      
      if(vm.count("r")) {
	MFe=vm["r"].as<double>();
      } else {
	if(!nofe) {
	  cerr << "Use -r to set the M/Fe ratio" << endl;
	  return 1;
	}
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
	if(!nofe) {
	  cerr << "Use --h to set the percentage of H (H%)" << endl;
	  return 1;
	}
      }
      
      if(vm.count("n")) {
	perN=vm["n"].as<double>();
	if(nofe & perN>0) {
	  throw "When --nofe is set, perN cannot be other than 0";
	} 
      } else {
	if(!nofe) {
	  cerr << "Use --n to set the percentage of N (N%)" << endl;
	  return 1;
	}
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
    if(vm.count("html")) {
      html=true;
    }
  }
  catch(exception& e) {
    cerr << "error: " << e.what() << "\n";
    return 1;
  } catch(char *str) {
    cout << "Exception raised: " << str << endl;
  } catch(...) {
    cerr << "Exception of unknown type!\n";
  }
  
  EA y;
  if(!nofe) {
    y.setValues(element, MFe, perC, perH, perN, perF, norm);
  } else {
    y.setValues(element, perC, perF, norm);
  }

  y.solve();
  if(html) {
    y.htmlPrint();
  } else {
    y.printCoefs();
  }
  return(0);
}
