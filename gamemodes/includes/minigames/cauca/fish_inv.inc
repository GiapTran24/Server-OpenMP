#include <YSI_Coding\y_hooks>

#define FISH_TD_ROWS  5
#define FISH_TD_COLS  5
#define MAX_FISH_SLOTS     (FISH_TD_ROWS * FISH_TD_COLS)
#define FISH_MODEL_DEFAULT 19300
#define MODEL_FISHING_ROD 18632
#define MAX_FISH_ROD_LEVEL 10

#define FISH_COLOR_TD_BG    0x232323FF
#define FISH_COLOR_TD_BOX   0x0A0A0AFF
#define FISH_COLOR_TD_ITEM  0x1F1F1FFF
#define FISH_COLOR_TD_HOVER 0xFFFFFF66
#define FISH_COLOR_TD_BUTTON 0x00A8E8FF

new bool:FishTdOpen[MAX_PLAYERS];
new FishSlotClicking[MAX_PLAYERS] = -1;

// General
enum e_fish_td_general {
    PlayerText:fish_bgMain,
    PlayerText:fish_bgGridItem,
    PlayerText:fish_btn_close,

    PlayerText:fish_bgHeader,
    PlayerText:fish_header_title,
    PlayerText:fish_header_player,

    PlayerText:fish_bgEquip
};
new FishGeneralTD[MAX_PLAYERS][e_fish_td_general];

// Grid Main
enum e_fish_td_grid {
    PlayerText:fish_grid_box,
    PlayerText:fish_grid_model,
    PlayerText:fish_grid_text
};
new FishGridTD[MAX_PLAYERS][MAX_FISH_SLOTS][e_fish_td_grid];

// Fish Equip
enum e_rod_td {
    PlayerText:rod_Model,
    PlayerText:rod_Title[3],
    PlayerText:rod_Type,
    PlayerText:rod_Level,
    PlayerText:rod_Exp,
    PlayerBar:rod_ExpBar
};
new FishRodTD[MAX_PLAYERS][e_rod_td];

// Info Item
enum e_fish_info_item {
    PlayerText:fish_item_background,
    PlayerText:fish_item_model,
    PlayerText:fish_item_name,
    PlayerText:fish_item_type,
    PlayerText:fish_item_weight,
    PlayerText:fish_item_amount,
    PlayerText:fish_item_description,

    PlayerText:fish_item_btn_sell,
    PlayerText:fish_item_btn_give,
    PlayerText:fish_item_btn_drop
};
new FishInfoItemTD[MAX_PLAYERS][e_fish_info_item];

// FishFight
enum e_td_fish_fight {
    PlayerText:fishFight_bg,
    PlayerText:fishFight_title,
    PlayerText:fishFight_name,
    PlayerText:fishFight_type,
    PlayerText:fishFight_help,
    
    PlayerBar:fishFight_progress
};
new FishFightTD[MAX_PLAYERS][e_td_fish_fight];


////////////////////////////Player Inv Data////////////////////////////////////////
enum e_player_fish
{
    pFishDB,
    pFishID,
    Float:pFishWeight,
    bool:pFishLoaded
};
new PlayerFishInv[MAX_PLAYERS][MAX_FISH_SLOTS][e_player_fish];


enum e_rod_type {
    ROD_COMMON,
    ROD_RARE,
    ROD_EPIC,
    ROD_LEGENDARY
};
enum e_player_rod {
    pRodDB,
    pRodType,
    pRodLevel,
    pRodExp
};
new PlayerFishRod[MAX_PLAYERS][e_player_rod];
/////////////////////////////////////////////////////////////////////////////////



