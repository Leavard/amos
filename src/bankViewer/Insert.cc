#include "Insert.hh"

using namespace AMOS;
using namespace std;

int omin (int a, int b)
{
  return (a < b) ? a : b;
}

int omax (int a, int b)
{
  return (a > b) ? a : b;
}

Insert::Insert(Tile_t * atile, 
               ID_t     aid,
               ID_t     acontig,
               Tile_t * btile,
               ID_t     bid,
               ID_t     bcontig,
               Distribution_t dist,
               int conslen)
{
  m_atile   = atile;
  m_aid     = aid;
  m_acontig = acontig;

  m_btile   = btile;
  m_bid     = bid;
  m_bcontig = bcontig;

  m_state = Unknown;
  m_dist = dist;
  m_active = -1;

  m_length = m_dist.mean;

  m_arc = 0;
  m_brc = 0;

  if (atile) { m_arc = (atile->range.end < atile->range.begin); }
  if (btile) { m_brc = (btile->range.end < btile->range.begin); }
  
  if (atile && btile)
  {
    m_loffset = omin(atile->offset, btile->offset);

    m_roffset = omax(atile->offset + atile->range.getLength() + atile->gaps.size() -1,
                     btile->offset + btile->range.getLength() + btile->gaps.size() -1);

    m_length = m_roffset - m_loffset + 1;

    if (abs(m_length - dist.mean) <= 3*dist.sd)
    {
      m_state = Happy;
      m_active = 2;
    }
    else
    {
      m_state = SizeViolation;
    }

    if (m_arc + m_brc != 1)
    {
      m_state = OrientationViolation;
    }
  }
  else if (atile)
  {
    m_state = LinkingMate;
    m_loffset = atile->offset;
    m_roffset = atile->offset + atile->range.getLength() + atile->gaps.size() -1;

    if (bid == NULL_ID)
    {
      m_state = NoMate;
      m_length = m_roffset - m_loffset + 1;
      m_active = 0;
    }
    else
    {
      int projected = getProjectedPosition(atile, m_dist);

      if (m_arc)
      { if (projected > 0)       { m_state = MissingMate; } }
      else
      { if (projected < conslen) { m_state = MissingMate; } }
    }
  }
  else if (btile)
  {
    m_state = LinkingMate;
    m_loffset = btile->offset;
    m_roffset = btile->offset + btile->range.getLength() + btile->gaps.size() -1;

    int projected = getProjectedPosition(btile, m_dist);

    if (m_brc)
    { if (projected > 0)       { m_state = MissingMate; } }
    else
    { if (projected < conslen) { m_state = MissingMate; } }
  }
  else
  {
    throw "No Tile!";
  }

  m_actual = m_roffset - m_loffset + 1;
  m_other = NULL;
  m_canvasItem = NULL;
}

void Insert::printInsert() const
{
  cerr << m_aid << "," << m_bid << " l:" << m_loffset << " r:" << m_roffset << " s:" << (char)m_state << endl;
}

int Insert::getProjectedPosition(Tile_t * tile, Distribution_t dist)
{
  const int READLEN = 500;

  if (tile->range.end < tile->range.begin)
  {
    return tile->offset + tile->range.getLength() + tile->gaps.size() - 1 - dist.mean - 3*dist.sd - READLEN;
  }
  else
  {
    return tile->offset + dist.mean + 3*dist.sd + READLEN;
  }
}

void Insert::setActive(int i, Insert * other)
{
  Tile_t * tile;
  m_other = other;

  m_active = i;

  if (i == 0) { tile = m_atile; }
  else        { tile = m_btile; }

  int len = tile->range.getLength() + tile->gaps.size()-1;

  if (tile->range.isReverse())
  {
    m_roffset = tile->offset + len;

    if (m_dist.mean > len)
    {
      m_loffset = m_roffset - m_dist.mean;
    }
    else
    {
      m_loffset = tile->offset - 250;
    }
  }
  else
  {
    m_loffset = tile->offset;

    if (m_dist.mean > len)
    {
      m_roffset = m_loffset + m_dist.mean;
    }
    else
    {
      m_roffset = m_loffset + len + 250;
    }
  }

  m_length = (m_roffset - m_loffset);
}
