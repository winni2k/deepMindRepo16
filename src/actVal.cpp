#include "actVal.hpp"

using namespace std;

namespace ActValFuncHelper {
std::string to_key(const State &state, unsigned field, unsigned player) {
  return state.to_string() + ":" + to_string(field) + ":" + to_string(player);
}

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

void ActValFunc::setVal(const State &state, unsigned field, unsigned player,
                        float value) {
  checkValidAction(state, field, player);
  m_actValTable[to_key(state, field, player)] = value;
}
