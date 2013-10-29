/**
 *
 * @file    bucket.c
 * @author  Daniele Bellavista (Daniele_Bellavista@iprel.it)
 * @date    10/29/2013 12:58:22 PM
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

off_t _read_word(char* dest, FILE* f);

off_t _read_word(char* dest, FILE* f)
{
  off_t l;
  char buffz[1000], *tmp;
  ssize_t size = MAX_WORD_SIZE;

  dest = fgets(dest, size, f);
  if(dest == NULL) {
    return -1;
  }
  tmp = dest;
  while(1) {
    l = strlen(tmp);
    if(l < size) {
      if(tmp[l - 1] == '\n') {
        tmp[l - 1] = 0;
        return ftello(f);
      }
    } else {
      size = sizeof(buffz);
      tmp = fgets(buffz, size, f);
    }
  }
}

uint64_t get_bucket_size(Bucket* b)
{
  switch(b->type) {
    case BUCKET_FILE:
      // TODO!!!
      return 0;
    case BUCKET_ARRAY:
      return b->array_size;
    default:
      return 0;
  }
}

void set_bucket_size(Bucket* b, size_t size)
{
  switch(b->type) {
    case BUCKET_FILE:
      // TODO!!!
      return;
      break;
    case BUCKET_ARRAY:
      if(size > MAX_BUCKET_ARRAY_SIZE) {
        b->array_size = MAX_BUCKET_ARRAY_SIZE;
      } else {
        b->array_size = size;
        b->last.array_position = size - 1;
      }
      break;
  }
}

bool set_word(Bucket* b, Word* in, Position* out)
{
  switch(b->type) {
    case BUCKET_FILE:
      // TODO!!!
      out->file_position = -1;
      return false;
      break;
    case BUCKET_ARRAY:
      if(b->position.array_position < 0) {
        out->file_position = -1;
        return false;
      }
      if(b->position.array_position < (ssize_t) b->array_size) {
        b->array[b->position.array_position] = *in;
        out->array_position = b->position.array_position + 1;
      } else {
        out->file_position = -1;
        return false;
      }
      break;
  }
  return true;
}

bool next_word(Word* out, Bucket* b)
{
  off_t new_fpos;
  switch(b->type) {
    case BUCKET_FILE:
      new_fpos = _read_word(out->word, b->file);
      if(new_fpos < 0) {
        return false;
      } else {
        b->position.file_position = new_fpos;
      }
      break;
    case BUCKET_ARRAY:
      if(get_word(out, b)) {
        b->position.array_position += 1;
      } else {
        return false;
      }
      break;
  }
  return true;
}

bool prev_word(Word* out, Bucket* b)
{
  switch(b->type) {
    case BUCKET_FILE:
      // TODO
      return false;
      break;
    case BUCKET_ARRAY:
      if(get_word(out, b)) {
        b->position.array_position -= 1;
      } else {
        return false;
      }
      break;
  }
  return true;
}

bool get_word(Word* out, Bucket* b)
{
  Position old_pos;
  switch(b->type) {
    case BUCKET_FILE:
      old_pos = b->position;
      if(next_word(out, b)) {
        set_bucket_position(b, &old_pos);
      } else {
        return false;
      }
      break;
    case BUCKET_ARRAY:
      if((size_t) b->position.array_position < b->array_size) {
        *out = b->array[b->position.array_position];
      } else {
        return false;
      }
      break;
  }
  return true;
}

bool set_bucket_position(Bucket* b, Position* pos)
{
  switch(b->type) {
    case BUCKET_FILE:
      if(pos->file_position >= 0) {
        fseeko(b->file, pos->file_position, SEEK_SET);
        b->position.file_position = pos->file_position;
        return true;
      }
      break;
    case BUCKET_ARRAY:
      if(pos->array_position >= 0 && (size_t) pos->array_position < b->array_size) {
        b->position.array_position = pos->array_position;
        return true;
      }
      break;
  }
  return false;
}

void print_bucket(Bucket* b)
{
  Word w;
  Position old_pos;
  old_pos = b->position;
  set_bucket_position(b, &b->first);
  while(next_word(&w, b)) {
    printf("%s\n", w.word);
  }
  set_bucket_position(b, &old_pos);
}

bool init_bucket_file(Bucket* b, char* filename)
{
  b->type = BUCKET_FILE;
  b->file = fopen(filename, "rw");
  if(b->file == NULL) {
    return false;
  }
  return true;
}

bool init_bucket_array(Bucket* b, size_t size)
{
  size_t i;
  if(size > MAX_BUCKET_ARRAY_SIZE) {
    return false;
  }
  b->type = BUCKET_ARRAY;
  b->first.array_position = 0;
  b->position.array_position = 0;
  set_bucket_size(b, size);
  for(i = 0; i < size; i++) {
    b->array[i].word[0] = 0;
  }

  return true;
}

bool relative_movement(Bucket* b, int movement)
{
  Word tmp;
  int res;
  Position oldp;
  bool (*file_op)(Word*,Bucket*);
  switch(b->type) {
    case BUCKET_ARRAY:
      res = b->position.array_position + movement;
      if(res >= 0 && (size_t) res < b->array_size) {
        b->position.array_position = res;
        return true;
      }
      break;
    case BUCKET_FILE:
      oldp = b->position;
      if(movement > 0) {
        file_op = next_word;
      } else if(movement < 0) {
        file_op = prev_word;
      } else {
        return true;
      }
      for(res = 0; res < movement; res++) {
        if(!(*file_op)(&tmp, b)) {
          set_bucket_position(b, &oldp);
          return false;
        }
      }
      return true;
  }
  return false;
}
