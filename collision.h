#pragma once

#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int checkDistance(long long x, long long y, long long r, Node *node);
int getNumCollisions(Node *node, int x, int y, int r);