#include <YSI_Coding\y_hooks>

#define INVENTORY_ROWS  5
#define INVENTORY_COLS  5
#define TOTAL_SLOTS     (INVENTORY_ROWS * INVENTORY_COLS)

#define INV_EQUIP_SLOTS 16

#define INV_COLOR_BG    0x232323FF // 0x0A0A0AFF
#define INV_COLOR_BOX   0x0A0A0AFF
#define INV_COLOR_ITEM  0x1F1F1FFF
#define INV_COLOR_HOVER 0x2D2D2DFF
#define INV_COLOR_BUTTON 0x00A8E8FF

new bool:InvOpen[MAX_PLAYERS];

// General
enum e_inv_td_general {
    PlayerText:inv_bgMain,
    PlayerText:inv_bgGridItem,
    PlayerText:inv_bgEquip,
    PlayerText:inv_btn_close,

    PlayerText:inv_bgHeader,
    PlayerText:inv_header_title,
    PlayerText:inv_header_player,
    PlayerText:inv_header_stats,

    PlayerText:inv_bgNavigation,
    PlayerText:inv_nav_btn_left,
    PlayerText:inv_nav_page,
    PlayerText:inv_nav_btn_right
};
new InvGeneralTD[MAX_PLAYERS][e_inv_td_general];

// Grid Main
enum e_inv_grid {
    PlayerText:inv_grid_box,
    PlayerText:inv_grid_model,
    PlayerText:inv_grid_text
};
new InvGridTD[MAX_PLAYERS][TOTAL_SLOTS][e_inv_grid];

//Grid Equip
new PlayerText:InvEquipCharacters[MAX_PLAYERS];
new PlayerBar:InvEquipBar[MAX_PLAYERS][INV_EQUIP_SLOTS];
new PlayerText:InvEquipBox[MAX_PLAYERS][INV_EQUIP_SLOTS];
new PlayerText:InvEquipModel[MAX_PLAYERS][INV_EQUIP_SLOTS];

// Info Item
enum e_inv_info_item {
    PlayerText:inv_item_background,
    PlayerText:inv_item_model,
    PlayerText:inv_item_name,
    PlayerText:inv_item_type,
    PlayerText:inv_item_weight,
    PlayerText:inv_item_amount,
    PlayerText:inv_item_description,

    PlayerText:inv_item_btn_use,
    PlayerText:inv_item_btn_give,
    PlayerText:inv_item_btn_drop
};
new InvInfoItemTD[MAX_PLAYERS][e_inv_info_item];



stock InitInventoryTD(playerid) {
    InitInvGeneralTD(playerid);
    InitInvGrid(playerid);
    InitInvInfoItem(playerid);
    InitInvEquipTD(playerid);
    InvOpen[playerid] = false;
    return 1;
}

stock OpenInventoryTD(playerid)
{
    ShowInventoryTD(playerid);
    InvOpen[playerid] = true;
    SelectTextDraw(playerid, INV_COLOR_HOVER);
    return 1;
}

stock CloseInventoryTD(playerid)
{
    HideInventoryTD(playerid);
    InvOpen[playerid] = false;
    CancelSelectTextDraw(playerid);
    return 1;
}

