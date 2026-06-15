CMD:ooc(playerid, params[])
{
	if(gLoggedIn[playerid]) return 1;
    
	if(isnull(params)) return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: (/ooc) [ooc chat]");

    new string[128];
    format(string, sizeof(string), "(( %s: %s ))", GetPlayerNameEx(playerid), params);
    OOCOff(COLOR_OOC,string);
	return 1;
}

CMD:shout(playerid, params[]) {
	return cmd_s(playerid, params);
}
CMD:s(playerid, params[])
{
	if(gLoggedIn[playerid]) return 1;

	if(isnull(params)) return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: (/s)hout [shout chat]");
	new string[128];
	format(string, sizeof(string), "(het to) %s!", params);
	SetPlayerChatBubble(playerid,string,COLOR_WHITE,60.0,5000);
	format(string, sizeof(string), "%s het to: %s!", GetPlayerNameEx(playerid), params);
	ProxDetector(30.0, playerid, string,COLOR_WHITE,COLOR_WHITE,COLOR_WHITE,COLOR_FADE1,COLOR_FADE2);
	return 1;
}

CMD:low(playerid, params[]) {
    return cmd_l(playerid, params);
}
CMD:l(playerid, params[])
{
	if(gLoggedIn[playerid]) return 1;

	if(isnull(params)) return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: (/l)ow [close chat]");

	new string[128];
	format(string, sizeof(string), "%s noi nho: %s", GetPlayerNameEx(playerid), params);
	ProxDetector(5.0, playerid, string,COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5);
	format(string, sizeof(string), "(noi nho) %s", params);
	SetPlayerChatBubble(playerid,string,COLOR_WHITE,5.0,5000);
	return 1;
}

CMD:b(playerid, params[])
{
	if(gLoggedIn[playerid]) return 1;
	if(isnull(params)) return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /b [Chat ooc]");
	new string[128];
	format(string, sizeof(string), "%s: (( %s ))", GetPlayerNameEx(playerid), params);
	ProxDetector(20.0, playerid, string,COLOR_FADE1,COLOR_FADE2,COLOR_FADE3,COLOR_FADE4,COLOR_FADE5);
	return 1;
}

CMD:friends(playerid, params[]) {
    ListFriends(playerid);
    return 1;
}
CMD:friendlist(playerid, params[]) {
    new msg[128];
    for(new i; i < FriendCount[playerid]; i++) {
        format(msg, sizeof(msg), "[%d] Friend: %d | Status: %d | Sender: %d", i, PlayerFriends[playerid][i][ID], PlayerFriends[playerid][i][Status], PlayerFriends[playerid][i][Sender]);
        SendClientMessageEx(playerid, COLOR_GREEN, msg);
    }
    return 1;
}