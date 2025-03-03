#include "saveslotdisplay.h"
#include "ui_saveslotdisplay.h"

#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QPainter>

#include "mainwindow.h"
#include "savefileinfowidget.h"

SaveSlotDisplay::SaveSlotDisplay(QWidget *parent, SaveSlot *pSaveSlot, int index)
    : QFrame(parent)
    , ui(new Ui::SaveSlotDisplay)
    , m_pSaveSlot(pSaveSlot)
{
    ui->setupUi(this);
    parent->layout()->addWidget(this);

    QString str;
    ui->Index->setText(str.setNum(index));

    connect(this, &SaveSlotDisplay::GetAreaID, mainWindow->pSaveFileInfoWidget, &SaveFileInfoWidget::GetAreaName);
    ui->AreaName->setText(emit GetAreaID(m_pSaveSlot));

    connect(this, &SaveSlotDisplay::GetTime, mainWindow->pSaveFileInfoWidget, &SaveFileInfoWidget::GetTime);
    str = emit GetTime(m_pSaveSlot);
    QStringList strList = str.split(u',');
    ui->hours->setText(strList.at(0));
    ui->minutes->setText(strList.at(1));
    ui->seconds->setText(strList.at(2));

    setFocusPolicy(Qt::ClickFocus);

    connect(mainWindow, &MainWindow::SaveSlotChanged, this, &SaveSlotDisplay::onDeselect);

    connect(this, &SaveSlotDisplay::SaveSlotSelected, mainWindow, &MainWindow::onSaveSlotSelect);

    setAcceptDrops(true);

    this->setLineWidth(3);
    this->setMidLineWidth(3);
}

SaveSlotDisplay::~SaveSlotDisplay()
{
    delete ui;
}

void SaveSlotDisplay::onDeselect()
{
    ui->Select->setText("");
    this->setFrameStyle(QFrame::NoFrame);
    selected = false;

    //This should be moved elsewhere to update it as soon as it is changed in the editor
    {
        QString str;
        ui->AreaName->setText(emit GetAreaID(m_pSaveSlot));
        str = emit GetTime(m_pSaveSlot);
    }
}

void SaveSlotDisplay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        emit SaveSlotSelected(m_pSaveSlot);

        ui->Select->setText(">");
        this->setFrameStyle(QFrame::WinPanel | QFrame::Sunken);
        selected = true;

        dragStartPosition = event->position();
    }
}

void SaveSlotDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if (!(event->buttons() & Qt::LeftButton))
        return;
    if ((event->position() - dragStartPosition).manhattanLength()
        < QApplication::startDragDistance())
        return;


    QPixmap widgetPixmap(size());
    render(&widgetPixmap);

    QDrag *drag = new QDrag(this);
    QMimeData *mimeData = new QMimeData;

    drag->setPixmap(widgetPixmap);

    int index = this->parentWidget()->layout()->indexOf(this);

    QString data;
    data.setNum(index);
    mimeData->setText(data);

    drag->setMimeData(mimeData);
    if (mainWindow->currentState == Editor) hide();
    Qt::DropAction dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction);
    show();
}

void SaveSlotDisplay::enterEvent(QEnterEvent *event)
{
    if (!selected) this->setFrameStyle(QFrame::StyledPanel | QFrame::Plain);
}

void SaveSlotDisplay::leaveEvent(QEvent *event)
{
    if (!selected) this->setFrameStyle(QFrame::NoFrame);
}

void SaveSlotDisplay::dragEnterEvent(QDragEnterEvent *event)
{
    if ((mainWindow->currentState != SlotSwap) || (event->source()->parent() == this->parent())) return;
    if (!selected) this->setFrameStyle(QFrame::Panel | QFrame::Raised);

    if (event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
}

void SaveSlotDisplay::dropEvent(QDropEvent *event)
{
    if ((mainWindow->currentState != SlotSwap) || (event->source()->parent() == this->parent())) return;

    int index = this->parentWidget()->layout()->indexOf(this);
    if (mainWindow->pSaveSlotListContent->m_pBoxLayout->itemAt(index)->widget() == this)
    {
         mainWindow->SwapSlots(index, event->mimeData()->text().toInt());
    }
    else
    {
       mainWindow->SwapSlots(event->mimeData()->text().toInt(), index);
    }
    this->setFrameStyle(QFrame::Panel | QFrame::Sunken);
}

void SaveSlotDisplay::dragLeaveEvent(QDragLeaveEvent *event)
{
    if (!selected) this->setFrameStyle(QFrame::NoFrame);
}
/*
void SaveSlotDisplay::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
*/
