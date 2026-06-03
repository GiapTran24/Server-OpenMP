// Accounts
// 2 Phút 1 lần sẽ lưu tất cả tài khoản đang online
task SaveAllAccounts[12000]()
{
    foreach(new p: Player) {
        if(IsPlayerConnected(p))
            Account_Save(p);
    }
}