stock InitInvGeneralTD(playerid) {
    InvGeneralTD[playerid][inv_bgMain] = CreatePlayerTextDraw(playerid, 492.5, 246.0, "_");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_bgMain], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_bgMain], 0.6, 20.8);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_bgMain], 298.5, 255.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_bgMain], 1);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_bgMain], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_bgMain], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_bgMain], -1);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_bgMain], 255);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_bgMain], INV_COLOR_BG);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_bgMain], true);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_bgMain], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_bgMain], false);

    InvGeneralTD[playerid][inv_bgEquip] = CreatePlayerTextDraw(playerid, 580.0, 263.0, "_");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_bgEquip], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_bgEquip], 0.6, 18.9);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_bgEquip], 298.5, 80.0);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_bgEquip], 1);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_bgEquip], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_bgEquip], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_bgEquip], -1);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_bgEquip], 255);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_bgEquip], INV_COLOR_BOX);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_bgEquip], true);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_bgEquip], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_bgEquip], false);

    InvGeneralTD[playerid][inv_bgGridItem] = CreatePlayerTextDraw(playerid, 450.0, 263.0, "_");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_bgGridItem], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_bgGridItem], 0.6, 18.9);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_bgGridItem], 298.5, 170.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_bgGridItem], 1);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_bgGridItem], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_bgGridItem], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_bgGridItem], -1);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_bgGridItem], 255);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_bgGridItem], INV_COLOR_BOX);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_bgGridItem], true);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_bgGridItem], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_bgGridItem], false);

    InvGeneralTD[playerid][inv_btn_close] = CreatePlayerTextDraw(playerid, 350.0, 245.5, "ld_beat:cross");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_btn_close], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_btn_close], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_btn_close], 11.5, 13.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_btn_close], 1);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_btn_close], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_btn_close], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_btn_close], INV_COLOR_BUTTON);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_btn_close], -1094795776);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_btn_close], 65280);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_btn_close], true);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_btn_close], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_btn_close], true);

    // Header
    InvGeneralTD[playerid][inv_bgHeader] = CreatePlayerTextDraw(playerid, 492.5, 246.0, "_");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_bgHeader], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_bgHeader], 0.6, 1.4);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_bgHeader], 298.5, 255.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_bgHeader], 1);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_bgHeader], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_bgHeader], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_bgHeader], -1);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_bgHeader], 255);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_bgHeader], INV_COLOR_BOX);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_bgHeader], true);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_bgHeader], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_bgHeader], false);

    InvGeneralTD[playerid][inv_header_title] = CreatePlayerTextDraw(playerid, 365.0, 246.0, "inventory");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_header_title], TEXT_DRAW_FONT:2);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_header_title], 0.2, 1.2);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_header_title], 425.0, 0.0);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_header_title], 0);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_header_title], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_header_title], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_header_title], -1);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_header_title], 255);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_header_title], 50);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_header_title], false);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_header_title], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_header_title], false);

    InvGeneralTD[playerid][inv_header_player] = CreatePlayerTextDraw(playerid, 493.0, 246.0, "Player_Name");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_header_player], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_header_player], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_header_player], 425.5, 89.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_header_player], 0);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_header_player], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_header_player], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_header_player], -1);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_header_player], 255);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_header_player], 50);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_header_player], false);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_header_player], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_header_player], false);

    InvGeneralTD[playerid][inv_header_stats] = CreatePlayerTextDraw(playerid, 619.0, 248.0, "24.0kg / ~g~40.0kg");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_header_stats], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_header_stats], 0.1, 1.0);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_header_stats], 425.5, 89.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_header_stats], 0);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_header_stats], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_header_stats], TEXT_DRAW_ALIGN:3);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_header_stats], -1);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_header_stats], 255);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_header_stats], 50);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_header_stats], false);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_header_stats], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_header_stats], false);

    // Navigation
    InvGeneralTD[playerid][inv_bgNavigation] = CreatePlayerTextDraw(playerid, 450.0, 433.0, "_");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_bgNavigation], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_bgNavigation], 0.6, 0.3);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_bgNavigation], 298.5, 34.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_bgNavigation], 1);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_bgNavigation], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_bgNavigation], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_bgNavigation], -1);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_bgNavigation], 255);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_bgNavigation], INV_COLOR_BOX);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_bgNavigation], true);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_bgNavigation], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_bgNavigation], false);

    InvGeneralTD[playerid][inv_nav_btn_left] = CreatePlayerTextDraw(playerid, 432.0, 431.0, "ld_beat:left");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_nav_btn_left], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_nav_btn_left], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_nav_btn_left], 7.5, 7.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_nav_btn_left], 1);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_nav_btn_left], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_nav_btn_left], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_nav_btn_left], INV_COLOR_BUTTON);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_nav_btn_left], -1094795776);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_nav_btn_left], 65280);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_nav_btn_left], true);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_nav_btn_left], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_nav_btn_left], true);

    InvGeneralTD[playerid][inv_nav_page] = CreatePlayerTextDraw(playerid, 450.0, 432.5, "1 /~r~100");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_nav_page], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_nav_page], 0.1, 0.4);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_nav_page], 459.0, 17.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_nav_page], 0);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_nav_page], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_nav_page], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_nav_page], -1);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_nav_page], 255);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_nav_page], -421070286);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_nav_page], false);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_nav_page], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_nav_page], false);

    InvGeneralTD[playerid][inv_nav_btn_right] = CreatePlayerTextDraw(playerid, 461.0, 431.0, "ld_beat:right");
    PlayerTextDrawFont(playerid, InvGeneralTD[playerid][inv_nav_btn_right], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, InvGeneralTD[playerid][inv_nav_btn_right], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, InvGeneralTD[playerid][inv_nav_btn_right], 7.5, 7.5);
    PlayerTextDrawSetOutline(playerid, InvGeneralTD[playerid][inv_nav_btn_right], 1);
    PlayerTextDrawSetShadow(playerid, InvGeneralTD[playerid][inv_nav_btn_right], 0);
    PlayerTextDrawAlignment(playerid, InvGeneralTD[playerid][inv_nav_btn_right], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvGeneralTD[playerid][inv_nav_btn_right], INV_COLOR_BUTTON);
    PlayerTextDrawBackgroundColour(playerid, InvGeneralTD[playerid][inv_nav_btn_right], -1094795776);
    PlayerTextDrawBoxColour(playerid, InvGeneralTD[playerid][inv_nav_btn_right], 65280);
    PlayerTextDrawUseBox(playerid, InvGeneralTD[playerid][inv_nav_btn_right], true);
    PlayerTextDrawSetProportional(playerid, InvGeneralTD[playerid][inv_nav_btn_right], true);
    PlayerTextDrawSetSelectable(playerid, InvGeneralTD[playerid][inv_nav_btn_right], true);

    return 1;
}

