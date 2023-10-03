#include <cstdlib>
#include <cstring>
#include <iostream>
#include <unistd.h>

#include "chat.h"
#include "exceptions.h"

namespace {

const std::string BLACK = "\u001B[30m";
const std::string GREEN = "\u001B[32m";
const std::string YELLOW = "\u001B[33m";
const std::string BLUE = "\u001B[34m";
const std::string CYAN = "\u001B[36m";
const std::string WHITE = "\u001B[37m";
const std::string GREEN_BG = "\u001B[42m";
const std::string BLUE_BG = "\u001B[44m";
const std::string CYAN_BG = "\u001B[46m";
const std::string WHITE_BG = "\u001B[47m";
const std::string RESET = "\u001B[0m";

} // namespace

/// \brief gets an integer number from the input
static int getInt()
{
    int input = 0;
    while (true) {
        std::cin >> input;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            std::cerr << YELLOW << "Wrong input. Try again \n" << RESET;
        } else {
            std::cin.ignore();
            return input;
        }
    }
}

static void menuPrompt(const std::string& name,
    const std::vector<std::string>& actions, const std::string& color,
    const std::string& background)
{
    int i = 0;
    std::cout << std::endl
              << color << background << " " << name << " " << RESET << "    ";
    for (const auto& a : actions) {
        std::cout << color << background << " " << i++ << " " << RESET << a
                  << "  ";
    }
    std::cout << "\nChoose the action: ";
}

void Chat::showAuthMenu()
{
    std::vector<std::string> actions = { "exit", "sign in", "sign up" };
    while (!is_authorize_) {
        menuPrompt("Main menu", actions, WHITE, GREEN_BG);
        int action = getInt();
        switch (action) {
        case 0:
            exit(0);
        // регистрация нового пользователя
        case 2:
            signUp();
            break;
            // вход по логину ипаролю
        case 1:
            signIn();
            break;
        defualt:
            std::cout << "There is not such in the menu" << std::endl;
        }
    }
}

void Chat::signIn()
{
    std::string username, password;
    std::cout << "Enter the username: ";
    std::cin >> username;
    if (!conn_to_db_.isUserExisted(username)) {
        throw user_not_existed_error();
    }
    std::cout << "Password: ";
    std::cin >> password;
    if (!conn_to_db_.isUserExisted(username, password)) {
        throw pass_or_login_not_correct_error();
    }
    current_user_id_ = conn_to_db_.getUserID(username);
    username_ = username;
    is_authorize_ = true;
}

void Chat::selectAction(int action)
{
    switch (action) {
    case 0:
        exit(0);
    case 1:
        conn_to_db_.getAllMessages();
        break;

    case 2:
        conn_to_db_.getAllUsers();
        break;

    case 3:
        std::cout << "enter the messages " << std::endl;
        std::string msg, username;
        std::getline(std::cin, msg, '\n');
        /*std::cout << "enter the username or all" << std::endl;
        bool l = true;
        while (l) {
            std::cin >> username;
            if (conn_to_db_.isUserExisted(username) || username == "all") {
                l = false;
            } else {
                std::cout << "username is not existed!" << std::endl;
                std::cout << "username: ";
            }
        } */
        log.write("all", username_, msg);
        conn_to_db_.sendMessage("all", msg, current_user_id_);
        break;
    }
}

void Chat::showMenu()
try {
    std::cout << "Welcome to Chat!" << std::endl;
    std::cout << std::endl
              << "Number of Reistered users: " << conn_to_db_.getNumUserInDB()
              << std::endl;

    showAuthMenu();

    while (true) {
        std::vector<std::string> actions = { "log out", "display message",
            "all users in the db", "send a message" };

        menuPrompt("User's Menu", actions, WHITE, CYAN_BG);
        int action = getInt();
        selectAction(action);
    }
} catch (std::exception& e) {
    std::cerr << e.what() << std::endl;
}

void Chat::signUp()
{
    std::string username, password;
    std::cout << "username: ";
    bool l = true;
    while (l) {
        std::cin >> username;
        if (conn_to_db_.isUserExisted(username)) {
            std::cout << "username is busy!" << std::endl;
            std::cout << "username: ";
        } else {
            l = false;
        }
    }
    std::cout << "Password: ";
    std::cin >> password;
    conn_to_db_.createdUser(username, password);
    conn_to_db_.getAllUsers();
    std::cout << "Welcome to Chat!" << std::endl;

    std::cout << GREEN << "Account was created" << RESET << std::endl;
}
