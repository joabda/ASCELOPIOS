#include "monthview.h"
#include <QDebug>

void MonthView::on_mouse_move(QFrameExtended* frame)
{
    if ((frame->getDate() != nullptr) && //Checks if the frame is valid
        (selectionStart_ != nullptr) && //and if the selection is already started
        (!selectionEnd_ || frame->getDate()->compareTo(*selectionEnd_) != 0))
    { //and if this is a new cell
        selectionEnd_ = frame->getDate(); //Keeps in memory where we're arrived
        for (int i = 0; i < 42; i++)
            if (frames[i]->getDate() != nullptr)
            {
                if ((frames[i]->getDate()->compareTo(*selectionStart_) >= 0) &&
                    (frames[i]->getDate()->compareTo(*frame->getDate()) <= 0))
                {
                    frames[i]->setObjectName("selected");
                    frames[i]->setStyleSheet(CELL_STYLE);
                }
                else
                {
                    if ((i % 7) > 4)
                        frames[i]->setObjectName("holiday");
                    else
                        frames[i]->setObjectName("day");
                    frames[i]->setStyleSheet(CELL_STYLE);
                }
            }
    }
}

void MonthView::on_mouse_press(QFrameExtended* frame)
{
    if (frame->getDate() != nullptr) //Checks if the frame is valid
        selectionStart_ = frame->getDate();
}

void MonthView::on_mouse_release(QFrameExtended* frame)
{
    if ((frame->getDate() != nullptr) && //Checks if the frame is valid
        (selectionStart_ != nullptr)) { //and if the selection is already started
        selectionEnd_ = frame->getDate();
        /* Clean the selection */
        for (int i = 0; i < 42; i++)
            if (frames[i]->getDate() != nullptr)
            {
                if ((i % 7) > 4)
                    frames[i]->setObjectName("holiday");
                else
                    frames[i]->setObjectName("day");
                frames[i]->setStyleSheet(CELL_STYLE);
            }

        /* The following lines will popup the dialog to add an event with the start and the end already setted,
         * but only if the user selects a valid range.
         */
        if (selectionEnd_->compareTo(*selectionStart_) >= 0)
        {
            EventDialog* eventDialog = new EventDialog(this, *selectionStart_, *selectionEnd_);
            eventDialog->show();
        }
        selectionStart_ = nullptr;
    }
}

