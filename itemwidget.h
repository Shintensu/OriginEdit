#ifndef ITEMWIDGET_H
#define ITEMWIDGET_H

#include <QWidget>

#include "SaveSlot.h"

namespace Ui {
class ItemWidget;
}

class ItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ItemWidget(QWidget *parent = nullptr, QString labelText = "Item", size_t itemCountOffset = 0);
    ~ItemWidget();
public slots:
    void ReadValues(SaveSlot* saveSlot);
    void WriteValues(SaveSlot* saveSlot);

private:
    size_t m_itemCountOffset;

    Ui::ItemWidget *ui;
};

#endif // ITEMWIDGET_H
