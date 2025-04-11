#pragma once

// Summary:
// This file defines the `ShareModal` class, which represents a modal dialog for exporting a project.
// The modal allows the user to select a file path, specify a file name, and choose whether to include alpha transparency.
// The exported file is saved in BMP format.

#include "Project.h"
#include "BMP.h"

#include "Modal.h"
#include "Checkbox.h"
#include "FileSelector.h"

namespace yap
{
    /**
     * @class ShareModal
     * @brief A modal dialog for exporting a project to a BMP file.
     *
     * The `ShareModal` class provides a user interface for selecting a file path, specifying a file name,
     * and choosing whether to include alpha transparency in the exported BMP file. It uses various UI components
     * such as text inputs, checkboxes, and buttons to facilitate the export process.
     */
    class ShareModal : public Modal
    {
    public:
        /**
         * @brief Constructs a `ShareModal` instance.
         * 
         * @param project A shared pointer to the `Project` object representing the current project.
         * 
         * This constructor initializes the modal dialog with UI components, including:
         * - A file selector for choosing the export directory.
         * - A text input for specifying the file name.
         * - A checkbox for enabling or disabling alpha transparency.
         * - Buttons for canceling or confirming the export.
         */
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

            /**
             * @brief Closes the modal when the cancel button is pressed.
             */
            cancelButton->OnMousePress = [this](Element& e) {
                Close();
            };

            /**
             * @brief Exports the project to a BMP file when the export button is pressed.
             * 
             * The method retrieves the file path and name from the UI components, renders the project's canvas,
             * and saves it as a BMP file. The alpha transparency option is determined by the checkbox state.
             */
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

            /**
             * @brief Sets the default path for the file selector when the modal is mounted.
             * 
             * The default path is set to the `images` directory within the project folder.
             */
            OnMount = [this, fileSelector](Element& element)
            {
                fileSelector->SetPath("./Trab1JaimeADF/images");
            };

            AddChild(header);
            AddChild(body);
        }
    };
}