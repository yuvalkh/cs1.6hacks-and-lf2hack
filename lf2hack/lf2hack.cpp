#include "stdafx.h"
#include "Hprocess.h"
#include <iostream>
#include <string>
#include <math.h>
#include <thread>

using namespace std;

HDC HDC_Desktop;
HBRUSH EnemeyBrush;
float ZeroFloat = 0;
CHackProcess fProcess;

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

float* calcAngles(float myX, float hisX, float myY, float hisY, float myZ, float hisZ)
{
	float aimX = hisX - myX;
	float aimY = hisY - myY;
	float aimZ = hisZ - myZ + 2;//5
	float magnitude = sqrt(pow(aimX, 2) + pow(aimY, 2) + pow(aimZ, 2));
	float* angles = new float[2];//0 is x and 1 is y
	angles[0] = atanf(aimY / aimX);
	angles[1] = -asinf(aimZ / magnitude);
	angles[0] = angles[0] * (180 / 3.14159265358979323846) + 0.05;
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

void cs16WallHack()
{
	float Magnitude;
	float PlayerX, PlayerY, PlayerZ, TargetX, TargetY, TargetZ;
	float CurrentY, CurrentX, CalculatedY, CalculatedX;
	float firstAngle, secondAngle;
	int isAlive, isAlive2;
	int isDucking;
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

		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CurrentY, sizeof(int), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CurrentX, sizeof(int), 0);

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
						DrawBorderBox((400 - 4000 / Magnitude) - 7.5 * (360 - abs(CalculatedX - CurrentX)), (300 - 4000 / Magnitude) - 7.5 * (CurrentY - CalculatedY), 10000 / Magnitude, (31000 / Magnitude) / 2);
					}
					else
					{
						DrawBorderBox((400 - 4000 / Magnitude) - 7.5 * (360 - abs(CalculatedX - CurrentX)), (300 - 4000 / Magnitude) - 7.5 * (CurrentY - CalculatedY), 10000 / Magnitude, 31000 / Magnitude);
					}
				}
				else {
					//DrawBorderBox(400 - 8 * (CalculatedX - CurrentX), 300 - 7*(CurrentY - CalculatedY), 10 , 10);
					if (isDucking)
					{
						DrawBorderBox((400 - 4000 / Magnitude) - 7.5 * (CalculatedX - CurrentX), (300 - 4000 / Magnitude) - 7.5 * (CurrentY - CalculatedY), 10000 / Magnitude, (31000 / Magnitude) / 2 );
					}
					else
					{
						DrawBorderBox((400 - 4000 / Magnitude) - 7.5 * (CalculatedX - CurrentX), (300 - 4000 / Magnitude) - 7.5 * (CurrentY - CalculatedY), 10000 / Magnitude, 31000 / Magnitude);

					}
				}
			}
		}
		else {
			//break;
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

		if (AimingID > 0 && MyTeam != EntityTeam)//Terror is 1,CT is 2
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
	DWORD lookingAtSome1 = 0x663CC;//offset of if I'm aiming at some1
	DWORD doattack = 0x5971C;//offset of attack
	DWORD crosshair = 0x190EC;//offset of where I'm aiming
	DWORD playerSpot = 0xF3E1C;//offset of where the player I'm looking at is
	DWORD EntityList = 0x1E6F8;//hw.dll(offset 20)
	DWORD RecoilOffset = 0x10569B0;//hw.dll(this address + 0 is y recoil, this address + 4 is x recoil);

	HDC_Desktop = GetDC(fProcess.__HWNDCss);
	EnemeyBrush = CreateSolidBrush(RGB(255, 0, 0));
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

	float CalculatedX = 0;
	float CalculatedY = 0;
	bool found = false;
	DWORD Entity;
	float VectorSize;
	DWORD LastEntity = 0;
	float ClosestAngle;
	float firstAngle, secondAngle;
	float beforeX, beforeY, beforeZ, beforeAimY, beforeAimX;
	bool* players = new bool[32];
	int* afterXArray = new int[32];
	int* afterYArray = new int[32];
	int* afterZArray = new int[32];
	int* afterAimXArray = new int[32];
	int* afterAimYArray = new int[32];

	DWORD aim2;
	int AimingID;
	bool Lookaway = false;
	int attacking;

	while (1 == 1)
	{

		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + EntityList), &entityList, sizeof(DWORD), 0);
		ClosestAngle = 100000000;
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + crosshair), &aim, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + playerSpot), &spot, sizeof(DWORD), 0);
		ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"client.dll", fProcess.__gameProcess.th32ProcessID) + doattack), &Attack, sizeof(DWORD), 0);

		if (/*(GetKeyState(VK_LBUTTON) & 0x100) != 0*/GetKeyState(VK_SHIFT) & 0x8000)//first option is to enable the lock with mouse click and the second is when pressing shift
		{
			ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + EntityList), &entityList, sizeof(DWORD), 0);
			DWORD LastEntity2 = entityList + 0x20/*(the offset)*/;
			/*Every entity has:CoordinateX,CoordinateY,CoordinateZ,AimY,AimX*/
			float bestXangle = 0;
			float bestYangle = 0;
			float HefreshX;
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

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x0), &CurrentY, sizeof(int), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(aim + 0x4), &CurrentX, sizeof(int), 0);

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
				CalculatedX = AnglesAdd(CalculatedX, 0.025*(130 * -HefreshX / Magnitude));

				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x0), &RecoilInY, sizeof(float), 0);
				ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"hw.dll", fProcess.__gameProcess.th32ProcessID) + RecoilOffset + 0x4), &RecoilInX, sizeof(float), 0);
				CalculatedX = AnglesAdd(CalculatedX, 2 * -RecoilInX);
				CalculatedY = CalculatedY + 1.95 *-RecoilInY;

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
			}
			found = false;
		}
		if (GetKeyState(VK_CAPITAL) != 0)//if Caps Lock is ON, we will ultra lock on the player we were last locking on
		{
			if (found)
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
			}
		}
	}
}

int main()
{
	printf("This hack will continue when you'll run cs1.6\n");
	fProcess.RunProcess("hl.exe", (LPCWSTR)L"Counter-Strike");//we first find the process(Will wait untill it will be opened). 
	printf("cs1.6 found !\naimbot will start\n");
	thread t1(cs16AimBot);
	thread t2(cs16WallHack);
	thread t3(cs16TriggetBot);
	//thread t3(cs16TriggetBot);
	//lf2UnlimitHPandMPandGodmode();
	//cs16AimBot();
	t1.join();
}



