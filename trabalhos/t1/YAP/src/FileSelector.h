#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "TextInput.h"

namespace yap
{
    class FileSelector : public Box
    {
    private:
        std::shared_ptr<TextInput> m_PathInput;
        std::shared_ptr<Box> m_Items;

    public:
        FileSelector()
        {
            m_PathInput = std::make_shared<TextInput>();
            m_Items = std::make_shared<Box>();
                
            m_PathInput->SetValue(".");

            m_Items->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fill())
                    .WithDirection(BoxDirection::Column)
                    .WithGap(1)
            );

            SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
            );

            OnMount = [this](Element& element)
            {
                RefreshItems();
            };

            AddChild(m_PathInput);
            AddChild(m_Items);
        }
    
    private:
        void RefreshItems()
        {
            m_Items->ClearChildren();

            const std::string& path = m_PathInput->GetValue();

            struct dirent *de;
            DIR *dir = opendir(path.c_str());

            while (dir)
            {
                de = readdir(dir);
                if (de == nullptr)
                {
                    break;
                }

                m_Items->AddChild(CreateItem(path, de->d_name));
            }
        }

        std::shared_ptr<Box> CreateItem(const std::string& path, const std::string& filename)
        {
            auto item = std::make_shared<Box>();
            auto text = std::make_shared<Text>();

            struct stat path_stat;
            stat(path.c_str(), &path_stat);

            text->Content = filename;

            if (S_ISREG(path_stat.st_mode))
            {

            }

            text->SetStyle(
                StyleSheet()
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithForeground(ColorRGB(255, 255, 255))
            );

            item->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fixed(24))
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithBackground(BoxBackground::Solid(ColorRGB(44, 44, 44)))
                    .WithPadding(BoxPadding(4, 0))
            );

            item->SetStyle(
                ":hover",
                StyleSheet()
                    .WithBackground(BoxBackground::Solid(ColorRGB(56, 56, 56)))
            );

            item->SetStyle(
                ":focus",
                StyleSheet()
                    .WithBackground(BoxBackground::Solid(ColorRGB(74, 80, 124)))
            );

            item->AddChild(text);


            return item;
        }
    };
}