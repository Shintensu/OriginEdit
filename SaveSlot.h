#ifndef SAVESLOT_H
#define SAVESLOT_H

#include <QtTypes>
#include <qcontainerfwd.h>

enum SaveSlotGroups{ GeneralInformation = 0, Stats, Inventory, Unlocks, Other};

struct Inventory
{
    char padding[8];
    quint16 holyBoneL;
    quint16 unknown;
    char padding2[4];
    quint16 exorcismSlipL;
    quint16 exorcismSlipM;
    quint16 exorcismSlipS;
    quint16 peaceBell;
    quint16 vengeanceSlip;
    quint16 inkfinityStone;
    quint16 mermaidCoin;
    quint16 goldenPeach;

    quint16 divineRetribution;
    quint16 snarlingBeast;
    quint16 infinityJudge;
    quint16 trinityMirror;
    quint16 solarFlare;

    quint16 devoutBeads;
    quint16 lifeBeads;
    quint16 exorcismBeads;
    quint16 resurrectionBeads;
    quint16 tundraBeads;

    quint16 tsumugari;
    quint16 sevenStrike;
    quint16 bladeOfKusanagi;
    quint16 eigthWonder;
    quint16 thunderEdge;

    quint16 demonFangs;

    char padding3[68];

    quint16 canineTracker;
    quint16 luckyMallet;
    quint16 borderKey;
    quint16 dragonOrb;
    quint16 foxRods;
    quint16 thunderBrew;
    quint16 shellAmulet;
    quint16 impMask;
    quint16 ogreLiver;
    quint16 lipsOfIce;
    quint16 eyeballOfFire;
    quint16 blackDemonHorn;
    quint16 loyaltyOrb;
    quint16 justiceOrb;
    quint16 dutyOrb;

    char padding4[20];

    quint16 karmicTransformer1;

    char padding5[6];

    quint16 goldenMushroom;
    quint16 gimmickGear;

    quint16 unknown2;

    quint16 purificationSake;
    quint16 sewaprolo;

    char padding6[22];

    quint16 unknown3;

    quint16 travelersCharm;
    quint16 charcoal;
    quint16 blindingSnow;
    quint16 treasureBox;

    quint16 unknown4;

    quint16 herbalMedicine;
    quint16 pinwheel;
    quint16 marlinRod;

    quint16 unknown5;

    quint16 karmicTransformer3;
    quint16 karmicTransformer7;
    quint16 karmicTransformer8;
    quint16 karmicTransformer9;

    char padding7[16];
    quint16 unknown6;
    char padding8[16];

    //char unknown7[228];
    quint16 holyBoneM;
    quint16 holyBoneS;
    quint16 feedbagMeat;
    quint16 feedbagHerbs;
    quint16 feedbagSeeds;
    quint16 feedbagFish;

    quint16 unknown7;

    quint16 goldenLuckyCat;
    quint16 thiefsGlove;
    quint16 woodMat;
    quint16 goldenInkPot;
    quint16 steelFistSake;
    quint16 steelSoulSake;
    quint16 godlyCharm;
    quint16 waterTablet;
    quint16 fireTablet;

    quint16 goldDust;

    quint16 unknown8;

    quint16 whitePorcelainPot;
    quint16 kutaniPottery;

    quint16 unknown9;

    quint16 incenseBurner;
    quint16 vase;
    quint16 silverPocketWatch;
    quint16 ratStatue;
    quint16 bullHorn;

    quint16 unknown10;

    quint16 etchedGlass;
    quint16 lacquerwareSet;
    quint16 woodenBear;

    quint16 unknown11;

    quint16 glassBeads;
    quint16 dragonflyBead;

    quint16 unknown12;

    quint16 coralFragment;
    quint16 crystal;
    quint16 pearl;
    quint16 rubyTassels;
    quint16 bullStatue;
    quint16 tigerStatue;
    quint16 rabbitStatue;
    quint16 dragonStatue;
    quint16 snakeStatue;
    quint16 horseStatue;
    quint16 sheepStatue;
    quint16 monkeyStatue;
    quint16 roosterStatue;
    quint16 dogStatue;
    quint16 boarStatue;
    quint16 catStatue;
    quint16 sapphireTassels;
    quint16 emeraldTassels;
    quint16 tuequoiseTassels;
    quint16 agateTassels;
    quint16 amberTassels;
    quint16 catsEyeTassels;
    quint16 amethystTassels;
    quint16 jadeTassels;

    quint16 karmicReturner;
    quint16 karmicTransformer2;
    quint16 karmicTransformer6;
    quint16 karmicTransformer5;

    quint16 strayBeadCount;
    quint16 stringOfBeads;

    quint16 giantSalmon;

    quint16 karmicTransformer4;

    quint16 supremeTuna;
    quint16 mountainTrout;
    quint16 redSnapper;
    quint16 stripedSnapper;
    quint16 salmon;
    quint16 koi;
    quint16 huchen;
    quint16 robalo;
    quint16 blackBass;
    quint16 clownFish;
    quint16 bonito;
    quint16 yellowtail;
    quint16 sweetfish;
    quint16 trout;
    quint16 smelt;
    quint16 killifish;
    quint16 flyingFish;
    quint16 sturgeon;
    quint16 sunfish;
    quint16 freshwaterEel;
    quint16 loach;
    quint16 moray;
    quint16 oarfish;
    quint16 monkfish;
    quint16 catfish;
    quint16 giantCatfish;
    quint16 goby;
    quint16 lobster;
    quint16 crawfish;
    quint16 scallop;
    quint16 nautilus;
    quint16 manta;
    quint16 blowfish;
    quint16 riverCrab;
    quint16 starfish;
    quint16 marlin;
    quint16 loggerheadTurtle;
    quint16 seaHorse;
    quint16 octupus;
    quint16 squid;

    char padding9[12];

    quint16 whopper;
    quint16 cutlassFish;
};

struct SaveSlot{
    char header[4]; // Offset 0x0
    quint32 areaNameID; // Offset 0x4

    unsigned long long checksum; // Offset 0x8
    unsigned long long timeRTC; // Offset 0x10

    char unknown[8]; // Offset 0x18

    char padding[2]; // Offset 0x20
    qint16 currentPraise; // Offset 0x22
    qint16 totalPraise; // Offset 0x24
    char padding2[2]; // Offset 0x26

    char unknown0[16]; // Offset 0x28

    short mirrorUpgradeLevelArray; // Offset 0x38
    short glaiveUpgradeLevelArray; // Offset 0x3A
    short beadsUpgradeLevelArray; // Offset 0x3C
    short padding3; // Offset 0x3E

    char unknown1[8]; // Offset 0x40

    float x,y,z; // Offset 0x48
    float u,v,w; // Offset 0x54

    char unknown3[124]; // Offset 0x60

    quint32 timePlayed; // Offset 0xDC
    qint16 numberOfSaves; // Offset 0xE0
    quint8 currentAreaID; // Offset 0xE2
    quint8 currentRegionID; // Offset 0xE2
    quint8 lastAreaID; // Offset 0xE4
    quint8 lastRegionID; // Offset 0xE4
    char padding4[2]; // Offset 0xE6

    char unknown4[8]; // Offset 0xE8

    quint32 money; // Offset 0xF0
    quint8 purseUpgradeCount; // Offset 0xF4
    char padding5[2]; // Offset 0xF6

    char unknown6[72]; // Appears to be some kind of bitflag array | Offset 0xF8

    struct Inventory inventory;

    quint64 timeOfDay;

    char unknown8[21624];
    quint64 timeOfDay2;
    char unknown9[72408];
};

#endif // SAVESLOT_H
