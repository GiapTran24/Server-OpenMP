#include <open.mp>
#include <a_mysql>
#include <progress2>
#include <samp_bcrypt>
#include <zcmd>

new MySQL:g_DatabaseHandle;

// YSI
#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>


//Helpers
#include "includes/helpers/colors.inc"
#include "includes/helpers/notify.inc"
#include "includes/helpers/loading.inc"

// Main
#include "includes/modules/defines.inc"
#include "includes/modules/dialogs.inc"
#include "includes/modules/vars.inc"
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

        SendRconCommand("password 123123");
        SendRconCommand("name Server is under maintenance!");
    }

	ShowNameTags(false);
	return 1;
}

public OnGameModeExit()
{
	new query[256];
    mysql_format(g_DatabaseHandle, query, sizeof(query), "UPDATE `accounts` SET `Logged`=0 WHERE `Logged` = 1");
    mysql_tquery(g_DatabaseHandle, query);

	mysql_close(g_DatabaseHandle);
	return 1;
}