stock InitInvGrid(playerid)
{
    // Tọa độ bắt đầu (Góc trên bên trái của lưới $5 \times 5$)
    new Float:startX = 364.75 + 4.25; 
    new Float:startY = 263.0 + 3.5;    
    
    new Float:slotSize = 28.0;         // Kích thước ô vuông
    new Float:spacing = 5.5;           // Khoảng cách giữa các ô
    
    new slotId = 0;
    
    for(new r = 0; r < INVENTORY_ROWS; r++)
    {
        for(new c = 0; c < INVENTORY_COLS; c++)
        {
            new Float:posX = startX + (c * (slotSize + spacing));
            new Float:posY = startY + (r * (slotSize + spacing));
           
            InvGridTD[playerid][slotId][inv_grid_box] = CreatePlayerTextDraw(playerid, posX, posY, "_");
            PlayerTextDrawFont(playerid, InvGridTD[playerid][slotId][inv_grid_box], TEXT_DRAW_FONT:1);
            PlayerTextDrawLetterSize(playerid, InvGridTD[playerid][slotId][inv_grid_box], 0.0, (slotSize / 9.5)); 
            PlayerTextDrawTextSize(playerid, InvGridTD[playerid][slotId][inv_grid_box], posX + slotSize, 0.0);
            PlayerTextDrawAlignment(playerid, InvGridTD[playerid][slotId][inv_grid_box], TEXT_DRAW_ALIGN:1);
            PlayerTextDrawBoxColour(playerid, InvGridTD[playerid][slotId][inv_grid_box], INV_COLOR_ITEM);
            PlayerTextDrawUseBox(playerid, InvGridTD[playerid][slotId][inv_grid_box], true);
            
            InvGridTD[playerid][slotId][inv_grid_model] = CreatePlayerTextDraw(playerid, posX, posY, "_");
            PlayerTextDrawFont(playerid, InvGridTD[playerid][slotId][inv_grid_model], TEXT_DRAW_FONT:5);
            PlayerTextDrawLetterSize(playerid, InvGridTD[playerid][slotId][inv_grid_model], 0.0, (slotSize / 9.5));
            PlayerTextDrawTextSize(playerid, InvGridTD[playerid][slotId][inv_grid_model], slotSize, slotSize);
            PlayerTextDrawAlignment(playerid, InvGridTD[playerid][slotId][inv_grid_model], TEXT_DRAW_ALIGN:1);
            PlayerTextDrawColour(playerid, InvGridTD[playerid][slotId][inv_grid_model], -1);
            PlayerTextDrawBackgroundColour(playerid, InvGridTD[playerid][slotId][inv_grid_model], 0);
            PlayerTextDrawSetSelectable(playerid, InvGridTD[playerid][slotId][inv_grid_model], true);
            PlayerTextDrawSetPreviewModel(playerid, InvGridTD[playerid][slotId][inv_grid_model], 19300); 
            
            InvGridTD[playerid][slotId][inv_grid_text] = CreatePlayerTextDraw(playerid, (posX + slotSize - 2.0), (posY + slotSize - 8.0), "22");
            PlayerTextDrawFont(playerid, InvGridTD[playerid][slotId][inv_grid_text], TEXT_DRAW_FONT:1);
            PlayerTextDrawLetterSize(playerid, InvGridTD[playerid][slotId][inv_grid_text], 0.16, 0.7);
            PlayerTextDrawAlignment(playerid, InvGridTD[playerid][slotId][inv_grid_text], TEXT_DRAW_ALIGN:3);
            PlayerTextDrawColour(playerid, InvGridTD[playerid][slotId][inv_grid_text], -1); 
            PlayerTextDrawSetShadow(playerid, InvGridTD[playerid][slotId][inv_grid_text], 0);
            
            slotId++;
        }
    }

    return 1;
}

