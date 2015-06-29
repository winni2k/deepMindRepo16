/* @(#)actVal.hpp
 */

#ifndef _ACTVAL_HPP
#define _ACTVAL_HPP 1

#include <unordered_map>
#include <string>
#include "state.hpp"

/*
  ActValFunc: Class for storing action values
  Stores a value for each combination of board state, action and player (first
  or second).
  Implementation is by hash, but this could be changed


*/

class ActValFunc {

private:
  std::unordered_map<std::string, float> m_actValTable;
  float m_default_val = 0;
  unsigned chooseAction(const State &board) const;

public:
  // getters
  // get value of a board state, move and player number combination
  float getVal(const State &state, unsigned field, unsigned player) const;
  size_t size() const { return m_actValTable.size(); }

  // setters
  // store a value for a combination of state, field and player
  // will throw if invalid combination is supplied
  void setVal(const State &state, unsigned field, unsigned player, float value);
};

namespace ActValFuncHelper {

// converts state, field and player combination to a string representation used
// by ActValFunc for storing action value pairs.
std::string to_key(const State &state, unsigned field, unsigned player);

// check if an state, field and player combination constitutes a valid action
// throws if not valid
// most users will want to use State directly for checking. For example:
// state.validAction(field, player) == true; 
void checkValidAction(const State &state, unsigned field, unsigned player);
}

#endif /* _ACTVAL_HPP */
