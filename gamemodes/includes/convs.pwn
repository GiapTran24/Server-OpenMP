/*
    Hệ thống Messenger UI (List View & Chat Cache) chuẩn open.mp
    Thiết kế & Tối ưu bởi: Gemini AI x Bạn
*/

#include <open.mp>
#include <YSI_Coding\y_hooks>
// =============================================================================
// CONFIGURATION & DEFINES
// =============================================================================
#define MAX_VISIBLE_CONTACTS    (5)     // Số người hiển thị cùng lúc ngoài danh sách
#define MAX_VISIBLE_MESSAGES    (5)     // Số tin nhắn hiển thị cùng lúc trong hộp chat
#define CONTACT_ROW_SPACING     (29.0)  // Khoảng cách Y giữa các dòng liên hệ
#define CHAT_ROW_SPACING        (28.0)  // Khoảng cách Y giữa các dòng tin nhắn
#define MAX_MSG_LENGTH          (256)   // Độ dài tối đa của một tin nhắn

// ID Dialogs
#define DIALOG_MESSENGER_INPUT  (9542)
#define DIALOG_VIEW_FULL_MSG    (9543)

// Trạng thái giao diện
enum {
    MSG_STATE_CLOSED = 0,
    MSG_STATE_CONTACTS,       // Đang ở màn hình danh sách liên hệ
    MSG_STATE_CHAT_VIEW       // Đang ở màn hình xem nội dung chat
}

// =============================================================================
// TEXTDRAW AND CACHE ARRAYS
// =============================================================================
// 1. TextDraw cố định (Không lặp)
enum _:E_BASE_TD {
    PlayerText:TD_MAIN_BG,
    PlayerText:TD_INNER_BG,
    PlayerText:TD_TITLE,
    PlayerText:TD_CLOSE_BTN,
    PlayerText:TD_SCROLL_UP,
    PlayerText:TD_SCROLL_DOWN,
    PlayerText:TD_INPUT_BG,
    PlayerText:TD_INPUT_TXT
};
new PlayerText:BaseTD[MAX_PLAYERS][E_BASE_TD];

// 2. TextDraw dòng Liên hệ (Sẽ lặp)
enum _:E_CONTACT_ROW {
    PlayerText:TD_ROW_BG,
    PlayerText:TD_ROW_NAME,
    PlayerText:TD_ROW_LAST_MSG,
    PlayerText:TD_ROW_TIME,
    PlayerText:TD_ROW_STATUS
};
new PlayerText:ContactRowTD[MAX_PLAYERS][MAX_VISIBLE_CONTACTS][E_CONTACT_ROW];

// 3. TextDraw dòng Chat (Sẽ lặp)
enum _:E_CHAT_ROW {
    PlayerText:TD_CHAT_SENDER,
    PlayerText:TD_CHAT_BODY,
    PlayerText:TD_CHAT_TIME
};
new PlayerText:ChatRowTD[MAX_PLAYERS][MAX_VISIBLE_MESSAGES][E_CHAT_ROW];

// 4. Hệ thống dữ liệu & Bộ nhớ đệm (Cache)
enum E_PLAYER_MSG_DATA {
    p_MsgState,               // Trạng thái UI hiện tại
    p_TargetAccountID,        // ID tài khoản đang chat cùng
    p_ScrollIndex,            // Vị trí cuộn hiện tại
    p_TotalItems              // Tổng số mục dữ liệu từ Backend
}
new PlayerMsgData[MAX_PLAYERS][E_PLAYER_MSG_DATA];

// Bộ nhớ đệm chứa nội dung gốc chưa cắt để hiển thị Dialog FULL tin nhắn
new FullChatCache[MAX_PLAYERS][MAX_VISIBLE_MESSAGES][MAX_MSG_LENGTH];

// Tiền khai báo hàm (Forward Declarations)
forward Backend_LoadContactList(playerid, start_index);
forward Backend_LoadChatMessage(playerid, target_account_id, start_offset);
forward Backend_SaveAndSendMessage(playerid, target_account_id, const message[]);
forward TruncateString(const source[], dest[], max_len);

