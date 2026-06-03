
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

/// Accounts
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

Account_Save(playerid)
{
    if(!PlayerInfo[playerid][pLogged])
        return 0;

    GetPlayerPos(
        playerid,
        PlayerInfo[playerid][pPosX],
        PlayerInfo[playerid][pPosY],
        PlayerInfo[playerid][pPosZ]
    );

    GetPlayerFacingAngle(
        playerid,
        PlayerInfo[playerid][pAngle]
    );

    PlayerInfo[playerid][pInterior] = GetPlayerInterior(playerid);
    PlayerInfo[playerid][pVirtualWorld] = GetPlayerVirtualWorld(playerid);
    PlayerInfo[playerid][pSkin] = GetPlayerSkin(playerid);

    new query[1024];

    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "UPDATE Accounts SET \
        Level=%d,\
        Cash=%d,\
        Bank=%d,\
        Admin=%d,\
        PosX=%f,\
        PosY=%f,\
        PosZ=%f,\
        Angle=%f,\
        Interior=%d,\
        VirtualWorld=%d,\
        Skin=%d,\
        Gender=%d \
        WHERE ID=%d",

        PlayerInfo[playerid][pLevel],
        PlayerInfo[playerid][pCash],
        PlayerInfo[playerid][pBank],
        PlayerInfo[playerid][pAdmin],

        PlayerInfo[playerid][pPosX],
        PlayerInfo[playerid][pPosY],
        PlayerInfo[playerid][pPosZ],
        PlayerInfo[playerid][pAngle],

        PlayerInfo[playerid][pInterior],
        PlayerInfo[playerid][pVirtualWorld],

        PlayerInfo[playerid][pSkin],
        PlayerInfo[playerid][pGender],

        PlayerInfo[playerid][pID]
    );

    mysql_tquery(g_DatabaseHandle, query);

    return 1;
}
