// taken from example at https://wiki.qt.io/Combo_Boxes_in_Item_Views

#pragma once

#include <QStyledItemDelegate>

class CheckBoxItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    CheckBoxItemDelegate(QObject *parent = nullptr);
    ~CheckBoxItemDelegate();

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;
};
