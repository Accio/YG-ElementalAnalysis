#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <time.h>

#include "cgi.h"

using namespace std;

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
int main(int argc, char** argv)
{
  CCGI cgi;
  CCGIItem item;
  char* lpszOut;
  char* lpszContentLength = getenv("CONTENT_LENGTH");
  int i,nLength;
  string line;
  string tmpfile=tempfile();

  cout << "Content-type: text/html" << endl << endl;
  if(lpszContentLength == NULL) {
    std::ifstream infile("yg-mainpage.html");
    if(infile.is_open()) {
      while (infile.good()){
	getline(infile,line);
	std::cout << line << std::endl;
      }
      infile.close();
    } else {
      cout << "Unable to open file" << endl << endl;
    }
    return(1);
  }

  string comm="/usr/lib/cgi-bin/yg";

  cgi.Load();

  for(i=0; i<cgi.GetItemCount(); i++)
  {
    cgi.GetItem(i,&item);

    nLength = item.GetNameLength();
    lpszOut = (char*) malloc(nLength);
    item.GetName(lpszOut,nLength);

    comm += " --";
    comm += lpszOut;

    free(lpszOut);

    nLength = item.GetLength();
    lpszOut = (char*) malloc(nLength);
    item.GetValue(lpszOut,nLength);
    
    comm += " ";
    comm += lpszOut;

    free(lpszOut);
  }
  
  comm += " --html > " + tmpfile;
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
   cout << "</body>" << endl << "</html>" << endl;
   return(0);
}