// =============================================================================
// SYSTEM CALLBACKS
// =============================================================================
hook OnPlayerConnect(playerid)
{
    // Reset Dữ Liệu
    PlayerMsgData[playerid][p_MsgState] = MSG_STATE_CLOSED;
    PlayerMsgData[playerid][p_TargetAccountID] = 0;
    PlayerMsgData[playerid][p_ScrollIndex] = 0;
    PlayerMsgData[playerid][p_TotalItems] = 0;

    // A. KHỞI TẠO CÁC TEXTDRAW CỐ ĐỊNH (Theo tọa độ chuẩn của bạn)
    BaseTD[playerid][TD_MAIN_BG] = CreatePlayerTextDraw(playerid, 555.0, 252.0, "_");
    PlayerTextDrawFont(playerid, BaseTD[playerid][TD_MAIN_BG], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, BaseTD[playerid][TD_MAIN_BG], 0.0, 19.5);
    PlayerTextDrawTextSize(playerid, BaseTD[playerid][TD_MAIN_BG], 298.5, 130.0);
    PlayerTextDrawSetOutline(playerid, BaseTD[playerid][TD_MAIN_BG], 1);
    PlayerTextDrawSetShadow(playerid, BaseTD[playerid][TD_MAIN_BG], 0);
    PlayerTextDrawAlignment(playerid, BaseTD[playerid][TD_MAIN_BG], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BaseTD[playerid][TD_MAIN_BG], -1);
    PlayerTextDrawBackgroundColour(playerid, BaseTD[playerid][TD_MAIN_BG], 255);
    PlayerTextDrawBoxColour(playerid, BaseTD[playerid][TD_MAIN_BG], 135);
    PlayerTextDrawUseBox(playerid, BaseTD[playerid][TD_MAIN_BG], bool:1);
    PlayerTextDrawSetProportional(playerid, BaseTD[playerid][TD_MAIN_BG], bool:1);

    BaseTD[playerid][TD_INNER_BG] = CreatePlayerTextDraw(playerid, 555.0, 268.0, "_");
    PlayerTextDrawFont(playerid, BaseTD[playerid][TD_INNER_BG], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, BaseTD[playerid][TD_INNER_BG], 0.6, 16.0);
    PlayerTextDrawTextSize(playerid, BaseTD[playerid][TD_INNER_BG], 298.5, 125.0);
    PlayerTextDrawSetOutline(playerid, BaseTD[playerid][TD_INNER_BG], 1);
    PlayerTextDrawSetShadow(playerid, BaseTD[playerid][TD_INNER_BG], 0);
    PlayerTextDrawAlignment(playerid, BaseTD[playerid][TD_INNER_BG], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BaseTD[playerid][TD_INNER_BG], -1);
    PlayerTextDrawBackgroundColour(playerid, BaseTD[playerid][TD_INNER_BG], 255);
    PlayerTextDrawBoxColour(playerid, BaseTD[playerid][TD_INNER_BG], -2016478465);
    PlayerTextDrawUseBox(playerid, BaseTD[playerid][TD_INNER_BG], bool:1);
    PlayerTextDrawSetProportional(playerid, BaseTD[playerid][TD_INNER_BG], bool:1);

    BaseTD[playerid][TD_TITLE] = CreatePlayerTextDraw(playerid, 555.0, 254.0, "Danh sach lien he"); 
    PlayerTextDrawFont(playerid, BaseTD[playerid][TD_TITLE], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, BaseTD[playerid][TD_TITLE], 0.2, 1.0);
    PlayerTextDrawTextSize(playerid, BaseTD[playerid][TD_TITLE], 400.0, 125.0);
    PlayerTextDrawSetShadow(playerid, BaseTD[playerid][TD_TITLE], 0);
    PlayerTextDrawAlignment(playerid, BaseTD[playerid][TD_TITLE], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BaseTD[playerid][TD_TITLE], -1);
    PlayerTextDrawBackgroundColour(playerid, BaseTD[playerid][TD_TITLE], 255);
    PlayerTextDrawBoxColour(playerid, BaseTD[playerid][TD_TITLE], 1687547276);
    PlayerTextDrawUseBox(playerid, BaseTD[playerid][TD_TITLE], bool:1);
    PlayerTextDrawSetProportional(playerid, BaseTD[playerid][TD_TITLE], bool:1);

    BaseTD[playerid][TD_SCROLL_UP] = CreatePlayerTextDraw(playerid, 605.0, 253.0, "ld_beat:up"); 
    PlayerTextDrawFont(playerid, BaseTD[playerid][TD_SCROLL_UP], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, BaseTD[playerid][TD_SCROLL_UP], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, BaseTD[playerid][TD_SCROLL_UP], 15.0, 12.5);
    PlayerTextDrawAlignment(playerid, BaseTD[playerid][TD_SCROLL_UP], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, BaseTD[playerid][TD_SCROLL_UP], 1687547391);
    PlayerTextDrawBackgroundColour(playerid, BaseTD[playerid][TD_SCROLL_UP], -1378294017);
    PlayerTextDrawBoxColour(playerid, BaseTD[playerid][TD_SCROLL_UP], -1378294222);
    PlayerTextDrawUseBox(playerid, BaseTD[playerid][TD_SCROLL_UP], bool:1);
    PlayerTextDrawSetProportional(playerid, BaseTD[playerid][TD_SCROLL_UP], bool:1);
    PlayerTextDrawSetSelectable(playerid, BaseTD[playerid][TD_SCROLL_UP], bool:1);

    BaseTD[playerid][TD_CLOSE_BTN] = CreatePlayerTextDraw(playerid, 491.0, 252.0, "ld_beat:cross"); 
    PlayerTextDrawFont(playerid, BaseTD[playerid][TD_CLOSE_BTN], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, BaseTD[playerid][TD_CLOSE_BTN], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, BaseTD[playerid][TD_CLOSE_BTN], 12.0, 13.0);
    PlayerTextDrawAlignment(playerid, BaseTD[playerid][TD_CLOSE_BTN], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BaseTD[playerid][TD_CLOSE_BTN], -16776961);
    PlayerTextDrawBackgroundColour(playerid, BaseTD[playerid][TD_CLOSE_BTN], 255);
    PlayerTextDrawBoxColour(playerid, BaseTD[playerid][TD_CLOSE_BTN], 50);
    PlayerTextDrawUseBox(playerid, BaseTD[playerid][TD_CLOSE_BTN], bool:1);
    PlayerTextDrawSetProportional(playerid, BaseTD[playerid][TD_CLOSE_BTN], bool:1);
    PlayerTextDrawSetSelectable(playerid, BaseTD[playerid][TD_CLOSE_BTN], bool:1);

    BaseTD[playerid][TD_INPUT_BG] = CreatePlayerTextDraw(playerid, 555.0, 417.0, "_");
    PlayerTextDrawFont(playerid, BaseTD[playerid][TD_INPUT_BG], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, BaseTD[playerid][TD_INPUT_BG], 0.6, 0.9);
    PlayerTextDrawTextSize(playerid, BaseTD[playerid][TD_INPUT_BG], 298.5, 125.0);
    PlayerTextDrawAlignment(playerid, BaseTD[playerid][TD_INPUT_BG], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BaseTD[playerid][TD_INPUT_BG], -1);
    PlayerTextDrawBackgroundColour(playerid, BaseTD[playerid][TD_INPUT_BG], -1962934017);
    PlayerTextDrawBoxColour(playerid, BaseTD[playerid][TD_INPUT_BG], 1687547276);
    PlayerTextDrawUseBox(playerid, BaseTD[playerid][TD_INPUT_BG], bool:1);
    PlayerTextDrawSetProportional(playerid, BaseTD[playerid][TD_INPUT_BG], bool:1);

    BaseTD[playerid][TD_INPUT_TXT] = CreatePlayerTextDraw(playerid, 521.0, 417.0, "Soan Tin Nhan"); 
    PlayerTextDrawFont(playerid, BaseTD[playerid][TD_INPUT_TXT], TEXT_DRAW_FONT:1);
    PlayerTextDrawLetterSize(playerid, BaseTD[playerid][TD_INPUT_TXT], 0.1, 0.9);
    PlayerTextDrawTextSize(playerid, BaseTD[playerid][TD_INPUT_TXT], 9.0, 57.0);
    PlayerTextDrawAlignment(playerid, BaseTD[playerid][TD_INPUT_TXT], TEXT_DRAW_ALIGN:2);
    PlayerTextDrawColour(playerid, BaseTD[playerid][TD_INPUT_TXT], -1);
    PlayerTextDrawBackgroundColour(playerid, BaseTD[playerid][TD_INPUT_TXT], 255);
    PlayerTextDrawBoxColour(playerid, BaseTD[playerid][TD_INPUT_TXT], 200);
    PlayerTextDrawUseBox(playerid, BaseTD[playerid][TD_INPUT_TXT], bool:1);
    PlayerTextDrawSetProportional(playerid, BaseTD[playerid][TD_INPUT_TXT], bool:1);
    PlayerTextDrawSetSelectable(playerid, BaseTD[playerid][TD_INPUT_TXT], bool:1);

    BaseTD[playerid][TD_SCROLL_DOWN] = CreatePlayerTextDraw(playerid, 605.0, 415.0, "ld_beat:down"); 
    PlayerTextDrawFont(playerid, BaseTD[playerid][TD_SCROLL_DOWN], TEXT_DRAW_FONT:4);
    PlayerTextDrawLetterSize(playerid, BaseTD[playerid][TD_SCROLL_DOWN], 0.6, 2.0);
    PlayerTextDrawTextSize(playerid, BaseTD[playerid][TD_SCROLL_DOWN], 15.0, 12.5);
    PlayerTextDrawAlignment(playerid, BaseTD[playerid][TD_SCROLL_DOWN], TEXT_DRAW_ALIGN:1);
    PlayerTextDrawColour(playerid, BaseTD[playerid][TD_SCROLL_DOWN], 1687547391);
    PlayerTextDrawBackgroundColour(playerid, BaseTD[playerid][TD_SCROLL_DOWN], 255);
    PlayerTextDrawBoxColour(playerid, BaseTD[playerid][TD_SCROLL_DOWN], 50);
    PlayerTextDrawUseBox(playerid, BaseTD[playerid][TD_SCROLL_DOWN], bool:1);
    PlayerTextDrawSetProportional(playerid, BaseTD[playerid][TD_SCROLL_DOWN], bool:1);
    PlayerTextDrawSetSelectable(playerid, BaseTD[playerid][TD_SCROLL_DOWN], bool:1);

    // B. VÒNG LẶP TỰ ĐỘNG TẠO DANH SÁCH LIÊN HỆ (LIST VIEW)
    for(new i = 0; i < MAX_VISIBLE_CONTACTS; i++)
    {
        new Float:Y_Offset = i * CONTACT_ROW_SPACING;

        ContactRowTD[playerid][i][TD_ROW_BG] = CreatePlayerTextDraw(playerid, 555.0, 270.0 + Y_Offset, "_");
        PlayerTextDrawFont(playerid, ContactRowTD[playerid][i][TD_ROW_BG], TEXT_DRAW_FONT:1);
        PlayerTextDrawLetterSize(playerid, ContactRowTD[playerid][i][TD_ROW_BG], 0.0, 2.5);
        PlayerTextDrawTextSize(playerid, ContactRowTD[playerid][i][TD_ROW_BG], 298.5, 122.0);
        PlayerTextDrawAlignment(playerid, ContactRowTD[playerid][i][TD_ROW_BG], TEXT_DRAW_ALIGN:2);
        PlayerTextDrawBoxColour(playerid, ContactRowTD[playerid][i][TD_ROW_BG], 150);
        PlayerTextDrawUseBox(playerid, ContactRowTD[playerid][i][TD_ROW_BG], bool:1);

        ContactRowTD[playerid][i][TD_ROW_NAME] = CreatePlayerTextDraw(playerid, 496.0, 271.0 + Y_Offset, "Ten lien he"); 
        PlayerTextDrawFont(playerid, ContactRowTD[playerid][i][TD_ROW_NAME], TEXT_DRAW_FONT:1);
        PlayerTextDrawLetterSize(playerid, ContactRowTD[playerid][i][TD_ROW_NAME], 0.2, 1.0);
        PlayerTextDrawTextSize(playerid, ContactRowTD[playerid][i][TD_ROW_NAME], 606.0, 20.5); // Vùng nhấp chuột tên
        PlayerTextDrawSetShadow(playerid, ContactRowTD[playerid][i][TD_ROW_NAME], 0);
        PlayerTextDrawAlignment(playerid, ContactRowTD[playerid][i][TD_ROW_NAME], TEXT_DRAW_ALIGN:1);
        PlayerTextDrawColour(playerid, ContactRowTD[playerid][i][TD_ROW_NAME], -1);
        PlayerTextDrawSetSelectable(playerid, ContactRowTD[playerid][i][TD_ROW_NAME], bool:1); // Clickable

        ContactRowTD[playerid][i][TD_ROW_LAST_MSG] = CreatePlayerTextDraw(playerid, 496.0, 284.0 + Y_Offset, "Noi dung..."); 
        PlayerTextDrawFont(playerid, ContactRowTD[playerid][i][TD_ROW_LAST_MSG], TEXT_DRAW_FONT:1);
        PlayerTextDrawLetterSize(playerid, ContactRowTD[playerid][i][TD_ROW_LAST_MSG], 0.1, 0.8);
        PlayerTextDrawTextSize(playerid, ContactRowTD[playerid][i][TD_ROW_LAST_MSG], 580.5, 20.0);
        PlayerTextDrawAlignment(playerid, ContactRowTD[playerid][i][TD_ROW_LAST_MSG], TEXT_DRAW_ALIGN:1);
        PlayerTextDrawSetShadow(playerid, ContactRowTD[playerid][i][TD_ROW_LAST_MSG], 0);
        PlayerTextDrawColour(playerid, ContactRowTD[playerid][i][TD_ROW_LAST_MSG], -76);

        ContactRowTD[playerid][i][TD_ROW_TIME] = CreatePlayerTextDraw(playerid, 584.0, 284.0 + Y_Offset, "00:00"); 
        PlayerTextDrawFont(playerid, ContactRowTD[playerid][i][TD_ROW_TIME], TEXT_DRAW_FONT:1);
        PlayerTextDrawLetterSize(playerid, ContactRowTD[playerid][i][TD_ROW_TIME], 0.1, 0.8);
        PlayerTextDrawTextSize(playerid, ContactRowTD[playerid][i][TD_ROW_TIME], 614.0, 20.0);
        PlayerTextDrawAlignment(playerid, ContactRowTD[playerid][i][TD_ROW_TIME], TEXT_DRAW_ALIGN:1);
        PlayerTextDrawSetShadow(playerid, ContactRowTD[playerid][i][TD_ROW_TIME], 0);
        PlayerTextDrawColour(playerid, ContactRowTD[playerid][i][TD_ROW_TIME], -76);

        ContactRowTD[playerid][i][TD_ROW_STATUS] = CreatePlayerTextDraw(playerid, 608.0, 274.0 + Y_Offset, "ld_pool:ball"); 
        PlayerTextDrawFont(playerid, ContactRowTD[playerid][i][TD_ROW_STATUS], TEXT_DRAW_FONT:4);
        PlayerTextDrawTextSize(playerid, ContactRowTD[playerid][i][TD_ROW_STATUS], 5.0, 5.0);
        PlayerTextDrawColour(playerid, ContactRowTD[playerid][i][TD_ROW_STATUS], 16711935);
    }

    // C. VÒNG LẶP TỰ ĐỘNG TẠO KHUNG TRÒ CHUYỆN ĐA TIN NHẮN
    for(new i = 0; i < MAX_VISIBLE_MESSAGES; i++)
    {
        new Float:Y_Offset = i * CHAT_ROW_SPACING;

        ChatRowTD[playerid][i][TD_CHAT_SENDER] = CreatePlayerTextDraw(playerid, 494.0, 274.0 + Y_Offset, "Nguoi gui"); 
        PlayerTextDrawFont(playerid, ChatRowTD[playerid][i][TD_CHAT_SENDER], TEXT_DRAW_FONT:1);
        PlayerTextDrawLetterSize(playerid, ChatRowTD[playerid][i][TD_CHAT_SENDER], 0.1, 0.7);
        PlayerTextDrawAlignment(playerid, ChatRowTD[playerid][i][TD_CHAT_SENDER], TEXT_DRAW_ALIGN:1);
        PlayerTextDrawSetShadow(playerid, ChatRowTD[playerid][i][TD_CHAT_SENDER], 0);
        PlayerTextDrawColour(playerid, ChatRowTD[playerid][i][TD_CHAT_SENDER], 1296911871);

        ChatRowTD[playerid][i][TD_CHAT_BODY] = CreatePlayerTextDraw(playerid, 494.0, 281.0 + Y_Offset, "Noi dung chat..."); 
        PlayerTextDrawFont(playerid, ChatRowTD[playerid][i][TD_CHAT_BODY], TEXT_DRAW_FONT:1);
        PlayerTextDrawLetterSize(playerid, ChatRowTD[playerid][i][TD_CHAT_BODY], 0.1, 0.9);
        PlayerTextDrawTextSize(playerid, ChatRowTD[playerid][i][TD_CHAT_BODY], 615.5, 12.0); // Vùng click dòng chat
        PlayerTextDrawAlignment(playerid, ChatRowTD[playerid][i][TD_CHAT_BODY], TEXT_DRAW_ALIGN:1);
        PlayerTextDrawSetShadow(playerid, ChatRowTD[playerid][i][TD_CHAT_BODY], 0);
        PlayerTextDrawColour(playerid, ChatRowTD[playerid][i][TD_CHAT_BODY], 255);
        PlayerTextDrawSetSelectable(playerid, ChatRowTD[playerid][i][TD_CHAT_BODY], bool:1); // Clickable để xem chi tiết!

        ChatRowTD[playerid][i][TD_CHAT_TIME] = CreatePlayerTextDraw(playerid, 604.0, 274.0 + Y_Offset, "00:00"); 
        PlayerTextDrawFont(playerid, ChatRowTD[playerid][i][TD_CHAT_TIME], TEXT_DRAW_FONT:1);
        PlayerTextDrawLetterSize(playerid, ChatRowTD[playerid][i][TD_CHAT_TIME], 0.1, 0.7);
        PlayerTextDrawSetShadow(playerid, ChatRowTD[playerid][i][TD_CHAT_TIME], 0);
        PlayerTextDrawAlignment(playerid, ChatRowTD[playerid][i][TD_CHAT_TIME], TEXT_DRAW_ALIGN:2);
        PlayerTextDrawColour(playerid, ChatRowTD[playerid][i][TD_CHAT_TIME], 1296911871);
    }
    return 1;
}

