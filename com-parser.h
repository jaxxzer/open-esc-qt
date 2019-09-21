#ifndef COMPARSER_H
#define COMPARSER_H

#include <inttypes.h>
#include <QByteArray>

#include <ping-parser.h>

class ComParser
{
public:
    ComParser();

    typedef enum
    {
        ERROR,
        PARSING,
        NEW_MESSAGE,
    } ParseState;

    ParseState parseByte(uint8_t b);
    PingParser parser;
};

#endif // COMPARSER_H
