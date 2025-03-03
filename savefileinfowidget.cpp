#include "savefileinfowidget.h"
#include "ui_savefileinfowidget.h"

#include "mainwindow.h"

#define SAVESLOTNULL &((struct SaveSlot *)nullptr)

SaveFileInfoWidget::SaveFileInfoWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SaveFileInfoWidget)
{
    ui->setupUi(this);

    parent->layout()->addWidget(this);

    InitializeComboBox();

    connect(mainWindow, &MainWindow::LoadValuesToEditor, this, &SaveFileInfoWidget::ReadValues);
    connect(mainWindow, &MainWindow::SaveValuesFromEditor, this, &SaveFileInfoWidget::WriteValues);

    // Potentially recursively calling each other
    connect(ui->areaNameIDSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::UpdateAreaNameComboBoxIndex);
    connect(ui->areaNameComboBox, &QComboBox::currentIndexChanged, this, &SaveFileInfoWidget::UpdateCurrentAreaNameID);

    connect(ui->playtimeHSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onHUpdate);
    connect(ui->playtimeMSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onMUpdate);
    connect(ui->playtimeSSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onSUpdate);
    connect(ui->playtimeMSDoubleSpinBox, &QDoubleSpinBox::valueChanged, this, &SaveFileInfoWidget::onMSUpdate);

    connect(ui->realtimeYearsSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onRealYUpdate);
    connect(ui->realtimeDaysSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onRealDUpdate);
    connect(ui->realtimeHoursSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onRealHUpdate);
    connect(ui->realtimeMinutesSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onRealMUpdate);
    connect(ui->realtimeSecondsSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onRealSUpdate);
    connect(ui->realtimeMillisecondsSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onRealMSUpdate);
    connect(ui->realtimeMicrosecondsSpinBox, &QSpinBox::valueChanged, this, &SaveFileInfoWidget::onRealMCSUpdate);
}

SaveFileInfoWidget::~SaveFileInfoWidget()
{
    delete ui;
}

void SaveFileInfoWidget::ReadValues(SaveSlot *saveSlot)
{
    ui->areaNameIDSpinBox->setValue(*(qint32*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->areaNameID)) );
    ui->timesSavedSpinBox->setValue(*(qint16*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->numberOfSaves)));

    ReadRealtime((*(quint64*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->timeRTC)) ));
    ReadPlaytime((*(qint32*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->timePlayed) )));
    ReadIGTime((*(qint32*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->timeOfDay)) ));
}

void SaveFileInfoWidget::WriteValues(SaveSlot *saveSlot)
{
    *(qint32*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->areaNameID)) = ui->areaNameIDSpinBox->value();
    *(qint16*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->numberOfSaves)) = ui->timesSavedSpinBox->value();

    *(quint64*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->timeRTC)) = WriteRealtime();
    *(qint32*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->timePlayed)) = WritePlaytime();
    *(qint32*) (((size_t)saveSlot) + (size_t)(SAVESLOTNULL->timeOfDay)) = WriteIGtime();


}

void SaveFileInfoWidget::onHUpdate(int hour)
{
    if (hour == 2000000001) ui->playtimeHSpinBox->setValue(0);
    if (hour == -1) ui->playtimeHSpinBox->setValue(2000000000);
}

void SaveFileInfoWidget::onMUpdate(int minute)
{
    if (minute == 60)
    {
        ui->playtimeMSpinBox->setValue(0);
        ui->playtimeHSpinBox->setValue(ui->playtimeHSpinBox->value() + 1);
    }
    if (minute == -1)
    {
        ui->playtimeMSpinBox->setValue(59);
        ui->playtimeHSpinBox->setValue(ui->playtimeHSpinBox->value() - 1);
    }
}

void SaveFileInfoWidget::onSUpdate(int second)
{
    if (second == 60)
    {
        ui->playtimeSSpinBox->setValue(0);
        ui->playtimeMSpinBox->setValue(ui->playtimeMSpinBox->value() + 1);
    }
    if (second == -1)
    {
        ui->playtimeSSpinBox->setValue(59);
        ui->playtimeMSpinBox->setValue(ui->playtimeMSpinBox->value() - 1);
    }
}

void SaveFileInfoWidget::onMSUpdate(double millisecond)
{
    if (millisecond >= (1000.0 - ui->playtimeMSDoubleSpinBox->singleStep()))
    {
        ui->playtimeMSDoubleSpinBox->setValue(0.0);
        ui->playtimeSSpinBox->setValue(ui->playtimeSSpinBox->value() + 1);
    }
    if (millisecond < 0)
    {
        ui->playtimeMSDoubleSpinBox->setValue(1000.0 - ui->playtimeMSDoubleSpinBox->singleStep());
        ui->playtimeSSpinBox->setValue(ui->playtimeSSpinBox->value() - 1);
    }
}