hook OnPlayerDisconnect(playerid, reason)
{
    // Giải phóng bộ nhớ toàn bộ TextDraw tránh tràn giới hạn
    for(new i = 0; i < E_BASE_TD; i++) {
        if(BaseTD[playerid][i] != INVALID_PLAYER_TEXT_DRAW) PlayerTextDrawDestroy(playerid, BaseTD[playerid][i]);
    }
    for(new i = 0; i < MAX_VISIBLE_CONTACTS; i++) {
        for(new j = 0; j < E_CONTACT_ROW; j++) {
            if(ContactRowTD[playerid][i][j] != INVALID_PLAYER_TEXT_DRAW) PlayerTextDrawDestroy(playerid, ContactRowTD[playerid][i][j]);
        }
    }
    for(new i = 0; i < MAX_VISIBLE_MESSAGES; i++) {
        for(new j = 0; j < E_CHAT_ROW; j++) {
            if(ChatRowTD[playerid][i][j] != INVALID_PLAYER_TEXT_DRAW) PlayerTextDrawDestroy(playerid, ChatRowTD[playerid][i][j]);
        }
    }
    return 1;
}

// =============================================================================
// UI CONTROL FUNCTIONS
// =============================================================================
stock HidePlayerMessenger(playerid)
{
    for(new i = 0; i < E_BASE_TD; i++) PlayerTextDrawHide(playerid, BaseTD[playerid][i]);
    
    for(new i = 0; i < MAX_VISIBLE_CONTACTS; i++) {
        for(new j = 0; j < E_CONTACT_ROW; j++) PlayerTextDrawHide(playerid, ContactRowTD[playerid][i][j]);
    }
    for(new i = 0; i < MAX_VISIBLE_MESSAGES; i++) {
        for(new j = 0; j < E_CHAT_ROW; j++) PlayerTextDrawHide(playerid, ChatRowTD[playerid][i][j]);
    }
    CancelSelectTextDraw(playerid);
    PlayerMsgData[playerid][p_MsgState] = MSG_STATE_CLOSED;
}

