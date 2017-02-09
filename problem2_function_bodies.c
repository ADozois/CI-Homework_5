/*
##########################################################################################
Student Functions:

- Implement the bodies of the problem prototypes within this file!
- The import rl_homework_lib.h is sufficient to solve all the problems.

##########################################################################################
*/

#include "rl_homework_lib.h"


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
  float max, tmp;
  Action next_a;
  State next_s;

  next_s = getNextState(s_t, UP);
  max = getNextReward(next_s);
  next_a = UP;

  for (int action = 1; action < 4; ++action) {
    next_s = getNextState(s_t, (Action)action);
    tmp = getNextReward(next_s);
    if(tmp > max){
      max = tmp;
      next_a = (Action)action;
    }
  }

  return next_a;
}


