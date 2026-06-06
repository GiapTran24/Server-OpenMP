// Thong Bao
enum e_NOTIFY_TYPE 
{
    NOTIFY_TYPE_INFO,
    NOTIFY_TYPE_SUCCESS,
    NOTIFY_TYPE_WARNING,
    NOTIFY_TYPE_ERROR
};

enum e_NOTIFY_SLOT_DATA 
{
    bool:slot_Active,
    slot_Title[32],
    slot_Message[128],
    e_NOTIFY_TYPE:slot_Type,
    slot_Duration,
    Float:slot_Height,
    Float:slot_CurrentHeight,
    slot_Alpha,
    bool:slot_IsFadingOut,
    PlayerText:slot_BG,
    PlayerText:slot_TitleTD,
    PlayerText:slot_BodyTD,
    PlayerText:slot_Progress,
    slot_Timer,
    slot_AnimTimer
};

new PlayerNotifications[MAX_PLAYERS][2][e_NOTIFY_SLOT_DATA]; // Chỉ cần 2 slot (0 là Trên, 1 là Dưới)

// Hàm này chuyên dùng để XÓA LẬP TỨC (Dùng khi bị đẩy slot hoặc ngắt kết nối)
stock ClearNotificationSlot(playerid, slotid)
{
    KillTimer(PlayerNotifications[playerid][slotid][slot_Timer]);
    KillTimer(PlayerNotifications[playerid][slotid][slot_AnimTimer]);

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

    // RESET TOÀN BỘ BIẾN TRẠNG THÁI VỀ MẶC ĐỊNH
    PlayerNotifications[playerid][slotid][slot_Active] = false;
    PlayerNotifications[playerid][slotid][slot_IsFadingOut] = false;
    PlayerNotifications[playerid][slotid][slot_Alpha] = 0;
    return 1;
}

// Hàm này CHỈ XỬ LÝ khi thông báo tự hết thời gian sống -> Kích hoạt Fade Out
forward HideNotificationSlot(playerid, slotid);
public HideNotificationSlot(playerid, slotid)
{
    if(!PlayerNotifications[playerid][slotid][slot_Active]) return 0;

    if(!PlayerNotifications[playerid][slotid][slot_IsFadingOut])
    {
        PlayerNotifications[playerid][slotid][slot_IsFadingOut] = true;
        KillTimer(PlayerNotifications[playerid][slotid][slot_Timer]);
        KillTimer(PlayerNotifications[playerid][slotid][slot_AnimTimer]);

        // Chạy Timer lặp lại mỗi 15ms để thực hiện giảm độ mờ (Fade Out)
        PlayerNotifications[playerid][slotid][slot_AnimTimer] = SetTimerEx("OnAnimateNotifyFadeOut", 15, true, "dd", playerid, slotid);
    }
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

stock CreateSlotNotifyTDDirect(playerid, slotid)
{
    new Float:posX = 460.0;
    new Float:baseY = 140.0;
    // Vì hàm này chỉ dùng cho Slot 1, posY sẽ tính dựa trên chiều cao Slot 0 + khoảng cách 10.0
    new Float:posY = baseY + PlayerNotifications[playerid][0][slot_Height] + 10.0;
    new Float:width = 160.0;
    new Float:height = PlayerNotifications[playerid][slotid][slot_Height];

    new titleColor, barColor;
    switch(PlayerNotifications[playerid][slotid][slot_Type])
    {
        case NOTIFY_TYPE_SUCCESS: { titleColor = 0x2ECC71FF; barColor = 0x27AE60FF; }
        case NOTIFY_TYPE_ERROR:   { titleColor = 0xE74C3CFF; barColor = 0xC0392BFF; }
        case NOTIFY_TYPE_WARNING: { titleColor = 0xF1C40FFF; barColor = 0xD35400FF; }
        default:                  { titleColor = 0x3498DBFF; barColor = 0x2980B9FF; }
    }

    // Thiết lập trạng thái hiển thị đầy đủ ngay lập tức
    PlayerNotifications[playerid][slotid][slot_Alpha] = 170; 
    PlayerNotifications[playerid][slotid][slot_IsFadingOut] = false;

    // 1. Background (Hiện thẳng với chiều cao chuẩn)
    PlayerNotifications[playerid][slotid][slot_BG] = CreatePlayerTextDraw(playerid, posX, posY, "_");
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_BG], 0.0, height / 10.2);
    PlayerTextDrawTextSize(playerid, PlayerNotifications[playerid][slotid][slot_BG], posX + width, 0.0);
    PlayerTextDrawUseBox(playerid, PlayerNotifications[playerid][slotid][slot_BG], bool:1);
    PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_BG], 0x000000AA); // Hiện rõ Alpha AA liền
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BG]);

    // 2. Title
    PlayerNotifications[playerid][slotid][slot_TitleTD] = CreatePlayerTextDraw(playerid, posX + 5.0, posY + 4.0, PlayerNotifications[playerid][slotid][slot_Title]);
    PlayerTextDrawFont(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], 0.28, 1.1);
    PlayerTextDrawSetShadow(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], bool:0);
    PlayerTextDrawColour(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], titleColor);
    PlayerTextDrawSetProportional(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], bool:1);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD]);

    // 3. Message Body
    PlayerNotifications[playerid][slotid][slot_BodyTD] = CreatePlayerTextDraw(playerid, posX + 5.0, posY + 17.0, PlayerNotifications[playerid][slotid][slot_Message]);
    PlayerTextDrawFont(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], 0.23, 0.9);
    PlayerTextDrawColour(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], 0xFFFFFFFF);
    PlayerTextDrawSetShadow(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], bool:0);
    PlayerTextDrawTextSize(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], (posX + width) - 5.0, 0.0);
    PlayerTextDrawSetProportional(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], bool:1);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD]);

    // 4. Progress Bar
    PlayerNotifications[playerid][slotid][slot_Progress] = CreatePlayerTextDraw(playerid, posX, (posY + height) - 2.5, "_");
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_Progress], 0.0, 0.25);
    PlayerTextDrawTextSize(playerid, PlayerNotifications[playerid][slotid][slot_Progress], posX + width, 0.0);
    PlayerTextDrawUseBox(playerid, PlayerNotifications[playerid][slotid][slot_Progress], bool:1);
    PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_Progress], barColor);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_Progress]);

    // Bắt đầu đếm ngược thời gian kết thúc (để chuẩn bị chạy hiệu ứng Fade Out khi hết hạn)
    PlayerNotifications[playerid][slotid][slot_Timer] = SetTimerEx("HideNotificationSlot", PlayerNotifications[playerid][slotid][slot_Duration], false, "dd", playerid, slotid);
    return 1;
}

