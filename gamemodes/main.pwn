#include <open.mp>
#include <a_mysql>
#include <samp_bcrypt>
#include <zcmd>

// YSI
#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>

// Main
#include "includes/modules/colors.inc"
#include "includes/modules/defines.inc"
#include "includes/modules/dialogs.inc"
#include "includes/modules/vars.inc"
#include "includes/modules/sql.inc"
#include "includes/modules/funcs.inc"
#include "includes/modules/task.inc"
#include "includes/modules/ad_cmds.inc"
#include "includes/modules/g_cmds.inc"
#include "includes/main.pwn"

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