void SaveFileInfoWidget::onRealYUpdate(qint32 year)
{
    if (year == 2000000001) ui->realtimeYearsSpinBox->setValue(0);
    if (year == -1) ui->realtimeYearsSpinBox->setValue(2000000000);
}

void SaveFileInfoWidget::onRealDUpdate(qint32 day)
{
    if (day == 365)
    {
        ui->realtimeDaysSpinBox->setValue(0);
        ui->realtimeYearsSpinBox->setValue(ui->realtimeYearsSpinBox->value() + 1);
    }
    if (day == -1)
    {
        ui->realtimeDaysSpinBox->setValue(364);
        ui->realtimeYearsSpinBox->setValue(ui->realtimeYearsSpinBox->value() - 1);
    }
}

void SaveFileInfoWidget::onRealHUpdate(qint32 hour)
{
    if (hour == 24)
    {
        ui->realtimeHoursSpinBox->setValue(0);
        ui->realtimeDaysSpinBox->setValue(ui->realtimeDaysSpinBox->value() + 1);
    }
    if (hour == -1)
    {
        ui->realtimeHoursSpinBox->setValue(23);
        ui->realtimeDaysSpinBox->setValue(ui->realtimeDaysSpinBox->value() - 1);
    }
}

void SaveFileInfoWidget::onRealMUpdate(qint32 minute)
{
    if (minute == 60)
    {
        ui->realtimeMinutesSpinBox->setValue(0);
        ui->realtimeHoursSpinBox->setValue(ui->realtimeHoursSpinBox->value() + 1);
    }
    if (minute == -1)
    {
        ui->realtimeMinutesSpinBox->setValue(59);
        ui->realtimeHoursSpinBox->setValue(ui->realtimeHoursSpinBox->value() - 1);
    }
}

void SaveFileInfoWidget::onRealSUpdate(qint32 second)
{
    if (second == 60)
    {
        ui->realtimeSecondsSpinBox->setValue(0);
        ui->realtimeMinutesSpinBox->setValue(ui->realtimeMinutesSpinBox->value() + 1);
    }
    if (second == -1)
    {
        ui->realtimeSecondsSpinBox->setValue(59);
        ui->realtimeMinutesSpinBox->setValue(ui->realtimeMinutesSpinBox->value() - 1);
    }
}

void SaveFileInfoWidget::onRealMSUpdate(qint32 millisecond)
{
    if (millisecond == 1000)
    {
        ui->realtimeMillisecondsSpinBox->setValue(0);
        ui->realtimeSecondsSpinBox->setValue(ui->realtimeSecondsSpinBox->value() + 1);
    }
    if (millisecond  == -1)
    {
        ui->realtimeMillisecondsSpinBox->setValue(999);
        ui->realtimeSecondsSpinBox->setValue(ui->realtimeSecondsSpinBox->value() - 1);
    }
}

void SaveFileInfoWidget::onRealMCSUpdate(qint32 microsecond)
{
    if (microsecond == 1000)
    {
        ui->realtimeMicrosecondsSpinBox->setValue(0);
        ui->realtimeMillisecondsSpinBox->setValue(ui->realtimeMillisecondsSpinBox->value() + 1);
    }
    if (microsecond  == -1)
    {
        ui->realtimeMicrosecondsSpinBox->setValue(999);
        ui->realtimeMillisecondsSpinBox->setValue(ui->realtimeMillisecondsSpinBox->value() - 1);
    }
}

void SaveFileInfoWidget::UpdateAreaNameComboBoxIndex()
{
    qint32 areaNameID = ui->areaNameIDSpinBox->value();
    if (areaNameID > 0x34) areaNameID = 0x35;
    ui->areaNameComboBox->setCurrentIndex(ui->areaNameComboBox->findData(areaNameID, 0x100));
}

void SaveFileInfoWidget::UpdateCurrentAreaNameID()
{
    ui->areaNameIDSpinBox->setValue(ui->areaNameComboBox->currentData(0x100).toInt());
}

QString SaveFileInfoWidget::GetAreaName(SaveSlot* saveSlot)
{
    qint32 areaNameID = saveSlot->areaNameID;
    if (areaNameID > 0x34) areaNameID = 0x35;
    return ui->areaNameComboBox->itemText(ui->areaNameComboBox->findData(areaNameID, 0x100));
}

QString SaveFileInfoWidget::GetTime(SaveSlot *saveSlot)
{
    qint32 playtime = saveSlot->timePlayed;
    int seconds = (playtime/60) % 60;
    int minutes = (playtime/60/60) % 60;
    int hours = playtime/60/60/60;

    ui->playtimeHSpinBox->setValue(hours);
    ui->playtimeMSpinBox->setValue(minutes);
    ui->playtimeSSpinBox->setValue(seconds);

    QString timeString;
    timeString.append(ui->playtimeHSpinBox->text());
    timeString.append(",");
    timeString.append(ui->playtimeMSpinBox->text());
    timeString.append(",");
    timeString.append(ui->playtimeSSpinBox->text());

    return timeString;
}

