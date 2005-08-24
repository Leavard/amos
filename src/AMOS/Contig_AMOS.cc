////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Adam M Phillippy
//! \date 10/23/2003
//!
//! \brief Source for Contig_t
//!
////////////////////////////////////////////////////////////////////////////////

#include "Contig_AMOS.hh"
#include <cctype>
using namespace AMOS;
using namespace std;




//================================================ Contig_t ====================
const NCode_t Contig_t::NCODE = M_CONTIG;


//----------------------------------------------------- gap2ungap --------------
Pos_t Contig_t::gap2ungap (Pos_t gap) const
{
  Pos_t retval = gap;

  for (Pos_t i = 0; i < gap; ++ i )
    if ( ! isalpha (getBase (i) . first) )
      -- retval;

  return retval;
}


//----------------------------------------------------- ungap2gap --------------
Size_t Contig_t::ungap2gap (Pos_t ungap) const
{
  Pos_t retval = ungap;

  for (Pos_t i = 0; i <= retval; ++ i )
    if ( ! isalpha (getBase (i) . first) )
      ++ retval;

  return retval;
}


//----------------------------------------------------- getSpan ----------------
Size_t Contig_t::getSpan ( ) const
{
  Pos_t hi,lo;


  if ( reads_m . empty( ) )
    {
      lo = hi = 0;
    }
  else
    {
      vector<Tile_t>::const_iterator ti = reads_m . begin( );

      lo = ti -> offset;
      hi = ti -> offset + ti -> range . getLength( );

      for ( ++ ti; ti != reads_m . end( ); ++ ti )
        {
          if ( ti -> offset < lo )
            lo = ti -> offset;
          if ( ti -> offset + ti -> range . getLength( ) > hi )
            hi = ti -> offset + ti -> range . getLength( );
        }
    }

  return hi - lo;
}


//----------------------------------------------------- getUngappedLength ------
Size_t Contig_t::getUngappedLength( ) const
{
  Size_t retval = length_m;

  for (Pos_t i = 0; i < length_m; ++ i )
    if ( ! isalpha (getBase (i) . first) )
      -- retval;

  return retval;
}


//----------------------------------------------------- getUngappedQualString --
string Contig_t::getUngappedQualString (Range_t range) const
{
  Pos_t lo = range . getLo( );
  Pos_t hi = range . getHi( );

  //-- Check preconditions
  if ( lo < 0  ||  hi > getLength( ) )
    AMOS_THROW_ARGUMENT ("Invalid quality subrange");

  string retval;
  retval . reserve (hi - lo);
  pair<char, char> seqqualc;

  //-- Skip the gaps in the sequence and populate the retval
  for ( ; lo < hi; lo ++ )
    {
      seqqualc = getBase (lo);
      if ( isalpha (seqqualc . first) )
        retval . push_back (seqqualc . second);
    }

  if ( range . isReverse( ) )
    AMOS::Reverse (retval);

  return retval;
}


//----------------------------------------------------- getUngappedSeqString ---
string Contig_t::getUngappedSeqString (Range_t range) const
{
  Pos_t lo = range . getLo( );
  Pos_t hi = range . getHi( );

  //-- Check preconditions
  if ( lo < 0  ||  hi > getLength( ) )
    AMOS_THROW_ARGUMENT ("Invalid sequence subrange");

  string retval;
  retval . reserve (hi - lo);
  char seqc;

  //-- Skip the gaps in the sequence and populate the retval
  for ( ; lo < hi; lo ++ )
    {
      seqc = getBase (lo) . first;
      if ( isalpha (seqc) )
        retval . push_back (seqc);
    }
  
  if ( range . isReverse( ) )
    AMOS::ReverseComplement (retval);

  return retval;
}



//----------------------------------------------------- insertGapColumn --------
void Contig_t::insertGapColumn (Pos_t gindex)
{
  // Insert gap into the consensus
  string qual(getQualString());
  string seq(getSeqString());

  if (gindex > seq.length())
  {
    AMOS_THROW_ARGUMENT("Invalid position specified for inserting gap column");
  }

  seq.insert(gindex, "-", 1); 
  qual.insert(gindex, "N", 1); // TODO: Set qv more intelligently
  setSequence(seq, qual);

  // Adjust the reads
  vector<Tile_t>::iterator i;
  for (i =  reads_m.begin();
       i != reads_m.end();
       i++)
  {
    if (i->offset >= gindex)
    {
      // insert before read, shift over 1
      // Don't insert starting gaps
      i->offset++;
    }
    else if (i->getRightOffset() < gindex)
    {
      // insert after read, nothing to do
    }
    else
    {
      // gap inserted into read
      int gseqpos = gindex - i->offset;

      // create a new vector of gaps, with a gap at gseqpos
      vector<Pos_t> newgaps; 

      // count gaps that occur before gap we are inserting
      int gapcount = 0;
      vector<Pos_t>::iterator g;
      for (g =  i->gaps.begin();
           g != i->gaps.end();
           g++, gapcount++)
      {
        int cgseqpos = *g+gapcount;
        if (cgseqpos > gseqpos) { break; }

        newgaps.push_back(*g);
      }

      // Add new gap
      newgaps.push_back(gseqpos-gapcount);

      // Copy gaps following the inserted gap
      while (g != i->gaps.end())
      {
        newgaps.push_back(*g);
        g++;
      }

      i->gaps = newgaps;
    }
  }
}



