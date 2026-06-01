#define ACCOUNT_MIN_PASSWORD_LENGTH (6)

//-----------------------------------------------------------------------------
// Embedded Colors
//-----------------------------------------------------------------------------

#define EMBED_WHITE "{FFFFFF}"
#define EMBED_RED   "{FFDE97}"


//-----------------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------------
// Enum Dialogs
enum
{
	DIALOG_NOTHING,
    DIALOG_REGISTER,
    DIALOG_LOGIN,
    DIALOG_SETADMIN,
    DIALOG_GOTO
};

// Pinfo
enum pInfo {
	pID,

    pLevel,
    pCash,
    pBank,
    pAdmin,

    Float:pPosX,
    Float:pPosY,
    Float:pPosZ,
    Float:pAngle,

    pInterior,
    pVirtualWorld,

    pSkin,
    pGender,

    bool:pLogged
}
new PlayerInfo[MAX_PLAYERS][pInfo];
new Float:InitPlayerPos[4] = {1958.38, 1343.16, 15.3746, 0.0};

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
new MySQL:g_DatabaseHandle;

enum E_ADMIN_LEVEL
{
    adminLevel,
    adminName[32]
};

new AdminLevels[][E_ADMIN_LEVEL] =
{
    {1, "Trial Admin"},
    {2, "Junior Admin"},
    {3, "Senior Admin"},
    {4, "Lead Admin"},
    {5, "Head Admin"},
    {6, "Developer"}
};

enum E_GOTO_LOC
{
    gotoName[32],
    Float:gotoX,
    Float:gotoY,
    Float:gotoZ,
    gotoInt,
    gotoVW
};
new SetAdminTarget[MAX_PLAYERS];

new GotoLocations[][E_GOTO_LOC] =
{
    {"LS",        1529.6,  -1691.2, 13.3,    0, 0},
    {"SF",       -1605.0,   720.0,  12.0,    0, 0},
    {"LV",        1699.2,  1435.1,  10.7,    0, 0},
    {"RC",        1253.70, 343.73,  19.41,   0, 0},
    {"El Que",   -1446.59,2608.44,  55.83,   0, 0},
    {"Bayside",  -2465.13,2333.65,   4.83,   0, 0},
    {"Bank",      1487.91,-1030.60, 23.66,   0, 0},
    {"FBI",         344.77,-1526.08, 33.28,   0, 0},
    {"DOC",      -2029.23, -78.33, 35.32,    0, 0},
    {"IC Prison",-2069.76,-200.05,991.53,   10, 0}
};
//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
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
// Login / Register Stock
Account_Check(playerid) {
	new query[256];
    TogglePlayerSpectating(playerid, true);
    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "SELECT * FROM Accounts WHERE Username='%e'",
        GetPlayerNameEx(playerid));

    mysql_tquery(g_DatabaseHandle, query, "OnAccountCheck", "d", playerid);
	return 1;
}

forward OnAccountCheck(playerid);
public OnAccountCheck(playerid)
{
    if(cache_num_rows())
    {
        ShowPlayerDialog(
            playerid,
            DIALOG_LOGIN,
            DIALOG_STYLE_PASSWORD,
            "Dang nhap",
            "Nhap mat khau:",
            "Login",
            "Thoat"
        );
    }
    else
    {
        ShowPlayerDialog(
            playerid,
            DIALOG_REGISTER,
            DIALOG_STYLE_PASSWORD,
            "Dang ky",
            "Chua co du lieu tai khoan.\nHay nhap vao o de tao mat khau moi:",
            "Register",
            "Thoat"
        );
    }
    return 1;
}