void SaveFileInfoWidget::InitializeComboBox()
{
    ui->areaNameComboBox->addItem(tr("Kamiki Village")                 ,0x00);
    ui->areaNameComboBox->addItem(tr("Shinshu Field")                  ,0x01);
    ui->areaNameComboBox->addItem(tr("Agata Forest")                   ,0x02);
    ui->areaNameComboBox->addItem(tr("Taka Pass")                      ,0x03);
    ui->areaNameComboBox->addItem(tr("Kusa Village")                   ,0x04);
    ui->areaNameComboBox->addItem(tr("Susa Sanctuary")                 ,0x05);
    ui->areaNameComboBox->addItem(tr("City Checkpoint")                ,0x06);
    ui->areaNameComboBox->addItem(tr("Ryoshima Coast")                 ,0x07);
    ui->areaNameComboBox->addItem(tr("N. Ryoshima Coast")              ,0x08);
    ui->areaNameComboBox->addItem(tr("Dragon Palace")                  ,0x09);
    ui->areaNameComboBox->addItem(tr("Kamui")                          ,0x0a);
    ui->areaNameComboBox->addItem(tr("Wep'keer")                       ,0x0b);
    ui->areaNameComboBox->addItem(tr("N. Ryoshima Coast (Rocky Area)") ,0x0c);
    ui->areaNameComboBox->addItem(tr("Kamui (Ezofuji Rocky Area)")     ,0x0d);
    ui->areaNameComboBox->addItem(tr("Inner Yoshpet")                  ,0x0e);
    ui->areaNameComboBox->addItem(tr("River of the Heavens")           ,0x0f);
    ui->areaNameComboBox->addItem(tr("Inner Hana Valley")              ,0x10);
    ui->areaNameComboBox->addItem(tr("Tsuta Ruins")                    ,0x11);
    ui->areaNameComboBox->addItem(tr("Gale Shrine")                    ,0x12);
    ui->areaNameComboBox->addItem(tr("Moon Cave")                      ,0x13);
    ui->areaNameComboBox->addItem(tr("Sei-an City (Commoners' Qtr.)")  ,0x14);
    ui->areaNameComboBox->addItem(tr("Sunken Ship")                    ,0x15);
    ui->areaNameComboBox->addItem(tr("Imperial Palace")                ,0x16);
    ui->areaNameComboBox->addItem(tr("Inside the Dragon")              ,0x17);
    ui->areaNameComboBox->addItem(tr("Oni Island (Interior/1F)")       ,0x18);
    ui->areaNameComboBox->addItem(tr("Kamiki Village")                 ,0x19);
    ui->areaNameComboBox->addItem(tr("Moon Cave Entrance")             ,0x1a);
    ui->areaNameComboBox->addItem(tr("Wawku Shrine")                   ,0x1b);
    ui->areaNameComboBox->addItem(tr("Agata Forest")                   ,0x1c);
    ui->areaNameComboBox->addItem(tr("Kamiki Village")                 ,0x1d);
    ui->areaNameComboBox->addItem(tr("Gale Shrine")                    ,0x1e);
    ui->areaNameComboBox->addItem(tr("Imperial Palace")                ,0x1f);
    ui->areaNameComboBox->addItem(tr("N. Ryoshima Coast")              ,0x20);
    ui->areaNameComboBox->addItem(tr("Yoshpet")                        ,0x21);
    ui->areaNameComboBox->addItem(tr("Kamui (Ezofuji)")                ,0x22);
    ui->areaNameComboBox->addItem(tr("Tsuta Ruins (Mid.)")             ,0x22);
    ui->areaNameComboBox->addItem(tr("Tsuta Ruins (Deep)")             ,0x23);
    ui->areaNameComboBox->addItem(tr("Gale Shrine (Deep)")             ,0x24);
    ui->areaNameComboBox->addItem(tr("Emperor's Body")                 ,0x25);
    ui->areaNameComboBox->addItem(tr("Oni Island (Interior/4F)")       ,0x26);
    ui->areaNameComboBox->addItem(tr("Wawku Shrine (Mid.)")            ,0x27);
    ui->areaNameComboBox->addItem(tr("Wawku Shrine (Deep)")            ,0x28);
    ui->areaNameComboBox->addItem(tr("Ark of Yamato")                  ,0x29);
    ui->areaNameComboBox->addItem(tr("Oni Island (High)")              ,0x2a);
    ui->areaNameComboBox->addItem(tr("Ponc'Tan")                       ,0x2b);
    ui->areaNameComboBox->addItem(tr("Hana Valley")                    ,0x2c);
    ui->areaNameComboBox->addItem(tr("Catcall Tower")                  ,0x2d);
    ui->areaNameComboBox->addItem(tr("Yoshpet")                        ,0x2e);
    ui->areaNameComboBox->addItem(tr("Kamui (Inner Ezofuji)")          ,0x2f);
    ui->areaNameComboBox->addItem(tr("Himiko's Palace")                ,0x30);
    ui->areaNameComboBox->addItem(tr("Sei-an City (Aristocratic Qtr.)"),0x31);
    ui->areaNameComboBox->addItem(tr("Oni Island (Interior/3F)")       ,0x32);
    ui->areaNameComboBox->addItem(tr("Kamui (Ezofuji)")                ,0x33);
    ui->areaNameComboBox->addItem(tr("Start from beginning")           ,0x34);
    ui->areaNameComboBox->addItem(tr("Corrupted game data")            ,0x35);
}

