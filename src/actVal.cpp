#include "actVal.hpp"

using namespace std;

namespace ActValFuncHelper {
std::string to_key(const State &state, unsigned field, unsigned player) {
  assert(field < 10);
  assert(player < 10);
  string ret;
  ret.reserve(13);
  ret = state.to_string() + ":";
  ret += ('0' + field);
  ret += ":";
  ret += ('0' + player);
  return ret;
}

// check if the field and player constitute a valid action for the state
void checkValidAction(const State &state, unsigned field, unsigned player) {
  if (!state.validAction(field, player))
    throw runtime_error("Invalid action: field [" + to_string(field) +
                        "] player [" + to_string(player) + "]");
}
}

using namespace ActValFuncHelper;

// retrieve action value
float ActValFunc::getVal(const State &state, unsigned field,
                         unsigned player) const {

  // check action is valid given state
  checkValidAction(state, field, player);

  // retrieve action value
  auto ret = m_actValTable.find(to_key(state, field, player));

  // return default value if the action value has not been set yet
  if (ret == m_actValTable.end())
    return m_default_val;
  return ret->second;
}

// set action value
void ActValFunc::setVal(const State &state, unsigned field, unsigned player,
                        float value) {
  checkValidAction(state, field, player);
  m_actValTable[to_key(state, field, player)] = value;
}
