#include "agent.hpp"
#include <iostream>
#include <numeric>

using namespace std;

int main(int argc, const char *argv[]) {

  cout << "Training up a pair of players..." << endl;

  std::default_random_engine generator;
  std::uniform_int_distribution<unsigned> bernInt(1, 2);

  AgentHelper::init init;
  Agent player1(init);
  init.pNum = 2;
  Agent player2(init);
  size_t numIter = 10000;

  // training loop
  float reward1 = 0, reward2 = 0;
  vector<unsigned> winnerCount(3, 0);
  for (size_t iter = 0; iter < numIter; ++iter) {
    if (iter > 0 && iter % 1000 == 0) {
      unsigned sum = accumulate(winnerCount.begin(), winnerCount.end(), 0);
      printf("Draws: %f\%\tPlayer1 wins: %f\%\tPlayer2 wins: \%f\n",
             winnerCount[0] / static_cast<float>(sum),
             winnerCount[1] / static_cast<float>(sum),
             winnerCount[2] / static_cast<float>(sum));
    }

    State board;
    // run through a whole episode
    // randomly select first player
    unsigned pNumToGo = bernInt(generator) == 1 ? 1 : 2;
    player1.setPlayerNum(pNumToGo);
    player2.setPlayerNum(pNumToGo == 1 ? 2 : 1);
    while (!board.isTerminal()) {
      pair<unsigned, unsigned> action = make_pair(9, 9);
      if (pNumToGo == 1) {
        action = player1.getAction(board, reward1);
        reward1 = 0;
      } else {
        action = player2.getAction(board, reward2);
        reward2 = 0;
      }
      board.setField(action.first, action.second);
      pNumToGo = pNumToGo == 1 ? 2 : 1;
    }
    int winner = board.getWinner();
    ++winnerCount[winner];
    if (winner == 1) {
      reward1 = 1;
      reward2 = -1;
    } else if (winner == 2) {
      reward1 = -1;
      reward2 = 1;
    } else
      reward1 = reward2 = 0;
  }
}
