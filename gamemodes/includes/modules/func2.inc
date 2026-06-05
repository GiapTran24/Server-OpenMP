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
forward HideNotificationSlot(playerid, slotid);
public HideNotificationSlot(playerid, slotid)
{
    if(!PlayerNotifications[playerid][slotid][slot_Active]) return 0;

    // Hủy các Textdraw của slot này
    if(PlayerNotifications[playerid][slotid][slot_BG] != INVALID_PLAYER_TEXT_DRAW) {
        PlayerTextDrawDestroy(playerid, PlayerNotifications[playerid][slotid][slot_BG]);
        PlayerNotifications[playerid][slotid][slot_BG] = INVALID_PLAYER_TEXT_DRAW;
    }
    if(PlayerNotifications[playerid][slotid][slot_TitleTD] != INVALID_PLAYER_TEXT_DRAW) {
        PlayerTextDrawDestroy(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD]);
        PlayerNotifications[playerid][slotid][slot_TitleTD] = INVALID_PLAYER_TEXT_DRAW;
    }
    if(PlayerNotifications[playerid][slotid][slot_BodyTD] != INVALID_PLAYER_TEXT_DRAW) {
        PlayerTextDrawDestroy(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD]);
        PlayerNotifications[playerid][slotid][slot_BodyTD] = INVALID_PLAYER_TEXT_DRAW;
    }
    if(PlayerNotifications[playerid][slotid][slot_Progress] != INVALID_PLAYER_TEXT_DRAW) {
        PlayerTextDrawDestroy(playerid, PlayerNotifications[playerid][slotid][slot_Progress]);
        PlayerNotifications[playerid][slotid][slot_Progress] = INVALID_PLAYER_TEXT_DRAW;
    }

    KillTimer(PlayerNotifications[playerid][slotid][slot_Timer]);
    PlayerNotifications[playerid][slotid][slot_Active] = false;
    return 1;
}

// Hàm ước tính chiều cao Textdraw dựa theo độ dài của Message
stock Float:CalculateNotifyHeight(const message[])
{
    new len = strlen(message);
    new lines = 1;

    // Ước lượng: Với TextSize chiều rộng là 155.0, một dòng chứa được khoảng 32 ký tự
    if(len > 32) {
        lines = (len / 32) + 1;
    }
    
    // Giới hạn tối đa 4 dòng để tránh thông báo quá dài làm vỡ giao diện
    if(lines > 4) lines = 4; 

    // Chiều cao cơ bản (cho 1 dòng) là 45.0. Mỗi dòng tăng thêm cộng thêm 10.0 đơn vị Y
    new Float:dynamicHeight = 55.0 + (Float:lines - 1) * 10.0;
    return dynamicHeight;
}

