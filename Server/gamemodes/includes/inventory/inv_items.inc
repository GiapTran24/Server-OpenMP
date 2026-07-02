#define MAX_ITEMS 50

enum E_ITEM_DATA
{
    itemID,
    itemModelID,
    itemName[32],
    itemType,
    Float:itemWeight,
    itemMaxStack,
    itemPrice,
    itemDescription[128]
};

new ItemData[MAX_ITEMS][E_ITEM_DATA];


LoadItems()
{
    print("Loading Items ...");
    new query[128];

    mysql_format(
        g_DatabaseHandle,
        query,
        sizeof(query),
        "SELECT * FROM `items` ORDER BY `id` ASC LIMIT %d",
        MAX_ITEMS
    );

    mysql_tquery(g_DatabaseHandle, query, "OnItemsLoaded");
}

forward OnItemsLoaded();
public OnItemsLoaded()
{
    new rows = cache_num_rows();
    new count = (rows > MAX_ITEMS) ? MAX_ITEMS : rows;

    for(new i = 0; i < MAX_ITEMS; i++)
    {
        ItemData[i][itemID] = -1;
    }

    for(new i = 0; i < count; i++)
    {
        cache_get_value_name_int(i, "id", ItemData[i][itemID]);
        cache_get_value_name_int(i, "itemModelID", ItemData[i][itemModelID]);
        cache_get_value_name(i, "itemName", ItemData[i][itemName], 32);
        cache_get_value_name_int(i, "itemType", ItemData[i][itemType]);
        cache_get_value_name_float(i, "itemWeight", ItemData[i][itemWeight]);
        cache_get_value_name_int(i, "itemMaxStack", ItemData[i][itemMaxStack]);
        cache_get_value_name_int(i, "itemPrice", ItemData[i][itemPrice]);
        cache_get_value_name(i, "itemDescription", ItemData[i][itemDescription], 128);
    }

    printf("[SQL] Loaded %d/%d items.", count, rows);
    return 1;
}

stock GetItemIndex(item_id) {
    for(new i = 0; i < MAX_ITEMS; i++) {
        if(ItemData[i][itemID] == item_id) return i;
    }
    return -1;
}

