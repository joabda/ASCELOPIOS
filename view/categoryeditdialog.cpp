#include "categoryeditdialog.h"

CategoryEditDialog::CategoryEditDialog(CategoryDialog* parentDialog, QWidget* parent) :
    QDialog(parent),
    ui(new Ui::CategoryEditDialog)
{
    parentDialog_ = parentDialog;
    Category* selected_category = nullptr;
    if (parentDialog_->getListWidget()->selectedItems().size() != 0)
        selected_category = parentDialog_->getCategoryList()[parentDialog_->getListWidget()->currentIndex().row()];
    QVBoxLayout* vl = new QVBoxLayout;
    QHBoxLayout* hl = new QHBoxLayout;
    QHBoxLayout* hl2 = new QHBoxLayout;
    buttonColor_ = new QPushButton;
    if (selected_category != nullptr)
        selectedColor_ = QColor(selected_category->getColor().c_str());
    else
        selectedColor_ = QColor(DEFAULT_COLOR);
    QPixmap pixmap(ICON_SIZE, ICON_SIZE);
    pixmap.fill(selectedColor_);
    buttonColor_->setIcon(QIcon(pixmap));
    editName_ = new QLineEdit;
    if (selected_category != nullptr)
        editName_->setText(selected_category->getName().c_str());
    else
        editName_->setPlaceholderText("Type a name for the new category");
    hl->addWidget(editName_);
    hl->addWidget(buttonColor_);

    QPushButton* button_save = new QPushButton("&Save");
    QPushButton* button_delete = new QPushButton("&Delete");

    hl2->addWidget(button_save);
    hl2->addWidget(button_delete);
    vl->addLayout(hl);
    vl->addLayout(hl2);
    setModal(true);
    setWindowFlags(Qt::WindowCloseButtonHint);
    setWindowTitle("Category Edit");
    setLayout(vl);

    connect(buttonColor_, &QPushButton::clicked, this, &CategoryEditDialog::on_button_color_click);
    connect(button_delete, &QPushButton::clicked, this, &CategoryEditDialog::on_button_delete_click);
    connect(button_save, &QPushButton::clicked, this, &CategoryEditDialog::on_button_save_click);
}

CategoryEditDialog::~CategoryEditDialog()
{
    delete ui;
}

void CategoryEditDialog::on_button_save_click()
{
    if(editName_->text().length() < 3)
    {
        QMessageBox::critical(this, "Error", "The name must have a length greater than 2", QMessageBox::Ok);
        return;
    }
    Category category(0, editName_->text().toStdString(), selectedColor_.name().toStdString());
    //Create new category
    if ((parentDialog_->getListWidget()->selectedItems().size() == 0) && (parentDialog_->getPManager()->add_category(&category)))
    {
        parentDialog_->load_categories();
        close();
        delete this;
    }
    else //Change the selected category
    if (parentDialog_->getPManager()->replace_category(parentDialog_->getCategoryList()[parentDialog_->getListWidget()->currentIndex().row()], &category))
    {
        parentDialog_->load_categories();
        parentDialog_->getParentView()->refresh_events();
        close();
        delete this;
    }
    else
        QMessageBox::critical(this, "Error", "Persistence error. Try with a different name or color.");
}

void CategoryEditDialog::on_button_delete_click()
{
    if (parentDialog_->getListWidget()->selectedItems().size() == 0)
    {
        QMessageBox::critical(this, "Error", "No category selected", QMessageBox::Ok);
        return;
    }
    QString selected_category = parentDialog_->getListWidget()->selectedItems().at(0)->text();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm", QString("Do you want to delete ") + selected_category + QString("?"), QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        if (!parentDialog_->getPManager()->remove_category(parentDialog_->getCategoryList()[parentDialog_->getListWidget()->currentIndex().row()]))
            QMessageBox::critical(this, "Error", "Persistence error. You can't remove a category used by events. First remove these events and then try again.");
        else
        {
            parentDialog_->load_categories();
            close();
            delete this;
        }
    }
}

void CategoryEditDialog::on_button_color_click()
{
    QColor color = QColorDialog::getColor(QColor(selectedColor_));
    if (color.isValid())
    {
        selectedColor_ = color;
        QPixmap pixmap(ICON_SIZE, ICON_SIZE);
        pixmap.fill(QColor(selectedColor_));
        buttonColor_->setIcon(QIcon(pixmap));
    }
}
