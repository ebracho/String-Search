/*
 * Copyright (c) <2016> <Eddie Bracho 
 * This software is licensed under the MIT License
 */

#include <stdio.h>
#include <string.h>


/* 
 * https://upload.wikimedia.org/wikipedia/commons/d/d9/DFA_search_mommy.svg
 *  
 * This algorithm works by computing a representation of a
 * Finite Automaton for matching string (like the link above)
 * in O(n) time and space (where n is the length of the search 
 * string).
 *
 * The insight for this procedure is the observation that string-
 * matching FAs work by finding "prefix runs" in the string. In the
 * example above, the 2nd and 3rd M are prefix runs of length 1. 
 * A state that is a prefix run of length 1 inherits the transition
 * of state 1. So in the example above, states 3 and 4 inherit state
 * 1's transtion to state 2 on 'O'. 
 *
 * This works for prefix runs of any length. A prefix run of length
 * 5 inherits the transtion of state 5. 
 *
 * In this implemenation, prefix runs are saved in an array called
 * repeat_states.
 */
unsigned int string_search(const char string[], const char text[]) {
  unsigned int repeat_states[strlen(string)], state, i;

  /* Initialize repeat_states */
  repeat_states[0] = 0;
  for (i = 1; string[i]; i++) {
    if (string[i] == string[repeat_states[i-1]]) {
      repeat_states[i] = repeat_states[i-1] + 1;
    }
    else if (string[i] == string[0]) {
      repeat_states[i] = 1;
    }
    else {
      repeat_states[i] = 0;
    }
  }

  /* Search text */
  state = -1; /* Start state */
  for (i = 0; text[i]; i++) {
    if (text[i] == string[state + 1]) {
      /* Update state and check for accept state */      
      if (string[++state + 1] == '\0') {
        return i - state;
      }
    }
    else if (state != -1 && text[i] == string[repeat_states[state]]) {
      state = repeat_states[state];
    }
    else {
      state = -1;
    }
  }
  return -1;
}

/*
 * After thinking about the problem some more, I realized that the procedure
 * is possible to do using constant space. This new version works by keeping
 * track of prefix runs on the fly while passing through text.
 *
 * This version has a much higher constant constant than the version above.
 */
unsigned int string_search_(const char string[], const char text[]) {
  unsigned int i, match_index, prefix_run;

  match_index = prefix_run = 0;
  for (i = 0; text[i]; i++) {

    /* Update prefix run */
    if (match_index > 1) {
      if (text[i] == string[prefix_run]) {
        prefix_run++;
      }
      else if (text[i] == string[0]) {
        prefix_run = 1;
      }
      else {
        prefix_run = 0;
      }
    }

    /* Update match index */
    if (text[i] == string[match_index]) {
      match_index++;
    }
    else if (text[i] == string[0]) {
      match_index = 1;
    }
    else {
      match_index = prefix_run;
      prefix_run = 0;
    }

    /* Check for string match */
    if (!string[match_index]) {
      return i - match_index + 1;
    }
  }
  return -1;
}

int main() {
  printf("%d\n", string_search_("aaaaabaaaabaaabaabab",
                                "aaaaaaaabaaaabaaabaabab"));
  return 0;
}