stock InitInvInfoItem(playerid) {
    InvInfoItemTD[playerid][inv_item_background] = CreatePlayerTextDraw(playerid, 310.0, 263.0, "_");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_background], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_background], 0.6, 18.9);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_background], 298.5, 100.0);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_background], 1);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_background], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_background], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_background], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_background], 255);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_background], INV_COLOR_BG);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_background], true);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_background], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_background], false);

    InvInfoItemTD[playerid][inv_item_model] = CreatePlayerTextDraw(playerid, 277.0, 262.0, "HUD:radar_burgershot");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_model], TEXT_DRAW_FONT:5);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_model], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_model], 66.0, 62.5);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_model], 1);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_model], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_model], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_model], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_model], 0);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_model], -206);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_model], true);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_model], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_model], false);
    PlayerTextDrawSetPreviewModel(playerid, InvInfoItemTD[playerid][inv_item_model], 2703);
    PlayerTextDrawSetPreviewRot(playerid, InvInfoItemTD[playerid][inv_item_model], 248.0, 0.0, -15.0, 1.0);
    PlayerTextDrawSetPreviewVehicleColours(playerid, InvInfoItemTD[playerid][inv_item_model], 1, 1);

    InvInfoItemTD[playerid][inv_item_name] = CreatePlayerTextDraw(playerid, 265.0, 324.0, "Name: ~y~Burger");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_name], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_name], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_name], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_name], 0);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_name], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_name], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_name], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_name], 255);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_name], -206);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_name], false);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_name], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_name], false);

    InvInfoItemTD[playerid][inv_item_type] = CreatePlayerTextDraw(playerid, 265.0, 333.0, "Category: ~b~Food");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_type], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_type], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_type], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_type], 0);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_type], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_type], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_type], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_type], 255);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_type], -206);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_type], false);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_type], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_type], false);

    InvInfoItemTD[playerid][inv_item_weight] = CreatePlayerTextDraw(playerid, 265.0, 343.0, "Weight: ~g~0.2kg");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_weight], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_weight], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_weight], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_weight], 0);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_weight], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_weight], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_weight], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_weight], 255);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_weight], -206);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_weight], false);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_weight], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_weight], false);

    InvInfoItemTD[playerid][inv_item_amount] = CreatePlayerTextDraw(playerid, 265.0, 352.0, "Quantity: ~r~5");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_amount], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_amount], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_amount], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_amount], 0);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_amount], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_amount], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_amount], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_amount], 255);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_amount], -206);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_amount], false);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_amount], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_amount], false);

    InvInfoItemTD[playerid][inv_item_description] = CreatePlayerTextDraw(playerid, 265.0, 364.0, "~b~Description: ~w~A basic cooked burger. Restores hunger when consumed.");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_description], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_description], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_description], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_description], 0);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_description], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_description], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_description], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_description], 255);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_description], -206);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_description], false);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_description], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_description], false);

    // Button
    InvInfoItemTD[playerid][inv_item_btn_use] = CreatePlayerTextDraw(playerid, 275.0, 423.5, "Use");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_btn_use], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_btn_use], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_btn_use], 9.0, 30.0);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_btn_use], 0);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_btn_use], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_btn_use], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_btn_use], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_btn_use], 255);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_btn_use], INV_COLOR_BUTTON);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_btn_use], true);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_btn_use], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_btn_use], true);

    InvInfoItemTD[playerid][inv_item_btn_give] = CreatePlayerTextDraw(playerid, 310.0, 423.5, "Give");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_btn_give], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_btn_give], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_btn_give], 9.0, 32.0);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_btn_give], 0);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_btn_give], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_btn_give], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_btn_give], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_btn_give], 255);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_btn_give], INV_COLOR_BUTTON);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_btn_give], true);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_btn_give], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_btn_give], true);

    InvInfoItemTD[playerid][inv_item_btn_drop] = CreatePlayerTextDraw(playerid, 345.0, 423.5, "Drop");
    PlayerTextDrawFont(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], 9.0, 30.0);
    PlayerTextDrawSetOutline(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], 0);
    PlayerTextDrawSetShadow(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], 0);
    PlayerTextDrawAlignment(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], -1);
    PlayerTextDrawBackgroundColour(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], 255);
    PlayerTextDrawBoxColour(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], INV_COLOR_BUTTON);
    PlayerTextDrawUseBox(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], true);
    PlayerTextDrawSetProportional(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], true);
    PlayerTextDrawSetSelectable(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], true);

    return 1;
}