void MonthView::on_back_button_click()
{
    Date newDate;
    if (QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
        newDate = DateUtil::decrease_year(CURRENT_MONTH);
    else
        newDate = DateUtil::decrease_month(CURRENT_MONTH);
    display_days(newDate);
    /* Reload events */
    selectedEvent_ = nullptr;
    display_events(newDate, selectedCategory_);
}

void MonthView::on_next_button_click()
{
    Date newDate;
    if (QGuiApplication::queryKeyboardModifiers().testFlag(Qt::ControlModifier))
        newDate = DateUtil::increase_year(CURRENT_MONTH);
    else
        newDate = DateUtil::increase_month(CURRENT_MONTH);
    display_days(newDate);
    /* Reload events */
    selectedEvent_ = nullptr;
    display_events(newDate, selectedCategory_);
}

MonthView::MonthView(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MonthView)
{
    selectedEvent_ = nullptr;
    selectedCategory_ = nullptr;
    plm_ = new PluginManager;
    Date current_date = DateUtil::get_current_date();
    pm_ = new PManager;
    selectionStart_ = nullptr;
    selectionEnd_ = nullptr;
    layout_ = new QVBoxLayout;
    labelDate_ = new QLabel;
    labelDate_->setMaximumHeight(40);
    labelDate_->setFixedWidth(400);
    labelDate_->setStyleSheet(LABELDATE_STYLE);
    QPushButton* back = new QPushButton("<");
    QPushButton* next = new QPushButton(">");
    toDoButton_ = new QPushButton;
    refresh_todos();
    back->setMaximumWidth(60);
    next->setMaximumWidth(60);
    back->setMaximumHeight(40);
    next->setMaximumHeight(40);
    back->setShortcut(QKeySequence(Qt::Key_Left));
    next->setShortcut(QKeySequence(Qt::Key_Right));
    next->setToolTip("Go to the next month, press ctrl to move to the next year");
    back->setToolTip("Go to the previous month, press ctrl to move to the previous year");
    toDoButton_->setToolTip("Show the list of TODOs");
    toDoButton_->setStyleSheet(TODOSBUTTON_STYLE);
    connect(back, &QPushButton::clicked, this, &MonthView::on_back_button_click);
    connect(next, &QPushButton::clicked, this, &MonthView::on_next_button_click);
    connect(toDoButton_, &QPushButton::clicked, this, &MonthView::on_todo_button_click);
    QHBoxLayout* hl = new QHBoxLayout;
    hl->addWidget(back, 1, Qt::AlignRight);
    hl->addWidget(labelDate_, 1, Qt::AlignCenter);
    hl->addWidget(next, 1, Qt::AlignLeft);
    hl->addWidget(toDoButton_);
    layout_->addLayout(hl);

    //Create 7x7 grid
    QGridLayout* grid_layout = new QGridLayout;
    //First row contains the names of the days of the week
    for (int j = 0; j < 7; j++)
    {
        QFrame* frame = new QFrame;
        QHBoxLayout* hl = new QHBoxLayout;
        hl->setAlignment(Qt::AlignCenter);
        frame->setFixedHeight(50);
        QLabel* wday_name = new QLabel(DateUtil::numeric2literal_day_of_week(j+1).c_str());
        wday_name->setObjectName("header");
        frame->setObjectName("header");
        hl->addWidget(wday_name);
        hl->setMargin(0);
        frame->setLayout(hl);
        frame->setStyleSheet(CELL_STYLE);
        grid_layout->addWidget(frame, 0, j);
    }

    //Next rows contain the days of the selected month
    for (int i = 1; i < 7; i++)  //rows
        for (int j = 0; j < 7; j++)
        { //columns
            //Map a Time object to each frame
            QFrameExtended *frame = createQFrameExtended(nullptr);
            grid_layout->addWidget(frame, i, j);
            frames[7*(i-1)+j] = frame;
        }

    grid_layout->setHorizontalSpacing(0);
    grid_layout->setVerticalSpacing(0);

    //Remove events too old
    int past_months = settings_.value(SettingsValues::past_months_expiration, 0).toInt();
    if (past_months > 0)
    {
        Date target = DateUtil::get_first_day_of_month(current_date);
        do {
            target = DateUtil::decrease_month(target);
            past_months--;
        } while (past_months > 0);
        pm_->remove_past_events(QDateTime(QDate(target.getYear(), target.getMonth() , target.getMonthDay())).toTime_t());
    }

    //Fill the grid with the days of the default month (i.e. the current month)
    display_days(current_date);

    //Load the events for the current month
    display_events(current_date, selectedCategory_);

    grid_layout->setMargin(5);
    layout_->addLayout(grid_layout);

     // Set layout in QWidget
     QWidgetExtended* window = new QWidgetExtended;
     window->setObjectName("mainwindow");
     window->setStyleSheet(MAINWINDOW_STYLE);
     window->setLayout(layout_);
     window->setMinimumHeight(600);
     window->setMinimumWidth(1100);

     connect(window, &QWidgetExtended::mousePress, this, &MonthView::on_mouse_press);
     connect(window, &QWidgetExtended::mouseRelease, this, &MonthView::on_mouse_release);
     connect(window, &QWidgetExtended::mouseMove, this, &MonthView::on_mouse_move);

     createMenu();

     // Set QWidget as the central layout of the main window
     setCentralWidget(window);

     patientManager_ = new PatientManager;
}

MonthView::~MonthView()
{
    delete selectedCategory_;
    delete pm_;
    delete plm_;
    delete ui;
    /* this->selection_start and this->selection_end are pointers to dates wrapped inside some QFrameExtended widgets. When
     * Qt frees QFrameExtended widgets they will free their dates.
     */
}

void MonthView::createMenu()
{
    QAction* loadAct = new QAction(tr("&Load database"), this);
    loadAct->setStatusTip(tr("Load database in Kal format"));
    connect(loadAct, &QAction::triggered, this, &MonthView::load_database);

    QAction* saveAct = new QAction(tr("&Save database"), this);
    saveAct->setStatusTip(tr("Save database in Kal format"));
    connect(saveAct, &QAction::triggered, this, &MonthView::save_database);

    QAction* importAct = new QAction(tr("&Import events"), this);
    importAct->setStatusTip(tr("Import events in iCal format"));
    connect(importAct, &QAction::triggered, this, &MonthView::import_events);

    QAction* exportAct = new QAction(tr("E&xport events"), this);
    exportAct->setStatusTip(tr("Export events in iCal format"));
    connect(exportAct, &QAction::triggered, this, &MonthView::export_events);

    QAction* exitAct = new QAction(tr("&Exit"), this);
    exitAct->setStatusTip(tr("Exit from the application"));
    connect(exitAct, &QAction::triggered, this, &MonthView::exit);



    QAction* addEventAct = new QAction(tr("&Add new event"), this);
    addEventAct->setStatusTip(tr("Show a dialog to add a new event"));
    connect(addEventAct, &QAction::triggered, this, &MonthView::add_event);

    QAction* editCategoriesAct = new QAction(tr("Edit &Categories"), this);
    editCategoriesAct->setStatusTip(tr("Show a dialog to edit the categories"));
    connect(editCategoriesAct, &QAction::triggered, this, &MonthView::edit_categories);

    // ONLY FOR DEBUGGING REMOVE AFTERWARDS
    QAction* deleteAllAct = new QAction(tr("&Delete database"), this);
    deleteAllAct->setStatusTip(tr("Delete all the events and categories"));
    connect(deleteAllAct, &QAction::triggered, this, &MonthView::delete_db);

    QAction* showAgendaAct = new QAction(tr("Show &Agenda"), this);
    showAgendaAct->setStatusTip(tr("Show a dialog with all the events"));
    connect(showAgendaAct, &QAction::triggered, this, &MonthView::show_agenda);

    QAction* showMonthviewAct = new QAction(tr("Show &Monthview"), this);
    showMonthviewAct->setStatusTip(tr("Show a a new window with the month view"));
    connect(showMonthviewAct, &QAction::triggered, this, &MonthView::show_monthview);

    QAction* showOnlyAct = new QAction(tr("Show &only..."), this);
    showOnlyAct->setStatusTip(tr("Show only events of a single category"));
    connect(showOnlyAct, &QAction::triggered, this, &MonthView::filter_by_category);

    QAction* showSettingsAct = new QAction(tr("Settings"), this);
    showSettingsAct->setStatusTip(tr("Show settings"));
    connect(showSettingsAct, &QAction::triggered, this, &MonthView::show_settings);


    QAction* addPatient = new QAction(tr("Add new Patient"), this);
    // connect(addPatient, &QAction::triggered, this, &MonthView);

    QAction* searchPatient = new QAction(tr("Search for a Patient"), this);
    // connect(searchPatient, &QAction::triggered, this, &Patient::search);



    QMenu* fileMenu = nullptr;
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(loadAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(importAct);
    fileMenu->addAction(exportAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    QMenu* editMenu = nullptr;
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addEventAct);
    editMenu->addAction(editCategoriesAct);
    editMenu->addAction(deleteAllAct); // DEBUGGING REMOVE BEFORE RELEASE
    editMenu->addAction(showOnlyAct);
    editMenu->addAction(showSettingsAct);
    dbMenu_ = menuBar()->addMenu(tr("&Calendars"));
    refresh_db_menu();

    QMenu* patients = nullptr;
    patients = menuBar()->addMenu(tr("&Patients"));
    patients->addAction(addPatient);
    patients->addAction(searchPatient);

    QMenu* viewsMenu = nullptr;
    viewsMenu = menuBar()->addMenu(tr("&Views"));
    viewsMenu->addAction(showAgendaAct);
    viewsMenu->addAction(showMonthviewAct);
    //TODO: Add the other future views (each view will be displayed in a different window)

    QMenu* toolsMenu = nullptr;
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    /* Dinamically generate the tools submenu */
    for (string tool : plm_->get_tools())
    {
        string text = tool.substr(0, tool.find('.'));
        QAction* t = new QAction(tr(text.c_str()), this);
        connect(t, &QAction::triggered, this, [this,tool]{ run_tool(tool); });
        toolsMenu->addAction(t);
    }
}

void MonthView::switch_db(const string& db)
{
    pm_->set_db(db);
    refresh_events();
    refresh_todos();
    refresh_db_menu();
}

void MonthView::refresh_db_menu()
{
    dbMenu_->clear();
    /* Dinamically generate the databases submenu */
    for (string db : pm_->get_db_list())
    {
        string text = db.substr(0, db.find('.'));
        QAction* t = new QAction(tr(text.c_str()), this);
        connect(t, &QAction::triggered, this, [this,db]{ switch_db(db); });
        dbMenu_->addAction(t);
    }
    QAction* newdb = new QAction(tr("&New database"), this);
    connect(newdb, &QAction::triggered, this, &MonthView::create_database);
    dbMenu_->addAction(newdb);
}

void MonthView::run_tool(const string& tool)
{
    plm_->runTool(tool);
}

void MonthView::create_database()
{
    bool ok;
    QString name = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                             tr("Database name:"), QLineEdit::Normal,
                                             "", &ok);
    if (ok && !name.isEmpty())
    {
        pm_->init_db(name.toStdString() + ".sql");
        refresh_db_menu();
        switch_db(name.toStdString() + ".sql");
    }
}

void MonthView::show_settings()
{
    Settings* settings = new Settings();
    settings->show();
}

void MonthView::contextMenuEvent(QContextMenuEvent* event)
{
    QMenu menu(this);
    menu.exec(event->globalPos());
}

void MonthView::exit()
{
    QApplication::quit();
}

void MonthView::filter_by_category()
{
    CategorySelectDialog* dialog = new CategorySelectDialog(this,"Show only events in this category: ");
    dialog->setModal(true);
    dialog->exec(); //Blocking call
    selectedCategory_ = dialog->getSelectedCategory();
    refresh_events();
}

void MonthView::delete_db()
{
    int ret = QMessageBox::question(this, "Confirm", "Do you really want to delete all the events and categories of the current database?", QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes)
    {
        pm_->remove_db();
        if (pm_->get_db_name() == DEFAULT_DATABASE_NAME)
        {
            delete pm_;
            pm_ = new PManager;
            refresh_events();
            refresh_todos();
        }
        else
            switch_db(pm_->get_db_list().at(0));
        refresh_db_menu();
    }
}

void MonthView::load_database()
{
    QString path = QFileDialog::getOpenFileName(this, "Load events and categories", QDir::homePath(), "ASCELOPIOS Files (*.kal)");
    if (path != "")
    {
        CustomDialog* custom_dialog = show_progress_bar("Loading events and categories...");
        QFuture<int> ret =  QtConcurrent::run([this,path] { return pm_->load_db(path.toStdString()); });
        while (!ret.isFinished())
            QCoreApplication::processEvents();

        int result = ret.result();
        refresh_events();
        custom_dialog->close();
        QMessageBox::information(this, "Success", "Loaded " + QString::number(result) + " events/categories", QMessageBox::Ok);
    }
}

void MonthView::save_database()
{
    QString path = QFileDialog::getSaveFileName(this, "Save events and categories", QDir::homePath(), "ASCELOPIOS Files (*.kal)");
    if (path != "") {
        CustomDialog* custom_dialog = show_progress_bar("Saving events and categories...");
        QFuture<int> ret =  QtConcurrent::run([this,path] { return pm_->save_db(path.toStdString()); });
        while (!ret.isFinished())
            QCoreApplication::processEvents();

        int result = ret.result();
        custom_dialog->close();
        QMessageBox::information(this, "Success", "Saved " + QString::number(result) + " events/categories", QMessageBox::Ok);
    }
}

void MonthView::export_events()
{
    QString path = QFileDialog::getSaveFileName(this, "Export events to other calendars", QDir::homePath(), "iCal Files (*.ics)");
    Category* c = nullptr;
    if (path != "") {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Export from single category", "Do you want export only the events belonging to a specific category?", QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes)
        {
            CategorySelectDialog* dialog = new CategorySelectDialog(this,"Select from which category export the events: ");
            dialog->setModal(true);
            dialog->exec(); //Blocking call
            c = dialog->getSelectedCategory();
        }
        QMessageBox::information(this, "Please wait", "Exporting events may requires some minutes", QMessageBox::Ok);
        list<Event*> events = pm_->get_events(c);
        int result = pm_->export_db_iCal_format(events,path.toStdString());
        for (Event* event : events)
            delete event;
        QMessageBox::information(this, "Success", "Exported " + QString::number(result) + " events", QMessageBox::Ok);
    }
}

CustomDialog* MonthView::show_progress_bar(QString title)
{
    QVBoxLayout* main_layout = new QVBoxLayout;
    QProgressBar* bar = new QProgressBar();
    bar->setRange(0,0);
    main_layout->addWidget(bar);
    CustomDialog* custom_dialog = new CustomDialog(main_layout);
    custom_dialog->setWindowFlags(Qt::Window | Qt::WindowTitleHint | Qt::CustomizeWindowHint);
    custom_dialog->setFixedWidth(500);
    custom_dialog->setWindowTitle(title);
    custom_dialog->setModal(true);
    custom_dialog->show();
    return custom_dialog;
}

void MonthView::import_events()
{
    QString path = QFileDialog::getOpenFileName(this, "Import events from other calendars", QDir::homePath(), "iCal Files (*.ics)");
    if (path != "") {
        CategorySelectDialog *dialog = new CategorySelectDialog(this,"Select a category for the imported events: ");
        dialog->setModal(true);
        dialog->exec(); //Blocking call
        if (dialog->getSelectedCategory() != nullptr)
        {
            unsigned int category_id = dialog->getSelectedCategory()->getId();
            Category* category = pm_->get_category(category_id);
            delete dialog->getSelectedCategory();
            QMessageBox::information(this, "Please wait", "Importing events may requires some minutes", QMessageBox::Ok);
            CustomDialog* custom_dialog = show_progress_bar("Importing events...");
            QFuture<int> ret =  QtConcurrent::run([this,path,category] { return pm_->import_db_iCal_format(path.toStdString(),category); });
            while (!ret.isFinished())
                QCoreApplication::processEvents();

            int result = ret.result();
            refresh_events();
            custom_dialog->close();
            QMessageBox::information(this, "Success", "Imported " + QString::number(result) + " events", QMessageBox::Ok);
        }
    }
}

void MonthView::add_event()
{
    EventDialog* eventDialog = new EventDialog(this, CURRENT_MONTH, CURRENT_MONTH);
    eventDialog->show();
}

void MonthView::edit_categories()
{
    CategoryDialog* category_dialog = new CategoryDialog(this);
    category_dialog->show();
}

void MonthView::on_todo_button_click()
{
    /* Show only TODOs */
    show_agenda(true);
}

void MonthView::show_monthview()
{
    MonthView* mv = new MonthView;
    mv->show();
}

void MonthView::show_agenda(bool only_todos)
{
    list<Event*> events_list;
    if (only_todos)
    {
        Date todoDate = DateUtil::date_from_timestamp(TODO_DATE);
        events_list = pm_->get_events_of_month(todoDate.getMonth(), todoDate.getYear());
    }
    else
        events_list = pm_->get_all_events();
    QFrameExtended* frame = createQFrameExtended(nullptr);
    QLabel* header = static_cast<QLabel*> (frame->children().at(1));
    header->setStyleSheet("QLabel { background-color: #ffffb3; border-bottom: 1px solid #000000; margin-bottom: 2px; }");
    header->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    Date today = DateUtil::get_current_date();
    QString title(" Event(s)");
    if (only_todos)
        title = QString(" TODO(s)");
    int list_size = static_cast<int>(events_list.size());
    for (Event* event : events_list)
    {
        QHBoxLayout* hl = new QHBoxLayout;
        QString text;
        if (!only_todos)
            if ((event->getStart() == TODO_DATE) || (DateUtil::date_from_timestamp(event->getEnd()).compareTo(today) < 0))
            {
                list_size--;
                continue; //skip the todo or the past event
            }

        Date start = DateUtil::date_from_timestamp(event->getStart());
        Date end = DateUtil::date_from_timestamp(event->getEnd());
        text = QString(start.toString(false).c_str()) + QString(" - ") + QString(end.toString(false).c_str());
        QLabel *label_time = new QLabel(text);
        label_time->setFixedWidth(215);
        if (!only_todos)
            hl->addWidget(label_time);
        hl->addWidget(createLabelEvent(event));
        (static_cast <QVBoxLayout*> (frame->layout()))->addLayout(hl);
    }
    header->setText(QString::number(list_size) + title);
    QVBoxLayout* main_layout = new QVBoxLayout;
    QScrollArea* scroll_area = new QScrollArea;
    scroll_area->setMaximumHeight(600);
    scroll_area->setWidget(frame);
    scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    frame->setFixedWidth(scroll_area->width());
    main_layout->addWidget(scroll_area);
    CustomDialog* custom_dialog = new CustomDialog(main_layout);
    custom_dialog->setFixedWidth(500);
    custom_dialog->setWindowTitle("Agenda");
    custom_dialog->show();
}

void MonthView::display_days(Date date)
{
    //Update the label that contains month and year
    labelDate_->setText(QString(DateUtil::get_literal_month(date.getMonth()).c_str()) + QString("    ") + QString::number(date.getYear()));
    //The current time is needed to highlight the current day
    Date current_date = DateUtil::get_current_date();
    int tot_days = DateUtil::get_days_in_month(date.getMonth(), date.getYear());

    //first week day of the current month
    int start_wday = DateUtil::get_first_day_of_month(date).getWeekDay();
    int x = 1;
    for (int i = 0; i < 42; i++)
    {
        //Set an invalid date
        frames[i]->setDate(nullptr);

        //Delete all the event labels inside the frame
        remove_events_from_frame(i);

        QLabel *day = static_cast<QLabel*> (frames[i]->children().at(1));
        day->setMaximumWidth(40);
        day->setObjectName("");
        day->setText("");
        //Checks right cells that will contain the days
        if (( i > 6 || //if I'm after the first week or
            (i % 7 >= start_wday-1)) && //if I'm in the first week and I'm in the right days
            (x <= tot_days))
        { //and I'm not out of bound
            frames[i]->setDate(new Date(x, (start_wday + (x-1)) % 7, date.getMonth(), date.getYear()));
            //insert into the label the zero-padded number of the day
            day->setText(QString("%1").arg(x, 2, 10, QChar('0')));
            //Checks current day
            if ((x == current_date.getMonthDay()) && (date.getMonth() == current_date.getMonth()) && (date.getYear() == current_date.getYear()))
                day->setObjectName("today");
            frames[i]->layout()->addWidget(day);
            //check if the current frame refers to an holiday (i.e. saturday or sunday)
            if ((i % 7) > 4)  //frame in the last two columns
                //mark the cell as an holiday
                frames[i]->setObjectName("holiday");
            else  //is a generic day
                // mark the cell as enabled
                frames[i]->setObjectName("enabled");
            x++;
        }
        else// mark the cell as disabled
            frames[i]->setObjectName("disabled");

        //Refresh the css rules
        frames[i]->setStyleSheet(CELL_STYLE);
    }
}

void MonthView::refresh_todos()
{
    Date todoDate = DateUtil::date_from_timestamp(TODO_DATE);
    list<Event*> todos = pm_->get_events_of_month(todoDate.getMonth(), todoDate.getYear());
    toDoButton_->setText(QString::number(todos.size()));
    for (Event* e : todos)
        delete e;
}

void MonthView::refresh_events()
{
    display_events(CURRENT_MONTH, selectedCategory_);
    refresh_todos();
}

void MonthView::display_events(Date date, Category* category)
{
    list<Event*> event_list = pm_->get_events_of_month(date.getMonth(), date.getYear());
    int start_offset;
    Event* selected_event = nullptr;
    //Check if there was a selected event
    if (selectedEvent_ != nullptr)
        selected_event = selectedEvent_->getEvent();


    //Remove all displayed events
    remove_events_from_all_frames();

    //Find at which cell the month starts
    for (start_offset = 0; start_offset < 42; start_offset++)
    {
        //Looks where is the first day of the month
        if (frames[start_offset]->getDate() != nullptr)
            break;
    }
    //Add events to the gui
    for (Event* event : event_list)
    {
        /* TODO: maybe it's better to move this filter at low level, i.e. in the persistence class */
        if ((category != nullptr) && (!event->getCategory()->equals(*category)))
            continue; //Don't add this event to the view
        Date start = DateUtil::date_from_timestamp(event->getStart());
        Date end = DateUtil::date_from_timestamp(event->getEnd());
        if (((start.getMonth() < date.getMonth()) && (start.getYear() == date.getYear())) || (start.getYear() < date.getYear()))
            start = DateUtil::get_first_day_of_month(date);
        if (((end.getMonth() > start.getMonth()) && (end.getYear() == start.getYear())) || (end.getYear() > start.getYear()))
            end = DateUtil::get_last_day_of_month(start);

        for (int i = start_offset+start.getMonthDay()-1; i < (start_offset+end.getMonthDay()); i++)
        {
            QLabelEvent *label_event = createLabelEvent(event);
            /* Check if the event should be selected */
            if ((selected_event != nullptr) && (selected_event->equals(*event)))
                label_event->markSelection(true);
                selectedEvent_ = label_event;

            if (frames[i]->children().size() == 5)
            {
                QPushButtonExtended *button_show_all = new QPushButtonExtended("Show All");
                button_show_all->setIndex(i);
                connect(button_show_all, &QPushButtonExtended::on_click, this, &MonthView::on_button_extended_click);
                frames[i]->layout()->addWidget(button_show_all);
                label_event->setHidden(true);
            }
            else
                if (frames[i]->children().size() > 5)
                    label_event->setHidden(true);
            //Events will be copied and wrapped inside the QLabelEvent widgets
            (static_cast <QVBoxLayout*> (frames[i]->layout()))->insertWidget(1, label_event);
        }
        delete event;
    }
    if (selectedEvent_ != nullptr)
        selectedEvent_->setFocus();
}

QFrameExtended* MonthView::createQFrameExtended(Date* date)
{
    QFrameExtended* frame = new QFrameExtended;
    QVBoxLayout* vl = new QVBoxLayout;
    frame->setDate(date);
    frame->setObjectName("day");
    vl->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    vl->setMargin(0);
    vl->setSpacing(1);
    vl->addWidget(new QLabel);
    frame->setMinimumWidth(150);
    frame->setMinimumHeight(60);
    frame->setLayout(vl);
    frame->setStyleSheet(CELL_STYLE);
    frame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    return frame;
}

void MonthView::on_button_extended_click(const int index)
{
    //Make a copy
    Date* date = new Date(frames[index]->getDate()->getMonthDay(), frames[index]->getDate()->getWeekDay(), frames[index]->getDate()->getMonth(), frames[index]->getDate()->getYear());
    QFrameExtended* frame = createQFrameExtended(date);
    QString text = QString(DateUtil::numeric2literal_day_of_week(frame->getDate()->getWeekDay()).c_str()) +
                   QString("    ") +
                   QString::number(frame->getDate()->getMonthDay()) + QString(" ") +
                   QString(DateUtil::get_literal_month(frame->getDate()->getMonth()).c_str()) + QString(" ") +
                   QString::number(frame->getDate()->getYear());
    QLabel *label_day = static_cast<QLabel*> (frame->children().at(1));
    label_day->setText(text);
    label_day->setStyleSheet("QLabel { background-color: #ffffb3; border-bottom: 1px solid #000000; margin-bottom: 2px; }");
    char stime[14];
    for (QLabelEvent *label_event : frames[index]->findChildren<QLabelEvent*>())
    {
        Event* event = new Event(*label_event->getEvent());
        QTime start = QDateTime::fromTime_t(event->getStart()).time();
        QTime end = QDateTime::fromTime_t(event->getEnd()).time();
        snprintf(stime, 14, "%02d:%02d - %02d:%02d", start.hour(), start.minute(), end.hour(), end.minute());
        QLabel* time = new QLabel(stime);
        QHBoxLayout* hl = new QHBoxLayout;
        hl->addWidget(time);
        hl->addWidget(createLabelEvent(event));
        (static_cast <QVBoxLayout*> (frame->layout()))->addLayout(hl); //TODO: add the entries ordered by timestamp
    }
    QVBoxLayout* main_layout = new QVBoxLayout;
    main_layout->addWidget(frame);
    CustomDialog* custom_dialog = new CustomDialog(main_layout);
    custom_dialog->setFixedWidth(400);
    custom_dialog->setWindowTitle("Day Dialog");
    custom_dialog->show();
}

QLabelEvent* MonthView::createLabelEvent(Event* event)
{
    //Make a copy
    Event* newEvent = new Event(*event);
    QLabelEvent* label_event = new QLabelEvent;
    label_event->setEvent(newEvent);
    label_event->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    connect(label_event, &QLabelEvent::clicked, this, &MonthView::on_event_click);
    connect(label_event, &QLabelEvent::keyPressed, this, &MonthView::on_event_key_press);
    return label_event;
}

void MonthView::remove_events_from_all_frames()
{//Delete all the event labels inside the frame
    for (int i = 0; i < 42; i++)
        remove_events_from_frame(i);
}

void MonthView::remove_events_from_frame(const int i)
{
    QLabelEvent label_event;
    QPushButtonExtended button("");
    QListIterator<QObject *> it (frames[i]->children());
    while (it.hasNext())
    {
        QObject* o = qobject_cast<QObject*> (it.next());
        /* Don't delete a QLabelEvent before returning to the event loop (on_event_click), otherwise will be triggered a segmentation fault */
        if (o->metaObject()->className() == label_event.metaObject()->className()) {
            (qobject_cast<QLabelEvent*>(o))->setHidden(true);
            (qobject_cast<QLabelEvent*>(o))->setParent(nullptr); //Detach from parent QFrame
            (qobject_cast<QLabelEvent*>(o))->deleteLater();
        }
        else
            if (o->metaObject()->className() == button.metaObject()->className()) delete o;
    }
}

void MonthView::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
        case Qt::Key_C:
            for (int i = 0; i < pm_->get_db_list().size(); i++)
                if (pm_->get_db_list().at(i) == pm_->get_db_name())
                {
                    if (i < pm_->get_db_list().size()-1)
                        switch_db(pm_->get_db_list().at(i+1));
                    else
                        switch_db(pm_->get_db_list().at(0));
                    break;
                }
            break;
    }
}

