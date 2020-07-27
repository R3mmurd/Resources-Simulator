/*
  Resources Simulator System.

  Author: Alejandro Mujica (aledrums@gmail.com)
*/

# include <stdexcept>

# include <event.H>
# include <event_factory.H>

void Event_Factory::empty_stack(Designar::ListStack<Event *> & stack)
{
  while (not stack.is_empty())
    delete stack.pop();
}

Event_Factory::~Event_Factory()
{
  empty_stack(external_arrival_events);
  empty_stack(internal_arrival_events);
  empty_stack(walkout_events);
}

Event * Event_Factory::get_external_arrival_event()
{
  if (external_arrival_events.is_empty())
    return new External_Arrival_Event;
  
  return external_arrival_events.pop();
}

Event * Event_Factory::get_internal_arrival_event()
{
  if (internal_arrival_events.is_empty())
    return new Internal_Arrival_Event;
  
  return internal_arrival_events.pop();
}

Event * Event_Factory::get_walkout_event()
{
  if (walkout_events.is_empty())
    return new Walkout_Event;
  
  return walkout_events.pop();
}

void Event_Factory::store_external_arrival_event(Event * event)
{
  external_arrival_events.push(event);
}

void Event_Factory::store_internal_arrival_event(Event * event)
{
  internal_arrival_events.push(event);
}

void Event_Factory::store_walkout_event(Event * event)
{
  walkout_events.push(event);
}
