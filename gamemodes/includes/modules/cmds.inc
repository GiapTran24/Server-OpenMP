CMD:setadmin(playerid, params[])
{
    new targetid;

    if(sscanf(params, "u", targetid))
        return SendClientMessageEx(playerid, COLOR_GREY,
            "SU DUNG: /setadmin [player]");

    if(!IsPlayerConnected(targetid))
        return SendClientMessageEx(playerid, COLOR_GREY,
            "Nguoi choi khong ton tai.");

    SetAdminTarget[playerid] = targetid;

    new dialog[1024];

    for(new i; i < sizeof(AdminLevels); i++)
    {
        format(dialog, sizeof(dialog),
            "%s%s\n",
            dialog,
            AdminLevels[i][adminName]);
    }

    ShowPlayerDialog(playerid,
        DIALOG_SETADMIN,
        DIALOG_STYLE_LIST,
        "Chon Cap Admin",
        dialog,
        "Chon",
        "Dong");

    return 1;
}

CMD:goto(playerid, params[])
{
    new str[2048];
    for(new i; i < sizeof(GotoLocations); i++)
    {
        format(str, sizeof(str),
            "%s%s\n",
            str,
            GotoLocations[i][gotoName]);
    }

    ShowPlayerDialog(playerid,
        DIALOG_GOTO,
        DIALOG_STYLE_LIST,
        "Goto Locations",
        str,
        "Chon",
        "Dong");

    return 1;
}
CMD:gotoco(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 1)
        return SendClientMessage(playerid, COLOR_DANGER, "Ban khong du quyen.");

    new Float:x, Float:y, Float:z;
    new interior;

    if(sscanf(params, "p<,>fffi", x, y, z, interior)) return SendClientMessage(playerid, COLOR_INFO, "Su dung: /gotoco x,y,z,int");

    if(GetPlayerState(playerid) == PLAYER_STATE_DRIVER)
    {
        new vehicleid = GetPlayerVehicleID(playerid);

        SetVehiclePos(vehicleid, x, y, z);
        LinkVehicleToInterior(vehicleid, interior);
    }
    else
    {
        SetPlayerPos(playerid, x, y, z);
    }

    SetPlayerInterior(playerid, interior);
    PlayerInfo[playerid][pInterior] = interior;

    SetPlayerVirtualWorld(playerid, 0);
    PlayerInfo[playerid][pVirtualWorld] = 0;

    SendClientMessage(playerid, COLOR_SUCCESS,
        "Da dich chuyen thanh cong.");

    return 1;
}

CMD:veh(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 2)
        return SendClientMessage(playerid, COLOR_DANGER, "Ban khong du quyen.");

    new vehicleModel;
    new color1, color2;

    if(sscanf(params, "iii", vehicleModel, color1, color2))
    {
        SendClientMessage(playerid, COLOR_INFO,
            "Su dung: /veh [modelid] [color1] [color2]");
        SendClientMessage(playerid, COLOR_SECONDARY,
            "Delete: /delveh de xoa xe dang ngoi !");
        return 1;
    }

    if(vehicleModel < 400 || vehicleModel > 611)
    {
        return SendClientMessage(playerid, COLOR_DANGER,
            "Model xe phai tu 400 den 611.");
    }

    new Float:x, Float:y, Float:z, Float:a;

    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);

    new vehicleid = CreateVehicle(
        vehicleModel,
        x + 2.0,
        y,
        z,
        a,
        color1,
        color2,
        -1
    );

    PutPlayerInVehicle(playerid, vehicleid, 0);

    new str[128];
    format(str, sizeof(str),
        "Da tao xe ID %d (VehicleID: %d).",
        vehicleModel,
        vehicleid);

    SendClientMessage(playerid, COLOR_SUCCESS, str);
    return 1;
}
CMD:delveh(playerid, params[])
{
    if(!IsPlayerInAnyVehicle(playerid))
        return SendClientMessage(playerid, COLOR_DANGER,
            "Ban khong o trong xe.");

    new vehicleid = GetPlayerVehicleID(playerid);

    RemovePlayerFromVehicle(playerid);
    DestroyVehicle(vehicleid);

    SendClientMessage(playerid, COLOR_SUCCESS,
        "Da xoa xe.");
    return 1;
}

