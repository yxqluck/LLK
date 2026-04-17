#pragma once
#include "global.h"
class CGameControl
{
public:
	CGameControl();
	~CGameControl();

	void StartGame();
	int GetElement(int nRow, int nCol);
	void SetFirstPoint(int nRow, int nCol);
	void SetSectPoint(int nRow, int nCol);
    bool Link(Vertex avPath[], int &pathLen);
	int m_anMap[MAP_ROWS][MAP_COLS];
private:
    
	Vertex m_ptSelFirst;
	Vertex m_ptSelSec;
};

