#pragma once

#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int checkDistance(int x, int y, int r, Node *node);
void getMaxMinXMaxMinY(int x, int y, int r, int *maxX, int *minX, int *maxY, int *minY);
int getNumCollisions(Node *node, int maxX, int minX, int maxY, int minY, int r);