#include <YSI_Coding\y_hooks>

stock DestroyTaiXiuTD(playerid) {
    PlayerTextDrawDestroy(playerid, TxMain[playerid]);
    for(new i = 0; i < MAX_TX_TD; i++) {
        PlayerTextDrawDestroy(playerid, TaiXiuTD[playerid][i]);
    }
    PlayerTextDrawDestroy(playerid, TX_ThongBao[playerid]);
    PlayerTextDrawDestroy(playerid, tiendattai[playerid]);
    PlayerTextDrawDestroy(playerid, tiendatxiu[playerid]);
    PlayerTextDrawDestroy(playerid, tongtiencuoctai[playerid]);
    PlayerTextDrawDestroy(playerid, tongtiencuocxiu[playerid]);
    PlayerTextDrawDestroy(playerid, songuoicuocTai[playerid]);
    PlayerTextDrawDestroy(playerid, songuoicuocXiu[playerid]);
    PlayerTextDrawDestroy(playerid, btn_lsuphien[playerid]);
    for(new i = 0; i < MAX_TAIXIU_HISTORY_TD; i++) {
        PlayerTextDrawDestroy(playerid, LSuPhien[playerid][i]);
    }
    PlayerTextDrawDestroy(playerid, PicXucXac[playerid]);
    PlayerTextDrawDestroy(playerid, thoigiancuoc[playerid]);
    PlayerTextDrawDestroy(playerid, formketqua[playerid]);
    PlayerTextDrawDestroy(playerid, tiencuoc_min[playerid]);
    PlayerTextDrawDestroy(playerid, tiencuoc_max[playerid]);
    PlayerTextDrawDestroy(playerid, id_phiencuoc[playerid]);

    // hide btn
    PlayerTextDrawDestroy(playerid, btn_cuocTai[playerid]);
    PlayerTextDrawDestroy(playerid, btn_cuocXiu[playerid]);
    PlayerTextDrawDestroy(playerid, btn_thuhoicuoc[playerid]);
    PlayerTextDrawDestroy(playerid, btn_roiban[playerid]);
    
    for(new i = 0; i < MAX_TAIXIU_TOP; i++) {
        PlayerTextDrawDestroy(playerid, tien_top[playerid][i]);
        PlayerTextDrawDestroy(playerid, name_top[playerid][i]);
    }

    return 1;
}

stock ShowTaiXiuDisplay(playerid) {
    PlayerTextDrawShow(playerid, TxMain[playerid]);
    for(new i = 0; i < MAX_TX_TD; i++) {
        PlayerTextDrawShow(playerid, TaiXiuTD[playerid][i]);
    }

    PlayerTextDrawShow(playerid, tiendattai[playerid]);
    PlayerTextDrawShow(playerid, tiendatxiu[playerid]);

    PlayerTextDrawShow(playerid, tongtiencuoctai[playerid]);
    PlayerTextDrawShow(playerid, tongtiencuocxiu[playerid]);

    PlayerTextDrawShow(playerid, songuoicuocTai[playerid]);
    PlayerTextDrawShow(playerid, songuoicuocXiu[playerid]);

    for(new i = 0; i < MAX_TAIXIU_HISTORY_TD; i++) {
        PlayerTextDrawShow(playerid, LSuPhien[playerid][i]);
    }
    PlayerTextDrawShow(playerid, PicXucXac[playerid]);
    PlayerTextDrawShow(playerid, thoigiancuoc[playerid]);

    PlayerTextDrawShow(playerid, tiencuoc_min[playerid]);
    PlayerTextDrawShow(playerid, tiencuoc_max[playerid]);
    PlayerTextDrawShow(playerid, id_phiencuoc[playerid]);

    // show btn
    PlayerTextDrawShow(playerid, btn_cuocTai[playerid]);
    PlayerTextDrawShow(playerid, btn_cuocXiu[playerid]);
    PlayerTextDrawShow(playerid, btn_thuhoicuoc[playerid]);
    PlayerTextDrawShow(playerid, btn_lsuphien[playerid]);
    PlayerTextDrawShow(playerid, btn_roiban[playerid]);
    
    for(new i = 0; i < MAX_TAIXIU_TOP; i++) {
        PlayerTextDrawShow(playerid, tien_top[playerid][i]);
        PlayerTextDrawShow(playerid, name_top[playerid][i]);
    }

    SelectTextDraw(playerid, COLOR_YELLOW);
    return 1;
}

