#ifndef _VJIT_ATT_ERROR_H
#define _VJIT_ATT_ERROR_H

#include <string>

enum struct ErrorCode {
    NOError = 0,
    CustomizeError,
};

struct Error {
    ErrorCode Errno;
};

static std::string ToError(Error noerr);

#endif 
