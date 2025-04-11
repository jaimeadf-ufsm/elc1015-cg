/**
 * @file Project.h
 * @brief Defines the `Project` class, which manages a collection of layers and a canvas bitmap for rendering.
 * 
 * The `Project` class provides functionality for creating, deleting, and manipulating layers, as well as rendering
 * the final composite image on a canvas. It also supports saving and loading project data to and from files.
 */

#pragma once

#include "Layer.h"

namespace yap
{
    /**
     * @class Project
     * @brief Manages a collection of layers and a canvas bitmap for rendering.
     * 
     * The `Project` class provides methods for creating, deleting, and manipulating layers, rendering the canvas,
     * and saving/loading project data. It also supports event callbacks for layer operations.
     */
    class Project
    {
    private:
        int m_NextLayerId = 0; ///< ID to assign to the next created layer.
        std::shared_ptr<Layer> m_ActiveLayer; ///< The currently active layer.
        std::vector<std::shared_ptr<Layer>> m_Layers; ///< Collection of all layers in the project.
        std::shared_ptr<Bitmap> m_CanvasBitmap; ///< The canvas bitmap used for rendering.

    public:
        // Event callbacks
        std::function<void(Project&, std::shared_ptr<Layer>)> OnLayerCreated = nullptr; ///< Callback for layer creation.
        std::function<void(Project&, std::shared_ptr<Layer>)> OnLayerDeleted = nullptr; ///< Callback for layer deletion.
        std::function<void(Project&, std::shared_ptr<Layer>)> OnLayerMoved = nullptr; ///< Callback for layer movement.
        std::function<void(Project&, std::shared_ptr<Layer>)> OnLayerSelected = nullptr; ///< Callback for layer selection.

        /**
         * @brief Constructs a `Project` with a specified canvas size.
         * @param width Width of the canvas.
         * @param height Height of the canvas.
         */
        Project(int width, int height) : m_CanvasBitmap(std::make_shared<Bitmap>(width, height))
        {
        }

        /**
         * @brief Gets the canvas bitmap.
         * @return A shared pointer to the canvas bitmap.
         */
        std::shared_ptr<Bitmap> GetCanvas() const
        {
            return m_CanvasBitmap;
        }

        /**
         * @brief Renders the canvas by compositing all visible layers.
         * @return A shared pointer to the rendered canvas bitmap.
         */
        std::shared_ptr<const Bitmap> RenderCanvas()
        {
            for (int x = 0; x < m_CanvasBitmap->GetWidth(); ++x)
            {
                for (int y = 0; y < m_CanvasBitmap->GetHeight(); ++y)
                {
                    ColorRGBA canvasColor = ColorRGBA(0.0f, 0.0f, 0.0f, 0.0f);

                    for (const auto& layer : m_Layers)
                    {
                        if (layer->IsVisible())
                        {
                            canvasColor = layer->GetPixel(x, y).CompositeOver(canvasColor);
                        }
                    }

                    m_CanvasBitmap->SetPixel(x, y, canvasColor);
                }
            }

            return m_CanvasBitmap;
        }

        /**
         * @brief Sets the active layer.
         * @param layer The layer to set as active.
         */
        void SetActiveLayer(std::shared_ptr<Layer> layer)
        {
            m_ActiveLayer = layer;

            if (OnLayerSelected)
            {
                OnLayerSelected(*this, layer);
            }
        }

        /**
         * @brief Gets the currently active layer.
         * @return A shared pointer to the active layer.
         */
        std::shared_ptr<Layer> GetActiveLayer() const
        {
            return m_ActiveLayer;
        }

        /**
         * @brief Deletes the currently active layer.
         */
        void DeleteActiveLayer()
        {
            if (m_ActiveLayer)
            {
                DeleteLayer(m_ActiveLayer);
            }
        }

        /**
         * @brief Moves the active layer up in the layer stack.
         */
        void MoveActiveLayerUp()
        {
            MoveLayerUp(m_ActiveLayer);
        }

        /**
         * @brief Moves the active layer down in the layer stack.
         */
        void MoveActiveLayerDown()
        {
            MoveLayerDown(m_ActiveLayer);
        }

        /**
         * @brief Creates a new layer with the same size as the canvas.
         * @return A shared pointer to the newly created layer.
         */
        std::shared_ptr<Layer> CreateLayer()
        {
            return CreateLayer(Bitmap(m_CanvasBitmap->GetWidth(), m_CanvasBitmap->GetHeight()));
        }