stock HideTaixiuDisplay(playerid) {
    PlayerTextDrawHide(playerid, TxMain[playerid]);
    for(new i = 0; i < MAX_TX_TD; i++) {
        PlayerTextDrawHide(playerid, TaiXiuTD[playerid][i]);
    }
    PlayerTextDrawHide(playerid, TX_ThongBao[playerid]);
    PlayerTextDrawHide(playerid, tiendattai[playerid]);
    PlayerTextDrawHide(playerid, tiendatxiu[playerid]);
    PlayerTextDrawHide(playerid, tongtiencuoctai[playerid]);
    PlayerTextDrawHide(playerid, tongtiencuocxiu[playerid]);
    PlayerTextDrawHide(playerid, songuoicuocTai[playerid]);
    PlayerTextDrawHide(playerid, songuoicuocXiu[playerid]);
    PlayerTextDrawHide(playerid, btn_lsuphien[playerid]);
    for(new i = 0; i < MAX_TAIXIU_HISTORY_TD; i++) {
        PlayerTextDrawHide(playerid, LSuPhien[playerid][i]);
    }
    PlayerTextDrawHide(playerid, PicXucXac[playerid]);
    PlayerTextDrawHide(playerid, thoigiancuoc[playerid]);
    PlayerTextDrawHide(playerid, formketqua[playerid]);
    PlayerTextDrawHide(playerid, tiencuoc_min[playerid]);
    PlayerTextDrawHide(playerid, tiencuoc_max[playerid]);
    PlayerTextDrawHide(playerid, id_phiencuoc[playerid]);

    // hide btn
    PlayerTextDrawHide(playerid, btn_cuocTai[playerid]);
    PlayerTextDrawHide(playerid, btn_cuocXiu[playerid]);
    PlayerTextDrawHide(playerid, btn_thuhoicuoc[playerid]);
    PlayerTextDrawHide(playerid, btn_roiban[playerid]);
    
    for(new i = 0; i < MAX_TAIXIU_TOP; i++) {
        PlayerTextDrawHide(playerid, tien_top[playerid][i]);
        PlayerTextDrawHide(playerid, name_top[playerid][i]);
    }

    CancelSelectTextDraw(playerid);
    return 1;
}

stock ToggleTaiXiuDisplay(playerid) {
    if(pTaiXiuTDShowed[playerid]) {
        HideTaixiuDisplay(playerid);
        pTaiXiuTDShowed[playerid] = false;
        pTXThongBao[playerid] = false;
    }
    else 
    {
        ShowTaiXiuDisplay(playerid);
        pTaiXiuTDShowed[playerid] = true;
        // Load Top Win
        for(new slot; slot < MAX_TAIXIU_TOP; slot++) {
            TaiXiu_SetTop(playerid, slot, gTXTopPlayer[slot], gTXTopMoney[slot]);
        }
        TaiXiu_SetThongBao(playerid, "Chao mung ban den voi Mini Game TAI hay XIU, chuc ban may man !");
    }
    return 1;
}

// He thong thong bao !
stock TaiXiu_SetThongBao(playerid, const msg[]) {
    SetPVarString(playerid, "TX_THONGBAO", msg);
    ShowTXThongBao(playerid);
    return 1;
}

stock TaiXiu_SetThongBaoAll(const msg[]) {
    foreach(new i: Player)
    {
        if(IsPlayerConnected(i))
        {
            TaiXiu_SetThongBao(i, msg);
        }
    }
    return 1;
}

forward AnimateTXThongBao(playerid);
public AnimateTXThongBao(playerid)
{
    if(!IsPlayerConnected(playerid) || !pTaiXiuTDShowed[playerid]) {
        PlayerTextDrawHide(playerid, TX_ThongBao[playerid]); 
        KillTimer(TX_AnimTimerTB[playerid]);
        TX_AnimTimerTB[playerid] = -1;
        return 1;
    }

    TX_AnimSizeY[playerid] += 0.08; 

    if(TX_AnimSizeY[playerid] >= 1.0) 
    {
        TX_AnimSizeY[playerid] = 1.0;
        PlayerTextDrawLetterSize(playerid, TX_ThongBao[playerid], 0.2, TX_AnimSizeY[playerid]);
        
        new str[524];
        GetPVarString(playerid, "TX_THONGBAO", str, sizeof(str));
        PlayerTextDrawSetString(playerid, TX_ThongBao[playerid], str);
        PlayerTextDrawShow(playerid, TX_ThongBao[playerid]);
        pTXThongBao[playerid] = true;

        KillTimer(TX_AnimTimerTB[playerid]);
        TX_AnimTimerTB[playerid] = -1;
    }
    else 
    {
        PlayerTextDrawLetterSize(playerid, TX_ThongBao[playerid], 0.2, TX_AnimSizeY[playerid]);
        PlayerTextDrawShow(playerid, TX_ThongBao[playerid]); 
    }
    return 1;
}
task CheckTXThongBaoLatest[500]()
{
    foreach(new i: Player)
    {
        if(!IsPlayerConnected(i) || !pTaiXiuTDShowed[i]) return 1;
        if(pTXThongBao[i] == true) {
            new str[524];
            GetPVarString(i, "TX_THONGBAO", str, sizeof(str));
            PlayerTextDrawSetString(i, TX_ThongBao[i], str);
            return 1;
        }
    }
    return 1;
}

