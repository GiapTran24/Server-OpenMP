// Accounts
// 2 Phút 1 lần sẽ lưu tất cả tài khoản đang online
task SaveAllAccounts[120000]()
{
    foreach(new p: Player) {
        if(IsPlayerConnected(p))
            Account_Save(p);
    }
}

// Playtime
task RunPlaytime[60000]()
{
    PlaytimeTimer();
}