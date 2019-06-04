#ifndef CATEGORYEDITDIALOG_H
#define CATEGORYEDITDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLineEdit>
#include <QColorDialog>
#include "../view.UI/ui_categoryeditdialog.h"
#include "model/category.h"
#include "categorydialog.h"

//We need this forward declaration to avoid circular dependencies among headers
class CategoryDialog;

namespace Ui {
class CategoryEditDialog;
}

class CategoryEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryEditDialog(CategoryDialog* parentDialog, QWidget* parent = nullptr);
    ~CategoryEditDialog();

public slots:
    void on_button_delete_click();
    void on_button_save_click();
    void on_button_color_click();

private:
    Ui::CategoryEditDialog* ui;
    CategoryDialog* parentDialog_;
    QColor selectedColor_;
    QLineEdit* editName_;
    QPushButton *buttonColor_;
};

#endif
