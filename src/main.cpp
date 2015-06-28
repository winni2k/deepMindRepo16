#include "agent.hpp"
#include <iostream>
#include <numeric>

using namespace std;

void play(Agent &player1);
void train(Agent &player1, Agent &player2, size_t numIter);
void displayEnd(State &board, unsigned &userPNum, Agent &player);
char promptGoFirst();
int promptForField(const State &board, unsigned userPNum);
void drawBoard(const State &board, const Agent &player, unsigned userPNum);

int main(int argc, const char *argv[]) {

  cout << "Training up a pair of players..." << endl;

  AgentHelper::init init;

  Agent player1(init);
  init.pNum = 2;
  Agent player2(init);
  size_t numIter = 10000;

  train(player1, player2, numIter);

  play(player1);
}

// allows human to play against player
void play(Agent &player) {

  char first = promptGoFirst();

  unsigned userPNum = first == 'n' ? 2 : 1;
  State board;
  if (userPNum == 2) {
    player.setPlayerNum(1);
    auto action = player.getAction(board, 0);
    board.setField(action.first, action.second);
  } else
    player.setPlayerNum(2);

  while (1) {

    drawBoard(board, player, userPNum);

    // prompts for choice and
    // makes sure choice is valid
    int field = promptForField(board, userPNum);
    if (field < 0)
      break;

    board.setField(static_cast<unsigned>(field), userPNum);
    if (board.isTerminal()) {
      displayEnd(board, userPNum, player);
      continue;
    }

    auto action = player.getAction(board, 0);
    board.setField(action.first, action.second);
    if (board.isTerminal()) {
      displayEnd(board, userPNum, player);
      continue;
    }
  }
}

void drawActVals(const Agent &p1, const Agent &p2) {

  State board;
  vector<float> av1(9, 0);
  vector<float> av2(9, 0);
  for (auto i : board.getValidFields()) {
    av1[i] = p1.getActVal(board, i, 1);
    av2[i] = p2.getActVal(board, i, 1);
  }

  printf("Player1        Player2\n");
  printf("---------------------- ----------------------\n");
  printf("|%.4f|%.4f|%.4f| |%.4f|%.4f|%.4f|\n", av1[0], av1[1], av1[2], av2[0],
         av2[1], av2[2]);
  printf("---------------------- ----------------------\n");
  printf("|%.4f|%.4f|%.4f| |%.4f|%.4f|%.4f|\n", av1[3], av1[4], av1[5], av2[3],
         av2[4], av2[5]);
  printf("---------------------- ----------------------\n");
  printf("|%.4f|%.4f|%.4f| |%.4f|%.4f|%.4f|\n", av1[6], av1[7], av1[8], av2[6],
         av2[7], av2[8]);
  printf("---------------------- ----------------------\n");
}

void drawBoard(const State &board, const Agent &player, unsigned userPNum) {

  // grab the action values of player
  vector<float> av(9, 0);
  for (auto i : board.getValidFields())
    av[i] = player.getActVal(board, i, userPNum);

  cout << "\n\n";
  cout << "Field numbering    Opponent action values\n";
  cout << "-------------      -------------\n";
  printf("| 1 | 2 | 3 |      |%.2f|%.2f|%.2f|\n", av[0], av[1], av[2]);
  cout << "-------------      -------------\n";
  printf("| 4 | 5 | 6 |      |%.2f|%.2f|%.2f|\n", av[3], av[4], av[5]);
  cout << "-------------      -------------\n";
  printf("| 7 | 8 | 9 |      |%.2f|%.2f|%.2f|\n", av[6], av[7], av[8]);
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

void displayEnd(State &board, unsigned &userPNum, Agent &player) {

  if (board.getWinner() == userPNum)
    cout << "You Won!\n";
  else if (board.getWinner() == 0)
    cout << "The game is a draw.\n";
  else
    cout << "You lost :(\n";
  char first = promptGoFirst();

  userPNum = first == 'n' ? 2 : 1;
  board.clear();
  if (userPNum == 2) {
    player.setPlayerNum(1);
    auto action = player.getAction(board, 0);
    board.setField(action.first, action.second);
  } else
    player.setPlayerNum(2);
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
