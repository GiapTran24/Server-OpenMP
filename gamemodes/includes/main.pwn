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
    if(gAdminGod[playerid]) {
        SpawnPlayer(playerid);
        SetPlayerHealth(playerid, 100);
        return 0;
    }
	return 1;
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
	return 1;
}

public OnPlayerUpdate(playerid)
{
	if(GetPlayerMoney(playerid) != PlayerInfo[playerid][pCash])
    {
        ResetPlayerMoney(playerid);
        GivePlayerMoney(playerid, PlayerInfo[playerid][pCash]);
    }

	return 1;
}

public OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
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
				WHERE Username='%e'",
				GetPlayerNameEx(playerid)
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