#include "runtime/function/render/render_swap_context.h"

#include <utility>

namespace Pilot
{
    void GameObjectResourceDesc::add(GameObjectDesc desc) { game_object_descs.push_back(desc); }

    bool GameObjectResourceDesc::isEmpty() const { return game_object_descs.empty(); }

    GameObjectDesc GameObjectResourceDesc::getNextProcessObject()
    {
        if (!game_object_descs.empty())
        {
            return game_object_descs.front();
        }
        else
        {
            return GameObjectDesc();
        }
    }

    void GameObjectResourceDesc::popProcessObject() { game_object_descs.pop_front(); }

    RenderSwapData& RenderSwapContext::getLogicSwapData() { return m_swap_data[m_logic_swap_data_index]; }

    RenderSwapData& RenderSwapContext::getRenderSwapData() { return m_swap_data[m_render_swap_data_index]; }

    void RenderSwapContext::swapLogicRenderData()
    {
        if (isReadyToSwap())
        {
            swap();
        }
    }

    bool RenderSwapContext::isReadyToSwap() const
    {
        return !(m_swap_data[m_render_swap_data_index].level_resource_desc.has_value() ||
                 m_swap_data[m_render_swap_data_index].game_object_resource_desc.has_value() ||
                 m_swap_data[m_render_swap_data_index].game_object_to_delete.has_value() ||
                 m_swap_data[m_render_swap_data_index].camera_swap_data.has_value());
    }

    void RenderSwapContext::resetLevelRsourceSwapData()
    {
        m_swap_data[m_render_swap_data_index].level_resource_desc.reset();
    }

    void RenderSwapContext::resetGameObjectResourceSwapData()
    {
        m_swap_data[m_render_swap_data_index].game_object_resource_desc.reset();
    }

    void RenderSwapContext::resetGameObjectToDelete()
    {
        m_swap_data[m_render_swap_data_index].game_object_to_delete.reset();
    }

    void RenderSwapContext::resetCameraSwapData() { m_swap_data[m_render_swap_data_index].camera_swap_data.reset(); }

    void RenderSwapContext::swap()
    {
        resetLevelRsourceSwapData();
        resetGameObjectResourceSwapData();
        resetGameObjectToDelete();
        resetCameraSwapData();
        std::swap(m_logic_swap_data_index, m_render_swap_data_index);
    }

    void RenderSwapData::addDirtyGameObject(GameObjectDesc desc)
    {
        if (game_object_resource_desc.has_value())
        {
            game_object_resource_desc->add(desc);
        }
        else
        {
            GameObjectResourceDesc go_descs;
            go_descs.add(desc);
            game_object_resource_desc = go_descs;
        }
    }

    void RenderSwapData::addDeleteGameObject(GameObjectDesc desc)
    {
        if (game_object_to_delete.has_value())
        {
            game_object_to_delete->add(desc);
        }
        else
        {
            GameObjectResourceDesc go_descs;
            go_descs.add(desc);
            game_object_to_delete = go_descs;
        }
    }
} // namespace Pilot
