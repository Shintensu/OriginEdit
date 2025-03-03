#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListView>
#include <QStringListModel>

#include "SaveSlot.h"
#include "pointerlistmodel.h"
#include "itemwidget.h"
#include "weaponwidget.h"
#include "savefileinfowidget.h"
#include "areainfowidget.h"

#include "saveslotlistcontent.h"

#ifdef Q_OS_WINDOWS
#include "gameprocesshandlerwin.h"
#endif

#ifdef Q_OS_LINUX
#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void SwapSlots(int listIndex1, int listIndex2);

protected slots:
    void onNewSaveFile();
    void onOpenSaveFile();
    void onSaveSaveFile();

    void onOpenSaveFile2();
    void onSaveSaveFile2();

    void onOpenEditor();
    void onOpenSlotSwap();

    void onAttachToGame();
    void onDetachFromGame();
public slots:
    void onSaveSlotSelect(SaveSlot* selectedSaveSlot);



signals:
    void LoadValuesToEditor(SaveSlot *saveSlot);
    void SaveValuesFromEditor(SaveSlot *saveSlot);

    void SaveSlotChanged();

    void SuccesfullyAttachedToOkami(GameProcessHandler* gameProcessHandler);
    void SuccesfullyDetachedFromOkami();

    void StateUpdate(State newState);

private:
    unsigned long long GenerateChecksum(unsigned long long* param_1);

    void InitializeWidgets();

    QString saveFilePath;

    SaveSlot* saveSlots;
    SaveSlot* saveSlots2;

    SaveSlot* currentSaveSlot;
    SaveSlot* currentSaveSlot2;

    PointerListModel* ptrListModel;

    QVector<ItemWidget*> consumableItemList;
    QVector<ItemWidget*> karmicTransformList;
    QVector<ItemWidget*> keyItemList;
    QVector<ItemWidget*> treasureItemList;
    QVector<ItemWidget*> fishItemList;

    QVector<WeaponWidget*> weaponItemList;
    QVector<ItemWidget*> holyArtifactList;

    // QListView* m_pSaveSlotList;
    Ui::MainWindow *ui;


public:
    GameProcessHandler* gameProcessHandler;

    State currentState = Editor;
    SaveSlotListContent* pSaveSlotListContent;
    SaveSlotListContent* pSaveSlotListContent2;
    SaveFileInfoWidget* pSaveFileInfoWidget;
    AreaInfoWidget* pAreaInfoWidget;
};

extern MainWindow *mainWindow;

#endif // MAINWINDOW_H
