stock GivePlayerSavedWeapon(playerid, weaponid, ammo)
{
    new slot = GetWeaponSlot(weaponid);

    if(slot < 0 || slot >= MAX_WEAPON_SLOTS)
        return 0;

    PlayerWeapon[playerid][slot][WeaponID] = weaponid;
    PlayerWeapon[playerid][slot][Ammo] = ammo;

    GivePlayerWeapon(playerid, weaponid, ammo);
    return 1;
}

stock SyncPlayerWeapons(playerid)
{
    for(new slot; slot < MAX_WEAPON_SLOTS; slot++)
    {
        GetPlayerWeaponData(
            playerid,
            slot,
            PlayerWeapon[playerid][slot][WeaponID],
            PlayerWeapon[playerid][slot][Ammo]
        );
    }

    return 1;
}

stock LoadPlayerWeapons(playerid)
{
    new query[128];

    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "SELECT WeaponID, Ammo FROM p_weapons WHERE PlayerID = %d",
        PlayerInfo[playerid][pID]);

    mysql_tquery(g_DatabaseHandle, query, "OnPlayerWeaponsLoaded", "d", playerid);
    return 1;
}
forward OnPlayerWeaponsLoaded(playerid);
public OnPlayerWeaponsLoaded(playerid)
{
    new rows = cache_num_rows();

    for(new i; i < rows; i++)
    {
        new weaponid, ammo;
        cache_get_value_name_int(i, "WeaponID", weaponid);
        cache_get_value_name_int(i, "Ammo", ammo);

        new slot = GetWeaponSlot(weaponid);

        if(slot < 0 || slot >= MAX_WEAPON_SLOTS)
            continue;

        PlayerWeapon[playerid][slot][WeaponID] = weaponid;
        PlayerWeapon[playerid][slot][Ammo] = ammo;

        //setgun for player
        GivePlayerWeapon(
            playerid,
            PlayerWeapon[playerid][slot][WeaponID],
            PlayerWeapon[playerid][slot][Ammo]
        );
    }

    return 1;
}

stock SavePlayerWeapons(playerid)
{
    SyncPlayerWeapons(playerid);

    new query[256];

    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "DELETE FROM p_weapons WHERE PlayerID = %d",
        PlayerInfo[playerid][pID]);

    mysql_tquery(g_DatabaseHandle, query);

    for(new slot; slot < MAX_WEAPON_SLOTS; slot++)
    {
        if(PlayerWeapon[playerid][slot][WeaponID] == 0)
            continue;

        mysql_format(g_DatabaseHandle, query, sizeof(query),
            "INSERT INTO p_weapons \
            (PlayerID, WeaponID, Ammo) \
            VALUES (%d, %d, %d)",

            PlayerInfo[playerid][pID],
            PlayerWeapon[playerid][slot][WeaponID],
            PlayerWeapon[playerid][slot][Ammo]
        );

        mysql_tquery(g_DatabaseHandle, query);
    }

    return 1;
}