stock InitFishGeneralTD(playerid) {
    FishGeneralTD[playerid][fish_bgMain] = CreatePlayerTextDraw(playerid, 492.5, 246.0, "_");
    PlayerTextDrawFont(playerid, FishGeneralTD[playerid][fish_bgMain], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishGeneralTD[playerid][fish_bgMain], 0.6, 20.8);
    PlayerTextDrawTextSize(playerid, FishGeneralTD[playerid][fish_bgMain], 298.5, 255.5);
    PlayerTextDrawSetOutline(playerid, FishGeneralTD[playerid][fish_bgMain], 1);
    PlayerTextDrawSetShadow(playerid, FishGeneralTD[playerid][fish_bgMain], 0);
    PlayerTextDrawAlignment(playerid, FishGeneralTD[playerid][fish_bgMain], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishGeneralTD[playerid][fish_bgMain], -1);
    PlayerTextDrawBackgroundColour(playerid, FishGeneralTD[playerid][fish_bgMain], 255);
    PlayerTextDrawBoxColour(playerid, FishGeneralTD[playerid][fish_bgMain], FISH_COLOR_TD_BG);
    PlayerTextDrawUseBox(playerid, FishGeneralTD[playerid][fish_bgMain], true);
    PlayerTextDrawSetProportional(playerid, FishGeneralTD[playerid][fish_bgMain], true);
    PlayerTextDrawSetSelectable(playerid, FishGeneralTD[playerid][fish_bgMain], false);

    FishGeneralTD[playerid][fish_bgGridItem] = CreatePlayerTextDraw(playerid, 450.0, 263.0, "_");
    PlayerTextDrawFont(playerid, FishGeneralTD[playerid][fish_bgGridItem], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishGeneralTD[playerid][fish_bgGridItem], 0.6, 18.9);
    PlayerTextDrawTextSize(playerid, FishGeneralTD[playerid][fish_bgGridItem], 298.5, 170.5);
    PlayerTextDrawSetOutline(playerid, FishGeneralTD[playerid][fish_bgGridItem], 1);
    PlayerTextDrawSetShadow(playerid, FishGeneralTD[playerid][fish_bgGridItem], 0);
    PlayerTextDrawAlignment(playerid, FishGeneralTD[playerid][fish_bgGridItem], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishGeneralTD[playerid][fish_bgGridItem], -1);
    PlayerTextDrawBackgroundColour(playerid, FishGeneralTD[playerid][fish_bgGridItem], 255);
    PlayerTextDrawBoxColour(playerid, FishGeneralTD[playerid][fish_bgGridItem], FISH_COLOR_TD_BOX);
    PlayerTextDrawUseBox(playerid, FishGeneralTD[playerid][fish_bgGridItem], true);
    PlayerTextDrawSetProportional(playerid, FishGeneralTD[playerid][fish_bgGridItem], true);
    PlayerTextDrawSetSelectable(playerid, FishGeneralTD[playerid][fish_bgGridItem], false);

    // Header
    FishGeneralTD[playerid][fish_bgHeader] = CreatePlayerTextDraw(playerid, 492.5, 246.0, "_");
    PlayerTextDrawFont(playerid, FishGeneralTD[playerid][fish_bgHeader], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishGeneralTD[playerid][fish_bgHeader], 0.6, 1.4);
    PlayerTextDrawTextSize(playerid, FishGeneralTD[playerid][fish_bgHeader], 298.5, 255.5);
    PlayerTextDrawSetOutline(playerid, FishGeneralTD[playerid][fish_bgHeader], 1);
    PlayerTextDrawSetShadow(playerid, FishGeneralTD[playerid][fish_bgHeader], 0);
    PlayerTextDrawAlignment(playerid, FishGeneralTD[playerid][fish_bgHeader], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishGeneralTD[playerid][fish_bgHeader], -1);
    PlayerTextDrawBackgroundColour(playerid, FishGeneralTD[playerid][fish_bgHeader], 255);
    PlayerTextDrawBoxColour(playerid, FishGeneralTD[playerid][fish_bgHeader], FISH_COLOR_TD_BOX);
    PlayerTextDrawUseBox(playerid, FishGeneralTD[playerid][fish_bgHeader], true);
    PlayerTextDrawSetProportional(playerid, FishGeneralTD[playerid][fish_bgHeader], true);
    PlayerTextDrawSetSelectable(playerid, FishGeneralTD[playerid][fish_bgHeader], false);

    FishGeneralTD[playerid][fish_header_title] = CreatePlayerTextDraw(playerid, 365.0, 246.0, "Tui Ca");
    PlayerTextDrawFont(playerid, FishGeneralTD[playerid][fish_header_title], TEXT_DRAW_FONT:2);
    PlayerTextDrawLetterSize(playerid, FishGeneralTD[playerid][fish_header_title], 0.2, 1.2);
    PlayerTextDrawTextSize(playerid, FishGeneralTD[playerid][fish_header_title], 425.0, 0.0);
    PlayerTextDrawSetOutline(playerid, FishGeneralTD[playerid][fish_header_title], 0);
    PlayerTextDrawSetShadow(playerid, FishGeneralTD[playerid][fish_header_title], 0);
    PlayerTextDrawAlignment(playerid, FishGeneralTD[playerid][fish_header_title], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, FishGeneralTD[playerid][fish_header_title], -1);
    PlayerTextDrawBackgroundColour(playerid, FishGeneralTD[playerid][fish_header_title], 255);
    PlayerTextDrawBoxColour(playerid, FishGeneralTD[playerid][fish_header_title], 50);
    PlayerTextDrawUseBox(playerid, FishGeneralTD[playerid][fish_header_title], false);
    PlayerTextDrawSetProportional(playerid, FishGeneralTD[playerid][fish_header_title], true);
    PlayerTextDrawSetSelectable(playerid, FishGeneralTD[playerid][fish_header_title], false);

    FishGeneralTD[playerid][fish_header_player] = CreatePlayerTextDraw(playerid, 493.0, 246.0, GetPlayerNameEx(playerid));
    PlayerTextDrawFont(playerid, FishGeneralTD[playerid][fish_header_player], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishGeneralTD[playerid][fish_header_player], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, FishGeneralTD[playerid][fish_header_player], 425.5, 89.5);
    PlayerTextDrawSetOutline(playerid, FishGeneralTD[playerid][fish_header_player], 0);
    PlayerTextDrawSetShadow(playerid, FishGeneralTD[playerid][fish_header_player], 0);
    PlayerTextDrawAlignment(playerid, FishGeneralTD[playerid][fish_header_player], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishGeneralTD[playerid][fish_header_player], -1);
    PlayerTextDrawBackgroundColour(playerid, FishGeneralTD[playerid][fish_header_player], 255);
    PlayerTextDrawBoxColour(playerid, FishGeneralTD[playerid][fish_header_player], 50);
    PlayerTextDrawUseBox(playerid, FishGeneralTD[playerid][fish_header_player], false);
    PlayerTextDrawSetProportional(playerid, FishGeneralTD[playerid][fish_header_player], true);
    PlayerTextDrawSetSelectable(playerid, FishGeneralTD[playerid][fish_header_player], false);

    FishGeneralTD[playerid][fish_bgEquip] = CreatePlayerTextDraw(playerid, 580.0, 263.0, "_");
    PlayerTextDrawFont(playerid, FishGeneralTD[playerid][fish_bgEquip], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishGeneralTD[playerid][fish_bgEquip], 0.6, 18.9);
    PlayerTextDrawTextSize(playerid, FishGeneralTD[playerid][fish_bgEquip], 298.5, 80.0);
    PlayerTextDrawSetOutline(playerid, FishGeneralTD[playerid][fish_bgEquip], 1);
    PlayerTextDrawSetShadow(playerid, FishGeneralTD[playerid][fish_bgEquip], 0);
    PlayerTextDrawAlignment(playerid, FishGeneralTD[playerid][fish_bgEquip], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishGeneralTD[playerid][fish_bgEquip], -1);
    PlayerTextDrawBackgroundColour(playerid, FishGeneralTD[playerid][fish_bgEquip], 255);
    PlayerTextDrawBoxColour(playerid, FishGeneralTD[playerid][fish_bgEquip], FISH_COLOR_TD_BOX);
    PlayerTextDrawUseBox(playerid, FishGeneralTD[playerid][fish_bgEquip], true);
    PlayerTextDrawSetProportional(playerid, FishGeneralTD[playerid][fish_bgEquip], true);
    PlayerTextDrawSetSelectable(playerid, FishGeneralTD[playerid][fish_bgEquip], false);

    FishGeneralTD[playerid][fish_btn_close] = CreatePlayerTextDraw(playerid, 609.5, 245.5, "ld_beat:cross");
    PlayerTextDrawFont(playerid, FishGeneralTD[playerid][fish_btn_close], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, FishGeneralTD[playerid][fish_btn_close], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, FishGeneralTD[playerid][fish_btn_close], 11.5, 13.5);
    PlayerTextDrawSetOutline(playerid, FishGeneralTD[playerid][fish_btn_close], 1);
    PlayerTextDrawSetShadow(playerid, FishGeneralTD[playerid][fish_btn_close], 0);
    PlayerTextDrawAlignment(playerid, FishGeneralTD[playerid][fish_btn_close], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, FishGeneralTD[playerid][fish_btn_close], FISH_COLOR_TD_BUTTON);
    PlayerTextDrawBackgroundColour(playerid, FishGeneralTD[playerid][fish_btn_close], -1094795776);
    PlayerTextDrawBoxColour(playerid, FishGeneralTD[playerid][fish_btn_close], 65280);
    PlayerTextDrawUseBox(playerid, FishGeneralTD[playerid][fish_btn_close], true);
    PlayerTextDrawSetProportional(playerid, FishGeneralTD[playerid][fish_btn_close], true);
    PlayerTextDrawSetSelectable(playerid, FishGeneralTD[playerid][fish_btn_close], true);

    return 1;
}

