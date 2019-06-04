#include "pmanager.h"

#include <QDebug>

PManager::PManager(const string& database)
{
    if (database != DEFAULT_DATABASE_NAME)
        dbName_ = database;
    init_db(dbName_);
}

bool PManager::error(const string& customMessage, char* sqlMessage, bool closeDB)
{
    fprintf(stderr, "%s: %s\n", customMessage.c_str(), sqlite3_errmsg(db_));
    sqlite3_free(sqlMessage);
    if(closeDB)
        sqlite3_close(db_);
    return false;
}

void PManager::init_db(const string& db_name)
{
    /* Close an already open database */
    if (db_ != nullptr)
        sqlite3_close(db_);

    /* Open the database (will be created if it doesn't exist) */
    dbFolder_ = string(getpwuid(getuid())->pw_dir) + string("/" FOLDER_NAME "/");
    dbPath_ = dbFolder_ + string(db_name);
    ifstream dbfile(dbPath_.c_str());
    bool db_not_exists = !dbfile;
    if (db_not_exists)
    {
        mkdir((string(getpwuid(getuid())->pw_dir) + string("/" FOLDER_NAME)).c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        ofstream new_dbfile(dbPath_.c_str());
        new_dbfile.close();
    }

    int rc = sqlite3_open(dbPath_.c_str(), &db_);
    char* errorMessage = nullptr;

    if (rc != SQLITE_OK)
        error("Cannot open database", errorMessage, true);

    if (db_not_exists)
    {
        string query = "CREATE TABLE Categories(id UNSIGNED INTEGER PRIMARY KEY, name TEXT, color TEXT);"
                          "CREATE TABLE Events(id UNSIGNED INTEGER PRIMARY KEY, name TEXT, description TEXT,"
                          "place TEXT, category UNSIGNED INTEGER, start DATETIME, end DATETIME, child UNSIGNED INTEGER,"
                          "FOREIGN KEY(category) REFERENCES Categories(id) ON DELETE RESTRICT,"
                          "FOREIGN KEY(child) REFERENCES Events(id) ON DELETE CASCADE);"
                          "INSERT INTO Categories VALUES(1, 'Default', '#1022A0');"
                          "PRAGMA foreign_keys = ON;";

        rc = sqlite3_exec(db_, query.c_str(), nullptr, nullptr, &errorMessage);

        if (rc != SQLITE_OK )
            error("SQL error", errorMessage, true);
    }
}

void PManager::set_db(const string& database)
{
    dbName_ = database;
    init_db(dbName_);
}

string PManager::get_db_name() const
{
    return dbName_;
}

string PManager::get_db_folder() const
{
    return dbFolder_;
}

vector<string> PManager::get_db_list() const
{
    vector<string> db_list;
    for (experimental::filesystem::directory_entry e : experimental::filesystem::directory_iterator(dbPath_.substr(0, dbPath_.find_last_of('/') )))
    {
        experimental::filesystem::path p = e.path();
        if (p.extension() == ".sql")
            db_list.push_back(p.filename());
    }
    return db_list;
}

string PManager::filterSpecialChars(string str)
{
    int startPosition = 0;
    string to = "’";
    while( (str.find('\'', startPosition)) != std::string::npos)
    {
        startPosition = str.find('\'', startPosition);
        str.replace(startPosition, 1, to);
        startPosition += to.length();
    }
    return str;
}

bool PManager::add_event(Event* e, Event* child)
{
    char* errorMessage = nullptr;
    sqlite3_stmt* stmt = nullptr;
    string filteredName, filteredDescription, filteredPlace;
    if ((e->getName().length() < 3) || (difftime(e->getStart(), e->getEnd()) > 0) || (e->getCategory() == nullptr))
        return false;
    int rc = sqlite3_prepare_v2(db_, "INSERT INTO Events VALUES(?, ?, ?, ?, ?, ?, ?, ?);", -1, &stmt, nullptr);
    if (rc != SQLITE_OK )
        return error("SQL error in prepare", errorMessage, true);

    sqlite3_bind_int64 (stmt, 1, e->getId());
    filteredName = filterSpecialChars(e->getName());
    sqlite3_bind_text(stmt, 2, filteredName.c_str(), filteredName.length(), nullptr);
    filteredDescription = filterSpecialChars(e->getDescription());
    sqlite3_bind_text(stmt, 3, filteredDescription.c_str(), filteredDescription.length(), nullptr);
    filteredPlace = filterSpecialChars(e->getPlace());
    sqlite3_bind_text(stmt, 4, filteredPlace.c_str(), filteredPlace.length(), nullptr);
    sqlite3_bind_int64(stmt, 5, e->getCategory()->getId());
    sqlite3_bind_int64(stmt, 6, e->getStart());
    sqlite3_bind_int64(stmt, 7, e->getEnd());
    if (child != nullptr)
        sqlite3_bind_int64(stmt, 8, child->getId());
    else
        sqlite3_bind_null(stmt, 8);
    //commit
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE )
        return error("SQL error in commit", errorMessage);

    //free memory
    sqlite3_finalize(stmt);
    return true;
}

