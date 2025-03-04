#include "areainfowidget.h"
#include "ui_areainfowidget.h"

#include "mainwindow.h"

#include <QVector>
#include <QDoubleSpinBox>
#include <QDial>

#include <math.h>

#define SAVESLOTNULL &((struct SaveSlot *)nullptr)

// Widget containing information pertaining to the players starting location.
// After attaching to the game, allows to load the values from and to the game.
AreaInfoWidget::AreaInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::AreaInfoWidget)
{
    ui->setupUi(this);

    parent->layout()->addWidget(this);

    InitializeComboBoxes();

    connect(mainWindow, &MainWindow::LoadValuesToEditor, this, &AreaInfoWidget::ReadValues);
    connect(mainWindow, &MainWindow::SaveValuesFromEditor, this, &AreaInfoWidget::WriteValues);

    connect(ui->uSpinBox, &QDoubleSpinBox::valueChanged, this, &AreaInfoWidget::UpdateDials);
    connect(ui->vSpinBox, &QDoubleSpinBox::valueChanged, this, &AreaInfoWidget::UpdateDials);
    connect(ui->wSpinBox, &QDoubleSpinBox::valueChanged, this, &AreaInfoWidget::UpdateDials);

    connect(ui->uDial, &QDial::valueChanged, this, &AreaInfoWidget::UpdateSpinBoxes);
    connect(ui->vDial, &QDial::valueChanged, this, &AreaInfoWidget::UpdateSpinBoxes);
    connect(ui->wDial, &QDial::valueChanged, this, &AreaInfoWidget::UpdateSpinBoxes);

    connect(ui->curRegionIDSpinBox, &QSpinBox::valueChanged, this, &AreaInfoWidget::ChangeComboModel);

    connect(ui->curRegionIDSpinBox, &QSpinBox::valueChanged, this, &AreaInfoWidget::UpdateRegionComboBoxIndex);
    connect(ui->curRegionComboBox, &QComboBox::currentIndexChanged, this, &AreaInfoWidget::UpdateCurrentRegionID);

    connect(ui->curAreaIDSpinBox, &QSpinBox::valueChanged, this, &AreaInfoWidget::UpdateAreaComboBoxIndex);
    connect(ui->curAreaComboBox, &QComboBox::currentIndexChanged, this, &AreaInfoWidget::UpdateCurrentAreaID);


    connect(ui->lastRegionIDSpinBox, &QSpinBox::valueChanged, this, &AreaInfoWidget::ChangeComboModelLast);

    connect(ui->lastRegionIDSpinBox, &QSpinBox::valueChanged, this, &AreaInfoWidget::UpdateLastRegionComboBoxIndex);
    connect(ui->lastRegionComboBox, &QComboBox::currentIndexChanged, this, &AreaInfoWidget::UpdateLastRegionID);

    connect(ui->lastAreaIDSpinBox, &QSpinBox::valueChanged, this, &AreaInfoWidget::UpdateLastAreaComboBoxIndex);
    connect(ui->lastAreaComboBox, &QComboBox::currentIndexChanged, this, &AreaInfoWidget::UpdateLastAreaID);


    connect(mainWindow, &MainWindow::SuccesfullyAttachedToOkami, this, &AreaInfoWidget::ProcessHandlerEnabled);
    connect(ui->loadValuesFromGame, &QPushButton::pressed, this, &AreaInfoWidget::LoadValuesFromGame);
    connect(ui->loadValuesToGame, &QPushButton::pressed, this, &AreaInfoWidget::LoadValuesToGame);
    connect(mainWindow, &MainWindow::SuccesfullyDetachedFromOkami, this, &AreaInfoWidget::ProcessHandlerDisabled);
}

AreaInfoWidget::~AreaInfoWidget()
{
    delete ui;
}