stock InitFishTdGrid(playerid)
{
    new Float:startX = 364.75 + 4.25; 
    new Float:startY = 263.0 + 3.5;    
    
    new Float:slotSize = 28.0;         
    new Float:spacing = 5.5;           
    
    new slotId = 0;
    
    for(new r = 0; r < FISH_TD_ROWS; r++)
    {
        for(new c = 0; c < FISH_TD_COLS; c++)
        {
            new Float:posX = startX + (c * (slotSize + spacing));
            new Float:posY = startY + (r * (slotSize + spacing));
           
            FishGridTD[playerid][slotId][fish_grid_box] = CreatePlayerTextDraw(playerid, posX, posY, "_");
            PlayerTextDrawFont(playerid, FishGridTD[playerid][slotId][fish_grid_box], TEXT_DRAW_FONT:1);
            PlayerTextDrawLetterSize(playerid, FishGridTD[playerid][slotId][fish_grid_box], 0.0, (slotSize / 9.5)); 
            PlayerTextDrawTextSize(playerid, FishGridTD[playerid][slotId][fish_grid_box], posX + slotSize, 0.0);
            PlayerTextDrawAlignment(playerid, FishGridTD[playerid][slotId][fish_grid_box], TEXT_DRAW_ALIGN:1);
            PlayerTextDrawBoxColour(playerid, FishGridTD[playerid][slotId][fish_grid_box], FISH_COLOR_TD_ITEM);
            PlayerTextDrawUseBox(playerid, FishGridTD[playerid][slotId][fish_grid_box], true);
            
            FishGridTD[playerid][slotId][fish_grid_text] = CreatePlayerTextDraw(playerid, (posX + slotSize - 2.0), (posY + slotSize - 8.0), "_");
            PlayerTextDrawFont(playerid, FishGridTD[playerid][slotId][fish_grid_text], TEXT_DRAW_FONT:1);
            PlayerTextDrawLetterSize(playerid, FishGridTD[playerid][slotId][fish_grid_text], 0.13, 0.6); //1.1, 0.7
            PlayerTextDrawAlignment(playerid, FishGridTD[playerid][slotId][fish_grid_text], TEXT_DRAW_ALIGN:3);
            PlayerTextDrawColour(playerid, FishGridTD[playerid][slotId][fish_grid_text], -1); 
            PlayerTextDrawSetShadow(playerid, FishGridTD[playerid][slotId][fish_grid_text], 0);

            FishGridTD[playerid][slotId][fish_grid_model] = CreatePlayerTextDraw(playerid, posX, posY, "_");
            PlayerTextDrawFont(playerid, FishGridTD[playerid][slotId][fish_grid_model], TEXT_DRAW_FONT:5);
            PlayerTextDrawLetterSize(playerid, FishGridTD[playerid][slotId][fish_grid_model], 0.0, (slotSize / 9.5));
            PlayerTextDrawTextSize(playerid, FishGridTD[playerid][slotId][fish_grid_model], slotSize, slotSize);
            PlayerTextDrawAlignment(playerid, FishGridTD[playerid][slotId][fish_grid_model], TEXT_DRAW_ALIGN:1);
            PlayerTextDrawColour(playerid, FishGridTD[playerid][slotId][fish_grid_model], -1);
            PlayerTextDrawBackgroundColour(playerid, FishGridTD[playerid][slotId][fish_grid_model], 0);
            PlayerTextDrawSetSelectable(playerid, FishGridTD[playerid][slotId][fish_grid_model], true);
            PlayerTextDrawSetPreviewModel(playerid, FishGridTD[playerid][slotId][fish_grid_model], FISH_MODEL_DEFAULT); 
            
            slotId++;
        }
    }

    return 1;
}

stock InitFishInfoItem(playerid) {
    FishInfoItemTD[playerid][fish_item_background] = CreatePlayerTextDraw(playerid, 310.0, 263.0, "_");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_background], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_background], 0.6, 18.9);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_background], 298.5, 100.0);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_background], 1);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_background], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_background], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_background], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_background], 255);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_background], FISH_COLOR_TD_ITEM);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_background], true);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_background], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_background], false);

    FishInfoItemTD[playerid][fish_item_model] = CreatePlayerTextDraw(playerid, 277.0, 262.0, "HUD:radar_burgershot");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_model], TEXT_DRAW_FONT:5);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_model], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_model], 66.0, 62.5);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_model], 1);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_model], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_model], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_model], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_model], 0);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_model], -206);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_model], true);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_model], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_model], false);
    PlayerTextDrawSetPreviewModel(playerid, FishInfoItemTD[playerid][fish_item_model], 2932);

    FishInfoItemTD[playerid][fish_item_name] = CreatePlayerTextDraw(playerid, 265.0, 324.0, "Name: ~y~Burger");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_name], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_name], 0.15, 0.8);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_name], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_name], 0);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_name], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_name], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_name], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_name], 255);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_name], -206);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_name], false);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_name], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_name], false);

    FishInfoItemTD[playerid][fish_item_type] = CreatePlayerTextDraw(playerid, 265.0, 333.0, "Category: ~b~Food");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_type], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_type], 0.15, 0.8);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_type], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_type], 0);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_type], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_type], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_type], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_type], 255);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_type], -206);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_type], false);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_type], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_type], false);

    FishInfoItemTD[playerid][fish_item_weight] = CreatePlayerTextDraw(playerid, 265.0, 343.0, "Weight: ~g~0.2kg");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_weight], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_weight], 0.15, 0.8);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_weight], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_weight], 0);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_weight], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_weight], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_weight], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_weight], 255);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_weight], -206);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_weight], false);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_weight], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_weight], false);

    FishInfoItemTD[playerid][fish_item_amount] = CreatePlayerTextDraw(playerid, 265.0, 352.0, "Quantity: ~r~5");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_amount], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_amount], 0.15, 0.8);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_amount], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_amount], 0);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_amount], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_amount], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_amount], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_amount], 255);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_amount], -206);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_amount], false);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_amount], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_amount], false);

    FishInfoItemTD[playerid][fish_item_description] = CreatePlayerTextDraw(playerid, 265.0, 364.0, "~b~Description: ~w~A basic cooked burger. Restores hunger when consumed.");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_description], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_description], 0.15, 0.8);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_description], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_description], 0);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_description], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_description], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_description], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_description], 255);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_description], -206);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_description], false);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_description], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_description], false);

    // Button
    FishInfoItemTD[playerid][fish_item_btn_sell] = CreatePlayerTextDraw(playerid, 275.0, 423.5, "Sell");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], 9.0, 30.0);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], 0);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], 255);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], FISH_COLOR_TD_BUTTON);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], true);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_btn_sell], true);

    FishInfoItemTD[playerid][fish_item_btn_give] = CreatePlayerTextDraw(playerid, 310.0, 423.5, "Give");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_btn_give], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_btn_give], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_btn_give], 9.0, 32.0);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_btn_give], 0);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_btn_give], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_btn_give], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_btn_give], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_btn_give], 255);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_btn_give], FISH_COLOR_TD_BUTTON);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_btn_give], true);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_btn_give], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_btn_give], true);

    FishInfoItemTD[playerid][fish_item_btn_drop] = CreatePlayerTextDraw(playerid, 345.0, 423.5, "Drop");
    PlayerTextDrawFont(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], 9.0, 30.0);
    PlayerTextDrawSetOutline(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], 0);
    PlayerTextDrawSetShadow(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], 0);
    PlayerTextDrawAlignment(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], -1);
    PlayerTextDrawBackgroundColour(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], 255);
    PlayerTextDrawBoxColour(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], FISH_COLOR_TD_BUTTON);
    PlayerTextDrawUseBox(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], true);
    PlayerTextDrawSetProportional(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], true);
    PlayerTextDrawSetSelectable(playerid, FishInfoItemTD[playerid][fish_item_btn_drop], true);

    return 1;
}