forward OnPasswordHash(playerid);
public OnPasswordHash(playerid)
{
    new hash[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(hash);

    new query[512];
	mysql_format(g_DatabaseHandle, query, sizeof(query),
		"INSERT INTO Accounts \
		(Username, Password) \
		VALUES ('%e', '%e')",
		GetPlayerNameEx(playerid),
		hash
	);

	mysql_tquery(g_DatabaseHandle, query, "OnAccountRegister", "d", playerid);
    return 1;
}

forward OnAccountRegister(playerid);
public OnAccountRegister(playerid)
{
    PlayerInfo[playerid][pID] = cache_insert_id();
	PlayerInfo[playerid][pLevel] = 1;
	PlayerInfo[playerid][pCash] = 5000;
	PlayerInfo[playerid][pBank] = 0;
	PlayerInfo[playerid][pAdmin] = 0;
	PlayerInfo[playerid][pPosX] = InitPlayerPos[0];
	PlayerInfo[playerid][pPosY] = InitPlayerPos[1];
	PlayerInfo[playerid][pPosZ] = InitPlayerPos[2];
	PlayerInfo[playerid][pAngle] = InitPlayerPos[3];
	PlayerInfo[playerid][pInterior] = 0;
	PlayerInfo[playerid][pVirtualWorld] = 0;
	PlayerInfo[playerid][pSkin] = 27;
	PlayerInfo[playerid][pGender] = 0;
	SetPlayerSpawn(playerid);
    return 1;
}

forward OnPasswordCheck(playerid, password[]);
public OnPasswordCheck(playerid, password[])
{
    new dbHash[BCRYPT_HASH_LENGTH];
    cache_get_value_name(0, "Password", dbHash);

	bcrypt_verify(playerid, "ResultPasswordCheck", password, dbHash);
    return 1;
}

forward ResultPasswordCheck(playerid, bool:match);
public ResultPasswordCheck(playerid, bool:match) {
	if(!match) {
		ShowPlayerDialog(
            playerid,
            DIALOG_LOGIN,
            DIALOG_STYLE_PASSWORD,
            "Dang nhap",
            "Sai mat khau.\nVui long nhap lai:",
            "Login",
            "Thoat"
        );
		return 1;
	} 

	LoadPlayerData(playerid);
    return 1;
}

LoadPlayerData(playerid)
{
    new query[256];

    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "SELECT * FROM Accounts WHERE Username='%e'",
        GetPlayerNameEx(playerid)
    );

    mysql_tquery(g_DatabaseHandle, query, "OnPlayerDataLoaded", "d", playerid);
}

forward OnPlayerDataLoaded(playerid);
public OnPlayerDataLoaded(playerid)
{
    if(!cache_num_rows())
        return 1;

    cache_get_value_name_int(0, "ID",
        PlayerInfo[playerid][pID]);

    cache_get_value_name_int(0, "Level",
        PlayerInfo[playerid][pLevel]);

    cache_get_value_name_int(0, "Cash",
        PlayerInfo[playerid][pCash]);

    cache_get_value_name_int(0, "Bank",
        PlayerInfo[playerid][pBank]);

    cache_get_value_name_int(0, "Admin",
        PlayerInfo[playerid][pAdmin]);

    cache_get_value_name_float(0, "PosX",
        PlayerInfo[playerid][pPosX]);

    cache_get_value_name_float(0, "PosY",
        PlayerInfo[playerid][pPosY]);

    cache_get_value_name_float(0, "PosZ",
        PlayerInfo[playerid][pPosZ]);

    cache_get_value_name_float(0, "Angle",
        PlayerInfo[playerid][pAngle]);

    cache_get_value_name_int(0, "Interior",
        PlayerInfo[playerid][pInterior]);

    cache_get_value_name_int(0, "VirtualWorld",
        PlayerInfo[playerid][pVirtualWorld]);

    cache_get_value_name_int(0, "Skin",
        PlayerInfo[playerid][pSkin]);

    cache_get_value_name_int(0, "Gender",
        PlayerInfo[playerid][pGender]);

    
	SetPlayerSpawn(playerid);
    return 1;
}

SetPlayerSpawn(playerid) {
    TogglePlayerSpectating(playerid, false);
	SetSpawnInfo(
        playerid,
        0,
        PlayerInfo[playerid][pSkin],
        PlayerInfo[playerid][pPosX],
        PlayerInfo[playerid][pPosY],
        PlayerInfo[playerid][pPosZ],
        PlayerInfo[playerid][pAngle],
        WEAPON_FIST,0,WEAPON_FIST,0,WEAPON_FIST,0
    );

	SetPlayerScore(
		playerid,
		PlayerInfo[playerid][pLevel]
	);

    SetPlayerInterior(
        playerid,
        PlayerInfo[playerid][pInterior]
    );

    SetPlayerVirtualWorld(
        playerid,
        PlayerInfo[playerid][pVirtualWorld]
    );

    GivePlayerMoney(
        playerid,
        PlayerInfo[playerid][pCash]
    );

	PlayerInfo[playerid][pLogged] = true;
    SpawnPlayer(playerid);
	return 1;
}