stock OpenContactList(playerid)
{
    PlayerMsgData[playerid][p_MsgState] = MSG_STATE_CONTACTS;
    PlayerMsgData[playerid][p_ScrollIndex] = 0;

    // Tải danh sách liên hệ từ Backend đổ vào UI trước
    Backend_LoadContactList(playerid, PlayerMsgData[playerid][p_ScrollIndex]);

    // Hiện Base UI dùng chung
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_MAIN_BG]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_INNER_BG]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_INPUT_BG]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_TITLE]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_CLOSE_BTN]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_SCROLL_UP]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_SCROLL_DOWN]);

    // Ẩn vùng nhập liệu chat
    // PlayerTextDrawHide(playerid, BaseTD[playerid][TD_INPUT_BG]);
    PlayerTextDrawHide(playerid, BaseTD[playerid][TD_INPUT_TXT]);

    // Ẩn các dòng chat cũ
    for(new i = 0; i < MAX_VISIBLE_MESSAGES; i++) {
        for(new j = 0; j < E_CHAT_ROW; j++) PlayerTextDrawHide(playerid, ChatRowTD[playerid][i][j]);
    }

    SelectTextDraw(playerid, 0xFF0000FF);
}

stock OpenChatBox(playerid, target_account_id)
{
    PlayerMsgData[playerid][p_MsgState] = MSG_STATE_CHAT_VIEW;
    PlayerMsgData[playerid][p_TargetAccountID] = target_account_id;
    PlayerMsgData[playerid][p_ScrollIndex] = 0;

    // Tải nội dung hội thoại chat từ Backend đổ vào UI & Cache
    Backend_LoadChatMessage(playerid, target_account_id, PlayerMsgData[playerid][p_ScrollIndex]);

    // Ẩn các hàng liên hệ ngoài danh sách
    for(new i = 0; i < MAX_VISIBLE_CONTACTS; i++) {
        for(new j = 0; j < E_CONTACT_ROW; j++) PlayerTextDrawHide(playerid, ContactRowTD[playerid][i][j]);
    }

    // Hiện vùng nhập liệu chat chuyên sâu
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_INPUT_BG]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_INPUT_TXT]);

    // Giữ các nút điều hướng
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_MAIN_BG]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_INNER_BG]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_TITLE]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_CLOSE_BTN]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_SCROLL_UP]);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_SCROLL_DOWN]);
}

