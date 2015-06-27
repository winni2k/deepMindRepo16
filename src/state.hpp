/* @(#)state.hpp
 */

#ifndef _STATE_HPP
#define _STATE_HPP 1

#include <cassert>
#include <stdexcept>
#include <string>
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
  void setField(unsigned field, unsigned value) {
    if (value > 2)
      throw std::runtime_error("Attempted to set field to invalid value: " +
                               std::to_string(value));
    if (field > 8)
      throw std::range_error("Tried to set non-existent field: " +
                             std::to_string(field));
    if (m_fields[field])
      throw std::runtime_error("A field can only be set once");
    m_fields[field] = static_cast<unsigned char>(value);
  }
  std::string to_string() {
    std::string ret;
    for (auto f : m_fields) {
      assert(f < 10);
      ret += std::to_string(f);
    }
    return ret;
  }
};

#endif /* _STATE_HPP */
