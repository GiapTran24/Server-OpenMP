public OnPlayerConnect(playerid)
{
	Account_Check(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    Account_Save(playerid);
    ClearNotificationSlot(playerid, 0);
    ClearNotificationSlot(playerid, 1);
	SetAccountOnline(playerid, 0);
	return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    if(!PlayerInfo[playerid][pLogged]) return 0;
    return 1;
}

public OnPlayerSpawn(playerid)
{
	if(!PlayerInfo[playerid][pLogged]) return 0;
    if(gLoggedIn[playerid]) {
        new string[128];
        if(PlayerInfo[playerid][pAdmin] > 0) {
            format(string, sizeof(string), "[AD] Ban dang dang nhap voi tai khoan {FF0000}%s", GetAdminName(PlayerInfo[playerid][pAdmin]));
        }
        SendClientMessageEx(playerid, COLOR_SUCCESS, string);

		LoadPlayerFriends(playerid);
        SetTimerEx("PlaytimeTimer", 60000, true, "d", playerid);
        SetTimerEx("NotifyPlayerSpawn", 1000, false, "d", playerid);
        gLoggedIn[playerid] = false;
    }
	return 1;
}
forward NotifyPlayerSpawn(playerid);
public NotifyPlayerSpawn(playerid)
{
    SendClientNotification(playerid, "HE THONG", "Chao mung ban den voi may chu!", NOTIFY_TYPE_SUCCESS, 5000);
    return 1;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
	SpawnPlayer(playerid);
    SetPlayerHealth(playerid, 100);
    if(gAdminGod[playerid]) {
        SpawnPlayer(playerid);
        SetPlayerHealth(playerid, 100);
        return 0;
    }
	return 0;
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	return 1;
}

public OnPlayerExitVehicle(playerid, vehicleid)
{
	return 1;
}

public OnVehicleSpawn(vehicleid)
{
	return 1;
}

public OnVehicleDeath(vehicleid, killerid)
{
	return 1;
}


public OnPlayerText(playerid, text[])
{
	if(gLoggedIn[playerid]) return 0;

	if(strfind(text, "|", true) != -1) {
	    SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong the su dung '|' nhan vat trong van ban.");
		return 0;
	}

	new Float: f_playerPos[3], sendername[MAX_PLAYER_NAME], string[255];
	sendername = GetPlayerNameEx(playerid);
	GetPlayerPos(playerid, f_playerPos[0], f_playerPos[1], f_playerPos[2]);

	foreach(new i: Player)
	{
		if(GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid)) {
			
			if(IsPlayerInRangeOfPoint(i, 20.0 / 16, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				format(string, sizeof(string), "%s noi: %s", sendername, text);
				SendClientMessageEx(i, COLOR_FADE1, string);
			}
			else if(IsPlayerInRangeOfPoint(i, 20.0 / 8, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				format(string, sizeof(string), "%s noi: %s", sendername, text);
				SendClientMessageEx(i, COLOR_FADE2, string);
			}
			else if(IsPlayerInRangeOfPoint(i, 20.0 / 4, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				format(string, sizeof(string), "%s noi: %s", sendername, text);
				SendClientMessageEx(i, COLOR_FADE3, string);
			}
			else if(IsPlayerInRangeOfPoint(i, 20.0 / 2, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				format(string, sizeof(string), "%s noi: %s", sendername, text);
				SendClientMessageEx(i, COLOR_FADE4, string);
			}
			else if(IsPlayerInRangeOfPoint(i, 20.0, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				format(string, sizeof(string), "%s noi: %s", sendername, text);
				SendClientMessageEx(i, COLOR_FADE5, string);
			}
		}
	}

	SetPlayerChatBubble(playerid, text, COLOR_WHITE, 20.0, 5000);
	return 0;
}

public OnPlayerUpdate(playerid)
{
	if(PlayerInfo[playerid][pLogged] == 1) {
		if(GetPlayerMoney(playerid) != PlayerInfo[playerid][pCash])
		{
			ResetPlayerMoney(playerid);
			GivePlayerMoney(playerid, PlayerInfo[playerid][pCash]);
		}
	}
	return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
	if(newkeys && KEY_YES) {
		if(GetPlayerTargetPlayer(playerid) != INVALID_PLAYER_ID && ProxDetectorS(3.0, playerid, GetPlayerTargetPlayer(playerid)) && !IsPlayerNPC(GetPlayerTargetPlayer(playerid))) {
			new title[MAX_PLAYER_NAME + 52];
			InteractTarget[playerid] = GetPlayerTargetPlayer(playerid);

			if(IsFriend(playerid, GetPlayerTargetPlayer(playerid))) {
				format(title, sizeof(title), "%s (Your Friend)", GetPlayerNameEx(GetPlayerTargetPlayer(playerid)));
				ShowPlayerDialog(playerid, DIALOG_CLICK_PLAYER, DIALOG_STYLE_LIST, title, "Pay", "Chon", "Thoat");
			} else {
				GetPlayerName(GetPlayerTargetPlayer(playerid), title, sizeof(title));
				ShowPlayerDialog(playerid, DIALOG_CLICK_PLAYER, DIALOG_STYLE_LIST, title, "Pay\nKet Ban", "Chon", "Thoat");
			}
		}
	}
    return 1;
}

public OnPlayerStateChange(playerid, PLAYER_STATE:newstate, PLAYER_STATE:oldstate)
{
	return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveCheckpoint(playerid)
{
	return 1;
}

public OnPlayerEnterRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerLeaveRaceCheckpoint(playerid)
{
	return 1;
}

public OnPlayerGiveDamageActor(playerid, damaged_actorid, Float:amount, WEAPON:weaponid, bodypart)
{
	return 1;
}

public OnActorStreamIn(actorid, forplayerid)
{
	return 1;
}

public OnActorStreamOut(actorid, forplayerid)
{
	return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
	switch (dialogid)
    {
        case DIALOG_REGISTER:
        {
            if(!response) return Kick(playerid);

			bcrypt_hash(playerid, "OnPasswordHash", inputtext, BCRYPT_COST);
			return 1;
        }

        case DIALOG_LOGIN:
        {
            if(!response) return Kick(playerid);

			new query[256];

			mysql_format(g_DatabaseHandle, query, sizeof(query),
				"SELECT Password FROM Accounts \
				WHERE ID='%d'",
				PlayerInfo[playerid][pID]
			);

			mysql_tquery(g_DatabaseHandle, query, "OnPasswordCheck", "ds",
				playerid,
				inputtext
			);
            return 1;  
        }
        case DIALOG_GOTO:
        {
            if(!response) return 1;

            TeleportPlayerToLocation(playerid, listitem);
            return 1;
        }
        case DIALOG_SETADMIN: {
            if(!response) return 1;

            new targetid = SetAdminTarget[playerid];

            if(!IsPlayerConnected(targetid))
                return SendClientMessageEx(playerid, COLOR_GREY,
                    "Nguoi choi da thoat.");

            PlayerInfo[targetid][pAdmin] =
                AdminLevels[listitem][adminLevel];

            new str[128];

            format(str, sizeof(str),
                "Ban da cap %s cho %s.",
                AdminLevels[listitem][adminName],
                GetPlayerNameEx(targetid));

            SendClientMessageEx(playerid, COLOR_GREEN, str);

            format(str, sizeof(str),
                "%s da cap cho ban quyen %s.",
                GetPlayerNameEx(playerid),
                AdminLevels[listitem][adminName]);

            SendClientMessageEx(targetid, COLOR_GREEN, str);

        }

		case DIALOG_CLICK_PLAYER: {
			if(!response) {
				InteractTarget[playerid] = INVALID_PLAYER_ID;
				return 1;
			}
			switch(listitem) {
				case 0: {
					ShowPlayerDialog(playerid, DIALOG_GIVE_MONEY, DIALOG_STYLE_INPUT, "Pay", "Nhap so tien !", "Gui", "Thoat");
				}
				case 1: {
					RequestFriend(playerid, InteractTarget[playerid]);
					InteractTarget[playerid] = INVALID_PLAYER_ID;
				}
			}
		}
    }
	return 1;
}

public OnPlayerEnterGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerLeaveGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerEnterPlayerGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerLeavePlayerGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerClickGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerClickPlayerGangZone(playerid, zoneid)
{
	return 1;
}

public OnPlayerSelectedMenuRow(playerid, row)
{
	return 1;
}

public OnPlayerExitedMenu(playerid)
{
	return 1;
}

public OnClientCheckResponse(playerid, actionid, memaddr, retndata)
{
	return 1;
}

public OnRconLoginAttempt(ip[], password[], success)
{
	return 1;
}

public OnPlayerFinishedDownloading(playerid, virtualworld)
{
	return 1;
}

public OnPlayerRequestDownload(playerid, DOWNLOAD_REQUEST:type, crc)
{
	return 1;
}

public OnRconCommand(cmd[])
{
	return 0;
}

public OnPlayerSelectObject(playerid, SELECT_OBJECT:type, objectid, modelid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}

public OnPlayerEditObject(playerid, playerobject, objectid, EDIT_RESPONSE:response, Float:fX, Float:fY, Float:fZ, Float:fRotX, Float:fRotY, Float:fRotZ)
{
	return 1;
}

public OnPlayerEditAttachedObject(playerid, EDIT_RESPONSE:response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
	return 1;
}

public OnObjectMoved(objectid)
{
	return 1;
}

public OnPlayerObjectMoved(playerid, objectid)
{
	return 1;
}

public OnPlayerPickUpPickup(playerid, pickupid)
{
	return 1;
}

public OnPlayerPickUpPlayerPickup(playerid, pickupid)
{
	return 1;
}

public OnPickupStreamIn(pickupid, playerid)
{
	return 1;
}

public OnPickupStreamOut(pickupid, playerid)
{
	return 1;
}

public OnPlayerPickupStreamIn(pickupid, playerid)
{
	return 1;
}

public OnPlayerPickupStreamOut(pickupid, playerid)
{
	return 1;
}

public OnPlayerStreamIn(playerid, forplayerid)
{
	return 1;
}

public OnPlayerStreamOut(playerid, forplayerid)
{
	return 1;
}

public OnPlayerTakeDamage(playerid, issuerid, Float:amount, WEAPON:weaponid, bodypart)
{
    if(gAdminGod[playerid]) return 0;
    
	return 1;
}

public OnPlayerGiveDamage(playerid, damagedid, Float:amount, WEAPON:weaponid, bodypart)
{
	return 1;
}

public OnPlayerClickPlayer(playerid, clickedplayerid, CLICK_SOURCE:source)
{
	if(clickedplayerid == INVALID_PLAYER_ID) return 1;
	return 1;
}

public OnPlayerWeaponShot(playerid, WEAPON:weaponid, BULLET_HIT_TYPE:hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    return 1;
}

public OnPlayerClickMap(playerid, Float:fX, Float:fY, Float:fZ)
{
	return 1;
}

public OnIncomingConnection(playerid, ip_address[], port)
{
	return 1;
}

public OnPlayerInteriorChange(playerid, newinteriorid, oldinteriorid)
{
	return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
	return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
	return 1;
}

public OnTrailerUpdate(playerid, vehicleid)
{
	return 1;
}

public OnVehicleSirenStateChange(playerid, vehicleid, newstate)
{
	return 1;
}

public OnVehicleStreamIn(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleStreamOut(vehicleid, forplayerid)
{
	return 1;
}

public OnVehicleMod(playerid, vehicleid, componentid)
{
	return 1;
}

public OnEnterExitModShop(playerid, enterexit, interiorid)
{
	return 1;
}

public OnVehiclePaintjob(playerid, vehicleid, paintjobid)
{
	return 1;
}

public OnVehicleRespray(playerid, vehicleid, color1, color2)
{
	return 1;
}

public OnVehicleDamageStatusUpdate(vehicleid, playerid)
{
	return 1;
}

public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
	return 1;
}

public OnPlayerCommandPerformed(playerid, cmdtext[], success)
{
    if(!success)  {
		new string[128];
		format(string, sizeof(string), "Lenh %s khong ton tai !", cmdtext);
		return SendClientNotification(playerid, "He Thong", string, NOTIFY_TYPE_ERROR, 3000);
	} 
    return 1;
}

//-----------------------------------------------------------------------------
// Functions Support
//-----------------------------------------------------------------------------

stock GetPlayerNameEx(playerid) {

	new
		szName[MAX_PLAYER_NAME],
		iPos;

	GetPlayerName(playerid, szName, MAX_PLAYER_NAME);
	while ((iPos = strfind(szName, "_", false, iPos)) != -1) szName[iPos] = ' ';
	return szName;
}

stock GetPlayerSQLId(playerid) {
	return PlayerInfo[playerid][pID];
}

stock GetplayerSQLName(sqlid)
{
    new query[128];
	new name[MAX_PLAYER_NAME];

    mysql_format(g_DatabaseHandle, query, sizeof(query),
        "SELECT Username FROM Accounts WHERE ID = %d LIMIT 1",
        sqlid
    );

    mysql_query(g_DatabaseHandle, query);

    if(cache_num_rows())
    {
        cache_get_value_name(0, "Username", name, sizeof(name));
        return name;
    }
	name = "Unknown";
	return name;
}

stock GetPlayerCash(playerid)
{
    return PlayerInfo[playerid][pCash];
}

stock GivePlayerCash(playerid, amount)
{
    PlayerInfo[playerid][pCash] += amount;
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

ProxDetector(Float: f_Radius, playerid, const string[],col1,col2,col3,col4,col5)
{
	new
		Float: f_playerPos[3];

	GetPlayerPos(playerid, f_playerPos[0], f_playerPos[1], f_playerPos[2]);
	foreach(new i: Player)
	{
		if(GetPlayerVirtualWorld(i) == GetPlayerVirtualWorld(playerid)) {
			if(IsPlayerInRangeOfPoint(i, f_Radius / 16, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				SendClientMessageEx(i, col1, string);
			}
			else if(IsPlayerInRangeOfPoint(i, f_Radius / 8, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				SendClientMessageEx(i, col2, string);
			}
			else if(IsPlayerInRangeOfPoint(i, f_Radius / 4, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				SendClientMessageEx(i, col3, string);
			}
			else if(IsPlayerInRangeOfPoint(i, f_Radius / 2, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				SendClientMessageEx(i, col4, string);
			}
			else if(IsPlayerInRangeOfPoint(i, f_Radius, f_playerPos[0], f_playerPos[1], f_playerPos[2])) {
				SendClientMessageEx(i, col5, string);
			}
		}
	}
	return 1;
}

ProxDetectorS(Float:radi, playerid, targetid)
{
	new
		Float: fp_playerPos[3];

	GetPlayerPos(targetid, fp_playerPos[0], fp_playerPos[1], fp_playerPos[2]);

	if(IsPlayerInRangeOfPoint(playerid, radi, fp_playerPos[0], fp_playerPos[1], fp_playerPos[2]) && GetPlayerVirtualWorld(playerid) == GetPlayerVirtualWorld(targetid))
	{
		return 1;
	}
	return 0;
}

stock OOCOff(color, const string[])
{
	foreach(new i: Player)
	{
		if(!gOoc[i]) {
			SendClientMessageEx(i, color, string);
		}
	}
}