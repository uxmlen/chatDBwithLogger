#pragma once

#include <stdexcept>

struct busy_login_error : public std::runtime_error {
    busy_login_error()
        : std::runtime_error(
            "\u001b[31merror: this username is already taken\u001b[0m")
    {
    }
};

struct user_not_existed_error : public std::runtime_error {
    user_not_existed_error()
        : std::runtime_error(
            "\u001b[31merror: this user does not exist\u001b[0m")
    {
    }
};

struct pass_or_login_not_correct_error : public std::runtime_error {
    pass_or_login_not_correct_error()
        : std::runtime_error(
            "\u001b[31merror: the password or username is not correct\u001b[0m")
    {
    }
};