bool PManager::replace_event(Event* old_event, Event* new_event)
{
    char* errorMessage = nullptr;
    sqlite3_stmt *stmt = nullptr;
    string filteredName, filteredDescription, filteredPlace;
    if ((new_event->getName().length() < 3) || (difftime(new_event->getStart(), new_event->getEnd()) > 0) || (new_event->getCategory() == nullptr))
        return false;
    int rc = sqlite3_prepare_v2(db_, "UPDATE Events SET id=?, name=?, description=?, place=?, category=?, start=?, end=? WHERE id=?;", -1, &stmt, nullptr);
    if (rc != SQLITE_OK )
        return error("SQL error in prepare", errorMessage);

    sqlite3_bind_int64(stmt, 1, new_event->getId());
    filteredName = filterSpecialChars(new_event->getName());
    sqlite3_bind_text(stmt, 2, filteredName.c_str(), filteredName.length(), nullptr);
    filteredDescription = filterSpecialChars(new_event->getDescription());
    sqlite3_bind_text(stmt, 3, filteredDescription.c_str(), filteredDescription.length(), nullptr);
    filteredPlace = filterSpecialChars(new_event->getPlace());
    sqlite3_bind_text(stmt, 4, filteredPlace.c_str(), filteredPlace.length(), nullptr);
    sqlite3_bind_int64(stmt, 5, new_event->getCategory()->getId());
    sqlite3_bind_int64(stmt, 6, new_event->getStart());
    sqlite3_bind_int64(stmt, 7, new_event->getEnd());
    sqlite3_bind_int64(stmt, 8, old_event->getId());
    //commit
    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE )
        return error("SQL error in commit", errorMessage);
    sqlite3_finalize(stmt);    //free memory
    return true;
}

bool PManager::replace_category(Category* old_category, Category* new_category)
{
    char* errorMessage = nullptr;
    char sql[1024];
    if (new_category->getName().length() < 3)
        return false;
    snprintf(sql, 1024, "UPDATE Categories SET name='%s', color='%s' WHERE id=%u;", filterSpecialChars(new_category->getName()).c_str(), filterSpecialChars(new_category->getColor()).c_str(), old_category->getId());
    int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK )
        return error("SQL error", errorMessage);
    return true;
}