stock ShowTXThongBao(playerid)
{
    if(TX_AnimTimerTB[playerid] != -1) {
        KillTimer(TX_AnimTimerTB[playerid]);
        TX_AnimTimerTB[playerid] = -1;
    }
    if(pTaiXiuTDShowed[playerid])
    {
        if(pTXThongBao[playerid] == false) {
            TX_AnimSizeY[playerid] = 0.0;
            TX_AnimTimerTB[playerid] = SetTimerEx("AnimateTXThongBao", 40, true, "d", playerid);
        }
    }
    return 1;
}

/////// Handle & Update

stock TaiXiu_SetTop(playerid, slot, const name[], money)
{
    if (slot < 0 || slot >= MAX_TAIXIU_TOP)
    {
        return 0;
    }
    new sMoney[64];
    format(sMoney, sizeof sMoney, "$%d", money);
    PlayerTextDrawSetString(playerid, name_top[playerid][slot], name);
    PlayerTextDrawSetString(playerid, tien_top[playerid][slot], sMoney);
    return 1;
}

stock TaiXiu_SetHistory(playerid, slot, is_tai)
{
    if (slot < 0 || slot >= MAX_TAIXIU_HISTORY_TD)
    {
        return 0;
    }

    if (is_tai == 1)
    {
        PlayerTextDrawSetString(playerid, LSuPhien[playerid][slot], "T");
        PlayerTextDrawColour(playerid, LSuPhien[playerid][slot], TX_COLOR_TAI);
    }
    else if (is_tai == 2)
    {
        PlayerTextDrawSetString(playerid, LSuPhien[playerid][slot], "X");
        PlayerTextDrawColour(playerid, LSuPhien[playerid][slot], TX_COLOR_XIU);
    }
    else
    {
        PlayerTextDrawSetString(playerid, LSuPhien[playerid][slot], "-");
        PlayerTextDrawColour(playerid, LSuPhien[playerid][slot], TX_COLOR_TRANSPARENT);
    }

    if (pTaiXiuTDShowed[playerid])
    {
        PlayerTextDrawShow(playerid, LSuPhien[playerid][slot]);
    }
    return 1;
}

stock TaiXiu_UpdateMainInfo(playerid, tai_total, xiu_total, tai_players, xiu_players, tai_bet, xiu_bet)
{
    new str[32];

    format(str, sizeof str, "$%d", tai_total);
    PlayerTextDrawSetString(playerid, tongtiencuoctai[playerid], str);

    format(str, sizeof str, "$%d", xiu_total);
    PlayerTextDrawSetString(playerid, tongtiencuocxiu[playerid], str);

    format(str, sizeof str, "%d", tai_players);
    PlayerTextDrawSetString(playerid, songuoicuocTai[playerid], str);

    format(str, sizeof str, "%d", xiu_players);
    PlayerTextDrawSetString(playerid, songuoicuocXiu[playerid], str);

    format(str, sizeof str, "$%d", tai_bet);
    PlayerTextDrawSetString(playerid, tiendattai[playerid], str);

    format(str, sizeof str, "$%d", xiu_bet);
    PlayerTextDrawSetString(playerid, tiendatxiu[playerid], str);
    return 1;
}

stock TaiXiu_UpdateConfig(playerid, min_bet, max_bet, session_id)
{
    new str[32];

    format(str, sizeof str, "$%d", min_bet);
    PlayerTextDrawSetString(playerid, tiencuoc_min[playerid], str);

    format(str, sizeof str, "$%d", max_bet);
    PlayerTextDrawSetString(playerid, tiencuoc_max[playerid], str);

    format(str, sizeof str, "#%d", session_id);
    PlayerTextDrawSetString(playerid, id_phiencuoc[playerid], str);
    return 1;
}

stock TaiXiu_UpdateTime(playerid, seconds)
{
    new str[8];
    format(str, sizeof str, "%d", seconds);
    PlayerTextDrawSetString(playerid, thoigiancuoc[playerid], str);
    return 1;
}