stock CreateSlotNotifyTD(playerid, slotid)
{
    new Float:posX = 460.0;
    new Float:baseY = 140.0; // Tọa độ Y bắt đầu của thông báo trên cùng
    new Float:posY;
    
    // Tính toán chiều cao động của thông báo hiện tại
    new Float:height = PlayerNotifications[playerid][slotid][slot_Height];
    new Float:width = 160.0;

    if(slotid == 0)
    {
        posY = baseY;
    }
    else // Slot 1 (Dưới)
    {
        // Tọa độ Y của Slot dưới = Tọa độ Y Slot trên + Chiều cao Slot trên + Khoảng cách cách ra (10.0)
        posY = baseY + PlayerNotifications[playerid][0][slot_Height] + 10.0;
    }

    new titleColor, barColor;
    switch(PlayerNotifications[playerid][slotid][slot_Type])
    {
        case NOTIFY_TYPE_SUCCESS: { titleColor = 0x2ECC71FF; barColor = 0x27AE60FF; }
        case NOTIFY_TYPE_ERROR:   { titleColor = 0xE74C3CFF; barColor = 0xC0392BFF; }
        case NOTIFY_TYPE_WARNING: { titleColor = 0xF1C40FFF; barColor = 0xD35400FF; }
        default:                  { titleColor = 0x3498DBFF; barColor = 0x2980B9FF; }
    }

    // 1. Background (Chiều cao hộp đen co giãn theo biến 'height')
    PlayerNotifications[playerid][slotid][slot_BG] = CreatePlayerTextDraw(playerid, posX, posY, "_");
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_BG], 0.0, height / 10.2); // Chia 10.2 để đổi từ Y sang tỉ lệ Box
    PlayerTextDrawTextSize(playerid, PlayerNotifications[playerid][slotid][slot_BG], posX + width, 0.0);
    PlayerTextDrawUseBox(playerid, PlayerNotifications[playerid][slotid][slot_BG], bool:1);
    PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_BG], 0x000000AA);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BG]);

    // 2. Title
    PlayerNotifications[playerid][slotid][slot_TitleTD] = CreatePlayerTextDraw(playerid, posX + 5.0, posY + 4.0, PlayerNotifications[playerid][slotid][slot_Title]);
    PlayerTextDrawFont(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], 0.28, 1.1);
    PlayerTextDrawColour(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], titleColor);
    PlayerTextDrawSetProportional(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], bool:1);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD]);

    // 3. Message Body
    PlayerNotifications[playerid][slotid][slot_BodyTD] = CreatePlayerTextDraw(playerid, posX + 5.0, posY + 17.0, PlayerNotifications[playerid][slotid][slot_Message]);
    PlayerTextDrawFont(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], 0.23, 0.9);
    PlayerTextDrawColour(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], 0xFFFFFFFF);
    PlayerTextDrawTextSize(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], (posX + width) - 5.0, 0.0); // Giới hạn chiều rộng ép xuống dòng
    PlayerTextDrawSetProportional(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], bool:1);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD]);

    // 4. Progress Bar (Tự động bám sát vào đáy của Background bằng công thức: posY + height - 3.0)
    PlayerNotifications[playerid][slotid][slot_Progress] = CreatePlayerTextDraw(playerid, posX, (posY + height) - 2.5, "_");
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_Progress], 0.0, 0.25);
    PlayerTextDrawTextSize(playerid, PlayerNotifications[playerid][slotid][slot_Progress], posX + width, 0.0);
    PlayerTextDrawUseBox(playerid, PlayerNotifications[playerid][slotid][slot_Progress], bool:1);
    PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_Progress], barColor);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_Progress]);

    PlayerNotifications[playerid][slotid][slot_Timer] = SetTimerEx("HideNotificationSlot", PlayerNotifications[playerid][slotid][slot_Duration], false, "dd", playerid, slotid);
    return 1;
}

stock ShowNotification(playerid, const title[], const message[], e_NOTIFY_TYPE:type, duration = 5000)
{
    // Nếu Slot 1 (Dưới) đang có thông báo -> Xóa nó trước
    if(PlayerNotifications[playerid][1][slot_Active])
    {
        HideNotificationSlot(playerid, 1);
    }

    // Nếu Slot 0 (Trên) đang có thông báo -> Đẩy xuống Slot 1
    if(PlayerNotifications[playerid][0][slot_Active])
    {
        format(PlayerNotifications[playerid][1][slot_Title], 32, PlayerNotifications[playerid][0][slot_Title]);
        format(PlayerNotifications[playerid][1][slot_Message], 128, PlayerNotifications[playerid][0][slot_Message]);
        PlayerNotifications[playerid][1][slot_Type] = PlayerNotifications[playerid][0][slot_Type];
        PlayerNotifications[playerid][1][slot_Duration] = duration; 
        PlayerNotifications[playerid][1][slot_Height] = PlayerNotifications[playerid][0][slot_Height]; // Chuyển giao thông tin chiều cao cũ xuống slot dưới
        PlayerNotifications[playerid][1][slot_Active] = true;

        HideNotificationSlot(playerid, 0);
        CreateSlotNotifyTD(playerid, 1); // Vẽ lại thông báo cũ ở slot 1 với chiều cao chuẩn của nó
    }

    // Thiết lập thông báo MỚI cho Slot 0
    format(PlayerNotifications[playerid][0][slot_Title], 32, title);
    format(PlayerNotifications[playerid][0][slot_Message], 128, message);
    PlayerNotifications[playerid][0][slot_Type] = type;
    PlayerNotifications[playerid][0][slot_Duration] = duration;
    
    // TỰ ĐỘNG TÍNH CHIỀU CAO CHO THÔNG BÁO MỚI TẠI ĐÂY
    PlayerNotifications[playerid][0][slot_Height] = CalculateNotifyHeight(message);
    
    PlayerNotifications[playerid][0][slot_Active] = true;

    CreateSlotNotifyTD(playerid, 0);
    return 1;
}