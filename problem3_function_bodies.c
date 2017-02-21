/*
##########################################################################################
Student Functions:

- Implement the bodies of the problem prototypes within this file!
- The import rl_homework_lib.h is sufficient to solve all the problems.

##########################################################################################
*/

#include "rl_homework_lib.h"

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


