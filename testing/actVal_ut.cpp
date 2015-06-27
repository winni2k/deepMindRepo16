

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
TEST(ActValClass, updates){

    ActValFunc actVal;
    State state;
}
