#include "stdafx.h"
#include "Hprocess.h"
#include <iostream>
#include <string>
#include <math.h>
#include <thread>
#include <tuple>
#include <array>

using namespace std;

HDC HDC_Desktop;
HBRUSH EnemeyBrush = CreateSolidBrush(RGB(255, 0, 0));
HBRUSH FriendBrush = CreateSolidBrush(RGB(0, 0, 255));
float ZeroFloat = 0;
CHackProcess fProcess;
bool run = true;

void lf2UnlimitHPandMPandGodmode()
{
	DWORD Player = 0x58C9C;
	DWORD hpOffset = 0x2FC;
	DWORD mpOffset = 0x308;
	DWORD PlayerState = 0x8;

	CHackProcess fProcess;
	std::printf("This hack will continue when you'll run lf2\n");
	fProcess.RunProcess("lf2.exe", (LPCWSTR)"Little Fighter 2");//we first find the process. 
	std::printf("lf2 found !\nGodmode and unlimit hp/mp has been given !");
	DWORD LocalPlayer;
	int previousHP;
	//here we read what is the hp of the character.
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"lf2.exe", fProcess.__gameProcess.th32ProcessID) + Player), &LocalPlayer, sizeof(DWORD), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + hpOffset), &previousHP, sizeof(int), 0);
	int MaxHP = 500;
	int invincibleTrue = 1;//1 is godmode, 2 or 3 is invisible
	while (true)
	{
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"lf2.exe", fProcess.__gameProcess.th32ProcessID) + Player), &LocalPlayer, sizeof(DWORD), 0);
		//we do maxhp and maxmp everytime
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + hpOffset), &MaxHP, sizeof(int), 0);
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + mpOffset), &MaxHP, sizeof(int), 0);
		//We put the state we want the character will be.
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + PlayerState), &invincibleTrue, sizeof(int), 0);
	}
}

float AnglesAdd(float FirstAngle, float SecondAngle)
{
	if (FirstAngle + SecondAngle >= 0 && FirstAngle + SecondAngle < 360)//it means the angles doesn't go through 0
	{
		return FirstAngle + SecondAngle;
	}
	else
	{
		if (FirstAngle + SecondAngle < 0)
		{
			return FirstAngle + SecondAngle + 360;
		}
		else
		{
			if (FirstAngle + SecondAngle >= 360)
			{
				return FirstAngle + SecondAngle - 360;
			}
		}
	}
}

std::array<float, 2> calcAngles(float myX, float hisX, float myY, float hisY, float myZ, float hisZ)
{
	float aimX = hisX - myX;
	float aimY = hisY - myY;
	float aimZ = hisZ - myZ;//5
	float magnitude = sqrt(pow(aimX, 2) + pow(aimY, 2) + pow(aimZ, 2));
	std::array<float, 2> angles;
	//float* angles = new float[2];//0 is x and 1 is y
	angles[0] = atanf(aimY / aimX);
	angles[1] = -asinf(aimZ / magnitude);
	angles[0] = angles[0] * (180 / 3.14159265358979323846);
	angles[1] = angles[1] * (180 / 3.14159265358979323846);

	if (aimX < 0) {
		angles[0] += 180;
	}
	if (angles[0] < 0)
	{
		angles[0] += 360;
	}
	return angles;
}

void DrawFilledRect(int x, int y, int w, int h, bool enemey)
{
	RECT rect = { x,y,x + w,y + h };
	if (enemey) {//so we're using a red brush
		FillRect(HDC_Desktop, &rect, EnemeyBrush);
	}
	else {
		FillRect(HDC_Desktop, &rect, FriendBrush);
	}
}

void DrawBorderBox(int x, int y, int w, int h, bool enemey)
{
	DrawFilledRect(x, y, w, 2, enemey);
	DrawFilledRect(x, y, 2, h, enemey);
	DrawFilledRect(x + w, y, 2, h, enemey);
	DrawFilledRect(x, y + h, w + 2, 2, enemey);

}

