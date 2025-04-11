#pragma once

#include "Project.h"
#include "BMP.h"

#include "Modal.h"
#include "FileSelector.h"

namespace yap
{
    class ShareModal : public Modal
    {
    public:
        ShareModal(const std::shared_ptr<Project>& project)
        {
            auto header = CreateHeader("Exportar Projeto");
            auto body = CreateBody();

            auto fileSelector = std::make_shared<FileSelector>();

            auto field = std::make_shared<Box>();

            auto label = std::make_shared<Text>();
            auto nameInput = std::make_shared<TextInput>();

            auto buttons = std::make_shared<Box>();

            auto cancelButton = CreateTextButton("Cancelar");
            auto openButton = CreateTextButton("Exportar");

            label->Content = "Nome do arquivo: ";

            nameInput->SetValue("imagem.bmp");

            field->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
                    .WithGap(8)
            );

            field->AddChild(label);
            field->AddChild(nameInput);

            cancelButton->OnMousePress = [this](Element& e) {
                Close();
            };

            openButton->OnMousePress = [this, project, fileSelector, nameInput](Element& e) {
                std::string basePath = fileSelector->GetPath();
                std::string fileName = nameInput->GetValue();

                if (fileName.empty())
                {
                    fileName = "imagem.bmp";
                }

                std::string path = Path::Join({ basePath, fileName });

                const Bitmap& bitmap = *project->RenderCanvas();

                BMP::Save(path, bitmap);

                Close();
            };

            buttons->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithGap(8)
            );

            buttons->AddChild(cancelButton);
            buttons->AddChild(openButton);

            body->AddChild(fileSelector);
            body->AddChild(field);
            body->AddChild(buttons);

            OnMount = [this, fileSelector](Element& element)
            {
                fileSelector->SetPath("./YAP/images");
            };

            AddChild(header);
            AddChild(body);
        }
    };
}