void AreaInfoWidget::ReadValues(SaveSlot *saveSlot)
{
    ui->xSpinBox->setValue(*(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->x)) );
    ui->ySpinBox->setValue(*(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->y)) );
    ui->zSpinBox->setValue(*(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->z)) );

    ui->uSpinBox->setValue(*(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->u)) );
    ui->vSpinBox->setValue(*(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->v)) );
    ui->wSpinBox->setValue(*(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->w)) );

    ui->curRegionIDSpinBox->setValue(*(qint8*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->currentRegionID)) );
    ui->curAreaIDSpinBox->setValue(*(qint8*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->currentAreaID)) );

    ui->lastRegionIDSpinBox->setValue(*(qint8*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->lastRegionID)) );
    ui->lastAreaIDSpinBox->setValue(*(qint8*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->lastAreaID)) );    
}

void AreaInfoWidget::WriteValues(SaveSlot *saveSlot)
{
    *(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->x)) = ui->xSpinBox->value();
    *(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->y)) = ui->ySpinBox->value();
    *(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->z)) = ui->zSpinBox->value();

    *(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->u)) = ui->uSpinBox->value();
    *(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->v)) = ui->vSpinBox->value();
    *(float*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->w)) = ui->wSpinBox->value();

    *(qint8*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->currentRegionID)) = ui->curRegionIDSpinBox->value();
    *(qint8*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->currentAreaID)) = ui->curAreaIDSpinBox->value();

    *(qint8*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->lastRegionID)) = ui->lastRegionIDSpinBox->value();
    *(qint8*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->lastAreaID)) = ui->lastAreaIDSpinBox->value();    
}

void AreaInfoWidget::UpdateDials()
{
    ui->uDial->setSliderPosition((180.0/M_PI) * ui->uSpinBox->value());
    ui->vDial->setSliderPosition((180.0/M_PI) * ui->vSpinBox->value());
    ui->wDial->setSliderPosition((180.0/M_PI) * ui->wSpinBox->value());
}

void AreaInfoWidget::UpdateSpinBoxes()
{
    ui->uSpinBox->setValue((M_PI/180.0) * ui->uDial->value());
    ui->vSpinBox->setValue((M_PI/180.0) * ui->vDial->value());
    ui->wSpinBox->setValue((M_PI/180.0) * ui->wDial->value());
}

void AreaInfoWidget::ProcessHandlerEnabled(GameProcessHandler* gameProcessHandler)
{
    m_gameProcessHandler = gameProcessHandler;
    ui->loadValuesFromGame->setEnabled(true);
    ui->loadValuesToGame->setEnabled(true);
    ui->loadValuesFromGame->setText("Load Values From Game");
}

void AreaInfoWidget::LoadValuesFromGame()
{
    if (m_gameProcessHandler->m_processID != m_gameProcessHandler->GetProcessID(L"okami.exe"))
    {
        ProcessHandlerDisabled();
        delete m_gameProcessHandler;
        return;
    }

    Vec3 coordinates = m_gameProcessHandler->GetTypeValue<Vec3>(0xB6B2D0, {0}, 0x80); // Gets pl00 base address and offsets by 0x80 to directly go to the coordinates, skipping the intermediate pointer the game uses to access them
    ui->xSpinBox->setValue(coordinates.x);
    ui->ySpinBox->setValue(coordinates.y);
    ui->zSpinBox->setValue(coordinates.z);

    Vec3 radians = m_gameProcessHandler->GetTypeValue<Vec3>(0xB6B2D0, {0}, 0xB0);
    ui->uSpinBox->setValue(radians.x);
    ui->vSpinBox->setValue(radians.y);
    ui->wSpinBox->setValue(radians.z);

    qint8 curAreaID = m_gameProcessHandler->GetTypeValue<qint8>(0xB65E74, {}, 0x0);
    ui->curAreaIDSpinBox->setValue(curAreaID);
    qint8 curRegionID = m_gameProcessHandler->GetTypeValue<qint8>(0xB65E75, {}, 0x0);
    ui->curRegionIDSpinBox->setValue(curRegionID);

    //qint8 lastAreaID = m_gameProcessHandler->GetTypeValue<qint8>(0xB65E74, {}, 0x0);
    //ui->lastAreaIDSpinBox->setValue(lastAreaID);
    //qint8 lastRegionID = m_gameProcessHandler->GetTypeValue<qint8>(0xB65E74, {}, 0x0);
    //ui->lastRegionIDSpinBox->setValue(lastRegionID);
}

