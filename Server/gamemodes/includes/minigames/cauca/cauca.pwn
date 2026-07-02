#include <YSI_Coding\y_hooks>
#define DIALOG_GIVE_FISH 1000

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

///////////////////////////////////////////////////////////////////
//// Fish Data Variables
//////////////////////////////////////////////////////////////////

enum e_fish_rarity
{
    FISH_COMMON,
    FISH_RARE,
    FISH_EPIC,
    FISH_LEGENDARY
};

new const FishRarityDescription[][] =
{
    "Loai pho bien, xuat hien o hau het cac vung nuoc. De cau nhung gia tri khong cao.",

    "Loai kha hiem, can mot chut may man de bat duoc. Co gia ban tot hon ca pho bien.",

    "Loai cuc hiem duoc nhieu ngu dan san lung. Kho cau nhung mang lai gia tri rat cao.",

    "Loai huyen thoai chi xuat hien trong nhung dieu kien dac biet. Bat duoc chung la niem tu hao cua moi can thu."
};

enum e_fish_data
{
    fishName[32],

    fishModelId,

    fishPrice,

    fishMinWeight,

    fishMaxWeight,

    e_fish_rarity:fishRarity,

    fishExp,

    fishPullPower
};
new FishData[][e_fish_data] =
{
    //========================
    // COMMON
    //========================

    {"Ca Chep",         1599,  800, 1, 3, FISH_COMMON,      5, 3},
    {"Ca Ro",           1599,  900, 1, 4, FISH_COMMON,      5, 4},
    {"Ca Tre",          1605, 1000, 2, 5, FISH_COMMON,      6, 4},
    {"Ca Loc",          1605, 1200, 2, 6, FISH_COMMON,      6, 5},
    {"Ca Me",           1605, 1300, 3, 7, FISH_COMMON,      7, 4},
    {"Ca Dieu Hong",    1605, 1500, 2, 6, FISH_COMMON,      7, 3},
    {"Tom",             1609,  900, 1, 2, FISH_COMMON,      5, 2},
    {"Cua Dong",        1610, 1000, 1, 2, FISH_COMMON,      5, 2},

    //========================
    // RARE
    //========================

    {"Ca Hoi",          1604, 2200, 3, 8,  FISH_RARE,      10, 6},
    {"Ca Thu",          1606, 2600, 4,10,  FISH_RARE,      10, 8},
    {"Ca Ngu",          1606, 3000, 5,12,  FISH_RARE,      11, 9},
    {"Ca Chinh",        1605, 3200, 5,11,  FISH_RARE,      11, 7},
    {"Ca Chim",         1606, 3400, 4, 9,  FISH_RARE,      12, 7},
    {"Muc La",          1611, 3000, 3, 8,  FISH_RARE,      11, 6},
    {"Ghe",             1612, 2800, 2, 6,  FISH_RARE,      10, 6},
    {"Bach Tuoc",       1613, 3500, 4,10,  FISH_RARE,      12, 8},

    //========================
    // EPIC
    //========================

    {"Tom Hum",         1614, 5000, 5,12, FISH_EPIC,       18, 10},
    {"Ca Kiem",         1606, 5500, 8,18, FISH_EPIC,       18, 11},
    {"Ca Duoi",         1607, 5800, 8,20, FISH_EPIC,       19, 12},
    {"Ca Map Con",      1607, 6500,10,22, FISH_EPIC,       20, 13},
    {"Cua Hoang De",    1615, 7000, 8,18, FISH_EPIC,       20, 14},

    //========================
    // LEGENDARY
    //========================

    {"Ca Map",          1607,10000,15,35, FISH_LEGENDARY, 30, 15},
    {"Ca Map Trang",    1607,12000,18,40, FISH_LEGENDARY, 32, 16},
    {"Ca Rong",         1608,15000,10,30, FISH_LEGENDARY, 35, 17}
};

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

///////////////////////////////////////////////////////////////////
//// Fishing Variables
//////////////////////////////////////////////////////////////////

enum e_fishing_state
{
    FISH_STATE_NONE,

    FISH_STATE_WAITING,

    FISH_STATE_BITE,

    FISH_STATE_MINIGAME
};

enum e_player_fishing
{
    bool:pFishing,

    e_fishing_state:pState,

    pFishID,
    Float:pFishWeight,
    Float:pProgress,
    pFishFight
};

new PlayerFishing[MAX_PLAYERS][e_player_fishing];


///////////////////////////////////////////////////////////////////
//// Fish TD Functions
//////////////////////////////////////////////////////////////////

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

