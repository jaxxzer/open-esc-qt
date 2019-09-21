#ifndef COMPARSER_H
#define COMPARSER_H

#include <inttypes.h>

class ComParser
{
public:
    ComParser();

    enum ParseState
    {
        ERROR,
        PARSING,
        NEW_MESSAGE,
    };

    ParseState parseByte(uint8_t b);
};

#endif // COMPARSER_H