stock TaiXiu_UpdateResult(playerid, is_tai)
{
    if (is_tai == 1)
    {
        PlayerTextDrawSetString(playerid, formketqua[playerid], "TAI");
        PlayerTextDrawColour(playerid, formketqua[playerid], TX_COLOR_TAI);
    }
    else if(is_tai == 2)
    {
        PlayerTextDrawSetString(playerid, formketqua[playerid], "XIU");
        PlayerTextDrawColour(playerid, formketqua[playerid], TX_COLOR_XIU);
    }

    if (pTaiXiuTDShowed[playerid])
    {
        PlayerTextDrawHide(playerid, PicXucXac[playerid]);
        PlayerTextDrawShow(playerid, formketqua[playerid]);
    }
    return 1;
}

// Handle click dung trong model goc (OnPlayerClickPlayerTextDraw)
stock TaiXiu_HandleClick(playerid, PlayerText:playertextid)
{
    if(pTaiXiuTDShowed[playerid]) {
        if(playertextid == btn_cuocTai[playerid]) {
            pTX_SessionID[playerid] = TX[txSessionID];
            if(TX[txState] != TX_STATE_BETTING) return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "He thong dang tra ket qua, khong the dat cuoc luc nay!", "OK", "");
            if(PlayerBetTai[playerid] > 0 || PlayerBetXiu[playerid] > 0)  return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "Ban da dat cuoc roi!", "OK", "");
            ShowPlayerDialog(playerid, DIALOG_TX_TAI, DIALOG_STYLE_INPUT, "Dat cuoc vao TAI", "Nhap so tien muon cuoc", "Xac nhan", "Huy");
            return 1;
        }
        if(playertextid == btn_cuocXiu[playerid]) {
            pTX_SessionID[playerid] = TX[txSessionID];
            if(TX[txState] != TX_STATE_BETTING) return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "He thong dang tra ket qua, khong the dat cuoc luc nay!", "OK", "");
            if(PlayerBetTai[playerid] > 0 || PlayerBetXiu[playerid] > 0)  return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "Ban da dat cuoc roi!", "OK", "");
            ShowPlayerDialog(playerid, DIALOG_TX_XIU, DIALOG_STYLE_INPUT, "Dat cuoc vao XIU", "Nhap so tien muon cuoc","Xac nhan", "Huy");
            return 1;
        }
        if(playertextid == btn_lsuphien[playerid]) {
            new results[1024],str[1024];
            format(results, sizeof(results), "Phien\t\tKet Qua\n");
            for(new i = 0; i < MAX_TAIXIU_HISTORY; i++)
            {
                if(TXHistory[i][result] == 1)
                {
                    format(str, sizeof(str), "%d\t\tTAI\n", TXHistory[i][id_phien]);
                }
                else if(TXHistory[i][result] == 2)
                {
                    format(str, sizeof(str), "%d\t\tXIU\n", TXHistory[i][id_phien]);
                }
                else
                {
                    format(str, sizeof(str), "-\t\t-\n");
                }
                strcat(results, str);
            }
            ShowPlayerDialog(playerid, DIALOG_TX_HISTORY, DIALOG_STYLE_TABLIST, "Lich Su Phien", results,"X", "");
            return 1;
        }
        if(playertextid == btn_thuhoicuoc[playerid]) {
            pTX_SessionID[playerid] = TX[txSessionID];
            if(TX[txState] != TX_STATE_BETTING)  return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "He thong dang tra ket qua, khong the thu hoi cuoc luc nay!", "OK", "");
            if(PlayerBetTai[playerid] == 0 && PlayerBetXiu[playerid] == 0) return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "Ban chua dat cuoc, khong the thu hoi!", "OK", "");
            ShowPlayerDialog(playerid, DIALOG_TX_XACNHAN_THUHOI, DIALOG_STYLE_MSGBOX, "Xac nhan thu hoi cuoc", "Ban co chac chan muon thu hoi cuoc khong?", "Co", "Khong");
            return 1;
        }
        if(playertextid == btn_roiban[playerid]) {
            ShowPlayerDialog(playerid, DIALOG_TX_XACNHAN_ROIBAN, DIALOG_STYLE_MSGBOX, "Xac nhan roi ban", "Ban co chac chan muon roi ban khong?", "Co", "Khong");
            return 1;
        }
    }

    return 0;
}

InitTaiXiuSystems()
{
    TX[txSessionID] = 0;
    StartTaiXiuSession();
    SetTimer("TaiXiuUpdate", 1000, true);
    return 1;
}

