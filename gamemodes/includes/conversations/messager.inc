/*
#define MAX_CONVERSATIONS     (1000)
#define MAX_CONV_MESSAGES     (50)

#define DIALOG_MESSENGER_LIST           (1000)
#define DIALOG_CONVERSATION_VIEW        (1001)
#define DIALOG_SEND_MESSAGE             (1002)
#define DIALOG_NEW_MESSAGE              (1003)

// Player state
enum E_PLAYER_MESSENGER
{
    CurrentConvID,
    CurrentCacheID
};

new PlayerMessenger[MAX_PLAYERS][E_PLAYER_MESSENGER];

enum E_CONVERSATION_CACHE
{
    ConvID,
    ConvPlayer1,
    ConvPlayer2,
    ConvMessageCount,
    bool:ConvLoaded
};

new ConversationCache[MAX_CONVERSATIONS][E_CONVERSATION_CACHE];

enum E_MESSAGE_CACHE
{
    MsgSenderID,
    MsgTimestamp,
    MsgText[256]
};

new ConversationMessages[MAX_CONVERSATIONS][MAX_CONV_MESSAGES][E_MESSAGE_CACHE];
*/

stock FindConversationCache(convid)
{
    for (new i; i < MAX_CONVERSATIONS; i++)
    {
        if (
            ConversationCache[i][ConvLoaded]
            &&
            ConversationCache[i][ConvID] == convid
        )
        {
            return i;
        }
    }

    return -1;
}
stock CreateConversationCache(convid, player1, player2)
{
    for (new i; i < MAX_CONVERSATIONS; i++)
    {
        if (!ConversationCache[i][ConvLoaded])
        {
            ConversationCache[i][ConvLoaded] = true;

            ConversationCache[i][ConvID] = convid;
            ConversationCache[i][ConvPlayer1] = player1;
            ConversationCache[i][ConvPlayer2] = player2;
            ConversationCache[i][ConvMessageCount] = 0;

            return i;
        }
    }

    return -1;
}
stock AddConversationMessage(cacheid, senderid, const text[])
{
    if (cacheid == -1)
        return 0;

    new count = ConversationCache[cacheid][ConvMessageCount];

    if (count >= MAX_CONV_MESSAGES)
    {
        for (new i = 1; i < MAX_CONV_MESSAGES; i++)
        {
            ConversationMessages[cacheid][i - 1][MsgSenderID]
                =
                    ConversationMessages[cacheid][i][MsgSenderID];

            ConversationMessages[cacheid][i - 1][MsgTimestamp]
                =
                    ConversationMessages[cacheid][i][MsgTimestamp];

            format(
                ConversationMessages[cacheid][i - 1][MsgText],
                256,
                "%s",
                ConversationMessages[cacheid][i][MsgText]
            );
        }

        count = MAX_CONV_MESSAGES - 1;
    }

    ConversationMessages[cacheid][count][MsgSenderID]
        = senderid;

    ConversationMessages[cacheid][count][MsgTimestamp]
        = gettime();

    format(
        ConversationMessages[cacheid][count][MsgText],
        256,
        "%s",
        text
    );

    ConversationCache[cacheid][ConvMessageCount]
        = count + 1;

    return 1;
}

stock SaveMessageSQL(convid, senderid, const text[])
{
    new query[700];
    new escaped[512];

    mysql_escape_string(text, escaped);

    mysql_format(
        g_DatabaseHandle,
        query,
        sizeof(query),
        "INSERT INTO Messages \
        (ConversationID, SenderID, Content) \
        VALUES (%d,%d,'%e')",
        convid,
        senderid,
        escaped
    );

    mysql_tquery(g_DatabaseHandle, query);

    mysql_format(
        g_DatabaseHandle,
        query,
        sizeof(query),
        "UPDATE Conversations \
        SET Timestamp = CURRENT_TIMESTAMP \
        WHERE ID = %d",
        convid
    );

    mysql_tquery(g_DatabaseHandle, query);

    return 1;
}
stock SendConversationMessage(playerid, convid, const text[])
{
    new cacheid = FindConversationCache(convid);

    if (cacheid == -1)
        return 0;

    AddConversationMessage(
        cacheid,
        PlayerInfo[playerid][pID],
        text
    );

    SaveMessageSQL(
        convid,
        PlayerInfo[playerid][pID],
        text
    );

    // UpdateConversationForPlayers(cacheid);

    return 1;
}
stock LoadConversation(convid)
{
    new query[256];

    mysql_format(
        g_DatabaseHandle,
        query,
        sizeof(query),
        "SELECT * \
        FROM messages \
        WHERE ConversationID = %d \
        ORDER BY ID ASC \
        LIMIT %d",
        convid,
        MAX_CONV_MESSAGES
    );

    mysql_tquery(
        g_DatabaseHandle,
        query,
        "OnConversationLoaded",
        "i",
        convid
    );

    return 1;
}

