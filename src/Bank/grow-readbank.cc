//  A. L. Delcher
//
//  File:  grow-readbank.cc
//
//  Last Modified:  20 February 2003
//
//  Add reads to a readbank, creating a new one if specfied.


#include  "delcher.hh"
#include  "WGA_databank.hh"
#include  "WGA_datatypes.hh"
#include  "CelMsgWGA.hh"
#include  <vector>
#include  <string>

using namespace std;


const int  MAX_LINE = 1000;
const int  NEW_SIZE = 1000;


enum  Input_Format_t
     {FASTA_FORMAT, CELERA_MSG_FORMAT};


static string  Bank_Name;
  // Name of read bank to be added to.
static bool  Create_New_Bank = false;
  // If true, then start a new read bank
static bool  Force_New_Bank = false;
  // If true, then force a new read bank by deleting existing one first
static Input_Format_t  Input_Format = FASTA_FORMAT;
  // Type of input
static char  * Input_File_1, * Input_File_2;
  // Names of input file(s)



static void  Parse_Command_Line
    (int argc, char * argv []);
static void  Usage
    (const char * command);



int  main
    (int argc, char * argv [])

  {
   ReadBank_t  read_bank;
   ID_t  iid;
   Read_t  read;
   Celera_Message_t  msg;
   int  ct;

   fprintf (stderr, "Starting on  %s  at  %s\n", __DATE__, __TIME__);

   Verbose = 0;

   Parse_Command_Line (argc, argv);

   cout << "Bank is " << Bank_Name << endl;

   if  (Force_New_Bank)
       {
        read_bank . openStore (Bank_Name);
        read_bank . closeStore ();
        read_bank . destroyStore ();
       }

   if  (Create_New_Bank)
       read_bank . createStore (Bank_Name, "silly comment");

   ct = 0;
   if  (Input_Format == CELERA_MSG_FORMAT)
       {
        FILE  * fp;
        fp = File_Open (Input_File_1, "r");
        while  (msg . read (fp))
          if  (msg . getMsgType () == FRG_MSG)
              {
               string  s;

               ct ++;
               read . setClearRange (msg . getClearRange ());
               read . setComment (msg . getSource ());
               read . setEID (msg . getAccession ());
               read . setSequence (msg . getSequence (), msg . getQuality ());
               iid = read_bank . append (read);
              }
       }
   else if  (Input_Format == FASTA_FORMAT)
       {
        FILE  * fp1, * fp2;
        string  s, q, tag_line1, tag_line2;
        char  id1 [MAX_LINE], id2 [MAX_LINE];

        fp1 = File_Open (Input_File_1, "r");
        fp2 = File_Open (Input_File_2, "r");

        while  (Fasta_Read (fp1, s, tag_line1))
          {
           char  * p1, * p2;
           int  len;

           Fasta_Qual_Read (fp2, q, tag_line2);
           strcpy (id1, tag_line1 . c_str ());
           strcpy (id2, tag_line2 . c_str ());
           p1 = strtok (id1, " \t\n");
           p2 = strtok (id2, " \t\n");
           if  (strcmp (p1, p2) != 0)
               {
                sprintf (Clean_Exit_Msg_Line, "seq id %s doesn't match quality id %s",
                     p1, p2);
                Clean_Exit (Clean_Exit_Msg_Line, __FILE__, __LINE__);
               }
           len = s . length ();
           if  (s . length () != q . length ())
               {
                sprintf (Clean_Exit_Msg_Line,
                     "seq length %d doesn't match quality length %d",
                     len, int (q . length ()));
                Clean_Exit (Clean_Exit_Msg_Line, __FILE__, __LINE__);
               }

           ct ++;
           read . setClearRange (0, len);
           read . setComment (p1);
           sprintf (id1, "%d", ct);
           read . setEID (id1);
           read . setSequence (s . c_str (), q . c_str ());
           iid = read_bank . append (read);
          }
       }
   fprintf (stderr, "Processed %d reads\n", ct);

   read_bank . commitStore ();
   read_bank . closeStore ();

#if  0    // test random access reads from store
{
 int  i, j;

   read_bank . openStore (Bank_Name);
   for  (i = 0;  i < 10;  i ++)
     {
      j = 1 + lrand48 () % ct;
      read = read_bank . fetch (j);
      read . Print (cout);
     }
   read_bank . closeStore ();
}
#endif   

   return  0;
  }



static void  Parse_Command_Line
    (int argc, char * argv [])

//  Get options and parameters from command line with  argc
//  arguments in  argv [0 .. (argc - 1)] .

  {
   bool  errflg = false;
   int  ch;

   optarg = NULL;

   while  (! errflg && ((ch = getopt (argc, argv, "cCfh")) != EOF))
     switch  (ch)
       {
        case  'c' :
          Create_New_Bank = true;
          break;

        case  'C' :
          Input_Format = CELERA_MSG_FORMAT;
          break;

        case  'f' :
          Force_New_Bank = true;
          break;

        case  'h' :
          errflg = true;
          break;

        case  '?' :
          fprintf (stderr, "Unrecognized option -%c\n", optopt);

        default :
          errflg = true;
       }

   if  (errflg)
       {
        Usage (argv [0]);
        exit (EXIT_FAILURE);
       }

   if  (Input_Format == CELERA_MSG_FORMAT && optind > argc - 2)
       {
        fprintf (stderr, "ERROR:  Must specify bank name and .frg file\n");
        Usage (argv [0]);
        exit (EXIT_FAILURE);
       }
   if  (Input_Format == FASTA_FORMAT && optind > argc - 3)
       {
        fprintf (stderr, "ERROR:  Must specify bank name, seq  and qual files\n");
        Usage (argv [0]);
        exit (EXIT_FAILURE);
       }

   Bank_Name = argv [optind ++];
   Input_File_1 = argv [optind ++];
   if  (Input_Format == FASTA_FORMAT)
       Input_File_2 = argv [optind ++];

   return;
  }



static void  Usage
    (const char * command)

//  Print to stderr description of options and command line for
//  this program.   command  is the command that was used to
//  invoke it.

  {
   fprintf (stderr,
           "USAGE:  %s  <bank-name> <input-file[s]>\n"
           "\n"
           "Add reads from specified file(s) to the readbank\n"
           "<bank-name>.  Default input format is fasta which\n"
           "is two separate files (sequence and quality).\n"
           "If Celera msg format is used, then only one file is\n"
           "specified.\n"
           "\n"
           "Options:\n"
           "  -c    Create a new read bank\n"
           "  -C    Input is Celera msg format, i.e., a .frg file\n"
           "  -f    Force new read bank by deleting existing one first\n"
           "  -h    Print this usage message\n"
           "\n",
           command);

   return;
  }
