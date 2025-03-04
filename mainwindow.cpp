#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QLabel>
#include <QToolBar>
#include <QBoxLayout>
#include <QFileDialog>

#include <fstream>
#include <iostream>

#include "saveslotdisplay.h"

MainWindow *mainWindow;

// Using this define to get the offsets for each widgets value more easily
#define SAVESLOTNULL &((struct SaveSlot *)nullptr)

// Static save slot and file size, probably should make this based on the SaveSlot class, but oh well
const int saveSlotCount = 30;
const int saveSlotSize = 0x172A0;
const int saveFileSize = (saveSlotSize * saveSlotCount);

// Clear out the first save slot list and initialize it as completely empty
void MainWindow::onNewSaveFile()
{
    currentSaveSlot = nullptr;

    ui->saveSlotEditTabs->setDisabled(true);

    while(!pSaveSlotListContent->m_pBoxLayout->isEmpty())
    {
        delete pSaveSlotListContent->m_pBoxLayout->takeAt(0)->widget();
    }

    memset(saveSlots, 0, saveFileSize);

    for (int i = 0 ; i < 30 ; i++)
    {
        new SaveSlotDisplay(pSaveSlotListContent, &(saveSlots[i]), i + 1);
    }
}

// Save File Opening and saving for both of the save files
// (I honestly could have just made it take in all needed arguments as a parameter instead of duplicated functions but oh well, here we are)
void MainWindow::onOpenSaveFile()
{
    while(!pSaveSlotListContent->m_pBoxLayout->isEmpty())
    {
        delete pSaveSlotListContent->m_pBoxLayout->takeAt(0)->widget();
    }

    saveFilePath = QFileDialog::getOpenFileName(this, tr("Open Savefile"), "/home", tr("Save Files (OKAMI)"));
    if (saveFilePath == "") return;

    QFile okamiSave(saveFilePath);
    if(okamiSave.size() == saveFileSize)
    {
        currentSaveSlot = nullptr;

        ui->saveSlotEditTabs->setDisabled(true);

        std::ifstream infile(saveFilePath.toStdString(), std::ifstream::binary);
        infile.read((char*)saveSlots, saveFileSize);
        infile.close();

        for (int i = 0 ; i < 30 ; i++)
        {
            new SaveSlotDisplay(pSaveSlotListContent, &(saveSlots[i]), i + 1);
        }
        okamiSave.close();
        return;
    }
    else
    {
        okamiSave.close();
        std::cout << "invalid file size, must be " << saveFileSize << " bytes" << std::endl;
        return; //invalid file size, must be 2846400 bytes
    }
}
void MainWindow::onSaveSaveFile()
{
    if (!pSaveSlotListContent->m_pBoxLayout->count())
    {
        std::cout << "No Save File Loaded." << std::endl;
        return;
    }

    if (currentSaveSlot) emit SaveValuesFromEditor(currentSaveSlot);

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    saveFilePath = dialog.getSaveFileName(this, tr("Save Savefile"), "/home/", tr("Save Files (OKAMI)"));

    std::ofstream outfile(saveFilePath.toStdString(), std::ofstream::binary);

    for (int i = 0; i < 30; i++)
    {
        SaveSlot* saveSlotAddress = ((SaveSlotDisplay *)(pSaveSlotListContent->m_pBoxLayout->layout()->itemAt(i)->widget()))->m_pSaveSlot;

        saveSlotAddress->checksum = GenerateChecksum((unsigned long long*)saveSlotAddress);

        outfile.write((char*)saveSlotAddress, 94880);
    }
    outfile.close();
}
void MainWindow::onOpenSaveFile2()
{
    while(!pSaveSlotListContent2->m_pBoxLayout->isEmpty())
    {
        delete pSaveSlotListContent2->m_pBoxLayout->takeAt(0)->widget();
    }

    saveFilePath = QFileDialog::getOpenFileName(this, tr("Open Savefile"), "/home", tr("Save Files (OKAMI)"));
    if (saveFilePath == "") return;

    QFile okamiSave(saveFilePath);
    if(okamiSave.size() == saveFileSize)
    {
        currentSaveSlot2 = nullptr;

        std::ifstream infile(saveFilePath.toStdString(), std::ifstream::binary);
        infile.read((char*)saveSlots2, saveFileSize);
        infile.close();

        for (int i = 0 ; i < 30 ; i++)
        {
            new SaveSlotDisplay(pSaveSlotListContent2, &(saveSlots2[i]), i + 1);
        }
        ui->saveSlotList2->setEnabled(true);
        okamiSave.close();
        return;
    }
    else
    {
        okamiSave.close();
        std::cout << "invalid file size, must be " << saveFileSize << " bytes" << std::endl;
        return; //invalid file size, must be 2846400 bytes
    }

}
void MainWindow::onSaveSaveFile2()
{
    if (!pSaveSlotListContent2->m_pBoxLayout->count())
    {
        std::cout << "No Save File Loaded." << std::endl;
        return;
    }

    if (currentSaveSlot2) emit SaveValuesFromEditor(currentSaveSlot2);

    QFileDialog dialog(this);
    dialog.setFileMode(QFileDialog::AnyFile);

    saveFilePath = dialog.getSaveFileName(this, tr("Save Savefile"), "/home/", tr("Save Files (OKAMI)"));

    std::ofstream outfile(saveFilePath.toStdString(), std::ofstream::binary);

    for (int i = 0; i < 30; i++)
    {
        SaveSlot* saveSlotAddress = ((SaveSlotDisplay *)(pSaveSlotListContent2->m_pBoxLayout->layout()->itemAt(i)->widget()))->m_pSaveSlot;

        saveSlotAddress->checksum = GenerateChecksum((unsigned long long*)saveSlotAddress);

        outfile.write((char*)saveSlotAddress, 94880);
    }
    outfile.close();
}

