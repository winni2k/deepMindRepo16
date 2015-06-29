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

enum class LearnAlgorithm { SARSA, QLearn };

struct init {
  double epsilon = 0.5;
  double alphaScaleFactor = 10000;
  double gamma = 0.6;
  unsigned pNum = 1; // is this player going first (1) or second (2)?
  LearnAlgorithm learnAlg = LearnAlgorithm::SARSA;
};
}

/* Agent Class: implements a tictactoe player

   An Agent object is always initialized with a AgentHelper::init struct.  The
   struct may be modified before passing it to the Agent constructor to change
   parameters of the Agent object. For example:

   AgentHelper::init init;
   init.epsilon = 0.1;
   Agent player(init);
 */

class Agent {
private:
  AgentHelper::init m_init;
  std::default_random_engine m_generator;

  // current value of alpha
  // set by getActVal()
  double m_alpha = 1;

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

  // Chooses epsilon-greedy action based on s(board) and m_Q
  // set onlyGreedy = true to always make a greedy choice
  unsigned chooseAction(const State &board, bool onlyGreedy = false);

public:
  Agent(AgentHelper::init init);

  /*
   return: first unsigned is field and second unsigned is what to
   place on that field of the board (1 or 2)
   The reward passed in is always the reward resulting from the previous
   action. The first reward is disregarded. For no reward, passi n 0. For
   example:

   AgentHelper::init init;
   Agent p1(init);
   State board;
   auto act = getAction(board, 0);

   // ... act was a good move! give a positive reward

   float reward = 10;
   board.setField(act.first, act.second);
   act = p1.getAction(board, reward);

   // ... and so on ...

  */
  std::pair<unsigned, unsigned> getAction(const State &board, float reward,
                                          bool learn = true);

  // changes the player number of the object
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
  double getAlpha() const { return m_alpha; }
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