CMD:gotoid(playerid, params[])
{
	new giveplayerid;
	if(sscanf(params, "u", giveplayerid)) return SendClientMessageEx(playerid, COLOR_INFO, "SU DUNG: /gotoid [player]");

	new Float:plocx,Float:plocy,Float:plocz;
	if (IsPlayerConnected(giveplayerid))
	{
		if (PlayerInfo[playerid][pAdmin] >= 2)
		{
			if(GetPlayerState(giveplayerid) == PLAYER_STATE_SPECTATING)
			{
				SendClientMessageEx(playerid, COLOR_DANGER, "Nguoi do dang theo doi nguoi choi");
				return 1;
			}
			if(GetPlayerState(playerid) == PLAYER_STATE_SPECTATING)
			{
				SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong the lam dieu nay khi dang theo doi.");
				return 1;
			}
			GetPlayerPos(giveplayerid, plocx, plocy, plocz);
			SetPlayerVirtualWorld(playerid, PlayerInfo[giveplayerid][pVirtualWorld]);

            SetPlayerPos(playerid,plocx,plocy+2, plocz);
            SetPlayerInterior(playerid, GetPlayerInterior(giveplayerid));
            SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(giveplayerid));

			SendClientMessageEx(playerid, COLOR_SUCCESS, "   Ban da duoc dich chuyen!");
		}
		else
		{
			SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong duoc phep su dung lenh nay.");
		}

	}
	else SendClientMessageEx(playerid, COLOR_DANGER, "Nguoi choi khong hop le.");
	return 1;
}

CMD:givegun(playerid, params[])
{
    if (PlayerInfo[playerid][pAdmin] >= 1) {
        new sstring[128], playa, gun, ammo;

        if(sscanf(params, "udd", playa, gun, ammo)) {
            SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /givegun [player] [weaponid] [ammo]");
            SendClientMessageEx(playerid, COLOR_GREEN, "_______________________________________");
            SendClientMessageEx(playerid, COLOR_GREY, "(1)Brass Knuckles (2)Golf Club (3)Nite Stick (4)Knife (5)Baseball Bat (6)Shovel (7)Pool Cue (8)Katana (9)Chainsaw");
            SendClientMessageEx(playerid, COLOR_GREY, "(10)Purple Dildo (11)Small White Vibrator (12)Large White Vibrator (13)Silver Vibrator (14)Flowers (15)Cane (16)Frag Grenade");
            SendClientMessageEx(playerid, COLOR_GREY, "(17)Tear Gas (18)Molotov Cocktail (21)Jetpack (22)9mm (23)Silenced 9mm (24)Desert Eagle (25)Shotgun (26)Sawnoff Shotgun");
            SendClientMessageEx(playerid, COLOR_GREY, "(27)Combat Shotgun (28)Micro SMG (Mac 10) (29)SMG (MP5) (30)AK-47 (31)M4 (32)Tec9 (33)Rifle (34)Sniper Rifle");
            SendClientMessageEx(playerid, COLOR_GREY, "(35)Rocket Launcher (36)HS Rocket Launcher (37)Flamethrower (38)Minigun (39)Satchel Charge (40)Detonator");
            SendClientMessageEx(playerid, COLOR_GREY, "(41)Spraycan (42)Fire Extinguisher (43)Camera (44)Nightvision Goggles (45)Infared Goggles (46)Parachute");
            SendClientMessageEx(playerid, COLOR_GREEN, "_______________________________________");
            return 1;
        }

        format(sstring, sizeof(sstring), "Ban da cho %s gun ID %d!",GetPlayerNameEx(playa),gun);
        if(gun < 1||gun > 47)
            { SendClientMessageEx(playerid, COLOR_GREY, "ID vu khi khong hop le!"); return 1; }
        if(IsPlayerConnected(playa)) {
            if(playa != INVALID_PLAYER_ID && gun <= 20 || gun >= 22) {
                GivePlayerWeapon(playa, WEAPON:gun, ammo);
                SendClientMessageEx(playerid, COLOR_GREY, sstring);
            }
            else if(playa != INVALID_PLAYER_ID && gun == 21) {
                SetPlayerSpecialAction(playa, SPECIAL_ACTION_USEJETPACK);
                SendClientMessageEx(playerid, COLOR_GREY, sstring);
            }
        }
    }
    else {
        SendClientMessageEx(playerid, COLOR_GREY, "Ban khong duoc phep su dung lenh nay.");
    }
    return 1;
}