// Changing the state and current view
void MainWindow::onOpenEditor()
{
    currentState = Editor;
    ui->saveSlotEditTabs->show();
    ui->saveSlotList2->hide();
    emit StateUpdate(Editor);
}
void MainWindow::onOpenSlotSwap()
{
    currentState = SlotSwap;
    ui->saveSlotEditTabs->hide();
    ui->saveSlotList2->show();
    emit StateUpdate(SlotSwap);
}

// Change the currently selected save slot
void MainWindow::onSaveSlotSelect(SaveSlot* selectedSaveSlot)
{
    emit SaveSlotChanged();

    if (currentSaveSlot) emit SaveValuesFromEditor(currentSaveSlot);

    std::cout << "Test" << std::endl;

    currentSaveSlot = selectedSaveSlot;

    emit LoadValuesToEditor(currentSaveSlot);

    ui->saveSlotEditTabs->setDisabled(false);
}

// Handling Attaching and Detaching from the game. Used for functionality in areainfowidget.cpp.
// The GameProcessHandler also gets deleted there if the button is pressed with the handle invalid.
void MainWindow::onAttachToGame()
{
    if (!gameProcessHandler)
    {
        gameProcessHandler = new GameProcessHandler();
        if (gameProcessHandler->m_processID)
        {
            emit SuccesfullyAttachedToOkami(gameProcessHandler);
        }
        else delete gameProcessHandler;
    }
}
void MainWindow::onDetachFromGame()
{
    if (gameProcessHandler)
    {
        delete gameProcessHandler;
        gameProcessHandler = nullptr;
        emit SuccesfullyDetachedFromOkami();
    }
}

