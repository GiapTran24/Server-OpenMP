stock CreateWeaponTable()
{
    if (mysql_errno(g_DatabaseHandle) != 0) return 0;

    new query[512];
    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "CREATE TABLE IF NOT EXISTS player_weapons (AccountID INT UNSIGNED NOT NULL, Slot TINYINT NOT NULL, WeaponID SMALLINT NOT NULL, Ammo INT NOT NULL, PRIMARY KEY (AccountID, Slot)) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;");
    mysql_query(g_DatabaseHandle, query);
    return 1;
}

forward OnPlayerWeaponsLoaded(playerid);
public OnPlayerWeaponsLoaded(playerid)
{
    new rows = cache_num_rows();
    if (rows <= 0) return 1;

    ResetPlayerWeapons(playerid);
    for (new slot = 0; slot < MAX_WEAPON_SLOTS; slot++)
    {
        PlayerWeapon[playerid][slot][WeaponID] = WEAPON:0;
        PlayerWeapon[playerid][slot][Ammo] = 0;
    }

    new slot, weaponid, totalAmmo;
    for (new i = 0; i < rows; i++)
    {
        cache_get_value_name_int(i, "Slot", slot);
        cache_get_value_name_int(i, "WeaponID", weaponid);
        cache_get_value_name_int(i, "Ammo", totalAmmo);

        if (slot < 0 || slot >= MAX_WEAPON_SLOTS) continue;
        if (weaponid <= 0) continue;
        if (totalAmmo < 0) totalAmmo = 0;

        GivePlayerWeaponEx(playerid, WEAPON:weaponid, totalAmmo);
    }
    return 1;
}

stock LoadPlayerWeapons(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;
    if (PlayerInfo[playerid][pID] <= 0) return 0;

    CreateWeaponTable();

    new query[256];
    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "SELECT Slot, WeaponID, Ammo FROM player_weapons WHERE AccountID=%d ORDER BY Slot",
        PlayerInfo[playerid][pID]);
    mysql_tquery(g_DatabaseHandle, query, "OnPlayerWeaponsLoaded", "d", playerid);
    return 1;
}

stock SavePlayerWeapons(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;
    if (PlayerInfo[playerid][pID] <= 0) return 0;

    CreateWeaponTable();

    new query[256];
    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "DELETE FROM player_weapons WHERE AccountID=%d",
        PlayerInfo[playerid][pID]);
    mysql_query(g_DatabaseHandle, query);

    new slot, weaponid, slotAmmo, totalAmmo;
    for (slot = 0; slot < MAX_WEAPON_SLOTS; slot++)
    {
        weaponid = PlayerWeapon[playerid][slot][WeaponID];
        if (weaponid <= 0) continue;

        if (!GetPlayerWeaponData(playerid, WEAPON_SLOT:slot, WEAPON:weaponid, slotAmmo))
        {
            slotAmmo = 0;
        }

        totalAmmo = slotAmmo + PlayerWeapon[playerid][slot][Ammo];
        if (totalAmmo < 0) totalAmmo = 0;

        mysql_format(g_DatabaseHandle, query, sizeof(query),
            "INSERT INTO player_weapons (AccountID, Slot, WeaponID, Ammo) VALUES (%d, %d, %d, %d)",
            PlayerInfo[playerid][pID], slot, weaponid, totalAmmo);
        mysql_query(g_DatabaseHandle, query);
    }
    return 1;
}
