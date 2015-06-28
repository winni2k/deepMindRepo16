/* @(#)agent.hpp
 */

#ifndef _AGENT_HPP
#define _AGENT_HPP 1

#include <algorithm>
#include <iostream>
#include <random>
#include "state.hpp"
#include "actVal.hpp"

namespace AgentHelper {

struct init {
  double epsilon = 0.5;
  double alpha = 0.1;
  double gamma = 0.6;
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
  std::pair<unsigned, unsigned> getAction(const State &board, float reward,
                                          bool learn = true);
  void setPlayerNum(unsigned pNum) {
    assert(pNum > 0);
    assert(pNum < 3);
    m_init.pNum = pNum;
  }

  // setters
  void setEpsilon(double ep) { m_init.epsilon = ep; }

  // getters
  // retrieve action value
  float getActVal(const State &board, unsigned field, unsigned pNum) const;

  // get learning parameters
  double getEpsilon() const { return m_init.epsilon; }
  double getAlpha() const { return m_init.alpha; }
  double getGamma() const { return m_init.gamma; }
  size_t getActValSize() const { return m_Q.size(); }
};

namespace AgentHelper {
// plays an episode between two agents
// rewards are rewards from last episode
// return is winner (1=player1, 2=player2, 0=draw)
// first agent always goes first
unsigned playEpisode(Agent &player1, Agent &player2, float &reward1,
                     float &reward2);
}

#endif /* _AGENT_HPP */
