#include "agent.hpp"

using namespace std;

Agent::Agent(AgentHelper::init init)
    : m_init(std::move(init)),
      m_unifReal(uniform_real_distribution<float>(0.0, 1.0)) {
  assert(m_init.epsilon <= 1);
  assert(m_init.epsilon >= 0);
  assert(m_init.pNum > 0);
  assert(m_init.pNum < 3);
}

/*
  Implementation according to figure 6.9 (Sarsa)
  at http://webdocs.cs.ualberta.ca/~sutton/book/ebook/node64.html

  pass in new state (s') + reward from previous action (a)
*/
pair<unsigned, unsigned> Agent::getAction(const State &board, float reward) {

  size_t numFreeFields = board.getNumFreeFields();

  // action to return (invalid initial values)
  pair<unsigned, unsigned> action = make_pair(9, 0);

  // if this is the first call
  // Take action a
  if ((m_init.pNum == 1 && numFreeFields == 9) ||
      (m_init.pNum == 2 && numFreeFields == 8)) {
    t_episode = 0;
    m_a1 = chooseAction(board);
    m_s1 = board;
    m_p1 = m_init.pNum;

    action.first = m_a1;
    action.second = m_init.pNum;
  }
  // observe r(reward), s'(board)
  else {
    // Choose a' from s'(board) using policy derived from Q
    unsigned a2 = chooseAction(board);

    // q1 = Q(s,a)
    // q2 = Q(s',a')
    float q1 = m_Q.getVal(m_s1, m_a1, m_p1);

    // if s' is terminal, Q(s',a') = 0
    float q2 = board.isTerminal() ? 0 : m_Q.getVal(board, a2, m_init.pNum);

    // update Q(s,a)
    float newVal = q1 + m_init.alpha * (reward + m_init.gamma * q2 - q1);
    m_Q.setVal(m_s1, m_a1, m_p1, newVal);

    // s <- s'; a <- a'
    m_a1 = a2;
    m_s1 = board;
    m_p1 = m_init.pNum;

    // take action a
    action.first = a2;
    action.second = m_init.pNum;
  }

  ++t;
  return action;
}

// choose a state based on Q
unsigned Agent::chooseAction(const State &board) {

  auto validFields = board.getValidFields();

  assert(!validFields.empty());

  unsigned field = validFields[0];
  // using epsilon greedy method
  if (m_unifReal(m_generator) < m_init.epsilon) {
    uniform_int_distribution<unsigned> dist(0, validFields.size() - 1);
    field = validFields[dist(m_generator)];
  }
  // pick a greedy action
  else {
    float value = m_Q.getVal(board, field, m_init.pNum);
    for (auto f : validFields) {
      float newVal = m_Q.getVal(board, f, m_init.pNum);
      if (newVal > value) {
        field = f;
        value = newVal;
      }
    }
  }
  return field;
}
