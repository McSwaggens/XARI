#include "game.h"
#include "scheduler.h"
#include "particle/particle_system.h"
#include "pstring.h"

Engine* engine;


void HelloWorld ()
{
	cout << "Hello World!" << endl;
}

class Mob : public Actor
{
private:
	Scheduler* scheduler;
	Schedule* tocker;
	
public:
	int tock = 0;
	void DoSomethingAnnoying ()
	{
		cout << BOLD << RED << "TOCK " << BLUE << ++this->tock << RESET << endl;
		if (tock == 5)
		{
			this->scheduler->Remove (tocker);
			engine->Remove (this->scheduler);
		}
	}
	
	Mob ()
	{
		scheduler = new Scheduler ();
	}
	
	void Birth ()
	{
		engine->Add(scheduler);
		tocker = this->scheduler->Schedule (FUNCTION(&Mob::DoSomethingAnnoying, this), 5.0f, 1.0f);
	}
	
	void Tick ()
	{
		cout << BOLD << "Alive entities->" << engine->GetAliveEntities () << RESET << endl;
	}
	
	void draw(sf::RenderTarget& target, sf::RenderStates states)
	{
	}
};


class ParticleMouse : public Entity
{
public:
	ParticleSystem* particleSystem;
	
	void Birth ()
	{
		particleSystem = new ParticleSystem (10000, 3.0f);
		
		sf::Vector2i mousePosition = engine->GetMousePosition();
		particleSystem->setPosition (engine->MapPixelToCoords(mousePosition));
		
		engine->Add (particleSystem);
	}
	
	void Tick ()
	{
		sf::Vector2i mousePosition = engine->GetMousePosition();
		particleSystem->setPosition (engine->MapPixelToCoords(mousePosition));
	}
};

class FPS_Printer : public Actor
{
private:
	Font* font;
	Text text;
	float speed;
public:
	
	FPS_Printer (Font* font)
	{
		this->font = font;
		text.setFont (*font);
		text.setColor (Color::White);
		text.setCharacterSize (20.0f);
		speed = 100.0f;
	}
	
	
	void Tick ()
	{
		text.setPosition (this->getPosition());
		
		pstring out ("FPS: ");
		out + engine->GetFPS ();
		text.setString (out);
	}
	
	void draw(sf::RenderTarget& target, sf::RenderStates states)
	const
	{
		target.draw (text);
	}
};


void PrintFPS ()
{
	cout << "FPS: " << engine->GetFPS () << endl;
}


Font font;

void StartGame ()
{
	engine = new Engine;
	engine->Initialize ();
	
	Scheduler* scheduler = new Scheduler ();
	scheduler->Schedule (&PrintFPS, 1, 1);
	
	engine->Add (scheduler);
	
	ParticleMouse* particleMouse = new ParticleMouse ();
	
	engine->Add (particleMouse);
	
	Font* font = new Font ();
	
	bool success = font->loadFromFile ("/usr/share/fonts/truetype/Hack-Regular.ttf");
	
	FPS_Printer* fps_printer = new FPS_Printer (font);
	
	engine->Add (fps_printer);
	
	engine->Run ();
}