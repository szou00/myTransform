#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"

int main(int argc, char **argv) {

  screen s;
  struct matrix * edges;
  struct matrix * transform;

  edges = new_matrix(4, 4);
  transform = new_matrix(4, 4);

  if ( argc == 2 )
    parse_file( argv[1], transform, edges, s );
  else
    parse_file( "stdin", transform, edges, s );

  printf("TESTING TRANSLATE: \n");
  edges = make_translate(1,2,3);
  print_matrix(edges);
  printf("\n");

  printf("TESTING SCALE: \n");
  edges = make_scale(1,2,3);
  print_matrix(edges);
  printf("\n");

  printf("TESTING ROTX: \n");
  edges = make_rotX(1);
  print_matrix(edges);
  printf("\n");

  printf("TESTING ROTY: \n");
  edges = make_rotY(1);
  print_matrix(edges);
  printf("\n");

  printf("TESTING ROTZ: \n");
  edges = make_rotZ(1);
  print_matrix(edges);
  printf("\n");

  free_matrix( edges );
  free_matrix( transform );
}
