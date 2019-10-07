#include <register-model.h>


RegisterModel::RegisterModel(QObject* parent, QVector<register_t>* _registerList, uint8_t* data)
    : QAbstractTableModel(parent)
    , registerList(_registerList)
    , _data(data)
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
            switch ((*registerList)[index.row()].type) {
            case REG_TYPE_BOOL:
                return (bool&)_data[(*registerList)[index.row()].address];
            case REG_TYPE_UINT8:
                return (uint8_t&)_data[(*registerList)[index.row()].address];
            case REG_TYPE_UINT16:
                return (uint16_t&)_data[(*registerList)[index.row()].address];
            case REG_TYPE_UINT32:
                return (uint32_t&)_data[(*registerList)[index.row()].address];
            }
            break;
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
    if (index.column() == 1 && (*registerList)[index.row()].mode == REG_MODE_READWRITE) {
        return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
    }
    return QAbstractItemModel::flags(index);
}

bool RegisterModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role == Qt::EditRole) {
        if ((*registerList)[index.row()].mode == REG_MODE_READONLY) {
            return false;
        }
        switch ((*registerList)[index.row()].type) {
        case REG_TYPE_BOOL:
            if (((bool*)_data)[(*registerList)[index.row()].address] == value.toBool()) {
                return false;
            }
            ((bool*)_data)[(*registerList)[index.row()].address] = value.toBool();
            emit registerEdited(index.row());

            return true;
        case REG_TYPE_UINT8:
            if (((uint8_t*)_data)[(*registerList)[index.row()].address] == value.toInt()) {
                return false;
            }
            ((uint8_t*)_data)[(*registerList)[index.row()].address] = value.toInt();
            emit registerEdited(index.row());

            return true;
        case REG_TYPE_UINT16:
            if ((uint16_t&)_data[(*registerList)[index.row()].address] == value.toInt()) {
                return false;
            }
            (uint16_t&)_data[(*registerList)[index.row()].address] = value.toInt();
            emit registerEdited(index.row());

            return true;
        case REG_TYPE_UINT32:
            if ((uint32_t&)_data[(*registerList)[index.row()].address] == value.toInt()) {
                return false;
            }
            (uint32_t&)_data[(*registerList)[index.row()].address] = value.toInt();
            emit registerEdited(index.row());

            return true;
        }
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
