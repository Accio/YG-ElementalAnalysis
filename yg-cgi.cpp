#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>

#include "cgicc/CgiDefs.h"
#include "cgicc/Cgicc.h"
#include "cgicc/HTTPHTMLHeader.h"
#include "cgicc/HTMLClasses.h"

#define PROG "/home/david/web/cgi-bin/ygEA/yg"

using namespace std;
using namespace cgicc;

int random(int mod) {
  int RndNmb=0;
  srand( (time(NULL)*mod));
  RndNmb=rand()%256;
  return RndNmb;
}
string tempfile() {
  string my = "/tmp/";
  my.resize(20);
  int RnD, i, b;
  for(unsigned i=5; i<20; i++) {
    b++;
    RnD=random(b) % 25+65;
    my.at(i)=RnD;
  }
  return(my);
}

string gElement="";
double gRval=0.0, gCval=0.0, gHval=0.0, gNval=0.0, gFval=0.0;
bool gHasNoFe=true;

void printForm(const Cgicc& cgi) {
  string mgSel="", caSel="", srSel="", baSel="", znSel="";

  cout << "<form method=\"post\" action=\""
       << cgi.getEnvironment().getScriptName() << "\">" << endl;

  cout << "<table>" << endl;
  // get E (element)
  cout << "<tr><td>"
       << "<label for=\"e\">Select M</label>"
       << "</td>" << endl;
  cout << "<td><select name=\"e\">" << endl;
  const_form_iterator element=cgi.getElement("e");

  if(element != (*cgi).end() && !element->isEmpty()) {
    gElement=**element;
    if(**element=="Mg") {
      mgSel="selected=\"selected\"";
    } else if (**element == "Ca") {
      caSel="selected=\"selected\"";
    } else if (**element == "Sr") {
      srSel="selected=\"selected\"";
    } else  if (**element == "Ba") {
      baSel="selected=\"selected\"";
    } else if (**element== "Zn") {
      znSel="selected=\"selected\"";
    }
  }
  cout << "<option value=\"Mg\"" << mgSel << ">Mg</option>"
       << "<option value=\"Ca\"" << caSel << ">Ca</option>"
       << "<option value=\"Sr\"" << srSel << ">Sr</option>"
       << "<option value=\"Ba\"" << baSel << ">Ba</option>"
       << "<option value=\"Zn\"" << znSel << ">Zn</option>"
       << "</select></td></tr>" << endl;

  // test whether Fe exists
  gHasNoFe=cgi.queryCheckbox("hasNoFe");

  cout << "<tr><td>"
       << "<label for=\"hasNoFe\">The system does <b>NOT</b> contain Fe</label>"
       << "</td>" << endl;
  cout << "<td>"
       << "<input type=\"checkbox\" name=\"hasNoFe\" "
       << (gHasNoFe ? "checked=\"checked\"" : "") << "/>" 
       << "</td></tr>" << endl;

  // ratio
  const_form_iterator ratio = cgi.getElement("r");
  if(ratio != (*cgi).end() && !ratio->isEmpty()) {
    gRval=(*ratio).getDoubleValue();
  }
  cout << "<tr><td>"
       << "<label for=\"r\">M:Fe ratio</label>"
       << "</td>"
       << "<td>"
       << "<input type=\"text\" name=\"r\""
       << "value=\"" << gRval << "\" autocomplete=\"off\"/>"
       << "</td></tr>" << endl;

  // perC
  const_form_iterator perC = cgi.getElement("c");
  if(perC != (*cgi).end() && !perC->isEmpty()) {
    gCval=(*perC).getDoubleValue(0, 100);
  }
  cout << "<tr><td>"
       << "<label for=\"c\">C%</label>"
       << "</td>"
       << "<td>"
       << "<input type=\"text\" name=\"c\" "
       << "value=\"" << gCval << "\" autocomplete=\"off\"/>"
       << "</td></tr>" << endl;
     
  // perH
  const_form_iterator perH = cgi.getElement("h");

  if(perH != (*cgi).end() && !perH->isEmpty()) {
    gHval=(*perH).getDoubleValue(0, 100);
  } 
  cout << "<tr><td>"
       << "<label for=\"h\">H%</label>"
       << "</td>"
       << "<td>"
       << "<input type=\"text\" name=\"h\" "
       << "value=\"" << gHval <<"\" autocomplete=\"off\"/>"
       << "</td></tr>" << endl;

  // perN
  const_form_iterator perN = cgi.getElement("n");
  if(perN != (*cgi).end() && !perN->isEmpty()) {
    gNval=(*perN).getDoubleValue(0,100);
  } 
  cout << "<tr><td>"
       << "<label for=\"n\">N%</label>"
       << "</td>"
       << "<td>"
       << "<input type=\"text\" name=\"n\" "
       << "value=\"" << gNval <<"\" autocomplete=\"off\"/>"
       << "</td></tr>" << endl;

  // perF
  const_form_iterator perF = cgi.getElement("f");

  if(perF != (*cgi).end() && !perF->isEmpty()) {
    gFval=(*perF).getDoubleValue(0,100);
  } 
  cout<< "<tr><td>"
      << "<label for=\"f\">F%</label>"
      << "</td>"
      << "<td>"
      << "<input type=\"text\" name=\"f\" "
      << "value=\"" << gFval << "\" autocomplete=\"off\"/>"
      << "</td></tr>" << endl;

  cout << "</table>" << endl;

  cout << " <input type=\"submit\" value=\"submit\">" << endl;
}