        /**
         * @brief Creates a new layer with a specified bitmap.
         * @param bitmap The bitmap to use for the new layer.
         * @return A shared pointer to the newly created layer.
         */
        std::shared_ptr<Layer> CreateLayer(const Bitmap& bitmap)
        {
            auto layer = std::make_shared<Layer>(++m_NextLayerId, bitmap);

            RegisterLayer(layer);
            SetActiveLayer(layer);

            return layer;
        }

        /**
         * @brief Deletes a specified layer.
         * @param layer The layer to delete.
         */
        void DeleteLayer(std::shared_ptr<Layer> layer)
        {
            auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

            if (it != m_Layers.end())
            {
                if (it + 1 != m_Layers.end())
                {
                    SetActiveLayer(*(it + 1));
                }
                else if (it != m_Layers.begin())
                {
                    SetActiveLayer(*(it - 1));
                }
                else
                {
                    SetActiveLayer(nullptr);
                }

                m_Layers.erase(it);

                if (OnLayerDeleted)
                {
                    OnLayerDeleted(*this, layer);
                }
            }
        }

        /**
         * @brief Moves a specified layer up in the layer stack.
         * @param layer The layer to move up.
         */
        void MoveLayerUp(std::shared_ptr<Layer> layer)
        {
            auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

            if (it != m_Layers.end() && it + 1 != m_Layers.end())
            {
                std::iter_swap(it, it + 1);

                if (OnLayerMoved)
                {
                    OnLayerMoved(*this, layer);
                }
            }
        }

        /**
         * @brief Moves a specified layer down in the layer stack.
         * @param layer The layer to move down.
         */
        void MoveLayerDown(std::shared_ptr<Layer> layer)
        {
            auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);

            if (it != m_Layers.begin())
            {
                std::iter_swap(it, it - 1);

                if (OnLayerMoved)
                {
                    OnLayerMoved(*this, layer);
                }
            }
        }

        /**
         * @brief Saves the project to a file.
         * @param path The file path to save the project to.
         * @throws std::runtime_error If the file cannot be opened for writing.
         */
        void Save(const std::string& path)
        {
            std::ofstream file(path, std::ios::binary);

            if (!file)
            {
                throw std::runtime_error("Unable to open file for writing");
            }

            uint32_t type = 0x4410;

            int32_t nextLayerId = m_NextLayerId;
            int32_t activeLayerId = m_ActiveLayer ? m_ActiveLayer->GetId() : -1;
            int32_t canvasWidth = m_CanvasBitmap->GetWidth();
            int32_t canvasHeight = m_CanvasBitmap->GetHeight();
            int32_t layerCount = static_cast<int32_t>(m_Layers.size());

            file.write(reinterpret_cast<const char*>(&type), sizeof(type));
            file.write(reinterpret_cast<const char*>(&nextLayerId), sizeof(nextLayerId));
            file.write(reinterpret_cast<const char*>(&activeLayerId), sizeof(activeLayerId));
            file.write(reinterpret_cast<const char*>(&canvasWidth), sizeof(canvasWidth));
            file.write(reinterpret_cast<const char*>(&canvasHeight), sizeof(canvasHeight));
            file.write(reinterpret_cast<const char*>(&layerCount), sizeof(layerCount));

            for (const auto& layer : m_Layers)
            {
                int32_t layerId = layer->GetId();

                std::shared_ptr<const Bitmap> layerBitmap = layer->GetBitmap();

                Vec2 layerPosition = layer->GetPosition();
                Vec2 layerSize = layer->GetSize();

                bool layerVisibility = layer->IsVisible();

                file.write(reinterpret_cast<const char*>(&layerId), sizeof(layerId));
                file.write(reinterpret_cast<const char*>(&layerPosition.X), sizeof(layerPosition.X));
                file.write(reinterpret_cast<const char*>(&layerPosition.Y), sizeof(layerPosition.Y));
                file.write(reinterpret_cast<const char*>(&layerSize.X), sizeof(layerSize.X));
                file.write(reinterpret_cast<const char*>(&layerSize.Y), sizeof(layerSize.Y));
                file.write(reinterpret_cast<const char*>(&layerVisibility), sizeof(layerVisibility));

                for (int y = 0; y < layerBitmap->GetHeight(); ++y)
                {
                    for (int x = 0; x < layerBitmap->GetWidth(); ++x)
                    {
                        ColorRGBA pixel = layerBitmap->GetPixel(x, y);

                        file.write(reinterpret_cast<const char*>(&pixel.R), sizeof(pixel.R));
                        file.write(reinterpret_cast<const char*>(&pixel.G), sizeof(pixel.G));
                        file.write(reinterpret_cast<const char*>(&pixel.B), sizeof(pixel.B));
                        file.write(reinterpret_cast<const char*>(&pixel.A), sizeof(pixel.A));
                    }
                }
            }
        }