// Checksum algorithm used by the game. Essentially just copied the pseudo code from Ghidra, need to clean this up at some point
unsigned long long MainWindow::GenerateChecksum(unsigned long long* param_1)
{
        unsigned long long *puVar1;
        unsigned long long *puVar2;
        unsigned long long uVar3;
        unsigned long long *puVar4;
        unsigned long long uVar5;
        unsigned long long uVar6;
        unsigned long long uVar7;
        unsigned long long uVar8;
        unsigned long long uVar9;
        unsigned long long uVar10;

        uVar5 = 0;
        uVar3 = 0x9be6fa3b72afda1d;
        puVar4 = param_1;
        for (uVar6 = (unsigned long long)(param_1 <= param_1 + 1); uVar6 != 0; uVar6 = uVar6 - 1) {
            uVar3 = uVar3 ^ *puVar4;
            puVar4 = puVar4 + 1;
        }
        puVar4 = param_1 + 2;
        uVar6 = 0x2e52;
        if (param_1 + 0x2e54 < puVar4) {
            uVar6 = uVar5;
        }
        if (uVar6 != 0) {
            if (puVar4 <= param_1 + 0x2e54) {
                uVar7 = 0;
                uVar8 = 0;
                uVar9 = uVar7;
                uVar10 = uVar8;
                do {
                    uVar5 = uVar5 + 4;
                    uVar7 = *puVar4 ^ uVar7;
                    uVar8 = puVar4[1] ^ uVar8;
                    puVar1 = puVar4 + 2;
                    puVar2 = puVar4 + 3;
                    puVar4 = puVar4 + 4;
                    uVar9 = *puVar1 ^ uVar9;
                    uVar10 = *puVar2 ^ uVar10;
                } while (uVar5 < (uVar6 & 0xfffffffffffffffc));
                uVar3 = uVar3 ^ uVar9 ^ uVar7 ^ uVar10 ^ uVar8;
            }
            for (; uVar5 < uVar6; uVar5 = uVar5 + 1) {
                uVar3 = uVar3 ^ *puVar4;
                puVar4 = puVar4 + 1;
            }
        }
        return uVar3;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mainWindow = this;
    this->setWindowTitle("Origin Editor");
    //this->setWindowIcon();

    //hiding the second list here to have the intial state be the "Editor"
    ui->saveSlotList2->hide();

    // setting position manually cuz it was behaving weirdly and kept starting out at the top of the screen after I added the second save slot list
    this->setGeometry(this->geometry().x() + 100.0, this->geometry().y() + 150.0, this->geometry().width(), this->geometry().height());

    // Allocating our memory buffers for the two save files we will be loading
    saveSlots = (SaveSlot*)malloc(saveFileSize);
    saveSlots2 = (SaveSlot*)malloc(saveFileSize);

    // connecting all menu actions
    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::onNewSaveFile);
    connect(ui->actionOpen_1, &QAction::triggered, this, &MainWindow::onOpenSaveFile);
    connect(ui->actionSave_1, &QAction::triggered, this, &MainWindow::onSaveSaveFile);

    connect(ui->actionOpen_2, &QAction::triggered, this, &MainWindow::onOpenSaveFile2);
    connect(ui->actionSave_2, &QAction::triggered, this, &MainWindow::onSaveSaveFile2);

    connect(ui->actionAttach_to_Game, &QAction::triggered, this, &MainWindow::onAttachToGame);
    connect(ui->actionDetach_from_Game, &QAction::triggered, this, &MainWindow::onDetachFromGame);

    connect(ui->actionEditor, &QAction::triggered, this, &MainWindow::onOpenEditor);
    connect(ui->actionSave_Slot_Exchange, &QAction::triggered, this, &MainWindow::onOpenSlotSwap);

    InitializeWidgets();
}

MainWindow::~MainWindow()
{
    free(saveSlots);
    delete ui;
}

// This function swaps the slots from one list to the other. Gets called by the Saveslots themselves when they get dropped on in the Slotswap state
void MainWindow::SwapSlots(int listIndex1, int listIndex2)
{
    QWidget *widget1;
    QWidget *widget2;

    widget1 = pSaveSlotListContent->m_pBoxLayout->takeAt(listIndex1)->widget();
    widget2 = pSaveSlotListContent2->m_pBoxLayout->takeAt(listIndex2)->widget();

    widget1->setParent(pSaveSlotListContent2);
    widget2->setParent(pSaveSlotListContent);

    pSaveSlotListContent->m_pBoxLayout->insertWidget(listIndex1, widget2);
    pSaveSlotListContent2->m_pBoxLayout->insertWidget(listIndex2, widget1);
}

