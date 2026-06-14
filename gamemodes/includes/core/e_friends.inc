#include <YSI_Coding\y_hooks>

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_LIST_FRIENDS) {
        if(!response) return 1;
        if(listitem == 0) {
            ShowListReqFriends(playerid);
        }
        return 1;
    }
    if(dialogid == DIALOG_FRIEND_REQUEST) {
        if(GetPVarInt(playerid, "FriendClickedID")) {
            if(!response) {
                DeletePVar(playerid, "FriendClickedID");
                ShowListReqFriends(playerid);
                return 1;
            }
            switch(listitem) {
                case 0: {
                    AcceptFriend(playerid, GetPVarInt(playerid, "FriendClickedID"));
                    DeletePVar(playerid, "FriendClickedID");
                }
                    
                case 1: {
                    DenyFriend(playerid, GetPVarInt(playerid, "FriendClickedID"));
                    DeletePVar(playerid, "FriendClickedID");
                }
            }
            return 1;
        }

        if(!response) return 1;
        SetPVarInt(playerid, "FriendClickedID", gListIDReqFriends[playerid][listitem]);
        ShowPlayerDialog(playerid, DIALOG_FRIEND_REQUEST, DIALOG_STYLE_LIST, "Lua Chon", "Chap Nhan Ket Ban\nTu Choi", "Xac Nhan", "Quay Lai");
        return 1;
        
    }
    return 1;
}

stock ListFriends(playerid) {
    new status[52], listFriends[1024], line[128];
    new reqFriends = 0;
    for(new i; i < FriendCount[playerid]; i++) {
        if(PlayerFriends[playerid][i][ID] && PlayerFriends[playerid][i][Status] < 1 && PlayerFriends[playerid][i][Sender] != PlayerInfo[playerid][pID]) {
            reqFriends++;
        }
    }
    format(listFriends, sizeof(listFriends), "{FF0000}Yeu cau ket ban\t(%d)\n", reqFriends);

    for(new i; i < FriendCount[playerid]; i++) {
        if(PlayerFriends[playerid][i][ID] && PlayerFriends[playerid][i][Status] > 0) {
            format(status, sizeof(status), "Offline");
            foreach(new p: Player) {
                if(PlayerFriends[playerid][i][ID] == PlayerInfo[p][pID]) {
                    format(status, sizeof(status), "Online");
                }
            }
            format(line, sizeof(line), "%s\t%s\n", PlayerFriends[playerid][i][Name], status);
            strcat(listFriends, line);
        }
    }

    ShowPlayerDialog(playerid, DIALOG_LIST_FRIENDS, DIALOG_STYLE_TABLIST, "YOUR FRIENDS", listFriends, "Chon", "Thoat");
    return 1;
}

