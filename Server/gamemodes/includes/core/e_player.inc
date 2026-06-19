#include <YSI_Coding\y_hooks>

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
    Location\t%s\t(%0.2f, %0.2f, %0.2f)\n\n\
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
    return level * level * 65;
}

stock GivePlayerExp(playerid, const title[], amount = 5)
{
    if(!IsPlayerConnected(playerid))
        return 0;

    PlayerInfo[playerid][pExp] += amount;

    new string[128];
    format(string, sizeof(string),
        "[EXP] +%d Exp", amount);
    SendClientNotification(playerid, title, string, NOTIFY_TYPE_SUCCESS, 3000);

    CheckPlayerLevel(playerid);
    return 1;
}

stock CheckPlayerLevel(playerid)
{
    if(PlayerInfo[playerid][pExp] >= GetNextLevelExp(playerid))
    {
        PlayerInfo[playerid][pLevel]++;
        SetPlayerScore(playerid, PlayerInfo[playerid][pLevel]);

        new level = PlayerInfo[playerid][pLevel];

        new string[144];
        format(string, sizeof(string),
            "Chuc mung! ban da len level %d! (Thuong $%d)",
            level, level * 1000);
        SendClientNotification(playerid, "LEVEL UP", string, NOTIFY_TYPE_SUCCESS, 3000);        

        GivePlayerCash(playerid, level * 1000);
    }
    return 1;
}

// duoc chay khi player spawn khi vao game
forward PlaytimeTimer(playerid);
public PlaytimeTimer(playerid)
{
    PlayerInfo[playerid][pPlaytime]++;
    gTimeOnline[playerid]++;
    if(gTimeOnline[playerid] >= 60) {
        gTimeOnline[playerid] = 0;
        GivePlayerExp(playerid, "Thuong Online", 10); // Thuong online 1h (+10Exp)
    }
    return 1;
}

stock GetPlaytimeString(playerid, dest[], size)
{
    new hours = PlayerInfo[playerid][pPlaytime] / 60;
    new mins  = PlayerInfo[playerid][pPlaytime] % 60;

    if(hours < 1) {
        format(dest, size, "%dm", mins);
        return 1;
    }
    format(dest, size, "%dh %dm", hours, mins);
    return 1;
}