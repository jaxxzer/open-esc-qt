#include <register-model.h>


RegisterModel::RegisterModel(QObject* parent, QVector<register_t>* _registerList)
    : QAbstractTableModel(parent)
    , registerList(_registerList)
{
}


int RegisterModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    if (registerList) {
        return registerList->size();
    }
    return 0;
}

int RegisterModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return 3;
}

QVariant RegisterModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return (*registerList)[index.row()].address;
        case 1:
            return (*registerList)[index.row()].value;
        case 2:
            return (*registerList)[index.row()].name;
        default:
            return QString("error");
        }
    }
    return QVariant();
}

Qt::ItemFlags RegisterModel::flags(const QModelIndex &index) const
{
    if (index.column() == 1) {
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
    return QAbstractItemModel::flags(index);
}

bool RegisterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        (*registerList)[index.row()].value = value.toInt();
        emit registerEdited(index.row());
        return true;
    }
    return false;
}

bool RegisterModel::insertRows(int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)
    return false;
}

bool RegisterModel::insertColumns(int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)
    return false;
}

bool RegisterModel::removeRows(int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)
    return false;
}

bool RegisterModel::removeColumns(int row, int count, const QModelIndex& parent)
{
    Q_UNUSED(row)
    Q_UNUSED(count)
    Q_UNUSED(parent)
    return false;
}