// =============================================================================
// INTERACTION CONTROLLER (CLICK & DIALOG RESPONSES)
// =============================================================================
hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if(PlayerMsgData[playerid][p_MsgState] == MSG_STATE_CLOSED) return 0;

    // Nút đóng hệ thống công cộng
    if(playertextid == BaseTD[playerid][TD_CLOSE_BTN]) {
        HidePlayerMessenger(playerid);
        return 1;
    }

    // A. XỬ LÝ KHI Ở MÀN HÌNH DANH SÁCH LIÊN HỆ
    if(PlayerMsgData[playerid][p_MsgState] == MSG_STATE_CONTACTS) 
    {
        // 1. Quét kiểm tra xem người chơi click vào dòng liên hệ thứ mấy (0 -> 3)
        for(new i = 0; i < MAX_VISIBLE_CONTACTS; i++) {
            if(playertextid == ContactRowTD[playerid][i][TD_ROW_NAME]) {
                new target_click_index = PlayerMsgData[playerid][p_ScrollIndex] + i;
                
                // Mock logic: Lấy ID từ danh sách (ở đây gán đại diện test thử)
                new mock_target_id = (target_click_index == 0) ? 154 : 202; 
                OpenChatBox(playerid, mock_target_id);
                return 1;
            }
        }

        // 2. Click nút Cuộn Lên ngoài danh sách
        if(playertextid == BaseTD[playerid][TD_SCROLL_UP]) {
            if(PlayerMsgData[playerid][p_ScrollIndex] > 0) {
                PlayerMsgData[playerid][p_ScrollIndex]--;
                Backend_LoadContactList(playerid, PlayerMsgData[playerid][p_ScrollIndex]);
            }
            return 1;
        }

        // 3. Click nút Cuộn Xuống ngoài danh sách
        if(playertextid == BaseTD[playerid][TD_SCROLL_DOWN]) {
            if(PlayerMsgData[playerid][p_ScrollIndex] < (PlayerMsgData[playerid][p_TotalItems] - MAX_VISIBLE_CONTACTS)) {
                PlayerMsgData[playerid][p_ScrollIndex]++;
                Backend_LoadContactList(playerid, PlayerMsgData[playerid][p_ScrollIndex]);
            }
            return 1;
        }
    }

    // B. XỬ LÝ KHI ĐANG Ở TRONG KHUNG CHAT CHI TIẾT
    if(PlayerMsgData[playerid][p_MsgState] == MSG_STATE_CHAT_VIEW) 
    {
        // 1. Nhấp vào nút "Soạn Tin Nhắn" để mở Dialog gõ chữ
        if(playertextid == BaseTD[playerid][TD_INPUT_TXT]) {
            ShowPlayerDialog(playerid, DIALOG_MESSENGER_INPUT, DIALOG_STYLE_INPUT, "Soan tin nhan", "Nhap noi dung tin nhan (Toi da 120 ky tu):", "Gui", "Huy");
            return 1;
        }

        // 2. Click vào từng dòng tin nhắn để XEM CHI TIẾT ĐẦY ĐỦ CHUỖI GỐC (Tránh tràn chữ)
        for(new i = 0; i < MAX_VISIBLE_MESSAGES; i++) {
            if(playertextid == ChatRowTD[playerid][i][TD_CHAT_BODY]) {
                if(strlen(FullChatCache[playerid][i]) == 0) return 1;

                new dialog_content[256];
                format(dialog_content, sizeof(dialog_content), "{FFFFFF}%s", FullChatCache[playerid][i]);
                ShowPlayerDialog(playerid, DIALOG_VIEW_FULL_MSG, DIALOG_STYLE_MSGBOX, "Noi dung day du", dialog_content, "Dong", "");
                return 1;
            }
        }

        // 3. Cuộn ngược lên xem tin nhắn CŨ hơn
        if(playertextid == BaseTD[playerid][TD_SCROLL_UP]) {
            if(PlayerMsgData[playerid][p_ScrollIndex] < (PlayerMsgData[playerid][p_TotalItems] - MAX_VISIBLE_MESSAGES)) {
                PlayerMsgData[playerid][p_ScrollIndex]++;
                Backend_LoadChatMessage(playerid, PlayerMsgData[playerid][p_TargetAccountID], PlayerMsgData[playerid][p_ScrollIndex]);
            }
            return 1;
        }

        // 4. Cuộn xuống xem tin nhắn MỚI hơn
        if(playertextid == BaseTD[playerid][TD_SCROLL_DOWN]) {
            if(PlayerMsgData[playerid][p_ScrollIndex] > 0) {
                PlayerMsgData[playerid][p_ScrollIndex]--;
                Backend_LoadChatMessage(playerid, PlayerMsgData[playerid][p_TargetAccountID], PlayerMsgData[playerid][p_ScrollIndex]);
            }
            return 1;
        }
    }
    return 0;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_MESSENGER_INPUT) 
    {
        if(!response) return 1;

        new msg_length = strlen(inputtext);
        if(msg_length == 0 || msg_length > MAX_MSG_LENGTH) {
            SendClientMessage(playerid, 0xFF0000FF, "[MESSENGER] Tin nhan khong hop le hoac vuot qua 120 ky tu.");
            return 1;
        }

        new target_id = PlayerMsgData[playerid][p_TargetAccountID];
        if(target_id == 0) return 1;

        // Lưu vào DB qua Backend
        Backend_SaveAndSendMessage(playerid, target_id, inputtext);

        // Đưa cuộn về 0 để hiển thị ngay lập tức tin nhắn vừa nhắn xong lên đầu
        PlayerMsgData[playerid][p_ScrollIndex] = 0;
        Backend_LoadChatMessage(playerid, target_id, PlayerMsgData[playerid][p_ScrollIndex]);
        return 1;
    }
    return 0;
}

