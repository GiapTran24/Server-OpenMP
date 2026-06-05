// SQL INIT VARIABLE
new MySQL:g_DatabaseHandle;

// Pinfo
enum pInfo {
	pID,

    pLevel,
    pCash,
    pBank,
    pAdmin,

    Float:pPosX,
    Float:pPosY,
    Float:pPosZ,
    Float:pAngle,

    pInterior,
    pVirtualWorld,

    pSkin,
    pGender,

    bool:pLogged
}
new PlayerInfo[MAX_PLAYERS][pInfo];
new Float:InitPlayerPos[4] = {1958.38, 1343.16, 15.3746, 0.0};
new gLoggedIn[MAX_PLAYERS];

// Admin
enum E_ADMIN_LEVEL
{
    adminLevel,
    adminName[32]
};

new AdminLevels[][E_ADMIN_LEVEL] =
{
    {1, "Trial Admin"},
    {2, "Junior Admin"},
    {3, "Senior Admin"},
    {4, "Lead Admin"},
    {5, "Head Admin"},
    {6, "Developer"}
};

new bool:gAdminGod[MAX_PLAYERS];

enum E_GOTO_LOC
{
    gotoName[32],
    Float:gotoX,
    Float:gotoY,
    Float:gotoZ,
    gotoInt,
    gotoVW
};
new SetAdminTarget[MAX_PLAYERS];
new GotoLocations[][E_GOTO_LOC] =
{
    {"LS",        1529.6,  -1691.2, 13.3,    0, 0},
    {"SF",       -1605.0,   720.0,  12.0,    0, 0},
    {"LV",        1699.2,  1435.1,  10.7,    0, 0},
    {"RC",        1253.70, 343.73,  19.41,   0, 0},
    {"El Que",   -1446.59,2608.44,  55.83,   0, 0},
    {"Bayside",  -2465.13,2333.65,   4.83,   0, 0},
    {"Bank",      1487.91,-1030.60, 23.66,   0, 0},
    {"FBI",         344.77,-1526.08, 33.28,   0, 0},
    {"DOC",      -2029.23, -78.33, 35.32,    0, 0}
};


//-----------------------------------------------------------------------------
// Conversation && Messages
//-----------------------------------------------------------------------------
enum E_CONVERSATION
{
    ConvID,
    ConvPlayer1,
    ConvPlayer2,
    ConvLastMessage,
    ConvLastTime
};
new ConversationData[MAX_CONVERSATIONS][E_CONVERSATION];
new ConversationCount;

enum E_MESSAGE
{
    MsgID,
    MsgConversation,
    MsgSender,
    MsgReceiver,
    MsgTime,
    MsgRead,
    MsgText[MAX_MESSAGE_LEN]
};
new MessageData[MAX_MESSAGES][E_MESSAGE];
new MessageCount;