void cs16WallHack()
{
	float Magnitude;
	float PlayerX, PlayerY, PlayerZ, TargetX, TargetY, TargetZ;
	float CurrentY, CurrentX, CalculatedY, CalculatedX;
	float firstAngle, secondAngle;
	int isAlive, isAlive2;
	int isDucking;
	HDC_Desktop = GetDC(fProcess.__HWNDCss);
	EnemeyBrush = CreateSolidBrush(RGB(255, 0, 0));
	DWORD aim;
	DWORD crosshair = 0x190EC;//offset of where I'm aiming
	DWORD entityList;
	DWORD EntityList = 0x1E6F8;//hw.dll(offset 20)
	DWORD Entity;
	DWORD spot;
	DWORD playerSpot = 0xF3E1C;//offset of where the player I'm looking at is
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + playerSpot), &spot, sizeof(DWORD), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + crosshair), &aim, sizeof(DWORD), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + EntityList), &entityList, sizeof(DWORD), 0);
	DWORD LastEntity = entityList + 0x20;
	while (true)
	{
		for (int i = 0; i < 32; i++)
		{
			Entity = LastEntity + i * 0x28;
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity - 0xC), &isAlive, sizeof(int), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity - 0x4), &isAlive2, sizeof(int), 0);

			if (isAlive != 3 || isAlive2 != 1)
			{
				continue;
			}
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity - 0x8), &isDucking, sizeof(int), 0);

			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x140), &PlayerX, sizeof(float), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x144), &PlayerY, sizeof(float), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x148), &PlayerZ, sizeof(float), 0);

			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x0), &TargetX, sizeof(float), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x4), &TargetY, sizeof(float), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x8), &TargetZ, sizeof(float), 0);

			if (abs(PlayerX - TargetX) < 25 && abs(PlayerY - TargetY) < 25)
				continue;

			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CurrentY, sizeof(float), 0);
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CurrentX, sizeof(float), 0);

			CalculatedY = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[1];
			CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];
			Magnitude = sqrt(pow(PlayerX - TargetX, 2) + pow(PlayerY - TargetY, 2) + pow(PlayerZ - TargetZ, 2));
			firstAngle = abs(CalculatedX - CurrentX);
			secondAngle = 360 - firstAngle;
			if (TargetX != 0 && TargetY != 0 && TargetZ != 0 && TargetX < 100000 && TargetY < 100000 && TargetX > -100000 && TargetY > -100000)
			{
				if (abs(CalculatedY - CurrentY) && (firstAngle < 90 || secondAngle < 90)/*abs(CalculatedY - CurrentY) < 90 && (abs(CalculatedX - CurrentX) < 90 || abs(CalculatedX - CurrentX) > 270)*/) {
					if (abs(CalculatedX - CurrentX) > 270)
					{
						//DrawBorderBox(400 - 8 * (360 - abs(CalculatedX - CurrentX)), 300 - 7*(CurrentY - CalculatedY), 10,10);
						if (isDucking)
						{
							DrawBorderBox((400 - 4000 / Magnitude) - 7.5 * (360 - abs(CalculatedX - CurrentX)), (300 - 4000 / Magnitude) - 7.5 * (CurrentY - CalculatedY), 10000 / Magnitude, (31000 / Magnitude) / 2, true);
						}
						else
						{
							if (CalculatedX - CurrentX > 270)
							{
								DrawBorderBox((400 - 4000 / Magnitude) + 7.5 * (360 - abs(CalculatedX - CurrentX)), (300 - 4000 / Magnitude) - 7.5 * (CurrentY - CalculatedY), 10000 / Magnitude, 31000 / Magnitude, true);
							}
							else
							{
								DrawBorderBox((400 - 4000 / Magnitude) - 7.5 * (360 - abs(CalculatedX - CurrentX)), (300 - 4000 / Magnitude) - 7.5 * (CurrentY - CalculatedY), 10000 / Magnitude, 31000 / Magnitude, true);
							}
						}
					}
					else {
						//DrawBorderBox(400 - 8 * (CalculatedX - CurrentX), 300 - 7*(CurrentY - CalculatedY), 10 , 10);
						if (isDucking)
						{
							DrawBorderBox((400 - 4000 / Magnitude) - 7.5 * (CalculatedX - CurrentX), (300 - 4000 / Magnitude) - 7.5 * (CurrentY - CalculatedY), 10000 / Magnitude, (31000 / Magnitude) / 2, true);
						}
						else
						{
							DrawBorderBox((400 - 4000 / Magnitude) - 7.5 * (CalculatedX - CurrentX), (300 - 4000 / Magnitude) - 7.5 * (CurrentY - CalculatedY), 10000 / Magnitude, 31000 / Magnitude, true);
						}
					}
				}
			}
			else {
				//break;
			}
		}
	}
}

void cs16TriggetBot()
{
	DWORD aim2;
	DWORD MyTeamOffset = 0xFC94C;
	DWORD lookingAtSome1 = 0x663CC;//offset of if I'm aiming at some1
	int AimingID;
	int EntityTeam;
	int MyTeam;//(1 is T,2 is CT)
	bool time1 = true;
	int attacking = 4;
	DWORD doattack = 0x5971C;//offset of attack
	DWORD Attack;
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + lookingAtSome1), &aim2, sizeof(DWORD), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + doattack), &Attack, sizeof(DWORD), 0);

	while (true)
	{
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + MyTeamOffset), &MyTeam, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim2 + 0x8), &AimingID, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim2 + 0x4), &EntityTeam, sizeof(int), 0);

		if (AimingID > 0 && MyTeam != EntityTeam && MyTeam != 0)//Terror is 1,CT is 2
		{
			attacking = 5;
			WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Attack + 8), &attacking, sizeof(int), 0);
			time1 = false;
		}
		else
		{
			if (!time1)
			{
				attacking = 4;
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Attack + 8), &attacking, sizeof(int), 0);
				time1 = true;
			}
		}
	}
}

