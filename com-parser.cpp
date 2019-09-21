#include <com-parser.h>

#include <ping-parser.h>

ComParser::ComParser()
{}

ComParser::ParseState ComParser::parseByte(uint8_t b)
{
    switch (parser.parseByte(b)) {
    case PingParser::ParseState::NEW_MESSAGE:
        return ComParser::ParseState::NEW_MESSAGE;
    case PingParser::ParseState::ERROR:
        return ComParser::ParseState::ERROR;
    case PingParser::ParseState::WAIT_START:
    case PingParser::ParseState::WAIT_HEADER:
    case PingParser::ParseState::WAIT_LENGTH_L:
    case PingParser::ParseState::WAIT_LENGTH_H:
    case PingParser::ParseState::WAIT_MSG_ID_L:
    case PingParser::ParseState::WAIT_MSG_ID_H:
    case PingParser::ParseState::WAIT_SRC_ID:
    case PingParser::ParseState::WAIT_DST_ID:
    case PingParser::ParseState::WAIT_PAYLOAD:
    case PingParser::ParseState::WAIT_CHECKSUM_L:
    case PingParser::ParseState::WAIT_CHECKSUM_H:
        return ComParser::ParseState::PARSING;
    default:
        break;
    }
    return ComParser::ParseState::ERROR;
}
