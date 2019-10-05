#ifndef REGISTERMODEL_H
#define REGISTERMODEL_H

#include <QAbstractTableModel>

#include <QVector>
class RegisterModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    typedef struct
    {
        uint16_t address;
        QString name;
        uint32_t value;
    } register_t;

    RegisterModel(QObject* parent = nullptr, QVector<register_t>* _registerList = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    bool insertRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool insertColumns(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex& parent = QModelIndex()) override;
    bool removeColumns(int row, int count, const QModelIndex& parent = QModelIndex()) override;

    QVector<register_t>* registerList;
signals:
    void registerEdited(int index);
};

#endif // REGISTERMODEL_H
