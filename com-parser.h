#ifndef COMPARSER_H
#define COMPARSER_H

#include <inttypes.h>
#include <QByteArray>


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
};

#endif // COMPARSER_H
