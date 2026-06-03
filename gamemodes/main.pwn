#define MAX_PLAYERS (100)
#define MIXED_SPELLINGS

#include <open.mp>
#include <a_mysql>
#include <samp_bcrypt>
#include <zcmd>

// YSI
#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>

// Main
#include "includes/colors.inc"
#include "includes/main.pwn"


//SQL DATA
#include "includes/sql/Accounts.pwn"
#include "includes/sql/Weapons.pwn"


// TaiXiu
#include "includes/taixiu/Var.pwn"
#include "includes/taixiu/TD.pwn"
#include "includes/taixiu/System.pwn"

main() {}

public OnGameModeInit()
{
	print("Chuong trinh may chu dang duoc khoi dong, vui long cho doi....");
	MySQL_Init();
	return 1;
}

public OnGameModeExit()
{
	MySQL_Close();
	return 1;
}
