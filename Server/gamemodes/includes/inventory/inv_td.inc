#include <YSI_Coding\y_hooks>

#define INVENTORY_ROWS  5
#define INVENTORY_COLS  5
#define TOTAL_SLOTS     (INVENTORY_ROWS * INVENTORY_COLS)

#define INV_COLOR_BG    0x151515EE
#define INV_COLOR_BOX   0x00000088
#define INV_COLOR_ITEM  0x2E2E2EAA
#define INV_COLOR_HOVER 0xE0A96DFF

new PlayerText:Background_Under[MAX_PLAYERS];
new PlayerText:BgRight[MAX_PLAYERS];
new PlayerText:BgCenter[MAX_PLAYERS];
new PlayerText:BgLeft[MAX_PLAYERS];
new PlayerText:BgHeader[MAX_PLAYERS];
new PlayerText:TxtHeaderTitle[MAX_PLAYERS];
new PlayerText:TxtHeaderName[MAX_PLAYERS];
new PlayerText:TxtHeaderStats[MAX_PLAYERS];


stock CreateInvTD(playerid) {
    Background_Under[playerid] = CreatePlayerTextDraw(playerid, 492.5, 246.0, "_");
    PlayerTextDrawFont(playerid, Background_Under[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, Background_Under[playerid], 0.6, 20.8);
    PlayerTextDrawTextSize(playerid, Background_Under[playerid], 298.5, 255.5);
    PlayerTextDrawSetOutline(playerid, Background_Under[playerid], 1);
    PlayerTextDrawSetShadow(playerid, Background_Under[playerid], 0);
    PlayerTextDrawAlignment(playerid, Background_Under[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, Background_Under[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, Background_Under[playerid], 255);
    PlayerTextDrawBoxColour(playerid, Background_Under[playerid], INV_COLOR_BG); // old la 135
    PlayerTextDrawUseBox(playerid, Background_Under[playerid], true);
    PlayerTextDrawSetProportional(playerid, Background_Under[playerid], true);
    PlayerTextDrawSetSelectable(playerid, Background_Under[playerid], false);

    BgHeader[playerid] = CreatePlayerTextDraw(playerid, 492.5, 246.0, "_");
    PlayerTextDrawFont(playerid, BgHeader[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, BgHeader[playerid], 0.6, 1.4);
    PlayerTextDrawTextSize(playerid, BgHeader[playerid], 298.5, 255.5);
    PlayerTextDrawSetOutline(playerid, BgHeader[playerid], 1);
    PlayerTextDrawSetShadow(playerid, BgHeader[playerid], 0);
    PlayerTextDrawAlignment(playerid, BgHeader[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BgHeader[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, BgHeader[playerid], 255);
    PlayerTextDrawBoxColour(playerid, BgHeader[playerid], INV_COLOR_BOX);
    PlayerTextDrawUseBox(playerid, BgHeader[playerid], true);
    PlayerTextDrawSetProportional(playerid, BgHeader[playerid], true);
    PlayerTextDrawSetSelectable(playerid, BgHeader[playerid], false);

    BgRight[playerid] = CreatePlayerTextDraw(playerid, 580.0, 263.0, "_");
    PlayerTextDrawFont(playerid, BgRight[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, BgRight[playerid], 0.6, 18.9);
    PlayerTextDrawTextSize(playerid, BgRight[playerid], 298.5, 80.0);
    PlayerTextDrawSetOutline(playerid, BgRight[playerid], 1);
    PlayerTextDrawSetShadow(playerid, BgRight[playerid], 0);
    PlayerTextDrawAlignment(playerid, BgRight[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BgRight[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, BgRight[playerid], 255);
    PlayerTextDrawBoxColour(playerid, BgRight[playerid], INV_COLOR_BOX);
    PlayerTextDrawUseBox(playerid, BgRight[playerid], true);
    PlayerTextDrawSetProportional(playerid, BgRight[playerid], true);
    PlayerTextDrawSetSelectable(playerid, BgRight[playerid], false);

    BgCenter[playerid] = CreatePlayerTextDraw(playerid, 450.0, 263.0, "_");
    PlayerTextDrawFont(playerid, BgCenter[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, BgCenter[playerid], 0.6, 18.9);
    PlayerTextDrawTextSize(playerid, BgCenter[playerid], 298.5, 170.5);
    PlayerTextDrawSetOutline(playerid, BgCenter[playerid], 1);
    PlayerTextDrawSetShadow(playerid, BgCenter[playerid], 0);
    PlayerTextDrawAlignment(playerid, BgCenter[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BgCenter[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, BgCenter[playerid], 255);
    PlayerTextDrawBoxColour(playerid, BgCenter[playerid], INV_COLOR_BOX);
    PlayerTextDrawUseBox(playerid, BgCenter[playerid], true);
    PlayerTextDrawSetProportional(playerid, BgCenter[playerid], true);
    PlayerTextDrawSetSelectable(playerid, BgCenter[playerid], false);

    BgLeft[playerid] = CreatePlayerTextDraw(playerid, 310.0, 263.0, "_");
    PlayerTextDrawFont(playerid, BgLeft[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, BgLeft[playerid], 0.6, 18.9);
    PlayerTextDrawTextSize(playerid, BgLeft[playerid], 298.5, 100.0);
    PlayerTextDrawSetOutline(playerid, BgLeft[playerid], 1);
    PlayerTextDrawSetShadow(playerid, BgLeft[playerid], 0);
    PlayerTextDrawAlignment(playerid, BgLeft[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BgLeft[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, BgLeft[playerid], 255);
    PlayerTextDrawBoxColour(playerid, BgLeft[playerid], INV_COLOR_BG);
    PlayerTextDrawUseBox(playerid, BgLeft[playerid], true);
    PlayerTextDrawSetProportional(playerid, BgLeft[playerid], true);
    PlayerTextDrawSetSelectable(playerid, BgLeft[playerid], false);

    return 1;
}

////////////////////////////////
// BOX CENTER ( GRID ITEM INV )
///////////////////////////////

// Định nghĩa mảng cho 3 lớp
new PlayerText:InvSlot_Box[MAX_PLAYERS][TOTAL_SLOTS];   // Lớp 1: Ô nền
new PlayerText:InvSlot_Model[MAX_PLAYERS][TOTAL_SLOTS]; // Lớp 2: Hình ảnh vật phẩm
new PlayerText:InvSlot_Text[MAX_PLAYERS][TOTAL_SLOTS];  // Lớp 3: Số lượng chữ

stock CreatePlayerInventoryGrid(playerid)
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
            
            // ==========================================================
            // LỚP 1: Ô NỀN (BOX COULOUR)
            // ==========================================================
            InvSlot_Box[playerid][slotId] = CreatePlayerTextDraw(playerid, posX, posY, "_");
            PlayerTextDrawFont(playerid, InvSlot_Box[playerid][slotId], TEXT_DRAW_FONT:1);
            PlayerTextDrawLetterSize(playerid, InvSlot_Box[playerid][slotId], 0.0, (slotSize / 9.5)); 
            PlayerTextDrawTextSize(playerid, InvSlot_Box[playerid][slotId], posX + slotSize, 0.0);
            PlayerTextDrawAlignment(playerid, InvSlot_Box[playerid][slotId], TEXT_DRAW_ALIGN:1);
            PlayerTextDrawBoxColour(playerid, InvSlot_Box[playerid][slotId], INV_COLOR_ITEM); // Màu xám tối 0x333333AA
            PlayerTextDrawUseBox(playerid, InvSlot_Box[playerid][slotId], true);
            PlayerTextDrawShow(playerid, InvSlot_Box[playerid][slotId]);
            
            // ==========================================================
            // LỚP 2: HÌNH ẢNH VẬT PHẨM (MODEL PREVIEW / SPRITE)
            // ==========================================================
            // Trùng tọa độ X, Y và kích thước với ô nền để hình nằm trọn bên trong
            InvSlot_Model[playerid][slotId] = CreatePlayerTextDraw(playerid, posX, posY, "_");
            PlayerTextDrawFont(playerid, InvSlot_Model[playerid][slotId], TEXT_DRAW_FONT:5); // Font 5 chuyên hiện Model 3D/Sprite
            PlayerTextDrawLetterSize(playerid, InvSlot_Model[playerid][slotId], 0.0, (slotSize / 9.5));
            PlayerTextDrawTextSize(playerid, InvSlot_Model[playerid][slotId], slotSize, slotSize); // Size của model bằng size ô
            PlayerTextDrawAlignment(playerid, InvSlot_Model[playerid][slotId], TEXT_DRAW_ALIGN:1);
            PlayerTextDrawColour(playerid, InvSlot_Model[playerid][slotId], -1);
            PlayerTextDrawBackgroundColour(playerid, InvSlot_Model[playerid][slotId], 0);
            PlayerTextDrawSetSelectable(playerid, InvSlot_Model[playerid][slotId], true);
            // Mặc định ban đầu chưa có đồ thì ẩn đi (hoặc set model = 0)
            PlayerTextDrawSetPreviewModel(playerid, InvSlot_Model[playerid][slotId], 0); 
            PlayerTextDrawShow(playerid, InvSlot_Model[playerid][slotId]);
            
            // ==========================================================
            // LỚP 3: CHỮ SỐ LƯỢNG (TEXT)
            // ==========================================================
            // Căn lề PHẢI (Alignment 3) để số tự động đẩy vào trong, không bị tràn ra ngoài ô
            // Tọa độ X dịch sang phải sát mép ô (posX + slotSize - 2.0 pixel đệm)
            // Tọa độ Y dịch xuống dưới đáy ô (posY + slotSize - 8.0 pixel chiều cao chữ)
            InvSlot_Text[playerid][slotId] = CreatePlayerTextDraw(playerid, (posX + slotSize - 2.0), (posY + slotSize - 8.0), "22");
            PlayerTextDrawFont(playerid, InvSlot_Text[playerid][slotId], TEXT_DRAW_FONT:1);
            PlayerTextDrawLetterSize(playerid, InvSlot_Text[playerid][slotId], 0.16, 0.7); // Chữ siêu nhỏ để vừa góc ô
            PlayerTextDrawAlignment(playerid, InvSlot_Text[playerid][slotId], TEXT_DRAW_ALIGN:3); // CĂN LỀ PHẢI
            PlayerTextDrawColour(playerid, InvSlot_Text[playerid][slotId], -1); // Chữ màu trắng
            PlayerTextDrawSetShadow(playerid, InvSlot_Text[playerid][slotId], 0); // Đổ bóng cho rõ số trên nền vật phẩm
            PlayerTextDrawShow(playerid, InvSlot_Text[playerid][slotId]);
            
            slotId++;
        }
    }

    return 1;
}

////////////////////////////
// BOX LEFT
////////////////////////////
stock InvInfoItem(playerid) {
    new PlayerText:header_title[MAX_PLAYERS];
    new PlayerText:header_Pname[MAX_PLAYERS];
    new PlayerText:header_weight[MAX_PLAYERS];

    new PlayerText:btn_use[MAX_PLAYERS];
    new PlayerText:btn_give[MAX_PLAYERS];
    new PlayerText:btn_drop[MAX_PLAYERS];
    new PlayerText:btn_close[MAX_PLAYERS];

    new PlayerText:info_model[MAX_PLAYERS];
    new PlayerText:info_text1[MAX_PLAYERS];
    new PlayerText:info_text2[MAX_PLAYERS];
    new PlayerText:info_text3[MAX_PLAYERS];
    new PlayerText:info_text4[MAX_PLAYERS];
    new PlayerText:info_text5[MAX_PLAYERS];

    header_title[playerid] = CreatePlayerTextDraw(playerid, 365.0, 246.0, "inventory");
    PlayerTextDrawFont(playerid, header_title[playerid], TEXT_DRAW_FONT:2);
    PlayerTextDrawLetterSize(playerid, header_title[playerid], 0.2, 1.2);
    PlayerTextDrawTextSize(playerid, header_title[playerid], 425.0, 0.0);
    PlayerTextDrawSetOutline(playerid, header_title[playerid], 0);
    PlayerTextDrawSetShadow(playerid, header_title[playerid], 0);
    PlayerTextDrawAlignment(playerid, header_title[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, header_title[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, header_title[playerid], 255);
    PlayerTextDrawBoxColour(playerid, header_title[playerid], 50);
    PlayerTextDrawUseBox(playerid, header_title[playerid], false);
    PlayerTextDrawSetProportional(playerid, header_title[playerid], true);
    PlayerTextDrawSetSelectable(playerid, header_title[playerid], false);

    header_Pname[playerid] = CreatePlayerTextDraw(playerid, 493.0, 246.0, "Player_Name");
    PlayerTextDrawFont(playerid, header_Pname[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, header_Pname[playerid], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, header_Pname[playerid], 425.5, 89.5);
    PlayerTextDrawSetOutline(playerid, header_Pname[playerid], 0);
    PlayerTextDrawSetShadow(playerid, header_Pname[playerid], 0);
    PlayerTextDrawAlignment(playerid, header_Pname[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, header_Pname[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, header_Pname[playerid], 255);
    PlayerTextDrawBoxColour(playerid, header_Pname[playerid], 50);
    PlayerTextDrawUseBox(playerid, header_Pname[playerid], false);
    PlayerTextDrawSetProportional(playerid, header_Pname[playerid], true);
    PlayerTextDrawSetSelectable(playerid, header_Pname[playerid], false);

    header_weight[playerid] = CreatePlayerTextDraw(playerid, 619.0, 248.0, "24.0kg / ~g~40.0kg");
    PlayerTextDrawFont(playerid, header_weight[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, header_weight[playerid], 0.1, 1.0);
    PlayerTextDrawTextSize(playerid, header_weight[playerid], 425.5, 89.5);
    PlayerTextDrawSetOutline(playerid, header_weight[playerid], 0);
    PlayerTextDrawSetShadow(playerid, header_weight[playerid], 0);
    PlayerTextDrawAlignment(playerid, header_weight[playerid], TEXT_DRAW_ALIGN:3);
    PlayerTextDrawColour(playerid, header_weight[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, header_weight[playerid], 255);
    PlayerTextDrawBoxColour(playerid, header_weight[playerid], 50);
    PlayerTextDrawUseBox(playerid, header_weight[playerid], false);
    PlayerTextDrawSetProportional(playerid, header_weight[playerid], true);
    PlayerTextDrawSetSelectable(playerid, header_weight[playerid], false);

    btn_use[playerid] = CreatePlayerTextDraw(playerid, 275.0, 423.5, "Use");
    PlayerTextDrawFont(playerid, btn_use[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, btn_use[playerid], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, btn_use[playerid], 9.0, 30.0);
    PlayerTextDrawSetOutline(playerid, btn_use[playerid], 0);
    PlayerTextDrawSetShadow(playerid, btn_use[playerid], 0);
    PlayerTextDrawAlignment(playerid, btn_use[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, btn_use[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, btn_use[playerid], 255);
    PlayerTextDrawBoxColour(playerid, btn_use[playerid], 1687547336);
    PlayerTextDrawUseBox(playerid, btn_use[playerid], true);
    PlayerTextDrawSetProportional(playerid, btn_use[playerid], true);
    PlayerTextDrawSetSelectable(playerid, btn_use[playerid], true);

    btn_give[playerid] = CreatePlayerTextDraw(playerid, 310.0, 423.5, "Give");
    PlayerTextDrawFont(playerid, btn_give[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, btn_give[playerid], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, btn_give[playerid], 9.0, 32.0);
    PlayerTextDrawSetOutline(playerid, btn_give[playerid], 0);
    PlayerTextDrawSetShadow(playerid, btn_give[playerid], 0);
    PlayerTextDrawAlignment(playerid, btn_give[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, btn_give[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, btn_give[playerid], 255);
    PlayerTextDrawBoxColour(playerid, btn_give[playerid], 1687547336);
    PlayerTextDrawUseBox(playerid, btn_give[playerid], true);
    PlayerTextDrawSetProportional(playerid, btn_give[playerid], true);
    PlayerTextDrawSetSelectable(playerid, btn_give[playerid], true);

    btn_drop[playerid] = CreatePlayerTextDraw(playerid, 345.0, 423.5, "Drop");
    PlayerTextDrawFont(playerid, btn_drop[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, btn_drop[playerid], 0.2, 1.1);
    PlayerTextDrawTextSize(playerid, btn_drop[playerid], 9.0, 30.0);
    PlayerTextDrawSetOutline(playerid, btn_drop[playerid], 0);
    PlayerTextDrawSetShadow(playerid, btn_drop[playerid], 0);
    PlayerTextDrawAlignment(playerid, btn_drop[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, btn_drop[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, btn_drop[playerid], 255);
    PlayerTextDrawBoxColour(playerid, btn_drop[playerid], 1687547336);
    PlayerTextDrawUseBox(playerid, btn_drop[playerid], true);
    PlayerTextDrawSetProportional(playerid, btn_drop[playerid], true);
    PlayerTextDrawSetSelectable(playerid, btn_drop[playerid], true);

    info_model[playerid] = CreatePlayerTextDraw(playerid, 277.0, 262.0, "HUD:radar_burgershot");
    PlayerTextDrawFont(playerid, info_model[playerid], TEXT_DRAW_FONT:5);
    PlayerTextDrawLetterSize(playerid, info_model[playerid], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, info_model[playerid], 66.0, 62.5);
    PlayerTextDrawSetOutline(playerid, info_model[playerid], 1);
    PlayerTextDrawSetShadow(playerid, info_model[playerid], 0);
    PlayerTextDrawAlignment(playerid, info_model[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, info_model[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, info_model[playerid], 0);
    PlayerTextDrawBoxColour(playerid, info_model[playerid], -206);
    PlayerTextDrawUseBox(playerid, info_model[playerid], true);
    PlayerTextDrawSetProportional(playerid, info_model[playerid], true);
    PlayerTextDrawSetSelectable(playerid, info_model[playerid], false);
    PlayerTextDrawSetPreviewModel(playerid, info_model[playerid], 2703);
    PlayerTextDrawSetPreviewRot(playerid, info_model[playerid], 248.0, 0.0, -15.0, 1.0);
    PlayerTextDrawSetPreviewVehicleColours(playerid, info_model[playerid], 1, 1);

    info_text1[playerid] = CreatePlayerTextDraw(playerid, 265.0, 324.0, "Name: ~y~Burger");
    PlayerTextDrawFont(playerid, info_text1[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, info_text1[playerid], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, info_text1[playerid], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, info_text1[playerid], 0);
    PlayerTextDrawSetShadow(playerid, info_text1[playerid], 0);
    PlayerTextDrawAlignment(playerid, info_text1[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, info_text1[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, info_text1[playerid], 255);
    PlayerTextDrawBoxColour(playerid, info_text1[playerid], -206);
    PlayerTextDrawUseBox(playerid, info_text1[playerid], false);
    PlayerTextDrawSetProportional(playerid, info_text1[playerid], true);
    PlayerTextDrawSetSelectable(playerid, info_text1[playerid], false);

    info_text2[playerid] = CreatePlayerTextDraw(playerid, 265.0, 333.0, "Category: ~b~Food");
    PlayerTextDrawFont(playerid, info_text2[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, info_text2[playerid], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, info_text2[playerid], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, info_text2[playerid], 0);
    PlayerTextDrawSetShadow(playerid, info_text2[playerid], 0);
    PlayerTextDrawAlignment(playerid, info_text2[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, info_text2[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, info_text2[playerid], 255);
    PlayerTextDrawBoxColour(playerid, info_text2[playerid], -206);
    PlayerTextDrawUseBox(playerid, info_text2[playerid], false);
    PlayerTextDrawSetProportional(playerid, info_text2[playerid], true);
    PlayerTextDrawSetSelectable(playerid, info_text2[playerid], false);

    info_text3[playerid] = CreatePlayerTextDraw(playerid, 265.0, 343.0, "Weight: ~g~0.2kg");
    PlayerTextDrawFont(playerid, info_text3[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, info_text3[playerid], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, info_text3[playerid], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, info_text3[playerid], 0);
    PlayerTextDrawSetShadow(playerid, info_text3[playerid], 0);
    PlayerTextDrawAlignment(playerid, info_text3[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, info_text3[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, info_text3[playerid], 255);
    PlayerTextDrawBoxColour(playerid, info_text3[playerid], -206);
    PlayerTextDrawUseBox(playerid, info_text3[playerid], false);
    PlayerTextDrawSetProportional(playerid, info_text3[playerid], true);
    PlayerTextDrawSetSelectable(playerid, info_text3[playerid], false);

    info_text4[playerid] = CreatePlayerTextDraw(playerid, 265.0, 352.0, "Quantity: ~r~5");
    PlayerTextDrawFont(playerid, info_text4[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, info_text4[playerid], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, info_text4[playerid], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, info_text4[playerid], 0);
    PlayerTextDrawSetShadow(playerid, info_text4[playerid], 0);
    PlayerTextDrawAlignment(playerid, info_text4[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, info_text4[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, info_text4[playerid], 255);
    PlayerTextDrawBoxColour(playerid, info_text4[playerid], -206);
    PlayerTextDrawUseBox(playerid, info_text4[playerid], false);
    PlayerTextDrawSetProportional(playerid, info_text4[playerid], true);
    PlayerTextDrawSetSelectable(playerid, info_text4[playerid], false);

    info_text5[playerid] = CreatePlayerTextDraw(playerid, 265.0, 364.0, "~b~Description: ~w~A basic cooked burger. Restores hunger when consumed.");
    PlayerTextDrawFont(playerid, info_text5[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, info_text5[playerid], 0.2, 0.8);
    PlayerTextDrawTextSize(playerid, info_text5[playerid], 355.5, 16.0);
    PlayerTextDrawSetOutline(playerid, info_text5[playerid], 0);
    PlayerTextDrawSetShadow(playerid, info_text5[playerid], 0);
    PlayerTextDrawAlignment(playerid, info_text5[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, info_text5[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, info_text5[playerid], 255);
    PlayerTextDrawBoxColour(playerid, info_text5[playerid], -206);
    PlayerTextDrawUseBox(playerid, info_text5[playerid], false);
    PlayerTextDrawSetProportional(playerid, info_text5[playerid], true);
    PlayerTextDrawSetSelectable(playerid, info_text5[playerid], false);

    btn_close[playerid] = CreatePlayerTextDraw(playerid, 350.0, 245.5, "ld_beat:cross");
    PlayerTextDrawFont(playerid, btn_close[playerid], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, btn_close[playerid], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, btn_close[playerid], 11.5, 13.5);
    PlayerTextDrawSetOutline(playerid, btn_close[playerid], 1);
    PlayerTextDrawSetShadow(playerid, btn_close[playerid], 0);
    PlayerTextDrawAlignment(playerid, btn_close[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, btn_close[playerid], 1687547336);
    PlayerTextDrawBackgroundColour(playerid, btn_close[playerid], -1094795776);
    PlayerTextDrawBoxColour(playerid, btn_close[playerid], 65280);
    PlayerTextDrawUseBox(playerid, btn_close[playerid], true);
    PlayerTextDrawSetProportional(playerid, btn_close[playerid], true);
    PlayerTextDrawSetSelectable(playerid, btn_close[playerid], true);

    PlayerTextDrawShow(playerid, header_title[playerid]);
    PlayerTextDrawShow(playerid, header_Pname[playerid]);
    PlayerTextDrawShow(playerid, header_weight[playerid]);

    PlayerTextDrawShow(playerid, info_model[playerid]);
    PlayerTextDrawShow(playerid, info_text1[playerid]);
    PlayerTextDrawShow(playerid, info_text2[playerid]);
    PlayerTextDrawShow(playerid, info_text3[playerid]);
    PlayerTextDrawShow(playerid, info_text4[playerid]);
    PlayerTextDrawShow(playerid, info_text5[playerid]);

    PlayerTextDrawShow(playerid, btn_use[playerid]);
    PlayerTextDrawShow(playerid, btn_give[playerid]);
    PlayerTextDrawShow(playerid, btn_drop[playerid]);

    PlayerTextDrawShow(playerid, btn_close[playerid]);

    return 1;
}

//////////////////////////////////////////
// BOX RIGHT
/////////////////////////////////////////
new PlayerText:Equip_CharPreview[MAX_PLAYERS];
new PlayerText:Equip_Box[MAX_PLAYERS][16];   // Tăng lên 16 ô
new PlayerText:Equip_Model[MAX_PLAYERS][16]; // Tăng lên 16 ô
new PlayerBar:Equip_Bar[MAX_PLAYERS][16];   // Tăng lên 16 ô

stock CreatePlayerRpgEquipment(playerid)
{
    // ==========================================================
    // 1. MODEL NHÂN VẬT 3D (Giữ nguyên vị trí nửa trên)
    // ==========================================================
    Equip_CharPreview[playerid] = CreatePlayerTextDraw(playerid, 537.5, 268.0, "_");
    PlayerTextDrawFont(playerid, Equip_CharPreview[playerid], TEXT_DRAW_FONT:5);
    PlayerTextDrawLetterSize(playerid, Equip_CharPreview[playerid], 0.0, (80.0 / 9.5)); 
    PlayerTextDrawTextSize(playerid, Equip_CharPreview[playerid], 80.0, 80.0);
    PlayerTextDrawAlignment(playerid, Equip_CharPreview[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawBoxColour(playerid, Equip_CharPreview[playerid], 0);
    PlayerTextDrawBackgroundColour(playerid, Equip_CharPreview[playerid], 0);
    PlayerTextDrawUseBox(playerid, Equip_CharPreview[playerid], true);
    PlayerTextDrawColour(playerid, Equip_CharPreview[playerid], -1);
    PlayerTextDrawSetPreviewModel(playerid, Equip_CharPreview[playerid], GetPlayerSkin(playerid)); 
    PlayerTextDrawSetPreviewRot(playerid, Equip_CharPreview[playerid], -10.0, 0.0, -20.0, 1.0);
    PlayerTextDrawShow(playerid, Equip_CharPreview[playerid]);

    // ==========================================================
    // 2. TẠO 4 Ô TRANG BỊ TRÊN (ID 0 đến 3)
    // ==========================================================
    new Float:topSlotsX[4] = {543.0, 543.0, 600.0, 600.0};
    new Float:topSlotsY[4] = {275.0, 305.0, 275.0, 305.0};
    new Float:sSize = 16.0;

    for(new i = 0; i < 4; i++)
    {
        CreateSingleEquipSlot(playerid, i, topSlotsX[i], topSlotsY[i], sSize);
    }

    // ==========================================================
    // 3. TẠO 12 Ô LƯỚI PHÍA DƯỚI (Mở rộng thành 3 Hàng x 4 Cột)
    // ID chạy tiếp tục từ 4 đến 15
    // ==========================================================
    new Float:bottomStartX = 540.5;
    new Float:bottomStartY = 354.0; // Đẩy lên Y=354 để lấy thêm không gian cho hàng thứ 3
    new Float:spacingX = 5.0;
    new Float:spacingY = 12.0; // Khoảng cách dọc giữa các hàng ô dưới
    
    new slotId = 4;
    for(new r = 0; r < 3; r++) // TĂNG LÊN 3 HÀNG dọc
    {
        for(new c = 0; c < 4; c++) // 4 Cột ngang
        {
            new Float:posX = bottomStartX + (c * (sSize + spacingX));
            new Float:posY = bottomStartY + (r * (sSize + spacingY));
            
            CreateSingleEquipSlot(playerid, slotId, posX, posY, sSize);
            slotId++;
        }
    }
}

// Giữ nguyên hàm phụ trợ CreateSingleEquipSlot bên dưới...

// Hàm phụ trợ tạo nhanh từng ô để code gọn gàng, không bị lặp dòng
stock CreateSingleEquipSlot(playerid, slotId, Float:posX, Float:posY, Float:size)
{
    Equip_Bar[playerid][slotId] = CreatePlayerProgressBar(playerid, posX, posY + size - 0.5, size, 1.0);
    SetPlayerProgressBarValue(playerid, Equip_Bar[playerid][slotId], 30.0);
    ShowPlayerProgressBar(playerid, Equip_Bar[playerid][slotId]);

    // Lớp 1: Box nền
    Equip_Box[playerid][slotId] = CreatePlayerTextDraw(playerid, posX, posY, "_");
    PlayerTextDrawFont(playerid, Equip_Box[playerid][slotId], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, Equip_Box[playerid][slotId], 0.0, (size / 9.5));
    PlayerTextDrawTextSize(playerid, Equip_Box[playerid][slotId], posX + size, 0.0);
    PlayerTextDrawAlignment(playerid, Equip_Box[playerid][slotId], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawBoxColour(playerid, Equip_Box[playerid][slotId], 255);
    PlayerTextDrawBackgroundColour(playerid, Equip_Box[playerid][slotId], 255);
    PlayerTextDrawUseBox(playerid, Equip_Box[playerid][slotId], true);
    PlayerTextDrawShow(playerid, Equip_Box[playerid][slotId]);

    // Lớp 2: Model Preview vật phẩm
    Equip_Model[playerid][slotId] = CreatePlayerTextDraw(playerid, posX, posY, "_");
    PlayerTextDrawFont(playerid, Equip_Model[playerid][slotId], TEXT_DRAW_FONT:5);
    PlayerTextDrawLetterSize(playerid, Equip_Model[playerid][slotId], 0.0, (size / 9.5));
    PlayerTextDrawTextSize(playerid, Equip_Model[playerid][slotId], size, size);
    PlayerTextDrawAlignment(playerid, Equip_Model[playerid][slotId], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawBackgroundColour(playerid, Equip_Model[playerid][slotId], 0);
    PlayerTextDrawColour(playerid, Equip_Model[playerid][slotId], -1);
    PlayerTextDrawSetSelectable(playerid, Equip_Model[playerid][slotId], true);
    PlayerTextDrawSetPreviewModel(playerid, Equip_Model[playerid][slotId], 0);
    PlayerTextDrawShow(playerid, Equip_Model[playerid][slotId]);
}

//////////////////////////////////////////
// Navigation Bar
/////////////////////////////////////////
stock InvNavigationBtn(playerid) {
    new PlayerText:inv_navBox[MAX_PLAYERS];
    new PlayerText:inv_navLeft[MAX_PLAYERS];
    new PlayerText:inv_navRight[MAX_PLAYERS];
    new PlayerText:inv_navPageInfo[MAX_PLAYERS];

    inv_navBox[playerid] = CreatePlayerTextDraw(playerid, 450.0, 433.0, "_");
    PlayerTextDrawFont(playerid, inv_navBox[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, inv_navBox[playerid], 0.6, 0.3);
    PlayerTextDrawTextSize(playerid, inv_navBox[playerid], 298.5, 34.5);
    PlayerTextDrawSetOutline(playerid, inv_navBox[playerid], 1);
    PlayerTextDrawSetShadow(playerid, inv_navBox[playerid], 0);
    PlayerTextDrawAlignment(playerid, inv_navBox[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, inv_navBox[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, inv_navBox[playerid], 255);
    PlayerTextDrawBoxColour(playerid, inv_navBox[playerid], INV_COLOR_BG);
    PlayerTextDrawUseBox(playerid, inv_navBox[playerid], true);
    PlayerTextDrawSetProportional(playerid, inv_navBox[playerid], true);
    PlayerTextDrawSetSelectable(playerid, inv_navBox[playerid], false);

    inv_navLeft[playerid] = CreatePlayerTextDraw(playerid, 432.0, 431.0, "ld_beat:left");
    PlayerTextDrawFont(playerid, inv_navLeft[playerid], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, inv_navLeft[playerid], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, inv_navLeft[playerid], 7.5, 7.5);
    PlayerTextDrawSetOutline(playerid, inv_navLeft[playerid], 1);
    PlayerTextDrawSetShadow(playerid, inv_navLeft[playerid], 0);
    PlayerTextDrawAlignment(playerid, inv_navLeft[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, inv_navLeft[playerid], 1687547336);
    PlayerTextDrawBackgroundColour(playerid, inv_navLeft[playerid], -1094795776);
    PlayerTextDrawBoxColour(playerid, inv_navLeft[playerid], 65280);
    PlayerTextDrawUseBox(playerid, inv_navLeft[playerid], true);
    PlayerTextDrawSetProportional(playerid, inv_navLeft[playerid], true);
    PlayerTextDrawSetSelectable(playerid, inv_navLeft[playerid], true);

    inv_navRight[playerid] = CreatePlayerTextDraw(playerid, 461.0, 431.0, "ld_beat:right");
    PlayerTextDrawFont(playerid, inv_navRight[playerid], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, inv_navRight[playerid], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, inv_navRight[playerid], 7.5, 7.5);
    PlayerTextDrawSetOutline(playerid, inv_navRight[playerid], 1);
    PlayerTextDrawSetShadow(playerid, inv_navRight[playerid], 0);
    PlayerTextDrawAlignment(playerid, inv_navRight[playerid], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, inv_navRight[playerid], 1687547336);
    PlayerTextDrawBackgroundColour(playerid, inv_navRight[playerid], -1094795776);
    PlayerTextDrawBoxColour(playerid, inv_navRight[playerid], 65280);
    PlayerTextDrawUseBox(playerid, inv_navRight[playerid], true);
    PlayerTextDrawSetProportional(playerid, inv_navRight[playerid], true);
    PlayerTextDrawSetSelectable(playerid, inv_navRight[playerid], true);

    inv_navPageInfo[playerid] = CreatePlayerTextDraw(playerid, 450.0, 432.5, "1 /~r~100");
    PlayerTextDrawFont(playerid, inv_navPageInfo[playerid], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, inv_navPageInfo[playerid], 0.1, 0.4);
    PlayerTextDrawTextSize(playerid, inv_navPageInfo[playerid], 459.0, 17.5);
    PlayerTextDrawSetOutline(playerid, inv_navPageInfo[playerid], 0);
    PlayerTextDrawSetShadow(playerid, inv_navPageInfo[playerid], 0);
    PlayerTextDrawAlignment(playerid, inv_navPageInfo[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, inv_navPageInfo[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, inv_navPageInfo[playerid], 255);
    PlayerTextDrawBoxColour(playerid, inv_navPageInfo[playerid], -421070286);
    PlayerTextDrawUseBox(playerid, inv_navPageInfo[playerid], false);
    PlayerTextDrawSetProportional(playerid, inv_navPageInfo[playerid], true);
    PlayerTextDrawSetSelectable(playerid, inv_navPageInfo[playerid], false);
    
    PlayerTextDrawShow(playerid, inv_navBox[playerid]);
    PlayerTextDrawShow(playerid, inv_navLeft[playerid]);
    PlayerTextDrawShow(playerid, inv_navRight[playerid]);
    PlayerTextDrawShow(playerid, inv_navPageInfo[playerid]);
    return 1;
}


hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(playertextid == InvSlot_Model[playerid][0]) {
        PlayerTextDrawBoxColour(playerid, InvSlot_Box[playerid][0], INV_COLOR_HOVER);
        PlayerTextDrawShow(playerid, InvSlot_Box[playerid][0]);
        return 1;
    }
	return 1;
}

CMD:inv(playerid, params[]) {
    CreateInvTD(playerid);
    PlayerTextDrawShow(playerid, Background_Under[playerid]);
    PlayerTextDrawShow(playerid, BgHeader[playerid]);
    PlayerTextDrawShow(playerid, TxtHeaderTitle[playerid]);
    PlayerTextDrawShow(playerid, TxtHeaderName[playerid]);
    PlayerTextDrawShow(playerid, TxtHeaderStats[playerid]);

    InvNavigationBtn(playerid);

    PlayerTextDrawShow(playerid, BgRight[playerid]);
    CreatePlayerRpgEquipment(playerid);

    PlayerTextDrawShow(playerid, BgCenter[playerid]);
    CreatePlayerInventoryGrid(playerid);

    PlayerTextDrawShow(playerid, BgLeft[playerid]);
    InvInfoItem(playerid);

    SelectTextDraw(playerid, INV_COLOR_HOVER);
    return 1;
}