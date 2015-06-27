

#include "state.hpp"
#include "gtest/gtest.h"

// tests for the state object

using namespace std;

// make sure the state object builds and is initialized properly
TEST(StateClass, builds) {

  State state;
  for (size_t i = 0; i < 9; i++) {
    EXPECT_EQ(0, state.getField(i));
  }
  EXPECT_EQ("000000000", state.to_string());
}

TEST(StateClass, isChangeable) {

  State state;
  state.setField(0, 1);
  EXPECT_EQ(1, state.getField(0));

  // make sure that fields can only be set once
  for (size_t i = 0; i < 3; i++) {
    ASSERT_THROW(state.setField(0, i), runtime_error);
  }

  // make sure field can only be set to 1 or 2
  ASSERT_THROW(state.setField(2, 0), runtime_error);
  ASSERT_THROW(state.setField(2, 3), runtime_error);

  // players need to alternate
  ASSERT_THROW(state.setField(2, 1), runtime_error);

  // set another field
  state.setField(8, 2);
  EXPECT_EQ(2, state.getField(8));

  // finally, check other fields are still 0
  for (size_t i = 1; i < 8; i++)
    EXPECT_EQ(0, state.getField(i));

  EXPECT_EQ("100000002", state.to_string());
}

TEST(StateClass, terminates) {

  State state;
  // play a fiew moves
  EXPECT_EQ(-1, state.getWinner());
  EXPECT_EQ(false, state.isTerminal());
  state.setField(0, 1);
  EXPECT_EQ(false, state.isTerminal());
  state.setField(3, 2);
  EXPECT_EQ(false, state.isTerminal());
  state.setField(1, 1);
  EXPECT_EQ(false, state.isTerminal());
  state.setField(4, 2);
  EXPECT_EQ(false, state.isTerminal());
  EXPECT_EQ(-1, state.getWinner());
  state.setField(2, 1);
  EXPECT_EQ(true, state.isTerminal());
  EXPECT_EQ(1, state.getWinner());

  ASSERT_THROW(state.setField(8, 2), runtime_error);
}
