////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Adam M Phillippy
//! \date 09/24/2003
//!
//! \brief Source for Universal_t
//!
////////////////////////////////////////////////////////////////////////////////

#include "Universal_AMOS.hh"
using namespace AMOS;
using namespace Message_k;
using namespace std;




//================================================ Universal_t =================
//----------------------------------------------------- readMessage ------------
void Universal_t::readMessage (const Message_t & msg)
{
  clear( );

  try {

    if ( msg . exists (F_EID) )
      {
	istringstream ss; 

	ss . str (msg . getField (F_EID));
	ss >> eid_m;
	if ( !ss )
	  AMOS_THROW_ARGUMENT ("Invalid eid format");
	ss . clear( );
      }
    
    if ( msg . exists (F_COMMENT) )
      setComment (msg . getField (F_COMMENT));
  }
  catch (ArgumentException_t) {
    
    clear( );
    throw;
  }
}


//----------------------------------------------------- writeMessage -----------
void Universal_t::writeMessage (Message_t & msg) const
{
  msg . clear( );

  try {

    msg . setMessageCode (NCode( ));

    if ( eid_m != NULL_ID )
      {
	ostringstream ss;

	ss << eid_m;
	msg . setField (F_EID, ss . str( ));
	ss . str("");
      }

    if ( !comment_m . empty( ) )
      msg . setField (F_COMMENT, comment_m);
  }
  catch (ArgumentException_t) {

    msg . clear( );
    throw;
  }
}
