#include "stdafx.h"
#include "Hprocess.h"
#include <iostream>
#include <string>

using namespace std;

struct Player
{
	float x;
	float y;
	float z;
	float aimY;
	float aimX;
};

HDC HDC_Desktop;
HBRUSH EnemeyBrush;
DWORD iterateNum = 1;
float ZeroFloat = 0;
void lf2UnlimitHPandMPandGodmode()
{
	DWORD Player = 0x58C9C;
	DWORD hpOffset = 0x2FC;
	DWORD mpOffset = 0x308;
	DWORD PlayerState = 0x8;

	CHackProcess fProcess;
	printf("This hack will continue when you'll run lf2\n");
	fProcess.RunProcess("lf2.exe", (LPCWSTR)"Little Fighter 2");//we first find the process. 
	printf("lf2 found !\nGodmode and unlimit hp/mp has been given !");
	DWORD LocalPlayer;
	int previousHP;
	//here we read what is the hp of the character.
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"lf2.exe", fProcess.__gameProcess.th32ProcessID) + Player), &LocalPlayer, sizeof(DWORD), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + hpOffset), &previousHP, sizeof(int), 0);
	int MaxHP = 500;
	int invincibleTrue = 1;//1 is godmode, 2 or 3 is invisible
	while (1 == 1)
	{
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"lf2.exe", fProcess.__gameProcess.th32ProcessID) + Player), &LocalPlayer, sizeof(DWORD), 0);
		//we do maxhp and maxmp everytime
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + hpOffset), &MaxHP, sizeof(int), 0);
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + mpOffset), &MaxHP, sizeof(int), 0);
		//We put the state we want the character will be.
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + PlayerState), &invincibleTrue, sizeof(int), 0);
	}
}

float* calcAngles(float myX, float hisX, float myY, float hisY, float myZ, float hisZ)
{
	float aimX = hisX - myX;
	float aimY = hisY - myY;
	float aimZ = hisZ - myZ + 6;
	float magnitude = sqrt(pow(aimX, 2) + pow(aimY, 2) + pow(aimZ, 2));
	float* angles = new float[2];//0 is x and 1 is y
	angles[0] = atanf(aimY / aimX);
	angles[1] = -asinf(aimZ / magnitude);
	angles[0] = angles[0] * (180 / 3.14159265358979323846);
	angles[1] = angles[1] * (180 / 3.14159265358979323846);

	if (aimX < 0) {
		angles[0] += 180;
	}
	return angles;
}

void DrawFilledRect(int x, int y, int w, int h)
{
	RECT rect = { x,y,x + w,y + h };
	FillRect(HDC_Desktop, &rect, EnemeyBrush);
}

void DrawBorderBox(int x, int y, int w, int h)
{
	DrawFilledRect(x, y, w, 2);
	DrawFilledRect(x, y, 2, h);
	DrawFilledRect(x + w, y, 2, h);
	DrawFilledRect(x, y + h, w + 2, 2);

}

