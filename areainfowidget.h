#ifndef AREAINFOWIDGET_H
#define AREAINFOWIDGET_H

#include <QWidget>

#include "SaveSlot.h"

#include <QStandardItemModel>

#ifdef Q_OS_WINDOWS
#include "gameprocesshandlerwin.h"
#endif

#ifdef Q_OS_LINUX

#endif

namespace Ui {
class AreaInfoWidget;
}

class AreaInfoWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AreaInfoWidget(QWidget *parent = nullptr);
    ~AreaInfoWidget();
public slots:
    void ReadValues(SaveSlot* saveSlot);
    void WriteValues(SaveSlot* saveSlot);

    void UpdateDials();
    void UpdateSpinBoxes();

    void ProcessHandlerEnabled(GameProcessHandler* gameProcessHandler);
    void LoadValuesFromGame();
    void LoadValuesToGame();
    void ProcessHandlerDisabled();

    void ChangeComboModel();

    void UpdateCurrentRegionID();
    void UpdateRegionComboBoxIndex();

    void UpdateCurrentAreaID();
    void UpdateAreaComboBoxIndex();

    void ChangeComboModelLast();

    void UpdateLastRegionID();
    void UpdateLastRegionComboBoxIndex();

    void UpdateLastAreaID();
    void UpdateLastAreaComboBoxIndex();

private:
    GameProcessHandler* m_gameProcessHandler;

    QStandardItemModel reg;
    QStandardItemModel regDebug;
    QStandardItemModel regEastNip;
    QStandardItemModel regWestNip;
    QStandardItemModel regKamui;
    QStandardItemModel regOverworld;
    QStandardItemModel regMenu;
    QStandardItemModel regBeta;
    QStandardItemModel regFishing;

    QStandardItemModel regLast;
    QStandardItemModel regLastDebug;
    QStandardItemModel regLastEastNip;
    QStandardItemModel regLastWestNip;
    QStandardItemModel regLastKamui;
    QStandardItemModel regLastOverworld;
    QStandardItemModel regLastMenu;
    QStandardItemModel regLastBeta;
    QStandardItemModel regLastFishing;

    void InitializeComboBoxes();

    Ui::AreaInfoWidget *ui;
};

#endif // AREAINFOWIDGET_H
