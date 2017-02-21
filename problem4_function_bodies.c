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



// Function of the Agent:

/*The function getNextAction takes the agent struct and the current state st and returns the next action at.*/
Action getNextAction(Agent *a, State s_t) {
  double value = (double)rand() / (double)RAND_MAX;
  if (value < a->epsilon)
    return getRandomAction();
  else
    return getGreedyAction(a,s_t);
}

Action getRandomAction() {
  double value = (double)rand() / (double)RAND_MAX;
  if (value <= 0.25)
    return UP;
  else if (value > 0.25 && value <= 0.5)
    return DOWN;
  else if (value > 0.5 && value <= 0.75)
    return RIGHT;
  else
    return LEFT;
}

Action getGreedyAction(Agent *a, State s_t) {
  float max = 0.0, tmp;
  Action next_a;
  State next_s;

  if (s_t.y != next_s.y || s_t.x != next_s.x) {
    max = a->QValues[stateAction2idx(s_t, UP)];
    next_a = UP;
  }

  if (s_t.y != next_s.y || s_t.x != next_s.x) {
    tmp = a->QValues[stateAction2idx(s_t,RIGHT)];
    if (tmp > max) {
      max = tmp;
      next_a = RIGHT;
    }
  }

  if (s_t.y != next_s.y || s_t.x != next_s.x) {
    tmp = a->QValues[stateAction2idx(s_t,LEFT)];
    if (tmp > max) {
      max = tmp;
      next_a = LEFT;
    }
  }

  if (s_t.y != next_s.y || s_t.x != next_s.x) {
    tmp = a->QValues[stateAction2idx(s_t,DOWN)];
    if (tmp > max) {
      next_a = DOWN;
    }
  }

  return next_a;
}


// Function of Gameplay:
Agent *trainAgent(int nIter, int verbose, Agent *a) {
  float sum = 0.0;
  for (int i = 0; i < nIter; ++i) {
    a = playEpisode(i, verbose, a);
    sum += a->accumReward;
  }
  a->avgReward = sum / nIter;

  return a;
}

// Play episode
Agent *playEpisode(int nEpisode, int verbose, Agent *a) {
  State s_t;
  State next_st;
  Action next_at;
  float reward;

  s_t.x = 1;
  s_t.y = world_height - 2;

  a->accumReward = 0;

  while (isTerminalState(s_t) == 0) {
    next_at = getNextAction(a, s_t);
    next_st = getNextState(s_t, next_at);
    reward = getNextReward(next_st);
    a->accumReward += reward;
    if (a->type == SARSA) {
      a = sarsaUpdate(a, s_t, next_st, next_at, reward);
    } else {
      a = qLearningUpdate(a, s_t, next_st, next_at, reward);
    }
    s_t.x = next_st.x;
    s_t.y = next_st.y;
  }
  return a;
}

// SARSA and Q-Learning Updates:
Agent *sarsaUpdate(Agent *a, State s_t1, State s_t2, Action a_t1, float r_t) {
  Action next_a = getNextAction(a, s_t2);
  a->QValues[stateAction2idx(s_t1, a_t1)] = a->QValues[stateAction2idx(s_t1, a_t1)] + a->alpha
      * (r_t + a->gamma * a->QValues[stateAction2idx(s_t2, next_a)] - a->QValues[stateAction2idx(s_t1, a_t1)]);

  return a;
}
Agent *qLearningUpdate(Agent *a, State s_t1, State s_t2, Action a_t1, float r_t) {
  Action max_a = getGreedyAction(a, s_t2);
  a->QValues[stateAction2idx(s_t1, a_t1)] = a->QValues[stateAction2idx(s_t1, a_t1)] + a->alpha
      * (r_t + a->gamma * a->QValues[stateAction2idx(s_t2, max_a)] - a->QValues[stateAction2idx(s_t1, a_t1)]);
  return a;
}


