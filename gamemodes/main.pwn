#include <open.mp>
#include <a_mysql>
//#include <sscanf2>
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
#include "includes/modules/notify.inc"
#include "includes/modules/sql.inc"
#include "includes/modules/funcs.inc"
#include "includes/modules/task.inc"
#include "includes/modules/cmds.inc"
#include "includes/main.pwn"

//Conversations
//#include "includes/conversations/messager.inc"

// TaiXiu
#include "includes/taixiu/Var.pwn"
#include "includes/taixiu/TD.pwn"
#include "includes/taixiu/System.pwn"

main() {}

public OnGameModeInit()
{
	print("Chuong trinh may chu dang duoc khoi dong, vui long cho doi....");
	print("");
	MySQL_Init();
	return 1;
}

public OnGameModeExit()
{
	MySQL_Close();
	return 1;
}

MySQL_Init() {
    AddPlayerClass(0, 1958.33, 1343.12, 15.36, 269.15, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);
	g_DatabaseHandle = mysql_connect_file("mysql.ini");

    if (mysql_errno(g_DatabaseHandle) == 0) 
    {
        print("-----------------------------------------------");
        print("Successfully connected to database!");
        print("-----------------------------------------------");
    }
    else 
    {
        print("-----------------------------------------------");
        print("Failed to connect to database!");
        print("Please verify your mysql.ini settings");
        print("Server will be locked for maintenance...");
        print("-----------------------------------------------");

        SendRconCommand("password TvHRY2FmQjXEsCq");
        SendRconCommand("name Server is under maintenance!");
    }
	return 1;
}

MySQL_Close() {
	mysql_close(g_DatabaseHandle);
	return 1;
}