// =============================================================================
// BACKEND LOGIC SKELETON (NƠI KẾT NỐI DATABASE MYSQL/SQLITE CỦA BẠN)
// =============================================================================

public Backend_LoadContactList(playerid, start_index)
{
    // [Khu vực lập trình MySQL]: 
    // SELECT * FROM contacts WHERE user_id = ... LIMIT 4 OFFSET :start_index;
    
    PlayerMsgData[playerid][p_TotalItems] = 6; // Giả lập tổng cộng DB có 6 người bạn

    for(new i = 0; i < MAX_VISIBLE_CONTACTS; i++)
    {
        new current_data_offset = start_index + i;

        if(current_data_offset >= PlayerMsgData[playerid][p_TotalItems]) {
            // Nếu vị trí vượt quá tổng dữ liệu hiện có -> Ẩn dòng thừa đi
            for(new j = 0; j < E_CONTACT_ROW; j++) PlayerTextDrawHide(playerid, ContactRowTD[playerid][i][j]);
            continue;
        }

        new name_buffer[32], raw_msg_buffer[64], short_msg_buffer[28], time_buffer[16], bool:online_status;

        // Giả lập nạp dữ liệu phân trang thực tế từ DB
        if(current_data_offset == 0) {
            format(name_buffer, sizeof(name_buffer), "Nguyen_Van_A");
            format(raw_msg_buffer, sizeof(raw_msg_buffer), "You: Di mua sung thoi bro oi em cho san o net roi..");
            format(time_buffer, sizeof(time_buffer), "12:00"); online_status = true;
        }
        else if(current_data_offset == 1) {
            format(name_buffer, sizeof(name_buffer), "Tran_Thi_B");
            format(raw_msg_buffer, sizeof(raw_msg_buffer), "B: Toi nay co di kien hang ko, admin thong bao kìa");
            format(time_buffer, sizeof(time_buffer), "Hqua"); online_status = false;
        }
        else {
            format(name_buffer, sizeof(name_buffer), "Nguoi_Dung_Luu_Tru");
            format(raw_msg_buffer, sizeof(raw_msg_buffer), "Alo alo 1 2 3 4...");
            format(time_buffer, sizeof(time_buffer), "01/06"); online_status = false;
        }

        // Thực hiện cắt chuỗi an toàn ngoài danh sách (Lấy tối đa 24 kí tự preview)
        TruncateString(raw_msg_buffer, short_msg_buffer, 24);

        // Đổ chữ vào UI
        PlayerTextDrawSetString(playerid, ContactRowTD[playerid][i][TD_ROW_NAME], name_buffer);
        PlayerTextDrawSetString(playerid, ContactRowTD[playerid][i][TD_ROW_LAST_MSG], short_msg_buffer);
        PlayerTextDrawSetString(playerid, ContactRowTD[playerid][i][TD_ROW_TIME], time_buffer);
        
        if(online_status) PlayerTextDrawColour(playerid, ContactRowTD[playerid][i][TD_ROW_STATUS], 0x00FF00FF);
        else PlayerTextDrawColour(playerid, ContactRowTD[playerid][i][TD_ROW_STATUS], 0xAAAAAAFF);

        // Hiển thị lại toàn bộ dòng i sau khi xử lý dữ liệu sạch
        for(new j = 0; j < E_CONTACT_ROW; j++) PlayerTextDrawShow(playerid, ContactRowTD[playerid][i][j]);
    }
    return 1;
}

