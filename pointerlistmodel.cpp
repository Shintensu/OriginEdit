#include "pointerlistmodel.h"
#include <QStringListModel>

PointerListModel::PointerListModel(QObject *parent, SaveSlot* saveSlotList)
    : QAbstractListModel{parent}
{
    ;
    for (int i = 0; i < 30 ; i ++)
    {
        QString saveSlotName = "Save Slot ";
        saveSlotName.append(QString::number(i + 1, 10));
        m_dataList.append(entryData{saveSlotName, &(saveSlotList[i])});
    }
}

int PointerListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_dataList.count();
}

QVariant PointerListModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > m_dataList.count()) return QVariant();
    const entryData &item = m_dataList[index.row()];
    switch (role)
    {
    case Qt::DisplayRole:
        return item.saveSlotNumber;
    case 0x100:
        return (unsigned long long)item.ptr_saveSlot;
    default:
        return QVariant();
    }
}

Qt::ItemFlags PointerListModel::flags(const QModelIndex &index) const
{
    Qt::ItemFlags defaultFlags;

    if (!index.isValid())
        defaultFlags = QAbstractListModel::flags(index) | Qt::ItemIsDropEnabled;
    else
        defaultFlags = QAbstractListModel::flags(index) | Qt::ItemIsEditable | Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled;

    if (index.isValid()){
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
    }
    return defaultFlags;
}

bool PointerListModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if (sourceRow < 0
        || sourceRow + count - 1 >= rowCount(sourceParent)
        || destinationChild < 0
        || destinationChild > rowCount(destinationParent)
        || sourceRow == destinationChild
        || sourceRow == destinationChild - 1
        || count <= 0
        || sourceParent.isValid()
        || destinationParent.isValid()) {
        return false;
    }
    if (!beginMoveRows(QModelIndex(), sourceRow, sourceRow + count - 1, QModelIndex(), destinationChild))
        return false;

    int fromRow = sourceRow;
    if (destinationChild < sourceRow)
        fromRow += count - 1;
    else
        destinationChild--;
    while (count--)
        m_dataList.move(fromRow, destinationChild);
    endMoveRows();
    return true;
}

Qt::DropActions PointerListModel::supportedDropActions() const
{
    return QAbstractItemModel::supportedDropActions() | Qt::MoveAction;
}

