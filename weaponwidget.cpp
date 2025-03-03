#include "weaponwidget.h"
#include "ui_weaponwidget.h"

#include "mainwindow.h"

WeaponWidget::WeaponWidget(QWidget *parent, QString labelText, size_t weaponCountOffset, size_t upgradeByteOffset, size_t upgradeBitIndex)
    : QWidget(parent)
    , ui(new Ui::WeaponWidget)
{
    ui->setupUi(this);

    m_weaponCountOffset = weaponCountOffset;
    m_upgradeByteOffset = upgradeByteOffset;
    m_upgradeBitIndex = upgradeBitIndex;

    ui->label->setText(labelText);
    ui->spinBox->setMaximum(65535);

    parent->layout()->addWidget(this);

    connect(mainWindow, &MainWindow::LoadValuesToEditor, this, &WeaponWidget::ReadValues);
    connect(mainWindow, &MainWindow::SaveValuesFromEditor, this, &WeaponWidget::WriteValues);
}

WeaponWidget::~WeaponWidget()
{
    delete ui;
}

void WeaponWidget::ReadValues(SaveSlot *saveSlot)
{
    short temp;
    temp = *(short*)(((size_t)saveSlot) + m_weaponCountOffset);
    ui->spinBox->setValue(temp);

    temp = *(short*)(((size_t)saveSlot) + m_upgradeByteOffset);
    ui->checkBox->setChecked((temp & (0b00000001 << m_upgradeBitIndex)));
}

void WeaponWidget::WriteValues(SaveSlot *saveSlot)
{
    short temp;
    temp = ui->spinBox->value();
    *(short*)(((size_t)saveSlot) + m_weaponCountOffset) = temp;

    temp = *(short*)(((size_t)saveSlot) + m_upgradeByteOffset);
    *(short*)(((size_t)saveSlot) + m_upgradeByteOffset) = ((temp & ((0b00000001 << m_upgradeBitIndex) ^ 0b11111111)) | ((ui->checkBox->isChecked() == true) << m_upgradeBitIndex));
}
