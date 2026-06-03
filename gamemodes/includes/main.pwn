#define ACCOUNT_MIN_PASSWORD_LENGTH (6)
#define MAX_WEAPON_SLOTS 13

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

new MySQL:g_DatabaseHandle;

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
new gLoggedIn[MAX_PLAYERS];

enum E_WEAPON_DATA
{
    WEAPON:WeaponID,
    Ammo
};
new PlayerWeapon[MAX_PLAYERS][MAX_WEAPON_SLOTS][E_WEAPON_DATA];


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

new bool:gAdminGod[MAX_PLAYERS];

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
    {"DOC",      -2029.23, -78.33, 35.32,    0, 0}
};

//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
MySQL_Init() {
    AddPlayerClass(0, 1958.33, 1343.12, 15.36, 269.15, WEAPON_FIST, 0, WEAPON_FIST, 0, WEAPON_FIST, 0);
	g_DatabaseHandle = mysql_connect_file("mysql.ini");

    if (mysql_errno(g_DatabaseHandle) == 0) 
    {
        CreateWeaponTable();
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
    LoadPlayerWeapons(playerid);
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
    gLoggedIn[playerid] = true;
    SpawnPlayer(playerid);
	return 1;
}

TeleportPlayerToLocation(playerid, locationid)
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


stock SpectatePlayer(playerid, giveplayerid)
{
	if(IsPlayerConnected(giveplayerid)) {
    
        new Float: pPositions[3];
        GetPlayerPos(playerid, pPositions[0], pPositions[1], pPositions[2]);
        SetPVarFloat(playerid, "SpecPosX", pPositions[0]);
        SetPVarFloat(playerid, "SpecPosY", pPositions[1]);
        SetPVarFloat(playerid, "SpecPosZ", pPositions[2]);
        SetPVarInt(playerid, "SpecInt", GetPlayerInterior(playerid));
        SetPVarInt(playerid, "SpecVW", GetPlayerVirtualWorld(playerid));
        if(IsPlayerInAnyVehicle(giveplayerid)) {
            TogglePlayerSpectating(playerid, true);
            new carid = GetPlayerVehicleID( giveplayerid );
            PlayerSpectateVehicle( playerid, carid );
            SetPlayerInterior( playerid, GetPlayerInterior( giveplayerid ) );
            SetPlayerVirtualWorld( playerid, GetPlayerVirtualWorld( giveplayerid ) );
        }
        else {
            for(new i = 0; i < 2; i++) {
                TogglePlayerSpectating(playerid, true);
                PlayerSpectatePlayer( playerid, giveplayerid );
                SetPlayerInterior( playerid, GetPlayerInterior( giveplayerid ) );
                SetPlayerVirtualWorld( playerid, GetPlayerVirtualWorld( giveplayerid ) );
            }
        }
		new string[64];
		format(string, sizeof(string), "Ban dang theo doi %s (ID: %d).", GetPlayerNameEx(giveplayerid), giveplayerid);
		SendClientMessageEx(playerid, COLOR_SUCCESS, string);
	}
	return 1;
}

// Weapons
stock ReloadPlayerWeapon(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    new weaponid = GetPlayerWeapon(playerid);
    if (weaponid <= 0) return 0;

    new slot = GetWeaponSlot(WEAPON:weaponid);
    if (slot < 0) return 0;

    new reserve = PlayerWeapon[playerid][slot][Ammo];
    if (reserve <= 0) return 0;

    new currentAmmo = GetPlayerAmmo(playerid);
    new magazine = GetWeaponMagazineSize(weaponid);
    if (magazine <= 0) return 0;
    if (currentAmmo >= magazine) return 0;

    new available = magazine - currentAmmo;
    if (available > reserve) available = reserve;

    SetPlayerAmmo(playerid, WEAPON:weaponid, currentAmmo + available);
    PlayerWeapon[playerid][slot][Ammo] = reserve - available;

    new message[128];
    format(message, sizeof(message), "Da nap lai %d vien. Con lai %d vien trong tui.", available, PlayerWeapon[playerid][slot][Ammo]);
    SendClientMessageEx(playerid, COLOR_SUCCESS, message);
    return 1;
}

stock WeaponShot_UpdateReloadPrompt(playerid, weaponid)
{
    new slot = GetWeaponSlot(WEAPON:weaponid);
    if (slot < 0) return 0;
    if (!IsWeaponReloadable(weaponid)) return 0;

    new currentAmmo = GetPlayerAmmo(playerid);
    if (currentAmmo != 0) return 0;

    new reserve = PlayerWeapon[playerid][slot][Ammo];
    if (reserve <= 0) return 0;

    new message[128];
    format(message, sizeof(message), "Het dan! Nhan R de nap lai bang dan. Con lai %d vien.", reserve);
    SendClientMessageEx(playerid, COLOR_ORANGE, message);
    return 1;
}

stock HandleWeaponReloadKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if ((newkeys & KEY_ACTION) && !(oldkeys & KEY_ACTION))
    {
        new weaponid = GetPlayerWeapon(playerid);
        if (weaponid <= 0) return 0;

        if (!IsWeaponReloadable(weaponid)) return 0;

        new currentAmmo = GetPlayerAmmo(playerid);
        if (currentAmmo > 0) return 0;

        new slot = GetWeaponSlot(WEAPON:weaponid);
        if (slot < 0) return 0;

        if (PlayerWeapon[playerid][slot][Ammo] <= 0)
        {
            SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong con dan de nap lai.");
            return 1;
        }

        return ReloadPlayerWeapon(playerid);
    }
    return 0;
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

stock IsInvalidSkin(skin) {
	if(!(0 <= skin <= 299)) return 1;
    return 0;
}

stock GetAdminName(level)
{
    new str[32];
    for(new i = 0; i < sizeof(AdminLevels); i++)
    {
        if(AdminLevels[i][adminLevel] == level) {
            format(str, sizeof(str), "%s", AdminLevels[i][adminName]);
            return str;
        }
    }
    format(str, sizeof(str), "Unknown (%d)", level);
    return str;
}

stock GetWeaponMagazineSize(weaponid)
{
    static const WeaponMagazineSize[48] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 17, 17, 7, 8, 2, 8, 30, 30, 30, 30, 30,
        10, 10, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if (weaponid < 0 || weaponid >= sizeof(WeaponMagazineSize)) return 0;
    return WeaponMagazineSize[weaponid];
}

stock IsWeaponReloadable(weaponid)
{
    return GetWeaponMagazineSize(weaponid) > 0;
}

stock GivePlayerWeaponEx(playerid, weaponid, totalAmmo)
{
    if (!IsPlayerConnected(playerid)) return 0;
    new slot = GetWeaponSlot(WEAPON:weaponid);
    if (slot < 0 || weaponid <= 0) return 0;

    PlayerWeapon[playerid][slot][WeaponID] = WEAPON:weaponid;
    new magazine = GetWeaponMagazineSize(weaponid);
    if (magazine <= 0)
    {
        PlayerWeapon[playerid][slot][Ammo] = 0;
        GivePlayerWeapon(playerid, WEAPON:weaponid, totalAmmo);
        return 1;
    }

    if (totalAmmo < 0) totalAmmo = 0;
    new clip = totalAmmo;
    if (clip > magazine) clip = magazine;

    PlayerWeapon[playerid][slot][Ammo] = totalAmmo - clip;
    GivePlayerWeapon(playerid, WEAPON:weaponid, clip);
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
    Account_Save(playerid);
    SavePlayerWeapons(playerid);
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
    if(gLoggedIn[playerid]) {
        new string[128];
        format(string, sizeof(string), "Chao mung %s den voi may chu!", GetPlayerNameEx(playerid));
        SendClientMessageEx(playerid, COLOR_SUCCESS, string);
        if(PlayerInfo[playerid][pAdmin] > 0) {
            format(string, sizeof(string), "[AD] Ban dang dang nhap voi tai khoan {FF0000}%s", GetAdminName(PlayerInfo[playerid][pAdmin]));
        }
        SendClientMessageEx(playerid, COLOR_SUCCESS, string);
        gLoggedIn[playerid] = false;
    }
	return 1;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
    if(gAdminGod[playerid]) {
        SpawnPlayer(playerid);
        SetPlayerHealth(playerid, 100);
        return 0;
    }
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
    if (HandleWeaponReloadKey(playerid, newkeys, oldkeys))
    {
        return 1;
    }
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
    if(gAdminGod[playerid]) return 0;
    
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
    WeaponShot_UpdateReloadPrompt(playerid, weaponid);
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
CMD:gotoco(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
        return SendClientMessage(playerid, COLOR_DANGER, "Ban khong du quyen.");

    new Float:x, Float:y, Float:z;
    new interior;

    if(sscanf(params, "p<,>fffi", x, y, z, interior)) return SendClientMessage(playerid, COLOR_INFO, "Su dung: /gotoco x,y,z,int");

    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
    {
        new vehicleid = GetPlayerVehicleID(playerid);

        SetVehiclePos(vehicleid, x, y, z);
        LinkVehicleToInterior(vehicleid, interior);
    }
    else
    {
        SetPlayerPos(playerid, x, y, z);
    }

    SetPlayerInterior(playerid, interior);
    PlayerInfo[playerid][pInterior] = interior;

    SetPlayerVirtualWorld(playerid, 0);
    PlayerInfo[playerid][pVirtualWorld] = 0;

    SendClientMessage(playerid, COLOR_SUCCESS,
        "Da dich chuyen thanh cong.");

    return 1;
}

CMD:veh(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 2)
        return SendClientMessage(playerid, COLOR_DANGER, "Ban khong du quyen.");

    new vehicleModel;
    new color1, color2;

    if(sscanf(params, "iii", vehicleModel, color1, color2))
    {
        SendClientMessage(playerid, COLOR_INFO,
            "Su dung: /v [modelid] [color1] [color2]");
        SendClientMessage(playerid, COLOR_INFO,
            "Delete: /delveh de xoa xe dang ngoi !");
        return 1;
    }

    if(vehicleModel < 400 || vehicleModel > 611)
    {
        return SendClientMessage(playerid, COLOR_DANGER,
            "Model xe phai tu 400 den 611.");
    }

    new Float:x, Float:y, Float:z, Float:a;

    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    new vehicleid = CreateVehicle(
        vehicleModel,
        x + 2.0,
        y,
        z,
        a,
        color1,
        color2,
        -1
    );

    PutPlayerInVehicle(playerid, vehicleid, 0);

    new str[128];
    format(str, sizeof(str),
        "Da tao xe ID %d (VehicleID: %d).",
        vehicleModel,
        vehicleid);

    SendClientMessage(playerid, COLOR_SUCCESS, str);
    return 1;
}
CMD:delveh(playerid, params[])
{
    if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, COLOR_DANGER,
            "Ban khong o trong xe.");

    new vehicleid = GetPlayerVehicleID(playerid);

    RemovePlayerFromVehicle(playerid);
    DestroyVehicle(vehicleid);

    SendClientMessage(playerid, COLOR_SUCCESS,
        "Da xoa xe.");
    return 1;
}
CMD:spec(playerid, params[])
{
	if(PlayerInfo[playerid][pAdmin] < 1) return SendClientMessage(playerid, COLOR_DANGER, "Ban khong du quyen.");

	if(strcmp(params, "off", true) == 0)
	{
        TogglePlayerSpectating(playerid, false);
        SetCameraBehindPlayer(playerid);
        return 1;
	}

	new giveplayerid;
	if(sscanf(params, "u", giveplayerid)) return SendClientMessageEx(playerid, COLOR_INFO, "SU DUNG: /spec (playerid/off)");
	if(IsPlayerConnected(giveplayerid))
	{
		SpectatePlayer(playerid, giveplayerid);
	}
	else
	{
		SendClientMessageEx(playerid, COLOR_DANGER, "Target is not available.");
	}
	return 1;
}

