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


