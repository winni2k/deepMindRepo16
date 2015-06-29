#include "agent.hpp"

using namespace std;

namespace AgentHelper {

// plays an episode between two agents
// rewards are rewards from last episode
// return is winner (1=player1, 2=player2, 0=draw)
// first agent always goes first
unsigned playEpisode(Agent &player1, Agent &player2, float &reward1,
                     float &reward2) {

  State board;
  // run through a whole episode

  // make sure player order is set correctly
  player1.setPlayerNum(1);
  player2.setPlayerNum(2);
  unsigned pNumToGo = 1;
  while (!board.isTerminal()) {
    std::pair<unsigned, unsigned> action = std::make_pair(9, 9);

    // get player that is next to perform an action
    if (pNumToGo == 1) {
      action = player1.getAction(board, reward1);
      reward1 = 0;
    } else {
      action = player2.getAction(board, reward2);
      reward2 = 0;
    }

    // translate action to board
    board.setField(action.first, action.second);
    pNumToGo = pNumToGo == 1 ? 2 : 1;
  }

  // figure out who the winner is and set rewards
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

// constructor
// checks parameters for correctness
Agent::Agent(AgentHelper::init init)
    : m_init(std::move(init)),
      m_unifReal(uniform_real_distribution<float>(0.0, 1.0)) {
  assert(m_init.epsilon <= 1);
  assert(m_init.epsilon >= 0);
  assert(m_init.alphaScaleFactor >= 0);
  assert(m_init.gamma <= 1);
  assert(m_init.gamma >= 0);
  assert(m_init.pNum > 0);
  assert(m_init.pNum < 3);
}

/*
  Implementation according to figure 6.9 (Sarsa)
  at http://webdocs.cs.ualberta.ca/~sutton/book/ebook/node64.html

  pass in new state (s') + reward from previous action (a)
  set learn = false to turn off learning

  initialize with learnAlg = AgentHelper::QLearn to use Q-learning
  instead of SARSA
  Q-learning implementation according to figure 6.12 (Q-Learning)
  at http://webdocs.cs.ualberta.ca/~sutton/book/ebook/node65.html
*/
pair<unsigned, unsigned> Agent::getAction(const State &board, float reward,
                                          bool learn) {

  // check if we have started new episode
  const size_t nff = board.getNumFreeFields();
  bool newEpisode = false;
  if (nff == 9 || nff == 8)
    newEpisode = true;

  // action to return (invalid initial values)
  pair<unsigned, unsigned> action = make_pair(9, 0);

  // if this is the first call ever
  // take action a, ignore reward
  if (m_t == 0) {
    m_a1 = chooseAction(board);
    m_s1 = board;
    m_p1 = m_init.pNum;

    action.first = m_a1;
    action.second = m_p1;
  }
  // observe r(reward), s'(board)
  else if (m_init.learnAlg == AgentHelper::LearnAlgorithm::QLearn) {
    // update Q(s,a)
    float q1 = m_Q.getVal(m_s1, m_a1, m_p1);

    // find action a'(a2) that maximizes Q(s',a')
    // however, if s' is terminal, then Q(s',a') = 0
    float q2 = 0;
    if (!newEpisode) {
      unsigned a2 = chooseAction(board, true);
      q2 = m_Q.getVal(board, a2, m_init.pNum);
    }

    // set alpha as 1/m_t, plus some tweaks to allow faster learning
    m_alpha = min(1.0, m_init.alphaScaleFactor / m_t);

    // update Q(a,s)
    if (learn) {
      float newVal = q1 + m_alpha * (reward + m_init.gamma * q2 - q1);
      m_Q.setVal(m_s1, m_a1, m_p1, newVal);
    }
    // s <- s'
    m_s1 = board;

    // choose a(m_a1, m_p1) from s(board)
    m_a1 = chooseAction(board);
    m_p1 = m_init.pNum;

    action.first = m_a1;
    action.second = m_p1;
  }
  // observe r(reward), s'(board)
  else if (m_init.learnAlg == AgentHelper::LearnAlgorithm::SARSA) {
    // Choose a' from s'(board) using policy derived from Q
    unsigned a2 = chooseAction(board);

    // q1 = Q(s,a)
    // q2 = Q(s',a')
    float q1 = m_Q.getVal(m_s1, m_a1, m_p1);

    // if s' is terminal, Q(s',a') = 0
    float q2 = newEpisode ? 0 : m_Q.getVal(board, a2, m_init.pNum);

    // set alpha as 1/m_t, plus some tweaks to allow faster learning
    m_alpha = min(1.0, m_init.alphaScaleFactor / m_t);

    // update Q(s,a)
    if (learn) {
      float newVal = q1 + m_alpha * (reward + m_init.gamma * q2 - q1);
      m_Q.setVal(m_s1, m_a1, m_p1, newVal);
    }

    // s <- s'; a <- a'
    m_a1 = a2;
    m_s1 = board;
    m_p1 = m_init.pNum;

    // take action a
    action.first = a2;
    action.second = m_p1;
  } else
    throw logic_error("This learning algorithm has not been implemented yet.");

  ++m_t;
  return action;
}

// choose a state based on Q
unsigned Agent::chooseAction(const State &board, bool onlyGreedy) {

  auto validFields = board.getValidFields();

  assert(!validFields.empty());

  unsigned field = validFields[0];
  // using epsilon greedy method
  if (!onlyGreedy && m_unifReal(m_generator) < m_init.epsilon) {
    uniform_int_distribution<unsigned> dist(0, validFields.size() - 1);
    field = validFields[dist(m_generator)];
  }
  // pick a greedy action
  else {

    // pick at random from best options
    // this helps the agent explore more equally good possibilities
    vector<float> vals;
    float max = -numeric_limits<float>::max();
    for (auto f : validFields) {
      vals.push_back(m_Q.getVal(board, f, m_init.pNum));
      max = max < vals.back() ? vals.back() : max;
    }

    // figure out how many values with max val there are
    unsigned numMax = count(vals.begin(), vals.end(), max);
    assert(numMax > 0);

    // pick one of those max vals at random
    std::uniform_int_distribution<unsigned> dist(0, numMax - 1);
    unsigned chosenAction = dist(m_generator);

    // iterate through vals again until the correct max val is found
    for (size_t i = 0; i < vals.size(); ++i) {
      if (vals[i] == max) {
        if (numMax == 1) {
          field = validFields[i];
          break;
        } else
          --numMax;
      }
    }
  }
  return field;
}

float Agent::getActVal(const State &board, unsigned field,
                       unsigned pNum) const {

  assert(field < 9);
  assert(pNum < 3);
  assert(pNum > 0);
  return m_Q.getVal(board, field, pNum);
}
