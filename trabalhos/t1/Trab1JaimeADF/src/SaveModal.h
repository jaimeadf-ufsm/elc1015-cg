#pragma once

#include "Project.h"
#include "BMP.h"

#include "Modal.h"
#include "FileSelector.h"

/**
 * @file SaveModal.h
 * @brief Defines the SaveModal class, which provides a modal interface for saving a project.
 * 
 * This file contains the implementation of the SaveModal class, which allows users to select
 * a file path and name to save their project. It includes UI components such as a file selector,
 * text input for the file name, and buttons for saving or canceling the operation.
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
        /**
         * @brief Constructs a SaveModal instance.
         * 
         * Initializes the modal with UI components, including a header, body, file selector,
         * text input for the file name, and buttons for saving or canceling.
         * 
         * @param project A shared pointer to the Project instance to be saved.
         */
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

            // Set label content
            label->Content = "Nome do arquivo: ";

            // Set default file name
            nameInput->SetValue("projeto.yap");

            // Configure the field layout
            field->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithDirection(BoxDirection::Column)
                    .WithGap(8)
            );

            // Add label and input to the field
            field->AddChild(label);
            field->AddChild(nameInput);

            // Configure the cancel button behavior
            cancelButton->OnMousePress = [this](Element& e) {
                Close();
            };

            // Configure the save button behavior
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

            // Configure the buttons layout
            buttons->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithGap(8)
            );

            // Add buttons to the layout
            buttons->AddChild(cancelButton);
            buttons->AddChild(openButton);

            // Add components to the modal body
            body->AddChild(fileSelector);
            body->AddChild(field);
            body->AddChild(buttons);

            // Set the default path for the file selector on mount
            OnMount = [this, fileSelector](Element& element)
            {
                fileSelector->SetPath("./Trab1JaimeADF/images");
            };

            // Add header and body to the modal
            AddChild(header);
            AddChild(body);
        }
    };
}