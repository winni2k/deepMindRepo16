#include "agent.hpp"

using namespace std;

Agent::Agent(AgentHelper::init init)
    : m_init(std::move(init)),
      m_unifReal(uniform_real_distribution<float>(0.0, 1.0)) {
  assert(m_init.epsilon <= 1);
  assert(m_init.epsilon >= 0);
  assert(m_init.pNum > 0);
  assert(m_init.pNum < 3);

  // choose first action at random
  //  m_a1 =
}

pair<unsigned, unsigned> Agent::getAction(const State &board) {

  unsigned action = chooseAction(board);
  return make_pair(action, m_init.pNum);
}

// choose a state based on Q
unsigned Agent::chooseAction(const State &board) {

  auto validFields = board.getValidFields();
  assert(!validFields.empty());

  unsigned action = validFields[0];
  // using epsilon greedy method
  if (m_unifReal(m_generator) < m_init.epsilon) {
    uniform_int_distribution<unsigned> dist(0, validFields.size() - 1);
    action = validFields[dist(m_generator)];
  }
  // pick a greedy action
  else {
    float value = m_Q.getVal(board, action, m_init.pNum);
    for (auto f : validFields) {
      float newVal = m_Q.getVal(board, f, m_init.pNum);
      if (newVal > value) {
        action = f;
        value = newVal;
      }
    }
  }
  return action;
}
