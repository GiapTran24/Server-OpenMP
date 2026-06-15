stock ShowStats(playerid, targetid) {
    new string[1024];
    new Float:px, Float:py, Float:pz;
    GetPlayerPos(targetid, px, py, pz);

    new zone[MAX_ZONE_NAME];
    GetPlayer3DZone(targetid, zone, sizeof(zone));

    new playtimestr[62];
    GetPlaytimeString(targetid, playtimestr, sizeof(playtimestr));

    format(string, sizeof(string),
    "\n\
    Name\t%s\n\
    Level\t%d\n\n\
    Cash\t$%d\n\
    Bank\t$%d\n\n\
    Exp\t%d / %d\n\
    Playtime\t%s\n\n\
    Location\t%s\t(%0.2f, %0.2f, %0.2f)\n\
    \n",
    GetPlayerNameEx(targetid),
    PlayerInfo[targetid][pLevel],
    PlayerInfo[targetid][pCash],
    PlayerInfo[targetid][pBank],
    PlayerInfo[targetid][pExp],
    GetNextLevelExp(targetid),
    playtimestr,
    zone, px, py, pz
    );

    ShowPlayerDialog(playerid,
        DIALOG_NOTHING,
        DIALOG_STYLE_MSGBOX,
        "Thong Tin Nhan Vat",
        string,
        "Dong",
        "");
    return 1;
}

stock GetNextLevelExp(playerid)
{
    new level = PlayerInfo[playerid][pLevel];
    return level * level * 75;
}

stock GivePlayerExp(playerid, amount)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    PlayerInfo[playerid][pExp] += amount;

    new string[128];
    format(string, sizeof(string),
        "[EXP] +%d Exp", amount);
    SendClientNotification(playerid, "Completed", string, NOTIFY_TYPE_SUCCESS, 3000);

    CheckPlayerLevel(playerid);
    return 1;
}

stock CheckPlayerLevel(playerid)
{
    new level = PlayerInfo[playerid][pLevel];

    if(PlayerInfo[playerid][pExp] >= GetNextLevelExp(playerid))
    {
        PlayerInfo[playerid][pLevel]++;

        level = PlayerInfo[playerid][pLevel];

        new string[144];
        format(string, sizeof(string),
            "Chuc mung! ban da len level %d!",
            level);
        SendClientNotification(playerid, "LEVEL UP", string, NOTIFY_TYPE_SUCCESS, 3000);        

        GivePlayerCash(playerid, level * 1000);
        SetPlayerScore(playerid, level);
    }
    return 1;
}

stock PlaytimeTimer()
{
    foreach(new i : Player)
    {
        PlayerInfo[i][pPlaytime]++;
    }
    return 1;
}

stock GetPlaytimeString(playerid, dest[], size)
{
    new hours = PlayerInfo[playerid][pPlaytime] / 60;
    new mins  = PlayerInfo[playerid][pPlaytime] % 60;

    format(dest, size, "%dh %dm", hours, mins);
    return 1;
}