void cs16AimBot()
{
	DWORD lookingAtSome1 = 0x663CC;     //offset of if I'm aiming at some1
	DWORD doattack = 0x5971C;           //offset of attack
	DWORD crosshair = 0x190EC;          //offset of where I'm aiming
	DWORD playerSpot = 0xF3E1C;         //offset of where the player I'm looking at is
	DWORD EntityList = 0x1E6F8;         //hw.dll(offset 20)
	DWORD RecoilOffset = 0x10569B0;     //hw.dll(this address + 0 is y recoil, this address + 4 is x recoil);


	//fProcess.PrintModules(fProcess.__gameProcess.th32ProcessID);
	DWORD Attack;
	DWORD entityList;
	DWORD aim;
	DWORD spot;
	DWORD ClosestEntity = NULL;

	float PlayerX, PlayerY, PlayerZ;
	float TargetX, TargetY, TargetZ;
	float TargetAimX, TargetAimY;

	float CurrentX, CurrentY;

	float RecoilInY, RecoilInX;

	int isAlive, isAlive2;
	float CalculatedX = 0;
	float CalculatedY = 0;
	bool found = false;
	bool locked = false;
	DWORD Entity;
	float VectorSize;
	DWORD LastEntity = 0;
	float ClosestAngle;
	float firstAngle, secondAngle;
	float beforeX, beforeY, beforeZ, beforeAimY, beforeAimX;
	std::array<bool, 32> players;
	std::array<float, 32> afterXArray;
	std::array<float, 32> afterYArray;
	std::array<float, 32> afterZArray;
	std::array<float, 32> afterAimXArray;
	std::array<float, 32> afterAimYArray;
	//bool* players = new bool[32];
	//int* afterXArray = new int[32];
	//int* afterYArray = new int[32];
	//int* afterZArray = new int[32];
	//int* afterAimXArray = new int[32];
	//int* afterAimYArray = new int[32];

	DWORD aim2;
	int AimingID;
	bool Lookaway = false;
	int attacking;
	float HefreshX;

	while (1 == 1)
	{

		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + EntityList), &entityList, sizeof(DWORD), 0);
		ClosestAngle = 100000000;
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + crosshair), &aim, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + playerSpot), &spot, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + doattack), &Attack, sizeof(DWORD), 0);


		if (/*(GetKeyState(VK_LBUTTON) & 0x100) != 0*/GetKeyState(VK_SHIFT) & 0x8000)//first option is to enable the lock with mouse click and the second is when pressing shift
		{
			if (locked)
			{
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x140), &PlayerX, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x144), &PlayerY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x148), &PlayerZ, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x0), &TargetX, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x4), &TargetY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x8), &TargetZ, sizeof(float), 0);



				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0xC), &TargetAimY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x10), &TargetAimX, sizeof(float), 0);
				if (TargetAimX < 0)
				{
					TargetAimX += 360;
				}

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CurrentY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CurrentX, sizeof(float), 0);

				CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];//the base angle
				HefreshX = AnglesAdd(TargetAimX, -CalculatedX);
				HefreshX = 360 - HefreshX;

				if (abs(PlayerX - TargetX) < 25 && abs(PlayerY - TargetY) < 25)
					continue;

				if (HefreshX > 0 && HefreshX < 180)//his head is right from us 
				{
					if (HefreshX > 90)
					{
						HefreshX = 180 - HefreshX;
					}
				}
				else //his head is left from us
				{
					if (HefreshX > 270)
					{
						HefreshX = HefreshX - 360;
					}
					else
					{
						HefreshX = 180 - HefreshX;
					}
				}
				float Magnitude = sqrt(pow(PlayerX - TargetX, 2) + pow(PlayerY - TargetY, 2) + pow(PlayerZ - TargetZ, 2));


				CalculatedY = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[1];
				CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];
				CalculatedX = AnglesAdd(CalculatedX, 0.025 * (130 * -HefreshX / Magnitude));

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x0), &RecoilInY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x4), &RecoilInX, sizeof(float), 0);
				CalculatedX = AnglesAdd(CalculatedX, 2 * -RecoilInX);
				CalculatedY = CalculatedY + 1.95 * -RecoilInY;


				/*CalculatedY = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[1];
				CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x0), &RecoilInY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x4), &RecoilInX, sizeof(float), 0);
				CalculatedX = AnglesAdd(CalculatedX, 2 * -RecoilInX);
				CalculatedY = CalculatedY + 2 * -RecoilInY;*/
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CalculatedY, sizeof(float), 0);
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CalculatedX, sizeof(float), 0);

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity - 0xC), &isAlive, sizeof(int), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity - 0x4), &isAlive2, sizeof(int), 0);

				if (isAlive != 3 || isAlive2 != 1)//means that the man we're looking for is dead
				{
					locked = false;
				}
				continue;
			}
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + EntityList), &entityList, sizeof(DWORD), 0);
			DWORD LastEntity2 = entityList + 0x20/*(the offset)*/;
			/*Every entity has:CoordinateX,CoordinateY,CoordinateZ,AimY,AimX*/
			float bestXangle = 0;
			float bestYangle = 0;
			int isCTorT;//0 is CT, 1 is T
			found = false;
			int isAlive, isAlive2;
			ClosestAngle = 100000;
			for (int i = 0; i < 32; i++)
			{
				Entity = LastEntity2 + i * 0x28;

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity - 0xC), &isAlive, sizeof(int), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity - 0x4), &isAlive2, sizeof(int), 0);

				if (isAlive != 3 || isAlive2 != 1)
				{
					continue;
				}

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x140), &PlayerX, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x0), &TargetX, sizeof(float), 0);

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x144), &PlayerY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x4), &TargetY, sizeof(float), 0);

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x148), &PlayerZ, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x8), &TargetZ, sizeof(float), 0);

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0xC), &TargetAimY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity + 0x10), &TargetAimX, sizeof(float), 0);
				if (TargetAimX < 0)
				{
					TargetAimX += 360;
				}

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CurrentY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CurrentX, sizeof(float), 0);

				CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];//the base angle
				HefreshX = AnglesAdd(TargetAimX, -CalculatedX);
				HefreshX = 360 - HefreshX;

				if (abs(PlayerX - TargetX) < 25 && abs(PlayerY - TargetY) < 25)
					continue;

				if (HefreshX > 0 && HefreshX < 180)//his head is right from us 
				{
					if (HefreshX > 90)
					{
						HefreshX = 180 - HefreshX;
					}
				}
				else //his head is left from us
				{
					if (HefreshX > 270)
					{
						HefreshX = HefreshX - 360;
					}
					else
					{
						HefreshX = 180 - HefreshX;
					}
				}
				float Magnitude = sqrt(pow(PlayerX - TargetX, 2) + pow(PlayerY - TargetY, 2) + pow(PlayerZ - TargetZ, 2));


				CalculatedY = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[1];
				CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];
				CalculatedX = AnglesAdd(CalculatedX, 0.025 * (130 * -HefreshX / Magnitude));

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x0), &RecoilInY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x4), &RecoilInX, sizeof(float), 0);
				CalculatedX = AnglesAdd(CalculatedX, 2 * -RecoilInX);
				CalculatedY = CalculatedY + 1.95 * -RecoilInY;

				float xAngleCheck = min(abs(CalculatedX - CurrentX), 360 - abs(CalculatedX - CurrentX));
				VectorSize = 0;// sqrt(pow(PlayerX - TargetX, 2) + pow(PlayerY - TargetY, 2) + pow(PlayerZ - TargetZ, 2));We use 0 now because it doesn't matter if he's close or not. uncomment this line in order to give advantage on focusing on close players.
				if ((10 * abs(CalculatedY - CurrentY) + 3 * xAngleCheck) + VectorSize / 3 < ClosestAngle)
				{
					if (TargetX != 0 && TargetY != 0 && TargetZ != 0 && TargetX < 100000 && TargetY < 100000 && TargetX > -100000 && TargetY > -100000) {

						ClosestEntity = Entity;
						ClosestAngle = 10 * abs(CalculatedY - CurrentY) + 3 * xAngleCheck + VectorSize / 3;
						bestXangle = CalculatedX;
						bestYangle = CalculatedY;
						ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(Entity - 0x4), &isCTorT, sizeof(int), 0);
						found = true;
					}
					else {
						CalculatedY = bestYangle;
						CalculatedX = bestXangle;
					}
				}
				else
				{
					if (!(TargetX != 0 && TargetY != 0 && TargetZ != 0 && TargetX < 100000 && TargetY < 100000 && TargetX > -100000 && TargetY > -100000))
					{
						CalculatedY = bestYangle;
						CalculatedX = bestXangle;
					}
				}
			}
			if (found)
			{
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &bestYangle, sizeof(float), 0);
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &bestXangle, sizeof(float), 0);
				locked = true;
			}
			found = false;
		}
		else
		{
			locked = false;
		}
		if (GetKeyState(VK_CAPITAL) != 0)//if Caps Lock is ON, we will ultra lock on the player we were last locking on
		{
			if (locked)
			{
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x140), &PlayerX, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x144), &PlayerY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(spot + 0x148), &PlayerZ, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x0), &TargetX, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x4), &TargetY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity + 0x8), &TargetZ, sizeof(float), 0);

				CalculatedY = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[1];
				CalculatedX = calcAngles(PlayerX, TargetX, PlayerY, TargetY, PlayerZ, TargetZ)[0];
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x0), &RecoilInY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x4), &RecoilInX, sizeof(float), 0);
				CalculatedX = AnglesAdd(CalculatedX, 2 * -RecoilInX);
				CalculatedY = CalculatedY + 2 * -RecoilInY;
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CalculatedY, sizeof(float), 0);
				WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CalculatedX, sizeof(float), 0);

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity - 0xC), &isAlive, sizeof(int), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(ClosestEntity - 0x4), &isAlive2, sizeof(int), 0);

				if (isAlive != 3 || isAlive2 != 1)//means that the man we're looking for is dead
				{
					locked = false;
				}
			}
		}
	}
}

