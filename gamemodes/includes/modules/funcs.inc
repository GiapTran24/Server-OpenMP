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


// Admin 
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