stock InitInvEquipTD(playerid) {
    InvEquipCharacters[playerid] = CreatePlayerTextDraw(playerid, 537.5, 268.0, "_");
    PlayerTextDrawFont(playerid, InvEquipCharacters[playerid], TEXT_DRAW_FONT:5);
    PlayerTextDrawLetterSize(playerid, InvEquipCharacters[playerid], 0.0, (80.0 / 9.5)); 
    PlayerTextDrawTextSize(playerid, InvEquipCharacters[playerid], 80.0, 80.0);
    PlayerTextDrawAlignment(playerid, InvEquipCharacters[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawBoxColour(playerid, InvEquipCharacters[playerid], 0);
    PlayerTextDrawBackgroundColour(playerid, InvEquipCharacters[playerid], 0);
    PlayerTextDrawUseBox(playerid, InvEquipCharacters[playerid], true);
    PlayerTextDrawColour(playerid, InvEquipCharacters[playerid], -1);
    PlayerTextDrawSetPreviewModel(playerid, InvEquipCharacters[playerid], GetPlayerSkin(playerid)); 
    PlayerTextDrawSetPreviewRot(playerid, InvEquipCharacters[playerid], -10.0, 0.0, -20.0, 1.0);

    // Cac o chua Helmet, Armour, ...
    new Float:topSlotsX[4] = {543.0, 543.0, 600.0, 600.0};
    new Float:topSlotsY[4] = {275.0, 305.0, 275.0, 305.0};
    new Float:width = 16.0;

    for(new i = 0; i < 4; i++)
    {
        InitInvSingleEquipSlotsTD(playerid, i, topSlotsX[i], topSlotsY[i], width);
    }

    // Cac o chua weapon , ...
    new Float:bottomStartX = 540.5;
    new Float:bottomStartY = 354.0;
    new Float:spacingX = 5.0;
    new Float:spacingY = 12.0;
    
    new slotId = 4;
    for(new r = 0; r < 3; r++) 
    {
        for(new c = 0; c < 4; c++)
        {
            new Float:posX = bottomStartX + (c * (width + spacingX));
            new Float:posY = bottomStartY + (r * (width + spacingY));
            
            InitInvSingleEquipSlotsTD(playerid, slotId, posX, posY, width);
            slotId++;
        }
    }
    return 1;
}

stock InitInvSingleEquipSlotsTD(playerid, slotId, Float:posX, Float:posY, Float:width)
{
    InvEquipBar[playerid][slotId] = CreatePlayerProgressBar(playerid, posX, posY + width - 0.5, width, 1.0);
    SetPlayerProgressBarValue(playerid, InvEquipBar[playerid][slotId], 30.0);
    
    InvEquipBox[playerid][slotId] = CreatePlayerTextDraw(playerid, posX, posY, "_");
    PlayerTextDrawFont(playerid, InvEquipBox[playerid][slotId], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, InvEquipBox[playerid][slotId], 0.0, (width / 9.5));
    PlayerTextDrawTextSize(playerid, InvEquipBox[playerid][slotId], posX + width, 0.0);
    PlayerTextDrawAlignment(playerid, InvEquipBox[playerid][slotId], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawBoxColour(playerid, InvEquipBox[playerid][slotId], INV_COLOR_ITEM);
    PlayerTextDrawBackgroundColour(playerid, InvEquipBox[playerid][slotId], 255);
    PlayerTextDrawUseBox(playerid, InvEquipBox[playerid][slotId], true);

    InvEquipModel[playerid][slotId] = CreatePlayerTextDraw(playerid, posX, posY, "_");
    PlayerTextDrawFont(playerid, InvEquipModel[playerid][slotId], TEXT_DRAW_FONT:5);
    PlayerTextDrawLetterSize(playerid, InvEquipModel[playerid][slotId], 0.0, (width / 9.5));
    PlayerTextDrawTextSize(playerid, InvEquipModel[playerid][slotId], width, width);
    PlayerTextDrawAlignment(playerid, InvEquipModel[playerid][slotId], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawBackgroundColour(playerid, InvEquipModel[playerid][slotId], 0);
    PlayerTextDrawColour(playerid, InvEquipModel[playerid][slotId], -1);
    PlayerTextDrawSetSelectable(playerid, InvEquipModel[playerid][slotId], true);
    // PlayerTextDrawSetPreviewModel(playerid, InvEquipModel[playerid][slotId], 0);

    return 1;
}

stock ShowInventoryTD(playerid)
{
    ShowInvGeneralTD(playerid, true);
    ShowInvGridTD(playerid, true);
    ShowInvInfoItemTD(playerid, true);
    ShowInvEquipTD(playerid, true);
    return 1;
}

stock HideInventoryTD(playerid)
{
    ShowInvGeneralTD(playerid, false);
    ShowInvGridTD(playerid, false);
    ShowInvInfoItemTD(playerid, false);
    ShowInvEquipTD(playerid, false);
    return 1;
}

stock ShowInvGeneralTD(playerid, bool:show)
{
    if(show) { 
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_bgMain]);
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_bgGridItem]);
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_bgEquip]);
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_btn_close]);

        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_bgHeader]);
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_header_title]);
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_header_player]);
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_header_stats]);

        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_bgNavigation]);
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_nav_btn_left]);
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_nav_page]);
        PlayerTextDrawShow(playerid, InvGeneralTD[playerid][inv_nav_btn_right]);
    }
    else {
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_bgMain]);
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_bgGridItem]);
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_bgEquip]);
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_btn_close]);

        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_bgHeader]);
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_header_title]);
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_header_player]);
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_header_stats]);

        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_bgNavigation]);
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_nav_btn_left]);
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_nav_page]);
        PlayerTextDrawHide(playerid, InvGeneralTD[playerid][inv_nav_btn_right]);
    }
    return 1;
}