stock CreateSlotNotifyTD(playerid, slotid)
{
    new Float:posX = 460.0;
    new Float:baseY = 140.0;
    new Float:posY = (slotid == 0) ? baseY : (baseY + PlayerNotifications[playerid][0][slot_Height] + 10.0);
    new Float:width = 160.0;

    // Reset chiều cao bắt đầu của hiệu ứng về 0
    PlayerNotifications[playerid][slotid][slot_CurrentHeight] = 0.0;
    PlayerNotifications[playerid][slotid][slot_Alpha] = 0; // Bắt đầu từ độ trong suốt bằng 0
    PlayerNotifications[playerid][slotid][slot_IsFadingOut] = false; // Reset trạng thái Fade Out

    // Tạo Background rỗng ban đầu với Alpha = 0
    PlayerNotifications[playerid][slotid][slot_BG] = CreatePlayerTextDraw(playerid, posX, posY, "_");
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_BG], 0.0, 0.0); 
    PlayerTextDrawTextSize(playerid, PlayerNotifications[playerid][slotid][slot_BG], posX + width, 0.0);
    PlayerTextDrawUseBox(playerid, PlayerNotifications[playerid][slotid][slot_BG], bool:1);
    PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_BG], 0x00000000); // Khởi đầu trong suốt hoàn toàn
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BG]);

    // Gọi Timer chạy hiệu ứng thu phóng lặp lại sau mỗi 15 mili-giây (Mượt như 60 FPS)
    PlayerNotifications[playerid][slotid][slot_AnimTimer] = SetTimerEx("OnAnimateNotifyZoom", 15, true, "dd", playerid, slotid);
    return 1;
}

