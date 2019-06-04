#include "eventdialog.h"

#include <QDebug>

void EventDialog::setEvent(Event* event)
{
    everyMonth_->setEnabled(false); /* Not supported for now */
    everyYear_->setEnabled(false);
    event_ = event;
    if (event != nullptr) {
        editName_->setText(event->getName().c_str());
        editDescription_->setPlainText(event->getDescription().c_str());
        editPlace_->setText(event->getPlace().c_str());
        int index = 0;
        for (Category* category : categoryList_)
        {
            if (category->getName() == event->getCategory()->getName())
                editCategory_->setCurrentIndex(index);
            index++;
        }

        if (event->getStart() == TODO_DATE)
        {
            Date today = DateUtil::get_current_date();
            /* Set the current date, so if the user uncheck the "TODO" there is already a valid date set */
            editStart_->setDateTime(QDateTime(QDate(today.getYear(), today.getMonth(), today.getMonthDay())));
            editStart_->setEnabled(false);
            editEnd_->setDateTime(QDateTime(QDate(today.getYear(), today.getMonth(), today.getMonthDay())));
            editEnd_->setEnabled(false);
            cbtodo_->setChecked(true);
        }
        else
        {
            editStart_->setDateTime(QDateTime::fromTime_t(event->getStart()));
            editEnd_->setDateTime(QDateTime::fromTime_t(event->getEnd()));
        }
        buttonDelete_->setEnabled(true);
    }
}

EventDialog::EventDialog(View *parentView, Date start_date, Date end_date, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EventDialog)
{
    parent_ = parentView;
    setFixedWidth(400);
    setFixedHeight(500);
    setModal(true);
    setWindowTitle("Event Manager");

    pm_ = new PManager;
    event_ = nullptr;

    QVBoxLayout* main_layout = new QVBoxLayout;
    QHBoxLayout* first_row = new QHBoxLayout;
    QLabel* label_name = new QLabel("Name: ");
    editName_ = new QLineEdit;
    editName_->setMaxLength(100);
    first_row->addWidget(label_name);
    first_row->addWidget(editName_);
    main_layout->addLayout(first_row);
    QHBoxLayout *second_row = new QHBoxLayout;
    QLabel *label_place = new QLabel("Place: ");
    editPlace_ = new QLineEdit;
    second_row->addWidget(label_place);
    second_row->addWidget(editPlace_);
    main_layout->addLayout(second_row);
    main_layout->addWidget(new QLabel("Description: "));
    editDescription_ = new QPlainTextEdit;
    main_layout->addWidget(editDescription_);
    QHBoxLayout *third_row = new QHBoxLayout;
    QLabel* label_category = new QLabel("Category: ");
    editCategory_ = new QComboBox;
    categoryList_ = pm_->get_categories();
    for (Category* category : categoryList_)
    {
        QPixmap pixmap(10, 10);
        pixmap.fill(QColor(category->getColor().c_str()));
        editCategory_->addItem(QIcon(pixmap), QString(category->getName().c_str()));
    }
    third_row->addWidget(label_category);
    third_row->addWidget(editCategory_);
    main_layout->addLayout(third_row);
    QHBoxLayout* fourth_row = new QHBoxLayout;
    QLabel* label_todo = new QLabel("TODO: ");
    cbtodo_ = new QCheckBox;
    connect(cbtodo_, &QCheckBox::toggled, this, &EventDialog::on_checkbox_todo_toggle);
    fourth_row->addWidget(label_todo);
    fourth_row->addWidget(cbtodo_);
    main_layout->addLayout(fourth_row);
    QHBoxLayout *fifth_row = new QHBoxLayout;
    QLabel *label_start = new QLabel("Start: ");
    editStart_ = new QDateTimeEdit;
    editStart_->setCalendarPopup(true);
    QDateTime todoDate = QDateTime::fromTime_t(TODO_DATE);
    editStart_->setDateTime(QDateTime(QDate(start_date.getYear(), start_date.getMonth(), start_date.getMonthDay())));
    //I set a specific hour because the default is the midnight, but this could lead more easily problems caused by daylight saving time
    editStart_->setTime(QTime(8,0,0));
    connect(editStart_, &QDateTimeEdit::dateTimeChanged, this, &EventDialog::on_date_start_change);
    fifth_row->addWidget(label_start);
    fifth_row->addWidget(editStart_);
    main_layout->addLayout(fifth_row);
    QHBoxLayout* sixth_row = new QHBoxLayout;
    QLabel* label_end = new QLabel("End: ");
    editEnd_ = new QDateTimeEdit;
    editEnd_->setCalendarPopup(true);
    editEnd_->setDateTime(QDateTime(QDate(end_date.getYear(), end_date.getMonth(), end_date.getMonthDay())));
    editEnd_->setTime(QTime(9,0,0));
    sixth_row->addWidget(label_end);
    sixth_row->addWidget(editEnd_);
    main_layout->addLayout(sixth_row);
    QHBoxLayout *seventh_row = new QHBoxLayout;
    QLabel *label_recurrent = new QLabel("Recurrence: ");
    everyMonth_ = new QRadioButton("Monthly");
    everyYear_ = new QRadioButton("Yearly");
    options = new QGroupBox;
    seventh_row->addWidget(label_recurrent);
    seventh_row->addWidget(everyMonth_);
    seventh_row->addWidget(everyYear_);
    options->setLayout(seventh_row);
    main_layout->addWidget(options);
    QHBoxLayout *last_row = new QHBoxLayout;
    QPushButton *button_cancel = new QPushButton("&Cancel");
    connect(button_cancel, &QPushButton::clicked, this, &EventDialog::on_button_cancel_click);
    buttonDelete_ = new QPushButton("&Delete");
    buttonDelete_->setEnabled(false);
    connect(buttonDelete_, &QPushButton::clicked, this, &EventDialog::on_button_delete_click);
    QPushButton *button_save = new QPushButton("&Save");
    connect(button_save, &QPushButton::clicked, this, &EventDialog::on_button_save_click);
    last_row->addWidget(button_cancel);
    last_row->addWidget(buttonDelete_);
    last_row->addWidget(button_save);
    main_layout->addLayout(last_row);

    setLayout(main_layout);
}