list<Event*> PManager::get_events(Category* c) const
{
    if (c == nullptr)
        return get_all_events();
    list<Event*> result;
    sqlite3_stmt* res = nullptr;
    int rc = sqlite3_prepare_v2(db_, "SELECT * FROM Events WHERE category = ?;", -1, &res, nullptr);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db_));
        return result;
    }
    sqlite3_bind_int64(res, 1, static_cast<long>(c->getId()));
    while ((rc = sqlite3_step(res)) == SQLITE_ROW)
    {
        unsigned long id = static_cast<unsigned long>(sqlite3_column_int(res, 0));
        //string name(const_cast<std::string>(sqlite3_column_text(res, 1)));


        string name(reinterpret_cast<const char*>(sqlite3_column_text(res, 1)));
        string description(reinterpret_cast<const char*>(sqlite3_column_text(res, 2)));
        string place(reinterpret_cast<const char*>(sqlite3_column_text(res, 3)));
        Category* category = get_category(static_cast<unsigned int>(sqlite3_column_int64(res, 4)));
        if (category == nullptr)
        {
            fprintf(stderr, "Error: Received NULL category\n");
            continue;
        }
        time_t start = static_cast<long>(sqlite3_column_int64(res, 5));
        //time_t end = (unsigned long)sqlite3_column_int64(res, 6);
        time_t end =  static_cast<long>(sqlite3_column_int64(res, 6));
        Event* e = new Event(static_cast<unsigned int>(id), name, description, place, category, start, end);

        result.push_front(e);
    }
    sqlite3_finalize(res);
    return result;
}

list<Event*> PManager::get_events_of_month(const int month, const int year)  const
{
    list<Event*> result;

    /* A large number of countries use daylight saving time (DST) (called also summer time).
     * We need to manage DST. Most of the countries start to use DST between 1 March and 1 April,
     * but the dates where the DST ends are different. For example in Europe DST ends before 1 November, instead in U.S.A.
     * DST ends after the 1 November. We must calculate for the current user if DST ends before or after 1 November (geolocation dependent).
     * The following lines of code use localtime function that takes into account the location of the user. */
    time_t threshold = 26262000; // = 1 November 1970
    std::tm *t = localtime(&threshold);
    /* From documentation:
     * The Daylight Saving Time flag (tm_isdst) is greater than zero if Daylight Saving Time is in effect,
     * zero if Daylight Saving Time is not in effect,
     * and less than zero if the information is not available.
     */
    int s = 0;
    if (t->tm_isdst > 0)
        s = 1; //if tm_isdst is negative, s will have the default value

    std::tm tm;
    tm.tm_sec = 0;
    tm.tm_min = 0;
    tm.tm_hour = 0;
    tm.tm_mday = 1;
    tm.tm_mon = month - 1;    // Assuming month represents Jan with 1
    tm.tm_year = year - 1900; // Assuming year is the AD year number
    tm.tm_isdst = ((tm.tm_mon > 2) && (tm.tm_mon < 10+s)); // 10 is November
    long first_month = static_cast<long> (mktime(&tm)); //first of month
    tm.tm_mon = tm.tm_mon + 1; //This could be 12, but isn't a problem
    tm.tm_isdst = ((tm.tm_mon > 2) && (tm.tm_mon < 10+s));
    long last_month = static_cast<long> (mktime(&tm)); //last of month
    if ((first_month == -1) || (last_month == -1))
    {
        fprintf(stderr, "Failed to calculate timestamp\n");
        return result;
    }
    sqlite3_stmt *res;
    int rc = sqlite3_prepare_v2(db_, "SELECT * FROM Events WHERE (start >= ? AND start < ?) OR (end >= ? AND end < ?);", -1, &res, nullptr);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db_));
        return result;
    }
    sqlite3_bind_int64(res, 1, first_month);
    sqlite3_bind_int64(res, 2, last_month);
    sqlite3_bind_int64(res, 3, first_month);
    sqlite3_bind_int64(res, 4, last_month);
    while ((rc = sqlite3_step(res)) == SQLITE_ROW)
    {
        unsigned long id = (unsigned long)sqlite3_column_int(res, 0);
        string name((const char*)sqlite3_column_text(res, 1));
        string description((const char*)sqlite3_column_text(res, 2));
        string place((const char*)sqlite3_column_text(res, 3));
        Category* category = get_category(static_cast<unsigned>(sqlite3_column_int64(res, 4)));
        if (category == nullptr)
        {
            fprintf(stderr, "Error: Received NULL category\n");
            continue;
        }
        time_t start = static_cast<long>(sqlite3_column_int64(res, 5));
        time_t end = static_cast<long>(sqlite3_column_int64(res, 6));
        Event* e = new Event(static_cast<unsigned>(id), name, description, place, category, start, end);

        result.push_front(e);
    }
    sqlite3_finalize(res);
    return result;
}

