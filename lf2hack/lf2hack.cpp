#include "stdafx.h"
#include "Hprocess.h"
#include <iostream>
#include <string>
using namespace std;


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

int main()
{
	lf2UnlimitHPandMPandGodmode();
}



