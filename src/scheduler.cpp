#include "scheduler.h"

Scheduler::Scheduler ()
{
	this->clock = new sf::Clock ();
}

Scheduler::~Scheduler ()
{
	this->engine->Remove (this);
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

void Scheduler::Tick ()
{
	for (int i = 0; i < schedules.size (); i++)
	{
		s_schedule* schedule = schedules[i];
		if (!schedule->pastDelay)
		{
			if (clock->getElapsedTime().asSeconds() >= schedule->delay)
			{
				schedule->pastDelay = true;
				schedule->timeStage = clock->getElapsedTime().asSeconds();
				schedule->function ();
			}
		}
		else if (schedule->repeat)
		{
			if (clock->getElapsedTime().asSeconds() - schedule->timeStage >= schedule->repeatDelay)
			{
				schedule->timeStage = clock->getElapsedTime().asSeconds();
				schedule->function ();
			}
		}
	}
	
	
	// Process removeStack
	/*	Why do it this way?
		We keep the removed Schedules in a vector until the-
		loop has finished.
		Then we go through and construct a new vector without-
		the removed Schedules.
		
		This is to allow scheduled functions-
		to be able to remove it's self-
		and other schedules.
	*/
	
	if (removeStack.size () > 0) // Make sure there is at least one schedule inside of the removeStack
	{
		std::vector<s_schedule*> newSchedules;
		for (int i = 0; i < schedules.size(); i++)
		{
			s_schedule* it_schedule = schedules[i];
			if (!DoesVectorContainElement (it_schedule, removeStack))
			{
				newSchedules.push_back (it_schedule);
			}
		}
		
		// Replace old schedule stack with the new schedule stack
		schedules = newSchedules;
	}
}


/*	Remove
	Push a schedule into the removeStack,
	to be cleaned at a later time.
*/
void Scheduler::Remove (s_schedule* schedule)
{
	removeStack.push_back (schedule);
}

s_schedule* Scheduler::Schedule (std::function<void()> function, float delay)
{
	s_schedule* schedule = new s_schedule;
	schedule->function = function;
	schedule->delay = delay;
	schedule->repeat = false;
	schedule->pastDelay = false;
	schedule->timeStage = 0;
	
	schedules.push_back (schedule);
	
	return schedule;
}

s_schedule* Scheduler::Schedule (std::function<void()> function, float delay, float repeatDelay)
{
	s_schedule* schedule = new s_schedule;
	schedule->function = function;
	schedule->delay = delay;
	schedule->repeat = true;
	schedule->repeatDelay = repeatDelay;
	schedule->timeStage = 0;
	schedule->pastDelay = false;
	
	schedules.push_back (schedule);
	
	return schedule;
}