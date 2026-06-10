#include <YSI_Coding\y_hooks>

enum {
    LOADING_TYPE_NONE = 0,
    LOADING_TYPE_LOGIN,
    LOADING_TYPE_MAP,
    LOADING_TYPE_JOB
};

new PlayerText:LoadingTD[MAX_PLAYERS][4]; 
new PlayerBar:LoadingProgressBar[MAX_PLAYERS];

new Float:p_LoadingProgress[MAX_PLAYERS];
new p_LoadingType[MAX_PLAYERS];
new loadingTimer[MAX_PLAYERS];

hook OnPlayerConnect(playerid)
{
    p_LoadingProgress[playerid] = 0.0;
    p_LoadingType[playerid] = LOADING_TYPE_NONE;

    LoadingTD[playerid][0] = CreatePlayerTextDraw(playerid, 320.0, 255.0, "_");
    PlayerTextDrawFont(playerid, LoadingTD[playerid][0], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, LoadingTD[playerid][0], 0.6, 5.6);
    PlayerTextDrawTextSize(playerid, LoadingTD[playerid][0], 298.5, 186.0);
    PlayerTextDrawSetOutline(playerid, LoadingTD[playerid][0], 1);
    PlayerTextDrawSetShadow(playerid, LoadingTD[playerid][0], 0);
    PlayerTextDrawAlignment(playerid, LoadingTD[playerid][0], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, LoadingTD[playerid][0], -1);
    PlayerTextDrawBackgroundColour(playerid, LoadingTD[playerid][0], 255);
    PlayerTextDrawBoxColour(playerid, LoadingTD[playerid][0], -146);
    PlayerTextDrawUseBox(playerid, LoadingTD[playerid][0], true);
    PlayerTextDrawSetProportional(playerid, LoadingTD[playerid][0], true);
    PlayerTextDrawSetSelectable(playerid, LoadingTD[playerid][0], false);

    LoadingTD[playerid][1] = CreatePlayerTextDraw(playerid, 320.0, 256.0, "_");
    PlayerTextDrawFont(playerid, LoadingTD[playerid][1], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, LoadingTD[playerid][1], 0.6, 5.3);
    PlayerTextDrawTextSize(playerid, LoadingTD[playerid][1], 298.5, 183.5);
    PlayerTextDrawSetOutline(playerid, LoadingTD[playerid][1], 1);
    PlayerTextDrawSetShadow(playerid, LoadingTD[playerid][1], 0);
    PlayerTextDrawAlignment(playerid, LoadingTD[playerid][1], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, LoadingTD[playerid][1], -1);
    PlayerTextDrawBackgroundColour(playerid, LoadingTD[playerid][1], 255);
    PlayerTextDrawBoxColour(playerid, LoadingTD[playerid][1], 180);
    PlayerTextDrawUseBox(playerid, LoadingTD[playerid][1], true);
    PlayerTextDrawSetProportional(playerid, LoadingTD[playerid][1], true);
    PlayerTextDrawSetSelectable(playerid, LoadingTD[playerid][1], false);

    LoadingTD[playerid][2] = CreatePlayerTextDraw(playerid, 320.0, 263.0, "Dang Tai Du Lieu...");
    PlayerTextDrawFont(playerid, LoadingTD[playerid][2], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, LoadingTD[playerid][2], 0.35, 1.4);
    PlayerTextDrawTextSize(playerid, LoadingTD[playerid][2], 400.0, 180.5);
    PlayerTextDrawSetOutline(playerid, LoadingTD[playerid][2], 0);
    PlayerTextDrawSetShadow(playerid, LoadingTD[playerid][2], 1);
    PlayerTextDrawAlignment(playerid, LoadingTD[playerid][2], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, LoadingTD[playerid][2], -36);
    PlayerTextDrawBackgroundColour(playerid, LoadingTD[playerid][2], 255);
    PlayerTextDrawUseBox(playerid, LoadingTD[playerid][2], false);
    PlayerTextDrawSetProportional(playerid, LoadingTD[playerid][2], true);
    PlayerTextDrawSetSelectable(playerid, LoadingTD[playerid][2], false);

    LoadingTD[playerid][3] = CreatePlayerTextDraw(playerid, 320.0, 292.0, "0%");
    PlayerTextDrawFont(playerid, LoadingTD[playerid][3], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, LoadingTD[playerid][3], 0.25, 1.1);
    PlayerTextDrawAlignment(playerid, LoadingTD[playerid][3], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, LoadingTD[playerid][3], 0x33FF3340); 
    PlayerTextDrawSetShadow(playerid, LoadingTD[playerid][3], 1);

    LoadingProgressBar[playerid] = CreatePlayerProgressBar(playerid, 227.0, 290.0, 186.0, 15.5, -156, 100.0, BAR_DIRECTION_RIGHT);
    return 1;
}

