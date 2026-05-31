#define ACCOUNT_MIN_PASSWORD_LENGTH (6)

//-----------------------------------------------------------------------------
// Embedded Colors
//-----------------------------------------------------------------------------

#define EMBED_WHITE "{FFFFFF}"
#define EMBED_RED   "{FFDE97}"


//-----------------------------------------------------------------------------
// Enums
//-----------------------------------------------------------------------------
enum
{
	DIALOG_NOTHING,
    DIALOG_REGISTRATION,
    DIALOG_LOGIN
};

//-----------------------------------------------------------------------------
// Variables
//-----------------------------------------------------------------------------
new MySQL:g_DatabaseHandle;


//-----------------------------------------------------------------------------
// Functions
//-----------------------------------------------------------------------------
MySQL_Init() {
	g_DatabaseHandle = mysql_connect_file("mysql.ini");

    if (mysql_errno(g_DatabaseHandle) == 0) 
    {
        print("-----------------------------------------------");
        print("Successfully connected to database!");
        print("-----------------------------------------------");
    }
    else 
    {
        print("-----------------------------------------------");
        print("Failed to connect to database!");
        print("Please verify your mysql.ini settings");
        print("Server will be locked for maintenance...");
        print("-----------------------------------------------");

        SendRconCommand("password TvHRY2FmQjXEsCq");
        SendRconCommand("name Server is under maintenance!");
    }
	return 1;
}

MySQL_Close() {
	mysql_close(g_DatabaseHandle);
	return 1;
}

public OnPlayerConnect(playerid)
{
	Account_Check(playerid);
    CreateTaiXiuTD(playerid);
	return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
	return 1;
}

public OnPlayerSpawn(playerid)
{
	SetPlayerInterior(playerid, 0);
	return 1;
}

public OnPlayerDeath(playerid, killerid, WEAPON:reason)
{
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
        case DIALOG_REGISTRATION:
        {
            if (!response)
            {
                Kick(playerid);
            }
            else
            {
                // Check if the password meets the requirements.
                if (!IsValidPassword(inputtext))
                {
                    Account_ShowRegistrationDialog(playerid, .badpass = true);   
                }
                else
                {
                    // Password looks good.  Now hash the password.
                    HashPassword(playerid, inputtext);
                }
            }

            // Return 1 to indicate the dialog was handled. 
            return 1;
        }

        case DIALOG_LOGIN:
        {
            if (!response)
            {
                Kick(playerid);
            }
            else
            {
                // Get the temporarily stored hash.
                new tempHash[BCRYPT_HASH_LENGTH];
                GetPVarString(playerid, "tempPassword", tempHash);

                // Compare hashes.
                bcrypt_verify(playerid, "OnPasswordCheck", inputtext, tempHash);
            }

            // Return 1 to indicate the dialog was handled. 
            return 1;  
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
    TaiXiu_HandleClick(playerid, playertextid);
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