void cs16AimBot()
{
	DWORD lookingAtSome1 = 0x663CC;
	DWORD doattack = 0x5971C;
	DWORD health = 0x594E0;
	DWORD localPlayer = 0x66370;
	DWORD crosshair = 0x190EC;//this offset
	DWORD playerSpot = 0xF3E1C;
	DWORD EntityList = 0x1E6F8;//hw.dll(offset 20)
	DWORD LIST = 0xF3E1C;
	CHackProcess fProcess;
	printf("This hack will continue when you'll run cs1.6\n");
	fProcess.RunProcess("hl.exe", (LPCWSTR)L"Counter-Strike");//we first find the process(Will wait untill it will be opened). 
	printf("cs1.6 found !\naimbot will start\n");
	HDC_Desktop = GetDC(fProcess.__HWNDCss);
	EnemeyBrush = CreateSolidBrush(RGB(255, 0, 0));
	//fProcess.PrintModules(fProcess.__gameProcess.th32ProcessID);
	DWORD LocalPlayer;
	DWORD Attack;
	DWORD entityList;
	DWORD aim;
	DWORD temp;
	DWORD spot;
	DWORD people;
	DWORD ClosestEntity;
	int AimingID;
	int previousHP;
	int Data;

	float PlayerX;
	float PlayerY;
	float PlayerZ;
	float TargetX;
	float TargetY;
	float TargetZ;

	float CurrentX;
	float CurrentY;

	float tempMyX, tempMyY, tempMyZ;
	float tempEnemeyX, tempEnemeyY, tempEnemeyZ;
	float tempCalculatedX, tempCalculatedY;

	float CalculatedX = 4.5;
	float CalculatedY = 30;
	//here we read what is the hp of the character.
	int isCTorT;//1 is T, 0 is CT
	bool found = false;
	int numOfPeople;
	DWORD Entity;
	float VectorSize;
	DWORD LastEntity = 0;
	float firstX = 0;
	DWORD EntityBase = 0x48EC608;
	float TEMPPPP = 0;
	float ClosestAngle;
	float firstAngle;
	float secondAngle;
	float firstXangle, secondXangle, firstYangle, secondYangle;
	float beforeX, beforeY, beforeZ, beforeAimY, beforeAimX;
	bool* players = new bool[32];
	int* afterXArray = new int[32];
	int* afterYArray = new int[32];
	int* afterZArray = new int[32];
	int* afterAimXArray = new int[32];
	int* afterAimYArray = new int[32];

	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + EntityList), &entityList, sizeof(DWORD), 0);
	for (int i = 0; i < 32; i++) {
		Entity = LastEntity + i * 0x28;
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x0), &afterXArray[i] , sizeof(float), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x4), &afterYArray[i], sizeof(float), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x8), &afterZArray[i], sizeof(float), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0xC), &afterAimYArray[i], sizeof(float), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x10), &afterAimXArray[i], sizeof(float), 0);
	}
	while (1 == 1)
	{
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + EntityList), &entityList, sizeof(DWORD), 0);
		ClosestAngle = 100000000;
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + crosshair), &aim, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + playerSpot), &spot, sizeof(DWORD), 0);
		LastEntity = entityList + 0x20;
		if (iterateNum % 57 == 0)
		{
			for (int i = 0; i < 32; i++) {
				Entity = LastEntity + i * 0x28;
				beforeX = afterXArray[i];
				beforeY = afterYArray[i];
				beforeZ = afterZArray[i];
				beforeAimY = afterAimYArray[i];
				beforeAimX = afterAimXArray[i];
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x0), &afterXArray[i], sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x4), &afterYArray[i], sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x8), &afterZArray[i], sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0xC), &afterAimYArray[i], sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x10), &afterAimXArray[i], sizeof(float), 0);
				if ((beforeX == afterXArray[i] && beforeY == afterYArray[i] && beforeZ == afterZArray[i] && beforeAimY == afterAimYArray[i] && beforeAimX == afterAimXArray[i]))
				{
					players[i] = false;
				}
				else 
				{
					players[i] = true;
				}
			}
			iterateNum = 1;
		}
		LastEntity = entityList + 0x20;
		for (int i = 0; i < 32; i++) {
			if (!players[i]) {
				continue;
			}
			Entity = LastEntity + i * 0x28;
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x140), &PlayerX, sizeof(float), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x144), &PlayerY, sizeof(float), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x148), &PlayerZ, sizeof(float), 0);

			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x0), &TargetX, sizeof(float), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x4), &TargetY, sizeof(float), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x8), &TargetZ, sizeof(float), 0);

			if (TargetX == PlayerX && TargetY == PlayerY && TargetZ == PlayerZ)
				continue;

			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CurrentY, sizeof(int), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CurrentX, sizeof(int), 0);

			CalculatedY = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[1];
			CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];
			firstAngle = abs(CalculatedX - CurrentX);
			secondAngle = 360 - firstAngle;
			if (TargetX != 0 && TargetY != 0 && TargetZ != 0 && TargetX < 100000 && TargetY < 100000 && TargetX > -100000 && TargetY > -100000)
			{
				if (firstAngle < 90 || secondAngle < 90/*abs(CalculatedY - CurrentY) < 90 && (abs(CalculatedX - CurrentX) < 90 || abs(CalculatedX - CurrentX) > 270)*/) {
					if (abs(CalculatedX - CurrentX) > 270)
					{
						DrawBorderBox(400 - 7 * (360 - abs(CalculatedX - CurrentX)), 300 - 6.666666*(CurrentY - CalculatedY), 10, 10);
					}
					else {
						DrawBorderBox(400 - 7 * (CalculatedX - CurrentX), 300 - 6.666666*(CurrentY - CalculatedY), 10, 10);
					}
				}
			}
			else {
				//break;
			}
		}
		if (GetKeyState(VK_SHIFT) & 0x8000)
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + EntityList), &entityList, sizeof(DWORD), 0);
			DWORD LastEntity2 = entityList + 0x20/*(the offset)*/;
			/*Every entity has:CoordinateX,CoordinateY,CoordinateZ,AimY,AimX*/
			float bestXangle = 0;
			float bestYangle = 0;
			found = false;
			while (!found) {
				ClosestAngle = 100000;
				for (int i = 0; i < 32; i++) {
					if (!players[i]) {
						continue;
					}
					Entity = LastEntity2 + i * 0x28;

					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x140), &PlayerX, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x0), &TargetX, sizeof(float), 0);
					
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x144), &PlayerY, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x4), &TargetY, sizeof(float), 0);
					
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x148), &PlayerZ, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x8), &TargetZ, sizeof(float), 0);
					
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CurrentY, sizeof(int), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CurrentX, sizeof(int), 0);

					if (abs(PlayerX - TargetX) < 30 && abs(PlayerY - TargetY) < 30)
						continue;

					CalculatedY = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[1];
					CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];
					VectorSize = 0;// sqrt(pow(PlayerX - TargetX, 2) + pow(PlayerY - TargetY, 2) + pow(PlayerZ - TargetZ, 2));
					if ((10 * abs(CalculatedY - CurrentY) + 3 * abs(CalculatedX - CurrentX)) + VectorSize / 3 < ClosestAngle)
					{
						if (TargetX != 0 && TargetY != 0 && TargetZ != 0 && TargetX < 100000 && TargetY < 100000 && TargetX > -100000 && TargetY > -100000) {

							ClosestEntity = Entity;
							ClosestAngle = 10 * abs(CalculatedY - CurrentY) + 3 * abs(CalculatedX - CurrentX) + VectorSize / 3;
							bestXangle = CalculatedX;
							bestYangle = CalculatedY;
							found = true;
						}
						else {
							CalculatedY = bestYangle;
							CalculatedX = bestXangle;
							//break;
						}
					}
					else
					{
						if (!(TargetX != 0 && TargetY != 0 && TargetZ != 0 && TargetX < 100000 && TargetY < 100000 && TargetX > -100000 && TargetY > -100000))
						{
							CalculatedY = bestYangle;
							CalculatedX = bestXangle;
							//break;
						}
					}
				}
			}
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &bestYangle, sizeof(float), 0);
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &bestXangle, sizeof(float), 0);
		}
		if ((GetKeyState(VK_CAPITAL) & 0x0001) != 0)
		{
			if (found) {
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x140), &PlayerX, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x144), &PlayerY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x148), &PlayerZ, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x0), &TargetX, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x4), &TargetY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x8), &TargetZ, sizeof(float), 0);

				CalculatedY = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[1];
				CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CalculatedY, sizeof(float), 0);
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CalculatedX, sizeof(float), 0);
			}
		}
		iterateNum++;
	}
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + lookingAtSome1), &aim, sizeof(DWORD), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 8), &AimingID, sizeof(int), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + doattack), &Attack, sizeof(DWORD), 0);

	int MaxHP = 500;
	int invincibleTrue = 1;//1 is godmode, 2 or 3 is invisible
	SetCursorPos(100, 400);
	int attacking = 5;
	//Sleep(100000);
	bool Lookaway = true;
	double one = 50;
	while (1 == 1)
	{
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(0xFBED360 + 0xFC), &one, sizeof(double), 0);


		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 8), &AimingID, sizeof(int), 0);
		if (Lookaway == false) {
			attacking = 4;
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Attack + 8), &attacking, sizeof(int), 0);
			Lookaway = true;
		}
		if (AimingID > 0)
		{
			attacking = 5;
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Attack + 8), &attacking, sizeof(int), 0);
			Lookaway = false;
		}
		Sleep(10);

		//SetCursorPos(100, 400);
		cout << AimingID << " and it's in address: " << Attack << endl;
		//*printf("We're aiming in: %d and its address is in: %x", AimingID, Attack);
	}
}

int main()
{
	//lf2UnlimitHPandMPandGodmode();
	cs16AimBot();
}



