/*==========================================
  A matrix will be a 4xN array of doubles
  Each column will represent an [x, y, z, 1] point.
  For multiplication purposes, consider the rows like so:
  x0  x1      xn
  y0  y1      yn
  z0  z1  ... zn
  1  1        1
  ==========================================*/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "matrix.h"
#include "draw.h"


/*======== struct matrix * make_translate() ==========
Inputs:  int x
         int y
         int z
Returns: The translation matrix created using x, y and z
as the translation offsets.
====================*/
struct matrix * make_translate(double x, double y, double z) {
  // int r, c;
  struct matrix *tmp;
  tmp = new_matrix(4, 3);
  ident(tmp);
  add_point(tmp, x, y, z);
  return tmp;
}

/*======== struct matrix * make_scale() ==========
Inputs:  int x
         int y
         int z
Returns: The translation matrix creates using x, y and z
as the scale factors
====================*/
struct matrix * make_scale(double x, double y, double z) {
  int c = 0;
  struct matrix *tmp;
  tmp = new_matrix(4, 4);
  add_point(tmp, x, 0, 0);
  add_point(tmp, 0, y, 0);
  add_point(tmp, 0, 0, z);
  add_point(tmp, 0, 0, 0);
  for (c = 0; c < 3; c++) {
    tmp->m[3][c]= 0;
  }
  return tmp;
}

/*======== struct matrix * make_rotX() ==========
Inputs:  double theta

Returns: The rotation matrix created using theta as the
angle of rotation and X as the axis of rotation.
====================*/
struct matrix * make_rotX(double theta) {
  theta =theta * M_PI / 180 ;
  struct matrix *tmp;
  tmp = new_matrix(4, 4);
  add_point(tmp, 1, 0, 0);
  add_point(tmp, 0, cos(theta), (sin(theta)));
  add_point(tmp, 0, (-1 * sin(theta)), cos(theta));
  add_point(tmp, 0, 0, 0);
  tmp->m[3][0]= 0;
  tmp->m[3][1]= 0;
  tmp->m[3][2]= 0;
  return tmp;
}

/*======== struct matrix * make_rotY() ==========
Inputs:  double theta
         char c
Returns: The rotation matrix created using theta as the
angle of rotation and Y as the axis of rotation.
====================*/
struct matrix * make_rotY(double theta) {
  theta =theta * M_PI / 180;
  struct matrix *tmp;
  tmp = new_matrix(4, 4);
  add_point(tmp, cos(theta), 0, (-1 * sin(theta)));
  add_point(tmp, 0, 1, 0);
  add_point(tmp, sin(theta), 0, cos(theta));
  add_point(tmp, 0, 0, 0);
  tmp->m[3][0]= 0;
  tmp->m[3][1]= 0;
  tmp->m[3][2]= 0;
  return tmp;
}

/*======== struct matrix * make_rotZ() ==========
Inputs:  double theta
         char c
Returns: The rotation matrix created using theta as the
angle of rotation and Z as the axis of rotation.
====================*/
struct matrix * make_rotZ(double theta) {
  theta =theta * M_PI / 180;
  struct matrix *tmp;
  tmp = new_matrix(4, 4);
  add_point(tmp, cos(theta), sin(theta), 0);
  add_point(tmp, -sin(theta), cos(theta), 0);
  add_point(tmp, 0, 0, 1);
  add_point(tmp, 0, 0, 0);
  tmp->m[3][0]= 0;
  tmp->m[3][1]= 0;
  tmp->m[3][2]= 0;
  return tmp;
}


/*-------------- void print_matrix() --------------
Inputs:  struct matrix *m

print the matrix such that it looks like
the template in the top comment
*/
void print_matrix(struct matrix *m) {

  int r, c;
  for (r=0; r < m->rows; r++) {
    for (c=0; c < m->lastcol; c++)
      printf("%0.2f ", m->m[r][c]);
    printf("\n");
  }
}//end print_matrix

/*-------------- void ident() --------------
Inputs:  struct matrix *m <-- assumes m is a square matrix

turns m in to an identity matrix
*/
void ident(struct matrix *m) {
  int r, c;

  for (r=0; r < m->rows; r++)
    for (c=0; c < m->cols; c++)
      if ( r == c )
	m->m[r][c] = 1;
      else
	m->m[r][c] = 0;
  m->lastcol = m->cols;
}//end ident


/*-------------- void matrix_mult() --------------
Inputs:  struct matrix *a
         struct matrix *b

multiply a by b, modifying b to be the product
a*b -> b
*/
void matrix_mult(struct matrix *a, struct matrix *b) {
  int r, c;
  struct matrix *tmp;
  tmp = new_matrix(4, 1);

  for (c=0; c < b->lastcol; c++) {

    //copy current col (point) to tmp
    for (r=0; r < b->rows; r++)
      tmp->m[r][0] = b->m[r][c];

    for (r=0; r < b->rows; r++)
      b->m[r][c] = a->m[r][0] * tmp->m[0][0] +
	a->m[r][1] * tmp->m[1][0] +
	a->m[r][2] * tmp->m[2][0] +
	a->m[r][3] * tmp->m[3][0];
  }
  free_matrix(tmp);
}//end matrix_mult


/*===============================================
  These Functions do not need to be modified
  ===============================================*/

/*-------------- struct matrix *new_matrix() --------------
Inputs:  int rows
         int cols
Returns:

Once allocated, access the matrix as follows:
m->m[r][c]=something;
if (m->lastcol)...
*/
struct matrix *new_matrix(int rows, int cols) {
  double **tmp;
  int i;
  struct matrix *m;

  tmp = (double **)malloc(rows * sizeof(double *));
  for (i=0;i<rows;i++) {
      tmp[i]=(double *)malloc(cols * sizeof(double));
    }

  m=(struct matrix *)malloc(sizeof(struct matrix));
  m->m=tmp;
  m->rows = rows;
  m->cols = cols;
  m->lastcol = 0;

  return m;
}


/*-------------- void free_matrix() --------------
Inputs:  struct matrix *m
Returns:

1. free individual rows
2. free array holding row pointers
3. free actual matrix
*/
void free_matrix(struct matrix *m) {

  int i;
  for (i=0;i<m->rows;i++) {
      free(m->m[i]);
    }
  free(m->m);
  free(m);
}


/*======== void grow_matrix() ==========
Inputs:  struct matrix *m
         int newcols
Returns:

Reallocates the memory for m->m such that it now has
newcols number of collumns
====================*/
void grow_matrix(struct matrix *m, int newcols) {

  int i;
  for (i=0;i<m->rows;i++) {
      m->m[i] = realloc(m->m[i],newcols*sizeof(double));
  }
  m->cols = newcols;
}


/*-------------- void copy_matrix() --------------
Inputs:  struct matrix *a
         struct matrix *b
Returns:

copy matrix a to matrix b
*/
void copy_matrix(struct matrix *a, struct matrix *b) {

  int r, c;

  for (r=0; r < a->rows; r++)
    for (c=0; c < a->cols; c++)
      b->m[r][c] = a->m[r][c];
}
