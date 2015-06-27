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
  unsigned t = 0;
  // time in episode;
  unsigned t_episode = 0;

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
};

#endif /* _AGENT_HPP */
