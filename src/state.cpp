#include "state.hpp"

using namespace std;

// This constructor assumes the input is a valid state.
// There are edge cases that are not tested (for example, multiple winners)
// so use with care.
State::State(const std::string &init) : State() {

  assert(init.size() == m_fields.size());
  for (size_t i = 0; i < init.size(); i++) {
    int in = init[i] - '0';
    assert(in > -1 && in < 3);
    m_fields[i] = static_cast<unsigned char>(in);
  }
  updateIsTerminal();
}

bool State::validAction(unsigned field, unsigned value) const {

  if (isTerminal())
    return false;
  if (value < 1 || value > 2)
    return false;
  if (field >= m_fields.size())
    return false;
  if (m_fields[field])
    return false;
  if (m_nextMove && value != m_nextMove)
    return false;
  return true;
}

std::string State::to_string() const {
  std::string ret;
  for (auto f : m_fields) {
    assert(f < 10);
    ret += std::to_string(f);
  }
  return ret;
}

// check the state to see if a row of three has been reached, or if
// the board is full
void State::updateIsTerminal() {
  assert(m_isTerminal == false);

  // find winner
  // check possible three in a lines: horizontal and vertical
  for (size_t i = 0; i < 3; ++i) {
    if (m_fields[i] != 0 && m_fields[i] == m_fields[i + 3] &&
        m_fields[i] == m_fields[i + 6])
      m_winner = m_fields[i];
    else if (m_fields[i * 3] != 0 && m_fields[i * 3] == m_fields[i * 3 + 1] &&
             m_fields[i * 3] == m_fields[i * 3 + 2])
      m_winner = m_fields[i * 3];
  }
  if (m_winner > 0) {
    m_isTerminal = true;
    return;
  }
  // check diagonals
  if (m_fields[4] != 0 &&
      ((m_fields[0] == m_fields[4] && m_fields[0] == m_fields[8]) ||
       (m_fields[2] == m_fields[4] && m_fields[2] == m_fields[6]))) {
    m_winner = m_fields[4];
    m_isTerminal = true;
    return;
  }

  // if no winner and board full, then it is a draw
  if (*(min_element(m_fields.begin(), m_fields.end())) > 0) {
    m_winner = 0;
    m_isTerminal = true;
    return;
  }
}

// sets a field to "player" (1 or 2)
void State::setField(unsigned field, unsigned player) {
  if (validAction(field, player))
    m_fields[field] = static_cast<unsigned char>(player);
  else
    throw runtime_error("Invalid action: " + std::to_string(field) + "," +
                        std::to_string(player));

  // update next move to other player
  m_nextMove = player == 1 ? 2 : 1;

  // check to see if we are in a terminal state now
  updateIsTerminal();
}

// returns list of valid fields to make a move to
vector<unsigned> State::getValidFields() const {

  vector<unsigned> validFields;

  // return empty vector if we are in terminal state
  if (isTerminal())
    return validFields;

  // fill vector with possible moves
  for (size_t i = 0; i < m_fields.size(); ++i)
    if (!m_fields[i])
      validFields.push_back(i);

  return validFields;
}
