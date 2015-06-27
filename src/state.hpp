/* @(#)state.hpp
 */

#ifndef _STATE_HPP
#define _STATE_HPP 1

#include <stdexcept>
#include <vector>

class State {
private:
  /* holds state of board
     0 = empty
     1 = cross
     2 = circle */
  std::vector<unsigned char> m_fields;

public:
  State() { m_fields.resize(9, 0); }

  unsigned getField(unsigned field) const { return m_fields.at(field); }
};

#endif /* _STATE_HPP */