forward TaiXiuUpdate();
public TaiXiuUpdate()
{
    if(BaoTriTaiXiuAD == 1) return 1;
    if(TX[txState] == TX_STATE_BETTING)
    {
        TX[txTimer]--;
        foreach(new i: Player) {
            if(IsPlayerConnected(i) && pTaiXiuTDShowed[i]) {
                TaiXiu_UpdateTime(i, TX[txTimer]);
                TaiXiu_UpdateMainInfo(i, TX[txTotalTai], TX[txTotalXiu], TX[txPlayersTai], TX[txPlayersXiu], PlayerBetTai[i], PlayerBetXiu[i]);
            }
        }
        if(TX[txTimer] <= 0)
        {
            ProcessTaiXiuResult();
        }
    }
    else if(TX[txState] == TX_STATE_RESULT)
    {
        TX[txTimer]--;
        if(TX[txTimer] <= 0)
        {
            StartTaiXiuSession();
        }
    }
    return 1;
}

stock StartTaiXiuSession()
{
    TX[txState] = TX_STATE_BETTING;
    TX[txTimer] = TX_TIME_BET;
    TX[txTotalTai] = 0;
    TX[txTotalXiu] = 0;
    TX[txPlayersTai] = 0;
    TX[txPlayersXiu] = 0;
    TX[txAdminForce] = 0;
    TX[txSessionID]++;

    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        PlayerBetTai[i] = 0;
        PlayerBetXiu[i] = 0;
    }

    new msg[128];
    format(msg, sizeof(msg), "[THONG BAO] Phien #%d da bat dau! Ban co ( %d giay ) de dat cuoc.", TX[txSessionID], TX_TIME_BET);
    TaiXiu_SetThongBaoAll(msg);
    //printf(msg);
    foreach(new i: Player) {
        if(IsPlayerConnected(i) && pTaiXiuTDShowed[i]) {
            TaiXiu_UpdateConfig(i, TX_MIN_BET, TX_MAX_BET, TX[txSessionID]);
            TaiXiu_UpdateMainInfo(i, TX[txTotalTai], TX[txTotalXiu], TX[txPlayersTai], TX[txPlayersXiu], PlayerBetTai[i], PlayerBetXiu[i]);
            PlayerTextDrawShow(i, PicXucXac[i]);
            PlayerTextDrawHide(i, formketqua[i]);
            for(new slot = 0; slot < MAX_TAIXIU_HISTORY_TD; slot++)
            {
                TaiXiu_SetHistory(i, slot, TXHistory[slot][result]);
            }
        }
    }
}

