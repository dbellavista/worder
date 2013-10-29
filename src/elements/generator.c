/**
 *
 * @file    worder.c
 * @author  Daniele Bellavista (Daniele_Bellavista@iprel.it)
 * @date    10/28/2013 03:42:00 PM
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

bool next_combination(Bucket* out, Bucket* in, Generator* g);
bool next_lexic_permutation(Bucket* out, Bucket* in, Generator* g);
bool next_separator(Bucket* out, Bucket* in, Generator* g);
uint64_t string_value(char *string);
int compar(const void*,const void*);
bool next_product(Bucket* out, Bucket* in, Generator* g);

int compar(const void* c1, const void* c2)
{
  uint64_t v1, v2;
  v1 = *((uint64_t*)c1);
  v2 = *((uint64_t*)c2);
  if(v1 < v2) {
    return -1;
  } else if (v1 == v2) {
    return 0;
  } else { // v1 > v2
    return 1;
  }
}

bool generate_next(Bucket* out, Generator* g, Bucket* b)
{

  switch(g->type) {
    case GEN_COMBINATOR:
      return next_combination(out, b, g);
      break;
    case GEN_LEXIC_PERMUTATOR:
      return next_lexic_permutation(out, b, g);
      break;
    case GEN_SEPARATOR:
      return next_separator(out, b, g);
      break;
    case GEN_PRODUCT:
      return next_product(out, b, g);
  }

  return false;
}

uint64_t string_value(char *string)
{
  size_t i;
  uint64_t val = 0;
  for(i = 0; i < strlen(string); i++) {
    val += (uint64_t) pow(128, i) * string[i];
  }
  return val;
}

bool next_separator(Bucket* out, Bucket* in, Generator* g)
{
  size_t newsize, bucket_size, i;
  Word inword;
  Position next_pos;
  if(!g->first) {
    return false;
  }
  bucket_size = get_bucket_size(in);
  newsize = bucket_size + ((g->separators_count <= bucket_size - 1) ?
    g->separators_count : bucket_size - 1);
  set_bucket_size(out, newsize);

  set_bucket_position(out, &out->first);
  for(i = 0; next_word(&inword, in); i++) {
    set_word(out, &inword, &next_pos);
    set_bucket_position(out, &next_pos);
    if(i < g->separators_count) {
      set_word(out, &g->separators[i], &next_pos);
      set_bucket_position(out, &next_pos);
    }
  }

  g->first = false;
  return true;
}

bool next_lexic_permutation(Bucket* out, Bucket* in, Generator* g)
{
  unsigned int i, j; // Upper Index i; Lower Index j
  Word word;
  Position next_pos;

  if(g->first) {
    uint64_t tmp_vals[MAX_COMBINATION_ELEMENTS];
    Position tmp_positions[MAX_COMBINATION_ELEMENTS + 1];
    bool marked_indexes[MAX_COMBINATION_ELEMENTS];

    set_bucket_position(in, &(in->first));
    g->lexic_size = 0;
    g->lexic_positions[0] = in->position;

    while(next_word(&word, in)) {
      tmp_vals[g->lexic_size] = string_value(word.word);
      marked_indexes[g->lexic_size] = false;

      g->lexic_size += 1;
      tmp_positions[g->lexic_size] = in->position;
    }
    set_bucket_size(out, g->lexic_size);

    memcpy(g->lexic_positions, tmp_positions, MAX_COMBINATION_ELEMENTS * sizeof(uint64_t));
    memcpy(g->lexic_elem_value, tmp_vals, MAX_COMBINATION_ELEMENTS * sizeof(uint64_t));
    qsort(g->lexic_elem_value, g->lexic_size, sizeof(uint64_t), compar);

    for(i = 0; i < g->lexic_size; i++) {
      for(j = 0; j < g->lexic_size; j++) {
        if(tmp_vals[i] == g->lexic_elem_value[j] && !marked_indexes[j]) {
          marked_indexes[j] = true;
          g->lexic_positions[j] = tmp_positions[i];
          break;
        }
      }
    }
    g->first = false;
  } else {

    uint64_t tmp_a;
    Position tmp_i;

    i = g->lexic_size - 1;
    while (i > 0 && g->lexic_elem_value[i - 1] >= g->lexic_elem_value[i])
      i--;
    if (i == 0)
      return false;

    // Find successor to pivot
    j = g->lexic_size - 1;
    while (g->lexic_elem_value[j] <= g->lexic_elem_value[i - 1])
      j--;
    tmp_a = g->lexic_elem_value[i - 1];
    g->lexic_elem_value[i - 1] = g->lexic_elem_value[j];
    g->lexic_elem_value[j] = tmp_a;

    tmp_i = g->lexic_positions[i - 1];
    g->lexic_positions[i - 1] = g->lexic_positions[j];
    g->lexic_positions[j] = tmp_i;

    // Reverse suffix
    j = g->lexic_size - 1;
    while (i < j) {
      tmp_a = g->lexic_elem_value[i];
      g->lexic_elem_value[i] = g->lexic_elem_value[j];
      g->lexic_elem_value[j] = tmp_a;

      tmp_i = g->lexic_positions[i];
      g->lexic_positions[i] = g->lexic_positions[j];
      g->lexic_positions[j] = tmp_i;
      i++;
      j--;
    }
  }

  set_bucket_position(out, &out->first);
  for(i = 0; i < g->lexic_size; i++) {
    set_bucket_position(in, &g->lexic_positions[i]);
    get_word(&word, in);
    set_word(out, &word, &next_pos);
    set_bucket_position(out, &next_pos);
  }

  return true;
}

bool next_combination(Bucket* out, Bucket* in, Generator* g)
{
  ssize_t i, pivot_idx = 0;
  Position tmp_pos, pivot_pos, next_pos;
  Word pivot_word;
  Word word, tmp;

  if(g->first) {
    set_bucket_size(out, g->combination_number);
    tmp_pos = in->position;
    if(!next_word(&word, in)) {
      return false;
    }
    next_pos = in->position;
    set_bucket_position(in, &tmp_pos);

    for(i = 0; (size_t) i < g->combination_number; i++) {
      g->position_status[i] = next_pos;
      set_word(out, &word, &tmp_pos);
      set_bucket_position(out, &tmp_pos);
    }
    next_word(&word, in);
    g->first = false;
    return true;
  }

  for(set_bucket_position(out, &out->last), i = g->combination_number - 1;
      i >= 0;
      prev_word(&tmp, out), i--) {

    if(next_word(&pivot_word, in)) {
      pivot_pos = in->position;
      set_word(out, &pivot_word, &next_pos);
      set_bucket_position(out, &next_pos);
      g->position_status[i] = in->position;
      pivot_idx = i;
      break;
    }
    if(i == 0) {
      return false;
    }
    set_bucket_position(in, &g->position_status[i - 1]);
  }

  for(i = pivot_idx + 1; (size_t) i < g->combination_number; i++) {
    g->position_status[i] = pivot_pos;

    set_word(out, &pivot_word, &next_pos);
    set_bucket_position(out, &next_pos);
  }
  return true;
}

bool init_generator_combination(Generator *gcom, size_t combination_number)
{
  if(combination_number > MAX_COMBINATION_ELEMENTS) {
    return false;
  }
  gcom->combination_number = combination_number;
  gcom->first = true;
  gcom->type = GEN_COMBINATOR;
  return true;
}

bool init_generator_lexical_combination(Generator *g)
{
  g->type = GEN_LEXIC_PERMUTATOR;
  g->first = true;
  return true;
}

bool init_generator_product(Generator *g, size_t size)
{
  if(size == 0) {
    return false;
  }
  g->type = GEN_PRODUCT;
  g->first = true;
  g->product_size = size;
  return true;
}

bool init_generator_separator(Generator *g, Bucket* separators)
{
  size_t i;
  Word word;
  set_bucket_position(separators, &separators->first);
  for(i = 0; next_word(&word, separators); i++) {
    if(i > MAX_SEPARATOR_NUMBER) {
      return false;
    }
    g->separators[i] = word;
  }
  g->separators_count = i;
  g->type = GEN_SEPARATOR;
  g->first = true;
  return true;
}

bool next_product(Bucket* out, Bucket* in, Generator* g)
{
  ssize_t i;
  Word word, tmp;
  Position next_pos;

  if(g->first) {
    set_bucket_position(in, &(in->first));
    get_word(&word, in);
    set_bucket_size(out, g->product_size);
    for(i = 0; (size_t) i < g->product_size; i++) {
      g->product_positions[i] = in->first;
      set_word(out, &word, &next_pos);
      set_bucket_position(out, &next_pos);
    }
    g->first = false;
  } else {
    for(set_bucket_position(out, &out->last), i = g->product_size - 1;
        i >= 0;
        prev_word(&tmp, out), i--) {

      set_bucket_position(in, &g->product_positions[i]);
      if(next_word(&word, in)) {
        set_word(out, &word, &next_pos);
        set_bucket_position(out, &next_pos);
        g->product_positions[i] = in->position;
        break;
      }
      if(i == 0) {
        return false;
      }
      g->product_positions[i] = in->first;
    }
  }
  return true;
}
