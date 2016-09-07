#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "engine.h"
#include <functional>

#define FUNCTION(fn, args) bind (fn, args)

struct Schedule;
typedef Schedule s_schedule;


class Scheduler : public Entity
{
private:
	sf::Clock* clock;
	vector<s_schedule*> schedules;
	vector<s_schedule*> removeStack;
public:
	Scheduler ();
	~Scheduler ();
	
	s_schedule* Schedule (std::function<void()> function, float delay);
	s_schedule* Schedule (std::function<void()> function, float delay, float repeatDelay);
	
	void ClearSchedules ()
	{
		schedules.clear ();
	}
	
	unsigned long int GetAliveSchedules ()
	const
	{
		return schedules.size ();
	}
	
	void Remove (struct Schedule* schedule);
	
	void Tick ();
};

struct Schedule
{
	bool repeat;
	bool pastDelay;
	float delay;
	float repeatDelay;
	float timeStage;
	
	std::function<void()> function;
};


#endif