//-----------------------------------------------------------------------------
// Functions Support
//-----------------------------------------------------------------------------
stock GetPlayerNameEx(playerid)
{
    static name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    return name;
}

stock GetPlayerCash(playerid)
{
    return PlayerInfo[playerid][pCash];
}

stock GivePlayerCash(playerid, amount)
{
    PlayerInfo[playerid][pCash] += amount;

    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, PlayerInfo[playerid][pCash]);

    return 1;
}

stock SendClientMessageEx(playerid, color, const string[])
{
	SendClientMessage(playerid, color, string);
	return 1;
}

stock SendClientMessageToAllEx(color, const string[])
{
	foreach(new i: Player)
	{
		SendClientMessage(i, color, string);
	}
	return 1;
}

stock TeleportPlayerToLocation(playerid, locationid)
{
    if(locationid < 0 || locationid >= sizeof(GotoLocations))
        return 0;

    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
    {
        new vehicleid = GetPlayerVehicleID(playerid);

        SetVehiclePos(vehicleid,
            GotoLocations[locationid][gotoX],
            GotoLocations[locationid][gotoY],
            GotoLocations[locationid][gotoZ]);

        LinkVehicleToInterior(vehicleid,
            GotoLocations[locationid][gotoInt]);

        SetVehicleVirtualWorld(vehicleid,
            GotoLocations[locationid][gotoVW]);
    }
    else
    {
        SetPlayerPos(playerid,
            GotoLocations[locationid][gotoX],
            GotoLocations[locationid][gotoY],
            GotoLocations[locationid][gotoZ]);
    }

    SetPlayerInterior(playerid,
        GotoLocations[locationid][gotoInt]);

    SetPlayerVirtualWorld(playerid,
        GotoLocations[locationid][gotoVW]);

    PlayerInfo[playerid][pInterior] =
        GotoLocations[locationid][gotoInt];

    PlayerInfo[playerid][pVirtualWorld] =
        GotoLocations[locationid][gotoVW];

    SendClientMessageEx(playerid, COLOR_YELLOW,
        "Ban da duoc dich chuyen!");

    return 1;
}

//-----------------------------------------------------------------------------
// Callbacks Main
//-----------------------------------------------------------------------------
public OnPlayerConnect(playerid)
{
	Account_Check(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    if(!PlayerInfo[playerid][pLogged]) return 0;
    return 1;
}

public OnPlayerSpawn(playerid)
{
	if(!PlayerInfo[playerid][pLogged]) return 0;
	return 1;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
	return 1;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}


public OnPlayerText(playerid, text[])
{
	return 1;
}

public OnPlayerUpdate(playerid)
{
	if(GetPlayerMoney(playerid) != PlayerInfo[playerid][pCash])
    {
        ResetPlayerMoney(playerid);
        GivePlayerMoney(playerid, PlayerInfo[playerid][pCash]);
    }
	return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	return 1;
}

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerGiveDamageActor(playerid, damaged_actorid, Float:amount, WEAPON:weaponid, bodypart)
{
	return 1;
}

public OnActorStreamIn(actorid, forplayerid)
{
	return 1;
}

public OnActorStreamOut(actorid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid)
    {
        case DIALOG_REGISTER:
        {
            if(!response) return Kick(playerid);

			bcrypt_hash(playerid, "OnPasswordHash", inputtext, BCRYPT_COST);
			return 1;
        }

        case DIALOG_LOGIN:
        {
            if(!response) return Kick(playerid);

			new query[256];

			mysql_format(g_DatabaseHandle, query, sizeof(query),
				"SELECT Password FROM Accounts \
				WHERE Username='%e'",
				GetPlayerNameEx(playerid)
			);

			mysql_tquery(g_DatabaseHandle, query, "OnPasswordCheck", "ds",
				playerid,
				inputtext
			);
            return 1;  
        }
        case DIALOG_GOTO:
        {
            if(!response) return 1;

            TeleportPlayerToLocation(playerid, listitem);
            return 1;
        }
        case DIALOG_SETADMIN: {
            if(!response) return 1;

            new targetid = SetAdminTarget[playerid];

            if(!IsPlayerConnected(targetid))
                return SendClientMessageEx(playerid, COLOR_GREY,
                    "Nguoi choi da thoat.");

            PlayerInfo[targetid][pAdmin] =
                AdminLevels[listitem][adminLevel];

            new str[128];

            format(str, sizeof(str),
                "Ban da cap %s cho %s.",
                AdminLevels[listitem][adminName],
                GetPlayerNameEx(targetid));

            SendClientMessageEx(playerid, COLOR_GREEN, str);

            format(str, sizeof(str),
                "%s da cap cho ban quyen %s.",
                GetPlayerNameEx(playerid),
                AdminLevels[listitem][adminName]);

            SendClientMessageEx(targetid, COLOR_GREEN, str);

        }
    }
	return 1;
}

public OnPlayerEnterGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerLeaveGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerEnterPlayerGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerLeavePlayerGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerClickGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerClickPlayerGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerFinishedDownloading(playerid, virtualworld)
{
	return 1;
}

public OnPlayerRequestDownload(playerid, DOWNLOAD_REQUEST:type, crc)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 0;
}

public OnPlayerSelectObject(playerid, SELECT_OBJECT:type, objectid, modelid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}

public OnPlayerEditObject(playerid, playerobject, objectid, EDIT_RESPONSE:response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
	return 1;
}

public OnPlayerEditAttachedObject(playerid, EDIT_RESPONSE:response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnPlayerPickUpPlayerPickup(playerid, pickupid)
{
	return 1;
}

public OnPickupStreamIn(pickupid, playerid)
{
	return 1;
}

public OnPickupStreamOut(pickupid, playerid)
{
	return 1;
}

public OnPlayerPickupStreamIn(pickupid, playerid)
{
	return 1;
}

public OnPlayerPickupStreamOut(pickupid, playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, WEAPON:weaponid, bodypart)
{
	return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, WEAPON:weaponid, bodypart)
{
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, CLICK_SOURCE:source)
{
	return 1;
}

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}

public OnIncomingConnection(playerid, ip_address[], port)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    // TaiXiu_HandleClick(playerid, playertextid);
	return 1;
}

public OnTrailerUpdate(playerid, vehicleid)
{
	return 1;
}

public OnVehicleSirenStateChange(playerid, vehicleid, newstate)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnEnterExitModShop(playerid, enterexit, interiorid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
	return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	return 1;
}


// COMMANDS
CMD:setadmin(playerid, params[])
{
    new targetid;

    if(sscanf(params, "u", targetid))
        return SendClientMessageEx(playerid, COLOR_GREY,
            "SU DUNG: /setadmin [player]");

    if(!IsPlayerConnected(targetid))
        return SendClientMessageEx(playerid, COLOR_GREY,
            "Nguoi choi khong ton tai.");

    SetAdminTarget[playerid] = targetid;

    new dialog[1024];

    for(new i; i < sizeof(AdminLevels); i++)
    {
        format(dialog, sizeof(dialog),
            "%s%s\n",
            dialog,
            AdminLevels[i][adminName]);
    }

    ShowPlayerDialog(playerid,
        DIALOG_SETADMIN,
        DIALOG_STYLE_LIST,
        "Chon Cap Admin",
        dialog,
        "Chon",
        "Dong");

    return 1;
}

CMD:goto(playerid, params[])
{
    new str[2048];
    for(new i; i < sizeof(GotoLocations); i++)
    {
        format(str, sizeof(str),
            "%s%s\n",
            str,
            GotoLocations[i][gotoName]);
    }

    ShowPlayerDialog(playerid,
        DIALOG_GOTO,
        DIALOG_STYLE_LIST,
        "Goto Locations",
        str,
        "Chon",
        "Dong");

    return 1;
}