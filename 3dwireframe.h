#ifndef WIREFRAME_H
#define WIREFRAME_H

#include <stdbool.h>

#define N 3

typedef struct {
	double x;
	double y;
	double z;
} vec3_t;

typedef struct {
	double x;
	double y;
} vec2_t;


typedef struct {
	vec3_t* points;
	bool** lines;

	int size;
} wireframe3_t;

wireframe3_t* create_wireframe3(int size);
void destroy_wireframe3(wireframe3_t* w);

typedef struct {
	vec2_t* points;

	int size;
} wireframe2_t;

wireframe2_t* create_wireframe2(int size);
void destroy_wireframe2(wireframe2_t* w);


void matrix_multiply(double a[N][N], double b[N][N], double res[N][N]);
vec3_t vec_x_matrix_multiply(double m[N][N], vec3_t v);

void get_rotation_matrix_x(double angle, double m[N][N]);
void get_rotation_matrix_y(double angle, double m[N][N]);

wireframe3_t* create_cube();
void trasform_wireframe(wireframe3_t* src, wireframe3_t* dest, double tmatrix[3][3] );

#endif

