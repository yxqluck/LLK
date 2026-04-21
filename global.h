#pragma once
#include<vector>
typedef struct Vertex
{
	int row;
	int col;
	int info;
} Vertex;

extern std::vector<CString>RANK;
extern std::vector<int>RANKTIME;

// µØÍ¼¹æ¸ñ
#define MAP_ROWS 8
#define MAP_COLS 14

#define BLANK -1

#define NUM_IMAGES 10
