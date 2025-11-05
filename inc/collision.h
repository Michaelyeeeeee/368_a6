#pragma once

#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int checkDistance(int x, int y, int r, Node *node);
int getNumCollisions(Node *node, int x, int y, int r);