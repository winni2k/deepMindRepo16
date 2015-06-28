#include "agent.hpp"
#include <iostream>
#include <numeric>

using namespace std;

void play(Agent &p1, Agent &p2);
void train(Agent &player1, Agent &player2, size_t numIter);
void displayEnd(State &board, unsigned &userPNum);
char promptGoFirst();
int promptForField(const State &board, unsigned userPNum);
void drawBoard(const State &board, const Agent &p1, const Agent &p2,
               unsigned userPNum);

int main(int argc, const char *argv[]) {

  cout << "Training up a pair of players..." << endl;

  AgentHelper::init init;

  Agent player1(init);
  init.pNum = 2;
  Agent player2(init);
  size_t numIter = 100000;

  train(player1, player2, numIter);

  play(player1, player2);
}

// allows human to play against player
void play(Agent &p1, Agent &p2) {

  // turn off exploration for playing against human
  p1.setEpsilon(0);
  p2.setEpsilon(0);

  int field = 10;
  // enter play loop
  State board;
  while (field >= 0) {
    char first = promptGoFirst();

    board.clear();
    unsigned userPNum = first == 'n' ? 2 : 1;
    if (userPNum == 2) {
      auto action = p1.getAction(board, 0, false);
      board.setField(action.first, action.second);
    }
    // episode loop
    while (1) {

      drawBoard(board, p1, p2, userPNum);

      // prompts for choice and
      // makes sure choice is valid
      field = promptForField(board, userPNum);
      if (field < 0)
        break;

      board.setField(static_cast<unsigned>(field), userPNum);
      if (board.isTerminal()) {
        displayEnd(board, userPNum);
        break;
      }

      auto action = userPNum == 2 ? p1.getAction(board, 0, false)
                                  : p2.getAction(board, 0, false);
      board.setField(action.first, action.second);
      if (board.isTerminal()) {
        displayEnd(board, userPNum);
        break;
      }
    }
  }
}

void drawActVals(const Agent &p1, const Agent &p2) {

  State board;
  State board2;
  vector<float> av1(9, 0);
  vector<float> av2(9, 0);
  board2.setField(4, 1);
  for (auto i : board.getValidFields())
    av1[i] = p1.getActVal(board, i, 1);
  for (auto i : board2.getValidFields())
    av2[i] = p2.getActVal(board2, i, 2);

  printf("         Player1           Player2\n");
  printf("Alpha:   %.5e           %.5e\n", p1.getAlpha(), p2.getAlpha());
  printf("Gamma:   %.5e           %.5e\n", p1.getGamma(), p2.getGamma());
  printf("Epsilon: %.5e           %.5e\n", p1.getEpsilon(), p2.getEpsilon());
  printf("---------------------- ----------------------\n");
  printf("|%.4g|%.4g|%.4g|  \t|%.4g|%.4g|%.4g|\n", av1[0], av1[1], av1[2],
         av2[0], av2[1], av2[2]);
  printf("---------------------- ----------------------\n");
  printf("|%.4g|%.4g|%.4g|  \t|%.4g|%.4g|%.4g|\n", av1[3], av1[4], av1[5],
         av2[3], av2[4], av2[5]);
  printf("---------------------- ----------------------\n");
  printf("|%.4g|%.4g|%.4g|  \t|%.4g|%.4g|%.4g|\n", av1[6], av1[7], av1[8],
         av2[6], av2[7], av2[8]);
  printf("---------------------- ----------------------\n\n");
}

void drawBoard(const State &board, const Agent &p1, const Agent &p2,
               unsigned userPNum) {

  // grab the action values of p1 or p2 to predict which move to make
  vector<float> av(9, 0);
  for (auto i : board.getValidFields())
    av[i] = userPNum == 1 ? p1.getActVal(board, i, userPNum)
                          : p2.getActVal(board, i, userPNum);

  cout << "\n\n";
  cout << "Field numbering    Opponent action values\n";
  cout << "-------------      -------------\n";
  printf("| 1 | 2 | 3 |      |%.2g|%.2g|%.2g|\n", av[0], av[1], av[2]);
  cout << "-------------      -------------\n";
  printf("| 4 | 5 | 6 |      |%.2g|%.2g|%.2g|\n", av[3], av[4], av[5]);
  cout << "-------------      -------------\n";
  printf("| 7 | 8 | 9 |      |%.2g|%.2g|%.2g|\n", av[6], av[7], av[8]);
  cout << "-------------      -------------\n\n";

  // convert pieces that are 0 to spaces for display
  string pieces = board.to_string();
  for (size_t i = 0; i < pieces.size(); ++i)
    if (pieces[i] == '0')
      pieces[i] = ' ';

  cout << "Current Board:\n";
  cout << "-------------\n";
  printf("| %c | %c | %c |\n", pieces[0], pieces[1], pieces[2]);
  cout << "|---|---|---|\n";
  printf("| %c | %c | %c |\n", pieces[3], pieces[4], pieces[5]);
  cout << "|---|---|---|\n";
  printf("| %c | %c | %c |\n", pieces[6], pieces[7], pieces[8]);
  cout << "|---|---|---|\n\n";
}

char promptGoFirst() {
  cout << "Would you like to go first?[Y/n]" << endl;
  char first = 'y';
  cin >> first;
  return first == 'n' ? 'n' : 'y';
}

int promptForField(const State &board, unsigned userPNum) {

  while (true) {
    char field = 'c';
    cout << "Please choose move (1-9,q=quit) " << std::flush;
    cin >> field;

    // check if the choice is valid and leads to a terminal state
    if (field == 'q')
      return -1;
    if (field >= '1' && field <= '9' &&
        board.validAction(field - '1', userPNum))
      return static_cast<int>(field - '1');
    cout << "\nInvalid action: " << field << endl;
  }
}

void displayEnd(State &board, unsigned &userPNum) {

  if (board.getWinner() == userPNum)
    cout << "You Won!\n";
  else if (board.getWinner() == 0)
    cout << "The game is a draw.\n";
  else
    cout << "You lost :(\n";
}
// trains the players against each other
void train(Agent &player1, Agent &player2, size_t numIter) {

  std::default_random_engine generator;
  std::uniform_int_distribution<unsigned> bernInt(1, 2);

  // training loop
  float reward1 = 0, reward2 = 0;
  vector<unsigned> winnerCount(3, 0);
  for (size_t iter = 0; iter < numIter; ++iter) {

    // give visual update on training progress every 1000 iterations
    if (iter > 0 && iter % 1000 == 0) {
      unsigned sum = accumulate(winnerCount.begin(), winnerCount.end(), 0);
      printf("Draws: %u\tPlayer1 wins: %u\tPlayer2 wins: \%u\n", winnerCount[0],
             winnerCount[1], winnerCount[2]);
      winnerCount[0] = winnerCount[1] = winnerCount[2] = 0;
      drawActVals(player1, player2);
    }

    // play an episode
    ++winnerCount[AgentHelper::playEpisode(player1, player2, reward1, reward2)];
  }
}
