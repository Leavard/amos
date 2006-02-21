#include "foundation_AMOS.hh"
#include "amp.hh"
#include "fasta.hh"

using namespace AMOS;
using namespace std;

bool OPT_UseEIDs = 0;
bool OPT_UseIIDs = 0;
bool OPT_FASTA = 0;
int  OPT_Spacer = 100;
string OPT_BankName;

string OPT_EIDFile;
string OPT_IIDFile;

void PrintUsage (const char * s)
{
  cerr << "\nUSAGE: " << s << "  [options]  <bank path>\n\n";
}

//------------------------------------------------------------- PrintHelp ----//
void PrintHelp (const char * s)
{
  PrintUsage(s);
  cerr << "-h      Display help information\n"
       << "-v      Display the compatible bank version\n"
       << "-e      Use EIDs for names (DEFAULT)\n"
       << "-i      Use IIDs for names\n"
       << "-E file Dump just the contig eids listed in file\n"
       << "-I file Dump just the contig iids listed in file\n\n"

       << "-f      Print the scaffold consensus sequences in multi-fasta format\n"
       << "-N val  Specify number of N's to place between contigs\n"
       << endl;
  
  cerr << "Takes an AMOS bank directory and dumps the scaffolds to stdout.\n\n"
       << "Default format:\n"
       << ">scaffid numcontigs scaffbases scaffspan\n"
       << "contigid orientation contiglen gapsize\n"
       << endl;
}

void ParseArgs (int argc, char ** argv)
{
  int ch, errflg = 0;
  optarg = NULL;

  while ( !errflg && ((ch = getopt (argc, argv, "hveiE:I:fN:")) != EOF) )
  {
    switch (ch)
    {
      case 'h':
        PrintHelp (argv[0]);
        exit (EXIT_SUCCESS);
        break;

      case 'v':
        PrintBankVersion (argv[0]);
        exit (EXIT_SUCCESS);
      break;

      case 'e': OPT_UseEIDs = true;   break;
      case 'i': OPT_UseIIDs = true;   break;
      case 'E': OPT_EIDFile = optarg; break;
      case 'I': OPT_IIDFile = optarg; break;

      case 'f': OPT_FASTA   = true;   break;
      case 'N': OPT_Spacer  = atoi(optarg); break;

      default: errflg ++;
      }
  }

  if ( errflg > 0 || optind != argc - 1 )
  {
      PrintUsage (argv[0]);
      cerr << "Try '" << argv[0] << " -h' for more information.\n";
      exit (EXIT_FAILURE);
  }

  OPT_BankName = argv [optind];

  if (!OPT_UseEIDs && !OPT_UseIIDs) { OPT_UseEIDs = true; }
}


void printScaffold(Scaffold_t & scaff, Bank_t & contig_bank)
{
  vector<Tile_t> & contigs = scaff.getContigTiling();
  vector<Tile_t>::const_iterator ci;

  cout << ">";
  if (OPT_UseEIDs)
  {
    cout << scaff.getEID();
  }
  else
  {
    cout << scaff.getIID();
  }

  int bases = 0, span = 0;

  for (ci = contigs.begin(); ci != contigs.end(); ci++)
  {
    bases += ci->getGappedLength();
    int right = ci->getRightOffset();

    if (right > span) { span = right; }
  }

  cout << " " << contigs.size() << " " << bases << " " << span << endl;

  string seq;
  Contig_t contig;

  int lastrightoffset;

  for (ci = contigs.begin(); ci != contigs.end(); ci++)
  {
    if (OPT_FASTA)
    {
      if (ci != contigs.begin())
      {
        seq.append(OPT_Spacer, 'N');
      }

      contig_bank.fetch(ci->source, contig);
      seq += contig.getUngappedSeqString();
    }
    else
    {
      if (ci != contigs.begin())
      {
        int gapsize = ci->offset - lastrightoffset;
        cout << " " <<  gapsize << endl;
      }

      if (OPT_UseEIDs)
      {
        cout << contig_bank.getIDMap().lookupEID(ci->source);
      }
      else
      {
        cout << ci->source;
      }

      if (ci->range.isReverse()) { cout << " EB "; }
      else                       { cout << " BE "; }

      cout << ci->range.getLength();
      lastrightoffset = ci->getRightOffset();
    }
  }

  if (OPT_FASTA)
  {
    Fasta_Print(stdout, seq.c_str(), NULL, 60);
  }
  else
  {
    int gapsize = 0; // No gap after last contig
    cout << " " <<  gapsize << endl;
  }
}




int main (int argc, char ** argv)
{
  ParseArgs (argc, argv);

  BankStream_t scaff_bank(Scaffold_t::NCODE);
  Bank_t contig_bank(Contig_t::NCODE);

  cerr << "Processing " << OPT_BankName << " at " << Date() << endl;

  try
  {
    scaff_bank.open(OPT_BankName, B_READ);
    contig_bank.open(OPT_BankName, B_READ);

    Scaffold_t scaffold;
    ifstream file;
    string id;

    if (!OPT_EIDFile.empty())
    {
      file.open(OPT_EIDFile.c_str());
      
      if (!file)
      {
        throw Exception_t("Couldn't open EID File", __LINE__, __FILE__);
      }

      while (file >> id)
      {
        scaff_bank.seekg(scaff_bank.getIDMap().lookupBID(id));
        scaff_bank >> scaffold;
        printScaffold(scaffold, contig_bank);
      }
    }
    else if (!OPT_IIDFile.empty())
    {
      file.open(OPT_IIDFile.c_str());

      if (!file)
      {
        throw Exception_t("Couldn't open IID File", __LINE__, __FILE__);
      }

      while (file >> id)
      {
        scaff_bank.seekg(scaff_bank.getIDMap().lookupBID(atoi(id.c_str())));
        scaff_bank >> scaffold;
        printScaffold(scaffold, contig_bank);
      }
    }
    else
    {
      while (scaff_bank >> scaffold)
      {
        printScaffold(scaffold, contig_bank);
      }
    }

    contig_bank.close();
    scaff_bank.close();
  }
  catch (Exception_t & e)
  {
    cerr << "ERROR: -- Fatal AMOS Exception --\n" << e;
    return EXIT_FAILURE;
  }

  cerr << "End: " << Date() << endl;
  return EXIT_SUCCESS;
}
