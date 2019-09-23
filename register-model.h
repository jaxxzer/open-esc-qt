#ifndef REGISTERMODEL_H
#define REGISTERMODEL_H

#include <QAbstractTableModel>
class RegisterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertColumns(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int row, int count, const QModelIndex& parent = QModelIndex()) override;

};

#endif // REGISTERMODEL_H
