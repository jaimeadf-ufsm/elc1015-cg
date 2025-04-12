#pragma once

#include "Project.h"
#include "BMP.h"

#include "Modal.h"
#include "Checkbox.h"
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

            auto nameField = std::make_shared<Box>();

            auto nameLabel = std::make_shared<Text>();
            auto nameInput = std::make_shared<TextInput>();

            auto alphaField = std::make_shared<Box>();
            auto alphaCheckbox = std::make_shared<Checkbox>();
            auto alphaLabel = std::make_shared<Text>();

            auto buttons = std::make_shared<Box>();

            auto cancelButton = CreateTextButton("Cancelar");
            auto openButton = CreateTextButton("Exportar");

            nameLabel->Content = "Nome do arquivo: ";

            nameInput->SetValue("imagem.bmp");

            nameField->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
                    .WithGap(8)
            );

            nameField->AddChild(nameLabel);
            nameField->AddChild(nameInput);

            alphaField->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Row)
                    .WithAlignment(BoxAxisAlignment::Start, BoxAxisAlignment::Center)
                    .WithGap(8)
            );

            alphaLabel->Content = "Alfa";

            alphaCheckbox->SetChecked(true);

            alphaField->AddChild(alphaCheckbox);
            alphaField->AddChild(alphaLabel);

            cancelButton->OnMousePress = [this](Element& e) {
                Close();
            };

            openButton->OnMousePress = [this, project, fileSelector, nameInput, alphaCheckbox](Element& e) {
                std::string basePath = fileSelector->GetPath();
                std::string fileName = nameInput->GetValue();

                if (fileName.empty())
                {
                    fileName = "imagem.bmp";
                }

                std::string path = Path::Join({ basePath, fileName });

                const Bitmap& bitmap = *project->RenderCanvas();

                BMP::Save(path, bitmap, alphaCheckbox->IsChecked());

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
            body->AddChild(nameField);
            body->AddChild(alphaField);
            body->AddChild(buttons);

            OnMount = [this, fileSelector](Element& element)
            {
                fileSelector->SetPath("./Trab1JaimeADF/images");
            };

            AddChild(header);
            AddChild(body);
        }
    };
}