#ifndef SETTINGS_H
#define SETTINGS_H

#include <QDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLayout>
#include <QSettings>
#include "pmanager.h"
#include "../view.UI/ui_settings.h"

namespace Ui {
class Settings;
}

struct SettingsValues {
    /* initialization in settings.cpp */
    static const QString past_months_expiration;
};

class Settings : public QDialog
{
    Q_OBJECT

public:
    explicit Settings(QWidget* parent = nullptr);
    ~Settings();
    void on_button_cancel_click();
    void on_button_save_click();

private:
    Ui::Settings* ui;
    QSpinBox* pastMonthsExpiration_;

};

#endif // SETTINGS_H
