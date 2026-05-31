#define TX_COLOR_WHITE                 0xFFFFFFFF
#define TX_COLOR_TAI                   0x0000FFFF
#define TX_COLOR_XIU                   0xFF0000FF
#define TX_COLOR_PINK                  0xFF00FFFF
#define TX_COLOR_MONEY                 0x00FF00FF
#define TX_COLOR_GREEN                 0x008AFF7F
#define TX_COLOR_TRANSPARENT           0x00000032
#define TX_COLOR_BLACK_ALPHA           0x00000087

#define MAX_TAIXIU_TOP 7
#define MAX_TX_TD 21
#define MAX_TAIXIU_HISTORY_TD 5

#define DIALOG_TX_HISTORY 8000
#define DIALOG_TX_TAI 8001
#define DIALOG_TX_XIU 8002
#define DIALOG_TX_XACNHAN_THUHOI 8003
#define DIALOG_TX_XACNHAN_ROIBAN 8004


#define TX_STATE_BETTING 1
#define TX_STATE_RESULT  2
#define TX_TIME_RESULT   5
#define TX_MIN_BET       100000
#define TX_MAX_BET       100000000
#define MAX_TAIXIU_HISTORY 20

// Player Variables
new bool:pTaiXiuTDShowed[MAX_PLAYERS];
new bool:pTXThongBao[MAX_PLAYERS] = false;
new TX_AnimTimerTB[MAX_PLAYERS] = -1;
new Float:TX_AnimSizeY[MAX_PLAYERS];


new TX_TIME_BET = 21; // mac dinh thoi gian la 21
enum tx_data {
    txState,
    txTimer,
    txTotalTai,
    txTotalXiu,
    txPlayersTai,
    txPlayersXiu,
    txAdminForce, // 0: Random | 1: Ep ra Xiu | 2: Ep ra tai
    txSessionID
};
enum TXHistory_Data {
    id_phien,
    result // 1: Tai, 2: Xiu
};
new TX[tx_data];
new PlayerBetTai[MAX_PLAYERS];
new PlayerBetXiu[MAX_PLAYERS];
new TXHistory[MAX_TAIXIU_HISTORY][TXHistory_Data];
new pTX_SessionID[MAX_PLAYERS];

new pTX_WinMoney[MAX_PLAYERS];
new gTXTopPlayer[MAX_TAIXIU_TOP][MAX_PLAYER_NAME];
new gTXTopMoney[MAX_TAIXIU_TOP];

// Variables TD
// form nen
new PlayerText:TxMain[MAX_PLAYERS];
new PlayerText:TaiXiuTD[MAX_PLAYERS][MAX_TX_TD];
new PlayerText:TX_ThongBao[MAX_PLAYERS];

// list Top
new PlayerText:name_top[MAX_PLAYERS][MAX_TAIXIU_TOP];
new PlayerText:tien_top[MAX_PLAYERS][MAX_TAIXIU_TOP];

// button
new PlayerText:btn_thuhoicuoc[MAX_PLAYERS];
new PlayerText:btn_roiban[MAX_PLAYERS];
new PlayerText:btn_cuocTai[MAX_PLAYERS];
new PlayerText:btn_cuocXiu[MAX_PLAYERS];
new PlayerText:btn_lsuphien[MAX_PLAYERS];

// Thong tin
new PlayerText:tiendattai[MAX_PLAYERS];
new PlayerText:tiendatxiu[MAX_PLAYERS];
new PlayerText:tongtiencuoctai[MAX_PLAYERS];
new PlayerText:tongtiencuocxiu[MAX_PLAYERS];
new PlayerText:songuoicuocTai[MAX_PLAYERS];
new PlayerText:songuoicuocXiu[MAX_PLAYERS];
new PlayerText:LSuPhien[MAX_PLAYERS][MAX_TAIXIU_HISTORY_TD];
new PlayerText:thoigiancuoc[MAX_PLAYERS];
new PlayerText:PicXucXac[MAX_PLAYERS];
new PlayerText:formketqua[MAX_PLAYERS];
new PlayerText:tiencuoc_min[MAX_PLAYERS];
new PlayerText:tiencuoc_max[MAX_PLAYERS];
new PlayerText:id_phiencuoc[MAX_PLAYERS];