bool PManager::remove_db()
{/* Delete the database file, but not the folder */
    return (std::remove(dbPath_.c_str()) == 0);
}

bool PManager::remove_event(Event* e)
{
    char* errorMessage = nullptr;
    char sql[1024];
    snprintf(sql, 1024, "DELETE FROM Events WHERE id = %u;", e->getId());
    int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK )
        return error("SQL error", errorMessage);
    return true;
}

bool PManager::add_category(Category* c)
{
    char* errorMessage = nullptr;
    char sql[1024];
    if (c->getName().length() < 3)
        return false;
    snprintf(sql, 1024, "INSERT INTO Categories VALUES(%u, '%s', '%s');", c->getId(), filterSpecialChars(c->getName()).c_str(), filterSpecialChars(c->getColor()).c_str());
    int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK )
        return error("SQL error", errorMessage);
    return true;
}

vector<Category*> PManager::get_categories() const
{
    vector<Category*> result;
    sqlite3_stmt* res = nullptr;
    char sql[1024];
    snprintf(sql, 1024, "SELECT * FROM Categories;");
    int rc = sqlite3_prepare_v2(db_, sql, -1, &res, nullptr);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db_));
        return result;
    }
    while ((rc = sqlite3_step(res)) == SQLITE_ROW)
    {
        Category* c = new Category( static_cast<unsigned>(sqlite3_column_int(res, 0)),
                              string((const char*)sqlite3_column_text(res, 1)),
                              string((const char*)sqlite3_column_text(res, 2)));

        result.push_back(c);
    }
    sqlite3_finalize(res);
    return result;
}

bool PManager::remove_category(Category* c)
{
    char* errorMessage = nullptr;
    char sql[1024];
    snprintf(sql, 1024, "PRAGMA foreign_keys = ON; DELETE FROM Categories WHERE id = %u;", c->getId());
    int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK )
        return error("SQL error", errorMessage);
    return true;
}

Category* PManager::get_category(const unsigned int id) const
{
    sqlite3_stmt* res = nullptr;
    char sql[1024];
    snprintf(sql, 1024, "SELECT * FROM Categories WHERE id = %u;", id);
    int rc = sqlite3_prepare_v2(db_, sql, -1, &res, nullptr);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db_));
        return nullptr;
    }
    if ((rc = sqlite3_step(res)) == SQLITE_ROW)
    {
        Category* category = new Category( static_cast<unsigned>(sqlite3_column_int(res, 0)),
                                           string((const char*)sqlite3_column_text(res, 1)),
                                           string((const char*)sqlite3_column_text(res, 2)));
        sqlite3_finalize(res);
        return category;
    }
    else
        return nullptr;
}

bool PManager::remove_past_events(const time_t& timestamp)
{
    char* errorMessage = nullptr;
    char sql[1024];
    snprintf(sql, 1024, "DELETE FROM Events WHERE end <= %ld;", static_cast <long> (timestamp));
    int rc = sqlite3_exec(db_, sql, nullptr, nullptr, &errorMessage);
    if (rc != SQLITE_OK )
        return error("SQL error", errorMessage);
    return true;
}

