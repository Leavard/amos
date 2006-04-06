// $Id$

// Shotgun sequence simulator
// 
// Simulates a set of paired/unpaired reads at a certain coverage.

// STL includes
#include <vector>
#include <algorithm>
#include <map>
#include <string>

// C++ includes
#include <fstream>
#include <iostream>

// generic C includes
#include <ctype.h>
#include <getopt.h>
#include <math.h>

// GSL includes
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>

#define endl "\n"  // die endl die

using namespace std;

map<string, string> globals;   // global variables

struct Library_t {
  string name;
  int mean;
  int stdev;
  int readmean;
  int readstdev;
  int readmin;
  int readmax;
  float coverage;
  float permates;
} ;

struct Fragment_t {
  unsigned long left;
  unsigned long right;
  int libid;
  int forwlen;
  int revlen;
} ;

vector<Library_t> libraries;
vector<long> fastasizes;
long genomeSize = 0;

void printHelpText()
{
  cout << 
    "\n"
    "USAGE:\n"
    "\n"
    "shotgunSim -C config_file file.fasta\n"
    "\n"
    "OPTIONS:\n"
    "-C config_file configuration file containing experiment parameters\n"
    "-l layout.afg  output layout of reads into this file\n"
    "file.fasta     fasta sequence used as template for shotgun\n"
    "\n"
    "CONFIG FILE FORMAT:\n"
    "The config file consists in a series of blocks, one for each library.\n"
    "Blocks start with a library line:\n"
    "library <name> <mean> <stdev> <%mates>\n"
    "\n"
    "Even for unmated reads you need to specify a library as follows:\n"
    "library unmated 0 0 0\n"
    "\n"
    "The library line is followed by information about the reads being produced:\n"
    "reads <mean> <stdev> <min> <max>\n"
    "\n"
    "All parameters of the reads command must be specified.\n"
    "The reads will have lengths sampled from a gaussian distribution\n"
    "that is truncated to <min> <max>\n"
    "Note that the <min> value also restricts the smallest insert that can\n"
    "be generated by the library\n"
    "\n"
    "Finally, you need to specify the coverage of reads to be generated\n"
    "from the library:\n"
    "coverage <coverage>\n"
    "\n"
    "IMPORTANT NOTE: incorrect parameters may lead to the program running\n"
    "indefinitely\n"
       << endl;
} // printHelpText


bool GetOptions(int argc, char ** argv)
{
  int option_index = 0;
  static struct option long_options[] = {
    {"C",     1, 0, 'c'},
    {"l",     1, 0, 'l'},
    {"help",  0, 0, 'h'},
    {"h",     0, 0, 'h'}, 
    {0, 0, 0, 0}
  };
  
  bool helpRequested = false;
  bool confFile = false;
  int c;
  while ((c = getopt_long_only(argc, argv, "", long_options, &option_index))!= -1){
    switch (c){
    case 'c':
      confFile = true;
      globals["conffile"] = string(optarg);
      break;
    case 'l':
      globals["layoutfile"] = string(optarg);
      break;
    case 'h':
      //      printHelpText();
      helpRequested = true;
      break;
    case '?':
      return false;
    }
  } // while each parameter

  if (helpRequested){
    printHelpText();
    exit(0);
  } // if help

  if (optind < argc){
    globals["input"] = string(argv[optind]);
  } 

  return true;
} // GetOptions

void parseInFile(ifstream & in)
{
  string line;
  long size = 0;
  bool first = true;

  while (getline(in, line)){
    if (line[0] == '>'){ // header line
      if (! first){
	fastasizes.push_back(size);
	size = 0;
      }
      first = false;
    } else { // fasta record
      size += line.length();
    }
  }
  if (! first)
    fastasizes.push_back(size);

  size = 0;
  for (int i = 0; i < fastasizes.size(); i++){
    long tmp = fastasizes[i];
    fastasizes[i] = size;
    size += tmp;
  }
  genomeSize = size;
}