void doCalc () {
  string line;
  string tmpfile=tempfile();
  stringstream comms;
  if(!gHasNoFe) {
    comms << PROG << " --e " << gElement
	  << " --r " << gRval
	  <<  " --c " << gCval
	  << " --h " << gHval
	  << " --n " << gNval
	  << " --f " << gFval << " --html > " << tmpfile;
  } else {
    comms << PROG << " --nofe "
	  << " --e " << gElement
      	  << " --c " << gCval
      	  << " --f " << gFval
	  << " --html > " << tmpfile;
  }
  string comm = comms.str();
  system(comm.c_str());
  std::ifstream infile(tmpfile.c_str());
  if(infile.is_open()) {
    while (infile.good()){
      getline(infile,line);
      cout << line << endl;
    }
    infile.close();
  } else {
    cout << "Unable to open file" << endl << endl;
  }
  comm = "rm -f " + tmpfile;
  system(comm.c_str());
}

int main(int argc, char** argv)
{
  try{
    Cgicc cgi;
    
    // HTTP header
    cout << HTTPHTMLHeader() << endl;
    cout << HTMLDoctype(HTMLDoctype::eStrict) << endl;
    cout << html().set("lang", "EN").set("dir", "LTR") << endl;
    cout << html() << head() << title("Elemental Analysis of Fe-M ternary catalysts") << head() << endl;

    // style
    cout << "<style type=\"text/css\" media=\"screen, print, projection\">" << endl;
    cout << ".left {float:left;margin:0px 0px 0px 5px;width:150px;}" << endl;
    cout << ".right {float:right;width:200px;clear:right;}" << endl;
    cout << "</style>";
    cout << "<script>";
    cout << "</script>";

    cout << body().set("bgcolor","#E6E6DC").set("text","#000000").set("link","#0000ff").set("vlink","#000080") << endl;
    cout << h1("Elemental Analysis of Fe-M ternary catalysts") << endl;
    
    cout << br() << endl;
    printForm(cgi);
    doCalc();
 
    return(0);
  } catch (const exception& e) {
    html::reset();      head::reset();          body::reset();
    title::reset();     h1::reset();            h4::reset();
    comment::reset();   td::reset();            tr::reset(); 
    table::reset();     cgicc::div::reset();    p::reset(); 
    a::reset();         h2::reset();            colgroup::reset();
    
    // Output the HTTP headers for an HTML document, and the HTML 4.0 DTD info
    cout << HTTPHTMLHeader() << HTMLDoctype(HTMLDoctype::eStrict) << endl;
    cout << html().set("lang","en").set("dir","ltr") << endl;
    
    // Set up the page's header and title.
    // I will put in lfs to ease reading of the produced HTML. 
    cout << head() << endl;
    
    // Output the style sheet portion of the header
    cout << style() << comment() << endl;
    cout << "body { color: black; background-color: white; }" << endl;
    cout << "hr.half { width: 60%; align: center; }" << endl;
    cout << "span.red, STRONG.red { color: red; }" << endl;
    cout << "div.notice { border: solid thin; padding: 1em; margin: 1em 0; "
	 << "background: #ddd; }" << endl;
    
    cout << comment() << style() << endl;
    
    cout << title("GNU cgicc exception") << endl;
    cout << head() << endl;
    
    cout << body() << endl;
    
    cout << h1() << "GNU cgi" << span("cc", set("class","red"))
	 << " caught an exception" << h1() << endl; 
    
    cout << cgicc::div().set("align","center").set("class","notice") << endl;
    cout << h2(e.what()) << endl;
 
    // End of document
    cout << cgicc::div() << endl;
    cout << hr().set("class","half") << endl;
    cout << body() << html() << endl;
     
    return EXIT_SUCCESS;
  }
}
