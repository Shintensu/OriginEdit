#ifndef WEAPONWIDGET_H
#define WEAPONWIDGET_H

#include <QWidget>

#include "SaveSlot.h"

namespace Ui {
class WeaponWidget;
}

class WeaponWidget : public QWidget
{
    Q_OBJECT

public:
    explicit WeaponWidget(QWidget *parent = nullptr,  QString labelText = "Weapon", size_t weaponCountOffset = 0, size_t upgradeByteOffset = 0, size_t upgradeBitIndex = 0);
    ~WeaponWidget();
public slots:
    void ReadValues(SaveSlot* saveSlot);
    void WriteValues(SaveSlot* saveSlot);

private:
    size_t m_weaponCountOffset;
    size_t m_upgradeByteOffset;
    size_t m_upgradeBitIndex;

    Ui::WeaponWidget *ui;
};

#endif // WEAPONWIDGET_H