void csgoAimBot()
{
	DWORD DormantOffset = 0xED;
	DWORD boneMatrixOffset = 0x26A8;
	DWORD LocalPlayerOffset = 0xD8B2DC;
	DWORD ClientState = 0x58EFE4;
	DWORD ViewAngles = 0x4D90;
	DWORD EntityListOffset = 0x4DA2F24;
	DWORD MyLocationOffset = 0x3C804;
	DWORD RecoilOffsetY = 0x3020;
	DWORD myTeamOffset = 0xF4;
	DWORD myLocalPlayerLocation;
	DWORD temp;
	DWORD RecoilPointer;
	DWORD boneMatrixPointer;
	DWORD EntityList;
	int myTeam, entityTeam;
	DWORD myTeamPointer;
	float myXangle, myYangle;
	float myXposition, myYposition, myZposition;
	float enemeyX, enemeyY, enemeyZ;
	float previousEnemeyZ;
	int entityHP;
	float isCrouching;
	float RecoilX, RecoilY;
	float penaltyX, penaltyY;
	std:array<float, 2> CalculatedAngles;
	DWORD engineDll = fProcess.GetModuleNamePointer((LPSTR)"engine.dll", fProcess.__gameProcess.th32ProcessID);
	DWORD clientDll = fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID);
	DWORD serverDll = fProcess.GetModuleNamePointer((LPSTR)"server.dll", fProcess.__gameProcess.th32ProcessID);
	DWORD entityListPointer = clientDll + EntityListOffset + 0x20;
	while (true) {
		DWORD LocalPlayer;
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(clientDll + LocalPlayerOffset), &LocalPlayer, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(engineDll + MyLocationOffset), &myLocalPlayerLocation, sizeof(DWORD), 0);
		Sleep(1000);
		int targetBone = 8;
		byte isDormant;
		while (run) {
			Sleep(10);
			if ((/*(GetKeyState(VK_LBUTTON) & 0x100) != 0 ||*/ /*GetAsyncKeyState(VK_SHIFT)*/GetAsyncKeyState(VK_CAPITAL))) {
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(engineDll + ClientState), &temp, sizeof(DWORD), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles), &myYangle, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles + 0x4), &myXangle, sizeof(float), 0);
				if (myXangle < 0) {
					myXangle += 360;
				}
				int closestEntityNumber = 0;
				float closestDistance = 10000000;
				for (int i = -1; i < 32; i++) {//looking here who's the closest
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entityListPointer + (i * 16)), &EntityList, sizeof(DWORD), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + 0x100), &entityHP, sizeof(int), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + 0xF4), &entityTeam, sizeof(int), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + myTeamOffset), &myTeam, sizeof(int), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + DormantOffset), &isDormant, sizeof(byte), 0);

					

					if (EntityList == 0 || entityHP <= 0 || myTeam == entityTeam || isDormant)
					{
						continue;
					}
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + boneMatrixOffset), &boneMatrixPointer, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(boneMatrixPointer + 0x30 * targetBone + 0x0C), &enemeyX, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(boneMatrixPointer + 0x30 * targetBone + 0x1C), &enemeyY, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(boneMatrixPointer + 0x30 * targetBone + 0x2C), &enemeyZ, sizeof(float), 0);

					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation), &myXposition, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation + 0x4), &myYposition, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation + 0x8), &myZposition, sizeof(float), 0);

					CalculatedAngles = calcAngles(myXposition, enemeyX, myYposition, enemeyY, myZposition, enemeyZ);
					if (myXangle < 0) {
						myXangle += 360;
					}
					if (myXangle > 360) {
						myXangle -= 360;
					}
					float currentDistance = sqrt(pow(myXangle - CalculatedAngles[0], 2) + pow(myYangle - CalculatedAngles[1], 2));
					if (currentDistance < closestDistance) {
						closestDistance = currentDistance;
						closestEntityNumber = i;
					}
				}
				while ((/*(GetKeyState(VK_LBUTTON) & 0x100) != 0 ||*/ /*GetAsyncKeyState(VK_SHIFT)*/GetAsyncKeyState(VK_CAPITAL)) && closestDistance != 10000000) {//if we continue clicking shift it means we want to lock on him
					float enemeyXangle;
					float enemeyXspeed, enemeyYspeed;
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entityListPointer + (closestEntityNumber * 16)), &EntityList, sizeof(DWORD), 0);

					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + DormantOffset), &isDormant, sizeof(byte), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + boneMatrixOffset), &boneMatrixPointer, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(boneMatrixPointer + 0x30 * targetBone + 0x0C), &enemeyX, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(boneMatrixPointer + 0x30 * targetBone + 0x1C), &enemeyY, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(boneMatrixPointer + 0x30 * targetBone + 0x2C), &enemeyZ, sizeof(float), 0);

					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + 0x114), &enemeyXspeed, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + 0x114 + 0x4), &enemeyYspeed, sizeof(float), 0);
					enemeyX = enemeyX + (enemeyXspeed / 750.f);
					enemeyY = enemeyY + (enemeyYspeed / 750.f);

					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + 0xB380), &enemeyXangle, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles + 0x4), &myXangle, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles), &myYangle, sizeof(float), 0);

					if (enemeyXangle < 0) {
						enemeyXangle += 360;
					}
					if (myXangle < 0) {
						myXangle += 360;
					}
					if (myXangle > 360) {
						myXangle -= 360;
					}
					float addToXDegree = 0;
					float magnitude = sqrt(pow(myXposition - enemeyX, 2) + pow(myYposition - enemeyY, 2) + pow(myZposition - enemeyZ, 2));
					//int enemeyAngleBeyahasElai = 360 - ((int)(enemeyXangle + (360 - myXangle)) % 360);
					float enemeyAngleBeyahasElai = 360.0 - fmod((enemeyXangle + (360.0 - myXangle)), 360.0);

					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation), &myXposition, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation + 0x4), &myYposition, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation + 0x8), &myZposition, sizeof(float), 0);
					CalculatedAngles = calcAngles(myXposition, enemeyX, myYposition, enemeyY, myZposition, enemeyZ);
					//CalculatedAngles[0] += addToXDegree;

					float calculatedAngleXBeyahasElai = 360.0 - fmod((CalculatedAngles[0] + (360.0 - myXangle)), 360.0);

					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + 0x100), &entityHP, sizeof(int), 0);

					if (entityHP <= 0 || isDormant) {//means he's dead
						Sleep(600);
						break;
					}
					//Now we handle the Recoil
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + RecoilOffsetY + 0xC), &RecoilY, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + RecoilOffsetY + 0x10), &RecoilX, sizeof(float), 0);
					CalculatedAngles[0] -= 2 * RecoilX;
					CalculatedAngles[1] -= 2 * RecoilY;

					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + RecoilOffsetY + 0xC), &RecoilY, sizeof(float), 0);
					ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + RecoilOffsetY + 0x10), &RecoilX, sizeof(float), 0);

					//we change only if we get good angles(if we get something strange we ignore it)
					if (!isnan(CalculatedAngles[1]) && !isnan(CalculatedAngles[0]) && CalculatedAngles[1] < 89 && CalculatedAngles[1] > -89 && CalculatedAngles[0] >= 0 && CalculatedAngles[0] < 360)
					{
						//This is against VAC that's finding movement that is suspicious(or whatever you write that word)
						if (abs(CalculatedAngles[1] - myYangle) > 0.5) {
							if (CalculatedAngles[1] > myYangle) {
								CalculatedAngles[1] = myYangle + 0.5;
							}
							else {
								CalculatedAngles[1] = myYangle - 0.5;
							}
						}
						if (calculatedAngleXBeyahasElai > 0.5 && calculatedAngleXBeyahasElai < 359.5) {
							if (calculatedAngleXBeyahasElai < 180) {
								CalculatedAngles[0] = myXangle - 0.5;
							}
							else {
								CalculatedAngles[0] = myXangle + 0.5;
							}
						}
					}
					//Against Vac as well(aka clamping angles)
					if (CalculatedAngles[1] > 89.0) {
						CalculatedAngles[1] = 89.0;
					}
					if (CalculatedAngles[1] < -89.0) {
						CalculatedAngles[1] = -89.0;
					}
					if (CalculatedAngles[0] < 180) {
						CalculatedAngles[0] += 360;
					}
					if (CalculatedAngles[0] >= 180) {
						CalculatedAngles[0] -= 360;
					}
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles), &CalculatedAngles[1], sizeof(float), 0);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles + 0x4), &CalculatedAngles[0], sizeof(float), 0);
				}
			}
		}
	}
}