stock ShowInvGridTD(playerid, bool:show)
{
    new slotId;
    for(slotId = 0; slotId < TOTAL_SLOTS; slotId++) {
        if(show) {
            PlayerTextDrawShow(playerid, InvGridTD[playerid][slotId][inv_grid_box]);
            PlayerTextDrawShow(playerid, InvGridTD[playerid][slotId][inv_grid_model]);
            PlayerTextDrawShow(playerid, InvGridTD[playerid][slotId][inv_grid_text]);
        } else {
            PlayerTextDrawHide(playerid, InvGridTD[playerid][slotId][inv_grid_box]);
            PlayerTextDrawHide(playerid, InvGridTD[playerid][slotId][inv_grid_model]);
            PlayerTextDrawHide(playerid, InvGridTD[playerid][slotId][inv_grid_text]);
        }
    }
    return 1;
}

stock ShowInvInfoItemTD(playerid, bool:show)
{
    
    if(show) {
        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_background]);
        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_model]);
        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_name]);
        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_type]);
        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_weight]);
        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_amount]);
        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_description]);

        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_btn_use]);
        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_btn_give]);
        PlayerTextDrawShow(playerid, InvInfoItemTD[playerid][inv_item_btn_drop]);
    }
    else {
        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_background]);
        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_model]);
        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_name]);
        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_type]);
        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_weight]);
        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_amount]);
        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_description]);

        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_btn_use]);
        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_btn_give]);
        PlayerTextDrawHide(playerid, InvInfoItemTD[playerid][inv_item_btn_drop]);
    }
    return 1;
}

