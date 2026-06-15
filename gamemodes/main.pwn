#include <open.mp>
#include <a_mysql>
#include <progress2>
#include <samp_bcrypt>
#include <zcmd>

new MySQL:g_DatabaseHandle;

// YSI
#include <YSI_Coding\y_hooks>
#include <YSI_Coding\y_timers>


// UI
#include "includes/ui/colors.inc"
#include "includes/ui/notify.inc"
#include "includes/ui/loading.inc"
#include "includes/ui/zones.inc"

// System
#include "includes/system/defines.inc"
#include "includes/system/dialogs.inc"
#include "includes/system/vars.inc"
#include "includes/system/database.inc"
#include "includes/system/account.inc"
#include "includes/system/tasks.inc"


// Commands
#include "includes/commands/admins.inc"
#include "includes/commands/commands.inc"

// Core event handlers
#include "includes/core/events.inc"
#include "includes/core/e_friends.inc"
#include "includes/core/e_player.inc"

// Conversations
//#include "includes/conversations/messager.inc"

// TaiXiu
#include "includes/minigames/taixiu/Var.pwn"
#include "includes/minigames/taixiu/TD.pwn"
#include "includes/minigames/taixiu/System.pwn"

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

	//ShowNameTags(false);
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