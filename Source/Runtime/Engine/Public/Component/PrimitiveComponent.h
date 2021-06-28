#pragma once
#include <memory>
#include "Scene.h"

#include "Misc/MathHelper.h"
#include "Component/ActorComponent.h"


class PrimitiveComponent :public ActorComponent
{
public:

    PrimitiveComponent();
    virtual ~PrimitiveComponent();

    // Helper function for flipping winding of geometric primitives for LH vs. RH coords
    inline void ReverseWinding(std::vector<uint16_t>& indices,std::vector<VertexAttributes>& vertices);



    // Helper creates a triangle fan to close the end of a cylinder / cone
    void CreateCylinderCap(std::vector<VertexAttributes>& vertices, std::vector<uint16_t>& indices, size_t tessellation, float height,
        float radius, bool isTop);

    virtual std::shared_ptr<dx12lib::Scene> UploadResource(std::shared_ptr<dx12lib::CommandList>) = 0;

    std::shared_ptr<dx12lib::Scene> GetScence() {
        return Scene;
    };
protected:
    std::shared_ptr<dx12lib::Scene> Scene;

};

inline void PrimitiveComponent::ReverseWinding( std::vector<uint16_t>& indices, std::vector<VertexAttributes>& vertices)
{
    assert((indices.size() % 3) == 0);
    for (auto it = indices.begin(); it != indices.end(); it += 3)
    {
        std::swap(*it, *(it + 2));
    }

    for (auto it = vertices.begin(); it != vertices.end(); ++it)
    {
        it->TexCoord.x = (1.f - it->TexCoord.x);
    }
}
