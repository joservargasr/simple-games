#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>

using namespace std;

wstring tetramino[7];
int nFieldWidth = 12;
int nFieldHeight = 18;
unsigned char* pField = nullptr;
int nScreenWidth = 80;
int nScreenHeight = 30;

int Rotate(int px,int py,int r)
{
	switch (r%4)
	{
	case 0: return py * 4 + px;
	case 1: return 12 + py - (4 * px);
	case 2: return 15 - (py * 4) - px;
	case 3: return 3 - py + (px * 4);
	}
	return 0;
}
bool DoesPieceFit(int nTetromino,int nRotation,int nPosX, int nPosY)
{
	for(int px=0;px<4;px++)
		for (int py = 0; py < 4; py++)
		{
			int pi = Rotate(px, py, nRotation);
			int fi = (nPosY + py) * nFieldWidth + (nPosX + px);
			if ((nPosX + px) >= 0 && (nPosX + px) < nFieldWidth)
			{
				if ((nPosY + py) >= 0 && (nPosY + py) < nFieldHeight)
				{
					if (tetramino[nTetromino][pi] == L'X' && pField[fi] != 0)
						return false;
				}
			}
		}

	return true;
}
int main()
{	
	srand(time(NULL));
	tetramino[0].append(L"..X.");
	tetramino[0].append(L"..X.");
	tetramino[0].append(L"..X.");
	tetramino[0].append(L"..X.");

	tetramino[1].append(L".XX.");
	tetramino[1].append(L"..X.");
	tetramino[1].append(L"..X.");
	tetramino[1].append(L"..X.");

	tetramino[2].append(L".X..");
	tetramino[2].append(L".XX.");
	tetramino[2].append(L"..X.");
	tetramino[2].append(L"....");

	tetramino[3].append(L"..X.");
	tetramino[3].append(L"..X.");
	tetramino[3].append(L"..X.");
	tetramino[3].append(L".XX.");

	tetramino[4].append(L"..X.");
	tetramino[4].append(L".XX.");
	tetramino[4].append(L".X..");
	tetramino[4].append(L"....");

	tetramino[5].append(L"..X.");
	tetramino[5].append(L".XX.");
	tetramino[5].append(L"..X.");
	tetramino[5].append(L"....");

	tetramino[6].append(L".XX.");
	tetramino[6].append(L".XX.");
	tetramino[6].append(L"....");
	tetramino[6].append(L"....");

	pField = new unsigned char[nFieldWidth * nFieldHeight];
	for (int x = 0; x < nFieldWidth; x++)
		for (int y = 0; y < nFieldHeight; y++)
			pField[y * nFieldWidth + x] = (x == 0 or x == nFieldWidth - 1 or y == nFieldHeight-1) ? 9 : 0;

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) screen[i] = L' ';
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	bool bGameOver = false;

	int nCurrentPiece = 2;
	int nCurrentRotation = 0;
	int nCurrentX = nFieldWidth / 2;
	int nCurrentY = 0;

	bool bKey[4];
	bool bRotateHold = false;
	int nSpeed = 20;
	int nSpeedCounter = 0;
	bool bForceDown = false;
	vector<int> vLines;
	int nPieceCount = 0;
	int nScore = 0;

	while (!bGameOver)
	{
		// GAME TIMING---------------------------------------------------------------------
		this_thread::sleep_for(50ms);
		nSpeedCounter++;
		bForceDown = (nSpeedCounter== nSpeed);

		// INPUT---------------------------------------------------------------------------
		for (int k = 0; k < 4; k++)
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("\x27\x25\x28Z"[k]))) != 0;

		// GAME LOGIC----------------------------------------------------------------------
		nCurrentX -= bKey[1] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY);
		nCurrentX += bKey[0] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY);
		nCurrentY += bKey[2] && DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1);

		if (bKey[3])
		{
			nCurrentRotation += !bRotateHold && DoesPieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY);
			bRotateHold = true;
		}
		else
			bRotateHold = false;

		if (bForceDown)
		{
			if(DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1))
			nCurrentY++;
			else
			{

			for (int px = 0; px < 4; px++)
				for (int py = 0; py < 4; py++)
				{
					if (tetramino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
						pField[(nCurrentY + py) * nFieldWidth + (nCurrentX + px)] = nCurrentPiece + 1;
				}

			nPieceCount++;
			if (nPieceCount % 10 == 0)
				if (nSpeed >= 10)
					nSpeed--;
			for (int py = 0; py < 4; py++)
			{
				if(nCurrentY+py<nFieldHeight-1)
				{
					bool bLine = true;
					for (int px = 1; px < nFieldWidth - 1; px++) 
					{
						bLine &= (pField[(nCurrentY + py) * nFieldWidth + px]) != 0;
					}
					if (bLine) 
					{
						for (int px = 1; px < nFieldWidth - 1; px++)
							pField[(nCurrentY + py) * nFieldWidth + px]= 8;

						vLines.push_back(nCurrentY + py);
					}
				}
			}
			nScore += 25;
			if (!vLines.empty()) nScore += (1 << vLines.size()) * 100;

			nCurrentPiece = rand() % 7;
			nCurrentRotation = 0;
			nCurrentX = nFieldWidth / 2;
			nCurrentY = 0;

			bGameOver = !DoesPieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}
			nSpeedCounter = 0;
		}
		
		
		// RENDER OUTPUT-------------------------------------------------------------------


		for (int x = 0; x < nFieldWidth; x++)
			for (int y = 0; y < nFieldHeight; y++)
			{
				screen[(y + 2) * nScreenWidth + (x + 2)] = L" ABCDEFG=#"[pField[y * nFieldWidth + x]];
			}

		for (int px = 0; px < 4; px++)
			for (int py = 0; py < 4; py++)
			{
				if (tetramino[nCurrentPiece][Rotate(px, py, nCurrentRotation)] == L'X')
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;
			}

		if(!vLines.empty())
		{
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth* nScreenHeight, { 0,0 }, & dwBytesWritten);
			this_thread::sleep_for(400ms);

			for (auto const & v : vLines)
			{
				for (int px = 1; px < nFieldWidth - 1; px++) {
					for (int py = v; py > 0; py--) {
						pField[py * nFieldWidth + px] = pField[(py - 1) * nFieldWidth + px];
					}
					pField[px] = 0;
				}
			}
			vLines.clear();
		}
		swprintf_s(&screen[2 * nScreenWidth + nFieldWidth + 6], 16, L"SCORE: %8d", nScore);

		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	CloseHandle(hConsole);
	cout << "Game Over!! Score: " << nScore<<endl;
	system("pause");
	return 0;
}