/* @(#)state.hpp
 */

#ifndef _STATE_HPP
#define _STATE_HPP 1

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>

/*
  The numbering of the board is as follows:

   -------------
   | 0 | 1 | 2 |
   -------------
   | 3 | 4 | 5 |
   -------------
   | 8 | 7 | 6 |
   -------------

   SYNOPSIS
   State board;
   board.setField(1,1);
   assert(board.to_string() == "010000000");
   board.clear(); // start over
   
*/

class State {
private:
  /* holds state of board (see example at top of file)
     0 = empty
     1 = the first player placed a "token" here
     2 = the second player placed a "token" here */
  std::vector<unsigned char> m_fields;
  bool m_isTerminal = false;

  /* holds winner if terminal state has been reached
     0 = draw
     1 = 1 is winner
     2 = 2 is winner
     -1 = game is not over yet */
  char m_winner = -1;

  /* determines which player may play next.
     first player always goes first.
     1 = only first player may move
     2 = only second player may move */
  unsigned char m_nextMove = 1;

  // Checks if a terminal state has been entered
  // call after changing the board
  // may be costly
  void updateIsTerminal();

public:
  State() { m_fields.resize(9, 0); }

  // also allow initialization from string
  // careful: not all edge cases tested when updating from string
  State(const std::string &init);

  // getters
  // get the state of a field
  unsigned getField(unsigned field) const { return m_fields.at(field); }

  // test if the combination of field and value {1, 2} is valid
  bool validAction(unsigned field, unsigned value) const;

  // convert the board state to a string representation
  std::string to_string() const;

  // check if the board is in a terminal state (fast)
  bool isTerminal() const { return m_isTerminal; }

  // check if and if so, who has won
  int getWinner() const { return m_winner; }

  // create a vector of field indices that are still playable (see board diagram
  // at top of file)
  std::vector<unsigned> getValidFields() const;

  // who's turn is it?
  unsigned getNextPlayer() const { return m_nextMove; }

  // How many fields are still playable
  size_t getNumFreeFields() const {
    return count(m_fields.begin(), m_fields.end(), 0);
  }

  // setters
  // This is used to make a move on the board
  void setField(unsigned field, unsigned value);

  // reset board to initial conditions
  void clear();
};

#endif /* _STATE_HPP */
