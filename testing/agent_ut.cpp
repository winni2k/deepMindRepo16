

#include "agent.hpp"
#include "gtest/gtest.h"

// tests for the state object

using namespace std;

// make sure the state object builds and is initialized properly
TEST(AgentClass, builds) {

  AgentHelper::init init;
  Agent player(init);
  State board;

  // run a few actions
  for (int i = 0; i < 100; ++i) {
    auto action = player.getAction(board);
    EXPECT_LE(0, action.first);
    EXPECT_GE(8, action.first);
    EXPECT_EQ(init.pNum, action.second);
  }
}


// tests to see if the agents can play tictactoe without throwing errors...
TEST(AgentClass, plays) {

  AgentHelper::init init;
  Agent player1(init);
  init.pNum = 2;
  Agent player2(init);
  State board;
  for (int i = 0; i < 100; ++i) {
    auto action1 = player1.getAction(board);
    board.setField(action1.first, action1.second);
    if (board.isTerminal()){
      board.clear();
      continue;
    }
    auto action2 = player2.getAction(board);
    board.setField(action2.first, action2.second);
    if (board.isTerminal())
      board.clear();
  }
}