///////////////////////////////////////////////////////////////////
//// FishData Functions
//////////////////////////////////////////////////////////////////
stock GetFishName(fishid, dest[], len = sizeof(dest))
{
    if (fishid < 0 || fishid >= sizeof(FishData))
    {
        format(dest, len, "Unknown");
        return 0;
    }

    format(dest, len, "%s", FishData[fishid][fishName]);
    return 1;
}

stock GetFishModel(fishid) {
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 19300;

    return FishData[fishid][fishModelId];
}

stock GetFishPullPower(fishid) {
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 1;

    return FishData[fishid][fishPullPower];
}

stock GetFishRarityName(fishid, dest[], len = sizeof(dest))
{
    if (fishid < 0 || fishid >= sizeof(FishData))
    {
        format(dest, len, "Unknown");
        return 0;
    }

    switch(FishData[fishid][fishRarity]) {
        case FISH_COMMON: format(dest, len, "Common");
        case FISH_RARE: format(dest, len, "Rare");
        case FISH_EPIC: format(dest, len, "Epic");
        case FISH_LEGENDARY: format(dest, len, "Legendary");
        default: format(dest, len, "Unknown");
    }
    return 1;
}

stock GetFishPrice(fishid, Float:weight = 1.0)
{
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 0;

    return FishData[fishid][fishPrice] * floatround(weight);
}

stock e_fish_rarity:GetFishRarity(fishid)
{
    if (fishid < 0 || fishid >= sizeof(FishData))
        return FISH_COMMON;

    return FishData[fishid][fishRarity];
}

stock GetFishEXP(fishid)
{
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 0;

    return FishData[fishid][fishExp];
}

stock e_fish_rarity:GetRandomRarity(rod_level)
{
    new chance = random(100);
    if(rod_level == 1) {
        if (chance < 80)
            return FISH_COMMON;

        if (chance < 95)
            return FISH_RARE;

        if (chance < 99)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 2) {
        if (chance < 60)
            return FISH_COMMON;

        if (chance < 85)
            return FISH_RARE;

        if (chance < 97)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 3) {
        if (chance < 40)
            return FISH_COMMON;

        if (chance < 70)
            return FISH_RARE;

        if (chance < 90)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 4) {
        if (chance < 20)
            return FISH_COMMON;

        if (chance < 50)
            return FISH_RARE;

        if (chance < 80)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 5) {
        if (chance < 10)
            return FISH_COMMON;

        if (chance < 30)
            return FISH_RARE;

        if (chance < 70)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 6) {
        if (chance < 5)
            return FISH_COMMON;

        if (chance < 20)
            return FISH_RARE;

        if (chance < 60)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level >= 7) {
        if (chance < 1)
            return FISH_COMMON;

        if (chance < 10)
            return FISH_RARE;

        if (chance < 50)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    return FISH_COMMON;
}

stock GetRandomFish(rod_level)
{
    new e_fish_rarity:rarity = GetRandomRarity(rod_level);
    new list[sizeof(FishData)];
    new count;

    for (new i; i < sizeof(FishData); i++)
    {
        if (FishData[i][fishRarity] == rarity)
        {
            list[count++] = i;
        }
    }

    if (count == 0)
        return -1;

    return list[random(count)];
}

stock bool:IsValidFish(fishid)
{
    return (0 <= fishid < sizeof(FishData));
}

stock Float:GetRandomFishWeight(fishid)
{
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 0.0;

    new Float:minWeight = FishData[fishid][fishMinWeight];
    new Float:maxWeight = FishData[fishid][fishMaxWeight];

    return RandomFloatMinMax(minWeight, maxWeight);
}

stock ShowInfoAllFish(playerid) {
    new dialog[4096];

    format(dialog, sizeof(dialog),
        "Ten ca\tDo hiem\tCan nang\tGia ban\n");

    for(new i; i < sizeof(FishData); i++)
    {
        new rarity[16];
        GetFishRarityName(i, rarity);

        new minValue = FishData[i][fishMinWeight] * FishData[i][fishPrice];
        new maxValue = FishData[i][fishMaxWeight] * FishData[i][fishPrice];

        format(dialog, sizeof(dialog),
            "%s%s\t{33CCFF}%s\t{CC33FF}%d - %d kg\t{FFD700}$%d - $%d {66FF66}(+%d exp)\n",
            dialog,
            FishData[i][fishName],
            rarity,
            FishData[i][fishMinWeight],
            FishData[i][fishMaxWeight],
            minValue,
            maxValue,
            GetFishEXP(i)
        );
    }

    ShowPlayerDialog(playerid,
        DIALOG_NOTHING,
        DIALOG_STYLE_TABLIST_HEADERS,
        "Thong tin cac loai ca",
        dialog,
        "Dong", "");
    return 1;
}

///////////////////////////////////////////////////////////////////
//// Fishing
//////////////////////////////////////////////////////////////////
stock StartFishing(playerid)
{
    ResetFishingData(playerid);

    PlayerFishing[playerid][pFishing] = true;
    PlayerFishing[playerid][pState] = FISH_STATE_WAITING;

    ApplyAnimation(playerid,
        "SWORD",
        "sword_idle",
        4.0,
        true,
        false,
        false,
        true,
        0);

    SendClientMessage(playerid, -1,
        "Dang tha can...");
    SetPlayerAttachedObject(
        playerid,
        0,
        MODEL_FISHING_ROD,
        6,

        // Position
        0.08,
        0.02,
        0.00,

        // Rotation
        0.0,
        // 0.0,
        45.0,
        0.0,

        // Scale
        1.0,
        1.0,
        1.0
    );
    StartWaitingFish(playerid);
    return 1;
}

stock StopFishing(playerid)
{
    ClearAnimations(playerid);
    RemovePlayerAttachedObject(playerid, 0);

    ResetFishingData(playerid);
    ShowFishFightTD(playerid, false);
    return 1;
}

stock StartWaitingFish(playerid)
{
    new time = random(10000) + 21000;
    GameTextForPlayer(playerid, "~g~Dang tha cau ... !", 5000, 3);
    SetTimerEx("OnFishBite", time, false, "i", playerid);
    return 1;
}

forward OnFishBite(playerid);
public OnFishBite(playerid) {
    PlayerFishing[playerid][pState] = FISH_STATE_BITE;

    new rod_level = PlayerFishRod[playerid][pRodLevel];
    new fishid = GetRandomFish(rod_level);

    if(fishid == -1)
        return Fishing_OnFail(playerid);

    PlayerFishing[playerid][pFishID] = fishid;

    new name[32];
    GetFishName(fishid, name);

    new type[32];
    GetFishRarityName(fishid, type);

    new Float:weight = GetRandomFishWeight(fishid);
    PlayerFishing[playerid][pFishWeight] = weight;

    new string[128];
    format(string,
        sizeof(string),
        "[FISHING] %s da can cau. Bam SPACE de thu can !",
        name);
    SendClientMessage(playerid,
        -1,
        string);

    PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);

    UpdateFishFightTD(playerid, name, type);
    ShowFishFightTD(playerid, true);

    Fishing_StartMiniGame(playerid);
    return 1;
}