stock ShowInvEquipTD(playerid, bool:show)
{
    new slotId;
    if(show) PlayerTextDrawShow(playerid, InvEquipCharacters[playerid]);
    else PlayerTextDrawHide(playerid, InvEquipCharacters[playerid]);

    for(slotId = 0; slotId < INV_EQUIP_SLOTS; slotId++) {
        if(show) {
            ShowPlayerProgressBar(playerid, InvEquipBar[playerid][slotId]);
            PlayerTextDrawShow(playerid, InvEquipBox[playerid][slotId]);
            PlayerTextDrawShow(playerid, InvEquipModel[playerid][slotId]);
        } else {
            HidePlayerProgressBar(playerid, InvEquipBar[playerid][slotId]);
            PlayerTextDrawHide(playerid, InvEquipBox[playerid][slotId]);
            PlayerTextDrawHide(playerid, InvEquipModel[playerid][slotId]);
        }
    }
    return 1;
}

stock SetInvInfoItemData(playerid, const itemName[], const itemType[], const itemWeight[], const itemAmount[], const itemDescription[])
{
    PlayerTextDrawSetString(playerid, InvInfoItemTD[playerid][inv_item_name], itemName);
    PlayerTextDrawSetString(playerid, InvInfoItemTD[playerid][inv_item_type], itemType);
    PlayerTextDrawSetString(playerid, InvInfoItemTD[playerid][inv_item_weight], itemWeight);
    PlayerTextDrawSetString(playerid, InvInfoItemTD[playerid][inv_item_amount], itemAmount);
    PlayerTextDrawSetString(playerid, InvInfoItemTD[playerid][inv_item_description], itemDescription);
    return 1;
}

stock SetInvInfoItemModel(playerid, modelid, Float:rotX, Float:rotY, Float:rotZ, Float:zoom)
{
    PlayerTextDrawSetPreviewModel(playerid, InvInfoItemTD[playerid][inv_item_model], modelid);
    PlayerTextDrawSetPreviewRot(playerid, InvInfoItemTD[playerid][inv_item_model], rotX, rotY, rotZ, zoom);
    return 1;
}

stock UpdateInvHeader(playerid, const title[], const playerName[], const statsText[])
{
    PlayerTextDrawSetString(playerid, InvGeneralTD[playerid][inv_header_title], title);
    PlayerTextDrawSetString(playerid, InvGeneralTD[playerid][inv_header_player], playerName);
    PlayerTextDrawSetString(playerid, InvGeneralTD[playerid][inv_header_stats], statsText);
    return 1;
}

stock SetInvNavPage(playerid, const pageText[])
{
    PlayerTextDrawSetString(playerid, InvGeneralTD[playerid][inv_nav_page], pageText);
    return 1;
}

stock SetInvButtonText(playerid, const useText[], const removeText[], const giveText[], const dropText[])
{
    PlayerTextDrawSetString(playerid, InvInfoItemTD[playerid][inv_item_btn_use], useText);
    PlayerTextDrawSetString(playerid, InvInfoItemTD[playerid][inv_item_btn_give], giveText);
    PlayerTextDrawSetString(playerid, InvInfoItemTD[playerid][inv_item_btn_drop], dropText);
    return 1;
}

stock SetInvEquipPreview(playerid, modelid, Float:rotX, Float:rotY, Float:rotZ, Float:zoom)
{
    PlayerTextDrawSetPreviewModel(playerid, InvEquipCharacters[playerid], modelid);
    PlayerTextDrawSetPreviewRot(playerid, InvEquipCharacters[playerid], rotX, rotY, rotZ, zoom);
    return 1;
}

stock SetInvEquipSlotModel(playerid, slotId, modelid)
{
    PlayerTextDrawSetPreviewModel(playerid, InvEquipModel[playerid][slotId], modelid);
    return 1;
}

stock SetInvEquipSlotBarValue(playerid, slotId, Float:value)
{
    SetPlayerProgressBarValue(playerid, InvEquipBar[playerid][slotId], value);
    return 1;
}

stock UpdateInvGridSlotText(playerid, slotId, const text[])
{
    PlayerTextDrawSetString(playerid, InvGridTD[playerid][slotId][inv_grid_text], text);
    return 1;
}

stock UpdateInvGridSlotModel(playerid, slotId, const modelText[])
{
    PlayerTextDrawSetString(playerid, InvGridTD[playerid][slotId][inv_grid_model], modelText);
    return 1;
}


hook OnPlayerConnect(playerid) {
    InitInventoryTD(playerid);
    return 1;
}

CMD:inv(playerid, params[]) {
    if(!InvOpen[playerid]) {
        OpenInventoryTD(playerid);
    } else {
        CloseInventoryTD(playerid);
    }
    return 1;
}