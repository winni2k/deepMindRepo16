/* @(#)agent.hpp
 */

#ifndef _AGENT_HPP
#define _AGENT_HPP 1

#include <random>
#include "state.hpp"
#include "actVal.hpp"

namespace AgentHelper {

struct init {
  double epsilon = 0.1;
  double alpha = 0.1;
  double gamma = 0.1;
  unsigned pNum = 1;
};
}

class Agent {
private:
  AgentHelper::init m_init;
  std::default_random_engine m_generator;

  // action-value function
  ActValFunc m_Q;

  // action (a)
  unsigned m_a1 = 9, m_p1 = m_init.pNum;

  // state (s)
  State m_s1;

  // time total
  unsigned m_t = 0;

  // uniform real distribution for sampling
  std::uniform_real_distribution<float> m_unifReal;

  unsigned chooseAction(const State &board);

public:
  Agent(AgentHelper::init init);

  // return: first unsigned is field and second unsigned is what to
  // place on that field of the board (1 or 2)
  std::pair<unsigned, unsigned> getAction(const State &board, float reward);
  void setPlayerNum(unsigned pNum) {
    assert(pNum > 0);
    assert(pNum < 3);
    m_init.pNum = pNum;
  }

  // getters
  // retrieve action value
  float getActVal(const State &board, unsigned field, unsigned pNum) const;

  // get learning parameters
  float getEpsilon() const { return m_init.epsilon; }
  float getAlpha() const { return m_init.alpha; }
  float getGamma() const { return m_init.gamma; }
};

namespace AgentHelper {

// plays an episode between two agents
// rewards are rewards from last episode
// return is winner (1=player1, 2=player2, 0=draw)
unsigned playEpisode(unsigned pNumToGo, Agent &player1, Agent &player2,
                     float &reward1, float &reward2) {

  State board;
  // run through a whole episode
  // randomly select first player
  player1.setPlayerNum(pNumToGo);
  player2.setPlayerNum(pNumToGo == 1 ? 2 : 1);
  while (!board.isTerminal()) {
    std::pair<unsigned, unsigned> action = std::make_pair(9, 9);
    if (pNumToGo == 1) {
      action = player1.getAction(board, reward1);
      reward1 = 0;
    } else {
      action = player2.getAction(board, reward2);
      reward2 = 0;
    }
    board.setField(action.first, action.second);
    pNumToGo = pNumToGo == 1 ? 2 : 1;
  }
  int winner = board.getWinner();
  if (winner == 1) {
    reward1 = 1;
    reward2 = -1;
  } else if (winner == 2) {
    reward1 = -1;
    reward2 = 1;
  } else
    reward1 = reward2 = 0;
  return winner;
}
}

#endif /* _AGENT_HPP */
