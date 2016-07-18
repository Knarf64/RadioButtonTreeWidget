#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QRadioButton>
#include <QPainter>
#include <QStyledItemDelegate>
#include <QDebug>

#include "radiobuttonitem.h"

class TreeWidgetItemRadioButtonDelegate : public QStyledItemDelegate
{
public:
    TreeWidgetItemRadioButtonDelegate(QObject *parent = 0):QStyledItemDelegate(parent){}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        if (index.column() != 0 || !index.parent().isValid())
            return QStyledItemDelegate::paint(painter, option, index);

        int radioButtonWidth = QApplication::style()->pixelMetric(QStyle::PM_ExclusiveIndicatorWidth, &option);
        int spacing = QApplication::style()->pixelMetric(QStyle::PM_RadioButtonLabelSpacing, &option);

        // draw background if it's selected
        if (option.state & QStyle::State_Selected)
            painter->fillRect(option.rect, qApp->palette().color(QPalette::Highlight));

        // draw the text
        QStyleOptionViewItem optionText = option;
        int left = optionText.rect.left();
        optionText.rect.adjust(spacing * 2  + radioButtonWidth, 0, 0, 0);
        QApplication::style()->drawItemText(painter, optionText.rect, Qt::AlignRight & Qt::AlignVCenter,
                                            qApp->palette(), option.state & QStyle::State_Enabled, index.data(Qt::DisplayRole).toString());

        // draw the radio button
        QStyleOptionButton *optionRadioButton = new QStyleOptionButton();
        optionRadioButton->rect = option.rect;
        optionRadioButton->state = option.state;
        optionRadioButton->rect.setLeft(left + spacing / 2);
        optionRadioButton->rect.setWidth(radioButtonWidth + spacing / 2);
        bool radioButtonState = index.data(Qt::CheckStateRole).toInt();
        optionRadioButton->state |= radioButtonState == 0 ? QStyle::State_Off : QStyle::State_On;

        QApplication::style()->drawPrimitive(QStyle::PE_IndicatorRadioButton, optionRadioButton, painter);
        delete optionRadioButton;
    }

    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
    {
        QSize s = QStyledItemDelegate::sizeHint(option, index);
        /// determine size of radio buttons in current style
        int radioButtonHeight = QApplication::style()->pixelMetric(QStyle::PM_ExclusiveIndicatorHeight, &option);
        /// ensure that line is tall enough to draw radio button
        s.setHeight(qMax(s.height(), radioButtonHeight));
        return s;
    }
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->treeWidget->setColumnCount(1);
    ui->treeWidget->setItemDelegate(new TreeWidgetItemRadioButtonDelegate);
    ui->treeWidget->setRootIsDecorated(false);

    RootItem* root = new RootItem(ui->treeWidget, "root");
    for (int i=0; i < 4; ++i)
    {
        new RadioButtonItem(root, "name" + QString::number(i));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
