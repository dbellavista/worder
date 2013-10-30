/**
 *
 * @file    demo1.c
 * @author  Daniele Bellavista (Daniele_Bellavista@iprel.it)
 * @date    10/29/2013 02:36:55 PM
 * @brief   
 *
 * @details 
 *
 */

#include  <worder.h>
#include	<stdlib.h>
#include	<string.h>
#include	<stdio.h>

void print_help(char* prog_name)
{
  fprintf(stderr, "Usage: %s <dictionary_file> <num_combination> {<separator>}\n", prog_name);
  fprintf(stderr, "This demo iterate a given dictionary file, printing all the "
                  "combination of words, separated by any of the given separator\n\n"
                  "The cain is:\n"
                  "\tBucket File -> Combination -> Permutation -> Separators <- Product");
}

bool doit(char* file, int num_c, char** separators, size_t sep_size)
{
  size_t i;
  Bucket file_bucket;
  Generator gen_combin, gen_permut, gen_sep, gen_product_sep;
  Bucket separator_bucket, sep_buck_dest, out_comb, out_permut, out_sep;
  Word tmp;

  // Check separators
  if(sep_size == 0 || sep_size > MAX_SEPARATOR_NUMBER) {
    fprintf(stderr, "Error no separator or too much supplied!!\n");
    return false;
  }

  // Opening file
  if(!init_bucket_file(&file_bucket, file)) {
    fprintf(stderr, "Can't open file %s\n", file);
    return false;
  }

  // Converting the separators in bucket (TODO: create an utility function for it :/ )
  init_bucket_array(&separator_bucket, sep_size);
  for(i = 0; i < sep_size; i++) {
    strncpy(tmp.word, separators[i], MAX_WORD_SIZE);
    tmp.word[MAX_WORD_SIZE] = 0;
    b_set_word_and_increment(&separator_bucket, &tmp);
  }

  // Initializing the destination bucket
  init_bucket_array(&out_comb, 1); // The size is setted by the generator
  init_bucket_array(&out_permut, 1); // The size is setted by the generator
  init_bucket_array(&out_sep, 1); // The size is setted by the generator
  init_bucket_array(&sep_buck_dest, 1); // The size is setted by the generator

  init_generator_combination(&gen_combin, num_c);
  while(generate_next(&out_comb, &gen_combin, &file_bucket)) {
    init_generator_lexical_combination(&gen_permut);
    while(generate_next(&out_permut, &gen_permut, &out_comb)) {
      init_generator_product(&gen_product_sep, num_c - 1);
      while(generate_next(&sep_buck_dest, &gen_product_sep, &separator_bucket)) {
        init_generator_separator(&gen_sep, &sep_buck_dest);
        while(generate_next(&out_sep, &gen_sep, &out_permut)) {
          print_bucket(&out_sep, "", "");
          printf("\n");
        }
      }
    }
  }

  init_generator_separator(&gen_sep, &separator_bucket);
  return true;
}

/**
 *  @brief          
 *
 *  @param[in]      argc The arguments count
 *  @param[in]      argv The program arguments
 *
 *  @return         the exit code
 *
 *  @details        
 */
int main(int argc, char *argv[])
{
  char* file;
  int num_combination;
  if(argc < 4) {
    print_help(argv[0]);
    return 1;
  }
  file = argv[1];
  num_combination = atoi(argv[2]);

  if(doit(file, num_combination, &argv[3], argc - 3)) {
    return EXIT_SUCCESS;
  } else {
    return 1;
  }
}				/* ----------  end of function main  ---------- */
