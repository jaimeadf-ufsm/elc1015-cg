#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

/**
 * @file Path.h
 * @brief Provides utility functions for manipulating and analyzing file paths.
 * 
 * This header defines the `yap::Path` class, which includes static methods for
 * splitting, joining, normalizing, and extracting components of file paths.
 */

namespace yap
{
    /**
     * @class Path
     * @brief A utility class for handling file path operations.
     * 
     * The `Path` class provides static methods to manipulate and analyze file paths,
     * including splitting paths into components, joining components into a path,
     * normalizing paths, and extracting directory names, base names, and extensions.
     */
    class Path
    {
    public:
        /**
         * @brief The delimiter used to separate path components.
         */
        static const char Delimiter = '/';

        /**
         * @brief Splits a file path into its components.
         * 
         * @param str The file path to split.
         * @return A vector of strings representing the components of the path.
         * 
         * Components such as "." are ignored, and empty components are skipped.
         */
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

        /**
         * @brief Joins a vector of path components into a single path.
         * 
         * @param paths A vector of strings representing path components.
         * @return A single string representing the joined path.
         */
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

        /**
         * @brief Normalizes a file path by resolving "." and ".." components.
         * 
         * @param path The file path to normalize.
         * @return A normalized path string.
         * 
         * The method removes redundant "." components and resolves ".." components
         * by removing the preceding directory, if possible.
         */
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

        /**
         * @brief Extracts the directory name from a file path.
         * 
         * @param path The file path.
         * @return The directory name as a string.
         * 
         * If the path contains only one component, "." is returned.
         */
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

        /**
         * @brief Extracts the base name (file name) from a file path.
         * 
         * @param path The file path.
         * @return The base name as a string.
         * 
         * If the path is empty, the original path is returned.
         */
        static std::string BaseName(const std::string& path)
        {
            std::vector<std::string> parts = Path::Split(path);

            if (!parts.empty())
            {
                return parts.back();
            }

            return path;
        }

        /**
         * @brief Extracts the file extension from a file path.
         * 
         * @param path The file path.
         * @return The file extension as a string, or an empty string if none exists.
         * 
         * The method returns the substring after the last '.' in the path, unless
         * the '.' is the first or last character.
         */
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