CMD:gotoid(playerid, params[])
{
	new giveplayerid;
	if(sscanf(params, "u", giveplayerid)) return SendClientMessageEx(playerid, COLOR_INFO, "SU DUNG: /gotoid [player]");

	new Float:plocx,Float:plocy,Float:plocz;
	if (IsPlayerConnected(giveplayerid))
	{
		if (PlayerInfo[playerid][pAdmin] >= 2)
		{
			if(GetPlayerState(giveplayerid) == PLAYER_STATE_SPECTATING)
			{
				SendClientMessageEx(playerid, COLOR_DANGER, "Nguoi do dang theo doi nguoi choi");
				return 1;
			}
			if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
			{
				SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong the lam dieu nay khi dang theo doi.");
				return 1;
			}
			GetPlayerPos(giveplayerid, plocx, plocy, plocz);
			SetPlayerVirtualWorld(playerid, PlayerInfo[giveplayerid][pVirtualWorld]);

            SetPlayerPos(playerid,plocx,plocy+2, plocz);
            SetPlayerInterior(playerid, GetPlayerInterior(giveplayerid));
            SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(giveplayerid));

			SendClientMessageEx(playerid, COLOR_SUCCESS, "   Ban da duoc dich chuyen!");
		}
		else
		{
			SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong duoc phep su dung lenh nay.");
		}

	}
	else SendClientMessageEx(playerid, COLOR_DANGER, "Nguoi choi khong hop le.");
	return 1;
}

