#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

namespace yap
{
    class Path
    {
    public:
        static const char Delimiter = '/';

        static std::vector<std::string> Split(const std::string& str)
        {
            std::vector<std::string> result;
            std::stringstream ss(str);
            std::string item;

            while (std::getline(ss, item, Delimiter))
            {
                if (!item.empty() && item != ".")
                {
                    result.push_back(item);
                }
            }

            return result;
        }

        static std::string Join(const std::vector<std::string>& paths)
        {
            std::string result;

            for (const auto& part : paths)
            {
                if (!result.empty())
                {
                    result += Delimiter;
                }

                result += part;
            }

            return result;
        }

        static std::string Normalize(const std::string& path)
        {
            std::vector<std::string> parts = Split(path);
            std::vector<std::string> stack;

            for (const auto& part : parts)
            {
                if (part == ".." && !stack.empty() && stack.back() != "..")
                {
                    stack.pop_back();
                }
                else
                {
                    stack.push_back(part);
                }
            }

            if (stack.empty())
            {
                return ".";
            }

            if (*stack.begin() != "..")
            {
                stack.insert(stack.begin(), ".");
            }

            return Join(stack);
        }

        static std::string DirName(const std::string& path)
        {
            std::vector<std::string> parts = Path::Split(path);

            if (parts.size() > 1)
            {
                parts.pop_back();

                return Path::Join(parts);
            }

            return ".";
        }

        static std::string BaseName(const std::string& path)
        {
            std::vector<std::string> parts = Path::Split(path);

            if (!parts.empty())
            {
                return parts.back();
            }

            return path;
        }

        static std::string Extension(const std::string& path)
        {
            size_t pos = path.find_last_of('.');

            if (pos != std::string::npos && pos != 0 && pos != path.length() - 1)
            {
                return path.substr(pos + 1);
            }

            return "";
        }
    };
}