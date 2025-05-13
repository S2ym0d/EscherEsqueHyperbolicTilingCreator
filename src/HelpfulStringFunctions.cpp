#include "HelpfulStringFunctions.h"
#include <cctype>
#include <sstream>

std::string TrimLeadingSpaces(const std::string& str)
{
    size_t idx = 0;

    while (idx < str.length() && std::isspace(static_cast<unsigned char>(str.at(idx)))) 
    {
        idx++;
    }

    return str.substr(idx);
}

std::string TrimTrailingSpaces(const std::string& str)
{
    size_t idx = str.length();

    while (idx > 0 && std::isspace(static_cast<unsigned char>(str.at(idx - 1)))) 
    {
        idx--;
    }

    return str.substr(0, idx);
}

std::string TrimLeadingAndTrailingSpaces(const std::string& str)
{
    return TrimLeadingSpaces(TrimTrailingSpaces(str));
}

std::string CompressSpaces(const std::string& str) {
    std::string result;
    bool inBlockOfSpaces = false;

    for (size_t i{ 0 }; i < str.size(); i++)
    {
        if (std::isspace(static_cast<unsigned char>(str.at(i))) && !inBlockOfSpaces)
        {
            result += ' ';
            inBlockOfSpaces = true;
        }
        else 
        {
            result += str.at(i);
            inBlockOfSpaces = false;
        }
    }

    return result;
}

std::vector<std::string> SplitString(const std::string& str, const std::string& separator)
{
    std::vector<std::string> result;
    size_t start = 0;
    size_t pos;

    while ((pos = str.find(separator, start)) != std::string::npos)
    {
        result.push_back(str.substr(start, pos - start));
        start = pos + separator.length();
    }

    result.push_back(str.substr(start));

    return result;
}

std::string RemoveAllOccurrencesOfSubstring(const std::string& str, const std::string& substring)
{
    auto splited = SplitString(str, substring);

    std::stringstream ssWithoutSubstring;

    for (size_t i{ 0 }; i < splited.size(); i++)
    {
        ssWithoutSubstring << splited.at(i);
    }

    return ssWithoutSubstring.str();
}

std::string SubstringBeforeFirstOccurrenceOfSeparator(const std::string& str, const std::string& separator)
{
    return SplitString(str, separator).at(0);
}

std::string SubstringAfterFirstOccurrenceOfSeparator(const std::string& str, const std::string& separator)
{
    auto splited = SplitString(str, separator);

    std::stringstream ssAfterFirstOccurrence;

    for (size_t i{ 1 }; i < splited.size(); i++)
    {
        ssAfterFirstOccurrence << splited.at(i);

        if (i < splited.size() - 1)
        {
            ssAfterFirstOccurrence << separator;
        }
    }

    return ssAfterFirstOccurrence.str();
}