stock RequestFriend(playerid, targetID) {
    if(IsFriend(playerid, targetID)) return SendClientMessageEx(playerid, COLOR_WARNING, "** Hai ban da la ban be roi !");
    new msg[128];
    for(new i; i < MAX_FRIENDS; i++) {
        if(PlayerFriends[playerid][i][ID] == PlayerInfo[targetID][pID] && PlayerFriends[playerid][i][Status] <= 0 && PlayerFriends[playerid][i][Sender] == PlayerInfo[playerid][pID]) return SendClientMessageEx(playerid, COLOR_WARNING, "Dang cho doi phuong phan hoi !");
        if(PlayerFriends[playerid][i][ID] == PlayerInfo[targetID][pID] && PlayerFriends[playerid][i][Status] <= 0 && PlayerFriends[playerid][i][Sender] != PlayerInfo[playerid][pID]) return SendClientMessageEx(playerid, COLOR_WARNING, "Doi phuong dang cho ban phan hoi !");
    }

    for(new i; i < MAX_FRIENDS; i++) {
        if(!PlayerFriends[playerid][i][ID]) {
            PlayerFriends[playerid][i][ID] = PlayerInfo[targetID][pID];
            PlayerFriends[playerid][i][Name] = GetPlayerNameEx(targetID);
            PlayerFriends[playerid][i][Status] = 0;
            PlayerFriends[playerid][i][Sender] = PlayerInfo[playerid][pID];
            break;
        }
    }
    for(new i; i < MAX_FRIENDS; i++) {
        if(!PlayerFriends[targetID][i][ID]) {
            PlayerFriends[targetID][i][ID] = PlayerInfo[playerid][pID];
            PlayerFriends[targetID][i][Name] = GetPlayerNameEx(playerid);
            PlayerFriends[targetID][i][Status] = 0;
            PlayerFriends[targetID][i][Sender] = PlayerInfo[playerid][pID];
            break;
        }
    }

    format(msg, sizeof(msg), "%s Gui cho ban loi moi ket ban !", GetPlayerNameEx(playerid));
    SendClientNotification(targetID, "Friend Request", msg);

    format(msg, sizeof(msg), "Da gui loi moi ket ban toi %s", GetPlayerNameEx(targetID));
    SendClientNotification(playerid, "Friend Request", msg);

    FriendCount[playerid]++;
    FriendCount[targetID]++;
    return 1;
}

stock ShowListReqFriends(playerid) {
    new listReqFriends[1024], count = 0;
    for(new i; i < MAX_FRIENDS; i++) {
        if(PlayerFriends[playerid][i][ID] && PlayerFriends[playerid][i][Status] < 1 && PlayerFriends[playerid][i][Sender] != PlayerInfo[playerid][pID]) {
            format(listReqFriends, sizeof(listReqFriends), "%s%s\n", listReqFriends, PlayerFriends[playerid][i][Name]);
            count ++;
            gListIDReqFriends[playerid][count - 1] = PlayerFriends[playerid][i][ID];
        }
    }

    if(count <= 0) return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "REQUEST FRIENDS", "Khong co yeu cau ket ban nao !", "Dong", "");

    ShowPlayerDialog(playerid, DIALOG_FRIEND_REQUEST, DIALOG_STYLE_TABLIST, "REQUEST FRIENDS", listReqFriends, "Chon", "Thoat");
    return 1;
}

stock AcceptFriend(playerid, targetid) {
    for(new i; i < MAX_FRIENDS; i ++) {
        if(PlayerFriends[playerid][i][ID] == targetid) {
            PlayerFriends[playerid][i][Status] = 1;
            FriendCount[playerid] ++;
            break;
        }
    }
    foreach(new p: Player) {
        for(new i; i < MAX_FRIENDS; i ++) {
            if(PlayerFriends[p][i][ID] == GetPlayerSQLId(playerid)) {
                PlayerFriends[p][i][Status] = 1;
                FriendCount[p] ++;
                return 1;
            }
        }
    }
    return 1;
}
stock DenyFriend(playerid, targetid) {
    for(new i; i < MAX_FRIENDS; i ++) {
        if(PlayerFriends[playerid][i][ID] == targetid) {
            PlayerFriends[playerid][i][Status] = 0;
            PlayerFriends[playerid][i][ID] = 0;
            FriendCount[playerid] --;
            break;
        }
    }
    foreach(new p: Player) {
        for(new i; i < MAX_FRIENDS; i ++) {
            if(PlayerFriends[p][i][ID] == GetPlayerSQLId(playerid)) {
                PlayerFriends[p][i][Status] = 0;
                PlayerFriends[p][i][ID] = 0;
                FriendCount[p] --;
                return 1;
            }
        }
    }
    return 1;
}

stock IsFriend(playerid, friendid)
{
    new accountid = GetPlayerSQLId(friendid);

    for(new i; i < FriendCount[playerid]; i++)
    {
        if(PlayerFriends[playerid][i][ID] == accountid && PlayerFriends[playerid][i][Status] > 0) return 1;
    }

    return 0;
}