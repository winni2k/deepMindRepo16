

#include "state.hpp"
#include "gtest/gtest.h"

// tests for the state object

using namespace std;

// make sure the state object builds and is initialized properly
TEST(StateClass, builds) {

  State state1;
  for (size_t i = 0; i < 9; i++) {
    EXPECT_EQ(0, state1.getField(i));
  }
}

TEST(StateClass, isChangeable) {

  State state;
  state.setField(0, 1);
  EXPECT_EQ(1, state.getField(0));

  // make sure that fields can only be set once
  for (size_t i = 0; i < 3; i++) {
    ASSERT_THROW(state.setField(0, i), runtime_error);
  }

  // set another field
  state.setField(8, 2);
  EXPECT_EQ(2, state.getField(8));

  // finally, check other fields are still 0
  for (size_t i = 1; i < 8; i++)
    EXPECT_EQ(0, state.getField(i));
}