void AreaInfoWidget::LoadValuesToGame()
{
    if (m_gameProcessHandler->m_processID != m_gameProcessHandler->GetProcessID(L"okami.exe"))
    {
        ProcessHandlerDisabled();
        delete m_gameProcessHandler;
        return;
    }

    Vec3 coordinates;
    coordinates.x = ui->xSpinBox->value();
    coordinates.y = ui->ySpinBox->value();
    coordinates.z = ui->zSpinBox->value();
    m_gameProcessHandler->SetTypeValue(0xB65E64, {}, 0x0, coordinates);

    Vec3 radians;
    coordinates.x = ui->uSpinBox->value();
    coordinates.y = ui->vSpinBox->value();
    coordinates.z = ui->wSpinBox->value();
    m_gameProcessHandler->SetTypeValue(0xB65E70, {}, 0x0, radians.y);


    qint8 curAreaID = ui->curAreaIDSpinBox->value();
    m_gameProcessHandler->SetTypeValue<qint8>(0xB65E74, {}, 0x0, curAreaID);
    qint8 curRegionID = ui->curRegionIDSpinBox->value();
    m_gameProcessHandler->SetTypeValue<qint8>(0xB65E75, {}, 0x0, curRegionID);

    //qint8 lastAreaID = ui->lastAreaIDSpinBox->value();
    //m_gameProcessHandler->SetTypeValue<qint8>(0xB65E74, {}, 0x0, lastAreaID);
    //qint8 lastRegionID = ui->lastRegionIDSpinBox->value();
    //m_gameProcessHandler->SetTypeValue<qint8>(0xB65E74, {}, 0x0, lastRegionID);

    m_gameProcessHandler->FlipBit(0xB6B2AF, {}, 0x0, 1);
}

void AreaInfoWidget::ProcessHandlerDisabled()
{
    ui->loadValuesFromGame->setEnabled(false);
    ui->loadValuesToGame->setEnabled(false);
    ui->loadValuesFromGame->setText("Load Values From Game (Not attached->Tools)");
}



void AreaInfoWidget::ChangeComboModel()
{
    switch(ui->curRegionIDSpinBox->value())
    {
    case 0x0:
        ui->curAreaComboBox->setModel(&regDebug);
        break;
    case 0x1:
        ui->curAreaComboBox->setModel(&regEastNip);
        break;
    case 0x2:
        ui->curAreaComboBox->setModel(&regWestNip);
        break;
    case 0x3:
        ui->curAreaComboBox->setModel(&regKamui);
        break;
    case 0xf:
        ui->curAreaComboBox->setModel(&regOverworld);
        break;
    case 0xc:
        ui->curAreaComboBox->setModel(&regMenu);
        break;
    case 0xd:
        ui->curAreaComboBox->setModel(&regBeta);
        break;
    case 0xe:
        ui->curAreaComboBox->setModel(&regFishing);
        break;

    }
}

void AreaInfoWidget::UpdateCurrentRegionID()
{
    ui->curRegionIDSpinBox->setValue(ui->curRegionComboBox->currentData(0x100).toInt());
}

void AreaInfoWidget::UpdateRegionComboBoxIndex()
{
    ui->curRegionComboBox->setCurrentIndex(ui->curRegionComboBox->findData(ui->curRegionIDSpinBox->value(), 0x100));
}

void AreaInfoWidget::UpdateCurrentAreaID()
{
    ui->curAreaIDSpinBox->setValue(ui->curAreaComboBox->currentData(0x100).toInt());
}

void AreaInfoWidget::UpdateAreaComboBoxIndex()
{
    ui->curAreaComboBox->setCurrentIndex(ui->curAreaComboBox->findData(ui->curAreaIDSpinBox->value(), 0x100));
}