void csgoWallHack()
{
	DWORD DormantOffset = 0xED;
	DWORD glowManagerOffset = 0x52EB518;
	DWORD glowIndexOffset = 0xA438;
	DWORD boneMatrixOffset = 0x26A8;
	DWORD isScopedOffset = 0x3928;
	DWORD LocalPlayerOffset = 0xD8B2DC;
	DWORD myTeamOffset = 0xF4;
	DWORD ClientState = 0x58EFE4;
	DWORD ViewAngles = 0x4D90;
	DWORD EntityListOffset = 0x4DA2F24;
	DWORD MyLocationOffset = 0x3C754;
	DWORD GameWidthOffset = 0x283398;
	DWORD myLocalPlayerLocation;
	DWORD glowManagerPointer;
	DWORD glowIndexPointer;
	DWORD temp;
	DWORD myTeamPointer;
	float Magnitude;
	int gameWidth;
	int gameHeight;
	float isCrouching;
	float myXangle, myYangle;
	float myXposition, myYposition, myZposition;
	float enemeyX, enemeyY, enemeyZ;
	int isScoped;
	std:array<float, 2> CalculatedAngles;
	HDC_Desktop = GetDC(fProcess.__HWNDCss);
	DWORD aim;
	DWORD EntityList;
	DWORD engineDll = fProcess.GetModuleNamePointer((LPSTR)"engine.dll", fProcess.__gameProcess.th32ProcessID);
	DWORD clientDll = fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID);
	DWORD serverDll = fProcess.GetModuleNamePointer((LPSTR)"server.dll", fProcess.__gameProcess.th32ProcessID);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(engineDll + GameWidthOffset), &temp, sizeof(DWORD), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + 0xC), &gameWidth, sizeof(int), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + 0xC + 0x4), &gameHeight, sizeof(int), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(engineDll + MyLocationOffset), &myLocalPlayerLocation, sizeof(DWORD), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation), &myXposition, sizeof(float), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation + 0x4), &myYposition, sizeof(float), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation + 0x8), &myZposition, sizeof(float), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"engine.dll", fProcess.__gameProcess.th32ProcessID) + ClientState), &temp, sizeof(DWORD), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles), &myYangle, sizeof(float), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles + 0x4), &myXangle, sizeof(float), 0);
	DWORD entityListPointer = clientDll + EntityListOffset + 0x20;
	DWORD LocalPlayer;
	DWORD boneMatrixPointer;
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(clientDll + LocalPlayerOffset), &LocalPlayer, sizeof(int), 0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(clientDll + glowManagerOffset), &glowManagerPointer, sizeof(int), 0);
	int entityTeam;//3 is CT, 2 is T
	int myTeam;
	int entityHP;
	int targetBone = 8;//This number is for the head
	byte isDormant;//means if I can't get his position
	while (true) {
		Sleep(1000);
		while (run)
		{
			Sleep(20);
			for (int i = -1; i < 32; i++) {
				//Getting here the pointer to a specific entity and getting his hp straight away
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(entityListPointer + (i * 16)), &EntityList, sizeof(DWORD), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + 0x100), &entityHP, sizeof(int), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + DormantOffset), &isDormant, sizeof(byte), 0);

				if (entityHP <= 0 || EntityList == 0/* || isDormant*/)
				{
					continue;
				}
				//test for glow hack
				float oneInFloat = 1;
				float zeroInFloat = 0;
				int oneInInt = 1;
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + myTeamOffset), &myTeam, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + 0xF4), &entityTeam, sizeof(int), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + glowIndexOffset), &glowIndexPointer, sizeof(int), 0);
				if (myTeam == entityTeam) {
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0x4), &zeroInFloat, sizeof(float), 0);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0x8), &zeroInFloat, sizeof(float), 0);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0xC), &oneInFloat, sizeof(float), 0);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0x10), &oneInFloat, sizeof(float), 0);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0x24), &oneInInt, sizeof(int), 0);
				}
				else {
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0x4), &oneInFloat, sizeof(float), 0);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0x8), &zeroInFloat, sizeof(float), 0);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0xC), &zeroInFloat, sizeof(float), 0);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0x10), &oneInFloat, sizeof(float), 0);
					WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(glowManagerPointer + glowIndexPointer * 0x38 + 0x24), &oneInInt, sizeof(int), 0);
				}

				//this is my previous hack where I draw squares
				/*





				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + isScopedOffset), &isScoped, sizeof(float), 0);

				//Getting here the position of the player
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation), &myXposition, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation + 0x4), &myYposition, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(myLocalPlayerLocation + 0x8), &myZposition, sizeof(float), 0);
				//Getting here the position of the enemey
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + boneMatrixOffset), &boneMatrixPointer, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(boneMatrixPointer + 0x30 * targetBone + 0x0C), &enemeyX, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(boneMatrixPointer + 0x30 * targetBone + 0x1C), &enemeyY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(boneMatrixPointer + 0x30 * targetBone + 0x2C), &enemeyZ, sizeof(float), 0);
				//Getting here if the entity is crouching
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(EntityList + 0x378), &isCrouching, sizeof(float), 0);

				if (abs(myXposition - enemeyX) < 35 && abs(myYposition - enemeyY) < 35 && abs(myZposition - enemeyZ) < 35) {
					continue;
				}

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles), &myYangle, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(temp + ViewAngles + 0x4), &myXangle, sizeof(float), 0);


				CalculatedAngles = calcAngles(myXposition, enemeyX, myYposition, enemeyY, myZposition, enemeyZ);

				//int calculatedAngleXBeyahasElai = 360 - ((int)(CalculatedAngles[0] + (360 - myXangle)) % 360);
				float calculatedAngleXBeyahasElai = 360.0 - fmod((CalculatedAngles[0] + (360.0 - myXangle)), 360.0);
				float calculatedAngleYBeyahasElai = myYangle - CalculatedAngles[1];
				if (calculatedAngleXBeyahasElai > 315) {
					calculatedAngleXBeyahasElai -= 360;
				}


				Magnitude = sqrt(pow(myXposition - enemeyX, 2) + pow(myYposition - enemeyY, 2) + pow(myZposition - enemeyZ, 2));
				if (enemeyX != 0 && enemeyY != 0 && enemeyZ != 0 && enemeyX < 100000 && enemeyY < 100000 && enemeyX > -100000 && enemeyY > -100000)
				{
					if ((calculatedAngleXBeyahasElai > -45 && calculatedAngleXBeyahasElai < 45) && (calculatedAngleYBeyahasElai < 45 && calculatedAngleYBeyahasElai > -45))
					{
						float boxStartX;
						if (calculatedAngleXBeyahasElai != 0) {
							boxStartX = (gameWidth / 2 - 10 * gameWidth / Magnitude) + gameWidth / 100 * calculatedAngleXBeyahasElai;
						}
						else {
							boxStartX = (gameWidth / 2 - 10 * gameWidth / Magnitude);
						}
						float boxStartY = (gameHeight / 2 - 4 * gameWidth / Magnitude) - gameHeight / 65 * calculatedAngleYBeyahasElai;
						float boxWidth = 10000 / Magnitude;
						float boxHeight = 31500 / Magnitude;
						if (isCrouching < 60) {//The player is crouching so need to draw a smaller rectangle
							if (myTeam == entityTeam) {
								DrawBorderBox(boxStartX, boxStartY, boxWidth, boxHeight / 2, false);
							}
							else {
								DrawBorderBox(boxStartX, boxStartY, boxWidth, boxHeight / 2, true);
							}
						}
						else {
							if (myTeam == entityTeam) {
								DrawBorderBox(boxStartX, boxStartY, boxWidth, boxHeight, false);
							}
							else {
								DrawBorderBox(boxStartX, boxStartY, boxWidth, boxHeight, true);
							}
						}
						//std::printf("I'm drawing at: \nstartX: %f \nstartY: %f \nwidth: %f \nheight: %f \ncalculatedX: %f \ncalculatedY: %f \nmagnitude: %f", boxStartX, boxStartY, boxWidth, boxHeight, calculatedAngleXBeyahasElai, calculatedAngleYBeyahasElai);
					}
				}*/
			}
			//std::printf("\n\n\n");
		}
	}
}