stock ProcessTaiXiuResult()
{
    TaiXiu_SetThongBaoAll("[XUC XAC] Dang lac xi ngau, vui long cho...");
    new d1, d2, d3, total;
    
    // Admin 
    if(TX[txAdminForce] == 1) { // Ep XIU (Tong tu 3 den 10)
        do {
            d1 = random(6) + 1; d2 = random(6) + 1; d3 = random(6) + 1;
            total = d1 + d2 + d3;
        } while(total > 10);
    } 
    else if(TX[txAdminForce] == 2) { // Ep TAI (TTong tu 11 den 18)
        do {
            d1 = random(6) + 1; d2 = random(6) + 1; d3 = random(6) + 1;
            total = d1 + d2 + d3;
        } while(total < 11);
    }
    else if(TX[txAdminForce] == 3) { // Ben nhieu tien hon thang
        if(TX[txTotalTai] > TX[txTotalXiu]) {
            do {
                d1 = random(6) + 1; d2 = random(6) + 1; d3 = random(6) + 1;
                total = d1 + d2 + d3;
            } while(total < 11);
        } 
        else if(TX[txTotalTai] < TX[txTotalXiu]) {
            do {
                d1 = random(6) + 1; d2 = random(6) + 1; d3 = random(6) + 1;
                total = d1 + d2 + d3;
            } while(total > 10);
        } else {
            d1 = random(6) + 1; d2 = random(6) + 1; d3 = random(6) + 1;
            total = d1 + d2 + d3;
        }
    } 
    else if(TX[txAdminForce] == 4) { // Ben it tien hon thang
        if(TX[txTotalTai] < TX[txTotalXiu]) {
            do {
                d1 = random(6) + 1; d2 = random(6) + 1; d3 = random(6) + 1;
                total = d1 + d2 + d3;
            } while(total < 11);
        } 
        else if(TX[txTotalTai] > TX[txTotalXiu]) {
            do {
                d1 = random(6) + 1; d2 = random(6) + 1; d3 = random(6) + 1;
                total = d1 + d2 + d3;
            } while(total > 10);
        } else {
            d1 = random(6) + 1; d2 = random(6) + 1; d3 = random(6) + 1;
            total = d1 + d2 + d3;
        }
    } 
    else { 
        d1 = random(6) + 1; d2 = random(6) + 1; d3 = random(6) + 1;
        total = d1 + d2 + d3;
    }

    new isTai = (total >= 11) ? 1 : 2; // 1: Tai, 2: Xiu
    new resultName[10];
    format(resultName, sizeof(resultName), isTai == 1 ? "TAI" : "XIU");
    foreach(new i: Player) {
        if(IsPlayerConnected(i) && pTaiXiuTDShowed[i]) {
            TaiXiu_UpdateResult(i, isTai);
        }
    }

    // Luu lich su ket qua
    for(new i = MAX_TAIXIU_HISTORY - 1; i > 0; i--)
    {
        TXHistory[i][id_phien] = TXHistory[i-1][id_phien];
        TXHistory[i][result] = TXHistory[i-1][result];
    }
    TXHistory[0][id_phien] = TX[txSessionID];
    TXHistory[0][result] = isTai;
    foreach(new i: Player) {
        if(IsPlayerConnected(i) && pTaiXiuTDShowed[i]) {
            for(new slot = 0; slot < MAX_TAIXIU_HISTORY_TD; slot++)
            {
                TaiXiu_SetHistory(i, slot, TXHistory[slot][result]);
            }
        }
    }

    new msg[128];
    format(msg, sizeof(msg), "KET QUA PHIEN #%d: [ %d - %d - %d ] (Tong: %d) |=> %s", TX[txSessionID], d1, d2, d3, total, resultName);
    TaiXiu_SetThongBaoAll(msg);
    //printf(msg);
    // RESET TOP WIN
    for(new x; x < MAX_TAIXIU_TOP; x++)
    {
        gTXTopMoney[x] = 0;
        gTXTopPlayer[x][0] = EOS;
    }

    foreach(new i: Player)
    {
        if(IsPlayerConnected(i))
        {
            new tx_give_money;
            if(isTai == 1 && PlayerBetTai[i] > 0)
            {
                tx_give_money = PlayerBetTai[i] * 2;
                GivePlayerCash(i, tx_give_money);
                pTX_WinMoney[i] += tx_give_money;
                format(msg, sizeof(msg), "[TAI XIU] Ban da thang %d$ tu cuoc TAI!", tx_give_money);
                if(pTaiXiuTDShowed[i]) {
                    TaiXiu_SetThongBao(i, msg);
                } else {
                SendClientMessage(i, TX_COLOR_TAI, msg);
                }
                
            }
            else if(isTai == 2 && PlayerBetXiu[i] > 0)
            {
                tx_give_money = PlayerBetXiu[i] * 2;
                GivePlayerCash(i, tx_give_money);
                pTX_WinMoney[i] += tx_give_money;
                format(msg, sizeof(msg), "[TAI XIU] Ban da thang %d$ tu cuoc XIU!", tx_give_money);
                if(pTaiXiuTDShowed[i]) {
                    TaiXiu_SetThongBao(i, msg);
                } else {
                SendClientMessage(i, TX_COLOR_XIU, msg);
                }
            }
            else if (PlayerBetTai[i] > 0 || PlayerBetXiu[i] > 0)
            {
                pTX_WinMoney[i] += 0;
                if(pTaiXiuTDShowed[i]) {
                    TaiXiu_SetThongBao(i, "[TAI XIU] Ban da thua cuoc trong phien nay, chuc ban may man lan sau!");
                } else {
                SendClientMessage(i, TX_COLOR_TRANSPARENT, "[TAI XIU] Ban da thua cuoc trong phien nay, chuc ban may man lan sau!");
                }
            }
            // UPDATE TOP WIN DATA
            for(new pos; pos < MAX_TAIXIU_TOP; pos++)
            {
                if(pTX_WinMoney[i] > gTXTopMoney[pos])
                {
                    // day xuong
                    for(new move = MAX_TAIXIU_TOP - 1; move > pos; move--)
                    {
                        gTXTopMoney[move] = gTXTopMoney[move - 1];

                        format(
                            gTXTopPlayer[move],
                            MAX_PLAYER_NAME,
                            "%s",
                            gTXTopPlayer[move - 1]
                        );
                    }

                    // insert top moi
                    GetPlayerName(i,
                    gTXTopPlayer[pos],
                    MAX_PLAYER_NAME);

                    gTXTopMoney[pos] = pTX_WinMoney[i];

                    break;
                }
            }
        }
    }
    // Set Top To Textdrawn
    foreach(new p: Player) {
        if(pTaiXiuTDShowed[p] && IsPlayerConnected(p)) {
            for(new slot; slot < MAX_TAIXIU_TOP; slot++) {
                TaiXiu_SetTop(p, slot, gTXTopPlayer[slot], gTXTopMoney[slot]);
            }
        }
    }

    TX[txState] = TX_STATE_RESULT;
    TX[txTimer] = TX_TIME_RESULT;
}


