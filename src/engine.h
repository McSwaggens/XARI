#ifndef __ENGINE_H_
#define __ENGINE_H_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "colors.h"

using namespace std;

#define ENGINE_SHOW_DEBUG_INFO false
#define ENGINE_SHOW_CLEANUP_INFO false
#define ENGINE_PRINT_FPS false
#define ENGINE_PRINT_DELTATIME false
#define ENGINE_LIMIT_FRAMERATE false
#define ENGINE_LIMIT_FRAMERATE_TARGET 60
#define DEB(x) cout << BOLD << YELLOW << x << RESET << endl;


class Entity;
class Actor;

#define KeyPressed(key) Keyboard::isKeyPressed(key)

typedef sf::Keyboard	Keyboard;
typedef sf::Text		Text;
typedef sf::Font		Font;
typedef sf::Texture		Texture;
typedef sf::Shader		shader;
typedef sf::Mouse		Mouse;
typedef sf::Joystick	Joystick;
typedef sf::Vector2f	Vector2;
typedef sf::Vector3f	Vector3;
typedef sf::Color		Color;
typedef sf::Vertex		Vertex;
typedef sf::VertexArray	VertexArray;

class Engine
{
private:
	/*---- SFML ----*/
	sf::RenderWindow* renderWindow;
	
	/*---- Engine Logic ----*/
	float deltaTime;
	float fps;
	unsigned long int totalFrames;
	sf::Clock* clock;
	bool closing;
	vector<Entity*> entities;
	vector<Actor*> actors;
	vector<Entity*> removeQueue;
	
	void TockEntities ();
	void RenderActors ();
	
	void CleanUpEntities ();
	
public:
	
	void Initialize ();
	void Run ();
	
	void UpdateGameTime ();
	void ParseEvents ();
	
	void Add (Entity* entity);
	void Add (Actor* actor);
	
	void Remove (Entity* entity);
	
	bool IsRunning ();
	
	void Shutdown ();
	
	/*---- CONSTANTS ----*/
	float DeltaTime ()
	const
	{
		return this->deltaTime;
	}
	
	float GetFPS ()
	const
	{
		return this->fps;
	}
	
	unsigned long int GetTotalFrames ()
	const
	{
		return this->totalFrames;
	}
	
	unsigned long int GetAliveEntities ()
	const
	{
		return entities.size ();
	}
	
	unsigned long int GetAliveActors ()
	const
	{
		return actors.size ();
	}
	
	Vector2 MapPixelToCoords (sf::Vector2i pixelPos)
	{
		return renderWindow->mapPixelToCoords (pixelPos);
	}
	
	sf::Vector2i GetMousePosition ()
	{
		return Mouse::getPosition(*this->renderWindow);
	}
};



class Entity
{
public:
	Engine* engine;
	virtual void Birth () {}
	virtual void Tick () {}
};

class Actor : public Entity, public sf::Drawable, public sf::Transformable
{
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const { }
};


#endif
