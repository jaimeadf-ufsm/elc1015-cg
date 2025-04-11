#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "Path.h"
#include "TextInput.h"

/**
 * @file FileSelector.h
 * @brief Provides a graphical file selector component for navigating directories and selecting files.
 * 
 * This class allows users to navigate through directories, view files, and select a file or folder.
 * It supports pagination for large directories and provides a graphical interface with icons for files and folders.
 */

namespace yap
{
    /**
     * @class FileSelector
     * @brief A graphical component for navigating directories and selecting files or folders.
     */
    class FileSelector : public Box
    {
    private:
        std::string m_CurrentPath; ///< The current directory path being displayed.
        std::vector<std::string> m_CurrentFiles; ///< List of files and folders in the current directory.

        int m_ItemsPerPage = 10; ///< Number of items displayed per page.
        int m_CurrentPage; ///< The current page index.

        std::string m_SelectedPath; ///< The currently selected file or folder path.

        std::shared_ptr<TextInput> m_PathInput; ///< Input field for entering or displaying the current path.
        std::shared_ptr<Text> m_PageIndicator; ///< Text element displaying the current page and total pages.

        std::shared_ptr<Box> m_Items; ///< Container for displaying file and folder items.

        std::shared_ptr<Bitmap> m_FileIcon; ///< Icon for files.
        std::shared_ptr<Bitmap> m_FolderIcon; ///< Icon for folders.