EventDialog::~EventDialog()
{
    delete ui;
    delete pm_;
    for (Category* c : categoryList_)
        delete c;
}

void EventDialog::on_button_cancel_click()
{
    close();
    delete this;
}

void EventDialog::on_button_delete_click()
{
    pm_->remove_event(event_);
    event_->setInvalid();
    refresh();
}

void EventDialog::refresh()
{
    parent_->refresh_events();
    close();
    delete this;
}

void EventDialog::on_button_save_click()
{
    QProgressBar* bar = new QProgressBar;
    bool ret = true;
    bool isTodo = !editStart_->isEnabled();

    if (editName_->text().length() < 3)
    {
        QMessageBox::critical(this, "Error", "The name must have a length greater than 2", QMessageBox::Ok);
        return;
    }

    if (isTodo)
    {
        QDateTime todoDate = QDateTime::fromTime_t(TODO_DATE);
        editStart_->setDateTime(todoDate);
        editEnd_->setDateTime(todoDate);
    }
    else
    {
        if (editStart_->dateTime() > editEnd_->dateTime())
        {
            QMessageBox::critical(this, "Error", "Invalid range of time selected", QMessageBox::Ok);
            return;
        }
    }

    Category* category = nullptr;
    for (Category* c : categoryList_)
    {
        if (editCategory_->currentText().toStdString() == c->getName()) {
            category = new Category(*c);
            break;
        }
    }

    QDateTime start = editStart_->dateTime();
    QDateTime end = editEnd_->dateTime();
    Event *newEvent = new Event(0, editName_->text().toStdString(), editDescription_->toPlainText().toStdString(), editPlace_->text().toStdString(), category, start.toTime_t(), end.toTime_t());

    /* If the users has changed an existent event, I'll call the right function */
    if ((event_ != nullptr) && (pm_->replace_event(event_, newEvent))) {
        refresh();
        event_ = new Event(*newEvent);
    } else if ((event_ == nullptr) && (everyMonth_->isChecked() || everyYear_->isChecked())) {
        int reply = QMessageBox::warning(this, "Attention", "A recurrent event is considered as multiple independent events, after this operation you can modify it only as a single event.", QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            int offset = 8; /* Add the same event to the next 8 years */
            if (everyMonth_->isChecked()) offset = 24; /* Add the same event to every month of the next 2 years */
            bar->setRange(0, offset);
            layout()->addWidget(bar);
            Event* previous = nullptr;
            for (int i = 0; i < offset; i++) {
                ret = ret && pm_->add_event(newEvent, previous);
                if (previous != nullptr) delete previous;
                previous = newEvent;
                if (!ret) break;
                if (everyMonth_->isChecked())
                {
                    start = start.addMonths(1);
                    end = end.addMonths(1);
                }
                else
                {
                    start = start.addYears(1);
                    end = end.addYears(1);
                }
                newEvent = new Event(0, editName_->text().toStdString(), editDescription_->toPlainText().toStdString(), editPlace_->text().toStdString(), category, start.toTime_t(), end.toTime_t());
                bar->setValue(i);
                QCoreApplication::processEvents();
            }
            delete previous;
            if (ret)
            {
                delete newEvent;
                refresh();
            }
        }
    }
    else
        if ((event_ == nullptr) && (pm_->add_event(newEvent)))
        { //else I'll create a new Event
            delete newEvent;
            refresh();
        }
        else
            ret = false;

    if (!ret)
    {
        QMessageBox::critical(this, "Error", "Persistence error. Try with a different name.", QMessageBox::Ok);
        delete bar;
    }
}

void EventDialog::on_checkbox_todo_toggle(bool checked)
{
    editStart_->setEnabled(!checked);
    editEnd_->setEnabled(!checked);
    options->setEnabled(!checked);
}

void EventDialog::on_date_start_change()
{
    if (editStart_->date() > editEnd_->date())
        editEnd_->setDate(editStart_->date());
    if (editStart_->time() > editEnd_->time())
        editEnd_->setTime(editStart_->time().addSecs(60*60));
}

Event* EventDialog::getEvent() const
{
    return event_;
}