//----------------------------------------------------- readMessage ------------
void Contig_t::readMessage (const Message_t & msg)
{
  Sequence_t::readMessage (msg);

  try {
    vector<Message_t>::const_iterator i;

    for ( i  = msg . getSubMessages( ) . begin( );
          i != msg . getSubMessages( ) . end( ); i ++ )
      {
	if ( i -> getMessageCode( ) == M_TILE )
	  {
	    reads_m . push_back (Tile_t( ));
	    reads_m . back( ) . readMessage (*i);
	  }
      }
  }
  catch (ArgumentException_t) {
    
    clear( );
    throw;
  }

}


//----------------------------------------------------- readRecord -------------
void Contig_t::readRecord (istream & fix, istream & var)
{
  Sequence_t::readRecord (fix, var);

  Size_t sizet;
  readLE (fix, &sizet);

  reads_m . resize (sizet);
  for ( Pos_t i = 0; i < sizet; i ++ )
    reads_m [i] . readRecord (var);
}


//----------------------------------------------------- readUMD ----------------
bool Contig_t::readUMD (istream & in)
{
  string eid;
  Tile_t tile;
  istringstream ss;
  string line;

  while ( line . empty( )  ||  line [0] != 'C' )
    {
      if ( !in . good( ) )
	return false;
      getline (in, line);
    }

  clear( );

  try {

    ss . str (line);
    ss . ignore( );
    ss >> eid;
    if ( !ss )
      AMOS_THROW_ARGUMENT ("Invalid contig ID");
    ss . clear( );
    setEID (eid);

    while ( true )
      {
	getline (in, line);
	if ( line . empty( ) )
	  break;

	ss . str (line);
	ss >> tile . source;
	ss >> tile . range . begin;
	ss >> tile . range . end;
	if ( !ss )
	  AMOS_THROW_ARGUMENT ("Invalid read layout");
	ss . clear( );
	tile . offset = tile . range . begin < tile . range . end ?
	  tile . range . begin : tile . range . end;
	tile . range . begin -= tile . offset;
	tile . range . end -= tile . offset;
	getReadTiling( ) . push_back (tile);
      }

  }
  catch (IOException_t) {

    //-- Clean up and rethrow
    clear( );
    throw;
  }

  return true;
}

void Contig_t::reverseComplement()
{
  // Reverse the consensus
  string qual(getQualString());
  string seq(getSeqString());
  AMOS::ReverseComplement(seq);
  reverse(qual.begin(), qual.end());
  setSequence(seq, qual);

  // Flip the orientation of the reads
  vector<Tile_t>::iterator i;
  for (i =  reads_m.begin();
       i != reads_m.end();
       i++)
  {
    i->range.swap();
    i->offset = seq.length() - (i->offset + i->getGappedLength());

    Pos_t len = i->range.getLength();

    vector<Pos_t>::iterator g;
    for (g =  i->gaps.begin();
         g != i->gaps.end();
         g++)
    {
      *g = len - *g;
    }

    reverse(i->gaps.begin(), i->gaps.end());
  }
}


//----------------------------------------------------- writeMessage -----------
void Contig_t::writeMessage (Message_t & msg) const
{
  Sequence_t::writeMessage (msg);

  try {
    Pos_t begin = msg . getSubMessages( ) . size( );
    msg . getSubMessages( ) . resize (begin + reads_m . size( ));

    msg . setMessageCode (Contig_t::NCODE);

    if ( !reads_m . empty( ) )
      {
	vector<Tile_t>::const_iterator tvi;
        for ( tvi = reads_m . begin( ); tvi != reads_m . end( ); ++ tvi )
	  tvi -> writeMessage (msg . getSubMessages( ) [begin ++]);
      }
  }
  catch (ArgumentException_t) {

    msg . clear( );
    throw;
  }
}


//----------------------------------------------------- writeRecord ------------
void Contig_t::writeRecord (ostream & fix, ostream & var) const
{
  Sequence_t::writeRecord (fix, var);

  Size_t sizet = reads_m . size( );
  writeLE (fix, &sizet);

  for ( Pos_t i = 0; i < sizet; i ++ )
    reads_m [i] . writeRecord (var);
}


//----------------------------------------------------- writeUMD ---------------
void Contig_t::writeUMD (ostream & out) const
{
  vector<Tile_t>::const_iterator ti;

  out << "C " << getEID( ) << endl;

  for ( ti = reads_m . begin( ); ti != reads_m . end( ); ti ++ )
    out << ti -> source << ' '
	<< ti -> range . begin + ti -> offset << ' '
	<< ti -> range . end + ti -> offset << endl;

  out << endl;

  if ( !out . good( ) )
    AMOS_THROW_IO ("UMD message write failure");
}
