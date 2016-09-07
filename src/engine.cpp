#include "engine.h"


void Engine::Initialize ()
{
	clock = new sf::Clock ();
	renderWindow = new sf::RenderWindow (sf::VideoMode(600, 600), "X A R I");
	
	#if ENGINE_LIMIT_FRAMERATE
	renderWindow->setFramerateLimit (ENGINE_LIMIT_FRAMERATE_TARGET);
	#endif
}

/*	Run
	
	Start the game engine
	including it's game loop.
	
	To stop the engine from this point, you will have to either-
	close the window manually or change the boolean value (closing)-
	to true.
*/
void Engine::Run ()
{
	while (IsRunning())
	{
		Engine::UpdateGameTime ();
		
		#if ENGINE_PRINT_FPS
		cout << "FPS: " << Engine::fps << endl;
		#endif
		
		#if ENGINE_PRINT_DELTATIME
		cout << "DeltaTime: " << Engine::deltaTime << endl;
		#endif
		
		renderWindow->clear (sf::Color::Black);
		
		TockEntities ();
		
		RenderActors ();
		
		CleanUpEntities ();
		
		Engine::ParseEvents ();
		
		renderWindow->display ();
		this->totalFrames++;
	}
	
	renderWindow->close ();
}

//Update all entities in the game
void Engine::TockEntities ()
{
	for (int i = 0; i < this->entities.size(); i++)
	{
		Entity* entity = this->entities[i];
		
		entity->Tick ();
	}
}

//Render all Actors in the game to the screen
void Engine::RenderActors ()
{
	for (int i = 0; i < this->actors.size(); i++)
	{
		Actor* actor = this->actors[i];
		this->renderWindow->draw (*actor);
	}
}

template <typename T>
bool DoesVectorContainElement (T* element, std::vector<T*> vec)
{
	for (int i = 0; i < vec.size (); i++)
	{
		T* _element = vec[i];
		if (element == _element)
		{
			return true;
		}
	}
	return false;
}


void Engine::CleanUpEntities ()
{
	if (removeQueue.size() > 0)
	{
		#if ENGINE_SHOW_CLEANUP_INFO
		DEB("removeQueue is being cleaned...");
		#endif
		
		vector<Entity*> newEntities;
		
		for (int i = 0; i < entities.size (); i++)
		{
			Entity* entity = entities[i];
			
			if (!DoesVectorContainElement (entity, removeQueue))
			{
				newEntities.push_back (entity);
			}
		}
		
		int removedEntities = entities.size() - newEntities.size ();
		
		#if ENGINE_SHOW_CLEANUP_INFO
		cout << BOLD << YELLOW <<"Removed " << removedEntities << " entities" << RESET << endl;
		#endif
		
		entities = newEntities;
		
		vector<Actor*> newActors;
		
		for (int i = 0; i < actors.size (); i++)
		{
			Entity* entity = actors[i];
			
			if (!DoesVectorContainElement (entity, removeQueue))
			{
				newActors.push_back (actors[i]);
			}
		}
		
		int removedActors = actors.size() - actors.size ();
		
		#if ENGINE_SHOW_CLEANUP_INFO
		cout << BOLD << YELLOW <<"Removed " << removedActors << " actors" << RESET << endl;
		#endif
		
		actors = newActors;
		
		removeQueue.clear ();
	}
}


void Engine::ParseEvents ()
{
	sf::Event event;
	
	while (this->renderWindow->pollEvent (event))
	{
		if (event.type == sf::Event::Closed)
		{
			this->closing = true;
		}
	}
}

/*	UpdateGameTime
	
	updates the deltaTime and fps values
	and also resets the time for it's next use
*/
void Engine::UpdateGameTime ()
{
	sf::Time time = this->clock->getElapsedTime ();
	this->deltaTime = time.asSeconds ();
	this->fps = 1.0f / time.asSeconds();
	this->clock->restart().asSeconds ();
}

/*---- Main Engine Interaction Methods ----*/
void Engine::Add (Entity* entity)
{
	entity->engine = this;
	this->entities.push_back (entity);
	entity->Birth ();
}

void Engine::Add (Actor* actor)
{
	this->actors.push_back (actor);
	Entity* entity = actor;
	// Adding the entity will call the birth initializer
	this->Add(entity);
}


void Engine::Remove (Entity* entity)
{
	removeQueue.push_back (entity);
}


bool Engine::IsRunning ()
{
	return renderWindow->isOpen() && !closing;
}


void Engine::Shutdown ()
{
	this->closing = true;
}
