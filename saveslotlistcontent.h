#ifndef SAVESLOTLISTCONTENT_H
#define SAVESLOTLISTCONTENT_H

#include <QWidget>
#include <QVBoxLayout>

enum State{Editor, SlotSwap};

class SaveSlotListContent : public QWidget
{
    Q_OBJECT
public:
    explicit SaveSlotListContent(QWidget *parent = nullptr);
protected:
    virtual void dragEnterEvent(QDragEnterEvent *event) override;
    virtual void dragMoveEvent(QDragMoveEvent *event) override;
    virtual void dropEvent(QDropEvent *event) override;

public slots:
    void updateState(State updatedState);

public:
    QVBoxLayout* m_pBoxLayout;
private:
    int currentSpacerIndex = -1;

    int listID = 0;
    State currentState;
    QWidget *spacer;
    QLayoutItem* removedWidget = nullptr;
signals:
};

#endif // SAVESLOTLISTCONTENT_H