void parseConfFile(ifstream & conf)
{
  string line;
  int readmean;
  int readstdev;
  int mean; 
  int stdev;
  int min; 
  int max;
  char libid[256];
  float coverage;
  float permates;

  Library_t library;

  library.mean = 0; library.stdev = 0; library.coverage = 0;

  bool first = true;
  while (getline(conf, line)){
    if (line.length() > 1 && line[0] == '#')// skip comments
      continue;
    if (! first && strncmp(line.c_str(), "library", 7) == 0){
      libraries.push_back(library);
      library.mean = 0; library.stdev = 0; library.coverage = 0;
    } 
    if (strncmp(line.c_str(), "library", 7) == 0)
      first = false; // no longer first library

    if (sscanf(line.c_str(), "library %s %d %d %f", libid, &mean, &stdev, &permates) == 4){
      library.name = string(libid);
      library.mean = mean;
      library.stdev = stdev;
      library.permates = permates;
    } else if (sscanf(line.c_str(), "library %s", libid) == 1){
      library.name = string(libid);
    }
    if (sscanf(line.c_str(), "reads %d %d %d %d", &mean, &stdev, &min, &max) == 4){
      library.readmean = mean;
      library.readstdev = stdev;
      library.readmin = min;
      library.readmax = max;
    }
    if (sscanf(line.c_str(), "coverage %f", &coverage) == 1){
      library.coverage = coverage;
    }
  }
  if (first) {
    cerr << "config file seems empty" << endl;
    exit(1);
  }
  libraries.push_back(library);
} // parseConfFile

string parseFasta(ifstream & in){
  static string lastline = "";
  string line = "";
  string out = "";
  
  if (lastline == ""){
    getline(in, lastline);
    //    cerr << "Got " << lastline << endl;
    if (lastline[0] != '>'){
      cerr << "Sequence doesn't start with >: " << lastline << endl;
    }
  }
  while (getline(in, line)){
    lastline = line;
    if (line[0] == '>')
      break;
    out += line;
  }
  return out;
} // parseFasta

char revcompl(char c)
{
  switch (toupper(c)){
  case 'A':    return 'T';
  case 'C':    return 'G';
  case 'T':    return 'A';
  case 'G':    return 'C';
  case 'U':    return 'A';
  case 'M':    return 'K';
  case 'R':    return 'Y';
  case 'W':    return 'W';
  case 'S':    return 'S';
  case 'Y':    return 'R';
  case 'K':    return 'M';
  case 'V':    return 'B';
  case 'H':    return 'D';
  case 'D':    return 'H';
  case 'B':    return 'V';
  case 'N':    return 'N';
  case 'X':    return 'X';
  case '-':    return '-';
  default:     return 'N';
  }
} // revcompl char version

string revcompl(const string & in)
{
  string out = in;
  for (int i = 0; i < in.length(); i++)
    out[in.length() - i - 1] = revcompl(in[i]);
  return out;
} // revcompl string version

void printRED(string seq, int id, int frgid)
{
  cout << "{RED\n";
  cout << "iid:" << id << "\n";
  if (frgid != 0)
    cout << "frg:" << frgid << "\n";
  cout << "clr:0," << seq.length() << "\n";
  cout << "seq:\n";
  for (int i = 0; i < seq.length(); i+= 60)
    cout << seq.substr(i, 60) << "\n";
  cout << ".\n";
  cout << "qlt:\n";
  for (int i = 0; i < seq.length(); i++){
    if (i != 0 && i % 60 == 0)
      cout << "\n";
    cout << "l";
  }
  cout << "\n";
  cout << ".\n";
  cout << "}\n";
}

