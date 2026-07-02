#include <YSI_Coding\y_hooks>
#include "./includes/minigames/cauca/fish_data.inc"
#include "./includes/minigames/cauca/fish_inv.inc"

#define DIALOG_GIVE_FISH 1000

enum e_fishing_state
{
    FISH_STATE_NONE,

    FISH_STATE_WAITING,

    FISH_STATE_BITE,

    FISH_STATE_MINIGAME
};

enum e_player_fishing
{
    bool:pFishing,

    e_fishing_state:pState,

    pFishID,
    Float:pFishWeight,
    Float:pProgress,
    pFishFight
};

new PlayerFishing[MAX_PLAYERS][e_player_fishing];

stock StartFishing(playerid)
{
    ResetFishingData(playerid);

    PlayerFishing[playerid][pFishing] = true;
    PlayerFishing[playerid][pState] = FISH_STATE_WAITING;

    ApplyAnimation(playerid,
        "SWORD",
        "sword_idle",
        4.0,
        true,
        false,
        false,
        true,
        0);

    SendClientMessage(playerid, -1,
        "Dang tha can...");
    SetPlayerAttachedObject(
        playerid,
        0,
        MODEL_FISHING_ROD,
        6,

        // Position
        0.08,
        0.02,
        0.00,

        // Rotation
        0.0,
        // 0.0,
        45.0,
        0.0,

        // Scale
        1.0,
        1.0,
        1.0
    );
    StartWaitingFish(playerid);
    return 1;
}

stock StopFishing(playerid)
{
    ClearAnimations(playerid);
    RemovePlayerAttachedObject(playerid, 0);

    ResetFishingData(playerid);
    ShowFishFightTD(playerid, false);
    return 1;
}

stock StartWaitingFish(playerid)
{
    new time = random(10000) + 21000;
    GameTextForPlayer(playerid, "~g~Dang tha cau ... !", 5000, 3);
    SetTimerEx("OnFishBite", time, false, "i", playerid);
    return 1;
}

forward OnFishBite(playerid);
public OnFishBite(playerid) {
    PlayerFishing[playerid][pState] = FISH_STATE_BITE;

    new rod_level = PlayerFishRod[playerid][pRodLevel];
    new fishid = GetRandomFish(rod_level);

    if(fishid == -1)
        return Fishing_OnFail(playerid);

    PlayerFishing[playerid][pFishID] = fishid;

    new name[32];
    GetFishName(fishid, name);

    new type[32];
    GetFishRarityName(fishid, type);

    new Float:weight = GetRandomFishWeight(fishid);
    PlayerFishing[playerid][pFishWeight] = weight;

    new string[128];
    format(string,
        sizeof(string),
        "[FISHING] %s da can cau. Bam SPACE de thu can !",
        name);
    SendClientMessage(playerid,
        -1,
        string);

    PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);

    UpdateFishFightTD(playerid, name, type);
    ShowFishFightTD(playerid, true);

    Fishing_StartMiniGame(playerid);
    return 1;
}

stock Fishing_StartMiniGame(playerid)
{
    PlayerFishing[playerid][pState] = FISH_STATE_MINIGAME;
    Fishing_UpdateProgressBar(playerid, 20);

    new time = random(300) + 1000;
    PlayerFishing[playerid][pFishFight] = SetTimerEx("FishingFight", time, true, "i", playerid);
    return 1;
}

forward FishingFight(playerid);
public FishingFight(playerid) {
    if(PlayerFishing[playerid][pState] == FISH_STATE_MINIGAME) {
        new fishid = PlayerFishing[playerid][pFishID];
        new pullPower = GetFishPullPower(fishid);
        Fishing_UpdateProgressBar(playerid, -pullPower);
        if(PlayerFishing[playerid][pProgress] <= 0) {
            Fishing_OnFail(playerid);
            PlayerPlaySound(playerid, 1069, 0.0, 0.0, 0.0);
        }
    }
    return 1;
}

stock Fishing_OnSuccess(playerid)
{
    new fishid = PlayerFishing[playerid][pFishID];
    GivePlayerFish(playerid, fishid, PlayerFishing[playerid][pFishWeight]);

    new name[32];
    GetFishName(fishid, name);

    new string[128];

    format(string,
        sizeof(string),
        "[FISHING] Ban da cau duoc {FF0000}%s!",
        name);

    SendClientMessage(playerid,
        -1,
        string);

    StopFishing(playerid);
    return 1;
}

stock Fishing_OnFail(playerid)
{
    SendClientMessage(playerid,
        -1,
        "[FISHING] Oh no! Con ca da bo chay!");

    StopFishing(playerid);
    return 1;
}