        /**
         * @brief Loads a project from a file.
         * @param path The file path to load the project from.
         * @throws std::runtime_error If the file cannot be opened or the format is invalid.
         */
        void Load(const std::string& path)
        {
            std::ifstream file(path, std::ios::binary);

            if (!file)
            {
                throw std::runtime_error("Unable to open file for reading");
            }

            uint32_t type = 0;
            int32_t nextLayerId = 0;
            int32_t activeLayerId = 0;
            int32_t canvasWidth = 0;
            int32_t canvasHeight = 0;
            int32_t layerCount = 0;

            file.read(reinterpret_cast<char*>(&type), sizeof(type));

            if (type != 0x4410)
            {
                throw std::runtime_error("Invalid YAP file format");
            }

            file.read(reinterpret_cast<char*>(&nextLayerId), sizeof(nextLayerId));
            file.read(reinterpret_cast<char*>(&activeLayerId), sizeof(activeLayerId));
            file.read(reinterpret_cast<char*>(&canvasWidth), sizeof(canvasWidth));
            file.read(reinterpret_cast<char*>(&canvasHeight), sizeof(canvasHeight));
            file.read(reinterpret_cast<char*>(&layerCount), sizeof(layerCount));

            std::vector<std::shared_ptr<Layer>> layers;
            layers.reserve(layerCount);

            for (int i = 0; i < layerCount; ++i)
            {
                int32_t layerId = 0;
                Vec2 layerPosition;
                Vec2 layerSize;
                bool layerVisibility = false;

                file.read(reinterpret_cast<char*>(&layerId), sizeof(layerId));
                file.read(reinterpret_cast<char*>(&layerPosition.X), sizeof(layerPosition.X));
                file.read(reinterpret_cast<char*>(&layerPosition.Y), sizeof(layerPosition.Y));
                file.read(reinterpret_cast<char*>(&layerSize.X), sizeof(layerSize.X));
                file.read(reinterpret_cast<char*>(&layerSize.Y), sizeof(layerSize.Y));
                file.read(reinterpret_cast<char*>(&layerVisibility), sizeof(layerVisibility));

                Bitmap bitmap(static_cast<int>(layerSize.X), static_cast<int>(layerSize.Y));

                for (int y = 0; y < bitmap.GetHeight(); ++y)
                {
                    for (int x = 0; x < bitmap.GetWidth(); ++x)
                    {
                        ColorRGBA pixel;
                        file.read(reinterpret_cast<char*>(&pixel.R), sizeof(pixel.R));
                        file.read(reinterpret_cast<char*>(&pixel.G), sizeof(pixel.G));
                        file.read(reinterpret_cast<char*>(&pixel.B), sizeof(pixel.B));
                        file.read(reinterpret_cast<char*>(&pixel.A), sizeof(pixel.A));

                        bitmap.SetPixel(x, y, pixel);
                    }
                }

                auto layer = std::make_shared<Layer>(layerId, bitmap);
                layer->SetPosition(layerPosition);
                layer->SetVisible(layerVisibility);

                layers.push_back(layer);
            }

            while (m_Layers.size() > 0)
            {
                DeleteLayer(m_Layers.back());
            }

            SetSize(canvasWidth, canvasHeight);

            m_NextLayerId = nextLayerId;
            m_ActiveLayer = *std::find_if(layers.begin(), layers.end(), [activeLayerId](const std::shared_ptr<Layer>& layer) {
                return layer->GetId() == activeLayerId;
            });

            for (const auto& layer : layers)
            {
                RegisterLayer(layer);
            }
        }

        /**
         * @brief Gets all layers in the project.
         * @return A vector of shared pointers to the layers.
         */
        const std::vector<std::shared_ptr<Layer>> GetLayers() const
        {
            return m_Layers;
        }

        /**
         * @brief Sets the size of the canvas.
         * @param width The new width of the canvas.
         * @param height The new height of the canvas.
         */
        void SetSize(int width, int height)
        {
            m_CanvasBitmap->Reallocate(width, height);
        }

        /**
         * @brief Gets the width of the canvas.
         * @return The width of the canvas.
         */
        int GetWidth() const
        {
            return m_CanvasBitmap->GetWidth();
        }

        /**
         * @brief Gets the height of the canvas.
         * @return The height of the canvas.
         */
        int GetHeight() const
        {
            return m_CanvasBitmap->GetHeight();
        }
    
    private:
        /**
         * @brief Registers a new layer in the project.
         * @param layer The layer to register.
         */
        void RegisterLayer(std::shared_ptr<Layer> layer)
        {
            m_Layers.push_back(layer);

            if (OnLayerCreated)
            {
                OnLayerCreated(*this, layer);
            }
        }
    };
}