stock Fishing_StartMiniGame(playerid)
{
    PlayerFishing[playerid][pState] = FISH_STATE_MINIGAME;
    Fishing_UpdateProgressBar(playerid, 20);

    new time = random(300) + 1000;
    PlayerFishing[playerid][pFishFight] = SetTimerEx("FishingFight", time, true, "i", playerid);
    return 1;
}

forward FishingFight(playerid);
public FishingFight(playerid) {
    if(PlayerFishing[playerid][pState] == FISH_STATE_MINIGAME) {
        new fishid = PlayerFishing[playerid][pFishID];
        new pullPower = GetFishPullPower(fishid);
        Fishing_UpdateProgressBar(playerid, -pullPower);
        if(PlayerFishing[playerid][pProgress] <= 0) {
            Fishing_OnFail(playerid);
            PlayerPlaySound(playerid, 1069, 0.0, 0.0, 0.0);
        }
    }
    return 1;
}

stock Fishing_OnSuccess(playerid)
{
    new fishid = PlayerFishing[playerid][pFishID];
    GivePlayerFish(playerid, fishid, PlayerFishing[playerid][pFishWeight]);

    new name[32];
    GetFishName(fishid, name);

    new string[128];

    format(string,
        sizeof(string),
        "[FISHING] Ban da cau duoc {FF0000}%s!",
        name);

    SendClientMessage(playerid,
        -1,
        string);

    StopFishing(playerid);
    return 1;
}

stock Fishing_OnFail(playerid)
{
    SendClientMessage(playerid,
        -1,
        "[FISHING] Oh no! Con ca da bo chay!");

    StopFishing(playerid);
    return 1;
}

