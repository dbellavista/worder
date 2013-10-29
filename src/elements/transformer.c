/**
 *
 * @file    transformer.c
 * @author  Daniele Bellavista (Daniele_Bellavista@iprel.it)
 * @date    10/29/2013 01:00:54 PM
 * @brief   
 *
 * @details 
 *
 */

#include "worder.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

void transform(Word* out, Transformator* t, Word* el)
{
  size_t i;
  switch(t->type) {
    case TR_APPENDER:
      if(t->append_number) {
        snprintf(out->word, MAX_WORD_SIZE, "%s%d", el->word, t->number);
      } else if(t->append_special_char) {
        snprintf(out->word, MAX_WORD_SIZE, "%s%c", el->word, t->special_char);
      } else if(t->append_random_numbers) {
        snprintf(out->word, MAX_WORD_SIZE, "%s%d", el->word, rand() % t->range);
      }
      break;
    case TR_CONCATENATOR:
      snprintf(out->word, MAX_WORD_SIZE, "%s%s", el->word, t->word_to_contact.word);
      break;
    case TR_LEET:
      for(i = 0; i < MAX_WORD_SIZE && el->word[i] != 0; i++) {
        out->word[i] = t->leet_map[(size_t) el->word[i] - ' '];
      }
      out->word[i] = 0;
      break;
  }
}

bool init_transformator_appender(Transformator* t, bool t_rand, bool t_num,
    bool t_spec, int rand_range, int num, char spec_c)
{
  t->type = TR_APPENDER;
  if(t_rand && rand_range > 0) {
    srand(time(NULL));
    t->append_random_numbers = true;
    t->append_number = false;
    t->append_special_char = false;
    t->range = rand_range;
    return true;
  } else if(t_num) {
    t->append_random_numbers = false;
    t->append_number = true;
    t->append_special_char = false;
    t->number = num;
    return true;
  } else if(t_spec && spec_c >= ' ' && spec_c <= '~') {
    t->append_random_numbers = false;
    t->append_number = false;
    t->append_special_char = true;
    t->special_char = spec_c;
    return true;
  }
  return false;
}

bool init_transformator_leet(Transformator* t, const char partial_leet_map[][2], bool case_insensitive)
{
  size_t i;
  char dom, codom;
  t->type = TR_LEET;
  for(i = 0; i < sizeof(t->leet_map); i++) {
    t->leet_map[i] = i + ' ';
  }
  for(i = 0; partial_leet_map[i][0] != 0; i++) {
    dom = partial_leet_map[i][0];
    codom = partial_leet_map[i][1];
    t->leet_map[dom - ' '] = codom;
    if(case_insensitive) {
      if(dom >= 'A' && dom <= 'Z') {
        t->leet_map[dom - ' ' + ('a' - 'A')] = codom;
      } else if(dom >= 'a' && dom <= 'z') {
        t->leet_map[dom - ' ' - ('a' - 'A')] = codom;
      }
    }
  }
  return true;
}

bool init_transformator_concat(Transformator* t, Word *wc)
{
  t->type = TR_CONCATENATOR;
  t->word_to_contact = *wc;
  return true;
}