// Hàm chính để hiển thị thông báo với hiệu ứng thu phóng và xếp chồng
stock SendClientNotification(playerid, const title[], const message[], e_NOTIFY_TYPE:type, duration = 5000)
{
    // 1. Nếu Slot 1 (Dưới) đang hiện -> Xóa sạch lập tức để nhường chỗ
    if(PlayerNotifications[playerid][1][slot_Active])
    {
        ClearNotificationSlot(playerid, 1);
    }

    // 2. Nếu Slot 0 (Trên) đang hiện -> Đẩy dữ liệu xuống Slot 1
    if(PlayerNotifications[playerid][0][slot_Active])
    {
        format(PlayerNotifications[playerid][1][slot_Title], 32, PlayerNotifications[playerid][0][slot_Title]);
        format(PlayerNotifications[playerid][1][slot_Message], 128, PlayerNotifications[playerid][0][slot_Message]);
        PlayerNotifications[playerid][1][slot_Type] = PlayerNotifications[playerid][0][slot_Type];
        PlayerNotifications[playerid][1][slot_Duration] = duration; 
        PlayerNotifications[playerid][1][slot_Height] = PlayerNotifications[playerid][0][slot_Height]; 
        PlayerNotifications[playerid][1][slot_Active] = true;

        // XÓA SẠCH SLOT 0 TRƯỚC (Để hủy bỏ hoàn toàn cái Timer cũ đang chạy của Slot 0)
        ClearNotificationSlot(playerid, 0);

        // Tạo hiển thị mới cho Slot 1
        CreateSlotNotifyTDDirect(playerid, 1);
    }

    // 3. Thiết lập thông báo MỚI TOANH cho Slot 0
    format(PlayerNotifications[playerid][0][slot_Title], 32, title);
    format(PlayerNotifications[playerid][0][slot_Message], 128, message);
    PlayerNotifications[playerid][0][slot_Type] = type;
    PlayerNotifications[playerid][0][slot_Duration] = duration;
    PlayerNotifications[playerid][0][slot_Height] = CalculateNotifyHeight(message);
    PlayerNotifications[playerid][0][slot_Active] = true;

    // Phát âm thanh
    new soundId;
    switch(type)
    {
        case NOTIFY_TYPE_SUCCESS: soundId = 1057;
        case NOTIFY_TYPE_ERROR:   soundId = 1085;
        case NOTIFY_TYPE_WARNING: soundId = 1052;
        default:                  soundId = 1058;
    }
    PlayerPlaySound(playerid, soundId, 0.0, 0.0, 0.0);

    // Kích hoạt hiển thị cho Slot 0
    CreateSlotNotifyTD(playerid, 0);
    return 1;
}

stock ShowSlotTextAndBar(playerid, slotid, Float:posY, Float:height)
{
    new Float:posX = 460.0;
    new Float:width = 160.0;

    new titleColor, barColor;
    switch(PlayerNotifications[playerid][slotid][slot_Type])
    {
        case NOTIFY_TYPE_SUCCESS: { titleColor = 0x2ECC71FF; barColor = 0x27AE60FF; }
        case NOTIFY_TYPE_ERROR:   { titleColor = 0xE74C3CFF; barColor = 0xC0392BFF; }
        case NOTIFY_TYPE_WARNING: { titleColor = 0xF1C40FFF; barColor = 0xD35400FF; }
        default:                  { titleColor = 0x3498DBFF; barColor = 0x2980B9FF; }
    }

    // Tạo Title
    PlayerNotifications[playerid][slotid][slot_TitleTD] = CreatePlayerTextDraw(playerid, posX + 5.0, posY + 4.0, PlayerNotifications[playerid][slotid][slot_Title]);
    PlayerTextDrawFont(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], 0.28, 1.1);
    PlayerTextDrawColour(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], titleColor);
    PlayerTextDrawSetShadow(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], bool:0);
    PlayerTextDrawSetProportional(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], bool:1);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD]);

    // Tạo Message Body
    PlayerNotifications[playerid][slotid][slot_BodyTD] = CreatePlayerTextDraw(playerid, posX + 5.0, posY + 17.0, PlayerNotifications[playerid][slotid][slot_Message]);
    PlayerTextDrawFont(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], 0.23, 0.9);
    PlayerTextDrawColour(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], 0xFFFFFFFF);
    PlayerTextDrawSetShadow(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], bool:0);
    PlayerTextDrawTextSize(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], (posX + width) - 5.0, 0.0);
    PlayerTextDrawSetProportional(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], bool:1);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD]);

    // Tạo Progress Bar ở đáy
    PlayerNotifications[playerid][slotid][slot_Progress] = CreatePlayerTextDraw(playerid, posX, (posY + height) - 2.5, "_");
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_Progress], 0.0, 0.25);
    PlayerTextDrawTextSize(playerid, PlayerNotifications[playerid][slotid][slot_Progress], posX + width, 0.0);
    PlayerTextDrawUseBox(playerid, PlayerNotifications[playerid][slotid][slot_Progress], bool:1);
    PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_Progress], barColor);
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_Progress]);

    // Bắt đầu đếm ngược thời gian sống của thông báo
    PlayerNotifications[playerid][slotid][slot_Timer] = SetTimerEx("HideNotificationSlot", PlayerNotifications[playerid][slotid][slot_Duration], false, "dd", playerid, slotid);
}