forward OnConversationLoaded(convid);
public OnConversationLoaded(convid)
{
    new rows;

    cache_get_row_count(rows);

    new cacheid = FindConversationCache(convid);

    if (cacheid == -1)
        return 1;

    ConversationCache[cacheid][ConvMessageCount] = 0;

    for (new i; i < rows; i++)
    {
        cache_get_value_name_int(
            i,
            "SenderID",
            ConversationMessages[cacheid][i][MsgSenderID]
        );

        cache_get_value_name(
            i,
            "Content",
            ConversationMessages[cacheid][i][MsgText],
            256
        );

        cache_get_value_name_int(
            i,
            "Timestamp",
            ConversationMessages[cacheid][i][MsgTimestamp]
        );

        ConversationCache[cacheid][ConvMessageCount]++;
    }

    return 1;
}
stock BuildConversationText(cacheid, output[], size)
{
    output[0] = EOS;

    new count =
        ConversationCache[cacheid][ConvMessageCount];

    for (new i; i < count; i++)
    {
        new line[350];

        format(
            line,
            sizeof(line),
            "%d: %s\n",
            ConversationMessages[cacheid][i][MsgSenderID],
            ConversationMessages[cacheid][i][MsgText]
        );

        strcat(output, line);
    }

    return 1;
}

// Get or create conversation between two players
stock GetOrCreateConversation(player1, player2, &convid)
{
    new query[256];

    if (player1 > player2)
    {
        new tmp = player1;
        player1 = player2;
        player2 = tmp;
    }

    mysql_format(
        g_DatabaseHandle,
        query,
        sizeof(query),
        "SELECT ID FROM conversations \
        WHERE (Player1 = %d AND Player2 = %d) \
        OR (Player1 = %d AND Player2 = %d) \
        LIMIT 1",
        player1,
        player2,
        player2,
        player1
    );

    new rows, id;
    cache_get_row_count(rows);

    if (rows > 0)
    {
        cache_get_value_name_int(0, "ID", id);
        convid = id;
        return 1;
    }

    // Tạo conversation mới
    mysql_format(
        g_DatabaseHandle,
        query,
        sizeof(query),
        "INSERT INTO Conversations (Player1, Player2, Timestamp) \
        VALUES (%d, %d, CURRENT_TIMESTAMP)",
        player1,
        player2
    );

    mysql_tquery(g_DatabaseHandle, query);
    return 1;
}

// Display conversation messages with list dialog
stock ShowConversationDialog(playerid, convid)
{
    new cacheid = FindConversationCache(convid);

    if (cacheid == -1)
    {
        LoadConversation(convid);
        return 0;
    }

    new count = ConversationCache[cacheid][ConvMessageCount];
    new dialogText[4096] = "ID\t{00FF00}Người gửi{FFFFFF}\t{FF9900}Nội dung\n";

    for (new i = 0; i < count; i++)
    {
        new senderid = ConversationMessages[cacheid][i][MsgSenderID];
        new senderName[MAX_PLAYER_NAME];

        // Lấy tên người gửi từ database hoặc database cache
        new query[256];
        mysql_format(
            g_DatabaseHandle,
            query,
            sizeof(query),
            "SELECT Name FROM Players WHERE ID = %d",
            senderid
        );

        // new senderNameResult[MAX_PLAYER_NAME];

        // Sử dụng placeholder nếu không tìm được
        format(senderName, MAX_PLAYER_NAME, "[ID:%d]", senderid);

        new msgLine[512];
        format(
            msgLine,
            sizeof(msgLine),
            "%d\t%s\t%.50s...\n",
            i + 1,
            senderName,
            ConversationMessages[cacheid][i][MsgText]
        );

        strcat(dialogText, msgLine);
    }

    PlayerMessenger[playerid][CurrentConvID] = convid;
    PlayerMessenger[playerid][CurrentCacheID] = cacheid;

    ShowPlayerDialog(
        playerid,
        DIALOG_CONVERSATION_VIEW,
        DIALOG_STYLE_TABLIST_HEADERS,
        "Xem Cuộc Trò Chuyện",
        dialogText,
        "Trả Lời",
        "Đóng"
    );

    return 1;
}

