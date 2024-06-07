#include "error.h"

std::string ToError(Error noerr) {
    switch (noerr.Errno)
    {
    case ErrorCode::NOError:
        return "no error";
    default:
        return "unkonw error";
    }
}