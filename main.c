#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <termios.h>

#include "drawline.h"
#include "3dwireframe.h"
#include "project.h"

void wait_milliseconds(int milliseconds);
void interupt_handeler(int sig);

const double X_SCALE = 30;
const double Y_SCALE = 16;

const double X_BIAS = (((double)GRID_WIDTH)/2.);
const double Y_BIAS = (((double)GRID_HEIGHT)/2.);
static volatile sig_atomic_t keep_running = true;

int main() {
	struct termios old_settings, new_settings;
	signal(SIGINT, interupt_handeler);

	tcgetattr(STDIN_FILENO, &old_settings);
	new_settings = old_settings;
	new_settings.c_lflag &= (~ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_settings);

	char grid[GRID_HEIGHT][GRID_WIDTH];

	wireframe3_t* cube = create_cube();

	wireframe3_t* rotated_cube1 = create_wireframe3(cube->size);
	wireframe3_t* rotated_cube2 = create_wireframe3(cube->size);
	double rot_matrix_x[3][3];
	double rot_matrix_y[3][3];

	double rotation_x = 0;
	double rotation_y = 0;

	printf("Ctrl+C to exit\n");
	while(keep_running) {

		// clear the screen
		for (int i = 0; i < GRID_HEIGHT; i++)
			for (int j = 0; j < GRID_WIDTH; j++)
				grid[i][j] = ' ';

		// transform the cube using rotation matrix
		get_rotation_matrix_x(rotation_x, rot_matrix_x);
		get_rotation_matrix_y(rotation_y, rot_matrix_y);
		trasform_wireframe(cube, rotated_cube1, rot_matrix_x);
		trasform_wireframe(rotated_cube1, rotated_cube2, rot_matrix_y);
		// project the cube
		wireframe2_t* projed_cube = week_perspective_projection(rotated_cube2, 3., 2.5);

		// draw the edges of the cube
		for(int i= 0; i < cube->size; i++) {
			for(int j= 0; j < i; j++) {
				if(cube->lines[i][j]) {
					int x0 = (int)(X_SCALE * projed_cube->points[i].x + X_BIAS );
					int y0 = (int)(Y_SCALE * projed_cube->points[i].y + Y_BIAS );

					int x1 = (int)(X_SCALE * projed_cube->points[j].x + X_BIAS);
					int y1 = (int)(Y_SCALE * projed_cube->points[j].y + Y_BIAS);

					draw_line(x0, y0, x1, y1, grid);
					//printf("line %d --- %d\n", i,j);
					//grid[y0][x0] = i + '0';
					//grid[y1][x1] = j + '0';
				}
			}
		}

		// print the screen
		for (int i = 0; i < GRID_HEIGHT; i++)
		{
			for (int j = 0; j < GRID_WIDTH; j++)
				printf("%c", grid[i][j]);
			printf("\n");
		}
		fflush(stdout);

		printf("\r"); // go to the start of line
		for (int i = 0; i < GRID_HEIGHT; i++)
			printf("\x1b[A"); //back one line

		destroy_wireframe2(projed_cube);
		rotation_x += 0.04;
		rotation_y += 0.0143;
		wait_milliseconds(50);
	}
	destroy_wireframe3(cube);
	destroy_wireframe3(rotated_cube1);
	destroy_wireframe3(rotated_cube2);

	printf("\x1b[A"); //back one line
	for (int i = 0; i < GRID_HEIGHT+ 2; i++)
	{
		for (int j = 0; j < GRID_WIDTH; j++)
			printf(" ");
		printf("\n");
	}
	fflush(stdout);

	printf("\r"); // go to the start of line
	for (int i = 0; i < GRID_HEIGHT + 2; i++)
		printf("\x1b[A"); //back one line

	tcsetattr(STDIN_FILENO, TCSANOW, &old_settings);

	return 0;
}

void wait_milliseconds(int milliseconds) {
    usleep(milliseconds * 1000);
}

void interupt_handeler(int signal) {
	(void)signal;
	keep_running = false;
}
