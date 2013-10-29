/**
 *
 * @file    test_main.c
 * @author  Daniele Bellavista (Daniele_Bellavista@iprel.it)
 * @date    10/29/2013 09:45:20 AM
 * @brief   
 *
 * @details 
 *
 */

#include  <worder.h>
#include	<stdlib.h>
#include	<string.h>

const char my_leet_map[][2] = { {'a', '4'}, {'i', '1'}, {'o', '0'}, {'e', '3'}, {0} };

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
int main()
{
  Bucket b, b2, bs, bf;
  Word wo, wi, wc;
  Transformator tr;
  Position pos_out;
  Generator gen;

  init_bucket_array(&b, 6);
  strcpy(b.array[0].word, "CIAO");
  strcpy(b.array[1].word, "a");
  strcpy(b.array[2].word, "tutti");
  strcpy(b.array[3].word, "come");
  strcpy(b.array[4].word, "state");
  strcpy(b.array[5].word, "?");
  print_bucket(&b);

  init_bucket_array(&bs, 3);
  strcpy(bs.array[0].word, "-");
  strcpy(bs.array[1].word, "");
  strcpy(bs.array[2].word, "+");
  printf("-------------\n");

  init_bucket_array(&b2, 3);

  init_transformator_leet(&tr, my_leet_map, true);
  next_word(&wi, &b);
  transform(&wo, &tr, &wi);
  set_word(&b2, &wo, &pos_out);
  set_bucket_position(&b2, &pos_out);

  init_transformator_appender(&tr, true, false, false, 100, 0, 0);
  next_word(&wi, &b);
  transform(&wo, &tr, &wi);
  set_word(&b2, &wo, &pos_out);
  set_bucket_position(&b2, &pos_out);

  strcpy(wc.word, "AHHAHAA");
  init_transformator_concat(&tr, &wc);
  next_word(&wi, &b);
  transform(&wo, &tr, &wi);
  set_word(&b2, &wo, &pos_out);
  set_bucket_position(&b2, &pos_out);

  print_bucket(&b2);
  printf("-------------\n");

  init_bucket_array(&b2, 4);
  init_bucket_file(&bf, "./dict/dict.test");
  init_generator_combination(&gen, 3);
  while(generate_next(&b2, &gen, &bf)) {
    print_bucket(&b2);
    printf("--\n");
  }
  printf("-------------\n");

  init_bucket_array(&b2, 3);
  set_bucket_size(&b, 3);
  init_generator_lexical_combination(&gen);
  set_bucket_position(&b, &b.first);
  while(generate_next(&b2, &gen, &b)) {
    print_bucket(&b2);
    printf("--\n");
  }
  printf("-------------\n");

  init_bucket_array(&b2, 3);
  set_bucket_size(&b, 6);
  init_generator_separator(&gen, &bs);
  set_bucket_position(&b, &b.first);
  while(generate_next(&b2, &gen, &b)) {
    print_bucket(&b2);
    printf("--\n");
  }
  printf("-------------\n");

  return EXIT_SUCCESS;
}				/* ----------  end of function main  ---------- */
