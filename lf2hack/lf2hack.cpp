// lf2hack.cpp : Defines the entry point for the console application.
//
/*
#include "stdafx.h"


int main()
{
    return 0;
}
*/
// LittleFighterHack.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "Hprocess.h"
#include <iostream>
#include <string>
using namespace std;
DWORD Player = 0x58C9C;
DWORD hpOffset = 0x2FC;
DWORD mpOffset = 0x308;
DWORD PlayerState = 0x8;

int main()
{
	CHackProcess fProcess;
	fProcess.RunProcess("lf2.exe",(LPCWSTR)"Little Fighter 2");//we first find the process. 
	DWORD LocalPlayer;
	int previousHP;
	//here we read what is the hp of the character.
	ReadProcessMemory(fProcess.__HandleProcess,(PBYTE*)(fProcess.GetModuleNamePointer((LPSTR)"lf2.exe",fProcess.__gameProcess.th32ProcessID) + Player),&LocalPlayer,sizeof(DWORD),0);
	ReadProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + hpOffset), &previousHP, sizeof(int), 0);
	DWORD hpPointer = LocalPlayer + hpOffset;
	DWORD mpPointer = LocalPlayer + mpOffset;
	int MaxHP = 500;
	int invincibleTrue = 1;//1 is godmode, 2 or 3 is invisible
	while (1 == 1) 
	{
		//we do maxhp and maxmp everytime
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(hpPointer), &MaxHP, sizeof(int), 0);
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(mpPointer), &MaxHP, sizeof(int), 0);
		//We put the state we want the character will be.
		WriteProcessMemory(fProcess.__HandleProcess, (PBYTE*)(LocalPlayer + PlayerState), &invincibleTrue, sizeof(int), 0);
	}
}



