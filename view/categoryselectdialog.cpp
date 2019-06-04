#include "categoryselectdialog.h"

#include <QDebug>

CategorySelectDialog::CategorySelectDialog(View* parentView, QString text, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CategorySelectDialog)
{
    parent_ = parentView;
    setWindowTitle("Category Selector");
    pm_ = new PManager;
    selectedCategory_ = nullptr;
    QHBoxLayout* button_layout = new QHBoxLayout;
    QVBoxLayout* main_layout = new QVBoxLayout;
    listCategories_ = new QComboBox;
    load_categories();
    QPushButton* button_ok = new QPushButton("OK");
    QPushButton* button_cancel = new QPushButton("Cancel");
    button_cancel->setFixedWidth(50);
    button_ok->setFixedWidth(50);
    connect(button_cancel, &QPushButton::clicked, this, &CategorySelectDialog::on_button_cancel_click);
    connect(button_ok, &QPushButton::clicked, this, &CategorySelectDialog::on_button_ok_click);
    QLabel* message = new QLabel(text);
    main_layout->addWidget(message);
    main_layout->addWidget(listCategories_);
    button_layout->addWidget(button_ok);
    button_layout->addWidget(button_cancel);
    main_layout->addLayout(button_layout);
    setLayout(main_layout);
}

void CategorySelectDialog::load_categories()
{
    for (Category* category : categoryList_)
        delete category;
    categoryList_ = pm_->get_categories();
    for (Category* category : categoryList_)
    {
        QPixmap pixmap(10, 10);
        pixmap.fill(QColor(category->getColor().c_str()));
        listCategories_->addItem(QIcon(pixmap), QString(category->getName().c_str()));
    }
}

void CategorySelectDialog::on_button_cancel_click()
{
    selectedCategory_ = nullptr;
    close();
    delete this;
}

void CategorySelectDialog::on_button_ok_click()
{
    for (Category* c : categoryList_)
        if (listCategories_->currentText().toStdString() == c->getName())
        {
            selectedCategory_ = new Category(*c);
            break;
        }
    close();
    delete this;
}

Category* CategorySelectDialog::getSelectedCategory() const
{
    return selectedCategory_;
}

CategorySelectDialog::~CategorySelectDialog()
{
    delete ui;
    delete pm_;
    for (Category* category : categoryList_)
        delete category;
}
