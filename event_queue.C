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

# include <event_queue.H>
# include <event_factory.H>

Event Event_Queue::sentinel = Event(Event::Sentinel_Ctor);

Event * Event_Queue::ptr_sentinel = &Event_Queue::sentinel;

Event_Queue::Event_Queue()
{
  head.push(ptr_sentinel);
}

Event_Queue::~Event_Queue()
{
  clear();
}

void Event_Queue::push(Event * ptr_event)
{
  Event * aux = &head;

  while (aux->front()->get_time() < ptr_event->get_time())
    aux = aux->front();

  aux->push(ptr_event);
}

Event * Event_Queue::top()
{
  return head.front();
}

Event * Event_Queue::pop()
{
  return head.pop();
}

bool Event_Queue::empty()
{
  return head.front() != ptr_sentinel;
}

void Event_Queue::clear()
{
  while (head.front() != ptr_sentinel)
    STORE_EVENT(head.pop());
}

