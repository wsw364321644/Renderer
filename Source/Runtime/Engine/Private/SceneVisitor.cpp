#include <SceneVisitor.h>

#include <CommandList.h>
#include <IndexBuffer.h>
#include <Mesh.h>

using namespace dx12lib;

SceneVisitor::SceneVisitor( CommandList& commandList )
: m_CommandList( commandList )
{}

void SceneVisitor::Visit( Mesh& mesh )
{
    mesh.Draw( m_CommandList );
}