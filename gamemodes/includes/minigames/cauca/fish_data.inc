enum e_fish_rarity
{
    FISH_COMMON,
    FISH_RARE,
    FISH_EPIC,
    FISH_LEGENDARY
};

new const FishRarityDescription[][] =
{
    "Loai pho bien, xuat hien o hau het cac vung nuoc. De cau nhung gia tri khong cao.",

    "Loai kha hiem, can mot chut may man de bat duoc. Co gia ban tot hon ca pho bien.",

    "Loai cuc hiem duoc nhieu ngu dan san lung. Kho cau nhung mang lai gia tri rat cao.",

    "Loai huyen thoai chi xuat hien trong nhung dieu kien dac biet. Bat duoc chung la niem tu hao cua moi can thu."
};

enum e_fish_data
{
    fishName[32],

    fishModelId,

    fishPrice,

    fishMinWeight,

    fishMaxWeight,

    e_fish_rarity:fishRarity,

    fishExp,

    fishPullPower
};
new FishData[][e_fish_data] =
{
    //========================
    // COMMON
    //========================

    {"Ca Chep",         1599,  800, 1, 3, FISH_COMMON,      5, 3},
    {"Ca Ro",           1599,  900, 1, 4, FISH_COMMON,      5, 4},
    {"Ca Tre",          1605, 1000, 2, 5, FISH_COMMON,      6, 4},
    {"Ca Loc",          1605, 1200, 2, 6, FISH_COMMON,      6, 5},
    {"Ca Me",           1605, 1300, 3, 7, FISH_COMMON,      7, 4},
    {"Ca Dieu Hong",    1605, 1500, 2, 6, FISH_COMMON,      7, 3},
    {"Tom",             1609,  900, 1, 2, FISH_COMMON,      5, 2},
    {"Cua Dong",        1610, 1000, 1, 2, FISH_COMMON,      5, 2},

    //========================
    // RARE
    //========================

    {"Ca Hoi",          1604, 2200, 3, 8,  FISH_RARE,      10, 6},
    {"Ca Thu",          1606, 2600, 4,10,  FISH_RARE,      10, 8},
    {"Ca Ngu",          1606, 3000, 5,12,  FISH_RARE,      11, 9},
    {"Ca Chinh",        1605, 3200, 5,11,  FISH_RARE,      11, 7},
    {"Ca Chim",         1606, 3400, 4, 9,  FISH_RARE,      12, 7},
    {"Muc La",          1611, 3000, 3, 8,  FISH_RARE,      11, 6},
    {"Ghe",             1612, 2800, 2, 6,  FISH_RARE,      10, 6},
    {"Bach Tuoc",       1613, 3500, 4,10,  FISH_RARE,      12, 8},

    //========================
    // EPIC
    //========================

    {"Tom Hum",         1614, 5000, 5,12, FISH_EPIC,       18, 10},
    {"Ca Kiem",         1606, 5500, 8,18, FISH_EPIC,       18, 11},
    {"Ca Duoi",         1607, 5800, 8,20, FISH_EPIC,       19, 12},
    {"Ca Map Con",      1607, 6500,10,22, FISH_EPIC,       20, 13},
    {"Cua Hoang De",    1615, 7000, 8,18, FISH_EPIC,       20, 14},

    //========================
    // LEGENDARY
    //========================

    {"Ca Map",          1607,10000,15,35, FISH_LEGENDARY, 30, 15},
    {"Ca Map Trang",    1607,12000,18,40, FISH_LEGENDARY, 32, 16},
    {"Ca Rong",         1608,15000,10,30, FISH_LEGENDARY, 35, 17}
};

stock GetFishName(fishid, dest[], len = sizeof(dest))
{
    if (fishid < 0 || fishid >= sizeof(FishData))
    {
        format(dest, len, "Unknown");
        return 0;
    }

    format(dest, len, "%s", FishData[fishid][fishName]);
    return 1;
}

stock GetFishModel(fishid) {
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 19300;

    return FishData[fishid][fishModelId];
}

stock GetFishPullPower(fishid) {
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 1;

    return FishData[fishid][fishPullPower];
}

