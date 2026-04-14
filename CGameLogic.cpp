#include "pch.h"
#include "CGameLogic.h"
#include <vector>
#include <algorithm>
#include <random>

CGameLogic::CGameLogic()
{
}

CGameLogic::~CGameLogic()
{
}

void CGameLogic::InitMap(int anMap[][MAP_COLS]) {
	// initialize with pairs and shuffle to fill the MAP_ROWS x MAP_COLS grid
	int total = MAP_ROWS * MAP_COLS;
	int pairCount = total / 2;

	// Build pairCount image indices, cycling through available NUM_IMAGES
	std::vector<int> pairs;
	pairs.reserve(pairCount);
	for (int i = 0; i < pairCount; ++i) {
		pairs.push_back(i % NUM_IMAGES);
	}

	// expand to values (each image appears twice)
	std::vector<int> vals;
	vals.reserve(total);
	for (int v : pairs) {
		vals.push_back(v);
		vals.push_back(v);
	}

	// shuffle values
	std::random_device rd;
	std::mt19937 g(rd());
	std::shuffle(vals.begin(), vals.end(), g);

	int idx = 0;
	for (int i = 0; i < MAP_ROWS; ++i) {
		for (int j = 0; j < MAP_COLS; ++j) {
			anMap[i][j] = vals[idx++];
		}
	}
}

bool CGameLogic::IsLink(int anMap[][MAP_COLS],Vertex v1, Vertex v2, Vertex avPath[], int &pathLen) {
	
	/*int nRow1 = v1.row;
	int nCol1 = v1.col;
	int nRow2 = v2.row;
	int nCol2 = v2.col;

	if (nRow1 == nRow2 && nCol1 == nCol2)
		return false;
	int val1 = anMap[nRow1][nCol1];
	int val2 = anMap[nRow2][nCol2];
	if (val1 < 0 || val2 < 0)
		return false;
	return (val1 == val2);*/

    pathLen = 0;
	//一条直线联通
	int nRow1 = v1.row;
	int nRow2 = v2.row;
	int nCol1 = v1.col;
	int nCol2 = v2.col;
	if (nRow1 == nRow2) {
        if(LinkInRow(anMap, v1, v2)) {
			avPath[0] = v1; avPath[1] = v2; pathLen = 2; return true;
		}
	}
	if (nCol1 == nCol2) {
        if (LinkInCol(anMap, v1, v2)) {
			avPath[0] = v1; avPath[1] = v2; pathLen = 2; return true;
		}
	}
	//两条直线联通
    if (LinkInTwoLine(anMap, v1, v2, avPath, pathLen)) {
		return true;
	}

	//三条直线联通
    if(LinkInThreeLine(anMap, v1, v2, avPath, pathLen)) {
		return true;
	}

	return false;
}

void CGameLogic::Clear(int anMap[][MAP_COLS], Vertex v1, Vertex v2) {
	anMap[v1.row][v1.col] = BLANK;
	anMap[v2.row][v2.col] = BLANK;

}

bool CGameLogic::LinkInRow(int anMap[][MAP_COLS], Vertex v1, Vertex v2) {
    int row = v1.row;
	int c1 = v1.col;
	int c2 = v2.col;
	if (row != v2.row) return false;
	if (c1 == c2) return true; // same cell (handled elsewhere)
	if (c1 > c2) {
		int temp = c1;
		c1 = c2;
		c2 = temp;
	}
	for (int c = c1 + 1; c <= c2 - 1; ++c) {
		if (anMap[row][c] != BLANK) return false;
	}
	return true;
}

bool CGameLogic::LinkInCol(int anMap[][MAP_COLS], Vertex v1, Vertex v2) {
    int col = v1.col;
	int r1 = v1.row;
	int r2 = v2.row;
	if (col != v2.col) return false;
	if (r1 == r2) return true;
	if (r1 > r2) {
		int temp = r1;
		r1 = r2;
		r2 = temp;
	}
	for (int r = r1 + 1; r <= r2 - 1; ++r) {
		if (anMap[r][col] != BLANK) return false;
	}
	return true;
}

bool CGameLogic::LinkInTwoLine(int anMap[][MAP_COLS], Vertex v1, Vertex v2, Vertex avPath[], int &pathLen) {
    // One-turn (L-shaped) connection: check two possible corners
	// corner at (v1.row, v2.col)
	Vertex corner1 = { v1.row, v2.col, anMap[v1.row][v2.col] };
	if (anMap[corner1.row][corner1.col] == BLANK) {
        if (LinkInRow(anMap, v1, corner1) && LinkInCol(anMap, corner1, v2)) {
			avPath[0]=v1; avPath[1]=corner1; avPath[2]=v2; pathLen=3; return true;
		}
	}
	// corner at (v2.row, v1.col)
	Vertex corner2 = { v2.row, v1.col, anMap[v2.row][v1.col] };
	if (anMap[corner2.row][corner2.col] == BLANK) {
        if (LinkInCol(anMap, v1, corner2) && LinkInRow(anMap, corner2, v2)) {
			avPath[0]=v1; avPath[1]=corner2; avPath[2]=v2; pathLen=3; return true;
		}
	}
	return false;
}

bool CGameLogic::LinkInThreeLine(int anMap[][MAP_COLS], Vertex v1, Vertex v2, Vertex avPath[], int &pathLen) {
	// Two-turn connection: try patterns scanning intermediate columns/rows
	// Pattern A: v1 -> (v1.row, k) -> (v2.row, k) -> v2
    for (int k = 0; k < MAP_COLS; ++k) {
		Vertex p1 = { v1.row, k, anMap[v1.row][k] };
		Vertex p2 = { v2.row, k, anMap[v2.row][k] };
		if (anMap[p1.row][p1.col] != BLANK || anMap[p2.row][p2.col] != BLANK) continue;
		if (LinkInRow(anMap, v1, p1) && LinkInCol(anMap, p1, p2) && LinkInRow(anMap, p2, v2)) {
			avPath[0]=v1; avPath[1]=p1; avPath[2]=p2; avPath[3]=v2; pathLen=4; return true;
		}
	}
	// Pattern B: v1 -> (k, v1.col) -> (k, v2.col) -> v2
    for (int k = 0; k < MAP_ROWS; ++k) {
		Vertex p1 = { k, v1.col, anMap[k][v1.col] };
		Vertex p2 = { k, v2.col, anMap[k][v2.col] };
		if (anMap[p1.row][p1.col] != BLANK || anMap[p2.row][p2.col] != BLANK) continue;
		if (LinkInCol(anMap, v1, p1) && LinkInRow(anMap, p1, p2) && LinkInCol(anMap, p2, v2)) {
			avPath[0]=v1; avPath[1]=p1; avPath[2]=p2; avPath[3]=v2; pathLen=4; return true;
		}
	}
	return false;
}