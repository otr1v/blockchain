
#pragma once
#include "raylib.h"
#include <string>
#include <vector>
#include <iostream>
#include "blockchain.h"



int DrawTree(arranged_block_iterable_proxy current_block, int y, int x, float& rotationAngle, float& child_coord_x, float& child_coord_y, bool is_root);