public Backend_LoadChatMessage(playerid, target_account_id, start_offset)
{
    // [Khu vực lập trình MySQL]:
    // SELECT * FROM messages WHERE (sender = my_id AND receiver = target_id) ORDER BY id DESC LIMIT 4 OFFSET :start_offset;

    PlayerMsgData[playerid][p_TotalItems] = 5; // Giả lập cuộc trò chuyện có 5 tin nhắn

    new title_string[32];
    format(title_string, sizeof(title_string), "Hoi thoai: %d", target_account_id);
    PlayerTextDrawSetString(playerid, BaseTD[playerid][TD_TITLE], title_string);
    PlayerTextDrawShow(playerid, BaseTD[playerid][TD_TITLE]);

    for(new i = 0; i < MAX_VISIBLE_MESSAGES; i++)
    {
        new current_msg_offset = start_offset + i;

        if(current_msg_offset >= PlayerMsgData[playerid][p_TotalItems]) {
            // Không có tin nhắn tại vị trí offset này -> Ẩn dòng chat trống đi
            for(new j = 0; j < E_CHAT_ROW; j++) PlayerTextDrawHide(playerid, ChatRowTD[playerid][i][j]);
            // Xóa rỗng cache dòng này
            FullChatCache[playerid][i][0] = '\0';
            continue;
        }

        new sender_buf[32], body_buf[MAX_MSG_LENGTH], short_body_buf[35], time_buf[16];

        // Giả lập bốc dữ liệu tin nhắn dài từ DB
        if(current_msg_offset == 0) {
            format(sender_buf, sizeof(sender_buf), "You");
            format(body_buf, sizeof(body_buf), "Toi nay 8h di cuop truck nho mang theo giap day du nha ong ban oi!");
            format(time_buf, sizeof(time_buf), "20:01");
        }
        else if(current_msg_offset == 1) {
            format(sender_buf, sizeof(sender_buf), "Doi Phuong");
            format(body_buf, sizeof(body_buf), "Ok ong, toi chuan bi di mua deagle day. can mua ho khong?");
            format(time_buf, sizeof(time_buf), "20:00");
        }
        else {
            format(sender_buf, sizeof(sender_buf), "You");
            format(body_buf, sizeof(body_buf), "Tin nhan co dinh kiem tra tinh nang cuon trang du lieu chat.");
            format(time_buf, sizeof(time_buf), "19:45");
        }

        // Bước quan trọng 1: Đưa chuỗi gốc ĐẦY ĐỦ vào Cache bộ nhớ đệm
        format(FullChatCache[playerid][i], MAX_MSG_LENGTH, "%s", body_buf);

        // Bước quan trọng 2: Cắt chuỗi gọn gàng xuống 35 ký tự để hiện trên màn hình TextDraw cho đẹp
        TruncateString(body_buf, short_body_buf, 35);

        // Gán chuỗi sạch vào giao diện hiển thị
        PlayerTextDrawSetString(playerid, ChatRowTD[playerid][i][TD_CHAT_SENDER], sender_buf);
        PlayerTextDrawSetString(playerid, ChatRowTD[playerid][i][TD_CHAT_BODY], short_body_buf);
        PlayerTextDrawSetString(playerid, ChatRowTD[playerid][i][TD_CHAT_TIME], time_buf);

        // Đẩy lên màn hình
        for(new j = 0; j < E_CHAT_ROW; j++) PlayerTextDrawShow(playerid, ChatRowTD[playerid][i][j]);
    }
    return 1;
}

public Backend_SaveAndSendMessage(playerid, target_account_id, const message[])
{
    // [Khu vực lập trình MySQL]:
    // INSERT INTO messages (sender, receiver, content, time) VALUES (...)
    
    // printf("[BACKEND TEST]: Player %d gui den Account ID %d noi dung: %s", playerid, target_account_id, message);
    return 1;
}

// =============================================================================
// UTILITY HELPERS
// =============================================================================
public TruncateString(const source[], dest[], max_len)
{
    if(strlen(source) <= max_len) {
        format(dest, max_len, "%s", source);
    }
    else {
        format(dest, max_len, "%.*s...", max_len - 3, source);
    }
    return 1;
}

// Lệnh mẫu test mở điện thoại lên thử nghiệm
CMD:messager(playerid, params[])
{
    OpenContactList(playerid);
    return 1;
}