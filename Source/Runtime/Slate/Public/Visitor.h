#pragma once
 

/**
 *  @file Visitor.h
 *  @date November 2, 2020
 *  @author Jeremiah van Oosten
 *
 *  @brief Implements the visitor pattern for a scene.
 */

namespace dx12lib
{
class Scene;
class SceneNode;
class Mesh;

class Visitor
{
public:
    Visitor()          = default;
    virtual ~Visitor() = default;

    virtual void Visit( Scene& scene ) = 0;
    virtual void Visit( SceneNode& sceneNode ) = 0;
    virtual void Visit( Mesh& mesh )           = 0;
};

}  // namespace dx12lib