list<Event*> PManager::get_all_events() const
{
    list<Event*> result;
    sqlite3_stmt* res = nullptr;
    char sql[1024];
    snprintf(sql, 1024, "SELECT * FROM Events ORDER BY start DESC;");
    int rc = sqlite3_prepare_v2(db_, sql, -1, &res, nullptr);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to fetch data: %s\n", sqlite3_errmsg(db_));
        return result;
    }
    while ((rc = sqlite3_step(res)) == SQLITE_ROW)
    {
        unsigned long id = static_cast<unsigned long>(sqlite3_column_int(res, 0));
        string name((const char*)sqlite3_column_text(res, 1));
        string description((const char*)sqlite3_column_text(res, 2));
        string place((const char*)sqlite3_column_text(res, 3));
        Category* category = get_category(static_cast<unsigned>(sqlite3_column_int64(res, 4)));
        if (category == nullptr)
        {
            fprintf(stderr, "Error: Received NULL category\n");
            continue;
        }
        time_t start = static_cast<long>(sqlite3_column_int64(res, 5));
        time_t end = static_cast<long>(sqlite3_column_int64(res, 6));
        Event* e = new Event(static_cast<unsigned>(id), name, description, place, category, start, end);

        result.push_front(e);
    }
    sqlite3_finalize(res);
    return result;
}

int PManager::save_db(string path)
{
    if (path.length() < 5)
        return 0;
    if (path.substr(path.length()-4, 4) != ".kal")
        path += ".kal";
    int counter = 0;
    ofstream file;
    file.open(path);
    //TODO: An event with a description too long can exceed the buffer, use sqlite3_prepare_v2
    char sql[4096];
    vector<Category*> categories_list = get_categories();
    for (Category* category : categories_list)
    {
        snprintf(sql, 4096, "INSERT INTO Categories VALUES(%u, '%s', '%s');", category->getId(), category->getName().c_str(), category->getColor().c_str());
        file << sql << endl;
        delete category;
        counter++;
    }
    list<Event*> events_list = get_all_events();
    for (Event* event : events_list)
    {
        string description_single_line = event->getDescription();
        size_t start_pos = 0;
        while((start_pos = description_single_line.find("\n", start_pos)) != std::string::npos)
        {
            description_single_line.replace(start_pos, 1, "\\n");
            start_pos += 2;
        }
        snprintf(sql, 1024, "INSERT INTO Events VALUES(%u, '%s', '%s', '%s', %u, %ld, %ld);", event->getId(), event->getName().c_str(), description_single_line.c_str(), event->getPlace().c_str(), event->getCategory()->getId(), event->getStart(), event->getEnd());
        file << sql << endl;
        delete event;
        counter++;
    }
    return counter;
}

/* it's easy load a crafted file with malicious queries (e.g. "DELETE ... ")
 * we assume the user knows the source and the content of the file.
 */
int PManager::load_db(const string& path)
{
    if ((path.length() < 5) || (path.substr(path.length()-4, 4) != ".kal"))
        return 0;
    ifstream file;
    string line;
    int rc = 0;
    int counter = 0;
    char* errorMessage = nullptr;
    file.open(path);
    while ( getline (file,line) && (line.substr(0, 6) == "INSERT") )
    {
        //Transform line into a multiline (replacing \n)
        size_t start_pos = 0;
        while((start_pos = line.find("\\n", start_pos)) != std::string::npos)
        {
            line.replace(start_pos, 2, "\n");
            start_pos += 1;
        }
        rc = sqlite3_exec(db_, line.c_str(), nullptr, nullptr, &errorMessage);
        if (rc != SQLITE_OK )
            error("SQL error", errorMessage);
        else
            counter++;
    }
    return counter;
}

