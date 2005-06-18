//////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Adam M Phillippy
//! \date 06/08/2005
//!
//! \brief Layout the qry seqs and report alternate paths through the ref
//!
////////////////////////////////////////////////////////////////////////////////

#include "foundation_AMOS.hh"
#include "delta.hh"
#include "delta.cc"
#include "amp.hh"
#include <climits>
#include <list>
using namespace std;

const char * _NAME_ = "layout-paths";



//=============================================================== Options ====//
string   OPT_AlignName;                // alignment name parameter

int      OPT_MaxTrim        = 20;      // maximum ignorable trim length
int      OPT_Fuzzy          = 3;       // fuzzy equals tolerance
int      OPT_Seed           = -1;      // random seed

float    OPT_MinIdentity    = 0.0;     // min identity to tile

long int MAXIMAL = LONG_MAX;           // maximum integer


//============================================================= Constants ====//
struct Signature_t
{
  list<const DeltaNode_t *> qry;      // supporting queries

  vector<const DeltaNode_t *> ref;    // reference ids
  vector<long int> pos;               // reference alignment positions
  vector<long int> gap;               // query alignment gaps/overlaps

  // [sR]---[eR]   [sR]---[eR]   [sR]---[eR]
  //   [nodeR]       [nodeR]       [nodeR]
  //           [gap]         [gap]
};


bool operator==(const Signature_t & a, const Signature_t & b)
{
  //-- ** BEWARE ** of overflow for MAXIMAL values, use subtraction only!

  bool forward = true;
  vector<long int>::const_iterator i, j;
  vector<long int>::const_reverse_iterator k;
  long int ii, jj, kk;

  //-- Check the pos vector
  for ( i  = a . pos . begin( ), j  = b . pos . begin( );
        i != a . pos . end( ) && j != b . pos . end( );
        ++ i, ++ j )
    {
      ii = labs(*i); jj = labs(*j);
      if ( ii - OPT_Fuzzy > jj  ||  jj - OPT_Fuzzy > ii )
        break;
    }

  //-- If forward failed, try reverse
  if ( i != a . pos . end( ) || j != b . pos . end( ) )
    {
      for ( i  = a . pos . begin( ), k  = b . pos . rbegin( );
            i != a . pos . end( ) && k != b . pos . rend( );
            ++ i, ++ k )
        {
          ii = labs(*i);
          kk = labs(*k);
          if ( ii - OPT_Fuzzy > kk  ||  kk - OPT_Fuzzy > ii )
            break;
        }

      //-- Nothing worked, return false
      if ( i != a . pos . end( ) || k != b . pos . rend( ) )
        return false;

      forward = false;
    }


  //-- Check the gap vector
  if ( forward )
    {
      for ( i  = a . gap . begin( ), j  = b . gap . begin( );
            i != a . gap . end( ) && j != b . gap . end( );
            ++ i, ++ j )
        {
          ii = labs(*i); jj = labs(*j);
          if ( ii - OPT_Fuzzy > jj  ||  jj - OPT_Fuzzy > ii )
            break;
        }

      if ( i != a . gap . end( ) || j != b . gap . end( ) )
        return false;
    }
  else
    {
      for ( i  = a . gap . begin( ), k  = b . gap . rbegin( );
            i != a . gap . end( ) && k != b . gap . rend( );
            ++ i, ++ k )
        {
          ii = labs(*i);
          kk = labs(*k);
          if ( ii - OPT_Fuzzy > kk  ||  kk - OPT_Fuzzy > ii )
            break;
        }

      if ( i != a . gap . end( ) || k != b . gap . rend( ) )
        return false;
    }


  //-- Check the reference vector
  if ( a . ref != b . ref )
    return false;

  return true;
}

struct EdgeletCmp_t
//!< Compares query lo coord
{
  bool operator() (const DeltaEdgelet_t * a, const DeltaEdgelet_t * b) const
  { return ( a -> loQ < b -> loQ ); }
};


//========================================================== Fuction Decs ====//

//----------------------------------------------------- CombineSignatures ----//
//! \brief Combine like signatures
//!
//! \pre sigs vector is populated
//! \post Like signatures have been collapsed
//! \return void
//!
void CombineSignatures (list<Signature_t> & sigs);


//------------------------------------------------------ RecordSignatures ----//
//! \brief Reads the alignment graph and records the alignment signatures
//!
//! \post Populates the sigs vector, one per aligned sequence
//! \return void
//!
void RecordSignatures (const DeltaGraph_t & graph, list<Signature_t> & sigs);


//------------------------------------------------------------- ParseArgs ----//
//! \brief Sets the global OPT_% values from the command line arguments
//!
//! \param argc
//! \param argv
//! \return void
//!
void ParseArgs (int argc, char ** argv);


