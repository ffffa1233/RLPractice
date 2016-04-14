#ifndef BOX_H
#define BOX_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <Box2D/Box2D.h>

void box_init();
double get_position();
double get_velocity();
void apply_force(int random_number);
void box_cleanup();
#endif
