#include <YSI_Coding\y_hooks>
#include "includes/inventory/inv_td.inc"
#include "includes/inventory/inv_items.inc"

#define MAX_INV_TYPES 2
#define MAX_INV_SLOTS (MAX_INV_EQUIP_SLOTS + MAX_INV_MAIN_SLOTS)

enum E_INV_DATA
{
    invDBID,
    invItemID,
    invAmount,
    Float:invDurability,
    invSlotID,
    invSlotType
};

new PlayerInventory[MAX_PLAYERS][MAX_INV_SLOTS][E_INV_DATA];

enum E_INV_CLICK_SLOT {
    inv_click_slot,
    inv_click_slotType
};
new gInvClickedSlot[MAX_PLAYERS][E_INV_CLICK_SLOT];

stock LoadPlayerInventory(playerid)
{
    new query[128];
    mysql_format(g_DatabaseHandle, query, sizeof(query), "SELECT * FROM `player_inventory` WHERE `playerID` = %d", PlayerInfo[playerid][pID]);
    mysql_tquery(g_DatabaseHandle, query, "OnPlayerInventoryLoaded", "d", playerid);
}

forward OnPlayerInventoryLoaded(playerid);
public OnPlayerInventoryLoaded(playerid)
{
    new rows = cache_num_rows();
    new totals = 0;
    new msg[128];

    if(rows > 0)
    {
        for(new i = 0; i < rows; i++)
        {
            cache_get_value_name_int(i, "invDBID", PlayerInventory[playerid][i][invDBID]);
           
            cache_get_value_name_int(i, "invItemID", PlayerInventory[playerid][i][invItemID]);
            cache_get_value_name_int(i, "invAmount", PlayerInventory[playerid][i][invAmount]);
            cache_get_value_name_float(i, "invDurability", PlayerInventory[playerid][i][invDurability]);
            cache_get_value_name_int(i, "invSlotID", PlayerInventory[playerid][i][invSlotID]);
            cache_get_value_name_int(i, "invSlotType", PlayerInventory[playerid][i][invSlotType]);
            totals++;
        }
    }
    format(msg, sizeof(msg), "Da load %d vat pham cua ban !", totals);
    SendClientMessageEx(playerid, COLOR_INFO, msg);
    return 1;
}

hook OnPlayerConnect(playerid) {
    InitInventoryTD(playerid);
    return 1;
}
hook OnPlayerSpawn(playerid) {
    if(gLoggedIn[playerid]) {
        LoadPlayerInventory(playerid);
    }
    return 1;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(InvOpen[playerid]) {
        // Button
        if(playertextid == InvGeneralTD[playerid][inv_btn_close]) {
            CloseInventoryTD(playerid);
            return 1;
        }
        // Button Navigation
        if(playertextid == InvGeneralTD[playerid][inv_nav_btn_left]) {
            return 1;
        }
        if(playertextid == InvGeneralTD[playerid][inv_nav_btn_right]) {
            return 1;
        }

        // Button 
        if(playertextid == InvInfoItemTD[playerid][inv_item_btn_use]) {
            return 1;
        }
        if(playertextid == InvInfoItemTD[playerid][inv_item_btn_give]) {
            return 1;
        }
        if(playertextid == InvInfoItemTD[playerid][inv_item_btn_drop]) {
            return 1;
        }

        /// Change BackgroundColor model
        for(new slotId = 0; slotId < MAX_INV_MAIN_SLOTS; slotId++)
        {
            if(playertextid == InvGridTD[playerid][slotId][inv_grid_model])
            {
                PlayerTextDrawBackgroundColour(playerid, InvGridTD[playerid][slotId][inv_grid_model], INV_COLOR_HOVER);
                PlayerTextDrawShow(playerid, InvGridTD[playerid][slotId][inv_grid_model]);

                ShowTDInvItemInfo(playerid, GetInvIndex(playerid, slotId, 0));

            } else {
                PlayerTextDrawBackgroundColour(playerid, InvGridTD[playerid][slotId][inv_grid_model], 0);
                PlayerTextDrawShow(playerid, InvGridTD[playerid][slotId][inv_grid_model]);
            }
        }

        for(new slotId = 0; slotId < MAX_INV_EQUIP_SLOTS; slotId++)
        {
            if(playertextid == InvEquipModel[playerid][slotId])
            {
                PlayerTextDrawBackgroundColour(playerid, InvEquipModel[playerid][slotId], INV_COLOR_HOVER);
                PlayerTextDrawShow(playerid, InvEquipModel[playerid][slotId]);

                ShowTDInvItemInfo(playerid, GetInvIndex(playerid, slotId, 1));
            } else {
                PlayerTextDrawBackgroundColour(playerid, InvEquipModel[playerid][slotId], 0);
                PlayerTextDrawShow(playerid, InvEquipModel[playerid][slotId]);
            }
        }
        return 1;
    }
	return 1;
}

stock GetInvIndex(playerid, slot_id, zone) {
    for(new i = 0; i < MAX_INV_SLOTS; i++) {
        if(PlayerInventory[playerid][i][invItemID] > 0) {
            if(slot_id == PlayerInventory[playerid][i][invSlotID] && zone == PlayerInventory[playerid][i][invSlotType]) return i;
        }
    }
    return -1;
}

stock SetInvGridValue(playerid) {
    new slot, item_id, model_id, amount;
    for(new idx = 0; idx < MAX_INV_SLOTS; idx++) {
        if(PlayerInventory[playerid][idx][invItemID] > 0) {
            item_id = GetItemIndex(PlayerInventory[playerid][idx][invItemID]);
            model_id = ItemData[item_id][itemModelID];
            slot = PlayerInventory[playerid][idx][invSlotID];
            amount = PlayerInventory[playerid][idx][invAmount];

            if(PlayerInventory[playerid][idx][invSlotType] == 0) {
                UpdateInvGridSlotTD(playerid, slot, model_id, amount);
            } 
            else if(PlayerInventory[playerid][idx][invSlotType] == 1) {
                UpdateInvEquipSlotTD(playerid, slot, model_id, 20.0);
            }
            
        }
    }
    return 1;
}

stock ShowTDInvItemInfo(playerid, inv_index = -1) {
    if(inv_index <= -1) return ShowInvInfoItemTD(playerid, false);
    new item_index = GetItemIndex(PlayerInventory[playerid][inv_index][invItemID]);

    SetInvInfoItemTD(
        playerid, 
        ItemData[item_index][itemModelID],
        ItemData[item_index][itemName],
        ItemData[item_index][itemType],
        ItemData[item_index][itemWeight],
        PlayerInventory[playerid][inv_index][invAmount],
        ItemData[item_index][itemDescription]
    );
    return 1;
}

CMD:inv(playerid, params[]) {
    if(!InvOpen[playerid]) {
        UpdateInvHeader(playerid, "Inventory", GetPlayerNameEx(playerid), "24.0kg / ~g~40.0kg");
        PlayerTextDrawSetPreviewModel(playerid, InvEquipCharacters[playerid], GetPlayerSkin(playerid));
        SetInvGridValue(playerid);
        OpenInventoryTD(playerid);
    } else {
        CloseInventoryTD(playerid);
    }
    return 1;
}