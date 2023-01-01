#include "project.h"

wireframe2_t* week_perspective_projection(wireframe3_t* shape, double focal_length, double z_bias) {
	wireframe2_t* rt = create_wireframe2(shape->size);

	for(int i = 0; i < shape->size; i++) {
		//project points to the xy plane
		float z = shape->points[i].z + z_bias;
		rt->points[i].x = (shape->points[i].x * focal_length) / (z + focal_length);
		rt->points[i].y = (shape->points[i].y * focal_length) / (z + focal_length);
		//rt ->  ...  z = 0;

	}

	return rt;
}
