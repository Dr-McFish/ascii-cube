#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "3dwireframe.h"


wireframe3_t* create_wireframe3(int size) {
	wireframe3_t* rt = malloc(sizeof(wireframe3_t));

	rt->points = calloc(size, sizeof(vec3_t));
	rt->lines = calloc(size * size, sizeof(bool));
	rt->size = size;

	return rt;
}

void destroy_wireframe3(wireframe3_t* w) {
	free(w->points);
	free(w->lines);
	free(w);
}

wireframe2_t* create_wireframe2(int size) {
	wireframe2_t* rt = malloc(sizeof(wireframe2_t));

	rt->points = calloc(size, sizeof(vec2_t));
	rt->size = size;

	return rt;
}

void destroy_wireframe2(wireframe2_t* w) {
	free(w->points);
	free(w);
}

void matrix_multiply(double a[N][N], double b[N][N], double res[N][N]) {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) for (int k = 0; k < N; k++) res[i][j] += a[i][k] * b[k][j];
}

vec3_t vec_x_matrix_multiply(double m[N][N], vec3_t v) {
	vec3_t result;

    result.x = m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z;
    result.y = m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z;
    result.z = m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z;

	return result;
}

void get_rotation_matrix_x(double angle, double m[N][N]) {
	double c = cos(angle);
	double s = sin(angle);

	double mr[N][N] = {
		{1, 0, 0},
		{0, c,-s},
		{0, s, c}
	};
	memcpy(m, mr, N*N*sizeof(double));
}

void get_rotation_matrix_y(double angle, double m[N][N]) {
	double c = cos(angle);
	double s = sin(angle);

	double mr[N][N] = {
		{ c, 0, s},
		{ 0, 1, 0},
		{-s, 0, c}
	};

	memcpy(m, mr, N*N*sizeof(double));
}

wireframe3_t* create_cube() {
	#define NUM_POINTS_CUBE 8
	#define NUM_EDGES_CUBE 12

	// Vertices of a cube
	const vec3_t points[NUM_POINTS_CUBE] = {
	    {-1, -1, -1},	// 0
	    { 1, -1, -1},	// 1
	    { 1,  1, -1},	// 2
	    {-1,  1, -1},	// 3
	    {-1, -1,  1},	// 4
	    { 1, -1,  1},	// 5
	    { 1,  1,  1},	// 6
	    {-1,  1,  1}	// 7
	};

	// Edge table for a cube
	const bool lines[NUM_POINTS_CUBE][NUM_POINTS_CUBE] = {
	    //0  1  2  3  4  5  6  7
		{ 0, 1, 0, 1, 1, 0, 0, 0 }, // 0
		{ 1, 0, 1, 0, 0, 1, 0, 0 }, // 1
		{ 0, 1, 0, 1, 0, 0, 1, 0 }, // 2
		{ 1, 0, 1, 0, 0, 0, 0, 1 }, // 3
		{ 1, 0, 0, 0, 0, 1, 0, 1 }, // 4
		{ 0, 1, 0, 0, 1, 0, 1, 0 }, // 5
		{ 0, 0, 1, 0, 0, 1, 0, 1 }, // 6
		{ 0, 0, 0, 1, 1, 0, 1, 0 }  // 7
	};

	wireframe3_t* wireframe = malloc(sizeof(wireframe3_t));

    wireframe->size = NUM_POINTS_CUBE;
    wireframe->points = (vec3_t*) malloc(wireframe->size * sizeof(vec3_t));
    wireframe->lines = (bool**) malloc(wireframe->size * sizeof(bool*));
    for (int i = 0; i < wireframe->size; i++) {
        wireframe->lines[i] = (bool*) malloc(wireframe->size * sizeof(bool));
		memcpy(wireframe->lines[i], lines[i], NUM_POINTS_CUBE * sizeof(bool));
	}

	memcpy(wireframe->points, points, NUM_POINTS_CUBE * sizeof(vec3_t));

	return wireframe;
}

void trasform_wireframe(wireframe3_t* src, wireframe3_t* dest, double tmatrix[3][3]) {
	for(int i = 0; i < dest->size; i++) {
		dest->points[i] = vec_x_matrix_multiply(tmatrix, src->points[i]);
	}
}