stock ShowPlayerLoading(playerid, const title[], loading_type)
{
    p_LoadingProgress[playerid] = 0.0;
    p_LoadingType[playerid] = loading_type;

    PlayerTextDrawSetString(playerid, LoadingTD[playerid][2], title);
    PlayerTextDrawSetString(playerid, LoadingTD[playerid][3], "0%");
    SetPlayerProgressBarValue(playerid, LoadingProgressBar[playerid], 0.0);

    for(new i = 0; i < 4; i++) {
        PlayerTextDrawShow(playerid, LoadingTD[playerid][i]);
    }
    ShowPlayerProgressBar(playerid, LoadingProgressBar[playerid]);

    loadingTimer[playerid] = SetTimerEx("LoadingTimer", 100, true, "i", playerid);
}

stock HidePlayerLoading(playerid)
{
    p_LoadingType[playerid] = LOADING_TYPE_NONE;
    p_LoadingProgress[playerid] = 0.0;

    for(new i = 0; i < 4; i++) {
        PlayerTextDrawHide(playerid, LoadingTD[playerid][i]);
    }
    HidePlayerProgressBar(playerid, LoadingProgressBar[playerid]);
}

stock UpdatePlayerLoading(playerid, Float:value)
{
    if(p_LoadingType[playerid] == LOADING_TYPE_NONE) return 0;

    p_LoadingProgress[playerid] += value;
    if(p_LoadingProgress[playerid] > 100.0) p_LoadingProgress[playerid] = 100.0;

    SetPlayerProgressBarValue(playerid, LoadingProgressBar[playerid], p_LoadingProgress[playerid]);
    ShowPlayerProgressBar(playerid, LoadingProgressBar[playerid]);

    new str[16];
    format(str, sizeof(str), "%d%%", floatround(p_LoadingProgress[playerid]));
    PlayerTextDrawSetString(playerid, LoadingTD[playerid][3], str);

    if(p_LoadingProgress[playerid] >= 100.0)
    {
        PlayerTextDrawSetString(playerid, LoadingTD[playerid][2], "Hoan Thanh");
        PlayerTextDrawColour(playerid, LoadingTD[playerid][2], COLOR_CYAN);
        PlayerTextDrawShow(playerid, LoadingTD[playerid][2]);
        KillTimer(loadingTimer[playerid]); 
        SetTimerEx("OnPlayerFinishLoading", 1000, false, "i", playerid);
    }
    return 1;
}

forward OnPlayerFinishLoading(playerid);
public OnPlayerFinishLoading(playerid)
{
    PlayerTextDrawColour(playerid, LoadingTD[playerid][2], -1);
    HidePlayerLoading(playerid);
    switch(p_LoadingType[playerid])
    {
        case LOADING_TYPE_LOGIN:
        {
            SendClientMessage(playerid, -1, "Dang nhap thanh cong! Du lieu nhan vat da tai xong.");
        }
        case LOADING_TYPE_MAP:
        {
            SendClientMessage(playerid, -1, "Da tai xong Map khu vuc VIP.");
            SetPlayerPos(playerid, 123.45, 678.90, 10.0); 
        }
        case LOADING_TYPE_JOB:
        {
            SendClientMessage(playerid, -1, "Ban da hoan thanh cong viec va nhan duoc $500!");
            GivePlayerMoney(playerid, 500);
        }
    }
   
    return 1;
}

CMD:testloading(playerid, params[])
{
    ShowPlayerLoading(playerid, "Dang lay du lieu", LOADING_TYPE_JOB);
    return 1;
}

forward LoadingTimer(playerid);
public LoadingTimer(playerid)
{
    if(p_LoadingType[playerid] == LOADING_TYPE_NONE) return KillTimer(loadingTimer[playerid]); 
    UpdatePlayerLoading(playerid, 2.5); 
    return 1;
}