void AreaInfoWidget::ChangeComboModelLast()
{
    switch(ui->lastRegionIDSpinBox->value())
    {
    case 0x0:
        ui->lastAreaComboBox->setModel(&regDebug);
        break;
    case 0x1:
        ui->lastAreaComboBox->setModel(&regEastNip);
        break;
    case 0x2:
        ui->lastAreaComboBox->setModel(&regWestNip);
        break;
    case 0x3:
        ui->lastAreaComboBox->setModel(&regKamui);
        break;
    case 0xf:
        ui->lastAreaComboBox->setModel(&regOverworld);
        break;
    case 0xc:
        ui->lastAreaComboBox->setModel(&regMenu);
        break;
    case 0xd:
        ui->lastAreaComboBox->setModel(&regBeta);
        break;
    case 0xe:
        ui->lastAreaComboBox->setModel(&regFishing);
        break;

    }
}

void AreaInfoWidget::UpdateLastRegionID()
{
    ui->lastRegionIDSpinBox->setValue(ui->lastRegionComboBox->currentData(0x100).toInt());
}

void AreaInfoWidget::UpdateLastRegionComboBoxIndex()
{
    ui->lastRegionComboBox->setCurrentIndex(ui->lastRegionComboBox->findData(ui->lastRegionIDSpinBox->value(), 0x100));
}

void AreaInfoWidget::UpdateLastAreaID()
{
    ui->lastAreaIDSpinBox->setValue(ui->lastAreaComboBox->currentData(0x100).toInt());
}

void AreaInfoWidget::UpdateLastAreaComboBoxIndex()
{
    ui->lastAreaComboBox->setCurrentIndex(ui->lastAreaComboBox->findData(ui->lastAreaIDSpinBox->value(), 0x100));
}

