

#include "state.hpp"
#include "gtest/gtest.h"

// tests for the state object

// make sure the state object builds and is initialized properly
TEST(StateClass, builds) {

  State state1;
  for (size_t i = 0; i < 9; i++) {
    EXPECT_EQ(0, state1.getField(i));
  }
}
