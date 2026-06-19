#include <YSI_Coding\y_hooks>

#define MAX_ITEMS 50
#define ITEMS_FILE "items.csv"

enum
{
    ITEM_USE_NONE,

    ITEM_USE_EAT,
    ITEM_USE_DRINK,

    ITEM_USE_PHONE,

    ITEM_USE_HOUSE_KEY,
    ITEM_USE_VEHICLE_KEY,

    ITEM_USE_WEAPON
};

enum E_ITEM_DATA
{
    bool:itemExists,
    itemName[32],
    itemType,
    Float:itemWeight,
    itemMaxStack,
    itemPrice,
    itemModelID,
    itemDescription[128]
};

new ItemData[MAX_ITEMS][E_ITEM_DATA];
new TotalItems;

stock CreateItemsFile()
{
    new File:f = fopen(ITEMS_FILE, io_write);

    if(!f)
        return 0;

    fwrite(f,
        "ID,Name,Type,Weight,MaxStack,Price,ModelID,Description\r\n"
    );
    fclose(f);
    return 1;
}

stock SaveItems()
{
    new File:f = fopen(ITEMS_FILE, io_write);

    if(!f)
        return 0;

    fwrite(f,
        "ID,Name,Type,Weight,MaxStack,Price,ModelID,Description\r\n"
    );

    new line[256];

    for(new i = 1; i < MAX_ITEMS; i++)
    {
        if(!ItemData[i][itemExists])
            continue;

        format(
            line,
            sizeof(line),
            "%d,%s,%d,%.2f,%d,%d,%d,%s\r\n",
            i,
            ItemData[i][itemName],
            ItemData[i][itemType],
            ItemData[i][itemWeight],
            ItemData[i][itemMaxStack],
            ItemData[i][itemPrice],
            ItemData[i][itemModelID],
            ItemData[i][itemDescription]
        );

        fwrite(f, line);
    }

    fclose(f);

    printf("[ITEMS] Saved %d items.", TotalItems);

    return 1;
}

stock LoadItems()
{
    new File:f;
    new line[256];

    f = fopen(ITEMS_FILE, io_read);

    if(!f)
    {
        printf("[ITEMS] items.csv not found.");
        CreateItemsFile();

        f = fopen(ITEMS_FILE, io_read);

        if(!f)
            return 0;
    }

    for(new i; i < MAX_ITEMS; i++)
    {
        ItemData[i][itemExists] = false;
    }

    TotalItems = 0;

    // bỏ header
    fread(f, line);

    while(fread(f, line))
    {
        if(strlen(line) < 5)
            continue;

        new
            id,
            name[32],
            type,
            Float:weight,
            maxstack,
            price,
            modelid,
            description[128];

        if(sscanf(
            line,
            "p<,>ds[32]dfddds[128]",
            id,
            name,
            type,
            weight,
            maxstack,
            price,
            modelid,
            description
        ))
        {
            printf(
                "[ITEMS] Invalid line: %s",
                line
            );
            continue;
        }

        if(id < 1 || id >= MAX_ITEMS)
            continue;

        ItemData[id][itemExists] = true;

        format(
            ItemData[id][itemName],
            32,
            "%s",
            name
        );

        ItemData[id][itemType] = type;
        ItemData[id][itemWeight] = weight;
        ItemData[id][itemMaxStack] = maxstack;
        ItemData[id][itemPrice] = price;
        ItemData[id][itemModelID] = modelid;
        ItemData[id][itemDescription] = description;

        TotalItems++;
    }

    fclose(f);

    printf(
        "[ITEMS] Loaded %d items.",
        TotalItems
    );

    return 1;
}

stock CreateItem(
    const name[],
    type,
    Float:weight,
    maxstack,
    price,
    modelid,
    const description[]
)
{
    for(new i = 1; i < MAX_ITEMS; i++)
    {
        if(ItemData[i][itemExists])
            continue;

        ItemData[i][itemExists] = true;

        format(
            ItemData[i][itemName],
            32,
            "%s",
            name
        );

        ItemData[i][itemType] = type;
        ItemData[i][itemWeight] = weight;
        ItemData[i][itemMaxStack] = maxstack;
        ItemData[i][itemPrice] = price;
        ItemData[i][itemModelID] = modelid;
        format(ItemData[i][itemDescription], 128, "%s", description);

        TotalItems++;

        SaveItems();

        return i;
    }

    return -1;
}

stock DeleteItem(itemid)
{
    if(itemid < 1 || itemid >= MAX_ITEMS)
        return 0;

    if(!ItemData[itemid][itemExists])
        return 0;

    ItemData[itemid][itemExists] = false;

    TotalItems--;

    SaveItems();

    return 1;
}

stock ShowItemsDialog(playerid)
{
    new
        str[4096],
        line[256];

    strcat(
        str,
        "ID\tName\tPrice\tStack\tDescription\n"
    );

    for(new i = 1; i < MAX_ITEMS; i++)
    {
        if(!ItemData[i][itemExists])
            continue;

        format(
            line,
            sizeof(line),
            "%d\t%s\t$%d\t%d\t%s\n",
            i,
            ItemData[i][itemName],
            ItemData[i][itemPrice],
            ItemData[i][itemMaxStack],
            ItemData[i][itemDescription]
        );

        strcat(str, line);
    }

    ShowPlayerDialog(
        playerid,
        DIALOG_ITEMS,
        DIALOG_STYLE_TABLIST_HEADERS,
        "Item Manager",
        str,
        "Select",
        "Close"
    );

    return 1;
}

CMD:items(playerid, params[])
{
    ShowItemsDialog(playerid);
    return 1;
}

CMD:createitem(playerid, params[])
{
    new
        name[32],
        type,
        Float:weight,
        stack,
        price,
        modelid,
        description[128];

    if(sscanf(
        params,
        "s[32]dfddds[128]",
        name,
        type,
        weight,
        stack,
        price,
        modelid,
        description
    ))
    {
        SendClientMessage(
            playerid,
            -1,
            "/createitem [name] [type] [weight] [stack] [price] [model] [description]"
        );
        return 1;
    }

    new itemid = CreateItem(
        name,
        type,
        weight,
        stack,
        price,
        modelid,
        description
    );

    new msg[128];

    format(
        msg,
        sizeof(msg),
        "Created item ID %d",
        itemid
    );

    SendClientMessage(
        playerid,
        -1,
        msg
    );

    return 1;
}

CMD:deleteitem(playerid, params[])
{
    new itemid;

    if(sscanf(params, "d", itemid))
        return SendClientMessage(
            playerid,
            -1,
            "/deleteitem [id]"
        );

    if(!DeleteItem(itemid))
        return SendClientMessage(
            playerid,
            -1,
            "Invalid item."
        );

    SendClientMessage(
        playerid,
        -1,
        "Item deleted."
    );

    return 1;
}