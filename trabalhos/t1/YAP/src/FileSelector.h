#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "Path.h"

#include "TextInput.h"

namespace yap
{
    class FileSelector : public Box
    {
    private:
        std::string m_CurrentPath;
        std::vector<std::string> m_CurrentFiles;

        int m_ItemsPerPage = 10;
        int m_CurrentPage;

        std::string m_SelectedPath;

        std::shared_ptr<TextInput> m_PathInput;
        std::shared_ptr<Text> m_PageIndicator;

        std::shared_ptr<Box> m_Items;

        std::shared_ptr<Bitmap> m_FileIcon;
        std::shared_ptr<Bitmap> m_FolderIcon;

    public:
        FileSelector()
        {
            m_CurrentPath = ".";

            m_PathInput = std::make_shared<TextInput>();
            m_PageIndicator = std::make_shared<Text>();

            m_Items = std::make_shared<Box>();

            m_FileIcon = std::make_shared<Bitmap>(BMP::Load("YAP/assets/file-24x24.bmp"));
            m_FolderIcon = std::make_shared<Bitmap>(BMP::Load("YAP/assets/folder-24x24.bmp"));

            auto controls = std::make_shared<Box>();

            auto previousButton = CreateControlButton(std::make_shared<Bitmap>(BMP::Load("YAP/assets/chevron-left-24x24.bmp")));
            auto nextButton = CreateControlButton(std::make_shared<Bitmap>(BMP::Load("YAP/assets/chevron-right-24x24.bmp")));
                
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

        void SetPath(const std::string& path)
        {
            NavigateTo(path);
        }

        const std::string& GetPath() const
        {
            return m_CurrentPath;
        }

        const std::string& GetSelectedPath() const
        {
            return m_SelectedPath;
        }

    private:
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

        void SelectPath(const std::string& path)
        {
            m_SelectedPath = path;
            RefreshItems();
        }

        void NextPage()
        {
            ChangePage(m_CurrentPage + 1);
        }

        void PreviousPage()
        {
            ChangePage(m_CurrentPage - 1);
        }

        void ChangePage(int page)
        {
            int totalPages = CountPages();

            m_CurrentPage = (page + totalPages) % totalPages;
            RefreshPageIndicator();
            RefreshItems();
        }

        int CountPages()
        {
            return (m_CurrentFiles.size() + m_ItemsPerPage - 1) / m_ItemsPerPage;
        }

        void RefreshInput()
        {
            m_PathInput->SetValue(m_CurrentPath);
        }

        void RefreshPageIndicator()
        {
            m_PageIndicator->Content = std::to_string(m_CurrentPage + 1) + " / " + std::to_string(CountPages());
        }

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