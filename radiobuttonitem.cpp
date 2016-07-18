#include "radiobuttonitem.h"

#include <QDebug>

Q_DECLARE_METATYPE(QTreeWidgetItem*)

RootItem::RootItem(QTreeWidget *parent, const QString &text)
    : QTreeWidgetItem(parent, QStringList(text))
{
    QTreeWidgetItem::setExpanded(true);
    parent->setItemsExpandable(false);
    setFlags(flags() & ~Qt::ItemIsUserCheckable);
}

RootItem::~RootItem()
{
}

void RootItem::preserveMutualExclusion(QTreeWidgetItem *lastChecked)
{
    for (int i = 0; i < childCount(); ++i) {
        QTreeWidgetItem* childItem = QTreeWidgetItem::child(i);
        if (childItem && childItem != lastChecked)
            childItem->setCheckState(0, Qt::Unchecked);
    }
}

RadioButtonItem::RadioButtonItem(RootItem *parent, const QString &name)
    : QTreeWidgetItem(parent, QStringList(name))
    , m_parent(parent)
{
    setCheckState(0, Qt::Unchecked);
    QObject::connect(this, SIGNAL(toogleCheck(QTreeWidgetItem *)),
                     parent, SLOT(preserveMutualExclusion(QTreeWidgetItem*)));

}

RadioButtonItem::~RadioButtonItem()
{
}

void RadioButtonItem::setData(int column, int role, const QVariant &value)
{
    if (role == Qt::CheckStateRole && value.canConvert<int>() && value.toInt() == Qt::Checked)
        emit toogleCheck(this);

    QTreeWidgetItem::setData(column, role, value);
}
