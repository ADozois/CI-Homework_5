/*
##########################################################################################
Student Functions:

- Implement the bodies of the problem prototypes within this file!
- The import rl_homework_lib.h is sufficient to solve all the problems.

##########################################################################################
*/

#include "rl_homework_lib.h"



// Function of the World

/*
 *First, we will implement the basic dynamics of the grid-world. The function getNextState returns the next state
 *st+1 given the state st and action at. For example for the state-action combination ([1,1], RIGHT) the functions
 *returns [2,1]. If the agents walks against a wall (grey square in figure 1), the agent remains in the same position.
 *For example ([1,1], UP) or ([1,1], LEFT) yield [1,1].*/
State getNextState(State s_t, Action a_t) {
  // X axe
  if (a_t == RIGHT && s_t.x + 1 < world_width-1){
    s_t.x++;
  } else if(a_t == LEFT && s_t.x - 1 > 0){
    s_t.x--;
  }

  // Y axe
  if (a_t == DOWN && s_t.y + 1 < world_height-1){
    s_t.y++;
  } else if(a_t == UP && s_t.y - 1 > 0){
    s_t.y--;
  }
  return s_t;
}

/*As this grid-world is a finite Markov Decision Process, we need a function to check if a terminal state is reached.
 *The isTerminalState function checks if a terminal state is reached and returns 1 for a terminal state and 0 otherwise.
 * Terminal states are the bottom of the cliff (i.e. [2,4] - [6,4]) and the goal square (i.e. [7,4]).*/
int isTerminalState(State s_t) {
  if(s_t.y == world_height - 2 && s_t.x >= 2)
    return 1;
  else
    return 0;
}


/*For the reward we implement a third function, which returns the reward for a state-action-state sequence. As this
 * problem is deterministic and the path to a certain does not matter, we simplify the reward function such that the
 * reward is only conditioned on the next state st+1. The function getNextReward takes a state and returns the
 * associated reward. A mapping from state to reward is shown in the figure below. Within your implementation you
 * should use the global variables dropOffPenalty, stepPenalty and goalReward, as the test cases will change the
 * respective values. (Have a look at the Programming Help Section at the end for additional tips)*/
float getNextReward(State s_t) {
  if(s_t.y == 4 && s_t.x >= 2 && s_t.x <= 6)
    return dropOffPenalty;
  else if (s_t.y == 4 && s_t.x == 7)
    return goalReward;
  else
    return stepPenalty;
}

