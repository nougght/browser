#ifndef INTERNAL_ERRORS_H
#define INTERNAL_ERRORS_H

#include <string>

enum class RepositoryErrorCode {
    Success = 0,
    DatabaseError = 1,
    NotFound = 2,
    AlreadyExists = 3,
};

// ошибка репозитория
struct RepositoryError {
    RepositoryErrorCode code;
    std::string message;
};

#endif