//------------------------------------------------------------- PrintHelp ----//
//! \brief Prints help information to cout
//!
//! \param s The program name
//! \return void
//!
void PrintHelp (const char * s);


//------------------------------------------------------------ PrintUsage ----//
//! \brief Prints usage information to cout
//!
//! \param s The program name
//! \return void
//!
void PrintUsage (const char * s);


//========================================================== Inline Funcs ====//

//========================================================= Function Defs ====//
int main (int argc, char ** argv)
{
  DeltaGraph_t graph;
  list<Signature_t> sigs;

  //-- COMMAND
  ParseArgs (argc, argv);          // parse the command line arguments
  srand (OPT_Seed);

  //-- GET/CLEAN ALIGNMENTS
  graph . build (OPT_AlignName, false);
  graph . flagScore (0, OPT_MinIdentity);
  graph . flagQLIS( );
  graph . clean( );

  //-- GENERATE SIGNATURES
  RecordSignatures (graph, sigs);

  //-- COMBINE SIGNATURES
  CombineSignatures (sigs);

  list<Signature_t>::iterator s;
  for ( s = sigs . begin( ); s != sigs . end( ); ++ s )
    {
      if ( s -> pos . size( ) != 2 ||
           labs (s -> pos . front( )) != MAXIMAL ||
           labs (s -> pos . back( )) != MAXIMAL )
        {
          list<const DeltaNode_t *>::const_iterator q;
          vector<long int>::const_iterator p;
          vector<const DeltaNode_t *>::const_iterator r;
          vector<long int>::const_iterator g;

          cout << s -> qry . size( ) << '\n';

          for ( r = s -> ref . begin( ); r != s -> ref . end( ); ++ r )
            cout << *((*r)->id) << '\t';
          cout << '\n';
  
          g = s -> gap . begin( );
          for ( p = s -> pos . begin( ); p != s -> pos . end( ); ++ p )
            {
              if ( labs (*p) == MAXIMAL )
                cout << 'B';
              else
                cout << labs (*p);

              if ( *p < *(++ p) )
                cout << " <- ";
              else
                cout << " -> ";

              if ( labs (*p) == MAXIMAL )
                cout << 'E';
              else
                cout << labs (*p);

              if ( g != s -> gap . end( ) )
                cout << " |" << *(g++) << "| ";
            }
          cout << '\n';

          for ( q = s -> qry . begin( ); q != s -> qry . end( ); ++ q )
            cout << *((*q)->id) << '\n';
          cout << endl;
        }
    }

  return EXIT_SUCCESS;
}


//----------------------------------------------------- CombineSignatures ----//
void CombineSignatures (list<Signature_t> & sigs)
{
  list<Signature_t>::iterator i, j;

  for ( i = sigs . begin( ); i != sigs . end( ); ++ i )
    {
      j = i;
      ++ j;
      while ( j != sigs . end( ) )
        {
          if ( *i == *j )
            {
              i -> qry . splice (i -> qry . end( ), j -> qry);
              j = sigs . erase (j);
            }
          else
            ++ j;
        }
    }
}


//------------------------------------------------------ RecordSignatures ----//
//
// Signatures will be generated as group of integer vectors, such that for the
// set of alignments of a query to the reference will be represented as:
//
// [sR]---[eR]   [sR]---[eR]   [sR]---[eR]
//   [nodeR]       [nodeR]       [nodeR]
//           [gap]         [gap]
//
// where the [sR] [eR] pairs are the start and end of each query alignment in
// the reference, and the corresponding [nodeR] is the reference in which the
// alignment occurs. The [gap] is the distance (negative for overlap) between
// adjacent alignments in the query. The [sR] [eR] pairs are ordered by the
// alignment's query position, lo to hi, and the [nodeR], [gap] values are
// similarly ordered. The [sR] [eR] values are also given a sign to represent
// represent the direction of the match in the reference (i.e. the alignment
// slope), +- for forward, -+ for reverse. If the first or last position is
// maximal, that is cannot be extended without reaching the end of the query,
// it is marked with the MAXIMAL value to indicate the break as incidental.
//
void RecordSignatures (const DeltaGraph_t & graph, list<Signature_t> & sigs)
{
  long int qlen;
  const string * qid;
  vector<DeltaEdgelet_t *> edgelets;

  map<string, DeltaNode_t>::const_iterator mi;
  vector<DeltaEdge_t *>::const_iterator ei;
  vector<DeltaEdgelet_t *>::iterator eli;

  for ( mi = graph.qrynodes.begin( ); mi != graph.qrynodes.end( ); ++ mi )
    {
      qid = mi->second.id;
      qlen = mi->second.len;

      sigs . push_back (Signature_t());
      Signature_t & sig = sigs . back( );
      sig . qry . push_back (&(mi -> second));

      //-- Collect the alignments for this query
      //   graph should be clean, so no need to worry about bad alignments
      edgelets . clear( );
      for ( ei  = (mi -> second) . edges . begin( );
            ei != (mi -> second) . edges . end( ); ++ ei )
        for ( eli  = (*ei) -> edgelets . begin( );
              eli != (*ei) -> edgelets . end( ); ++ eli )
          edgelets . push_back (*eli);

      assert ( !edgelets.empty( ) );

      //-- Sort by loQ
      sort (edgelets . begin( ), edgelets . end( ), EdgeletCmp_t( ));

      int slope;
      long int pos;
      vector<DeltaEdgelet_t *>::iterator first = edgelets . begin( );
      vector<DeltaEdgelet_t *>::iterator last = -- edgelets . end( );

      for ( eli = edgelets . begin( ); eli != edgelets . end( ); ++ eli )
        {
          slope = ((*eli)->dirR == (*eli)->dirQ) ? 1 : -1;

          //-- [sR]
          if ( eli == first  &&  (*eli)->loQ <= OPT_MaxTrim )
            pos = MAXIMAL * slope;
          else
            pos = (slope == 1) ? (*eli)->loR * slope : (*eli)->hiR * slope;
          sig . pos . push_back (pos);

          //-- [nodeR]
          sig . ref . push_back ((*eli)->edge->refnode);

          //-- [eR]
          if ( eli == last  &&  (*eli)->hiQ > qlen - OPT_MaxTrim )
            pos = MAXIMAL * -slope;
          else
            pos = (slope == 1) ? (*eli)->hiR * -slope : (*eli)->loR * -slope;
          sig . pos . push_back (pos);

          //-- [gap]
          if ( eli != last )
            sig . gap . push_back ((*(eli + 1))->loQ - (*eli)->hiQ - 1);
        }
    }
}