stock ResetFishingData(playerid)
{
    PlayerFishing[playerid][pFishing] = false;
    PlayerFishing[playerid][pState] = FISH_STATE_NONE;

    PlayerFishing[playerid][pFishID] = -1;
    PlayerFishing[playerid][pProgress] = 0;

    if(PlayerFishing[playerid][pFishFight]) {
        KillTimer(PlayerFishing[playerid][pFishFight]);
    }

    return 1;
}

stock Fishing_UpdateProgressBar(playerid, Float:value)
{
    PlayerFishing[playerid][pProgress] += value;
    new Float:progress = PlayerFishing[playerid][pProgress];

    if(progress <= 0) progress = 0;
    if(progress >= 100) progress = 100;

    UpdateProgressFishFight(playerid, progress);
    return 1;
}

stock Fishing_Press(playerid)
{
    new pPowerRod = PlayerFishRod[playerid][pRodLevel] * 3;
    Fishing_UpdateProgressBar(playerid, pPowerRod);

    if(PlayerFishing[playerid][pProgress] >= 100) return Fishing_OnSuccess(playerid);
    return 1;
}


CMD:cauca(playerid, params[])
{
    if(!StartFishing(playerid))
        return 1;

    return 1;
}

CMD:huycauca(playerid, params[])
{
    if(!PlayerFishing[playerid][pFishing]) return 1;

    StopFishing(playerid);
    SendClientMessage(playerid, COLOR_DANGER, "[FISHING] Ban da dung cau ca !");
    return 1;
}

CMD:tuica(playerid, params[]) {
    if(FishTdOpen[playerid]) {
        CloseFishingTD(playerid);
    } else {
        new rod_type[32], rodlevel, rodexp;
        GetFishRodInfo(playerid, rod_type, rodlevel, rodexp);
        UpdateFishEquip(playerid, rod_type, rodlevel, rodexp, GetExpRodRequired(rodlevel));

        new fish, modelid, fish_weight[11];
        for(new slot; slot < MAX_FISH_SLOTS; slot++) {
            if(PlayerFishInv[playerid][slot][pFishID] > -1) {
                fish = PlayerFishInv[playerid][slot][pFishID];
                modelid = FishData[fish][fishModelId];
                format(fish_weight, sizeof(fish_weight), "%0.1f~g~(kg)", PlayerFishInv[playerid][slot][pFishWeight]);
                UpdateFishGridSlot(playerid, slot, modelid, fish_weight);
            } else {
                UpdateFishGridSlot(playerid, slot, FISH_MODEL_DEFAULT, "_");
            }
        }
        OpenFishingTD(playerid);
    }
    return 1;
}

CMD:fishs(playerid, params[]) {
    ShowInfoAllFish(playerid);
    return 1;
}

CMD:givefish(playerid, params[]) {
    new listFish[2000];
    new fishname[32], fishtype[32];
    for(new fishId; fishId < sizeof(FishData); fishId++) {
        GetFishName(fishId, fishname);
        GetFishRarityName(fishId, fishtype);
        format(listFish, sizeof(listFish), "%s{FFFF00}%s\t{FFC0CB}%s\n", listFish, fishname, fishtype);
    }
    ShowPlayerDialog(playerid, DIALOG_GIVE_FISH, DIALOG_STYLE_TABLIST, "List Fish", listFish, "Chon", "Dong");
    return 1;
}

hook OnPlayerConnect(playerid) {
    InitFishingTD(playerid);

    PlayerFishRod[playerid][pRodType] = e_rod_type:ROD_COMMON;
    PlayerFishRod[playerid][pRodLevel] = 1;
    PlayerFishRod[playerid][pRodExp] = 5;

    for(new slot; slot < MAX_FISH_SLOTS; slot++) {
        PlayerFishInv[playerid][slot][pFishID] = -1;
        PlayerFishInv[playerid][slot][pFishWeight] = 0.0;
        PlayerFishInv[playerid][slot][pFishLoaded] = true;
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(PlayerFishing[playerid][pFishing]) {
        if(!(newkeys & KEY_JUMP)) {
            if(PlayerFishing[playerid][pState] == FISH_STATE_MINIGAME)
            {
                Fishing_Press(playerid);
            }
        }
    }
    return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == INVALID_TEXT_DRAW) {
        if(FishTdOpen[playerid]) {
            CloseFishingTD(playerid);
            return 1;
        }
    }
	return 1;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    HandleClickInv(playerid, playertextid);
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_GIVE_FISH) {
        if(!response) return 1;
        new fishid = listitem;
        // new rand = random(3) + 7;
        new Float:weight = GetRandomFishWeight(fishid);
        GivePlayerFish(playerid, fishid, weight);
    }
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