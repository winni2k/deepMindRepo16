

#include "actVal.hpp"
#include "gtest/gtest.h"

// tests for the state object

using namespace std;

// make sure the action value object builds and is initialized properly
TEST(ActValClass, builds) {

  ActValFunc actVal;
  State state;
  EXPECT_EQ(0.0f, actVal.getVal(state, 0, 1));
}

// let's try to update and retrieve some action values
TEST(ActValClass, updates) {

  ActValFunc actVal;
  State state;

  for (size_t i = 0; i < 9; ++i) {
    actVal.setVal(state, i, 1, i);
    EXPECT_EQ(i, actVal.getVal(state, i, 1));
  }

  // make a move and see what new action value pairs are available
  // and settable
  state.setField(0, 1);
  for (size_t i = 0; i < 9; ++i) {
    ASSERT_THROW(actVal.getVal(state, i, 1), runtime_error);

    if (i == 0) {
      ASSERT_THROW(actVal.getVal(state, i, 2), runtime_error);

    } else {
      EXPECT_EQ(0, actVal.getVal(state, i, 2));
      actVal.setVal(state, i, 2, i * 2);
      EXPECT_EQ(i * 2, actVal.getVal(state, i, 2));
    }
  }
}
