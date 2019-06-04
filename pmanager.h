#ifndef PMANAGER_H
#define PMANAGER_H

#include <sqlite3.h> /* Version: 3.9.2 */
#include <stdio.h>
#include <list>
#include <vector>
#include <ctime>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <sys/stat.h>
#include <fstream>
#include <cstring>
#include <experimental/filesystem>
#include "model/event.h"
#include "model/category.h"

#define FOLDER_NAME "kalendar"
#define DEFAULT_DATABASE_NAME "default.sql"

using namespace std;
static string dbName_ = DEFAULT_DATABASE_NAME;

class PManager
{
public:
    PManager(const string& database = DEFAULT_DATABASE_NAME);
    void init_db(const string& db_name);
    void set_db(const string& database);
    string get_db_name() const;
    string get_db_folder() const;
    vector<string> get_db_list() const;
    bool add_event (Event* e, Event* child = nullptr);
    bool replace_event (Event* old_event, Event* new_event); //return true also if old_event doesn't exist
    bool remove_event(Event* e);
    bool remove_db();
    list<Event*> get_events_of_month(const int month, const int year) const;
    list<Event*> get_events(Category* c) const;
    list<Event*> get_all_events() const;
    bool add_category (Category* c);
    /* Note: the id will not be changed (to avoid to change the events with a reference to the category */
    bool replace_category(Category* old_category, Category* new_category);
    bool remove_category(Category* c);
    vector<Category*> get_categories() const;
    Category* get_category(const unsigned int id) const;
    bool remove_past_events(const time_t& timestamp);
    int save_db(string path);
    int export_db_iCal_format(list<Event *> events, string path);
    int load_db(const string& path);
    int import_db_iCal_format(const string& path,Category* category);
    ~PManager();

private:
    sqlite3* db_ = nullptr;
    string dbPath_;
    string dbFolder_;
    string filterSpecialChars(string str);
    bool error(const string& customMessage, char* sqlMessage, bool closeDB = false);
};

#endif // PMANAGER_H
