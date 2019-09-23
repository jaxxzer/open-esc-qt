#include <register-model.h>

bool RegisterModel::insertRows(int row, int count, const QModelIndex& parent)
{
//    QAbstractItemModel::beginInsertRows();
    endInsertRows();
    return false;
}
bool RegisterModel::insertColumns(int row, int count, const QModelIndex& parent)
{
    return false;
}
bool RegisterModel::removeRows(int row, int count, const QModelIndex& parent)
{
    return false;
}
bool RegisterModel::removeColumns(int row, int count, const QModelIndex& parent)
{
    return false;
}

