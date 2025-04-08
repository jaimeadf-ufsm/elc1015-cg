#pragma once

#include "Layer.h"

namespace yap
{
    class Project
    {
    private:
        int m_LayerId = 0;

        std::shared_ptr<Layer> m_ActiveLayer;
        std::vector<std::shared_ptr<Layer>> m_Layers;

        std::vector<bool> m_CanvasDirty;
        std::shared_ptr<Bitmap> m_CanvasBitmap;

    public:
        std::function<void(Project&, std::shared_ptr<Layer>)> OnLayerCreated = nullptr;
        std::function<void(Project&, std::shared_ptr<Layer>)> OnLayerDeleted = nullptr;
        std::function<void(Project&, std::shared_ptr<Layer>)> OnLayerMoved = nullptr;
        std::function<void(Project&, std::shared_ptr<Layer>)> OnLayerSelected = nullptr;
        
        Project(int width, int height) :
            m_CanvasDirty(width * height, false),
            m_CanvasBitmap(std::make_shared<Bitmap>(width, height))
        {
        }

        std::shared_ptr<const Bitmap> Render()
        {
            for (int x = 0; x < m_CanvasBitmap->GetWidth(); ++x)
            {
                for (int y = 0; y < m_CanvasBitmap->GetHeight(); ++y)
                {
                    // int index = y * m_CanvasBitmap->GetWidth() + x;

                    // if (!m_CanvasDirty[index])
                    // {
                    //     continue;
                    // }

                    ColorRGBA canvasColor = ColorRGBA(0.0f, 0.0f, 0.0f, 0.0f);

                    for (const auto& layer : m_Layers)
                    {
                        if (layer->IsVisible())
                        {
                            canvasColor = layer->GetPixel(x, y).CompositeOver(canvasColor);
                        }
                    }

                    m_CanvasBitmap->SetPixel(x, y, canvasColor);

                    // m_CanvasDirty[index] = false;
                }
            }

            return m_CanvasBitmap;
        }

        void SetActiveLayer(std::shared_ptr<Layer> layer)
        {
            m_ActiveLayer = layer;

            if (OnLayerSelected)
            {
                OnLayerSelected(*this, layer);
            }
        }

        std::shared_ptr<Layer> GetActiveLayer() const
        {
            return m_ActiveLayer;
        }

        void DeleteActiveLayer()
        {
            if (m_ActiveLayer)
            {
                DeleteLayer(m_ActiveLayer);
            }
        }

        void MoveActiveLayerUp()
        {
            MoveLayerUp(m_ActiveLayer);
        }

        void MoveActiveLayerDown()
        {
            MoveLayerDown(m_ActiveLayer);
        }

        std::shared_ptr<Layer> CreateLayer()
        {
            return CreateLayer(Bitmap(m_CanvasBitmap->GetWidth(), m_CanvasBitmap->GetHeight()));
        }

        std::shared_ptr<Layer> CreateLayer(const Bitmap& bitmap)
        {
            auto layer = std::make_shared<Layer>(++m_LayerId, bitmap);

            m_Layers.push_back(layer);

            if (OnLayerCreated)
            {
                OnLayerCreated(*this, layer);
            }

            if (m_Layers.size() == 1)
            {
                SetActiveLayer(layer);
            }

            return layer;
        }

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

        const std::vector<std::shared_ptr<Layer>> GetLayers() const
        {
            return m_Layers;
        }

        int GetWidth() const
        {
            return m_CanvasBitmap->GetWidth();
        }

        int GetHeight() const
        {
            return m_CanvasBitmap->GetHeight();
        }
    };
}