CMD:setskin(playerid, params[])
{
	if (PlayerInfo[playerid][pAdmin] >= 1)
	{
		new string[128], giveplayerid, skinid;
		if(sscanf(params, "ud", giveplayerid, skinid)) return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /setskin [player] [skinid]");

		if(IsPlayerConnected(giveplayerid))
		{
			if(!IsInvalidSkin(skinid))
			{
				if(GetPlayerSkin(giveplayerid) == skinid)
				{
					SendClientMessageEx( playerid, COLOR_WHITE, "The person you're trying to change skins of already is using the skin you're trying to set." );
				}
				else
				{
					PlayerInfo[giveplayerid][pSkin] = skinid;
					format(string, sizeof(string), "Your skin has been changed to ID %d by Administrator %s.", skinid, GetPlayerNameEx(playerid));
					SendClientMessageEx(giveplayerid, COLOR_WHITE, string);
					format(string, sizeof(string), "Ban da cho %s skin ID %d.", GetPlayerNameEx(giveplayerid), skinid);
					SendClientMessageEx(playerid, COLOR_WHITE, string);
                    SetPlayerSkin(giveplayerid, PlayerInfo[giveplayerid][pSkin]);
				}
			}
			else
			{
				SendClientMessageEx(playerid, COLOR_GREY, "Invalid skin ID!");
			}
		}
	}
	else
	{
		SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong duoc phep su dung lenh nay.");
	}
	return 1;
}

CMD:god(playerid, params[]) {
    if (PlayerInfo[playerid][pAdmin] >= 1) {
        gAdminGod[playerid] = !gAdminGod[playerid];
        SendClientMessageEx(playerid, COLOR_SUCCESS, gAdminGod[playerid] ? "[AD] God mode enabled." : "[AD] God mode disabled.");
    } else {
        SendClientMessageEx(playerid, COLOR_DANGER, "Ban khong duoc phep su dung lenh nay.");
    }
    return 1;
}

CMD:testtb(playerid, params[]) {
    new type;
    if(sscanf(params, "d", type)) return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /testtb [type]\n0: Success\n1: Error\n2: Warning\n3: Info");
    switch(type)
    {
        case 0: {
            ShowNotification(playerid,
                "Thong bao thanh cong!",
                "Day la mot thong bao mau xanh duoc hien thi trong 5 giay.",
                NOTIFY_TYPE_SUCCESS,
                5000
            );
        }
        case 1: {
            ShowNotification(playerid,
                "Thong bao loi!",
                "Day la mot thong bao mau do duoc hien thi trong 5 giay.",
                NOTIFY_TYPE_ERROR,
                5000
            );
        }
        case 2: {
            ShowNotification(playerid,
                "Thong bao canh bao!",
                "Day la mot thong bao mau vang duoc hien thi trong 5 giay.",
                NOTIFY_TYPE_WARNING,
                5000
            );
        }
        case 3: {
            ShowNotification(playerid,
                "Thong bao thong tin!",
                "Day la mot thong bao mau xanh duoc hien thi trong 5 giay.",
                NOTIFY_TYPE_INFO,
                5000
            );
        }
        default: return SendClientMessageEx(playerid, COLOR_GREY, "SU DUNG: /testtb [type]\n0: Success\n1: Error\n2: Warning\n3: Info");
    }
    
    return 1;
}