CMD:givegun(playerid, params[])
{
    if (PlayerInfo[playerid][pAdmin] >= 1) {
        new sstring[128], playa, gun, ammo;

        if(sscanf(params, "udd", playa, gun, ammo)) {
            SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /givegun [player] [weaponid] [ammo]");
            SendClientMessageEx(playerid, COLOR_GREEN, "_______________________________________");
            SendClientMessageEx(playerid, COLOR_GREY, "(1)Brass Knuckles (2)Golf Club (3)Nite Stick (4)Knife (5)Baseball Bat (6)Shovel (7)Pool Cue (8)Katana (9)Chainsaw");
            SendClientMessageEx(playerid, COLOR_GREY, "(10)Purple Dildo (11)Small White Vibrator (12)Large White Vibrator (13)Silver Vibrator (14)Flowers (15)Cane (16)Frag Grenade");
            SendClientMessageEx(playerid, COLOR_GREY, "(17)Tear Gas (18)Molotov Cocktail (21)Jetpack (22)9mm (23)Silenced 9mm (24)Desert Eagle (25)Shotgun (26)Sawnoff Shotgun");
            SendClientMessageEx(playerid, COLOR_GREY, "(27)Combat Shotgun (28)Micro SMG (Mac 10) (29)SMG (MP5) (30)AK-47 (31)M4 (32)Tec9 (33)Rifle (34)Sniper Rifle");
            SendClientMessageEx(playerid, COLOR_GREY, "(35)Rocket Launcher (36)HS Rocket Launcher (37)Flamethrower (38)Minigun (39)Satchel Charge (40)Detonator");
            SendClientMessageEx(playerid, COLOR_GREY, "(41)Spraycan (42)Fire Extinguisher (43)Camera (44)Nightvision Goggles (45)Infared Goggles (46)Parachute");
            SendClientMessageEx(playerid, COLOR_GREEN, "_______________________________________");
            return 1;
        }

        format(sstring, sizeof(sstring), "Ban da cho %s gun ID %d!",GetPlayerNameEx(playa),gun);
        if(gun < 1||gun > 47)
            { SendClientMessageEx(playerid, COLOR_GREY, "ID vu khi khong hop le!"); return 1; }
        if(IsPlayerConnected(playa)) {
            if(playa != INVALID_PLAYER_ID && gun <= 20 || gun >= 22) {
                GivePlayerWeaponEx(playa, gun, ammo);
                SendClientMessageEx(playerid, COLOR_GREY, sstring);
            }
            else if(playa != INVALID_PLAYER_ID && gun == 21) {
                SetPlayerSpecialAction(playa, SPECIAL_ACTION_USEJETPACK);
                SendClientMessageEx(playerid, COLOR_GREY, sstring);
            }
        }
    }
    else {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong duoc phep su dung lenh nay.");
    }
    return 1;
}