forward OnAnimateNotifyZoom(playerid, slotid);
public OnAnimateNotifyZoom(playerid, slotid)
{
    if(!PlayerNotifications[playerid][slotid][slot_Active]) return 0;

    new Float:baseY = 140.0;
    new Float:posY = (slotid == 0) ? baseY : (baseY + PlayerNotifications[playerid][0][slot_Height] + 10.0);

    // 1. Tăng chiều cao và Alpha (Tốc độ tăng Alpha là 15 đơn vị mỗi 15ms)
    PlayerNotifications[playerid][slotid][slot_CurrentHeight] += 4.0;
    PlayerNotifications[playerid][slotid][slot_Alpha] += 15;
    if(PlayerNotifications[playerid][slotid][slot_Alpha] > 170) PlayerNotifications[playerid][slotid][slot_Alpha] = 170; // Giới hạn Alpha của nền là ~66%

    // Nếu đã đạt kích thước đích
    if(PlayerNotifications[playerid][slotid][slot_CurrentHeight] >= PlayerNotifications[playerid][slotid][slot_Height])
    {
        PlayerNotifications[playerid][slotid][slot_CurrentHeight] = PlayerNotifications[playerid][slotid][slot_Height];
        PlayerNotifications[playerid][slotid][slot_Alpha] = 170; // Chốt Alpha nền
        KillTimer(PlayerNotifications[playerid][slotid][slot_AnimTimer]); 

        // Hiển thị nền lần cuối với Alpha chuẩn
        PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_BG], 0.0, PlayerNotifications[playerid][slotid][slot_CurrentHeight] / 10.2);
        PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_BG], GetColorWithAlpha(0x000000FF, PlayerNotifications[playerid][slotid][slot_Alpha]));
        PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BG]);

        // Kích hoạt hiển thị Chữ và Thanh tiến trình (Hàm này giữ nguyên như bài trước)
        ShowSlotTextAndBar(playerid, slotid, posY, PlayerNotifications[playerid][slotid][slot_Height]);
        return 1;
    }

    // Cập nhật hiệu ứng thu phóng + Mờ dần (Fade In) cho Background
    PlayerTextDrawLetterSize(playerid, PlayerNotifications[playerid][slotid][slot_BG], 0.0, PlayerNotifications[playerid][slotid][slot_CurrentHeight] / 10.2);
    PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_BG], GetColorWithAlpha(0x000000FF, PlayerNotifications[playerid][slotid][slot_Alpha]));
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BG]);
    return 1;
}

forward OnAnimateNotifyFadeOut(playerid, slotid);
public OnAnimateNotifyFadeOut(playerid, slotid)
{
    if(!PlayerNotifications[playerid][slotid][slot_Active]) return 0;

    PlayerNotifications[playerid][slotid][slot_Alpha] -= 15;

    new titleColor, barColor;
    switch(PlayerNotifications[playerid][slotid][slot_Type])
    {
        case NOTIFY_TYPE_SUCCESS: { titleColor = 0x2ECC71FF; barColor = 0x27AE60FF; }
        case NOTIFY_TYPE_ERROR:   { titleColor = 0xE74C3CFF; barColor = 0xC0392BFF; }
        case NOTIFY_TYPE_WARNING: { titleColor = 0xF1C40FFF; barColor = 0xD35400FF; }
        default:                  { titleColor = 0x3498DBFF; barColor = 0x2980B9FF; }
    }

    // Nếu đã mờ hẳn -> Gọi hàm Clear sạch sẽ bộ nhớ và biến trạng thái
    if(PlayerNotifications[playerid][slotid][slot_Alpha] <= 0)
    {
        ClearNotificationSlot(playerid, slotid);
        return 1;
    }

    // Đoạn code cập nhật độ mờ Alpha (Giữ nguyên)
    new currentAlpha = PlayerNotifications[playerid][slotid][slot_Alpha];
    new textAlpha = (currentAlpha * 255) / 170; 

    PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_BG], GetColorWithAlpha(0x000000FF, currentAlpha));
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BG]);

    PlayerTextDrawColour(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD], GetColorWithAlpha(titleColor, textAlpha));
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_TitleTD]);

    PlayerTextDrawColour(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD], GetColorWithAlpha(0xFFFFFFFF, textAlpha));
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_BodyTD]);

    PlayerTextDrawBoxColour(playerid, PlayerNotifications[playerid][slotid][slot_Progress], GetColorWithAlpha(barColor, textAlpha));
    PlayerTextDrawShow(playerid, PlayerNotifications[playerid][slotid][slot_Progress]);

    return 1;
}

stock GetColorWithAlpha(base_color, alpha_val)
{
    // Loại bỏ 8 bit Alpha cũ (RGBA -> RGB0) và chèn Alpha mới vào cuối
    return (base_color & 0xFFFFFF00) | (alpha_val & 0xFF);
}