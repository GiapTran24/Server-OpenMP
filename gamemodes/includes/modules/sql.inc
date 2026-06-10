/// Accounts

LoadPlayerData(playerid)
{
    new query[256];

    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "SELECT * FROM accounts WHERE ID='%d'",
        PlayerInfo[playerid][pID]
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
    cache_get_value_name(0, "Username",
        PlayerInfo[playerid][pName]);

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

    cache_get_value_name_int(0, "Logged",
        PlayerInfo[playerid][pLogged]);


	SetPlayerSpawn(playerid);
    return 1;
}

SetAccountOnline(playerid, stateid) {
    new query[256];
    mysql_format(g_DatabaseHandle, query, sizeof(query), "UPDATE `accounts` SET `Logged`=%d, `LastLogin` = NOW() WHERE `ID` = %d", stateid, PlayerInfo[playerid][pID]);
    mysql_tquery(g_DatabaseHandle, query);
    return 1;
}

Account_Save(playerid)
{
    if(PlayerInfo[playerid][pLogged] != 1) return 1;

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
        "UPDATE accounts SET \
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
