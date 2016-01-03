#include "stdafx.h"
#include "GameEngine.h"

CGameEngine::CGameEngine()
{
}


CGameEngine::~CGameEngine()
{
}

void CGameEngine::ChangeStatus(HWND &hwnd, HDC &hdc, MyPoint& pt)
{
	if (!_bGameStart)
	{
		if (_GameBoard.find(pt) == _GameBoard.end())
		{
			_GameBoard[pt] = eAlive;
			DrawAlive(hwnd, hdc, pt);
		}
		else
		{
			if (_GameBoard[pt] == eDead)
			{
				_GameBoard[pt] = eAlive;
				DrawAlive(hwnd, hdc, pt);
			}
			else
			{
				_GameBoard[pt] = eDead;
				DrawDead(hwnd, hdc, pt);
			}

		}
	}
	else
	{
		UpdateStatus(hwnd, hdc);
	}
}

void CGameEngine::DrawBackGround(HWND &hwnd, HDC &hdc)
{
	RECT rc;
	::GetWindowRect(hwnd, &rc);
	int nWidth = rc.right - rc.left;
	int nHeight = rc.bottom - rc.top;
	for (int i = 0; i < nWidth; i = i + 20)
	{
		for (int j = 0; j < nHeight; j = j + 20)
		{
			//»­ÊúÏß
			MoveToEx(hdc, i, 0, NULL);
			LineTo(hdc, i, j);
			//»­ºáÏß
			MoveToEx(hdc, 0, j, NULL);
			LineTo(hdc, i, j);
		}
	}
}

void CGameEngine::DrawAlive(HWND &hwnd, HDC &hdc, const MyPoint& pt)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	FillRect(hdc, &pt.GetRC(), hBrush);
	DeleteObject(hBrush);
	InvalidateRect(hwnd, &pt.GetRC(), false);
}

void CGameEngine::DrawDead(HWND &hwnd, HDC &hdc, const MyPoint& pt)
{
	HBRUSH hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
	FillRect(hdc, &pt.GetRC(), hBrush);
	DeleteObject(hBrush);
	InvalidateRect(hwnd, &pt.GetRC(), false);
}

void CGameEngine::ResetScreen(HWND &hwnd, HDC &hdc)
{
	RECT rc;
	::GetWindowRect(hwnd, &rc);
	InvalidateRect(hwnd, &rc, true);
}

void CGameEngine::GameStart(bool bStart)
{
	_bGameStart = bStart;
}

void CGameEngine::UpdateStatus(HWND &hwnd, HDC &hdc)
{
	_Recycle.clear();
	for (map<MyPoint, eLifeStatus>::iterator iter = _GameBoard.begin();iter != _GameBoard.end();++iter)
	{
		if (iter->second == eAlive)
		{
			_Recycle[iter->first] = eAlive;
			vector<MyPoint> vecPt = iter->first.GetPtsAround();
			for (size_t i = 0; i < vecPt.size(); ++i)
			{
				MyPoint &pt = vecPt[i];
				if (_GameBoard.find(pt) != _GameBoard.end())
				{
					_Recycle[pt] = _GameBoard[pt];
				}
				else
				{
					_Recycle[pt] = eDead;
				}
			}
		}
		else
		{
			_Recycle[iter->first] = eDead;
		}
	}

	vector<MyPoint> vecT;

	for (map<MyPoint, eLifeStatus>::iterator iter = _Recycle.begin();iter != _Recycle.end();++iter)
	{
		eLifeStatus es = UpdateStatus(iter->first, iter->second);
		_Recycle[iter->first] = es;
		if (es == eAlive)
		{
			DrawAlive(hwnd, hdc, iter->first);
		}
		else
		{
			DrawDead(hwnd, hdc, iter->first);
			vecT.push_back(iter->first);
		}
	}

	for (size_t i = 0; i < vecT.size();++i)
	{
		_Recycle.erase(vecT[i]);
	}

	_GameBoard.swap(_Recycle);
}

CGameEngine::eLifeStatus CGameEngine::UpdateStatus(const MyPoint& pt, eLifeStatus st)
{
	vector<MyPoint> vecPt = pt.GetPtsAround();
	int i = 0;
	for (vector<MyPoint>::iterator iter = vecPt.begin();iter != vecPt.end(); ++iter)
	{
		MyPoint &ptTemp = *iter;
		if (_GameBoard.find(ptTemp) != _GameBoard.end())
		{
			if (_GameBoard[ptTemp] == eAlive)
				++i;
		}
	}
	if (i < 2)
		return eDead;
	else if (i == 2 && st == eAlive)
		return eAlive;
	else if (i == 3)
		return eAlive;
	else
		return eDead;
}

void CGameEngine::Reset()
{
	_bGameStart = false;
	_GameBoard.clear();
	_Recycle.clear();
}

MyPoint::MyPoint(int nx /*= 0*/, int ny /*= 0*/) :x(nx), y(ny)
{
	if (x % 20 != 0)
	{
		x -= x % 20;
	}
	if (y % 20 != 0)
	{
		y -= y % 20;
	}
}

MyPoint::~MyPoint()
{

}

bool MyPoint::operator<(const MyPoint& pt) const
{
	return x == pt.x ? (y < pt.y) : (x < pt.x);
}

RECT MyPoint::GetRC() const
{
	RECT rc;
	rc.top = y;
	rc.left = x;
	rc.right = x + 20;
	rc.bottom = y + 20;
	return rc;
}

vector<MyPoint> MyPoint::GetPtsAround() const
{
	vector<MyPoint> vecPt;
	vecPt.push_back(MyPoint(x, y - 20));
	vecPt.push_back(MyPoint(x, y + 20));
	vecPt.push_back(MyPoint(x - 20, y));
	vecPt.push_back(MyPoint(x - 20, y - 20));
	vecPt.push_back(MyPoint(x - 20, y + 20));
	vecPt.push_back(MyPoint(x + 20, y));
	vecPt.push_back(MyPoint(x + 20, y - 20));
	vecPt.push_back(MyPoint(x + 20, y + 20));
	return vecPt;
}