stock InitFishEquipTD(playerid) {
    FishRodTD[playerid][rod_Model] = CreatePlayerTextDraw(playerid, 496.000, 259.000, "Preview_Model");
    PlayerTextDrawLetterSize(playerid, FishRodTD[playerid][rod_Model], 0.600, 2.000);
    PlayerTextDrawTextSize(playerid, FishRodTD[playerid][rod_Model], 113.000, 172.000);
    PlayerTextDrawAlignment(playerid, FishRodTD[playerid][rod_Model], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, FishRodTD[playerid][rod_Model], -1);
    PlayerTextDrawSetShadow(playerid, FishRodTD[playerid][rod_Model], 0);
    PlayerTextDrawSetOutline(playerid, FishRodTD[playerid][rod_Model], 0);
    PlayerTextDrawBackgroundColour(playerid, FishRodTD[playerid][rod_Model], 0);
    PlayerTextDrawFont(playerid, FishRodTD[playerid][rod_Model], TEXT_DRAW_FONT_MODEL_PREVIEW);
    PlayerTextDrawSetProportional(playerid, FishRodTD[playerid][rod_Model], true);
    PlayerTextDrawSetPreviewModel(playerid, FishRodTD[playerid][rod_Model], 18632);
    PlayerTextDrawSetPreviewRot(playerid, FishRodTD[playerid][rod_Model], 42.000, 15.000, -45.000, 0.839);
    PlayerTextDrawSetPreviewVehicleColours(playerid, FishRodTD[playerid][rod_Model], 1, 1);
        
    FishRodTD[playerid][rod_Title][0] = CreatePlayerTextDraw(playerid, 540.000, 381.000, "Can cau");
    PlayerTextDrawLetterSize(playerid, FishRodTD[playerid][rod_Title][0], 0.158, 1.000);
    PlayerTextDrawTextSize(playerid, FishRodTD[playerid][rod_Title][0], 563.500, 19.500);
    PlayerTextDrawAlignment(playerid, FishRodTD[playerid][rod_Title][0], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, FishRodTD[playerid][rod_Title][0], -1);
    PlayerTextDrawUseBox(playerid, FishRodTD[playerid][rod_Title][0], false);
    PlayerTextDrawBoxColour(playerid, FishRodTD[playerid][rod_Title][0], 9145138);
    PlayerTextDrawSetShadow(playerid, FishRodTD[playerid][rod_Title][0], 0);
    PlayerTextDrawSetOutline(playerid, FishRodTD[playerid][rod_Title][0], 0);
    PlayerTextDrawBackgroundColour(playerid, FishRodTD[playerid][rod_Title][0], 255);
    PlayerTextDrawFont(playerid, FishRodTD[playerid][rod_Title][0], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, FishRodTD[playerid][rod_Title][0], true);

    FishRodTD[playerid][rod_Title][1] = CreatePlayerTextDraw(playerid, 540.000, 393.000, "Level");
    PlayerTextDrawLetterSize(playerid, FishRodTD[playerid][rod_Title][1], 0.158, 1.000);
    PlayerTextDrawTextSize(playerid, FishRodTD[playerid][rod_Title][1], 563.500, 19.500);
    PlayerTextDrawAlignment(playerid, FishRodTD[playerid][rod_Title][1], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, FishRodTD[playerid][rod_Title][1], -1);
    PlayerTextDrawUseBox(playerid, FishRodTD[playerid][rod_Title][1], false);
    PlayerTextDrawBoxColour(playerid, FishRodTD[playerid][rod_Title][1], 9145138);
    PlayerTextDrawSetShadow(playerid, FishRodTD[playerid][rod_Title][1], 0);
    PlayerTextDrawSetOutline(playerid, FishRodTD[playerid][rod_Title][1], 0);
    PlayerTextDrawBackgroundColour(playerid, FishRodTD[playerid][rod_Title][1], 255);
    PlayerTextDrawFont(playerid, FishRodTD[playerid][rod_Title][1], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, FishRodTD[playerid][rod_Title][1], true);

    FishRodTD[playerid][rod_Title][2] = CreatePlayerTextDraw(playerid, 540.000, 405.000, "Exp");
    PlayerTextDrawLetterSize(playerid, FishRodTD[playerid][rod_Title][2], 0.158, 1.000);
    PlayerTextDrawTextSize(playerid, FishRodTD[playerid][rod_Title][2], 563.500, 19.500);
    PlayerTextDrawAlignment(playerid, FishRodTD[playerid][rod_Title][2], TEXT_DRAW_ALIGN_LEFT);
    PlayerTextDrawColour(playerid, FishRodTD[playerid][rod_Title][2], -1);
    PlayerTextDrawUseBox(playerid, FishRodTD[playerid][rod_Title][2], false);
    PlayerTextDrawBoxColour(playerid, FishRodTD[playerid][rod_Title][2], 9145138);
    PlayerTextDrawSetShadow(playerid, FishRodTD[playerid][rod_Title][2], 0);
    PlayerTextDrawSetOutline(playerid, FishRodTD[playerid][rod_Title][2], 0);
    PlayerTextDrawBackgroundColour(playerid, FishRodTD[playerid][rod_Title][2], 255);
    PlayerTextDrawFont(playerid, FishRodTD[playerid][rod_Title][2], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, FishRodTD[playerid][rod_Title][2], true);

    FishRodTD[playerid][rod_Type] = CreatePlayerTextDraw(playerid, 619.000, 381.000, "Huyen Thoai");
    PlayerTextDrawLetterSize(playerid, FishRodTD[playerid][rod_Type], 0.158, 1.000);
    PlayerTextDrawTextSize(playerid, FishRodTD[playerid][rod_Type], 619.500, 19.500);
    PlayerTextDrawAlignment(playerid, FishRodTD[playerid][rod_Type], TEXT_DRAW_ALIGN_RIGHT);
    PlayerTextDrawColour(playerid, FishRodTD[playerid][rod_Type], -16776961);
    PlayerTextDrawUseBox(playerid, FishRodTD[playerid][rod_Type], false);
    PlayerTextDrawBoxColour(playerid, FishRodTD[playerid][rod_Type], 9145138);
    PlayerTextDrawSetShadow(playerid, FishRodTD[playerid][rod_Type], 0);
    PlayerTextDrawSetOutline(playerid, FishRodTD[playerid][rod_Type], 0);
    PlayerTextDrawBackgroundColour(playerid, FishRodTD[playerid][rod_Type], 255);
    PlayerTextDrawFont(playerid, FishRodTD[playerid][rod_Type], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, FishRodTD[playerid][rod_Type], true);

    FishRodTD[playerid][rod_Level] = CreatePlayerTextDraw(playerid, 619.000, 393.000, "10");
    PlayerTextDrawLetterSize(playerid, FishRodTD[playerid][rod_Level], 0.158, 1.000);
    PlayerTextDrawTextSize(playerid, FishRodTD[playerid][rod_Level], 619.500, 19.500);
    PlayerTextDrawAlignment(playerid, FishRodTD[playerid][rod_Level], TEXT_DRAW_ALIGN_RIGHT);
    PlayerTextDrawColour(playerid, FishRodTD[playerid][rod_Level], 16777215);
    PlayerTextDrawUseBox(playerid, FishRodTD[playerid][rod_Level], false);
    PlayerTextDrawBoxColour(playerid, FishRodTD[playerid][rod_Level], 9145138);
    PlayerTextDrawSetShadow(playerid, FishRodTD[playerid][rod_Level], 0);
    PlayerTextDrawSetOutline(playerid, FishRodTD[playerid][rod_Level], 0);
    PlayerTextDrawBackgroundColour(playerid, FishRodTD[playerid][rod_Level], 255);
    PlayerTextDrawFont(playerid, FishRodTD[playerid][rod_Level], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, FishRodTD[playerid][rod_Level], true);

    FishRodTD[playerid][rod_Exp] = CreatePlayerTextDraw(playerid, 619.000, 405.000, "10 / 100");
    PlayerTextDrawLetterSize(playerid, FishRodTD[playerid][rod_Exp], 0.158, 1.000);
    PlayerTextDrawTextSize(playerid, FishRodTD[playerid][rod_Exp], 619.500, 19.500);
    PlayerTextDrawAlignment(playerid, FishRodTD[playerid][rod_Exp], TEXT_DRAW_ALIGN_RIGHT);
    PlayerTextDrawColour(playerid, FishRodTD[playerid][rod_Exp], 16777215);
    PlayerTextDrawUseBox(playerid, FishRodTD[playerid][rod_Exp], false);
    PlayerTextDrawBoxColour(playerid, FishRodTD[playerid][rod_Exp], 9145138);
    PlayerTextDrawSetShadow(playerid, FishRodTD[playerid][rod_Exp], 0);
    PlayerTextDrawSetOutline(playerid, FishRodTD[playerid][rod_Exp], 0);
    PlayerTextDrawBackgroundColour(playerid, FishRodTD[playerid][rod_Exp], 255);
    PlayerTextDrawFont(playerid, FishRodTD[playerid][rod_Exp], TEXT_DRAW_FONT_1);
    PlayerTextDrawSetProportional(playerid, FishRodTD[playerid][rod_Exp], true);

    FishRodTD[playerid][rod_ExpBar] = CreatePlayerProgressBar(playerid, 540.0, 423.5, 80.0, 9.5, FISH_COLOR_TD_BUTTON, 100.0);
    return 1;
}