void csgoTriggerBot() {
	DWORD LocalPlayerOffset = 0xD3FC5C;
	DWORD LocalPlayer;
	DWORD CrosshairID = 0xB3E4;
	DWORD clientDll = fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID);
	int aimingAt = 0;
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(clientDll + LocalPlayerOffset), &LocalPlayer, sizeof(DWORD), 0);
	while (true) {
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + CrosshairID), &aimingAt, sizeof(int), 0);
		if (aimingAt > 0) {
			printf("I'm aiming at something1\n");
			printf("I'm aiming at something2\n");
			printf("I'm aiming at something3\n");
			printf("I'm aiming at something4\n");
			PostMessage(fProcess.__HWNDCss, WM_LBUTTONDOWN, 0, MAKELPARAM(10, 10));
		}
	}
}

int main()
{
	/*printf("This hack will continue when you'll run cs1.6\n");
	fProcess.RunProcess("hl.exe", (LPCWSTR)L"Counter-Strike");//we first find the process(Will wait untill it will be opened).
	printf("cs1.6 found !\naimbot will start\n");
	thread t1(cs16AimBot);
	thread t2(cs16WallHack);
	//thread t3(cs16TriggetBot);
	//lf2UnlimitHPandMPandGodmode();
	//cs16AimBot();
	t2.join();*/

	fProcess.RunProcess("csgo.exe", (LPCWSTR)L"Counter-Strike: Global Offensive");
	thread t1(csgoWallHack);
	thread t2(csgoAimBot);
	while (true) {
		if (GetKeyState(VK_INSERT) & 0x8000/*Check if high-order bit is set (1 << 15)*/)
		{
			run = !run;
			std::cout << run << std::endl;
			Sleep(500);
		}
	}
	/*while (true) {
		csgoAimBot();
	}*/
	//thread t3(csgoTriggerBot);
	//t1.join();

}