// Hooks

hook OnGameModeInit() {
    InitTaiXiuSystems();
    return 1;
}

hook OnPlayerConnect(playerid) {
    CreateTaiXiuTD(playerid);
    return 1;
}

hook OnPlayerDisconnect(playerid, reason) {
    DestroyTaiXiuTD(playerid);
    return 1;
}

hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if(clickedid == INVALID_TEXT_DRAW) {
        if(pTaiXiuTDShowed[playerid]) {
            HideTaixiuDisplay(playerid);
            pTaiXiuTDShowed[playerid] = false;
            pTXThongBao[playerid] = false;
        }
    }

	return 1;
}

hook OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if(dialogid == DIALOG_TX_TAI) {
        if(response) {
            if(!IsNumeric(inputtext)) {
                ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "Tien cuoc khong hop le!", "OK", "");
                return 1;
            }
            if(TX[txState] != TX_STATE_BETTING) return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "He thong dang tra ket qua, khong the dat cuoc luc nay!", "OK", "");
            if(TX[txSessionID] != pTX_SessionID[playerid]) return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "Thao tac khong cung phien cuoc!", "OK", "");

            new str[32];
            new bet_amount = strval(inputtext);
            if(bet_amount < TX_MIN_BET || bet_amount > TX_MAX_BET || GetPlayerCash(playerid) < bet_amount) {
                ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "So tien khong hop le hoac ban khong du tien!", "OK", "");
                return 1;
            }
            format(str, sizeof str, "Ban dat $%d vao TAI", bet_amount);
            GivePlayerCash(playerid, -bet_amount);
            PlayerBetTai[playerid] += bet_amount;
            TX[txTotalTai] += bet_amount;
            TX[txPlayersTai]++;
            TaiXiu_SetThongBao(playerid, str);
            ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DAT CUOC THANH CONG", str, "X", "");
        }
    }
    if(dialogid == DIALOG_TX_XIU) {
        if(response) {
            if(!IsNumeric(inputtext)) {
                ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "Tien cuoc khong hop le!", "OK", "");
                return 1;
            }
            if(TX[txState] != TX_STATE_BETTING) return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "He thong dang tra ket qua, khong the dat cuoc luc nay!", "OK", "");
            if(TX[txSessionID] != pTX_SessionID[playerid]) return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "Thao tac khong cung phien cuoc!", "OK", "");
            
            new str[32];
            new bet_amount = strval(inputtext);
            if(bet_amount < TX_MIN_BET || bet_amount > TX_MAX_BET || GetPlayerCash(playerid) < bet_amount) {
                ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "So tien khong hop le hoac ban khong du tien!", "OK", "");
                return 1;
            }
            format(str, sizeof str, "Ban dat $%d vao XIU", bet_amount);
            GivePlayerCash(playerid, -bet_amount);
            PlayerBetXiu[playerid] += bet_amount;
            TX[txTotalXiu] += bet_amount;
            TX[txPlayersXiu]++;
            TaiXiu_SetThongBao(playerid, str);
            ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "DAT CUOC THANH CONG", str, "X", "");
        }
    }
    if(dialogid == DIALOG_TX_XACNHAN_THUHOI) {
        if(response) {
            if(TX[txSessionID] != pTX_SessionID[playerid]) return ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "Thao tac khong cung phien cuoc!", "OK", "");
            if(PlayerBetTai[playerid] > 0) {
                GivePlayerCash(playerid, PlayerBetTai[playerid]);
                TX[txTotalTai] -= PlayerBetTai[playerid];
                TX[txPlayersTai]--;
                PlayerBetTai[playerid] = 0;
                ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "THU HOI CUOC", "Ban da thu hoi cuoc TAI!", "OK", "");
            } 
            else if(PlayerBetXiu[playerid] > 0) {
                GivePlayerCash(playerid, PlayerBetXiu[playerid]);
                TX[txTotalXiu] -= PlayerBetXiu[playerid];
                TX[txPlayersXiu]--;
                PlayerBetXiu[playerid] = 0;
                ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "THU HOI CUOC", "Ban da thu hoi cuoc XIU!", "OK", "");
            } 
            else {
                ShowPlayerDialog(playerid, DIALOG_NOTHING, DIALOG_STYLE_MSGBOX, "Error", "Ban chua dat cuoc phien nay!", "OK", "");
            }
        }
    }
    if(dialogid == DIALOG_TX_XACNHAN_ROIBAN) {
        if(response) {
            ToggleTaiXiuDisplay(playerid);
            SendClientMessage(playerid, -1, "[TAi XIU]: Ban da roi khoi ban cuoc !");
        }
    }

    if(dialogid == DIALOG_TX_ADMIN_EDIT) {
        if(!response) return 1;
        switch(listitem) {
            case 0: {
                TX[txAdminForce] = 0;
                SendClientMessage(playerid, -1, "Phien nay se la ngau nhien.");
            }
            case 1: {
                TX[txAdminForce] = 1;
                SendClientMessage(playerid, -1, "Da can thiep: Ket qua se la XIU.");
            }
            case 2: {
                TX[txAdminForce] = 2;
                SendClientMessage(playerid, -1, "Da can thiep : Ket qua se la TAI.");
            }
            case 3: {
                TX[txAdminForce] = 3;
                SendClientMessage(playerid, -1, "Da can thiep : Ben cuoc nhieu hon se thang.");
            }
            case 4: {
                TX[txAdminForce] = 4;
                SendClientMessage(playerid, -1, "Da can thiep : Ben cuoc it hon se thang.");
            }
            case 5: {
                if(BaoTriTaiXiuAD == 0)
                {
                    BaoTriTaiXiuAD = 1;
                    SendClientMessageToAllEx(COLOR_RED, "[TAI XIU] He thong tai xiu da ngung nhan cuoc.!");
                    foreach(new p: Player) {
                        if(pTaiXiuTDShowed[p]) {
                            HideTaixiuDisplay(p);
                            pTaiXiuTDShowed[p] = false;
                            pTXThongBao[p] = false;
                            
                        }
                        if(PlayerBetTai[p] > 0 || PlayerBetXiu[p] > 0) {
                            new hoanTien = PlayerBetTai[p] + PlayerBetXiu[p];
                            SendClientMessage(p, COLOR_GREEN, "[TAI XIU] Ban da duoc hoan tra so tien da cuoc trong phien nay !");
                            GivePlayerCash(p, hoanTien);
                        }
                    }
                }
                else
                {
                    BaoTriTaiXiuAD = 0;
                    SendClientMessageToAllEx(COLOR_GREEN, "[TAI XIU] He thong tai xiu da cho phep dat cuoc.!");
                }
            }
        }
    }
    return 1;
}