stock InitFishFightTD(playerid) {
    FishFightTD[playerid][fishFight_bg] = CreatePlayerTextDraw(playerid, 318.0, 392.0, "_");
    PlayerTextDrawFont(playerid, FishFightTD[playerid][fishFight_bg], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishFightTD[playerid][fishFight_bg], 0.6, 3.9);
    PlayerTextDrawTextSize(playerid, FishFightTD[playerid][fishFight_bg], 298.5, 75.0);
    PlayerTextDrawSetOutline(playerid, FishFightTD[playerid][fishFight_bg], 1);
    PlayerTextDrawSetShadow(playerid, FishFightTD[playerid][fishFight_bg], 0);
    PlayerTextDrawAlignment(playerid, FishFightTD[playerid][fishFight_bg], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishFightTD[playerid][fishFight_bg], -1);
    PlayerTextDrawBackgroundColour(playerid, FishFightTD[playerid][fishFight_bg], 255);
    PlayerTextDrawBoxColour(playerid, FishFightTD[playerid][fishFight_bg], 255);
    PlayerTextDrawUseBox(playerid, FishFightTD[playerid][fishFight_bg], true);
    PlayerTextDrawSetProportional(playerid, FishFightTD[playerid][fishFight_bg], true);
    PlayerTextDrawSetSelectable(playerid, FishFightTD[playerid][fishFight_bg], false);

    FishFightTD[playerid][fishFight_title] = CreatePlayerTextDraw(playerid, 318.0, 393.0, "Fish Fight");
    PlayerTextDrawFont(playerid, FishFightTD[playerid][fishFight_title], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishFightTD[playerid][fishFight_title], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, FishFightTD[playerid][fishFight_title], 403.0, 73.5);
    PlayerTextDrawSetOutline(playerid, FishFightTD[playerid][fishFight_title], 0);
    PlayerTextDrawSetShadow(playerid, FishFightTD[playerid][fishFight_title], 0);
    PlayerTextDrawAlignment(playerid, FishFightTD[playerid][fishFight_title], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishFightTD[playerid][fishFight_title], 255);
    PlayerTextDrawBackgroundColour(playerid, FishFightTD[playerid][fishFight_title], 255);
    PlayerTextDrawBoxColour(playerid, FishFightTD[playerid][fishFight_title], -1);
    PlayerTextDrawUseBox(playerid, FishFightTD[playerid][fishFight_title], true);
    PlayerTextDrawSetProportional(playerid, FishFightTD[playerid][fishFight_title], true);
    PlayerTextDrawSetSelectable(playerid, FishFightTD[playerid][fishFight_title], false);

    FishFightTD[playerid][fishFight_name] = CreatePlayerTextDraw(playerid, 281.0, 404.0, "Ca Map Trang Con");
    PlayerTextDrawFont(playerid, FishFightTD[playerid][fishFight_name], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishFightTD[playerid][fishFight_name], 0.1, 0.8);
    PlayerTextDrawTextSize(playerid, FishFightTD[playerid][fishFight_name], 321.0, 73.5);
    PlayerTextDrawSetOutline(playerid, FishFightTD[playerid][fishFight_name], 0);
    PlayerTextDrawSetShadow(playerid, FishFightTD[playerid][fishFight_name], 0);
    PlayerTextDrawAlignment(playerid, FishFightTD[playerid][fishFight_name], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, FishFightTD[playerid][fishFight_name], -1);
    PlayerTextDrawBackgroundColour(playerid, FishFightTD[playerid][fishFight_name], 255);
    PlayerTextDrawBoxColour(playerid, FishFightTD[playerid][fishFight_name], -206);
    PlayerTextDrawUseBox(playerid, FishFightTD[playerid][fishFight_name], false);
    PlayerTextDrawSetProportional(playerid, FishFightTD[playerid][fishFight_name], true);
    PlayerTextDrawSetSelectable(playerid, FishFightTD[playerid][fishFight_name], false);

    FishFightTD[playerid][fishFight_type] = CreatePlayerTextDraw(playerid, 355.0, 404.0, "~r~Huyen Thoai");
    PlayerTextDrawFont(playerid, FishFightTD[playerid][fishFight_type], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishFightTD[playerid][fishFight_type], 0.1, 0.8);
    PlayerTextDrawTextSize(playerid, FishFightTD[playerid][fishFight_type], 160.0, 125.5);
    PlayerTextDrawSetOutline(playerid, FishFightTD[playerid][fishFight_type], 0);
    PlayerTextDrawSetShadow(playerid, FishFightTD[playerid][fishFight_type], 0);
    PlayerTextDrawAlignment(playerid, FishFightTD[playerid][fishFight_type], TEXT_DRAW_ALIGN:3);
    PlayerTextDrawColour(playerid, FishFightTD[playerid][fishFight_type], -1);
    PlayerTextDrawBackgroundColour(playerid, FishFightTD[playerid][fishFight_type], 255);
    PlayerTextDrawBoxColour(playerid, FishFightTD[playerid][fishFight_type], -206);
    PlayerTextDrawUseBox(playerid, FishFightTD[playerid][fishFight_type], false);
    PlayerTextDrawSetProportional(playerid, FishFightTD[playerid][fishFight_type], true);
    PlayerTextDrawSetSelectable(playerid, FishFightTD[playerid][fishFight_type], false);

    FishFightTD[playerid][fishFight_help] = CreatePlayerTextDraw(playerid, 318.0, 421.0, "~r~SPACE ~g~de keo day");
    PlayerTextDrawFont(playerid, FishFightTD[playerid][fishFight_help], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, FishFightTD[playerid][fishFight_help], 0.1, 0.6);
    PlayerTextDrawTextSize(playerid, FishFightTD[playerid][fishFight_help], 403.0, 73.5);
    PlayerTextDrawSetOutline(playerid, FishFightTD[playerid][fishFight_help], 0);
    PlayerTextDrawSetShadow(playerid, FishFightTD[playerid][fishFight_help], 0);
    PlayerTextDrawAlignment(playerid, FishFightTD[playerid][fishFight_help], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, FishFightTD[playerid][fishFight_help], -1);
    PlayerTextDrawBackgroundColour(playerid, FishFightTD[playerid][fishFight_help], 255);
    PlayerTextDrawBoxColour(playerid, FishFightTD[playerid][fishFight_help], -1);
    PlayerTextDrawUseBox(playerid, FishFightTD[playerid][fishFight_help], false);
    PlayerTextDrawSetProportional(playerid, FishFightTD[playerid][fishFight_help], true);
    PlayerTextDrawSetSelectable(playerid, FishFightTD[playerid][fishFight_help], false);


    FishFightTD[playerid][fishFight_progress] = CreatePlayerProgressBar(playerid, 281.0, 415.0, 73.5, 1.0, 16777215, 100.0);
    SetPlayerProgressBarValue(playerid, FishFightTD[playerid][fishFight_progress], 10.0);
    return 1;
}


