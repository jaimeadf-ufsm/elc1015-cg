#pragma once

#include "Project.h"
#include "BMP.h"

#include "Modal.h"
#include "FileSelector.h"

/**
 * @file SaveModal.h
 * @brief Defines the SaveModal class, which provides a modal interface for saving a project.
 */

namespace yap
{
    /**
     * @class SaveModal
     * @brief A modal dialog for saving a project.
     * 
     * The SaveModal class provides a user interface for saving a project. It includes a file
     * selector for choosing the save location, a text input for specifying the file name, and
     * buttons for confirming or canceling the save operation.
     */
    class SaveModal : public Modal
    {
    public:
        SaveModal(const std::shared_ptr<Project>& project)
        {
            auto header = CreateHeader("Salvar Projeto");
            auto body = CreateBody();

            auto fileSelector = std::make_shared<FileSelector>();

            auto field = std::make_shared<Box>();

            auto label = std::make_shared<Text>();
            auto nameInput = std::make_shared<TextInput>();

            auto buttons = std::make_shared<Box>();

            auto cancelButton = CreateTextButton("Cancelar");
            auto openButton = CreateTextButton("Salvar");

            label->Content = "Nome do arquivo: ";

            nameInput->SetValue("projeto.yap");

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
                    fileName = "projeto.yap";
                }

                std::string path = Path::Join({ basePath, fileName });

                project->Save(path);

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
                fileSelector->SetPath("./Trab1JaimeADF/images");
            };

            AddChild(header);
            AddChild(body);
        }
    };
}