stock GetFishRarityName(fishid, dest[], len = sizeof(dest))
{
    if (fishid < 0 || fishid >= sizeof(FishData))
    {
        format(dest, len, "Unknown");
        return 0;
    }

    switch(FishData[fishid][fishRarity]) {
        case FISH_COMMON: format(dest, len, "Common");
        case FISH_RARE: format(dest, len, "Rare");
        case FISH_EPIC: format(dest, len, "Epic");
        case FISH_LEGENDARY: format(dest, len, "Legendary");
        default: format(dest, len, "Unknown");
    }
    return 1;
}

stock GetFishPrice(fishid, Float:weight = 1.0)
{
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 0;

    return FishData[fishid][fishPrice] * floatround(weight);
}

stock e_fish_rarity:GetFishRarity(fishid)
{
    if (fishid < 0 || fishid >= sizeof(FishData))
        return FISH_COMMON;

    return FishData[fishid][fishRarity];
}

stock GetFishEXP(fishid)
{
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 0;

    return FishData[fishid][fishExp];
}

stock e_fish_rarity:GetRandomRarity(rod_level)
{
    new chance = random(100);
    if(rod_level == 1) {
        if (chance < 80)
            return FISH_COMMON;

        if (chance < 95)
            return FISH_RARE;

        if (chance < 99)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 2) {
        if (chance < 60)
            return FISH_COMMON;

        if (chance < 85)
            return FISH_RARE;

        if (chance < 97)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 3) {
        if (chance < 40)
            return FISH_COMMON;

        if (chance < 70)
            return FISH_RARE;

        if (chance < 90)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 4) {
        if (chance < 20)
            return FISH_COMMON;

        if (chance < 50)
            return FISH_RARE;

        if (chance < 80)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 5) {
        if (chance < 10)
            return FISH_COMMON;

        if (chance < 30)
            return FISH_RARE;

        if (chance < 70)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level == 6) {
        if (chance < 5)
            return FISH_COMMON;

        if (chance < 20)
            return FISH_RARE;

        if (chance < 60)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    if(rod_level >= 7) {
        if (chance < 1)
            return FISH_COMMON;

        if (chance < 10)
            return FISH_RARE;

        if (chance < 50)
            return FISH_EPIC;

        return FISH_LEGENDARY;
    }
    return FISH_COMMON;
}

stock GetRandomFish(rod_level)
{
    new e_fish_rarity:rarity = GetRandomRarity(rod_level);
    new list[sizeof(FishData)];
    new count;

    for (new i; i < sizeof(FishData); i++)
    {
        if (FishData[i][fishRarity] == rarity)
        {
            list[count++] = i;
        }
    }

    if (count == 0)
        return -1;

    return list[random(count)];
}

stock bool:IsValidFish(fishid)
{
    return (0 <= fishid < sizeof(FishData));
}

stock Float:GetRandomFishWeight(fishid)
{
    if (fishid < 0 || fishid >= sizeof(FishData))
        return 0.0;

    new Float:minWeight = FishData[fishid][fishMinWeight];
    new Float:maxWeight = FishData[fishid][fishMaxWeight];

    return RandomFloatMinMax(minWeight, maxWeight);
}

stock ShowInfoAllFish(playerid) {
    new dialog[4096];

    format(dialog, sizeof(dialog),
        "Ten ca\tDo hiem\tCan nang\tGia ban\n");

    for(new i; i < sizeof(FishData); i++)
    {
        new rarity[16];
        GetFishRarityName(i, rarity);

        new minValue = FishData[i][fishMinWeight] * FishData[i][fishPrice];
        new maxValue = FishData[i][fishMaxWeight] * FishData[i][fishPrice];

        format(dialog, sizeof(dialog),
            "%s%s\t{33CCFF}%s\t{CC33FF}%d - %d kg\t{FFD700}$%d - $%d {66FF66}(+%d exp)\n",
            dialog,
            FishData[i][fishName],
            rarity,
            FishData[i][fishMinWeight],
            FishData[i][fishMaxWeight],
            minValue,
            maxValue,
            GetFishEXP(i)
        );
    }

    ShowPlayerDialog(playerid,
        DIALOG_NOTHING,
        DIALOG_STYLE_TABLIST_HEADERS,
        "Thong tin cac loai ca",
        dialog,
        "Dong", "");
    return 1;
}