/////////////////////////////////
/// Handle Functions         ///
///////////////////////////////

stock InitFishingTD(playerid) {
    InitFishGeneralTD(playerid);
    InitFishEquipTD(playerid);
    InitFishTdGrid(playerid);
    InitFishInfoItem(playerid);
    FishTdOpen[playerid] = false;

    InitFishFightTD(playerid);
    return 1;
}

stock OpenFishingTD(playerid)
{
    ShowFishingTd(playerid);
    FishTdOpen[playerid] = true;
    FishSlotClicking[playerid] = -1;
    SelectTextDraw(playerid, FISH_COLOR_TD_HOVER);
    return 1;
}

stock CloseFishingTD(playerid)
{
    HideFishingTD(playerid);
    FishTdOpen[playerid] = false;
    FishSlotClicking[playerid] = -1;
    CancelSelectTextDraw(playerid);
    return 1;
}

stock ShowFishRodTD(playerid, bool:show)
{
    if(show) {
        PlayerTextDrawShow(playerid, FishRodTD[playerid][rod_Model]);
        PlayerTextDrawShow(playerid, FishRodTD[playerid][rod_Title][0]);
        PlayerTextDrawShow(playerid, FishRodTD[playerid][rod_Title][1]);
        PlayerTextDrawShow(playerid, FishRodTD[playerid][rod_Title][2]);

        PlayerTextDrawShow(playerid, FishRodTD[playerid][rod_Type]);
        PlayerTextDrawShow(playerid, FishRodTD[playerid][rod_Level]);
        PlayerTextDrawShow(playerid, FishRodTD[playerid][rod_Exp]);

        ShowPlayerProgressBar(playerid, FishRodTD[playerid][rod_ExpBar]);
    } else {
        PlayerTextDrawHide(playerid, FishRodTD[playerid][rod_Model]);
        PlayerTextDrawHide(playerid, FishRodTD[playerid][rod_Title][0]);
        PlayerTextDrawHide(playerid, FishRodTD[playerid][rod_Title][1]);
        PlayerTextDrawHide(playerid, FishRodTD[playerid][rod_Title][2]);

        PlayerTextDrawHide(playerid, FishRodTD[playerid][rod_Type]);
        PlayerTextDrawHide(playerid, FishRodTD[playerid][rod_Level]);
        PlayerTextDrawHide(playerid, FishRodTD[playerid][rod_Exp]);

        HidePlayerProgressBar(playerid, FishRodTD[playerid][rod_ExpBar]);
    }
    return 1;
}

