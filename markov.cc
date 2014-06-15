//
// markov.cc
//
// Author: David Rottenfusser
// Date: 26 Mar 2014
//
// Usage: ./a.out [options] [file]
//
// Using an input file (or Lorem Ipsum), creates a hidden Markov model of the
// characters in the file using the simplest possible algorithm.  For each character,
// it stores a frequency distribution of the characters found immediately after.  It
// then generates some text using the model.
//
// 4 Apr 2014: Added support for command line paramenters with getopt().
// 4 Apr 2014: Added default Lorem Ipsum text.
//
// Next Steps:
// * use Bayesian rules to update state transitions
// * store the last 2 characters instead of the last 1
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>

#include <cstdlib>
#include <ctime>

#include <unistd.h>

using namespace std;

// default source text, if no file is specified
#define LOREM_IPSUM "Lorem ipsum dolor sit amet, consectetur adipisicing elit, \
sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad \
minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea \
commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit \
esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat \
non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."

typedef map<char,int>   tFreq;
typedef map<char,tFreq> charTable;

char *progName = NULL;

void showhelp() {
  cout << "Usage: " << progName << " [options] [file]\n"
       << "Generates gibberish in the style of file, or Lorem Ipsum if no file is given.\n"
       << "\n"
       << "Options:\n"
       << "-c n\tNumber of bytes to generate (optional)\n"
       << "-h\tDisplay this message\n\n";
}

char next(tFreq &t) {
  int sum = 0;
  for ( tFreq::iterator i = t.begin(); i != t.end(); i++ ) {
    sum += i->second;
  }

  if ( !sum ) { return 0; }

  // pick a card, any card
  int r = rand() % sum;
  for ( tFreq::iterator i = t.begin(); i != t.end(); i++ ) {
    r -= i->second;
    if (r <= 0) { return i->first; }
  }

  return 0;
}

int main(int argc, char* argv[]) {

  // parse command line args
  progName = argv[0];
  long long bytes = -1;
  char c;
  while ( ( c = getopt ( argc, argv, "c:h" ) ) != -1 ) {
    switch ( c ) {
    case 'c':
      bytes = strtol(optarg, NULL, 0);
      if (bytes == 0) {
	printf("%s: Invalid number: '%s'\n", progName, optarg);
	showhelp();
	exit(1);
      }
      break;
    case 'h':
      showhelp();
      exit(0);
    case '?':
      showhelp();
      exit(1);
    }
  }

  // open source file (or default buffer)
  istream *in;
  if (optind >= argc) {
    in = new istringstream(LOREM_IPSUM);
  } else {
    char *f = argv[optind];
    in = new ifstream(f);
  }

  // assemble the model
  char last = 0;
  charTable t;
  while ( *in ) {
    char c;
    in->get( c );

    //if ( c == '\n' ) { continue; }

    t[last][c]++;
    last = c;
  }
  t[last][0]++;

  delete in;

  srand( time( NULL ) );

  // generate some text using the model
  last = 0;
  while (bytes != 0) {
    char c = next( t[last] );
    if ( !c && (bytes == -1) ) {
      break;
    }
    cout << c;
    last = c;
    if (bytes > 0) { --bytes; }
  }

  cout << endl;
  return 0;
}
