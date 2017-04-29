/*
  This file is part of Resources Simulator System.
  Copyright (C) 2014 by Alejandro J. Mujica

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.

  Any user request of this software, write to 

  Alejandro Mujica

  aledrums@gmail.com
*/

# include <stdexcept>

# include <event.H>
# include <event_factory.H>

void Event_Factory::empty_stack(DynListStack<Event *> & stack)
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