void SaveFileInfoWidget::ReadRealtime(quint64 time)
{
    quint64 microseconds = time % 1000;
    time /= 1000;
    quint64 milliseconds = time % 1000;
    time /= 1000;
    quint64 seconds = time % 60;
    time /= 60;
    quint64 minutes = time % 60;
    time /= 60;
    quint64 hours = time % 24;
    time /= 24;
    quint64 days = time % 365;
    time /= 365;
    quint64 years = time;

    ui->realtimeYearsSpinBox->setValue(years);
    ui->realtimeDaysSpinBox->setValue(days);
    ui->realtimeHoursSpinBox->setValue(hours);
    ui->realtimeMinutesSpinBox->setValue(minutes);
    ui->realtimeSecondsSpinBox->setValue(seconds);
    ui->realtimeMillisecondsSpinBox->setValue(milliseconds);
    ui->realtimeMicrosecondsSpinBox->setValue(microseconds);
}

quint64 SaveFileInfoWidget::WriteRealtime()
{
    quint64 combinedRealtime = 0;
    combinedRealtime += ui->realtimeYearsSpinBox->value();
    combinedRealtime *= 365;
    combinedRealtime += ui->realtimeDaysSpinBox->value();
    combinedRealtime *= 24;
    combinedRealtime += ui->realtimeHoursSpinBox->value();
    combinedRealtime *= 60;
    combinedRealtime += ui->realtimeMinutesSpinBox->value();
    combinedRealtime *= 60;
    combinedRealtime += ui->realtimeSecondsSpinBox->value();
    combinedRealtime *= 1000;
    combinedRealtime += ui->realtimeMillisecondsSpinBox->value();
    combinedRealtime *= 1000;
    combinedRealtime += ui->realtimeMicrosecondsSpinBox->value();
    return combinedRealtime;
}

void SaveFileInfoWidget::ReadIGTime(quint32 time)
{
    // IG time runs at 420 seconds per day cycle and 180 seconds per night
    // One time tick is saved as 100 in the address
    // IG time runs at half game ticks, e.g. two game ticks = one time tick, so to get to seconds you must first divide by 100, then by 30
    // 900 ticks before the actual time switches over to the next day/night cycle, the cutscene plays to switch over (if possible),
    // skipping over those 900 ticks faster than usual.

    quint32 currentDayTicks = time % 1800000;
    quint32 days = time/100/30/60/10;

    ui->currentDayHorizontalSlider->setValue(currentDayTicks);
    ui->igDaysPassedSpinBox->setValue(days);
}

quint32 SaveFileInfoWidget::WriteIGtime()
{
    quint32 timeTicks;
    timeTicks = ui->currentDayHorizontalSlider->value();
    timeTicks += (ui->igDaysPassedSpinBox->value() * 10 * 60 * 30 * 100);
    return timeTicks;
}

void SaveFileInfoWidget::ReadPlaytime(qint32 playtime)
{
    int remainingTicks = playtime % 60;
    double milliseconds = (1000.0/60.0) * remainingTicks;
    int seconds = (playtime/60) % 60;
    int minutes = (playtime/60/60) % 60;
    int hours = playtime/60/60/60;

    ui->playtimeHSpinBox->setValue(hours);
    ui->playtimeMSpinBox->setValue(minutes);
    ui->playtimeSSpinBox->setValue(seconds);
    ui->playtimeMSDoubleSpinBox->setValue(milliseconds);
}

qint32 SaveFileInfoWidget::WritePlaytime()
{
    qint32 combinedPlaytime = 0;
    combinedPlaytime += (ui->playtimeHSpinBox->value() * 60 * 60 * 60);
    combinedPlaytime += (ui->playtimeMSpinBox->value() * 60 * 60);
    combinedPlaytime += (ui->playtimeSSpinBox->value() * 60);
    combinedPlaytime += (ui->playtimeMSDoubleSpinBox->value() / (1000.0/60.0));

    return combinedPlaytime;
}


