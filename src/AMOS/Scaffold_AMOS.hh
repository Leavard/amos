////////////////////////////////////////////////////////////////////////////////
//! \file
//! \author Adam M Phillippy
//! \date 06/18/2003
//!
//! \brief Header for Scaffold_t
//!
////////////////////////////////////////////////////////////////////////////////

#ifndef __Scaffold_AMOS_HH
#define __Scaffold_AMOS_HH 1

#include "Universal_AMOS.hh"
#include <vector>




namespace AMOS {

//================================================ Scaffold_t ==================
//! \brief A ordered, oriented and positioned list of contigs
//!
//! A list of contigs that are ordered, oriented and positioned in
//! relation to one another.
//!
//==============================================================================
class Scaffold_t : public Universal_t
{

private:

  std::vector<Tile_t> contigs_m;   //!< contig tiling
  std::vector<ID_t> edges_m;       //!< list of contig edges


protected:

  //--------------------------------------------------- readRecord -------------
  virtual void readRecord (std::istream & fix,
			   std::istream & var);


  //--------------------------------------------------- writeRecord ------------
  virtual void writeRecord (std::ostream & fix,
			    std::ostream & var) const;


public:

  //--------------------------------------------------- NCode ------------------
  //! \brief Get the AMOS NCode type identifier (statically)
  //!
  //! Can be used for constructing a Bank with a certain NCode. e.g. 'Bank_t
  //! (Scaffold_t::NCode( ))'
  //!
  //! \return The AMOS NCode type identifier
  //!
  static NCode_t NCode ( )
  {
    return Bank_k::SCAFFOLD;
  }


  //--------------------------------------------------- Scaffold_t -------------
  //! \brief Constructs an empty Scaffold_t object
  //!
  Scaffold_t ( )
  {

  }


  //--------------------------------------------------- Scaffold_t -------------
  //! \brief Copy constructor
  //!
  Scaffold_t (const Scaffold_t & source)
  {
    *this = source;
  }


  //--------------------------------------------------- ~Scaffold_t ------------
  //! \brief Destroys a Scaffold_t object
  //!
  ~Scaffold_t ( )
  {

  }


  //--------------------------------------------------- clear ------------------
  virtual void clear ( )
  {
    Universal_t::clear( );
    contigs_m . clear( );
    edges_m . clear( );
  }


  //--------------------------------------------------- getContigTiling --------
  //! \brief Get the tiling of underlying contigs
  //!
  //! \return The vector of underlying contigs
  //!
  const std::vector<Tile_t> & getContigTiling ( ) const
  {
    return contigs_m;
  }


  //--------------------------------------------------- getContigTiling --------
  //! \brief Get the tiling of underlying contigs
  //!
  //! \return The vector of underlying contigs
  //!
  std::vector<Tile_t> & getContigTiling ( )
  {
    return contigs_m;
  }


  //--------------------------------------------------- getContigEdges ---------
  //! \brief Get the contig edge IIDs
  //!
  //! \return The vector of contig edge IIDs
  //!
  const std::vector<ID_t> & getContigEdges ( ) const
  {
    return edges_m;
  }


  //--------------------------------------------------- getContigEdges ---------
  //! \brief Get the contig edge IIDs
  //!
  //! \return The vector of contig edge IIDs
  //!
  std::vector<ID_t> & getContigEdges ( )
  {
    return edges_m;
  }


  //--------------------------------------------------- getNCode ---------------
  virtual NCode_t getNCode ( ) const
  {
    return Scaffold_t::NCode( );
  }


  //--------------------------------------------------- readMessage ------------
  virtual void readMessage (const Message_t & msg);


  //--------------------------------------------------- setContigTiling --------
  //! \brief Set the tiling of underlying contigs
  //!
  //! \param contigs The new vector of underlying contigs
  //! \return void
  //!
  void setContigTiling (const std::vector<Tile_t> & contigs)
  {
    contigs_m = contigs;
  }


  //--------------------------------------------------- setContigEdges ---------
  //! \brief Set the contig edge IIDs
  //!
  //! \param edges The new vector of contig edge IIDs
  //! \return void
  //!
  void setContigEdges (const std::vector<ID_t> & edges)
  {
    edges_m = edges;
  }


  //--------------------------------------------------- writeMessage -----------
  virtual void writeMessage (Message_t & msg) const;

};

} // namespace AMOS

#endif // #ifndef __Scaffold_AMOS_HH