//--------------------------------------------------------
int main(int argc, char ** argv)
{

  if (!GetOptions(argc, argv)){
    cerr << "Command line parsing failed, use -h option for usage info" 
	 << endl;
    //printHelpText();
    exit(1);
  }

  if (globals.find("conffile") == globals.end()){
    cerr << "A configuration file must be provided.  see -h" << endl;
    exit(1);
  }

  if (globals.find("input") == globals.end()){
    cerr << "An input file must be provided.  see -h" << endl;
    exit(1);
  }

  ifstream conf(globals["conffile"].c_str());
  if (! conf.is_open()){
    cerr << "Could not open conffile" << globals["conffile"] << endl;
    exit(1);
  }

  parseConfFile(conf);

  ifstream in(globals["input"].c_str());
  if (! in.is_open()){
    cerr << "Could not open input " << globals["input"] << endl;
    exit(1);
  }

  parseInFile(in);

  vector<vector<Fragment_t> > fragments;
  fragments.reserve(fastasizes.size());
  fragments.resize(fastasizes.size());

  // set up the random number generator
  gsl_rng * rng = gsl_rng_alloc(gsl_rng_mt19937);
  //  gsl_rng * rng = gsl_rng_alloc(gsl_rng_rand);

  for (int i = 0; i < libraries.size(); i++){
    if (libraries[i].coverage == 0)
      continue;

    unsigned long target = (long) libraries[i].coverage * genomeSize;
    unsigned long totLen = 0;

    unsigned long mean, stdev; 

    if (libraries[i].mean == 0){
      mean = libraries[i].readmean;
      stdev = libraries[i].readstdev;
    } else {
      mean = libraries[i].mean;
      stdev = libraries[i].stdev;
    }

    //    cerr << "Library mean " << libraries[i].mean << endl;
    //    cerr << "Library stdev " << libraries[i].stdev << endl;
    //    cerr << "Library read mean " << libraries[i].readmean << endl;
    //    cerr << "Library.readstdev " << libraries[i].readstdev << endl;
    
    while (totLen < target){
      unsigned long start = gsl_rng_uniform_int(rng, genomeSize);
      //      cerr << "gaussian stdev: " << stdev << " value: " << gsl_ran_gaussian(rng, stdev) << endl;
      unsigned long len = mean + (unsigned long) round(gsl_ran_gaussian(rng, stdev));
      
      // find molecule that contains start
      vector<long>::iterator mol = 
	upper_bound(fastasizes.begin(), fastasizes.end(), start);
      
      if (len < libraries[i].readmin)
	continue;               // inserts should be bigger than the
				// smallest read

      if (mol == fastasizes.end()) {
	//	cerr << "at end comparing " << len << " to " << genomeSize - start <<endl;
	if (len > genomeSize - start) // beyond end of genome
	  continue;
      } else {
	//	cerr << "comparing " << start + len << " to " << *mol << endl;
	if (len > *mol - start) // beyond end of molecule
	  continue;
      }

      //      cerr << "Length is " << (unsigned long) len << " , " << start + len << endl;

      assert (mol != fastasizes.begin());
      int idx = mol - fastasizes.begin() - 1; // where to insert fragment

      Fragment_t frg;

      vector<long>::iterator prev = mol - 1;
      frg.left = start - *prev;
      frg.right = frg.left + len;
      frg.libid = i + 1;

      // now the reads
      if (libraries[i].mean == 0){ // unpaired read
	frg.forwlen = len;
	frg.revlen = 0;
	totLen += len;
      } else {
	int forwlen = (int) (libraries[i].readmean 
			     + gsl_ran_gaussian(rng, libraries[i].readstdev));
	int revlen = (int) (libraries[i].readmean 
			    + gsl_ran_gaussian(rng, libraries[i].readstdev));

	// make sure reads are between min and max
	while (forwlen > libraries[i].readmax || forwlen < libraries[i].readmin)
	  forwlen = (int) (libraries[i].readmean 
			     + gsl_ran_gaussian(rng, libraries[i].readstdev));

	while (revlen > libraries[i].readmax || revlen < libraries[i].readmin)
	  revlen = (int) (libraries[i].readmean 
			     + gsl_ran_gaussian(rng, libraries[i].readstdev));


	if (forwlen > len) forwlen = len;
	if (revlen > len) revlen = len;
	frg.forwlen = forwlen;
	frg.revlen = revlen;
	totLen += forwlen + revlen;
      }

      fragments[idx].push_back(frg);
    } // while coverage not attained
  } // for each library
  
  // header record
  cout << "{UNV\n";
  cout << "com:\n";
  cout << "generated by shotgunSim\n";
  for (int i = 0; i < argc; i++)
    cout << argv[i] << " ";
  cout << "\n";
  cout << ".\n";
  cout << "}\n";
  // output libraries  
  for (int i = 0; i < libraries.size(); i++){
    if (libraries[i].coverage == 0)
      continue;
    if (libraries[i].mean == 0)
      continue;
  
    cout << "{LIB\n";
    cout << "iid:" << i + 1 << "\n";
    cout << "eid:" << libraries[i].name << "\n";
    cout << "{DST\n";
    cout << "mea:" << libraries[i].mean << "\n";
    cout << "std:" << libraries[i].stdev << "\n";
    cout << "}\n";
    cout << "}\n";
  } // for each library again

  // now the fragments are all built, we just need to write out the files
  in.clear();  // clear flags
  in.seekg(0, ifstream::beg); // re-process input file

  // if we do layouts, do them here
  ofstream laystream;
  if (globals.find("layoutfile") != globals.end()){
    laystream.open(globals["layoutfile"].c_str(), ofstream::out|ofstream::trunc);  
    if (! laystream.is_open()){
      cerr << "Could not open " << globals["layoutfile"] << endl;
      exit(1);
    }
  }

  int seq = 0;
  string sequence;
  int numseq = 0;
  while ((sequence = parseFasta(in)) != ""){
    if (globals.find("layoutfile") != globals.end()){ // start a layout
      laystream << "{LAY\n";
      laystream << "iid:" << seq + 1 << endl;
    }
    for (int f = 0; f < fragments[seq].size(); f++){
      int frgid = 0;
      //     cerr << fragments[seq][f].left << " " << fragments[seq][f].right << endl;
      //     cerr << "DoingF " << fragments[seq][f].left << " " << fragments[seq][f].forwlen << endl;
      string forw = sequence.substr(fragments[seq][f].left, fragments[seq][f].forwlen);//this one always exists

      int doRead = 0;  // do both
      double mates = gsl_rng_uniform(rng);

      if (fragments[seq][f].revlen == 0 || 
	  libraries[fragments[seq][f].libid].permates == 1 ||
	  mates < libraries[fragments[seq][f].libid].permates)
	doRead = 0;
      else
	doRead = 1;

      mates = gsl_rng_uniform(rng);
      if (mates < 0.5)
	doRead = -doRead; // do just reverse

      if (fragments[seq][f].revlen != 0) { // the whole shebang
	//      cerr << "DoingR " << fragments[seq][f].right - fragments[seq][f].revlen << " " << fragments[seq][f].revlen << endl;
	string rev = revcompl(sequence.substr(fragments[seq][f].right - fragments[seq][f].revlen, fragments[seq][f].revlen));
	frgid = ++numseq;
	cout << "{FRG\n";
	cout << "iid:" << frgid << "\n";
	cout << "lib:" << fragments[seq][f].libid << "\n";
	cout << "typ:I\n";
        if ( doRead == 0 )
          cout << "rds:" << numseq + 1 << ',' << numseq + 2 << '\n';
	cout << "}\n";
	if (doRead <= 0) {
	  printRED(rev, ++numseq, frgid);
	  if (globals.find("layoutfile") != globals.end()){ // add read to the layout
	    laystream << "{TLE\n";
	    laystream << "src:" << numseq << endl;
	    laystream << "off:" << fragments[seq][f].right - fragments[seq][f].revlen << endl;
	    laystream << "clr:" << rev.length() << ",0\n";
	    laystream << "}\n";
	  }
	}
      }
      if (doRead >= 0) {
	printRED(forw, ++numseq, frgid);
	if (globals.find("layoutfile") != globals.end()){ // add read to the layout
	  laystream << "{TLE\n";
	  laystream << "src:" << numseq << endl;
	  laystream << "off:" << fragments[seq][f].left << endl;
	  laystream << "clr:0," << forw.length() << "\n";
	  laystream << "}\n";
	}
      }
    } // for each fragment in this sequence
    
    seq++;
    if (globals.find("layoutfile") != globals.end())
      laystream << "}\n";
  }

  if (globals.find("layoutfile") != globals.end())
    laystream.close();

  return(0);
} // main
