#include <combobox-delegate.h>
#include <QCheckBox>

CheckBoxItemDelegate::CheckBoxItemDelegate(QObject *parent)
    : QStyledItemDelegate(parent)
{
}


CheckBoxItemDelegate::~CheckBoxItemDelegate()
{
}


QWidget *CheckBoxItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Create the combobox and populate it
    QCheckBox *cb = new QCheckBox(parent);

    return cb;
}


void CheckBoxItemDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
    Q_ASSERT(cb);
    cb->setChecked(index.data(Qt::CheckStateRole).toBool());
}


void CheckBoxItemDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const
{
    QCheckBox* cb = qobject_cast<QCheckBox*>(editor);
    Q_ASSERT(cb);
    model->setData(index, cb->isChecked(), Qt::CheckStateRole);
}
