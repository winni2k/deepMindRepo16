/* @(#)state.hpp
 */

#ifndef _STATE_HPP
#define _STATE_HPP 1

#include <algorithm>
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
  bool m_isTerminal = false;
  /* holds winner if terminal state has been reached
     0 = draw
     1 = 1 is winner
     2 = 2 is winner
     -1 = game is not over yet */
  char m_winner = -1;

  /* states which player may play next
     1 = only player 1 may move
     2 = only player 2 may move */
  unsigned char m_nextMove = 1;

  // for checking if a terminal state has been entered
  // may be costly
  void updateIsTerminal();

public:
  State() { m_fields.resize(9, 0); }
  // also allow initialization from string
  State(const std::string &init);

  // getters
  unsigned getField(unsigned field) const { return m_fields.at(field); }
  bool validAction(unsigned field, unsigned value) const;
  std::string to_string() const;
  bool isTerminal() const { return m_isTerminal; }
  int getWinner() const { return m_winner; }
  std::vector<unsigned> getValidFields() const;
  unsigned getNextPlayer() const { return m_nextMove; }
  size_t getNumFreeFields() const {
    return count(m_fields.begin(), m_fields.end(), 0);
  }

  // setters
  void setField(unsigned field, unsigned value);

  // reset board to initial conditions
  void clear();
};

#endif /* _STATE_HPP */
