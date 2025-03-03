#ifndef SAVESLOTDISPLAY_H
#define SAVESLOTDISPLAY_H

#include <QFrame>

#include "SaveSlot.h"

namespace Ui {
class SaveSlotDisplay;
}

class SaveSlotDisplay : public QFrame
{
    Q_OBJECT

public:
    explicit SaveSlotDisplay(QWidget *parent = nullptr, SaveSlot* pSaveSlot = nullptr, int index = 0);
    ~SaveSlotDisplay();

signals:
    QString GetAreaID(SaveSlot* saveSlot);
    QString GetTime(SaveSlot* saveSlot);

    void SaveSlotSelected(SaveSlot* saveSlot);
public slots:
    void onDeselect();

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;

    virtual void enterEvent(QEnterEvent *event) override;
    virtual void leaveEvent(QEvent *event) override;

    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;
    virtual void dragLeaveEvent(QDragLeaveEvent *event) override;

    //virtual void paintEvent(QPaintEvent *) override;
public:
    SaveSlot* m_pSaveSlot;

private:
    bool selected;
    QPointF dragStartPosition;
    Ui::SaveSlotDisplay *ui;
};

#endif // SAVESLOTDISPLAY_H