// Show list of conversations
stock ShowConversationList(playerid)
{
    new query[256];

    mysql_format(
        g_DatabaseHandle,
        query,
        sizeof(query),
        "SELECT ID, Player1, Player2 FROM Conversations \
        WHERE Player1 = %d OR Player2 = %d \
        ORDER BY Timestamp DESC \
        LIMIT 20",
        PlayerInfo[playerid][pID],
        PlayerInfo[playerid][pID]
    );

    mysql_tquery(
        g_DatabaseHandle,
        query,
        "OnConversationListLoaded",
        "i",
        playerid
    );

    return 1;
}

forward OnConversationListLoaded(playerid);
public OnConversationListLoaded(playerid)
{
    new rows;
    cache_get_row_count(rows);

    if (!IsPlayerConnected(playerid))
        return 0;

    new dialogText[3000] = "{00FF00}ID\t{FFFF00}Người khác\t{FF9900}Trạng thái\n";

    for (new i = 0; i < rows; i++)
    {
        new convid, player1, player2;

        cache_get_value_name_int(i, "ID", convid);
        cache_get_value_name_int(i, "Player1", player1);
        cache_get_value_name_int(i, "Player2", player2);

        new otherPlayerID = (player1 == PlayerInfo[playerid][pID]) ? player2 : player1;

        new line[256];
        format(
            line,
            sizeof(line),
            "%d\t[ID:%d]\t{90EE90}Đang hoạt động\n",
            convid,
            otherPlayerID
        );

        strcat(dialogText, line);
    }

    ShowPlayerDialog(
        playerid,
        DIALOG_MESSENGER_LIST,
        DIALOG_STYLE_TABLIST_HEADERS,
        "Danh Sách Tin Nhắn",
        dialogText,
        "Xem",
        "Đóng"
    );

    return 1;
}
// Dialog response handler
stock HandleDialogConversation(playerid, dialogid, response, listitem, inputtext[])
{
    switch (dialogid)
    {
        case DIALOG_MESSENGER_LIST:
        {
            if (!response)
                return 1;

            // Lấy convid từ listitem
            // Cần lưu danh sách conversation ID khi hiển thị
            new query[256];
            mysql_format(
                g_DatabaseHandle,
                query,
                sizeof(query),
                "SELECT ID FROM conversations \
            WHERE Player1 = %d OR Player2 = %d \
            ORDER BY Timestamp DESC \
            LIMIT %d,1",
                PlayerInfo[playerid][pID],
                PlayerInfo[playerid][pID],
                listitem
            );

            mysql_tquery(
                g_DatabaseHandle,
                query,
                "OnConversationSelected",
                "ii",
                playerid,
                listitem
            );

            return 1;
        }

        case DIALOG_CONVERSATION_VIEW:
        {
            if (!response)
                return 1;

            ShowPlayerDialog(
                playerid,
                DIALOG_SEND_MESSAGE,
                DIALOG_STYLE_INPUT,
                "Messager",
                "Nhap noi dung tin nhan cua ban:",
                "Gui",
                "Quay Lai"
            );

            return 1;
        }

        case DIALOG_SEND_MESSAGE:
        {
            if (!response)
                return ShowConversationDialog(playerid, PlayerMessenger[playerid][CurrentConvID]);

            if (!inputtext[0])
            {
                SendClientMessage(playerid, 0xFF0000FF, "ERROR: Vui lòng nhập nội dung!");
                return ShowPlayerDialog(
                           playerid,
                           DIALOG_SEND_MESSAGE,
                           DIALOG_STYLE_INPUT,
                           "Gửi Tin Nhắn",
                           "Nhập nội dung tin nhắn của bạn:",
                           "Gửi",
                           "Quay Lại"
                       );
            }

            new convid = PlayerMessenger[playerid][CurrentConvID];
            new cacheid = PlayerMessenger[playerid][CurrentCacheID];

            // Gửi tin nhắn
            SendConversationMessage(playerid, convid, inputtext);

            SendClientMessage(playerid, 0x00FF00FF, "Tin nhan da gui !");

            // Hiển thị lại cuộc trò chuyện
            new count = ConversationCache[cacheid][ConvMessageCount];
            if (count < MAX_CONV_MESSAGES)
                ConversationCache[cacheid][ConvMessageCount] = count + 1;

            return ShowConversationDialog(playerid, convid);
        }

        case DIALOG_NEW_MESSAGE:
        {
            if (!response)
                return 1;

            if (!inputtext[0])
                return 1;

            // Tìm player theo tên hoặc ID
            new targetPlayerID = -1;
            new idx = 0;

            if (sscanf(inputtext, "u", targetPlayerID) == 0 && IsPlayerConnected(targetPlayerID))
            {
                // Thành công
            }
            else
            {
                for (new i = 0; i < MAX_PLAYERS; i++)
                {
                    if (IsPlayerConnected(i))
                    {
                        new playerName[MAX_PLAYER_NAME];
                        GetPlayerName(i, playerName, MAX_PLAYER_NAME);

                        if (!strcmp(playerName, inputtext, true))
                        {
                            targetPlayerID = i;
                            break;
                        }
                    }
                }
            }

            if (targetPlayerID == -1 || targetPlayerID == playerid)
            {
                SendClientMessage(playerid, 0xFF0000FF, "ERROR: Không tìm thấy người chơi!");
                return 1;
            }

            new convid;
            GetOrCreateConversation(
                PlayerInfo[playerid][pID],
                PlayerInfo[targetPlayerID][pID],
                convid
            );

            new cacheid = CreateConversationCache(
                              convid,
                              PlayerInfo[playerid][pID],
                              PlayerInfo[targetPlayerID][pID]
                          );

            LoadConversation(convid);

            return ShowConversationDialog(playerid, convid);
        }
    }

    return 1;
}