stock ResetFishingData(playerid)
{
    PlayerFishing[playerid][pFishing] = false;
    PlayerFishing[playerid][pState] = FISH_STATE_NONE;

    PlayerFishing[playerid][pFishID] = -1;
    PlayerFishing[playerid][pProgress] = 0;

    if(PlayerFishing[playerid][pFishFight]) {
        KillTimer(PlayerFishing[playerid][pFishFight]);
    }

    return 1;
}

stock Fishing_UpdateProgressBar(playerid, Float:value)
{
    PlayerFishing[playerid][pProgress] += value;
    new Float:progress = PlayerFishing[playerid][pProgress];

    if(progress <= 0) progress = 0;
    if(progress >= 100) progress = 100;

    UpdateProgressFishFight(playerid, progress);
    return 1;
}

stock Fishing_Press(playerid)
{
    new pPowerRod = PlayerFishRod[playerid][pRodLevel] * 3;
    Fishing_UpdateProgressBar(playerid, pPowerRod);

    if(PlayerFishing[playerid][pProgress] >= 100) return Fishing_OnSuccess(playerid);
    return 1;
}


CMD:cauca(playerid, params[])
{
    if(!StartFishing(playerid))
        return 1;

    return 1;
}

CMD:huycauca(playerid, params[])
{
    if(!PlayerFishing[playerid][pFishing]) return 1;

    StopFishing(playerid);
    SendClientMessage(playerid, COLOR_DANGER, "[FISHING] Ban da dung cau ca !");
    return 1;
}

CMD:tuica(playerid, params[]) {
    if(FishTdOpen[playerid]) {
        CloseFishingTD(playerid);
    } else {
        new rod_type[32], rodlevel, rodexp;
        GetFishRodInfo(playerid, rod_type, rodlevel, rodexp);
        UpdateFishEquip(playerid, rod_type, rodlevel, rodexp, GetExpRodRequired(rodlevel));

        new fish, modelid, fish_weight[11];
        for(new slot; slot < MAX_FISH_SLOTS; slot++) {
            if(PlayerFishInv[playerid][slot][pFishID] > -1) {
                fish = PlayerFishInv[playerid][slot][pFishID];
                modelid = FishData[fish][fishModelId];
                format(fish_weight, sizeof(fish_weight), "%0.1f~g~(kg)", PlayerFishInv[playerid][slot][pFishWeight]);
                UpdateFishGridSlot(playerid, slot, modelid, fish_weight);
            } else {
                UpdateFishGridSlot(playerid, slot, FISH_MODEL_DEFAULT, "_");
            }
        }
        OpenFishingTD(playerid);
    }
    return 1;
}

CMD:fishs(playerid, params[]) {
    ShowInfoAllFish(playerid);
    return 1;
}

CMD:givefish(playerid, params[]) {
    new listFish[2000];
    new fishname[32], fishtype[32];
    for(new fishId; fishId < sizeof(FishData); fishId++) {
        GetFishName(fishId, fishname);
        GetFishRarityName(fishId, fishtype);
        format(listFish, sizeof(listFish), "%s{FFFF00}%s\t{FFC0CB}%s\n", listFish, fishname, fishtype);
    }
    ShowPlayerDialog(playerid, DIALOG_GIVE_FISH, DIALOG_STYLE_TABLIST, "List Fish", listFish, "Chon", "Dong");
    return 1;
}

hook OnPlayerConnect(playerid) {
    InitFishingTD(playerid);

    PlayerFishRod[playerid][pRodType] = e_rod_type:ROD_COMMON;
    PlayerFishRod[playerid][pRodLevel] = 1;
    PlayerFishRod[playerid][pRodExp] = 5;

    for(new slot; slot < MAX_FISH_SLOTS; slot++) {
        PlayerFishInv[playerid][slot][pFishID] = -1;
        PlayerFishInv[playerid][slot][pFishWeight] = 0.0;
        PlayerFishInv[playerid][slot][pFishLoaded] = true;
    }
    return 1;
}

hook OnPlayerKeyStateChange(playerid, KEY:newkeys, KEY:oldkeys)
{
    if(PlayerFishing[playerid][pFishing]) {
        if(!(newkeys & KEY_JUMP)) {
            if(PlayerFishing[playerid][pState] == FISH_STATE_MINIGAME)
            {
                Fishing_Press(playerid);
            }
        }
    }
    return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == INVALID_TEXT_DRAW) {
        if(FishTdOpen[playerid]) {
            CloseFishingTD(playerid);
            return 1;
        }
    }
	return 1;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    HandleClickInv(playerid, playertextid);
    return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_GIVE_FISH) {
        if(!response) return 1;
        new fishid = listitem;
        // new rand = random(3) + 7;
        new Float:weight = GetRandomFishWeight(fishid);
        GivePlayerFish(playerid, fishid, weight);
    }
    return 1;
}