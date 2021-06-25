#pragma once
#include <memory>
#include "Scene.h"
#include "GameFramework/ResourceManager.h"
#include "Component/PrimitiveComponent.h"

//todo static genarete class
class ProcedureMeshComponent :public PrimitiveComponent
{
public:

    ProcedureMeshComponent();
    virtual ~ProcedureMeshComponent();


 /**
 * Create a cube.
 *
 * @param size The size of one side of the cube.
 * @param reverseWinding Whether to reverse the winding order of the triangles (useful for skyboxes).
 */
    void CreateCube(float size = 1.0, bool reverseWinding = false);


    /**
 * Create a sphere.
 *
 * @param radius Radius of the sphere.
 * @param tessellation Determines how smooth the sphere is.
 * @param reverseWinding Whether to reverse the winding order of the triangles (useful for sydomes).
 */
    void CreateSphere(float radius = 0.5f, uint32_t tessellation = 16, bool reversWinding = false);

    /**
     * Create a Cylinder
     *
     * @param radius The radius of the primary axis of the cylinder.
     * @param hight The height of the cylinder.
     * @param tessellation How smooth the cylinder will be.
     * @param reverseWinding Whether to reverse the winding order of the triangles.
     */
    void CreateCylinder(float radius = 0.5f, float height = 1.0f, uint32_t tessellation = 32,
        bool reverseWinding = false);

    /**
     * Create a cone.
     *
     * @param radius The radius of the base of the cone.
     * @param height The height of the cone.
     * @param tessellation How smooth to make the cone.
     * @param reverseWinding Whether to reverse the winding order of the triangles.
     */
    void CreateCone(float radius = 0.5f, float height = 1.0f, uint32_t tessellation = 32,
        bool reverseWinding = false);

    /**
     * Create a torus.
     *
     * @param radius The radius of the torus.
     * @param thickness The The thickness of the torus.
     * @param tessellation The smoothness of the torus.
     * @param reverseWinding Reverse the winding order of the vertices.
     */
    void CreateTorus(float radius = 0.5f, float thickness = 0.333f, uint32_t tessellation = 32,
        bool reverseWinding = false);

    /**
     * Create a plane.
     *
     * @param width The width of the plane.
     * @param height The height of the plane.
     * @reverseWinding Whether to reverse the winding order of the plane.
     */
    void CreatePlane(float width = 1.0f, float height = 1.0f, bool reverseWinding = false);


private:


    std::shared_ptr<MeshInfo> Mesh;
};