CMD:items(playerid, params[])
{
    new str[144], count = 0;
    
    SendClientMessage(playerid, 0x00FFFFAA, "==================== TOAN BO THONG TIN ITEMS ====================");

    for(new i = 0; i < MAX_ITEMS; i++)
    {
        // Chỉ in những ô có vật phẩm hợp lệ (Tên không trống)
        if(ItemData[i][itemName][0] != '\0')
        {
            // Dòng 1: ID, Tên, Model, Loại, Cân nặng, Stack, Giá
            format(str, sizeof(str), "[ID: %d] %s (Model: %d) | Loai: %d | %.2f kg | Max: %d | Gia: $%d",
                ItemData[i][itemID],
                ItemData[i][itemName],
                ItemData[i][itemModelID],
                ItemData[i][itemType],
                ItemData[i][itemWeight],
                ItemData[i][itemMaxStack],
                ItemData[i][itemPrice]
            );
            SendClientMessage(playerid, 0xFFFFFFFF, str);

            // Dòng 2: Mô tả chi tiết (In chữ màu xám để dễ phân biệt giữa các item)
            format(str, sizeof(str), ">> Mo ta: %s", ItemData[i][itemDescription]);
            SendClientMessage(playerid, 0xAAAAAAFF, str);
            
            // In thêm một dòng kẻ nhỏ để phân cách giữa các item cho đỡ rối mắt
            SendClientMessage(playerid, 0x555555FF, "------------------------------------------------------------------");
            
            count++;
        }
    }

    if(count == 0) {
        SendClientMessage(playerid, 0xFF0000AA, "He thong: Khong co du lieu item nao de hien thi.");
    } else {
        format(str, sizeof(str), "========= TONG CONG: %d ITEMS =========", count);
        SendClientMessage(playerid, 0x00FFFFAA, str);
    }
    return 1;
}
/* Item TEMPLATE
INSERT INTO `items` (`itemModelID`, `itemName`, `itemType`, `itemWeight`, `itemMaxStack`, `itemPrice`, `itemDescription`) VALUES
-- ====================================================
-- NHOM 1: VU KHI (Type 1)
-- ====================================================
(346, 'Sung luc 9mm (Colt45)', 1, 1.2, 1, 1500, 'Sung luc tieu chuan, de su dung.'),
(348, 'Sung Luc Desert Eagle', 1, 1.8, 1, 3500, 'Sung luc hang nang, sat thuong cuc cao.'),
(349, 'Sung Shotgun', 1, 3.5, 1, 4000, 'Sung san tam gan, do toa sat thuong rong.'),
(353, 'Sung MP5', 1, 2.5, 1, 6000, 'Sung tieu lien toc do ban nhanh, tam trung.'),
(355, 'Sung truong M4A1', 1, 4.0, 1, 12000, 'Sung truong tan cong quan doi, tam xa tot.'),
(356, 'Sung truong AK-47', 1, 4.3, 1, 10000, 'Sung truong co do giat cao nhung sat thuong manh.'),
(357, 'Sung ngam Sniper Rifle', 1, 5.5, 1, 25000, 'Sung ban tia tam sieu xa, co ong ngam.'),
(331, 'Gay bong chay', 1, 1.0, 1, 200, 'Vu khi can chien lam bang go cung.'),
(335, 'Dao gam', 1, 0.3, 1, 150, 'Vu khi can chien sac ben, de giau trong nguoi.'),

-- ====================================================
-- NHOM 2: DAN DUOC (Type 2)
-- ====================================================
(2012, 'Hop dan 9mm', 2, 0.02, 100, 10, 'Dan danh cho sung luc Colt45.'),
(2013, 'Hop dan .50 AE', 2, 0.04, 50, 25, 'Dan co lon danh cho sung Desert Eagle.'),
(2014, 'Hop dan 12-Gauge', 2, 0.05, 50, 30, 'Dan vo boc danh cho sung Shotgun.'),
(2015, 'Hop dan 9x19mm Parabellum', 2, 0.02, 150, 15, 'Dan tieu lien danh cho MP5.'),
(2016, 'Hop dan 5.56mm NATO', 2, 0.03, 100, 40, 'Dan tieu chuan quan doi danh cho sung M4A1.'),
(2017, 'Hop dan 7.62x39mm', 2, 0.03, 100, 35, 'Dan danh cho sung truong AK-47.'),
(2018, 'Hop dan .338 Lapua', 2, 0.08, 20, 100, 'Dan xuyen giap tam xa danh cho Sniper.'),

-- ====================================================
-- NHOM 3: DO AN / THUC UONG (Type 3)
-- ====================================================
(2814, 'Banh Burger', 3, 0.3, 5, 25, 'Hoi phuc 30% Thanh Doi.'),
(2814, 'Banh Pizza Ga', 3, 0.4, 5, 30, 'Hoi phuc 40% Thanh Doi.'),
(19830, 'Dui ga ran', 3, 0.2, 10, 15, 'Mon ga ran gion rum, hoi 20% Thanh Doi.'),
(2355, 'Banh Mi Kep', 3, 0.2, 5, 12, 'Banh mi kep thit an nhanh.'),
(1951, 'Lon Nuoc Ngot Sprunk', 3, 0.3, 10, 10, 'Giai khat tuc thi, hoi 25% Thanh Khat.'),
(1952, 'Lon Coca-Cola', 3, 0.3, 10, 10, 'Nuoc ngot co ga, hoi 25% Thanh Khat.'),
(1953, 'Chai Nuoc Khoang', 3, 0.5, 10, 5, 'Nuoc loc tinh khiet, hoi 40% Thanh Khat.'),
(19823, 'Ly Ca Phe Nong', 3, 0.2, 5, 15, 'Giup tinh tao, giam met moi cho nhan vat.'),

-- ====================================================
-- NHOM 4: Y TE / HOI PHUC (Type 4)
-- ====================================================
(1240, 'Hop Cuu Thuong (Medkit)', 4, 1.5, 3, 500, 'Hoi phuc 50% Mau sau 5 giay bang bo.'),
(1242, 'Ao Giap Chong Dan', 4, 3.0, 1, 1200, 'Tang 100 diem Giap bao ve ban than.'),
(1578, 'Thuoc Giam Dau', 4, 0.1, 20, 50, 'Hoi phuc 10% Mau ngay lap tuc.'),
(1241, 'Bang Gac Ca Nhan', 4, 0.1, 10, 20, 'Cam mau nhanh khi bi thuong nhe.'),

-- ====================================================
-- NHOM 5: SUA CHUA / KY THUAT (Type 5)
-- ====================================================
(19904, 'Hop Dung Cu Sua Xe', 5, 5.0, 2, 800, 'Dung de sua dong co xe khi bi hong tren duong.'),
(1650, 'Binh Xang Du Phong', 5, 4.0, 2, 300, 'Chua 10 Lit xang, cuu tro khi xe het nhien lieu.'),
(19895, 'Khoa Van Nang (Lockpick)', 5, 0.2, 10, 150, 'Cong cu dung de be khoa trom xe hoac cay cua.'),
(18641, 'Bo Dam (Radio)', 5, 0.5, 1, 400, 'Dung de ket noi tan so tro chuyen voi nguoi khac.'),
(18875, 'Dien Thoai GigaPhone', 5, 0.2, 1, 1000, 'Thiet bi lien lac, goi dien va nhan tin.'),
(19623, 'Sim Dien Thoai', 5, 0.01, 1, 200, 'Kich hoat so dien thoai moi.'),

-- ====================================================
-- NHOM 6: BAT HOP PHAP / CHAT CAM (Type 6)
-- ====================================================
(19473, 'Goi Can Sa (Marijuana)', 6, 0.05, 50, 100, 'Chat cam, su dung gay ao giac nhung hoi Giap.'),
(1575, 'Bot Ma Tuy Da (Meth)', 6, 0.02, 50, 250, 'Chat kich thich cuc manh, tang toc do chay.'),
(19835, 'Hat Giong Can Sa', 6, 0.01, 100, 20, 'Dung de gieo trong can sa o cac khu vuc kin.'),

-- ====================================================
-- NHOM 7: PHU KIEN / NGUYEN LIEU / KHAC (Type 7)
-- ====================================================
(19038, 'Kinh Mat Den', 7, 0.1, 1, 150, 'Phu kien deo mat thoi trang.'),
(18912, 'Mu Luoi Trai', 7, 0.2, 1, 100, 'Mu doi dau che nang.'),
(19318, 'Khau Trang Y Te', 7, 0.01, 5, 10, 'Che mat, giau danh tinh hoac chong bui.'),
(1212, 'Boc Tien Ban', 7, 1.0, 1, 0, 'Tien thu duoc tu viec cuop boc, can dem di rua.'),
(19793, 'Quang Sat', 7, 2.0, 20, 0, 'Nguyen lieu tho lay tu tho mo, dung de che tao.'),
(19792, 'Quang Dong', 7, 2.0, 20, 0, 'Nguyen lieu tho lay tu tho mo.'),
(19300, 'Vi Tien', 7, 0.1, 1, 50, 'Dung de chua cac loai the can cuoc, bang lai.'),
(19625, 'Can Cau Ca', 7, 1.5, 1, 500, 'Dụng cụ dùng cho công việc câu cá giải trí.');

*/