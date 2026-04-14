#pragma once
typedef struct Vertex
{
	int row;
	int col;
	int info;
} Vertex;
// map size
#define MAP_ROWS 8
#define MAP_COLS 8

#define BLANK -1
// number of distinct tile images available in the element sprite (0..NUM_IMAGES-1)
#define NUM_IMAGES 10
