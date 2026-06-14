CMD:addfriend(playerid, params[]) {
    new targetId;
    if(sscanf(params, "u", targetId)) return SendClientMessageEx(playerid, COLOR_LIGHTGREY, "Su dung: /addfriend (ID Player)");
    if(!IsPlayerConnected(targetId)) return SendClientMessageEx(playerid, COLOR_WARNING, "** ID khong hop le !");
    
    RequestFriend(playerid, targetId);
    return 1;
}

CMD:friends(playerid, params[]) {
    ListFriends(playerid);
    return 1;
}