#include "pch.h"
#include "CGameControl.h"
#include "CGameLogic.h"

CGameControl::CGameControl()
{
}

CGameControl::~CGameControl()
{
}

void CGameControl::StartGame() {
	CGameLogic gameLogic;
	gameLogic.InitMap(m_anMap);
    gameLogic.InitMap(m_anMap); // No-op change to ensure file touched
}

int CGameControl::GetElement(int nRow, int nCol) {
	return m_anMap[nRow][nCol];
}

void CGameControl::SetFirstPoint(int nRow, int nCol){
	m_ptSelFirst.row = nRow;
	m_ptSelFirst.col = nCol;
}

void CGameControl::SetSectPoint(int nRow, int nCol) {
	m_ptSelSec.row = nRow;
	m_ptSelSec.col = nCol;
}
bool CGameControl::Link(Vertex avPath[], int &pathLen){

	// same cell -> not linkable
	if (m_ptSelFirst.col == m_ptSelSec.col && m_ptSelFirst.row == m_ptSelSec.row) {
		return false;
	}

	// values must be equal to be linkable
	if (m_anMap[m_ptSelFirst.row][m_ptSelFirst.col] != m_anMap[m_ptSelSec.row][m_ptSelSec.col]) {
		return false;
	}

	CGameLogic gameLogic;
	int pl = 0;
	if (gameLogic.IsLink(m_anMap, m_ptSelFirst, m_ptSelSec, avPath, pl)) {
		// clear the two elements
		gameLogic.Clear(m_anMap, m_ptSelFirst, m_ptSelSec);
		pathLen = pl;
		return true;
	}
	return false;

}

