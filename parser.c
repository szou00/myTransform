#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename
          struct matrix * transform,
          struct matrix * pm,
          screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix -
               takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
         ident: set the transform matrix to the identity matrix -
         scale: create a scale matrix,
                then multiply the transform matrix by the scale matrix -
                takes 3 arguments (sx, sy, sz)
         translate: create a translation matrix,
                    then multiply the transform matrix by the translation matrix -
                    takes 3 arguments (tx, ty, tz)
         rotate: create a rotation matrix,
                 then multiply the transform matrix by the rotation matrix -
                 takes 2 arguments (axis, theta) axis should be x y or z
         apply: apply the current transformation matrix to the edge matrix
         display: clear the screen, then
                  draw the lines of the edge matrix to the screen
                  display the screen
         save: clear the screen, then
               draw the lines of the edge matrix to the screen
               save the screen to a file -
               takes 1 argument (file name)
         quit: end parsing

See the file script for an example of the file format

IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to conver those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename,
                  struct matrix * transform,
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  clear_screen(s);
  char * nums;
  int x0,y0,z0,x1,y1,z1,sx,sy,sz;
  int l = 0;
  int m = 0;
  int r = 0;
  int sc = 0;
  int sv = 0;
  color c;
  c.red = 135;
  c.green = 206;
  c.blue = 250;

  if ( strcmp(filename, "stdin") == 0 )
    f = stdin;
  else
    f = fopen(filename, "r");

  while ( fgets(line, 255, f) != NULL ) {

    if (l) {
      // printf("it's a line\n");
      nums = strtok (line," ");
      x0 = atoi(nums);
      // printf ("x0: %s\n",nums);
      nums = strtok (NULL, " ");

      y0 = atoi(nums);
      // printf ("y0: %s\n",nums);
      nums = strtok (NULL, " ");

      z0 = atoi(nums);
      // printf ("z0: %s\n",nums);
      nums = strtok (NULL, " ");

      x1 = atoi(nums);
      // printf ("x1: %s\n",nums);
      nums = strtok (NULL, " ");

      y1 = atoi(nums);
      // printf ("y1: %s\n",nums);
      nums = strtok (NULL, " ");

      z1 = atoi(nums);
      // printf ("z1: %s\n",nums);
      nums = strtok (NULL, " ");

      add_edge(edges, x0, y0, z0, x1, y1, z1);
      l = 0;
    }

    if (sc) {

      nums = strtok (line," ");
      sx = atoi(nums);
      printf ("sx: %s\n",nums);
      nums = strtok (NULL, " ");

      sy = atoi(nums);
      printf ("sy: %s\n",nums);
      nums = strtok (NULL, " ");

      sz = atoi(nums);
      printf ("sz: %s\n",nums);
      nums = strtok (NULL, " ");

      struct matrix * tmp = make_scale(sx, sy, sz);
      matrix_mult(tmp, transform);
      print_matrix(transform);
      sc = 0;
    }

    if (m) {
      nums = strtok (line," ");
      x0 = atoi(nums);
      printf ("mx: %s\n",nums);
      nums = strtok (NULL, " ");

      y0 = atoi(nums);
      printf ("my: %s\n",nums);
      nums = strtok (NULL, " ");

      z0 = atoi(nums);
      printf ("mz: %s\n",nums);
      nums = strtok (NULL, " ");

      struct matrix * tmp = make_scale(x0, y0, z0);
      matrix_mult(tmp, transform);
      print_matrix(transform);
      m = 0;
    }

    // if (r) {
    //   nums = strtok (line," ");
    //   printf ("rotating about: %s\n",nums);
    //   if (strcmp(nums, "x") == 0) {
    //     nums = strtok (NULL, " ");
    //     make_rotX(atoi(nums));
    //   }
    //   if (strcmp(nums, "y") == 0) {
    //     nums = strtok (NULL, " ");
    //     make_rotY(atoi(nums));
    //   }
    //   if (strcmp(nums, "z") == 0) {
    //     nums = strtok (NULL, " ");
    //     make_rotZ(atoi(nums));
    //   }
    //
    //   struct matrix * tmp = make_scale(x0, y0, z0);
    //   matrix_mult(tmp, transform);
    //   print_matrix(transform);
    //   r = 0;
    // }

    if (sv) {
      nums = strtok (line," ");
      printf("in file: %s\n", nums);
      clear_screen(s);
      draw_lines(edges, s, c);
      display(s);
      save_extension(s, nums);
      save_ppm(s, "binary.ppm");
      save_ppm_ascii(s, "ascii.ppm");
    }

    line[strlen(line)-1]='\0';

    if (strcmp(line, "line") == 0) {
      l = 1;
      printf("it's a line!\n");
    }

    if (strcmp(line, "display") == 0) {
      printf("it's displaying!\n");
      clear_screen(s);
      draw_lines(edges, s, c);
      display(s);
    }

    if (strcmp(line, "ident") == 0) {
      printf("identity!\n");
      ident(transform);
      print_matrix(transform);
    }

    if (strcmp(line, "move") == 0) {
      printf("translating!\n");
      m = 1;
    }

    if (strcmp(line, "scale") == 0) {
      printf("scaling!\n");
      sc = 1;
    }

    // if (strcmp(line, "rotate") == 0) {
    //   printf("rotating!\n");
    //   r = 1;
    // }

    if (strcmp(line, "save") == 0) {
      printf("saving!\n");
      sv = 1;
    }

    if (strcmp(line, "apply") == 0) {
      printf("applying!\n");
      matrix_mult(transform, edges);
      print_matrix(edges);
    }

    if (strcmp(line, "quit") == 0) {
      printf("quitting :( bye bye)\n");
      break;
    }


  }

}
