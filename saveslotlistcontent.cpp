#include "saveslotlistcontent.h"

#include <QVBoxLayout>
#include <QDragEnterEvent>
#include <QMimeData>

//#include "mainwindow.h"

int listCounter = 0;


// Gets created inside of each of the scroll areas in the mainwindow to contain 30 SaveSlotDisplays each.
// Handles a good chunk of the drag and drop functionality in Editor mode, but SlotSwapping has been mostly transferred to the SaveSlotDisplays themselves.
SaveSlotListContent::SaveSlotListContent(QWidget *parent)
    : QWidget{parent}
{
    parent->layout()->addWidget(this);

    listID = listCounter++;
    m_pBoxLayout = new QVBoxLayout(this);
    this->setMaximumSize(parent->maximumSize());
    this->setBaseSize(parent->baseSize());
    this->setMinimumSize(parent->minimumSize());

    currentState = Editor;

    spacer = new QWidget(this);
    spacer->hide();
    spacer->setMinimumHeight(10.0);
    spacer->setPalette(QColor(0xA0, 0xD8, 0xF1));
    spacer->setAutoFillBackground(true);

    setAcceptDrops(true);
}

void SaveSlotListContent::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasText())
    {
        event->acceptProposedAction();
    }
}

void SaveSlotListContent::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasText())
    {
        switch (currentState)
        {
        case Editor:
                for (int i = 0 ; i < this->layout()->count() ; i++)
                {
                    if (currentSpacerIndex == i) continue;

                    else if (event->position().y() < (this->layout()->itemAt(i)->widget()->pos().y() + this->layout()->itemAt(i)->widget()->size().height() - spacer->height() ))
                    {
                        if (currentSpacerIndex != -1) m_pBoxLayout->takeAt(currentSpacerIndex);
                        if (i == 0)
                        {
                            m_pBoxLayout->insertWidget(i, spacer);
                            currentSpacerIndex = i;
                            spacer->show();
                            return;
                        }
                        m_pBoxLayout->insertWidget(i - 1, spacer);
                        currentSpacerIndex = i - 1;
                        spacer->show();
                        return;
                    }
                    else if (i == this->layout()->count() - 1)
                    {
                        if (currentSpacerIndex != -1) m_pBoxLayout->takeAt(currentSpacerIndex);
                        m_pBoxLayout->insertWidget(i, spacer);
                        currentSpacerIndex = i;
                        spacer->show();
                        return;
                    }
                }
                return;
        case SlotSwap:
            //Old unfinished implementation
            /*
            if (event->source()->parent() == this) return;
            for (int i = 0 ; i < this->layout()->count() ; i++)
            {
                if ((event->position().y() < (this->layout()->itemAt(i)->widget()->pos().y() + this->layout()->itemAt(i)->widget()->size().height())) && (i != 0))
                {
                    //Highlight Widget
                    //m_pBoxLayout->itemAt(i)->widget()->setPalette(QColor(0xA0, 0xD8, 0xF1));
                    return;
                }
            }
            */
            return;
        }
    }
}

void SaveSlotListContent::dropEvent(QDropEvent *event)
{
    switch (currentState)
    {
    case Editor:
        if (currentSpacerIndex != -1)
        {
            m_pBoxLayout->takeAt(currentSpacerIndex);
            currentSpacerIndex = -1;
            spacer->hide();
        }

        removedWidget = m_pBoxLayout->takeAt(event->mimeData()->text().toInt());

        for (int i = 0 ; i < this->layout()->count() ; i++)
        {
            if ((event->position().y() < (this->layout()->itemAt(i)->widget()->pos().y() - (this->layout()->itemAt(i)->widget()->height() / 2) ) ) && (i != 0))
            {
                m_pBoxLayout->insertItem(i - 1, removedWidget);
                removedWidget->widget()->show();
                return;
            }
        }

        m_pBoxLayout->insertItem(layout()->count(), removedWidget);
        removedWidget->widget()->show();
        removedWidget = nullptr;
        return;

    case SlotSwap:
        //Old implementation, new one in saveslotdisplay
        /*
        if (event->source()->parent() == this) return;
        for (int i = 0 ; i < this->layout()->count() ; i++)
        {
            if (event->position().y() < (this->layout()->itemAt(i)->widget()->pos().y() + this->layout()->itemAt(i)->widget()->height()) )
            {
                if (listID == 0) mainWindow->SwapSlots(i, event->mimeData()->text().toInt());
                if (listID == 1) mainWindow->SwapSlots(event->mimeData()->text().toInt(), i);
                return;
            }
        }
        */
        return;
    }
}

void SaveSlotListContent::dragLeaveEvent(QDragLeaveEvent *event)
{
    switch (currentState)
    {
    case Editor:
        if (currentSpacerIndex != -1)
        {
            m_pBoxLayout->takeAt(currentSpacerIndex);
            currentSpacerIndex = -1;
            spacer->hide();
        }
        return;
    case SlotSwap:
        return;
    }
}

void SaveSlotListContent::updateState(State updatedState)
{
    currentState = updatedState;

    switch (currentState)
    {
    case Editor:
        //Accept input actions from self
        return;
    case SlotSwap:
        //Dont accept input actions from self
        return;
    }
}
