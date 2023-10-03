#include "db.h"

DB::DB()
{
    conn_ = mysql_init(nullptr);
    conn_ = mysql_real_connect(
        conn_, "127.0.0.1", "root", "", "chat_cpp", 0, NULL, 0);
    if (conn_ == nullptr) {
        // TODO: exception
        std::cerr << "ERROR: " << mysql_error(conn_) << std::endl;
        mysql_close(conn_);
        exit(1);
    }
    mysql_set_character_set(conn_, "utf8");
}

DB::~DB()
{
    if (conn_) {
        mysql_close(conn_);
    }
}

void DB::createdUser(const std::string& username, const std::string& password)
{
    std::stringstream query;
    query << "INSERT INTO users (name, password) VALUES ('" << username
          << "', '" << password << "');";
    mysql_query(conn_, query.str().c_str());
}

int DB::getUserID(const std::string& username)
{
    MYSQL_RES* res;
    std::stringstream query;
    // if receiver_id == 0 then message to all users
    query << "SELECT user_id, name FROM users WHERE name = '" << username
          << "';";
    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    return atoi(mysql_fetch_row(res)[0]);
}

void DB::sendMessage(
    const std::string& username, const std::string& content, int author_id)
{
    int current_user_id = getUserID(username);
    std::stringstream query;
    // if receiver_id == 0 then message to all users
    query << "INSERT INTO messages (receiver_id, author_id, content) VALUES (0,"
          << current_user_id << ", '" << content << "');";
    mysql_query(conn_, query.str().c_str());
}

void DB::getAllMessages()
{
    MYSQL_RES* res;
    MYSQL_ROW row;

    mysql_query(conn_, "SELECT * FROM messages WHERE receiver_id = 0;");
    res = mysql_store_result(conn_);

    while ((row = mysql_fetch_row(res))) {
        for (int i = 0; i < mysql_num_fields(res); i++) {
            std::cout << row[i] << " ";
        }
        std::cout << std::endl;
    }
}

void DB::getAllUsers()
{
    MYSQL_RES* res;
    MYSQL_ROW row;

    mysql_query(conn_, "SELECT name, created_at FROM users");
    res = mysql_store_result(conn_);

    while ((row = mysql_fetch_row(res))) {
        for (size_t i = 0; i < mysql_num_fields(res); i++) {
            std::cout << row[i] << " ";
        }
        std::cout << std::endl;
    }
}

bool DB::isUserExisted(const std::string& username)
{
    MYSQL_RES* res;

    std::stringstream query;
    query << "SELECT * FROM users WHERE name = "
          << "'" << username << "';";

    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    if (res) {
        return mysql_num_rows(res) == 1;
    }
    return false;
}

bool DB::isUserExisted(const std::string& username, const std::string& password)
{
    MYSQL_RES* res;
    std::stringstream query;

    query << "SELECT * FROM users WHERE name = "
          << "'" << username << "' and password = '" << password << "';";

    mysql_query(conn_, query.str().c_str());
    res = mysql_store_result(conn_);
    if (res) {
        return mysql_num_rows(res) == 1;
    }
    return false;
}

int DB::getNumUserInDB()
{
    MYSQL_RES* res;
    mysql_query(conn_, "SELECT * FROM users;");

    res = mysql_store_result(conn_);
    if (res) {
        return mysql_num_rows(res);
    }
    return 0;
}
