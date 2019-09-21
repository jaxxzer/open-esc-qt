#include <com-parser.h>

ComParser::ComParser()
{}

ComParser::ParseState ComParser::parseByte(uint8_t b)
{
    return ParseState::ERROR;
}
