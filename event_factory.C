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

std::unique_ptr<Event_Factory> Event_Factory::uptr_instance =
  std::unique_ptr<Event_Factory>(nullptr);

Event_Factory::Event_Factory()
{
  // Empty
}

Event_Factory::~Event_Factory()
{
  EMPTY_STORE(external_arrival_events)
  EMPTY_STORE(internal_arrival_events)
  EMPTY_STORE(walkout_events)
}

Event_Factory & Event_Factory::get_instance()
{
  if (uptr_instance.get() == nullptr)
    uptr_instance = std::unique_ptr<Event_Factory>(new Event_Factory);

  return *uptr_instance;
}

Event * Event_Factory::new_event(const Event::Type & type)
{
  Event * ret_val = nullptr;

  switch (type)
    {
    case Event::External_Arrival:
      if (external_arrival_events.get_next() == nullptr)
        ret_val = new External_Arrival_Event;
      else
        ret_val = external_arrival_events.remove_next();

      break;

    case Event::Internal_Arrival:
      if (internal_arrival_events.get_next() == nullptr)
        ret_val = new Internal_Arrival_Event;
      else
        ret_val = internal_arrival_events.remove_next();

      break;

    case Event::Walkout:
      if (walkout_events.get_next() == nullptr)
        ret_val = new Walkout_Event;
      else
        ret_val = walkout_events.remove_next();

      break;

    default:
      throw std::domain_error("Error in event type");
    }

  return ret_val;
}

void Event_Factory::store_event(Event * ptr_event)
{
  switch (ptr_event->get_type())
    {
    case Event::External_Arrival:
      external_arrival_events.insert_next(ptr_event);
      break;
    case Event::Internal_Arrival:
      internal_arrival_events.insert_next(ptr_event);
      break;
    case Event::Walkout:
      walkout_events.insert_next(ptr_event);
      break;
    }
}