stock ShowFishingTd(playerid)
{
    ShowFishGeneralTD(playerid, true);
    ShowFishRodTD(playerid, true);
    ShowFishGridTD(playerid, true);
    // ShowFishInfoItemTD(playerid, true);
    return 1;
}

stock HideFishingTD(playerid)
{
    ShowFishGeneralTD(playerid, false);
    ShowFishRodTD(playerid, false);
    ShowFishGridTD(playerid, false);
    ShowFishInfoItemTD(playerid, false);
    return 1;
}

stock ShowFishGeneralTD(playerid, bool:show)
{
    if(show) { 
        PlayerTextDrawShow(playerid, FishGeneralTD[playerid][fish_bgMain]);
        PlayerTextDrawShow(playerid, FishGeneralTD[playerid][fish_bgGridItem]);

        PlayerTextDrawShow(playerid, FishGeneralTD[playerid][fish_bgHeader]);
        PlayerTextDrawShow(playerid, FishGeneralTD[playerid][fish_header_title]);
        PlayerTextDrawShow(playerid, FishGeneralTD[playerid][fish_header_player]);

        PlayerTextDrawShow(playerid, FishGeneralTD[playerid][fish_bgEquip]);
        
        PlayerTextDrawShow(playerid, FishGeneralTD[playerid][fish_btn_close]);
    }
    else {
        PlayerTextDrawHide(playerid, FishGeneralTD[playerid][fish_bgMain]);
        PlayerTextDrawHide(playerid, FishGeneralTD[playerid][fish_bgGridItem]);
        PlayerTextDrawHide(playerid, FishGeneralTD[playerid][fish_btn_close]);

        PlayerTextDrawHide(playerid, FishGeneralTD[playerid][fish_bgHeader]);
        PlayerTextDrawHide(playerid, FishGeneralTD[playerid][fish_header_title]);
        PlayerTextDrawHide(playerid, FishGeneralTD[playerid][fish_header_player]);

        PlayerTextDrawHide(playerid, FishGeneralTD[playerid][fish_bgEquip]);
    }
    return 1;
}

stock ShowFishGridTD(playerid, bool:show)
{
    new slotId;
    for(slotId = 0; slotId < MAX_FISH_SLOTS; slotId++) {
        if(show) {
            PlayerTextDrawShow(playerid, FishGridTD[playerid][slotId][fish_grid_box]);
            PlayerTextDrawShow(playerid, FishGridTD[playerid][slotId][fish_grid_model]);
            PlayerTextDrawShow(playerid, FishGridTD[playerid][slotId][fish_grid_text]);
        } else {
            PlayerTextDrawHide(playerid, FishGridTD[playerid][slotId][fish_grid_box]);
            PlayerTextDrawHide(playerid, FishGridTD[playerid][slotId][fish_grid_model]);
            PlayerTextDrawHide(playerid, FishGridTD[playerid][slotId][fish_grid_text]);
        }
    }
    return 1;
}

stock ShowFishInfoItemTD(playerid, bool:show)
{
    
    if(show) {
        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_background]);
        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_model]);
        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_name]);
        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_type]);
        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_weight]);
        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_amount]);
        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_description]);

        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_btn_sell]);
        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_btn_give]);
        PlayerTextDrawShow(playerid, FishInfoItemTD[playerid][fish_item_btn_drop]);
    }
    else {
        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_background]);
        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_model]);
        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_name]);
        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_type]);
        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_weight]);
        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_amount]);
        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_description]);

        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_btn_sell]);
        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_btn_give]);
        PlayerTextDrawHide(playerid, FishInfoItemTD[playerid][fish_item_btn_drop]);
    }
    return 1;
}


stock ShowFishFightTD(playerid, bool:show) {
    if(show) {
        PlayerTextDrawShow(playerid, FishFightTD[playerid][fishFight_bg]);
        PlayerTextDrawShow(playerid, FishFightTD[playerid][fishFight_title]);
        PlayerTextDrawShow(playerid, FishFightTD[playerid][fishFight_name]);
        PlayerTextDrawShow(playerid, FishFightTD[playerid][fishFight_type]);
        PlayerTextDrawShow(playerid, FishFightTD[playerid][fishFight_help]);

        ShowPlayerProgressBar(playerid, FishFightTD[playerid][fishFight_progress]);
    } else {
        PlayerTextDrawHide(playerid, FishFightTD[playerid][fishFight_bg]);
        PlayerTextDrawHide(playerid, FishFightTD[playerid][fishFight_title]);
        PlayerTextDrawHide(playerid, FishFightTD[playerid][fishFight_name]);
        PlayerTextDrawHide(playerid, FishFightTD[playerid][fishFight_type]);
        PlayerTextDrawHide(playerid, FishFightTD[playerid][fishFight_help]);

        HidePlayerProgressBar(playerid, FishFightTD[playerid][fishFight_progress]);
    }
    
    return 1;
}


stock UpdateFishInfoItemTD(playerid, modelid = FISH_MODEL_DEFAULT, const fish_name[], const fish_type[], Float:fish_weight = 0.0, price_fish = 0, const fish_des[] = "_")
{   
    PlayerTextDrawSetPreviewModel(playerid, FishInfoItemTD[playerid][fish_item_model], modelid);
    // PlayerTextDrawSetPreviewRot(playerid, FishInfoItemTD[playerid][fish_item_model], rotX, rotY, rotZ, zoom);
    
    new txt_item_name[50];
    format(txt_item_name, sizeof(txt_item_name), "Name: ~y~%s", fish_name);
    PlayerTextDrawSetString(playerid, FishInfoItemTD[playerid][fish_item_name], txt_item_name);

    new txt_item_type[50];
    format(txt_item_type, sizeof(txt_item_type), "Category: ~r~%s", fish_type);
    PlayerTextDrawSetString(playerid, FishInfoItemTD[playerid][fish_item_type], txt_item_type);

    new txt_item_weight[50];
    format(txt_item_weight, sizeof(txt_item_weight), "Weight: ~g~%0.1fkg", fish_weight);
    PlayerTextDrawSetString(playerid, FishInfoItemTD[playerid][fish_item_weight], txt_item_weight);

    new txt_item_amount[50];
    format(txt_item_amount, sizeof(txt_item_amount), "Price: ~r~$%d", price_fish);
    PlayerTextDrawSetString(playerid, FishInfoItemTD[playerid][fish_item_amount], txt_item_amount);

    new txt_item_description[152];
    format(txt_item_description, sizeof(txt_item_description), "~b~Description: ~w~%s", fish_des);
    PlayerTextDrawSetString(playerid, FishInfoItemTD[playerid][fish_item_description], txt_item_description);

    ShowFishInfoItemTD(playerid, true);
    return 1;
}