//------------------------------------------------------------- ParseArgs ----//
void ParseArgs (int argc, char ** argv)
{
  int ch, errflg = 0;
  optarg = NULL;

  while ( !errflg  &&
  ((ch = getopt (argc, argv, "f:hi:s:t:")) != EOF) )
    switch (ch)
      {
      case 'f':
        OPT_Fuzzy = atoi (optarg);
        break;

      case 'h':
        PrintHelp (_NAME_);
        exit (EXIT_SUCCESS);
        break;

      case 'i':
	OPT_MinIdentity = atof (optarg);
	break;

      case 's':
	OPT_Seed = atoi (optarg);
	break;

      case 't':
	OPT_MaxTrim = atoi (optarg);
	break;

      default:
        errflg ++;
      }

  if ( errflg > 0 || optind != argc - 1 )
    {
      PrintUsage (_NAME_);
      cout << "Try '" << _NAME_ << " -h' for more information.\n";
      exit (EXIT_FAILURE);
    }

  if ( OPT_Seed < 0 )
    OPT_Seed = time (NULL);

  OPT_AlignName = argv [optind ++];
}


//------------------------------------------------------------- PrintHelp ----//
void PrintHelp (const char * s)
{
  cout
    << ".NAME.\n\n"
    << s << "\n\n"
    << "Highlights structural disagreements between reference and query\n\n";

  PrintUsage (s);

  cout
    << ".OPTIONS.\n\n"
    << "-f uint       Set the fuzzy equals tolerance, default "
    << OPT_Fuzzy << endl
    << "-h            Display help information\n"
    << "-i float      Set the minimum alignment identity, default "
    << OPT_MinIdentity << endl
    << "-s uint       Set random generator seed to unsigned int, default\n"
    << "              seed is generated by the system clock\n"
    << "-t uint       Set maximum ignorable trim length, default "
    << OPT_MaxTrim << endl
    << endl;

  cout
    << ".DESCRIPTION.\n\n"
    << s << " highlights structural disagreements between reference and query\n"
    << "sequences by determining the 'path' each query takes through the\n"
    << "reference. The paths are generated by charting the reference position\n"
    << "and direction of the best alignments for each query. For redundant\n"
    << "query sequence data (e.g. shotgun sequencing reads) similar paths are\n"
    << "combined to form supporting evidence of polymorphism between the\n"
    << "reference and query. For each break in the path, the number of reads\n"
    << "spanning the break are also collected to provide counter evidence.\n"
    << endl
    << "This is an effective way of detecting misassemblies in a set of\n"
    << "contigs. By mapping the original set of sequencing reads to the\n"
    << "contig set, this program can identify the reads that disagree with\n"
    << "the assembly and report the alternate structure they support.\n\n";

  cout
    << ".KEYWORDS.\n\n"
    << "validation, comparison, assembly\n\n";

  return;
}


//------------------------------------------------------------ PrintUsage ----//
void PrintUsage (const char * s)
{
  cout
    << ".USAGE.\n\n"
    << s << " <deltafile>\n\n";

  return;
}
