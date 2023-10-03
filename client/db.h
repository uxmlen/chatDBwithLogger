#pragma once

#include <iostream>
#include <mysql/mysql.h>
#include <sstream>

class DB {
public:
    DB();
    ~DB();

    void createdUser(const std::string& username, const std::string& password);
    void sendMessage(const std::string& username, const std::string& content,
        int current_user_id);
    // getter
    void getAllMessages();
    void getAllUsers();
    // check if the user exists in the db
    bool isUserExisted(const std::string& username);
    bool isUserExisted(
        const std::string& username, const std::string& password);
    // numbers of users in the database
    int getNumUserInDB();
    int getUserID(const std::string& username);

private:
    MYSQL* conn_;
};
