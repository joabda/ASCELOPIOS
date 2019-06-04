#include "settings.h"

#include <QDebug>

const QString SettingsValues::past_months_expiration = "PastMonthsExpiration";

Settings::Settings(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::Settings)
{
    setWindowFlags(Qt::WindowCloseButtonHint | Qt::WindowMinimizeButtonHint);
    setWindowTitle("Settings");
    QVBoxLayout* main_layout = new QVBoxLayout;
    QHBoxLayout* row1 = new QHBoxLayout;
    QLabel* lblPastMonthExpiration = new QLabel("Delete all events older than (months): ");
    pastMonthsExpiration_ = new QSpinBox;
    pastMonthsExpiration_->setRange(0, 120);
    pastMonthsExpiration_->setSingleStep(1);
    pastMonthsExpiration_->setValue(2);
    pastMonthsExpiration_->setFixedWidth(100);
    row1->addWidget(lblPastMonthExpiration);
    row1->addWidget(pastMonthsExpiration_);
    QHBoxLayout* lastRow = new QHBoxLayout;
    QPushButton* btnCancel = new QPushButton("Cancel");
    QPushButton* btnSave = new QPushButton("Save");
    connect(btnCancel, &QPushButton::clicked, this, &Settings::on_button_cancel_click);
    connect(btnSave, &QPushButton::clicked, this, &Settings::on_button_save_click);
    lastRow->addWidget(btnCancel);
    lastRow->addWidget(btnSave);
    main_layout->addLayout(row1);
    main_layout->addLayout(lastRow);
    setLayout(main_layout);
}

Settings::~Settings()
{
    delete ui;
}

void Settings::on_button_cancel_click()
{
   close();
}

void Settings::on_button_save_click()
{
    QSettings settings;
    settings.setValue(SettingsValues::past_months_expiration, pastMonthsExpiration_->value());
    close();
}