int PManager::import_db_iCal_format(const string& path, Category* category)
{
    auto category_id = category->getId();
    if ((path.length() < 5) || (path.substr(path.length()-4, 4) != ".ics"))
        return 0;
    ifstream file;
    string line;
    string pattern;
    string summary;
    string location;
    string description;
    bool found_description = false;
    int counter = 0;
    struct tm start;
    start.tm_sec = start.tm_min = start.tm_hour = start.tm_wday = start.tm_yday = start.tm_year = start.tm_mday = start.tm_mon = 0;
    struct tm end;
    end.tm_sec = end.tm_min = end.tm_hour = end.tm_wday = end.tm_yday = end.tm_year = end.tm_mday = end.tm_mon = 0;
    /* Look at the explanation in get_events_of_month */
    time_t threshold = 26262000; // = 1 November 1970
    std::tm* t = localtime(&threshold);
    int s = 0;
    if (t->tm_isdst > 0)
        s = 1;

    file.open(path);
    while ( getline (file,line) )
    {
        pattern = "DTSTART;VALUE=DATE:";
        if (line.find(pattern) == 0)
        { //if line starts with the pattern
            found_description = false;
            string date = line.substr(pattern.length(),line.length()-pattern.length());
            start.tm_year = stoi(date.substr(0,4)) - 1900;
            start.tm_mon = stoi(date.substr(4,2)) - 1;
            start.tm_mday = stoi(date.substr(6,2));
            start.tm_hour = 8;
            start.tm_isdst = ((start.tm_mon > 2) && (start.tm_mon < 10+s));
            continue;
        }
        pattern = "DTEND;VALUE=DATE:";
        if (line.find(pattern) == 0)
        {
            found_description = false;
            string date = line.substr(pattern.length(),line.length()-pattern.length());
            end.tm_year = stoi(date.substr(0,4)) - 1900;
            end.tm_mon = stoi(date.substr(4,2)) - 1;
            end.tm_mday = stoi(date.substr(6,2)) - 1; /* -1 is to get the day before, mktime will normalize it */
            end.tm_hour = 22;
            end.tm_isdst = ((end.tm_mon > 2) && (end.tm_mon < 10+s));
            continue;
        }
        pattern = "SUMMARY:";
        if (line.find(pattern) == 0)
        {
            found_description = false;
            summary = line.substr(pattern.length(),line.length()-pattern.length());
            continue;
        }
        pattern = "LOCATION:";
        if (line.find(pattern) == 0)
        {
            found_description = false;
            location = line.substr(pattern.length(),line.length()-pattern.length());
            if (location.length() < 3) location = "";
            continue;
        }
        pattern = "DESCRIPTION:";
        if (line.find(pattern) == 0)
        {
            found_description = true;
            description = line.substr(pattern.length(),line.length()-pattern.length());
            if (description.length() < 3) description = "";
            continue;
        }
        pattern = "END:VEVENT";
        if (line.find(pattern) == 0)
        {
            found_description = false;
            if (add_event(new Event(0,summary,description,location,get_category(category_id),mktime(&start),mktime(&end))))
                counter++;
            else
                printf("Error: %s not imported\n", summary.c_str());
            /* Reset optional variables to import the next event without old values */
            location = "";
            description = "";
            continue;
        }
        if (found_description) /* Multi-line description */
            description = description + "\n" + line;
    }
    return counter;
}

int PManager::export_db_iCal_format(list<Event*> events, string path)
{
    if (path.length() < 5)
        return 0;
    if (path.substr(path.length()-4, 4) != ".ics")
        path = path + ".ics";
    char buff[9];
    ofstream file;
    file.open(path);
    file << "BEGIN:VCALENDAR" << endl;
    file << "CALSCALE:GREGORIAN" << endl;
    for (Event* event : events)
    {
        file << "BEGIN:VEVENT" << endl;
        time_t tmp = event->getStart();
        strftime(buff, sizeof(buff),"%Y%m%d",localtime(static_cast<const time_t*>(&tmp)));
        file << "DTSTART;VALUE=DATE:" << buff << endl;
        tmp = event->getEnd();
        struct tm *end = localtime(static_cast<const time_t*>(&tmp));
        end->tm_mday += 1;
        tmp = mktime(end);
        strftime(buff, sizeof(buff),"%Y%m%d",localtime(static_cast<const time_t*>(&tmp)));
        file << "DTEND;VALUE=DATE:" << buff << endl;
        file << "UID:" << to_string(event->getId()) << endl;
        file << "DESCRIPTION:" << event->getDescription() << endl;
        file << "LOCATION:" << event->getPlace() << endl;
        file << "SUMMARY:" << event->getName() << endl;
        file << "END:VEVENT" << endl;
    }
    file << "END:VCALENDAR" << endl;
    return static_cast<int>(events.size());
}

PManager::~PManager()
{
    sqlite3_close(db_);
}
