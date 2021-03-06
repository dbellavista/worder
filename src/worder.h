/**
 *
 * @file    worder.h
 * @author  Daniele Bellavista (Daniele_Bellavista@iprel.it)
 * @date    10/28/2013 02:47:48 PM
 * @brief   
 *
 * @details 
 *
 */

#ifndef  __WORDER_INC__
#define  __WORDER_INC__

#define _FILE_OFFSET_BITS 64
#define _LARGE_FILES 1

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_WORD_SIZE 100
#define MAX_BUCKET_ARRAY_SIZE 1024
#define MAX_SEPARATOR_NUMBER 100
#define MAX_COMBINATION_ELEMENTS 100

struct word
{
  char word[MAX_WORD_SIZE + 1];
};
typedef struct word Word;


union position
{
  off_t file_position;
  ssize_t array_position;
};
typedef union position Position;

enum bucket_type { BUCKET_FILE, BUCKET_ARRAY };

struct bucket
{
  enum bucket_type type;
  Position first;
  Position position;
  Position last;
  union
  {
    FILE* file;
    struct
    {
      Word array[MAX_BUCKET_ARRAY_SIZE];
      size_t array_size;
    };
  };
};

typedef struct bucket Bucket;

enum transform_type {TR_LEET, TR_APPENDER, TR_CONCATENATOR};

struct transformator
{
  enum transform_type type;
  union
  {
    struct
    {
      char leet_map[95];
    };
    struct
    {
      Word word_to_contact;
    };
    struct
    {
      bool append_random_numbers;
      bool append_number;
      bool append_special_char;
      union
      {
        int range;
        int number;
        char special_char;
      };
    };
  };
};
typedef struct transformator Transformator;

enum generator_type {GEN_COMBINATOR, GEN_LEXIC_PERMUTATOR, GEN_SEPARATOR, GEN_PRODUCT};

struct generator
{
  enum generator_type type;
  bool first;
  union
  {
    struct // Separator
    {
      Word separators[MAX_SEPARATOR_NUMBER];
      size_t separators_count;
    };
    struct // Combinator
    {
      Position input_status;
      Position position_status[MAX_COMBINATION_ELEMENTS];
      size_t combination_number;
    };
    struct // Lexic permutator
    {
      uint64_t lexic_elem_value[MAX_COMBINATION_ELEMENTS];
      Position lexic_positions[MAX_COMBINATION_ELEMENTS];
      size_t lexic_size;
    };
    struct // Product
    {
      Position product_positions[MAX_COMBINATION_ELEMENTS];
      size_t product_size;
    };
  };
};

typedef struct generator Generator;

bool set_bucket_size(Bucket* b, size_t size);

bool set_bucket_position(Bucket* b, Position* pos);

uint64_t get_bucket_size(Bucket* b);

bool b_relative_movement(Bucket* b, int movement);

bool b_get_word(Word* out, Bucket* b);

bool b_get_next_word(Word* out, Bucket* b, Position* next_position);

bool b_get_prev_word(Word* out, Bucket* b, Position* prev_position);

bool b_get_word_and_increment(Word* out, Bucket* b);

bool b_get_word_and_decrement(Word* out, Bucket* b);

bool b_increment_and_get_word(Word* out, Bucket* b);

bool b_decrement_and_get_word(Word* out, Bucket* b);

bool b_set_word(Bucket* b, Word* in);

bool b_set_word_and_increment(Bucket* b, Word* in);

bool b_set_word_and_decrement(Bucket* b, Word* in);

void transform(Word* out, Transformator* t, Word* el);

bool generate_next(Bucket* out, Generator* g, Bucket* b);

void print_bucket(Bucket* b, const char* prefix, const char* suffix);

bool init_bucket_array(Bucket* b, size_t size);

bool init_bucket_file(Bucket* b, char* filename);

bool destroy_bucket(Bucket* b);

bool init_transformator_appender(Transformator* t, bool t_rand, bool t_num, bool
    t_spec, int rand_range, int num, char spec_c);

bool init_transformator_leet(Transformator* t, const char partial_leet_map[][2], bool case_insensitive);

bool init_transformator_concat(Transformator* t, Word *wc);

bool init_generator_combination(Generator *gcom, size_t combination_number);

bool init_generator_lexical_combination(Generator *g);

bool init_generator_product(Generator *g, size_t size);

bool init_generator_separator(Generator *g, Bucket* separators);

/*-------------- PIPELINE STRUCTURES -------------------- */

#endif   /* ----- #ifndef __WORDER_INC__  ----- */
