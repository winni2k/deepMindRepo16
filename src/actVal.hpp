/* @(#)actVal.hpp
 */

#ifndef _ACTVAL_HPP
#define _ACTVAL_HPP 1

#include <unordered_map>
#include <string>
#include "state.hpp"

class ActValFunc {

private:
  std::unordered_map<std::string, float> m_actValTable;
  float m_default_val = 0;

public:
  // getters
  float getVal(const State &state, unsigned field, unsigned player) const;

  // setters
  void setVal(const State &state, unsigned field, unsigned player, float value);
};

namespace ActValFuncHelper {

std::string to_key(const State &state, unsigned field, unsigned player);
void checkValidAction(const State &state, unsigned field, unsigned player);
}

#endif /* _ACTVAL_HPP */
