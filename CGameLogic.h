#pragma once
#include "global.h"

class CGameLogic
{
public:
	CGameLogic();
	~CGameLogic();
public:
    void InitMap(int anMap[][MAP_COLS]);
    // Determine if v1 and v2 can be linked. If linked, fill avPath with the sequence of vertices
	// that represent the connection (including endpoints) and set pathLen to the number of points.
	bool IsLink(int anMap[][MAP_COLS], Vertex v1, Vertex v2, Vertex avPath[], int &pathLen);
	void Clear(int anMap[][MAP_COLS], Vertex v1, Vertex v2);
	bool LinkInRow(int anMap[][MAP_COLS], Vertex v1, Vertex v2);
	bool LinkInCol(int anMap[][MAP_COLS], Vertex v1, Vertex v2);
    bool LinkInTwoLine(int anMap[][MAP_COLS], Vertex v1, Vertex v2, Vertex avPath[], int &pathLen);
	bool LinkInThreeLine(int anMap[][MAP_COLS], Vertex v1, Vertex v2, Vertex avPath[], int &pathLen);
};