forward OnConversationSelected(playerid, listitem);
public OnConversationSelected(playerid, listitem)
{
    new rows;
    cache_get_row_count(rows);

    if (rows == 0)
        return 1;

    new convid;
    cache_get_value_name_int(0, "ID", convid);

    new cacheid = FindConversationCache(convid);

    if (cacheid == -1)
    {
        cacheid = CreateConversationCache(convid, 0, 0);
        LoadConversation(convid);
    }

    return ShowConversationDialog(playerid, convid);
}



// ============================================================================
// Messenger Commands - Hệ thống nhắn tin
// ============================================================================

// Lệnh /tin - Mở danh sách tin nhắn
CMD:tin(playerid, params[])
{
    if (isnull(params))
    {
        ShowConversationList(playerid);
    }
    else
    {
        // /tin [tên người chơi] - Gửi tin mới
        new targetPlayerID = -1;

        if (sscanf(params, "u", targetPlayerID) == 0 && IsPlayerConnected(targetPlayerID))
        {
            // OK
        }
        else
        {
            for (new i = 0; i < MAX_PLAYERS; i++)
            {
                if (IsPlayerConnected(i))
                {
                    new playerName[MAX_PLAYER_NAME];
                    GetPlayerName(i, playerName, MAX_PLAYER_NAME);

                    if (!strcmp(playerName, params, true))
                    {
                        targetPlayerID = i;
                        break;
                    }
                }
            }
        }

        if (targetPlayerID == -1 || targetPlayerID == playerid)
        {
            SendClientMessage(playerid, 0xFF0000FF, "ERROR: Khong tim thay nguoi choi!");
            return 1;
        }

        if (PlayerInfo[playerid][pID] == 0 || PlayerInfo[targetPlayerID][pID] == 0)
        {
            SendClientMessage(playerid, 0xFF0000FF, "ERROR: Nguoi choi chua duoc tai thong tin!");
            return 1;
        }

        new convid;
        GetOrCreateConversation(
            PlayerInfo[playerid][pID],
            PlayerInfo[targetPlayerID][pID],
            convid
        );

        new cacheid = FindConversationCache(convid);

        if (cacheid == -1)
        {
            cacheid = CreateConversationCache(
                          convid,
                          PlayerInfo[playerid][pID],
                          PlayerInfo[targetPlayerID][pID]
                      );
            LoadConversation(convid);
        }

        ShowConversationDialog(playerid, convid);
    }

    return 1;
}