hook OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    TaiXiu_HandleClick(playerid, playertextid);
	return 1;
}


// Commands
CMD:taixiu(playerid, params[]) {
    if(BaoTriTaiXiuAD == 1) return SendClientMessage(playerid, COLOR_RED, "[TAI XIU] He thong tai xiu da ngung nhan cuoc.!");
    else 
    {
        ToggleTaiXiuDisplay(playerid);
    }
    return 1;
}

CMD:edittaixiucailonmamay(playerid, params[])
{
    if(PlayerInfo[playerid][pAdmin] < 999990) 
        return SendClientMessage(playerid, -1, "Ban khong co quyen su dung lenh nay!");
        
    new str[1024];
    format(str, sizeof(str), "Random\nXiu Win\nTai Win\nNhieu Tien Win\nIt Tien Win\nBao Tri TX");
    ShowPlayerDialog(playerid, DIALOG_TX_ADMIN_EDIT, DIALOG_STYLE_LIST, "Dashboard TAi XIU", str, "Chon", "Huy");
    return 1;
}
CMD:settaixiutime(playerid, params[]) {
    if(!IsPlayerAdmin(playerid)) return SendClientMessage(playerid, -1, "Ban khong co quyen su dung lenh nay!");
    new newTimeTx;
    if(sscanf(params, "d", newTimeTx)) return SendClientMessage(playerid, -1, "USE: /settaixiutime [time]");
    TX_TIME_BET = newTimeTx;
    new str[255];
    format(str, sizeof(str), "Da set thoi gian cho Tai Xiu la: %d [Thoi gian se duoc su dung cho phien tai xiu tiep theo tro di !]", TX_TIME_BET);
    SendClientMessage(playerid, -1, str);
    return 1;
}