void AreaInfoWidget::InitializeComboBoxes()
{
    // Regions
    ui->curRegionComboBox->setModel(&reg);
    ui->curRegionComboBox->addItem("Debug",0x0);
    ui->curRegionComboBox->addItem("Eastern Nippon",0x1);
    ui->curRegionComboBox->addItem("Western Nippon",0x2);
    ui->curRegionComboBox->addItem("Kamui",0x3);
    ui->curRegionComboBox->addItem("Overworld",0xf);
    ui->curRegionComboBox->addItem("Menus",0xc);
    ui->curRegionComboBox->addItem("Beta",0xd);
    ui->curRegionComboBox->addItem("Fishing",0xe);

    ui->lastRegionComboBox->setModel(&reg);
    ui->lastAreaComboBox->setModel(&regDebug);

    // Areas
    ui->curAreaComboBox->setModel(&regDebug);
    ui->curAreaComboBox->addItem("Eternal Loading Screen",0x6);
    ui->curAreaComboBox->addItem("Weird Place",0x07);
    ui->curAreaComboBox->addItem("Item Debug Room",0x10);
    ui->curAreaComboBox->addItem("Item Debug Room (Empty)",0x11);
    ui->curAreaComboBox->addItem("Elemental Debug Room",0x20);
    ui->curAreaComboBox->addItem("Animal Culling Debug Room",0x21);
    ui->curAreaComboBox->addItem("Combat Debug Room (Wheels)",0x30);
    ui->curAreaComboBox->addItem("Combat Debug Room (Bulls)",0x31);
    ui->curAreaComboBox->addItem("Overlay Combat Debug Room?",0x32);
    ui->curAreaComboBox->addItem("Overlay Combat Debug Room 2?",0x33);
    ui->curAreaComboBox->addItem("Big Blue Wall Debug Room",0x34);
    ui->curAreaComboBox->addItem("General Debug Room",0xFF);

    ui->curAreaComboBox->setModel(&regEastNip);
    ui->curAreaComboBox->addItem("Kamiki Village (Game Start)",0x0);
    ui->curAreaComboBox->addItem("Cave of Nagi",0x01);
    ui->curAreaComboBox->addItem("Kamiki Village",0x02);
    ui->curAreaComboBox->addItem("Hana Valley",0x03);
    ui->curAreaComboBox->addItem("Tsuta Ruins",0x04);
    ui->curAreaComboBox->addItem("City Checkpoint",0x05);
    ui->curAreaComboBox->addItem("Spider Queen Arena",0x06);
    ui->curAreaComboBox->addItem("Gale Shrine",0x07);
    ui->curAreaComboBox->addItem("Kusa Village",0x08);
    ui->curAreaComboBox->addItem("Sasa Sanctuary",0x09);
    ui->curAreaComboBox->addItem("Agata Forest - Madame Fawn's",0x0a);
    ui->curAreaComboBox->addItem("Digging Game",0x0b);
    ui->curAreaComboBox->addItem("Dojo Learning Room",0x0c);
    ui->curAreaComboBox->addItem("Crimson Helm Arena",0x0d);
    ui->curAreaComboBox->addItem("Calcified Cavern",0x0e);

    ui->curAreaComboBox->addItem("Moon Cave",0x10);
    ui->curAreaComboBox->addItem("Orochi Arena",0x11);
    ui->curAreaComboBox->addItem("Kamiki Village (Full Moon/Festival)",0x12);
    ui->curAreaComboBox->addItem("Ryoshima Coast Spider Queen Grotto",0x13);
    ui->curAreaComboBox->addItem("Taka Pass - Moley's Treasure Trove",0x14);
    ui->curAreaComboBox->addItem("Kamui - CB3 Grotto",0x15);
    ui->curAreaComboBox->addItem("North Ryoshima Coast - PS2 Grotto",0x16);
    ui->curAreaComboBox->addItem("North Ryoshima Coast - CB2 Grotto",0x17);
    ui->curAreaComboBox->addItem("Kamui (Ezofuji) - PS 3 Grotto",0x18);
    ui->curAreaComboBox->addItem("Kamui - Blockhead Grotto",0x19);
    ui->curAreaComboBox->addItem("North Ryoshima Coast Spider Queen Grotto",0x1a);
    ui->curAreaComboBox->addItem("Kamui Spider Queen Grotto",0x1b);
    ui->curAreaComboBox->addItem("Dragon Palace Grotto Under Stairs",0x1c);
    ui->curAreaComboBox->addItem("North Ryoshima Coast Grotto Near Yoichi",0x1d);

    ui->curAreaComboBox->addItem("New Game",0x20);
    ui->curAreaComboBox->addItem("Broken Kamiki",0x21);
    ui->curAreaComboBox->addItem("River of the Heavens",0x22);

    ui->curAreaComboBox->setModel(&regWestNip);
    ui->curAreaComboBox->addItem("Sei'an Aristocratic Quarter",0x00);
    ui->curAreaComboBox->addItem("Sei'an Commoner's Quarter",0x01);
    ui->curAreaComboBox->addItem("Himiko's Palace",0x02);
    ui->curAreaComboBox->addItem("Dragon Palace",0x03);
    ui->curAreaComboBox->addItem("Inside the Dragon",0x04);
    ui->curAreaComboBox->addItem("Sunken Ship",0x05);
    ui->curAreaComboBox->addItem("Imperial Palace Entry",0x06);
    ui->curAreaComboBox->addItem("Imperial Palace",0x07);
    ui->curAreaComboBox->addItem("Oni Island Before Platforming",0x08);
    ui->curAreaComboBox->addItem("Ninetails Arena",0x09);
    ui->curAreaComboBox->addItem("Catcall Tower",0x0a);
    ui->curAreaComboBox->addItem("Inside the Emporer",0x0b);
    ui->curAreaComboBox->addItem("Sei'an Commoner's Quarter - Kimono Shop",0x0c);
    ui->curAreaComboBox->addItem("Oni Island Entry",0x0d);
    ui->curAreaComboBox->addItem("Oni Island Upper Floors",0x0e);
    ui->curAreaComboBox->addItem("Oni Island After Platforming",0x0f);

    ui->curAreaComboBox->setModel(&regKamui);
    ui->curAreaComboBox->addItem("Wep'keer",0x01);
    ui->curAreaComboBox->addItem("Kamiki Village (Past)",0x02);
    ui->curAreaComboBox->addItem("Wawku Shrine",0x03);
    ui->curAreaComboBox->addItem("Nechku Arena",0x04);
    ui->curAreaComboBox->addItem("Ponc'tan",0x05);
    ui->curAreaComboBox->addItem("Moon Cave Entry (Past)",0x06);
    ui->curAreaComboBox->addItem("Ark of Yamato",0x07);
    ui->curAreaComboBox->addItem("Spider Queen Arena (Ark)",0x08);
    ui->curAreaComboBox->addItem("Orochi Arena (Ark)",0x09);
    ui->curAreaComboBox->addItem("Blight Arena (Ark)",0x0a);
    ui->curAreaComboBox->addItem("Ninetails Area (Ark)",0x0b);
    ui->curAreaComboBox->addItem("Crimson Helm Arena (Ark)",0x0c);
    ui->curAreaComboBox->addItem("Ponc'tan - Mrs. Seal's House",0x0d);
    ui->curAreaComboBox->addItem("Yoshpet",0x10);
    ui->curAreaComboBox->addItem("Inner Yoshpet",0x11);
    ui->curAreaComboBox->addItem("Yami Arena",0x12);
    ui->curAreaComboBox->addItem("Wep'keer Square",0x13);
    ui->curAreaComboBox->addItem("Lechku and Nechkua Arena",0x14);

    ui->curAreaComboBox->setModel(&regOverworld);
    ui->curAreaComboBox->addItem("Shinshu Field (Cursed)",0x01);
    ui->curAreaComboBox->addItem("Shinshu Field (Healed)",0x02);
    ui->curAreaComboBox->addItem("AgataForest (Cursed)",0x03);
    ui->curAreaComboBox->addItem("AgataForest (Healed)",0x04);
    ui->curAreaComboBox->addItem("Moon Cave Entrance",0x06);
    ui->curAreaComboBox->addItem("Taka Pass (Cursed)",0x07);
    ui->curAreaComboBox->addItem("Taka Pass (Healed)",0x08);
    ui->curAreaComboBox->addItem("Ryoshima Coast (Cursed)",0x09);
    ui->curAreaComboBox->addItem("Ryoshima Coast (Healed)",0x0a);
    ui->curAreaComboBox->addItem("North Ryoshima Coast",0x0c);
    ui->curAreaComboBox->addItem("Kamui (Cursed)",0x11);
    ui->curAreaComboBox->addItem("Kamui (Healed)",0x12);
    ui->curAreaComboBox->addItem("Kamui (Ezofuji)",0x13);
    ui->curAreaComboBox->addItem("Shinshu Field (Past)",0x20);
    ui->curAreaComboBox->addItem("Moon Cave Entrance (Past)",0x21);

    ui->curAreaComboBox->setModel(&regMenu);
    ui->curAreaComboBox->addItem("Starting Screen",0x00);
    ui->curAreaComboBox->addItem("Media Menu",0x02);

    ui->curAreaComboBox->setModel(&regBeta);
    ui->curAreaComboBox->addItem("Shinshu field no exits, some missing collisions, cursed area but doesn't kill you.",0x00);
    ui->curAreaComboBox->addItem("Buggy Hana Valley",0x01);
    ui->curAreaComboBox->addItem("idk falling forever",0x02);
    ui->curAreaComboBox->addItem("Buggy Agata maybe?",0x03);
    ui->curAreaComboBox->addItem("Buggy Ryoshima Coast?",0x04);
    ui->curAreaComboBox->addItem("Crashes",0x05);
    ui->curAreaComboBox->addItem("Buggy Taka Pass",0x06);
    ui->curAreaComboBox->addItem("Buggy Beta Kusa maybe?",0x07);

    ui->curAreaComboBox->setModel(&regFishing);
    ui->curAreaComboBox->addItem("Sei'an City Bridge Fishing",0x00);
    ui->curAreaComboBox->addItem("Sei'an Aristocratic Quarter Fishing",0x01);
    ui->curAreaComboBox->addItem("Agata Forest Fishing",0x02);
    ui->curAreaComboBox->addItem("Kamui Fishing",0x03);
    ui->curAreaComboBox->addItem("North Ryoshima Coast Fishing",0x04);

    ui->curAreaComboBox->setModel(&regDebug);
}
