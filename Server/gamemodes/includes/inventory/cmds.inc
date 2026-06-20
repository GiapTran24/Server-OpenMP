CMD:inv(playerid, params[])
{
    EnsureInvCreated(playerid);

    if(!PlayerHasInvVisible[playerid]) {
        ShowInventory(playerid);
    } else {
        HideInventory(playerid);
    }
    return 1;
}