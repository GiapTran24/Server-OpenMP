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
    PlayerTextDrawAlignment(playerid, info_model[playerid], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, info_model[playerid], -1);
    PlayerTextDrawBackgroundColour(playerid, info_model[playerid], 1687547136);
    PlayerTextDrawBoxColour(playerid, info_model[playerid], -206);
    PlayerTextDrawUseBox(playerid, info_model[playerid], true);
    PlayerTextDrawSetProportional(playerid, info_model[playerid], true);
    PlayerTextDrawSetSelectable(playerid, info_model[playerid], false);
    PlayerTextDrawSetPreviewModel(playerid, info_model[playerid], 2703);
    PlayerTextDrawSetPreviewRot(playerid, info_model[playerid], 248.0, 0.0, -15.0, 1.0);
    PlayerTextDrawSetPreviewVehCol(playerid, info_model[playerid], 1, 1);

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