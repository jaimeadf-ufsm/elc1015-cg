#include <regex>
#include <deque>
#include <sstream>

#include "PathParser.h"

std::deque<std::string> Tokenize(const std::string& d)
{
    std::deque<std::string> tokens;

    std::regex regex("((-?)\\d+(?:\\.\\d+)?(?:e(?:-?)\\d+)?)|([A-Za-z])");

    auto tokensBegin = std::sregex_iterator(d.begin(), d.end(), regex);
    auto tokensEnd = std::sregex_iterator();

    for (auto it = tokensBegin; it != tokensEnd; it++)
    {
        std::smatch match = *it;
        std::string content = match.str();

        tokens.emplace_back(content);
    }

    return tokens;
}

bool ParseCommand(const std::string& token, char& c)
{
    if (token.size() != 1 || !std::isalpha(token.front()))
    {
        return false;
    }

    c = token.front();

    return true;
}

bool ParseNumber(const std::string& token, float& number)
{
    // float tmp;

    std::istringstream iss(token);
    iss >> number;

    // number = tmp;

    return iss.eof() && !iss.fail();
}

bool ReadCommand(std::deque<std::string>& tokens, char& c)
{
    if (tokens.size() < 1)
    {
        return false;
    }

    if (ParseCommand(tokens.front(), c))
    {
        tokens.pop_front();
        return true;
    }

    return false;
}

bool ReadNumber(std::deque<std::string>& tokens, float& number)
{
    if (tokens.size() < 1)
    {
        return false;
    }

    if (ParseNumber(tokens.front(), number))
    {
        tokens.pop_front();
        return true;
    }

    return false;
}

bool ReadCoordinate(std::deque<std::string>& tokens, Vector2& coordinate)
{
    if (tokens.size() < 2)
    {
        return false;
    }

    if (ParseNumber(tokens[0], coordinate.X) && ParseNumber(tokens[1], coordinate.Y))
    {
        tokens.pop_front();
        tokens.pop_front();
        return true;
    }

    return false;
}

std::vector<Path> PathParser::ParseCommands(const std::string& d)
{
    std::vector<Path> paths;
    std::deque<std::string> tokens = Tokenize(d);

    paths.emplace_back();

    Vector2 lastCoordinate;

    while (tokens.size() > 0)
    {
        char command;

        float auxiliaryNumber;

        Vector2 auxiliaryCoordinate1;
        Vector2 auxiliaryCoordinate2;
        Vector2 auxiliaryCoordinate3;

        if (!ReadCommand(tokens, command))
        {
            break;
        }

        switch (command)
        {
            case 'M':
                ReadCoordinate(tokens, auxiliaryCoordinate1);

                if (!paths.back().IsEmpty())
                {
                    paths.emplace_back();
                }

                paths.back().Clear();
                paths.back().MoveTo(auxiliaryCoordinate1);

                lastCoordinate = auxiliaryCoordinate1;

                while (ReadCoordinate(tokens, auxiliaryCoordinate1))
                {
                    paths.back().LineTo(auxiliaryCoordinate1);
                    lastCoordinate = auxiliaryCoordinate1;
                }

                break;
            case 'L':
                while (ReadCoordinate(tokens, auxiliaryCoordinate1))
                {
                    paths.back().LineTo(auxiliaryCoordinate1);
                    lastCoordinate = auxiliaryCoordinate1;
                }

                break;
            case 'Q':
                while (ReadCoordinate(tokens, auxiliaryCoordinate1) && ReadCoordinate(tokens, auxiliaryCoordinate2))
                {
                    paths.back().QuadraticBezierTo(auxiliaryCoordinate1, auxiliaryCoordinate2);
                    lastCoordinate = auxiliaryCoordinate2;
                }

                break;
            case 'C':
                while (ReadCoordinate(tokens, auxiliaryCoordinate1) && ReadCoordinate(tokens, auxiliaryCoordinate2) && ReadCoordinate(tokens, auxiliaryCoordinate3))
                {
                    paths.back().CubicBezierTo(auxiliaryCoordinate1, auxiliaryCoordinate2, auxiliaryCoordinate3);
                    lastCoordinate = auxiliaryCoordinate3;
                }
                
                break;
            case 'H':
                while (ReadNumber(tokens, auxiliaryNumber))
                {
                    paths.back().LineTo(Vector2(auxiliaryNumber, lastCoordinate.Y));
                    lastCoordinate.X = auxiliaryNumber;
                }


                break;
            case 'V':
                while (ReadNumber(tokens, auxiliaryNumber))
                {
                    paths.back().LineTo(Vector2(lastCoordinate.X, auxiliaryNumber));
                    lastCoordinate.Y = auxiliaryNumber;
                }

                break;
            case 'Z':
            case 'z':
                paths.back().Close();
                break;
            default:
                throw std::runtime_error(std::string("Invalid command: ") + command);
                break;
        }
    }

    return paths;
}