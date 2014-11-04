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

# include <limits>

# include <event.H>
# include <event_factory.H>
# include <simulator.H>

Event::Event()
  : time(-1.0), ptr_node(nullptr), ptr_next(nullptr)
{
  // Empty
}

Event::Event(Event::Ctor)
  : time(std::numeric_limits<double>::max()), ptr_node(nullptr),
    ptr_next(nullptr)
{
  // Empty
}

Event::~Event()
{
  // Empty
}

const double & Event::get_time() const
{
  return time;
}

void Event::set_time(const double & _time)
{
  time = _time;
}

Node * Event::get_ptr_node()
{
  return ptr_node;
}

void Event::set_ptr_node(Node * _ptr_node)
{
  ptr_node = _ptr_node;
}

void Event::insert_next(Event * ptr_event)
{
  ptr_event->ptr_next = ptr_next;
  ptr_next = ptr_event;
}

Event * Event::get_next()
{
  return ptr_next;
}

Event * Event::remove_next()
{
  Event * ret_val = ptr_next;

  ptr_next = ret_val->ptr_next;

  ret_val->ptr_next = nullptr;

  return ret_val;
}

void Event::perform(const double & current_time, Event_Queue *, gsl_rng *)
{
  Node * ptr_node = get_ptr_node();

  Node::Statistics & statistics = ptr_node->statistics();

  statistics.total_wait_time +=
    ptr_node->get_queue() * (current_time - statistics.prev_event_time);

  statistics.pond_use +=
    ptr_node->get_use() * (current_time - statistics.prev_event_time);
}

Event::Type Event::get_type() const
{
  return Event::Num_Types;
}

void Arrival_Event::perform(const double & current_time,
                            Event_Queue * ptr_queue, gsl_rng * rng)
{
  // Llama al evento perform de la clase Event.
  Event::perform(current_time, ptr_queue, rng);

  Node * ptr_node = get_ptr_node();

  Node::Statistics & statistics = ptr_node->statistics();

  statistics.arrived++;

  if (ptr_node->is_full()) // El nodo está totalmente ocupado, a cola.
    {
      ptr_node->inc_queue();
      statistics.max_queue = std::max<size_t>(statistics.max_queue,
                                              ptr_node->get_queue());
    }
  else // Hay disponibilidad de atención.
    {
      if (ptr_node->get_use() == 0) // El nodo esta sin atender a nadie.
        statistics.empty_time += current_time - statistics.prev_event_time;

        // Pasa a ser atendido de inmediato, genero su salida.
        Event * ptr_walkout_event = NEW_EVENT(Event::Walkout);

        ptr_walkout_event->set_ptr_node(ptr_node);
        ptr_walkout_event->set_time(
          current_time + EXPO(ptr_node->get_service_time())
        );
        ptr_queue->push(ptr_walkout_event);
        ptr_node->inc_use();
    }

  statistics.prev_event_time = current_time;
}

void External_Arrival_Event::perform(const double & current_time,
                                     Event_Queue * ptr_queue, gsl_rng * rng)
{
  // Llama al evento perform de la clase Arrival_Event.
  Arrival_Event::perform(current_time, ptr_queue, rng);

  Node * ptr_node = get_ptr_node();

  /* Como fue una llegada externa se genera la próxima llegada externa sobre
     este mismo evento para reutilizar el espacio de memoria.
  */
  set_time(current_time + EXPO(ptr_node->get_time_between_arrivals()));
  ptr_queue->push(this);

}

Event::Type External_Arrival_Event::get_type() const
{
  return Event::External_Arrival;
}

void Internal_Arrival_Event::perform(const double & current_time,
                                     Event_Queue * ptr_queue, gsl_rng * rng)
{
  // Llama al evento perform de la clase Arrival_Event.
  Arrival_Event::perform(current_time, ptr_queue, rng);

  /* Como fue una llegada interna almaceno el espacio de memoria para
     reutilizarlo luego.
  */
  STORE_EVENT(this);
}

Event::Type Internal_Arrival_Event::get_type() const
{
  return Event::Internal_Arrival;
}

void Walkout_Event::perform(const double & current_time,
                            Event_Queue * ptr_queue, gsl_rng * rng)
{
  // Llama al evento perform de la clase Event.
  Event::perform(current_time, ptr_queue, rng);

  Node * ptr_node = get_ptr_node();

  Node::Statistics & statistics = ptr_node->statistics();

  // Selecciono nodo sucesor aleatorio.
  double p = gsl_rng_uniform(rng);

  Node * ptr_tgt_node = ptr_node->get_target(p);

  if (ptr_tgt_node != nullptr) // Si hubo un sucesor
    {
      // Creo el evento de llegada interna
      Event * ptr_internal_arrival_event = NEW_EVENT(Event::Internal_Arrival);
      ptr_internal_arrival_event->set_time(current_time);
      ptr_internal_arrival_event->set_ptr_node(ptr_tgt_node);

      ptr_queue->push(ptr_internal_arrival_event);
    }

  statistics.served++;

  if (ptr_node->get_queue() > 0)
    {
      /* Si hay elementos en cola decremento y genero salida reutilizando la
         memoria.
      */
      ptr_node->dec_queue();
      set_time(current_time + EXPO(ptr_node->get_service_time()));
      ptr_queue->push(this);
    }
  else // Si no había nadie en cola decremento uso y almaceno la memoria.
    {
      ptr_node->dec_use();
      STORE_EVENT(this);
    }

  statistics.prev_event_time = current_time;
}

Event::Type Walkout_Event::get_type() const
{
  return Event::Walkout;
}

