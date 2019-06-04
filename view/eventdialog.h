#ifndef EVENTDIALOG_H
#define EVENTDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProgressBar>
#include <QDateTimeEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QComboBox>
#include <QMessageBox>
#include <QIconEngine>
#include <QPixmap>
#include <QIcon>
#include "util/dateutil.h"
#include "view.h"
#include "pmanager.h"
#include "../view.UI/ui_eventdialog.h"

namespace Ui {
class EventDialog;
}

class EventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EventDialog(View* parentView, Date start_date = Date(), Date end_date = Date(), QWidget* parent = nullptr);
    ~EventDialog();
    void setEvent(Event* event);
    Event* getEvent() const;

public slots:
    void on_button_cancel_click();
    void on_button_save_click();
    void on_button_delete_click();
    void on_date_start_change();
    void on_checkbox_todo_toggle(bool checked);

private:
    Ui::EventDialog* ui;
    QLineEdit* editName_;
    QLineEdit* editPlace_;
    QComboBox* editCategory_;
    QDateTimeEdit* editStart_;
    QDateTimeEdit* editEnd_;
    QPlainTextEdit* editDescription_;
    QPushButton* buttonDelete_;
    QCheckBox* cbtodo_;
    QRadioButton* everyMonth_;
    QRadioButton* everyYear_;
    QGroupBox *options;
    View* parent_;
    PManager* pm_;
    Event* event_;
    vector<Category *> categoryList_;
    void refresh();
};

#endif // EVENTDIALOG_H
