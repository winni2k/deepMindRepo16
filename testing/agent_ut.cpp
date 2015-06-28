

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
    auto action = player.getAction(board, 0);
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
  float reward = 0;
  for (int i = 0; i < 100; ++i) {

    auto action1 = player1.getAction(board, reward);
    board.setField(action1.first, action1.second);
    if (board.isTerminal()) {
      board.clear();
      continue;
    }
    auto action2 = player2.getAction(board, reward);
    board.setField(action2.first, action2.second);
    if (board.isTerminal())
      board.clear();
  }
}

TEST(AgentClass, learns) {

  std::default_random_engine generator;
  std::uniform_int_distribution<unsigned> bernInt(1, 2);

  AgentHelper::init init;
  Agent p1(init);
  Agent p2(init);

  float r1 = 0, r2 = 0;
  for (int i = 0; i < 10000; ++i) {
    unsigned startP = bernInt(generator) == 1 ? 1 : 2;
    AgentHelper::playEpisode(startP, p1, p2, r1, r2);
  }

  State board;
  vector<float> avs1(9, 0);
  vector<float> avs2(9, 0);
  for (auto i : board.getValidFields()) {
    avs1[i] = p1.getActVal(board, i, 1);
    avs2[i] = p1.getActVal(board, i, 1);
  }

  // make sure initial values are not all zero
  EXPECT_LT(numeric_limits<float>::min(),
            accumulate(avs1.begin(), avs1.end(), 0.0f,
                       [](float sum, float elem) { return sum + abs(elem); }));

  // make sure initial values are not all zero
  EXPECT_LT(numeric_limits<float>::min(),
            accumulate(avs2.begin(), avs2.end(), 0.0f,
                       [](float sum, float elem) { return sum + abs(elem); }));

  // test to see if player avoids stupid first steps
  p1.setEpsilon(0);
  EXPECT_EQ(0, p1.getEpsilon());
  p2.setEpsilon(0);

  // test first moves
  p1.setPlayerNum(1);
  p2.setPlayerNum(1);
  unsigned badMoveSum = 0;
  unsigned bms2 = 0;
  for (int i = 0; i < 100; ++i) {
    board.clear();
    auto act1 = p1.getAction(board, 0, false);
    if (act1.first % 2 == 1)
      ++badMoveSum;
    auto act2 = p2.getAction(board, 0, false);
    if (act2.first % 2 == 1)
      ++bms2;
  }
  EXPECT_EQ(0, badMoveSum);
  EXPECT_EQ(0, bms2);

  // test second moves
  badMoveSum = bms2 = 0;
  p1.setPlayerNum(2);
  p2.setPlayerNum(2);
  std::uniform_int_distribution<unsigned> binInt(0, 4);
  for (int i = 0; i < 100; ++i) {
    board.clear();
    unsigned choice = binInt(generator) * 2;
    board.setField(choice, 1);
    auto act = p1.getAction(board, 0, false);
    auto act2 = p2.getAction(board, 0, false);

    // p1 should choose the center after any smart first move
    // that is not the center (4)
    // otherwise p1 should choose a corner
    if ((choice != 4 && act.first != 4) || act.first % 2 == 1)
      ++badMoveSum;
    // same for p2
    if ((choice != 4 && act2.first != 4) || act2.first % 2 == 1)
      ++bms2;
  }
  EXPECT_EQ(0, badMoveSum);
  EXPECT_EQ(0, bms2);
}
