#ifndef CATEGORYSELECTDIALOG_H
#define CATEGORYSELECTDIALOG_H

#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include "../view.UI/ui_categoryselectdialog.h"
#include "view.h"
#include "pmanager.h"
#include "categorydialog.h"

namespace Ui {
class CategorySelectDialog;
}

class CategorySelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategorySelectDialog(View* parentView, QString text, QWidget* parent = nullptr);
    ~CategorySelectDialog();
    void load_categories();
    Category* getSelectedCategory() const;

public slots:
    void on_button_cancel_click();
    void on_button_ok_click();

private:
    Ui::CategorySelectDialog* ui;
    QComboBox* listCategories_;
    vector<Category*> categoryList_;
    PManager* pm_;
    View* parent_;
    Category* selectedCategory_;
};

#endif // CATEGORYSELECTDIALOG_H