void MonthView::on_event_key_press(int key)
{
    Event* newEvent;
    time_t next_end_day = selectedEvent_->getEvent()->getEnd() + SECONDS_IN_1DAY;
    time_t previous_end_day = selectedEvent_->getEvent()->getEnd() - SECONDS_IN_1DAY;
    time_t next_start_day = selectedEvent_->getEvent()->getStart() + SECONDS_IN_1DAY;
    time_t previous_start_day = selectedEvent_->getEvent()->getStart() - SECONDS_IN_1DAY;

    switch(key)
    {
    case Qt::Key_Delete:
        selectedEvent_ = nullptr;
        refresh_events();
        break;
    case Qt::Key_D:
        newEvent = new Event(0, selectedEvent_->getEvent()->getName(), selectedEvent_->getEvent()->getDescription(),
                                    selectedEvent_->getEvent()->getPlace(), new Category(*selectedEvent_->getEvent()->getCategory()), selectedEvent_->getEvent()->getStart(), next_end_day);
        pm_->replace_event(selectedEvent_->getEvent(), newEvent);
        selectedEvent_->setEvent(newEvent);
        refresh_events();
        break;
    case Qt::Key_S:
        if (previous_end_day >= selectedEvent_->getEvent()->getStart()) {
            newEvent = new Event(0, selectedEvent_->getEvent()->getName(), selectedEvent_->getEvent()->getDescription(),
                                        selectedEvent_->getEvent()->getPlace(), new Category(*selectedEvent_->getEvent()->getCategory()), selectedEvent_->getEvent()->getStart(), previous_end_day);
            pm_->replace_event(selectedEvent_->getEvent(), newEvent);
            selectedEvent_->setEvent(newEvent);
            refresh_events();
        }
        break;
    case Qt::Key_A:
        newEvent = new Event(0, selectedEvent_->getEvent()->getName(), selectedEvent_->getEvent()->getDescription(),
                                    selectedEvent_->getEvent()->getPlace(), new Category(*selectedEvent_->getEvent()->getCategory()), previous_start_day, previous_end_day);
        pm_->replace_event(selectedEvent_->getEvent(), newEvent);
        selectedEvent_->setEvent(newEvent);
        refresh_events();
        break;
    case Qt::Key_F:
        newEvent = new Event(0, selectedEvent_->getEvent()->getName(), selectedEvent_->getEvent()->getDescription(),
                                    selectedEvent_->getEvent()->getPlace(), new Category(*selectedEvent_->getEvent()->getCategory()), next_start_day, next_end_day);
        pm_->replace_event(selectedEvent_->getEvent(), newEvent);
        selectedEvent_->setEvent(newEvent);
        refresh_events();
        break;
    }
}

void MonthView::on_event_click(QLabelEvent* label_event, Qt::MouseButton button)
{
    if (button == Qt::RightButton) {
        pm_->remove_event(label_event->getEvent());
        label_event->getEvent()->setInvalid();
        label_event->drawUI();
        refresh_events();
    } else if ((button == Qt::MiddleButton) && (selectedEvent_ == nullptr)) {
        selectedEvent_ = label_event;
        selectedEvent_->markSelection(true);
        refresh_events();
    } else if ((button == Qt::MiddleButton) && (selectedEvent_ != nullptr)) {
        selectedEvent_ = nullptr;
        refresh_events();
    } else {
        EventDialog *eventDialog = new EventDialog(this);
        eventDialog->setEvent(label_event->getEvent());
        eventDialog->exec();
        //if the event is changed, we'll update the label
        if (!eventDialog->getEvent()->equals(*label_event->getEvent())) {
            label_event->setEvent(eventDialog->getEvent()); /* Automatically free the old event */
            label_event->drawUI();
        }
    }
}