// Initializing all runtime widgets, which each use offsets to access their revelant data in the currently selected Saveslot
void MainWindow::InitializeWidgets()
{
    pSaveFileInfoWidget = new SaveFileInfoWidget(ui->GeneralInformation);
    pAreaInfoWidget = new AreaInfoWidget(ui->GeneralInformation);
    pSaveSlotListContent = new SaveSlotListContent(ui->saveSlotListBase);
    pSaveSlotListContent2 = new SaveSlotListContent(ui->saveSlotListBase2);

    //connecting both Saveslot lists to the View dropdown options so they can change their functionality depending on the state of the save editor
    connect(this, &MainWindow::StateUpdate, pSaveSlotListContent, &SaveSlotListContent::updateState);
    connect(this, &MainWindow::StateUpdate, pSaveSlotListContent2, &SaveSlotListContent::updateState);

    // Consumables
    consumableItemList.append(new ItemWidget(ui->feedBagGroupBox,          tr("Feedbag (Seeds)"     ), (size_t)(SAVESLOTNULL->inventory.feedbagSeeds)));
    consumableItemList.append(new ItemWidget(ui->feedBagGroupBox,          tr("Feedbag (Herbs)"     ), (size_t)(SAVESLOTNULL->inventory.feedbagHerbs)));
    consumableItemList.append(new ItemWidget(ui->feedBagGroupBox,          tr("Feedbag (Fish)"      ), (size_t)(SAVESLOTNULL->inventory.feedbagFish)));
    consumableItemList.append(new ItemWidget(ui->feedBagGroupBox,          tr("Feedbag (Meat)"      ), (size_t)(SAVESLOTNULL->inventory.feedbagMeat)));
    consumableItemList.append(new ItemWidget(ui->holyBonesGroupBox,        tr("Holy Bone S"         ), (size_t)(SAVESLOTNULL->inventory.holyBoneS)));
    consumableItemList.append(new ItemWidget(ui->holyBonesGroupBox,        tr("Holy Bone M"         ), (size_t)(SAVESLOTNULL->inventory.holyBoneM)));
    consumableItemList.append(new ItemWidget(ui->holyBonesGroupBox,        tr("Holy Bone L"         ), (size_t)(SAVESLOTNULL->inventory.holyBoneL)));
    consumableItemList.append(new ItemWidget(ui->exorcismSlipsGroupBox,    tr("Exorcism Slip S"     ), (size_t)(SAVESLOTNULL->inventory.exorcismSlipS)));
    consumableItemList.append(new ItemWidget(ui->exorcismSlipsGroupBox,    tr("Exorcism Slip M"     ), (size_t)(SAVESLOTNULL->inventory.exorcismSlipM)));
    consumableItemList.append(new ItemWidget(ui->exorcismSlipsGroupBox,    tr("Exorcism Slip L"     ), (size_t)(SAVESLOTNULL->inventory.exorcismSlipL)));
    consumableItemList.append(new ItemWidget(ui->utilityGroupBox,          tr("Vengeance Slip"      ), (size_t)(SAVESLOTNULL->inventory.vengeanceSlip)));
    consumableItemList.append(new ItemWidget(ui->utilityGroupBox,          tr("Inkfinity Stone"     ), (size_t)(SAVESLOTNULL->inventory.inkfinityStone)));
    consumableItemList.append(new ItemWidget(ui->charmsGroupBox,           tr("Traveler's Charm"    ), (size_t)(SAVESLOTNULL->inventory.travelersCharm)));
    consumableItemList.append(new ItemWidget(ui->charmsGroupBox,           tr("Godly Charm"         ), (size_t)(SAVESLOTNULL->inventory.godlyCharm)));
    consumableItemList.append(new ItemWidget(ui->sakeGroupBox,             tr("Steel Fist Sake"     ), (size_t)(SAVESLOTNULL->inventory.steelFistSake)));
    consumableItemList.append(new ItemWidget(ui->sakeGroupBox,             tr("Steel Soul Sake"     ), (size_t)(SAVESLOTNULL->inventory.steelSoulSake)));
    consumableItemList.append(new ItemWidget(ui->specialGroupBox,          tr("Gold Dust"           ), (size_t)(SAVESLOTNULL->inventory.goldDust)));
    consumableItemList.append(new ItemWidget(ui->specialGroupBox,          tr("Mermaid Coin"        ), (size_t)(SAVESLOTNULL->inventory.mermaidCoin)));
    consumableItemList.append(new ItemWidget(ui->specialGroupBox,          tr("Golden Peach"        ), (size_t)(SAVESLOTNULL->inventory.goldenPeach)));

    // Karmic Transformers
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Returner"     ), (size_t)(SAVESLOTNULL->inventory.karmicReturner)));
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Transformer 1"), (size_t)(SAVESLOTNULL->inventory.karmicTransformer1)));
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Transformer 2"), (size_t)(SAVESLOTNULL->inventory.karmicTransformer2)));
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Transformer 3"), (size_t)(SAVESLOTNULL->inventory.karmicTransformer3)));
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Transformer 4"), (size_t)(SAVESLOTNULL->inventory.karmicTransformer4)));
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Transformer 5"), (size_t)(SAVESLOTNULL->inventory.karmicTransformer5)));
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Transformer 6"), (size_t)(SAVESLOTNULL->inventory.karmicTransformer6)));
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Transformer 7"), (size_t)(SAVESLOTNULL->inventory.karmicTransformer7)));
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Transformer 8"), (size_t)(SAVESLOTNULL->inventory.karmicTransformer8)));
    karmicTransformList.append(new ItemWidget(ui->karmicTransformerWidget, tr("Karmic Transformer 9"), (size_t)(SAVESLOTNULL->inventory.karmicTransformer9)));

    // Key Items
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Canine Tracker"      ), (size_t)(SAVESLOTNULL->inventory.canineTracker)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Loyalty Orb"         ), (size_t)(SAVESLOTNULL->inventory.loyaltyOrb)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Justice Orb"         ), (size_t)(SAVESLOTNULL->inventory.justiceOrb)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Duty Orb"            ), (size_t)(SAVESLOTNULL->inventory.dutyOrb)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Golden Mushroom"     ), (size_t)(SAVESLOTNULL->inventory.goldenMushroom)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Pinwheel"            ), (size_t)(SAVESLOTNULL->inventory.pinwheel)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Thunder Brew"        ), (size_t)(SAVESLOTNULL->inventory.thunderBrew)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Imp Mask"            ), (size_t)(SAVESLOTNULL->inventory.impMask)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Ogre Liver"          ), (size_t)(SAVESLOTNULL->inventory.ogreLiver)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Lips of Ice"         ), (size_t)(SAVESLOTNULL->inventory.lipsOfIce)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Eyeball of Fire"     ), (size_t)(SAVESLOTNULL->inventory.eyeballOfFire)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Black Demon Horn"    ), (size_t)(SAVESLOTNULL->inventory.blackDemonHorn)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Blinding Snow"       ), (size_t)(SAVESLOTNULL->inventory.blindingSnow)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Marlin Rod"          ), (size_t)(SAVESLOTNULL->inventory.marlinRod)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Herbal Medicine"     ), (size_t)(SAVESLOTNULL->inventory.herbalMedicine)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Charcoal"            ), (size_t)(SAVESLOTNULL->inventory.charcoal)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Lucky Mallet"        ), (size_t)(SAVESLOTNULL->inventory.luckyMallet)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Border Key"          ), (size_t)(SAVESLOTNULL->inventory.borderKey)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Shell Amulet"        ), (size_t)(SAVESLOTNULL->inventory.shellAmulet)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Dragon Orb"          ), (size_t)(SAVESLOTNULL->inventory.dragonOrb)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Fox Rods"            ), (size_t)(SAVESLOTNULL->inventory.foxRods)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Treasure Box"        ), (size_t)(SAVESLOTNULL->inventory.treasureBox)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Gimmick Gear"        ), (size_t)(SAVESLOTNULL->inventory.gimmickGear)));
    keyItemList.append(new ItemWidget(ui->keyItemsWidget,                  tr("Sewaprolo"           ), (size_t)(SAVESLOTNULL->inventory.sewaprolo)));

    // Treasures
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("White Porcelain Pot" ), (size_t)(SAVESLOTNULL->inventory.whitePorcelainPot)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Kutani Pottery"      ), (size_t)(SAVESLOTNULL->inventory.kutaniPottery)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Incense Burner"      ), (size_t)(SAVESLOTNULL->inventory.incenseBurner)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Vase"                ), (size_t)(SAVESLOTNULL->inventory.vase)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Silver Pocket Watch" ), (size_t)(SAVESLOTNULL->inventory.silverPocketWatch)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Rat Statue"          ), (size_t)(SAVESLOTNULL->inventory.ratStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Bull Horn"           ), (size_t)(SAVESLOTNULL->inventory.bullHorn)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Etched Glass"        ), (size_t)(SAVESLOTNULL->inventory.etchedGlass)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Lacquerware Set"     ), (size_t)(SAVESLOTNULL->inventory.lacquerwareSet)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Wooden Bear"         ), (size_t)(SAVESLOTNULL->inventory.woodenBear)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Glass Beads"         ), (size_t)(SAVESLOTNULL->inventory.glassBeads)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Dragonfly Bead"      ), (size_t)(SAVESLOTNULL->inventory.dragonflyBead)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Coral Fragment"      ), (size_t)(SAVESLOTNULL->inventory.coralFragment)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Crystal"             ), (size_t)(SAVESLOTNULL->inventory.crystal)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Pearl"               ), (size_t)(SAVESLOTNULL->inventory.pearl)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Ruby Tassels"        ), (size_t)(SAVESLOTNULL->inventory.rubyTassels)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Bull Statue"         ), (size_t)(SAVESLOTNULL->inventory.bullStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Tiger Statue"        ), (size_t)(SAVESLOTNULL->inventory.tigerStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Rabbit Statue"       ), (size_t)(SAVESLOTNULL->inventory.rabbitStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Dragon Statue"       ), (size_t)(SAVESLOTNULL->inventory.dragonStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Snake Statue"        ), (size_t)(SAVESLOTNULL->inventory.snakeStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Horse Statue"        ), (size_t)(SAVESLOTNULL->inventory.horseStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Sheep Statue"        ), (size_t)(SAVESLOTNULL->inventory.sheepStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Monkey Statue"       ), (size_t)(SAVESLOTNULL->inventory.monkeyStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Rooster Statue"      ), (size_t)(SAVESLOTNULL->inventory.roosterStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Dog Statue"          ), (size_t)(SAVESLOTNULL->inventory.dogStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Boar Statue"         ), (size_t)(SAVESLOTNULL->inventory.boarStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Cat Statue"          ), (size_t)(SAVESLOTNULL->inventory.catStatue)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Sapphire Tassels"    ), (size_t)(SAVESLOTNULL->inventory.sapphireTassels)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Emerald Tassels"     ), (size_t)(SAVESLOTNULL->inventory.emeraldTassels)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Tuequoise Tassels"   ), (size_t)(SAVESLOTNULL->inventory.tuequoiseTassels)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Agate Tassels"       ), (size_t)(SAVESLOTNULL->inventory.agateTassels)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Amber Tassels"       ), (size_t)(SAVESLOTNULL->inventory.amberTassels)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Cats Eye Tassels"    ), (size_t)(SAVESLOTNULL->inventory.catsEyeTassels)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Amethyst Tassels"    ), (size_t)(SAVESLOTNULL->inventory.amethystTassels)));
    treasureItemList.append(new ItemWidget(ui->treasuresWidget,            tr("Jade Tassels"        ), (size_t)(SAVESLOTNULL->inventory.jadeTassels)));

    // Fish
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Giant Salmon"        ), (size_t)(SAVESLOTNULL->inventory.giantSalmon)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Supreme Tuna"        ), (size_t)(SAVESLOTNULL->inventory.supremeTuna)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Mountain Trout"      ), (size_t)(SAVESLOTNULL->inventory.mountainTrout)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Red Snapper"         ), (size_t)(SAVESLOTNULL->inventory.redSnapper)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Striped Snapper"     ), (size_t)(SAVESLOTNULL->inventory.stripedSnapper)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Salmon"              ), (size_t)(SAVESLOTNULL->inventory.salmon)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Koi"                 ), (size_t)(SAVESLOTNULL->inventory.koi)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Huchen"              ), (size_t)(SAVESLOTNULL->inventory.huchen)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Robalo"              ), (size_t)(SAVESLOTNULL->inventory.robalo)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Black Bass"          ), (size_t)(SAVESLOTNULL->inventory.blackBass)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Clown Fish"          ), (size_t)(SAVESLOTNULL->inventory.clownFish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Bonito"              ), (size_t)(SAVESLOTNULL->inventory.bonito)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Yellowtail"          ), (size_t)(SAVESLOTNULL->inventory.yellowtail)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Sweet Fish"          ), (size_t)(SAVESLOTNULL->inventory.sweetfish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Trout"               ), (size_t)(SAVESLOTNULL->inventory.trout)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Smelt"               ), (size_t)(SAVESLOTNULL->inventory.smelt)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Killifish"           ), (size_t)(SAVESLOTNULL->inventory.killifish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Flying Fish"         ), (size_t)(SAVESLOTNULL->inventory.flyingFish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Sturgeon"            ), (size_t)(SAVESLOTNULL->inventory.sturgeon)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Sunfish"             ), (size_t)(SAVESLOTNULL->inventory.sunfish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Freshwater Eel"      ), (size_t)(SAVESLOTNULL->inventory.freshwaterEel)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Loach"               ), (size_t)(SAVESLOTNULL->inventory.loach)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Moray"               ), (size_t)(SAVESLOTNULL->inventory.moray)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Oarfish"             ), (size_t)(SAVESLOTNULL->inventory.oarfish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Monkfish"            ), (size_t)(SAVESLOTNULL->inventory.monkfish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Catfish"             ), (size_t)(SAVESLOTNULL->inventory.catfish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Giant Catfish"       ), (size_t)(SAVESLOTNULL->inventory.giantCatfish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Goby"                ), (size_t)(SAVESLOTNULL->inventory.goby)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Lobster"             ), (size_t)(SAVESLOTNULL->inventory.lobster)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Crawfish"            ), (size_t)(SAVESLOTNULL->inventory.crawfish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Scallop"             ), (size_t)(SAVESLOTNULL->inventory.scallop)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Nautilus"            ), (size_t)(SAVESLOTNULL->inventory.nautilus)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Manta"               ), (size_t)(SAVESLOTNULL->inventory.manta)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Blowfish"            ), (size_t)(SAVESLOTNULL->inventory.blowfish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("River Crab"          ), (size_t)(SAVESLOTNULL->inventory.riverCrab)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Starfish"            ), (size_t)(SAVESLOTNULL->inventory.starfish)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Marlin"              ), (size_t)(SAVESLOTNULL->inventory.marlin)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Loggerhead Turtle"   ), (size_t)(SAVESLOTNULL->inventory.loggerheadTurtle)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Sea Horse"           ), (size_t)(SAVESLOTNULL->inventory.seaHorse)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Octupus"             ), (size_t)(SAVESLOTNULL->inventory.octupus)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Squid"               ), (size_t)(SAVESLOTNULL->inventory.squid)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Whopper"             ), (size_t)(SAVESLOTNULL->inventory.whopper)));
    fishItemList.append(new ItemWidget(ui->fishWidget,                     tr("Cutlass Fish"        ), (size_t)(SAVESLOTNULL->inventory.cutlassFish)));

    // Weapons
    weaponItemList.append(new WeaponWidget(ui->mirrorsGroupBox,            tr("Divine Retribution"  ), (size_t)(SAVESLOTNULL->inventory.divineRetribution), (size_t)(SAVESLOTNULL->mirrorUpgradeLevelArray), 0));
    weaponItemList.append(new WeaponWidget(ui->mirrorsGroupBox,            tr("Snarling Beast"      ), (size_t)(SAVESLOTNULL->inventory.snarlingBeast),     (size_t)(SAVESLOTNULL->mirrorUpgradeLevelArray), 1));
    weaponItemList.append(new WeaponWidget(ui->mirrorsGroupBox,            tr("Infinity Judge"      ), (size_t)(SAVESLOTNULL->inventory.infinityJudge),     (size_t)(SAVESLOTNULL->mirrorUpgradeLevelArray), 2));
    weaponItemList.append(new WeaponWidget(ui->mirrorsGroupBox,            tr("Trinity Mirror"      ), (size_t)(SAVESLOTNULL->inventory.trinityMirror),     (size_t)(SAVESLOTNULL->mirrorUpgradeLevelArray), 3));
    weaponItemList.append(new WeaponWidget(ui->mirrorsGroupBox,            tr("Solar Flare"         ), (size_t)(SAVESLOTNULL->inventory.solarFlare),        (size_t)(SAVESLOTNULL->mirrorUpgradeLevelArray), 4));
    weaponItemList.append(new WeaponWidget(ui->glaivesGroupBox,            tr("Tsumugari"           ), (size_t)(SAVESLOTNULL->inventory.tsumugari),         (size_t)(SAVESLOTNULL->glaiveUpgradeLevelArray), 0));
    weaponItemList.append(new WeaponWidget(ui->glaivesGroupBox,            tr("Seven Strike"        ), (size_t)(SAVESLOTNULL->inventory.sevenStrike),       (size_t)(SAVESLOTNULL->glaiveUpgradeLevelArray), 1));
    weaponItemList.append(new WeaponWidget(ui->glaivesGroupBox,            tr("Blade of Kusanagi"   ), (size_t)(SAVESLOTNULL->inventory.bladeOfKusanagi),   (size_t)(SAVESLOTNULL->glaiveUpgradeLevelArray), 2));
    weaponItemList.append(new WeaponWidget(ui->glaivesGroupBox,            tr("Eigth Wonder"        ), (size_t)(SAVESLOTNULL->inventory.eigthWonder),       (size_t)(SAVESLOTNULL->glaiveUpgradeLevelArray), 3));
    weaponItemList.append(new WeaponWidget(ui->glaivesGroupBox,            tr("Thunder Edge"        ), (size_t)(SAVESLOTNULL->inventory.thunderEdge),       (size_t)(SAVESLOTNULL->glaiveUpgradeLevelArray), 4));
    weaponItemList.append(new WeaponWidget(ui->beadsGroupBox,              tr("Devout Beads"        ), (size_t)(SAVESLOTNULL->inventory.devoutBeads),       (size_t)(SAVESLOTNULL->beadsUpgradeLevelArray),  0));
    weaponItemList.append(new WeaponWidget(ui->beadsGroupBox,              tr("Life Beads"          ), (size_t)(SAVESLOTNULL->inventory.lifeBeads),         (size_t)(SAVESLOTNULL->beadsUpgradeLevelArray),  1));
    weaponItemList.append(new WeaponWidget(ui->beadsGroupBox,              tr("Exorcism Beads"      ), (size_t)(SAVESLOTNULL->inventory.exorcismBeads),     (size_t)(SAVESLOTNULL->beadsUpgradeLevelArray),  2));
    weaponItemList.append(new WeaponWidget(ui->beadsGroupBox,              tr("Resurrection Beads"  ), (size_t)(SAVESLOTNULL->inventory.resurrectionBeads), (size_t)(SAVESLOTNULL->beadsUpgradeLevelArray),  3));
    weaponItemList.append(new WeaponWidget(ui->beadsGroupBox,              tr("Tundra Beads"        ), (size_t)(SAVESLOTNULL->inventory.tundraBeads),       (size_t)(SAVESLOTNULL->beadsUpgradeLevelArray),  4));

    // Holy Artifacts
    holyArtifactList.append(new ItemWidget(ui->holyArtifactsWidget,        tr("Peace Bell"          ), (size_t)(SAVESLOTNULL->inventory.peaceBell)));
    holyArtifactList.append(new ItemWidget(ui->holyArtifactsWidget,        tr("Wood Mat"            ), (size_t)(SAVESLOTNULL->inventory.woodMat)));
    holyArtifactList.append(new ItemWidget(ui->holyArtifactsWidget,        tr("Golden Lucky Cat"    ), (size_t)(SAVESLOTNULL->inventory.goldenLuckyCat)));
    holyArtifactList.append(new ItemWidget(ui->holyArtifactsWidget,        tr("Thief's Glove"       ), (size_t)(SAVESLOTNULL->inventory.thiefsGlove)));
    //holyArtifactList.append(new ItemWidget(ui->holyArtifactsWidget,        tr("Fog Pot"             ), (size_t)(SAVESLOTNULL->inventory.fogPot)));
    holyArtifactList.append(new ItemWidget(ui->holyArtifactsWidget,        tr("Golden Ink Pot"      ), (size_t)(SAVESLOTNULL->inventory.goldenInkPot)));
    holyArtifactList.append(new ItemWidget(ui->holyArtifactsWidget,        tr("Fire Tablet"         ), (size_t)(SAVESLOTNULL->inventory.fireTablet)));
    holyArtifactList.append(new ItemWidget(ui->holyArtifactsWidget,        tr("Water Tablet"        ), (size_t)(SAVESLOTNULL->inventory.waterTablet)));
    holyArtifactList.append(new ItemWidget(ui->holyArtifactsWidget,        tr("String of Beads"     ), (size_t)(SAVESLOTNULL->inventory.stringOfBeads)));
}