// Lệnh /message [playerid] - Gửi tin nhắn nhanh
CMD:msg(playerid, params[]) return cmd_message(playerid, params);
CMD:message(playerid, params[])
{
    new targetPlayerID, message[256];

    if (sscanf(params, "us[256]", targetPlayerID, message) == -1)
    {
        SendClientMessage(playerid, 0xFFFF00FF, "Usage: /message [playerid] [text]");
        return 1;
    }

    if (!IsPlayerConnected(targetPlayerID))
    {
        SendClientMessage(playerid, 0xFF0000FF, "ERROR: Nguoi choi khong ton tai!");
        return 1;
    }

    if (targetPlayerID == playerid)
    {
        SendClientMessage(playerid, 0xFF0000FF, "ERROR: Khong the gui cho chinh minh!");
        return 1;
    }

    if (PlayerInfo[playerid][pID] == 0 || PlayerInfo[targetPlayerID][pID] == 0)
    {
        SendClientMessage(playerid, 0xFF0000FF, "ERROR: Thong tin nguoi choi chua duoc tai!");
        return 1;
    }

    new convid;
    GetOrCreateConversation(
        PlayerInfo[playerid][pID],
        PlayerInfo[targetPlayerID][pID],
        convid
    );

    new cacheid = FindConversationCache(convid);

    if (cacheid == -1)
    {
        cacheid = CreateConversationCache(
                      convid,
                      PlayerInfo[playerid][pID],
                      PlayerInfo[targetPlayerID][pID]
                  );
    }

    SendConversationMessage(playerid, convid, message);

    // SendClientNotification(playerid, "Messager", "Tin nhan da duoc gui !", NOTIFY_TYPE_SUCCESS);

    new targetName[MAX_PLAYER_NAME];
    GetPlayerName(targetPlayerID, targetName, MAX_PLAYER_NAME);

    new senderName[MAX_PLAYER_NAME];
    GetPlayerName(playerid, senderName, MAX_PLAYER_NAME);
    new string[128];
    format(string, sizeof(string), "Ban da gui tin nhan cho %s: %s", targetName, message);
    SendClientMessage(playerid, 0x00FF00FF, string);
    format(string, sizeof(string), "Ban nhan duoc tin nhan tu %s: %s", senderName, message);
    SendClientMessage(
        targetPlayerID,
        0x00CCFFFF,
        string
    );

    return 1;
}

// Lệnh /inbox - Xem hộp thư đến
CMD:inbox(playerid, params[])
{
    ShowConversationList(playerid);
    return 1;
}

// Lệnh /delconv [id] - Xóa cuộc trò chuyện (nếu cần)
CMD:delconv(playerid, params[])
{
    new convid;

    if (sscanf(params, "i", convid) == -1)
    {
        SendClientMessage(playerid, 0xFFFF00FF, "usage: /delconv [conversation_id]");
        return 1;
    }

    new query[256];
    mysql_format(
        g_DatabaseHandle,
        query,
        sizeof(query),
        "DELETE FROM Messages WHERE ConversationID = %d",
        convid
    );

    mysql_tquery(g_DatabaseHandle, query);

    mysql_format(
        g_DatabaseHandle,
        query,
        sizeof(query),
        "DELETE FROM Conversations WHERE ID = %d",
        convid
    );

    mysql_tquery(g_DatabaseHandle, query);

    SendClientMessage(playerid, 0x00FF00FF, "Da xao cuoc tro chuyen.");

    return 1;
}