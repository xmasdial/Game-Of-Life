#pragma once
#include <map>
#include <vector>
using namespace std;

class MyPoint 
{
public:
	MyPoint(int nx = 0, int ny = 0);
	~MyPoint();

public:
	bool operator<(const MyPoint& pt) const;
	RECT GetRC() const;
	vector<MyPoint> GetPtsAround() const;
public:
	int x;
	int y;
};

class CGameEngine
{
public:
	enum eLifeStatus
	{
		eAlive = 0,
		eDead
	};

public:
	CGameEngine();
	~CGameEngine();

public:
	void ChangeStatus(HWND &hwnd, HDC &hdc, MyPoint& pt);
	void GameStart(bool bStart);
	void UpdateStatus(HWND &hwnd, HDC &hdc);
	void Reset();
public:
	void DrawBackGround(HWND &hwnd, HDC &hdc);
	void DrawAlive(HWND &hwnd, HDC &hdc, const MyPoint& pt);
	void DrawDead(HWND &hwnd, HDC &hdc, const MyPoint& pt);
	void ResetScreen(HWND &hwnd, HDC &hdc);

private:
	eLifeStatus UpdateStatus(const MyPoint& pt, eLifeStatus st);
private:
	map<MyPoint, eLifeStatus>	_GameBoard;
	map<MyPoint, eLifeStatus>	_Recycle;
	bool						_bGameStart;
};

