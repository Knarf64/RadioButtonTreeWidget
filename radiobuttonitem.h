#ifndef RADIOBUTTONITEM_H
#define RADIOBUTTONITEM_H

#include <QTreeWidgetItem>

class RootItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    RootItem(QTreeWidget *parent, const QString &text);
    virtual ~RootItem();

public slots:
    void preserveMutualExclusion(QTreeWidgetItem* lastChecked);
};

class RadioButtonItem : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
public:
    RadioButtonItem(RootItem* parent, const QString& name);
    ~RadioButtonItem();
signals:
    void toogleCheck(QTreeWidgetItem *);
protected:
    void setData(int column, int role, const QVariant & value);
private:
    RootItem* m_parent;

};

#endif
