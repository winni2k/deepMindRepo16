

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

  // make sure that fields can only be set once
  for (size_t i = 0; i < 3; i++) {
    ASSERT_THROW(state.setField(0, i), runtime_error);
  }

  // set another field
  state.setField(2, 2);
  
}
