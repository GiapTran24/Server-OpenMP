//-----------------------------------------------------------------------------
// Functions Support
//-----------------------------------------------------------------------------

stock GetPlayerNameEx(playerid)
{
    static name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof(name));
    return name;
}

stock GetPlayerCash(playerid)
{
    return PlayerInfo[playerid][pCash];
}

stock GivePlayerCash(playerid, amount)
{
    PlayerInfo[playerid][pCash] += amount;

    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, PlayerInfo[playerid][pCash]);

    return 1;
}

stock SendClientMessageEx(playerid, color, const string[])
{
	SendClientMessage(playerid, color, string);
	return 1;
}

stock SendClientMessageToAllEx(color, const string[])
{
	foreach(new i: Player)
	{
		SendClientMessage(i, color, string);
	}
	return 1;
}

stock IsInvalidSkin(skin) {
	if(!(0 <= skin <= 299)) return 1;
    return 0;
}

stock GetAdminName(level)
{
    new str[32];
    for(new i = 0; i < sizeof(AdminLevels); i++)
    {
        if(AdminLevels[i][adminLevel] == level) {
            format(str, sizeof(str), "%s", AdminLevels[i][adminName]);
            return str;
        }
    }
    format(str, sizeof(str), "Unknown (%d)", level);
    return str;
}

// Thong bao
stock ShowNotification(playerid, const title[], const message[], e_NOTIFY_TYPE:type, duration = 5000)
{
    // Neu da co thong bao dang hien thi thi an no di truoc khi hien thong bao moi
    if(PlayerNotification[playerid][nt_IsShowing]) {
        HidePlayerNotification(playerid);
    }

    // 1. Theme
    new titleColor, barColor;
    switch(type)
    {
        case NOTIFY_TYPE_SUCCESS: { titleColor = 0x2ECC71FF; barColor = 0x27AE60FF; } // Green
        case NOTIFY_TYPE_ERROR:   { titleColor = 0xE74C3CFF; barColor = 0xC0392BFF; } // Red
        case NOTIFY_TYPE_WARNING: { titleColor = 0xF1C40FFF; barColor = 0xD35400FF; } // Yellow
        default:                  { titleColor = 0x3498DBFF; barColor = 0x2980B9FF; } // Blue (Info)
    }

    // Toa do goc (640x448)
    new Float:posX = 460.0;
    new Float:posY = 150.0;
    new Float:width = 160.0;
    new Float:height = 65.0;

    // 2. Background
    PlayerNotification[playerid][nt_BG] = CreatePlayerTextDraw(playerid, posX, posY, "_");
    PlayerTextDrawLetterSize(playerid, PlayerNotification[playerid][nt_BG], 0.0, height / 10.0);
    PlayerTextDrawTextSize(playerid, PlayerNotification[playerid][nt_BG], posX + width, 0.0);
    PlayerTextDrawUseBox(playerid, PlayerNotification[playerid][nt_BG], bool:1);
    PlayerTextDrawBoxColour(playerid, PlayerNotification[playerid][nt_BG], 0x000000AA); // Alpha AA (~66%)
    PlayerTextDrawShow(playerid, PlayerNotification[playerid][nt_BG]);

    // 3. Title
    PlayerNotification[playerid][nt_Title] = CreatePlayerTextDraw(playerid, posX + 5.0, posY + 4.0, title);
    PlayerTextDrawFont(playerid, PlayerNotification[playerid][nt_Title], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, PlayerNotification[playerid][nt_Title], 0.28, 1.1);
    PlayerTextDrawColour(playerid, PlayerNotification[playerid][nt_Title], titleColor);
    PlayerTextDrawSetProportional(playerid, PlayerNotification[playerid][nt_Title], bool:1);
    PlayerTextDrawShow(playerid, PlayerNotification[playerid][nt_Title]);

    // 4. Message
    PlayerNotification[playerid][nt_Body] = CreatePlayerTextDraw(playerid, posX + 5.0, posY + 18.0, message);
    PlayerTextDrawFont(playerid, PlayerNotification[playerid][nt_Body], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, PlayerNotification[playerid][nt_Body], 0.23, 0.9);
    PlayerTextDrawColour(playerid, PlayerNotification[playerid][nt_Body], 0xFFFFFFFF);
    PlayerTextDrawTextSize(playerid, PlayerNotification[playerid][nt_Body], (posX + width) - 5.0, 0.0); // Gioi han chieu rong de tu xuong hang
    PlayerTextDrawSetProportional(playerid, PlayerNotification[playerid][nt_Body], bool:1);
    PlayerTextDrawShow(playerid, PlayerNotification[playerid][nt_Body]);

    // 5. Bottom
    
    PlayerNotification[playerid][nt_Progress] = CreatePlayerTextDraw(playerid, posX, (posY + height) - 3.0, "_");
    PlayerTextDrawLetterSize(playerid, PlayerNotification[playerid][nt_Progress], 0.0, 0.3);
    PlayerTextDrawTextSize(playerid, PlayerNotification[playerid][nt_Progress], posX + width, 0.0);
    PlayerTextDrawUseBox(playerid, PlayerNotification[playerid][nt_Progress], bool:1);
    PlayerTextDrawBoxColour(playerid, PlayerNotification[playerid][nt_Progress], barColor);
    PlayerTextDrawShow(playerid, PlayerNotification[playerid][nt_Progress]);

    // Hen gio tat thong bao
    PlayerNotification[playerid][nt_Timer] = SetTimerEx("HidePlayerNotification", duration, false, "d", playerid);
    PlayerNotification[playerid][nt_IsShowing] = true;
    return 1;
}


forward HidePlayerNotification(playerid);
public HidePlayerNotification(playerid)
{
    if(!PlayerNotification[playerid][nt_IsShowing]) return 0;

    if(PlayerNotification[playerid][nt_BG] != INVALID_PLAYER_TEXT_DRAW) {
        PlayerTextDrawDestroy(playerid, PlayerNotification[playerid][nt_BG]);
        PlayerNotification[playerid][nt_BG] = INVALID_PLAYER_TEXT_DRAW;
    }
    if(PlayerNotification[playerid][nt_Title] != INVALID_PLAYER_TEXT_DRAW) {
        PlayerTextDrawDestroy(playerid, PlayerNotification[playerid][nt_Title]);
        PlayerNotification[playerid][nt_Title] = INVALID_PLAYER_TEXT_DRAW;
    }
    if(PlayerNotification[playerid][nt_Body] != INVALID_PLAYER_TEXT_DRAW) {
        PlayerTextDrawDestroy(playerid, PlayerNotification[playerid][nt_Body]);
        PlayerNotification[playerid][nt_Body] = INVALID_PLAYER_TEXT_DRAW;
    }
    if(PlayerNotification[playerid][nt_Progress] != INVALID_PLAYER_TEXT_DRAW) {
        PlayerTextDrawDestroy(playerid, PlayerNotification[playerid][nt_Progress]);
        PlayerNotification[playerid][nt_Progress] = INVALID_PLAYER_TEXT_DRAW;
    }

    KillTimer(PlayerNotification[playerid][nt_Timer]);
    PlayerNotification[playerid][nt_IsShowing] = false;
    return 1;
}