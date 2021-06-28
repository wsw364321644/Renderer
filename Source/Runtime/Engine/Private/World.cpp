#include <World.h>



World::World()
{	
}

World::~World()
{
}

Actor* World::CreateActor()
{
	auto actor = new Actor();
	Actors.emplace(actor->GetID(), actor);
	actor->OnAddComponent.Bind(&World::OnAddComponent, this);
	actor->OnRemoveComponent.Bind(&World::OnRemoveComponent, this);
	return Actors[actor->GetID()].get();
}

std::vector<Actor*> World::GetActors()
{
	std::vector<Actor*> outvector;
	for (auto& [key, value] : Actors) {
		outvector.push_back(value.get());
	}
	return outvector;
}

void World::ItrActors(std::function<void(Actor*)> f)
{
	for (auto& [key, value] : Actors) {
		f(value.get());
	}
}

void World::OnAddComponent(Actor* InActor, ActorComponent* Component)
{
	auto PC = dynamic_cast<PrimitiveComponent*>(Component);
	if (PC) {
		CachedPrimitiveComponents.emplace_back(PC);
	}
}

void World::OnRemoveComponent(Actor* InActor, ActorComponent* Component)
{
	auto PC = dynamic_cast<PrimitiveComponent*>(Component);
	if (PC) {
		CachedPrimitiveComponents.remove(PC);
	}
}