CMD:setskin(playerid, params[])
{
	if (PlayerInfo[playerid][pAdmin] >= 1)
	{
		new string[128], giveplayerid, skinid;
		if(sscanf(params, "ud", giveplayerid, skinid)) return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /setskin [player] [skinid]");

		if(IsPlayerConnected(giveplayerid))
		{
			if(!IsInvalidSkin(skinid))
			{
				if(GetPlayerSkin(giveplayerid) == skinid)
				{
					SendClientMessageEx( playerid, COLOR_WHITE, "The person you're trying to change skins of already is using the skin you're trying to set." );
				}
				else
				{
					PlayerInfo[giveplayerid][pSkin] = skinid;
					format(string, sizeof(string), "Your skin has been changed to ID %d by Administrator %s.", skinid, GetPlayerNameEx(playerid));
					SendClientMessageEx(giveplayerid, COLOR_WHITE, string);
					format(string, sizeof(string), "Ban da cho %s skin ID %d.", GetPlayerNameEx(giveplayerid), skinid);
					SendClientMessageEx(playerid, COLOR_WHITE, string);
                    SetPlayerSkin(giveplayerid, PlayerInfo[giveplayerid][pSkin]);
				}
			}
			else
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Invalid skin ID!");
			}
		}
	}
	else
	{
		SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong duoc phep su dung lenh nay.");
	}
	return 1;
}

CMD:god(playerid, params[]) {
    if (PlayerInfo[playerid][pAdmin] >= 1) {
        gAdminGod[playerid] = !gAdminGod[playerid];
        SendClientMessageEx(playerid, COLOR_SUCCESS, gAdminGod[playerid] ? "[AD] God mode enabled." : "[AD] God mode disabled.");
    } else {
        SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong duoc phep su dung lenh nay.");
    }
    return 1;
}