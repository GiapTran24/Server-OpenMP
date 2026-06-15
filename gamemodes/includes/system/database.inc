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

    cache_get_value_name_int(0, "Level",
        PlayerInfo[playerid][pLevel]);

    cache_get_value_name_int(0, "Cash",
        PlayerInfo[playerid][pCash]);

    cache_get_value_name_int(0, "Bank",
        PlayerInfo[playerid][pBank]);

    cache_get_value_name_int(0, "Admin",
        PlayerInfo[playerid][pAdmin]);

    cache_get_value_name_float(0, "Health",
        PlayerInfo[playerid][pHealth]);

    cache_get_value_name_float(0, "Armour",
        PlayerInfo[playerid][pArmour]);

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
    new Float:playerHealth, Float:playerArmour;
    GetPlayerHealth(playerid, playerHealth);
    GetPlayerArmour(playerid, playerArmour);

    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "UPDATE accounts SET \
        Level=%d,\
        Health=%f,\
        Armour=%f,\
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
        playerHealth,
        playerArmour,
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


// Sy_Friends
stock SavePlayerFriends(playerid)
{
    new query[256];

    for(new i; i < FriendCount[playerid]; i++)
    {
        if(!PlayerFriends[playerid][i][Status]) continue;

        mysql_format(g_DatabaseHandle, query, sizeof(query),
            "INSERT INTO FRIENDS \
            (PlayerID, FriendID, Status, Sender) \
            VALUES (%d, %d, %d, %d)",
            PlayerInfo[playerid][pID],
            PlayerFriends[playerid][i][ID],
            PlayerFriends[playerid][i][Status],
            PlayerFriends[playerid][i][Sender]
        );

        mysql_tquery(g_DatabaseHandle, query);
    }
    return 1;
}

stock LoadPlayerFriends(playerid) {
    new query[128];
    print("Loading List Friends");
    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "SELECT * FROM FRIENDS WHERE PlayerID = %d",
        PlayerInfo[playerid][pID]
    );

    mysql_tquery(g_DatabaseHandle, query, "OnPlayerFriendsLoad", "d", playerid);
}
forward OnPlayerFriendsLoad(playerid);
public OnPlayerFriendsLoad(playerid)
{
    new rows = cache_num_rows();

    FriendCount[playerid] = 0;

    for(new i; i < rows; i++)
    {
        if(FriendCount[playerid] >= MAX_FRIENDS)
            break;

        cache_get_value_name_int(i, "FriendID",
            PlayerFriends[playerid][i][ID]);

        cache_get_value_name_int(i, "Status",
            PlayerFriends[playerid][i][Status]);

        cache_get_value_name_int(i, "Sender",
            PlayerFriends[playerid][i][Sender]);

        FriendCount[playerid]++;
    }
    
    UpdateFriendTags(playerid);
    return 1;
}