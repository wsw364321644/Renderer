#include <World.h>



World::World()
{	
}

World::~World()
{
}

FActor* World::CreateActor()
{
	auto actor = new FActor();
	Actors.emplace(actor->GetID(), actor);
	actor->OnAddComponent.Bind(&World::OnAddComponent, this);
	actor->OnRemoveComponent.Bind(&World::OnRemoveComponent, this);
	return Actors[actor->GetID()].get();
}

std::vector<FActor*> World::GetActors()
{
	std::vector<FActor*> outvector;
	for (auto& [key, value] : Actors) {
		outvector.push_back(value.get());
	}
	return outvector;
}

void World::ItrActors(std::function<void(FActor*)> f)
{
	for (auto& [key, value] : Actors) {
		f(value.get());
	}
}

void World::OnAddComponent(FActor* InActor, ActorComponent* Component)
{
	auto PC = dynamic_cast<PrimitiveComponent*>(Component);
	if (PC) {
		CachedPrimitiveComponents.emplace_back(PC);
	}
}

void World::OnRemoveComponent(FActor* InActor, ActorComponent* Component)
{
	auto PC = dynamic_cast<PrimitiveComponent*>(Component);
	if (PC) {
		CachedPrimitiveComponents.remove(PC);
	}
}

