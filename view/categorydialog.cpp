#include "categorydialog.h"

#include <QDebug>
#include <functional>

CategoryDialog::CategoryDialog(View* parentView, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CategoryDialog)
{
    parent_ = parentView;
    setFixedWidth(300);
    setFixedHeight(400);
    setWindowTitle("Category Manager");
    pm_ = new PManager;
    QVBoxLayout *layout = new QVBoxLayout;
    listWidget_ = new QListWidget;
    connect(listWidget_, &QListWidget::itemClicked, this, &CategoryDialog::on_item_click);
    load_categories();
    QPushButton* button_add = new QPushButton("&Add new category");
    QPushButton* button_cancel = new QPushButton("&Cancel");
    connect(button_cancel, &QPushButton::clicked, this, &CategoryDialog::on_button_cancel_click);
    connect(button_add, &QPushButton::clicked, this, &CategoryDialog::on_button_add_click);
    layout->addWidget(listWidget_);
    layout->addWidget(button_add);
    layout->addWidget(button_cancel);
    setLayout(layout);
}

CategoryDialog::~CategoryDialog()
{
    delete ui;
    delete pm_;
    for (Category* category : categoryList_)
        delete category;
}

void CategoryDialog::load_categories()
{
    for (Category *category : categoryList_)
        delete category;
    categoryList_ = pm_->get_categories();
    listWidget_->clear();
    for (Category* category : categoryList_)
    {
        QPixmap pixmap(ICON_SIZE, ICON_SIZE);
        pixmap.fill(QColor(category->getColor().c_str()));
        listWidget_->addItem(new QListWidgetItem(QIcon(pixmap), QString(category->getName().c_str())));
    }
}

void CategoryDialog::on_item_click()
{
    CategoryEditDialog* category_edit_dialog = new CategoryEditDialog(this);
    category_edit_dialog->show();
}

void CategoryDialog::on_button_cancel_click()
{
    close();
    delete this;
}

void CategoryDialog::on_button_add_click()
{
    CategoryEditDialog* category_edit_dialog = new CategoryEditDialog(this);
    category_edit_dialog->show();
}

vector<Category*> CategoryDialog::getCategoryList() const
{
    return categoryList_;
}

QListWidget* CategoryDialog::getListWidget() const
{
    return listWidget_;
}

PManager* CategoryDialog::getPManager() const
{
    return pm_;
}
View* CategoryDialog::getParentView() const
{
    return parent_;
}
