#pragma once
#include <string>
#include <vector>

std::string TrimLeadingSpaces(const std::string& str);

std::string TrimTrailingSpaces(const std::string& str);

std::string TrimLeadingAndTrailingSpaces(const std::string& str);

std::string CompressSpaces(const std::string& str);

std::vector<std::string> SplitString(const std::string& str, const std::string& separator);

std::string RemoveAllOccurrencesOfSubstring(const std::string& str, const std::string& substring);

std::string SubstringBeforeFirstOccurrenceOfSeparator(const std::string& str, const std::string& separator);

std::string SubstringAfterFirstOccurrenceOfSeparator(const std::string& str, const std::string& separator);