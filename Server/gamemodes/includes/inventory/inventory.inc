#include <YSI_Coding\y_hooks>

#define MAX_INV_SLOTS 24

enum E_INV_DATA
{
    invItemID,
    invAmount,

    invValue1,
    invValue2,
    invValue3
};

new PlayerInventory[MAX_PLAYERS][MAX_INV_SLOTS][E_INV_DATA];

stock ResetInventory(playerid)
{
    for(new slot; slot < MAX_INV_SLOTS; slot++)
    {
        PlayerInventory[playerid][slot][invItemID] = 0;
        PlayerInventory[playerid][slot][invAmount] = 0;

        PlayerInventory[playerid][slot][invValue1] = 0;
        PlayerInventory[playerid][slot][invValue2] = 0;
        PlayerInventory[playerid][slot][invValue3] = 0;
    }

    return 1;
}

stock LoadInventory(playerid) {
    new query[256];

    mysql_format(g_DatabaseHandle query, sizeof(query),
        "SELECT * FROM Inventory WHERE player_id = %d",
        PlayerInfo[playerid][pID]
    );

    mysql_tquery(g_DatabaseHandle query, "OnInventoryLoad", "d", playerid);
    return 1;
}

forward OnInventoryLoad(playerid);
public OnInventoryLoad(playerid)
{
    ResetInventory(playerid);

    new rows = cache_num_rows();

    for(new i; i < rows; i++)
    {
        new slot;

        cache_get_value_name_int(i, "slot", slot);

        if(slot < 0 || slot >= MAX_INV_SLOTS)
            continue;

        cache_get_value_name_int(i, "item_id",
            PlayerInventory[playerid][slot][invItemID]);

        cache_get_value_name_int(i, "amount",
            PlayerInventory[playerid][slot][invAmount]);

        cache_get_value_name_int(i, "value1",
            PlayerInventory[playerid][slot][invValue1]);

        cache_get_value_name_int(i, "value2",
            PlayerInventory[playerid][slot][invValue2]);

        cache_get_value_name_int(i, "value3",
            PlayerInventory[playerid][slot][invValue3]);
    }

    printf("[INV] Loaded %d slots for player %d",
        rows,
        playerid
    );

    return 1;
}

stock GetFreeInventorySlot(playerid)
{
    for(new slot; slot < MAX_INV_SLOTS; slot++)
    {
        if(PlayerInventory[playerid][slot][invItemID] == 0)
            return slot;
    }

    return -1;
}

stock GiveItem(playerid, itemid, amount)
{
    new slot = GetFreeInventorySlot(playerid);

    if(slot == -1)
        return 0;

    PlayerInventory[playerid][slot][invItemID] = itemid;
    PlayerInventory[playerid][slot][invAmount] = amount;

    return 1;
}

stock ClearInventorySlot(playerid, slot)
{
    PlayerInventory[playerid][slot][invItemID] = 0;
    PlayerInventory[playerid][slot][invAmount] = 0;

    PlayerInventory[playerid][slot][invValue1] = 0;
    PlayerInventory[playerid][slot][invValue2] = 0;
    PlayerInventory[playerid][slot][invValue3] = 0;

    new query[512];
    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "DELETE FROM Inventory WHERE player_id = %d AND slot = %d",
        PlayerInfo[playerid][pID],
        slot
    );

    return 1;
}

stock SaveInventory(playerid)
{
    new query[512];

    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "DELETE FROM Inventory WHERE player_id = %d",
        PlayerInfo[playerid][pID]
    );
    mysql_tquery(g_DatabaseHandle, query);

    for(new slot; slot < MAX_INV_SLOTS; slot++)
    {
        if(PlayerInventory[playerid][slot][invItemID] == 0)
            continue;

        mysql_format(g_DatabaseHandle, query, sizeof(query),
            "INSERT INTO Inventory \
            (player_id, slot, item_id, amount, value1, value2, value3) \
            VALUES (%d,%d,%d,%d,%d,%d,%d)",

            PlayerInfo[playerid][pID],

            slot,

            PlayerInventory[playerid][slot][invItemID],
            PlayerInventory[playerid][slot][invAmount],

            PlayerInventory[playerid][slot][invValue1],
            PlayerInventory[playerid][slot][invValue2],
            PlayerInventory[playerid][slot][invValue3]
        );

        mysql_tquery(g_DatabaseHandle, query);
    }

    return 1;
}