stock UpdateFishGridSlot(playerid, slotId, modelid = FISH_MODEL_DEFAULT, const info[])
{
    if(modelid != FISH_MODEL_DEFAULT && modelid > 0) {
        PlayerTextDrawSetPreviewModel(playerid, FishGridTD[playerid][slotId][fish_grid_model], modelid); 
    } else {
        PlayerTextDrawSetPreviewModel(playerid, FishGridTD[playerid][slotId][fish_grid_model], FISH_MODEL_DEFAULT);
    }
    
    PlayerTextDrawBackgroundColour(playerid, FishGridTD[playerid][slotId][fish_grid_model], 0);
    PlayerTextDrawSetString(playerid, FishGridTD[playerid][slotId][fish_grid_text], info);
    return 1;
}

stock UpdateFishEquip(playerid, const rodtype[], rodlevel = 1, exp = 5, target_exp = 100) {
    new txt_type[32];
    format(txt_type, sizeof(txt_type), "~r~%s", rodtype);
    PlayerTextDrawSetString(playerid, FishRodTD[playerid][rod_Type], txt_type);

    new txt_level[11];
    format(txt_level, sizeof(txt_level), "~b~%d", rodlevel);
    PlayerTextDrawSetString(playerid, FishRodTD[playerid][rod_Level], txt_level);
    
    new txt_exp[15];
    format(txt_exp, sizeof(txt_exp), "~g~%d / %d", exp, target_exp);
    PlayerTextDrawSetString(playerid, FishRodTD[playerid][rod_Exp], txt_exp);

    SetPlayerProgressBarMaxValue(playerid, FishRodTD[playerid][rod_ExpBar], target_exp); 
    SetPlayerProgressBarValue(playerid, FishRodTD[playerid][rod_ExpBar], exp); 
    return 1;
}

stock UpdateFishFightTD(playerid, const name[], const type[]) {
    PlayerTextDrawSetString(playerid, FishFightTD[playerid][fishFight_name], name);
    PlayerTextDrawSetString(playerid, FishFightTD[playerid][fishFight_type], type);
    return 1;
}

stock UpdateProgressFishFight(playerid, Float:value = 10.0) {

    SetPlayerProgressBarValue(playerid, FishFightTD[playerid][fishFight_progress], value);
    ShowPlayerProgressBar(playerid, FishFightTD[playerid][fishFight_progress]);
    return 1;
}

stock HandleClickInv(playerid, PlayerText:playertextid)
{
    if(FishTdOpen[playerid]) {
        if(playertextid == FishGeneralTD[playerid][fish_btn_close]) return CloseFishingTD(playerid);

        for(new slotId = 0; slotId < MAX_FISH_SLOTS; slotId++)
        {
            if(playertextid == FishGridTD[playerid][slotId][fish_grid_model])
            {
                if(slotId == FishSlotClicking[playerid]) 
                {
                    PlayerTextDrawBackgroundColour(playerid, FishGridTD[playerid][slotId][fish_grid_model], 0);
                    PlayerTextDrawShow(playerid, FishGridTD[playerid][slotId][fish_grid_model]);
                    FishSlotClicking[playerid] = -1;
                    ShowFishInfoItemTD(playerid, false);
                    return 1;
                } 

                PlayerTextDrawBackgroundColour(playerid, FishGridTD[playerid][slotId][fish_grid_model], FISH_COLOR_TD_HOVER);
                PlayerTextDrawShow(playerid, FishGridTD[playerid][slotId][fish_grid_model]);
                FishSlotClicking[playerid]  = slotId;

                new fishid = PlayerFishInv[playerid][slotId][pFishID];
                if(fishid >= 0) {
                    new Float:weight = PlayerFishInv[playerid][slotId][pFishWeight];

                    new name[50], type[50], e_fish_rarity:rarity;
                    GetFishName(fishid, name, sizeof(name));
                    GetFishRarityName(fishid, type, sizeof(type));
                    rarity = GetFishRarity(fishid);

                    new price = GetFishPrice(fishid, weight);
                    UpdateFishInfoItemTD(playerid, GetFishModel(fishid), name, type, weight, price, FishRarityDescription[rarity]);
                }
            }
            else {
                PlayerTextDrawBackgroundColour(playerid, FishGridTD[playerid][slotId][fish_grid_model], 0);
                PlayerTextDrawShow(playerid, FishGridTD[playerid][slotId][fish_grid_model]);
            }
        }
        return 1;
    }
	return 1;
}



////////////////////////////////////////////////////
/// Inventory Functions
////////////////////////////////////////////////////

stock GetExpRodRequired(level)
{
    return level * 23;
}

stock bool:IsPlayerFishSlotUsed(playerid, slot)
{
    return PlayerFishInv[playerid][slot][pFishID] != -1;
}

stock ResetPlayerFish(playerid, slot)
{
    PlayerFishInv[playerid][slot][pFishID]      = -1;
    PlayerFishInv[playerid][slot][pFishWeight]  = 0.0;
    return 1;
}

stock GetFreeFishSlot(playerid)
{
    for(new slot; slot < MAX_FISH_SLOTS; slot++)
    {
        if(!IsPlayerFishSlotUsed(playerid, slot))
            return slot;
    }
    return -1;
}

stock GivePlayerFish(playerid, fishid, Float:weight)
{
    new slot = GetFreeFishSlot(playerid);

    if(slot == -1) {
        return -1;
    }
    PlayerFishInv[playerid][slot][pFishID] = fishid;
    PlayerFishInv[playerid][slot][pFishWeight] = weight;
    return slot;
}

stock RemovePlayerFish(playerid, slot)
{
    if(!IsPlayerFishSlotUsed(playerid, slot))
        return 0;

    // Sau này thêm DELETE SQL ở đây

    ResetPlayerFish(playerid, slot);
    return 1;
}
new const RodTypeName[][] =
{
    "Common",
    "Rare",
    "Epic",
    "Legendary"
};

stock GetFishRodInfo(playerid, name[], &level, &exp, size = sizeof(name))
{
    level = PlayerFishRod[playerid][pRodLevel];
    exp   = PlayerFishRod[playerid][pRodExp];
    
    new e_rod_type:type;
    if(level >= 0 && level <= 2) type = e_rod_type:ROD_COMMON;
    else if(level >= 3 && level <= 4) type = e_rod_type:ROD_RARE;
    else if(level >= 5 && level <= 6) type = e_rod_type:ROD_EPIC;
    else if(level >= 7) type = e_rod_type:ROD_LEGENDARY;
    else type = e_rod_type:ROD_COMMON;

    if (_:type >= sizeof(RodTypeName))
    {
        format(name, size, "Unknown");
    }
    else
    {
        format(name, size, "%s", RodTypeName[_:type]);
    }
    return 1;
}