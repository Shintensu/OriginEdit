#ifndef POINTERLISTMODEL_H
#define POINTERLISTMODEL_H

#include <QAbstractListModel>
#include "SaveSlot.h"

class PointerListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    struct entryData
    {
        QString saveSlotNumber;
        SaveSlot* ptr_saveSlot;
    };
public:
    explicit PointerListModel(QObject *parent = nullptr, SaveSlot* saveSlotList = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild) override;
    Qt::DropActions supportedDropActions() const override;
private:
    QList<entryData> m_dataList;
};

#endif // POINTERLISTMODEL_H