    public:
        /**
         * @brief Constructs a FileSelector instance and initializes its components.
         */
        FileSelector()
        {
            m_CurrentPath = ".";

            m_PathInput = std::make_shared<TextInput>();
            m_PageIndicator = std::make_shared<Text>();

            m_Items = std::make_shared<Box>();

            m_FileIcon = std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/file-24x24.bmp"));
            m_FolderIcon = std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/folder-24x24.bmp"));

            auto controls = std::make_shared<Box>();

            auto previousButton = CreateControlButton(std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/chevron-left-24x24.bmp")));
            auto nextButton = CreateControlButton(std::make_shared<Bitmap>(BMP::Load("Trab1JaimeADF/assets/chevron-right-24x24.bmp")));
                
            m_PathInput->SetValue(".");

            m_PathInput->OnSubmit = [this](TextInput& element)
            {
                NavigateTo(element.GetValue());
            };

            m_PathInput->OnCancel = [this](TextInput& element)
            {
                m_PathInput->SetValue(m_CurrentPath);
            };

            m_PathInput->OnLeave = [this](TextInput& element)
            {
                if (element.GetValue() != m_CurrentPath)
                {
                    element.SetValue(m_CurrentPath);
                }
            };

            previousButton->OnMousePress = [this](Element& element)
            {
                PreviousPage();
            };

            nextButton->OnMousePress = [this](Element& element)
            {
                NextPage();
            };

            controls->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithGap(8)
            );

            controls->AddChild(m_PathInput);
            controls->AddChild(previousButton);
            controls->AddChild(m_PageIndicator);
            controls->AddChild(nextButton);

            m_Items->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(24 * m_ItemsPerPage))
                    .WithDirection(BoxDirection::Column)
                    .WithGap(1)
            );

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
                    .WithGap(8)
            );

            OnMount = [this](Element& element)
            {
                NavigateTo(m_CurrentPath);
            };

            AddChild(controls);
            AddChild(m_Items);
        }

        /**
         * @brief Sets the current path and navigates to it.
         * @param path The directory path to navigate to.
         */
        void SetPath(const std::string& path)
        {
            NavigateTo(path);
        }

        /**
         * @brief Gets the current directory path.
         * @return The current directory path.
         */
        const std::string& GetPath() const
        {
            return m_CurrentPath;
        }

        /**
         * @brief Gets the currently selected file or folder path.
         * @return The selected file or folder path.
         */
        const std::string& GetSelectedPath() const
        {
            return m_SelectedPath;
        }

    private:
        /**
         * @brief Navigates to the specified directory path.
         * @param path The directory path to navigate to.
         */
        void NavigateTo(const std::string& path)
        {
            std::string normalizedPath = Path::Normalize(path);

            struct stat path_stat;
            stat(normalizedPath.c_str(), &path_stat);

            if (S_ISDIR(path_stat.st_mode))
            {
                m_CurrentPath = normalizedPath;
                m_CurrentPage = 0;
                m_CurrentFiles.clear();
                m_SelectedPath.clear();

                struct dirent* de;
                DIR* dir = opendir(m_CurrentPath.c_str());

                while (dir)
                {
                    de = readdir(dir);

                    if (de == nullptr)
                    {
                        break;
                    }

                    std::string filename = de->d_name;

                    if (filename == ".")
                    {
                        continue;
                    }

                    m_CurrentFiles.push_back(filename);
                }

                RefreshInput();
                RefreshPageIndicator();
                RefreshItems();
            }
        }

        /**
         * @brief Selects a file or folder.
         * @param path The path of the file or folder to select.
         */
        void SelectPath(const std::string& path)
        {
            m_SelectedPath = path;
            RefreshItems();
        }

        /**
         * @brief Navigates to the next page of items.
         */
        void NextPage()
        {
            ChangePage(m_CurrentPage + 1);
        }

        /**
         * @brief Navigates to the previous page of items.
         */
        void PreviousPage()
        {
            ChangePage(m_CurrentPage - 1);
        }

        /**
         * @brief Changes the current page to the specified page index.
         * @param page The page index to navigate to.
         */
        void ChangePage(int page)
        {
            int totalPages = CountPages();

            m_CurrentPage = (page + totalPages) % totalPages;
            RefreshPageIndicator();
            RefreshItems();
        }

        /**
         * @brief Counts the total number of pages based on the number of items and items per page.
         * @return The total number of pages.
         */
        int CountPages()
        {
            return (m_CurrentFiles.size() + m_ItemsPerPage - 1) / m_ItemsPerPage;
        }

        /**
         * @brief Refreshes the path input field with the current path.
         */
        void RefreshInput()
        {
            m_PathInput->SetValue(m_CurrentPath);
        }

        /**
         * @brief Updates the page indicator with the current page and total pages.
         */
        void RefreshPageIndicator()
        {
            m_PageIndicator->Content = std::to_string(m_CurrentPage + 1) + " / " + std::to_string(CountPages());
        }

        /**
         * @brief Refreshes the displayed items based on the current page.
         */
        void RefreshItems()
        {
            m_Items->ClearChildren();

            int startIndex = m_CurrentPage * m_ItemsPerPage;
            int endIndex = std::min(startIndex + m_ItemsPerPage, static_cast<int>(m_CurrentFiles.size()));

            for (int i = startIndex; i < endIndex; ++i)
            {
                m_Items->AddChild(CreateItem(m_CurrentPath, m_CurrentFiles[i]));
            }
        }

        /**
         * @brief Creates a graphical item representing a file or folder.
         * @param path The directory path of the item.
         * @param filename The name of the file or folder.
         * @return A shared pointer to the created item.
         */
        std::shared_ptr<Box> CreateItem(const std::string& path, const std::string& filename)
        {
            auto item = std::make_shared<Box>();
            auto icon = std::make_shared<Box>();
            auto text = std::make_shared<Text>();

            std::string fullPath = Path::Join({ path, filename });

            struct stat path_stat;
            stat(fullPath.c_str(), &path_stat);
            
            bool isDirectory = S_ISDIR(path_stat.st_mode);
            bool isSelected = (fullPath == m_SelectedPath);

            icon->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(24), AxisSizingRule::Fixed(24))
            );

            if (isDirectory)
            {
                icon->SetStyle(
                    icon->GetStyle()
                        .WithBackground(BoxBackground::Image(m_FolderIcon))
                );
            }
            else
            {
                icon->SetStyle(
                    icon->GetStyle()
                        .WithBackground(BoxBackground::Image(m_FileIcon))
                );
            }

            text->Content = filename;

            text->SetStyle(
                StyleSheet()
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithForeground(ColorRGB(255, 255, 255))
            );

            item->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(24))
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithPadding(BoxPadding(4, 0))
                    .WithGap(8)
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
            );

            if (isSelected)
            {
                item->SetStyle(
                    item->GetStyle()
                        .WithBackground(BoxBackground::Solid(ColorRGB(74, 80, 124)))
                );

                icon->SetStyle(
                    icon->GetStyle()
                        .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(74, 80, 124)))
                );
            }
            else
            {
                item->SetStyle(
                    ":hover",
                    StyleSheet()
                        .WithBackground(BoxBackground::Solid(ColorRGB(56, 56, 56)))
                );
            }

            item->OnAnimate = [this, icon, isSelected](Element& element)
            {
                if (!isSelected)
                {
                    if (element.IsHovered())
                    {
                        icon->SetStyle(
                            icon->GetStyle()
                                .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(56, 56, 56)))
                        );
                    }
                    else
                    {
                        icon->SetStyle(
                            icon->GetStyle()
                                .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(44, 44, 44)))
                        );
                    }
                }
            };

            if (isDirectory)
            {
                item->OnMousePress = [this, fullPath](Element& element)
                {
                    std::shared_ptr<Screen> screen = element.GetScreen();

                    if (screen)
                    {
                        screen->ExecuteNextFrame([this, fullPath]()
                        {
                            NavigateTo(fullPath);
                        });
                    }
                };
            }
            else
            {
                item->OnMousePress = [this, fullPath](Element& element)
                {
                    std::shared_ptr<Screen> screen = element.GetScreen();

                    screen->ExecuteNextFrame([this, fullPath]()
                    {
                        SelectPath(fullPath);
                    });
                };
            }

            item->AddChild(icon);
            item->AddChild(text);

            return item;
        }

        /**
         * @brief Creates a control button with the specified icon.
         * @param icon The bitmap icon for the button.
         * @return A shared pointer to the created button.
         */
        std::shared_ptr<Box> CreateControlButton(const std::shared_ptr<Bitmap>& icon)
        {
            auto button = std::make_shared<Box>();

            button->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fixed(24), AxisSizingRule::Fixed(24))
                    .WithBackground(BoxBackground::Image(icon))
                    .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(44, 44, 44)))
            );

            button->SetStyle(
                ":hover",
                StyleSheet()
                    .WithBackgroundReference(BoxBackgroundTransparencyReference::Static(ColorRGB(56, 56, 56)))
            );

            return button;
        }
    };
}