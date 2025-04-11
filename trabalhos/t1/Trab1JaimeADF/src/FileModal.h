#pragma once

#include "Modal.h"
#include "Project.h"

#include "FileSelector.h"

namespace yap
{
    class FileModal : public Modal
    {
    public:
        FileModal(const std::shared_ptr<Project>& project)
        {
            auto header = CreateHeader("Abrir Arquivo");
            auto body = CreateBody();

            auto fileSelector = std::make_shared<FileSelector>();

            auto buttons = std::make_shared<Box>();

            auto cancelButton = CreateTextButton("Cancelar");
            auto openButton = CreateTextButton("Abrir");

            auto error = std::make_shared<Box>();
            auto errorText = std::make_shared<Text>();

            cancelButton->OnMousePress = [this](Element& e) {
                Close();
            };

            openButton->OnMousePress = [this, project, fileSelector, error, errorText](Element& e) {
                error->SetStyle(
                    error->GetStyle()
                        .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fit())
                );

                std::string path = fileSelector->GetSelectedPath();
                std::string extension = Path::Extension(path);

                try {
                    if (extension == "bmp")
                    {
                        project->CreateLayer(BMP::Load(path));
                    }
                    else if (extension == "yap")
                    {
                        project->Load(path);
                    }
                    else
                    {
                        error->SetStyle(
                            error->GetStyle()
                                .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fit())
                        );
                        errorText->Content = "Selecione um arquivo .bmp ou .yap para abrir.";
                        return;
                    }
                } catch (const std::exception& e) {
                    error->SetStyle(
                        error->GetStyle()
                            .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fit())
                    );
                    errorText->Content = "Erro ao abrir o arquivo: " + std::string(e.what());
                    return;
                }

                Close();
            };

            error->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fit(), AxisSizingRule::Fixed(0))
                    .WithPadding(BoxPadding(8))
                    .WithForeground(ColorRGB(255, 0, 0))
            );

            error->AddChild(errorText);

            buttons->SetStyle(
                StyleSheet()
                    .WithSize(AxisSizingRule::Fill(), AxisSizingRule::Fit())
                    .WithAlignment(BoxAxisAlignment::Center, BoxAxisAlignment::Center)
                    .WithGap(8)
            );

            buttons->AddChild(cancelButton);
            buttons->AddChild(openButton);

            body->AddChild(fileSelector);
            body->AddChild(error);
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