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

# ifndef EVENT_H
# define EVENT_H

# include <random>

# include <node.H>

# include <heap.H>

using rng_t = std::mt19937_64;

using expo_dist_t = std::exponential_distribution<double>;

class Event;

struct EventCmp
{
  bool operator () (Event *, Event *) const;
};

using Event_Queue = Designar::LHeap<Event *, EventCmp>;

/** Define un evento genérico para simular. Está implementado como un nodo de
 *  lista simplemente enlazada no circular.
 */
class Event
{
  /// Tiempo en el cual ocurrirá el evento.
  double time;

  /// Puntero al nodo sobre el cual ocurrirá el evento.
  Node * ptr_node;
public:
  Event();

  virtual ~Event();

  const double & get_time() const;

  void set_time(const double &);

  Node * get_ptr_node();

  void set_ptr_node(Node *);

  virtual void perform(const double &, Event_Queue *, rng_t &);
};

/// Representa un evento de llegada (Externa o Interna).
class Arrival_Event : public Event
{
public:
  /** Sobrecarga del comportamiento del evento en la simulación. Invoca el
   *  método perform de la clase padre y luego ejecuta las acciones generales
   *  de cualquier evento de llegada.
   */
  virtual void perform(const double &, Event_Queue *, rng_t &);
};

/// Representa un evento de llegada externa.
class External_Arrival_Event : public Arrival_Event
{
public:
  /** Sobrecarga del comportamiento del evento en la simulación. Invoca el
   *  método perform de la clase padre y luego ejecuta las acciones propias
   *  de un evento de llegada externa.
   */
  void perform(const double &, Event_Queue *, rng_t &) override;
};

/// Representa un evento de llegada interna.
class Internal_Arrival_Event : public Arrival_Event
{
public:

  /** Sobrecarga del comportamiento del evento en la simulación. Invoca el
   *  método perform de la clase padre y luego ejecuta las acciones propias
   *  de un evento de llegada interna.
   */
  virtual void perform(const double &, Event_Queue *, rng_t &) override;
};

/// Representa un evento de salida.
class Walkout_Event : public Event
{
public:
  /** Sobrecarga del comportamiento del evento en la simulación. Invoca el
   *  método perform de la clase padre y luego ejecuta las acciones propias
   *  de un evento de llegada interna.
   */
  virtual void perform(const double &, Event_Queue *, rng_t &) override;
};

# endif // EVENT_H

