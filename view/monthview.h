#ifndef MONTHVIEW_H
#define MONTHVIEW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLabel>
#include <QListIterator>
#include <QColor>
#include <QProgressBar>
#include <QtConcurrent/QtConcurrent>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QDir>
#include <QFileDialog>
#include <QScrollArea>
#include <QInputDialog>
#include <QSettings>
#include "qlabel_event.h"
#include "qpushbutton_extended.h"
#include "view.h"
#include "view/customdialog.h"
#include "view.UI/ui_monthview.h"
#include "view/categoryselectdialog.h"
#include "view/settings.h"
#include "view/eventdialog.h"
#include "view/qframe_extended.h"
#include "view/qwidget_extended.h"
#include "view/categorydialog.h"
#include "util/dateutil.h"
#include "util/pluginmanager.h"
#include "patient/patientmanager.h"

/* Gets the current month displayed using an hack. Infact, the cell in the middle will have always a value setted.
 * This should be used when you don't care about the day */
#define CURRENT_MONTH *this->frames[21]->getDate()

/* Style Sheets */
#define MAINWINDOW_STYLE "#mainwindow { background-color: #FFFFFF; border: 2px solid #DDDDFF; }"

#define LABELDATE_STYLE "QLabel { color: #000000; qproperty-alignment: AlignCenter; padding-left: 100px; padding-right: 100px; font-size: 20px; }"

#define TODOSBUTTON_STYLE "QPushButton { border: 1px solid #000000; color: #000000; padding: 5px; font-weight: bold; border-radius: 10px; } QPushButton:hover { color: #2222BB; }"

#define CELL_STYLE "QFrame { background-color: #FFFFFF; color: #000000; border: 1px outset #CCCCCC; }" \
                   "QFrame#holiday { background-color: #FFF5F5; }" \
                   "QLabel { border: none; font-size: 16px; padding: 5px; background-color:rgba(0,0,0,0); }" \
                   "QLabel#today { background-color: #FFFF88; color: #FF0000; border: 1px solid #FF0000;}" \
                   "QFrame#selected { background-color: #EEEEFF; }" \
                   "QLabel#header { font-weight: bold; }" \
                   "QFrame#header { background-color: #DDDDFF; }" \
                   "QFrame#disabled { background-color: #EFEFEF; }"

namespace Ui {
    class MonthView;
}

class MonthView : public QMainWindow, public View
{
    Q_OBJECT

public:
    explicit MonthView(QWidget* parent = nullptr);
    void refresh_events() override;
    void refresh_todos();
    void display_events(Date date, Category* category = nullptr) override;
    ~MonthView() override;

public slots:
    void on_mouse_press(QFrameExtended* frame);
    void on_mouse_release(QFrameExtended* frame);
    void on_mouse_move(QFrameExtended* frame);
    void on_back_button_click();
    void on_next_button_click();
    void on_event_click(QLabelEvent* label_event, Qt::MouseButton button);
    void on_event_key_press(int key);
    void on_todo_button_click();
    void keyPressEvent(QKeyEvent* e) override;

protected:
    void contextMenuEvent(QContextMenuEvent *event) Q_DECL_OVERRIDE;

private:
    QSettings settings_;
    Category* selectedCategory_;
    PluginManager* plm_;
    QVBoxLayout* layout_;
    QLabel* labelDate_;
    QLabelEvent* selectedEvent_;
    QPushButton* toDoButton_;
    Date* selectionStart_;
    Date* selectionEnd_;
    Ui::MonthView* ui;
    PManager* pm_;
    QFrameExtended* frames[42];
    QMenu* dbMenu_;
    PatientManager* patientManager_;
    void display_days(Date date);
    void remove_events_from_all_frames();
    void remove_events_from_frame(const int i);
    void createMenu();
    void on_button_extended_click(const int index);
    void refresh_db_menu();
    QLabelEvent* createLabelEvent(Event* event);
    QFrameExtended* createQFrameExtended(Date* date);
    CustomDialog* show_progress_bar(QString title);

private slots:
    void exit();
    void delete_db();
    void save_database();
    void load_database();
    void import_events();
    void export_events();
    void add_event();
    void edit_categories();
    void show_agenda(bool only_todos = false);
    void show_monthview();
    void filter_by_category();
    void run_tool(const string& tool);
    void switch_db(const string& db);
    void create_database();
    void show_settings();
};

#endif // MONTHVIEW_H
