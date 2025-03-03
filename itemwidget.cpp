#include "itemwidget.h"
#include "ui_itemwidget.h"

#include "mainwindow.h"

ItemWidget::ItemWidget(QWidget *parent, QString labelText, size_t itemCountOffset)
    : QWidget(parent)
    , ui(new Ui::ItemWidget)
{
    ui->setupUi(this);

    m_itemCountOffset = itemCountOffset;

    ui->label->setText(labelText);
    ui->spinBox->setMaximum(65535);

    parent->layout()->addWidget(this);

    connect(mainWindow, &MainWindow::LoadValuesToEditor, this, &ItemWidget::ReadValues);
    connect(mainWindow, &MainWindow::SaveValuesFromEditor, this, &ItemWidget::WriteValues);
}

ItemWidget::~ItemWidget()
{
    delete ui;
}

void ItemWidget::ReadValues(SaveSlot *saveSlot)
{
    short temp;
    temp = *(short*)(((size_t)saveSlot) + m_itemCountOffset);
    ui->spinBox->setValue(temp);
}

void ItemWidget::WriteValues(SaveSlot *saveSlot)
{
    short temp;
    temp = ui->spinBox->value();
    *(short*)(((size_t)saveSlot) + m_itemCountOffset) = temp;
}
