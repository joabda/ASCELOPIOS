#ifndef CATEGORYDIALOG_H
#define CATEGORYDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include "pmanager.h"
#include "categoryeditdialog.h"
#include "view.h"
#include "../view.UI/ui_categorydialog.h"

#define DEFAULT_COLOR "#1022A0"
#define ICON_SIZE 10

namespace Ui {
class CategoryDialog;
}

class CategoryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CategoryDialog(View* parentView, QWidget* parent = nullptr);
    ~CategoryDialog();
    QListWidget* getListWidget() const;
    vector<Category*> getCategoryList() const;
    PManager* getPManager() const;
    View* getParentView() const;
    void load_categories();

public slots:
    void on_button_cancel_click();
    void on_button_add_click();
    void on_item_click();

private:
    Ui::CategoryDialog* ui;
    QListWidget* listWidget_;
    vector<Category*> categoryList_;
    PManager* pm_;
    View* parent_;
};

#endif // CATEGORYDIALOG_H
