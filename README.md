# Google DeepMind RE Exercise

Here is the coding exercise:
Your task is to create an agent that learns to play a game of
tic-tac-toe. It's up to you to decide the training method:  

- implementing a teacher that knows the optimal strategy
- bootstrapping two random agents against themselves
- train the agent yourself and store its state for us to load
  or any other approach. 

However the trained agent should learn the value or action-value
function or policy for each state purely from game play, rather than
from knowing the rules and heuristics of the game. Code should be
written in C++ or Python.  If you prefer to use a different language,
please check with us first. 

Use Q-learning and Sarsa to solve the problem. Plot a reward per
episode graph for both algorithms.  

- SARSA http://webdocs.cs.ualberta.ca/~sutton/book/ebook/node64.html
- Q-learning http://webdocs.cs.ualberta.ca/~sutton/book/ebook/node65.html

We will assign high weight to good code documentation and
style. Please include unit tests in your project. Only spare time
should be spent on presentation of game play, text output to terminal
is sufficient. Also we expect the agent to be pretty good at the
game. 

# Installation and usage

To get up and running, clone this repository:

    git clone git@github.com:DMRec/repo16.git

Then enter the new directory and run make. For best performance, use
`-O3` compilation flag:

    export CPPFLAGS='-O3'
    cd repo16
    make

This will compile the code and run automatic tests and may take a
minute.

## Playing against the computer

To play a round of tic tac toe against a player, run the main
executable in interactive mode:

    src/ttt --interactive

The executable has a few more interesting options, including verbose
mode that can be displayed with the `--help` flag.

## Visualization of reward per episode

To see a plot of reward per episode for both the SARSA and QLearn
algorithms, run the python script.  This runs the main executable with
logging and plots the results:

    python vis.py


