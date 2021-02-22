#pragma once

/**
 *  @file SceneVisitor.h
 *  @date November 6, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief A scene visitor is used to render the meshes in a scene. It uses the Visitor design pattern to iterate the 
 * nodes of a scene.
 */

#include <Visitor.h>

namespace dx12lib
{
class CommandList;
}

class SceneVisitor : public dx12lib::Visitor
{
public:
    /**
     * Constructor for the SceneVisitor.
     * @param commandList The CommandList that is used to render the meshes in the scene.
     */
    SceneVisitor( dx12lib::CommandList& commandList );

    // For this sample, we don't need to do anything when visiting the Scene.
    virtual void Visit( dx12lib::Scene& scene ) override {}
    // For this sample, we don't need to do anything when visiting the SceneNode.
    virtual void Visit( dx12lib::SceneNode& sceneNode ) override {}
    // When visiting a mesh, the mesh must be rendered.
    virtual void Visit( dx12lib::Mesh& mesh ) override;

private:
    dx12lib::CommandList& m_CommandList;
};