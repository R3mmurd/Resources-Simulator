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

# ifndef EVENT_FACTORY_H
# define EVENT_FACTORY_H

# include <singleton.H>
# include <stack.H>

class Event;

/** Fábrica de eventos implementada como un singleton.
 *
 *  Está implementada como fábrica y almacén, de modo que los punteros que no
 *  se estén utilizando se almacenen aquí para ser reutilizados cuando se pida
 *  nueva memoria, de esa manera se reducen los llamados a new y delete.
 *
 *  Valiéndonos de que Event se implementa como un nodo de lista simplemente
 *  enlazada, se coloca una cabecera por cada tipo de nodo para enlazarlos y
 *  mantenerlos allí para ser reutilizados o para ser liberados en la
 *  destrucción del objeto fábrica.
 */
class Event_Factory : public Designar::Singleton<Event_Factory>
{
  friend class Designar::Singleton<Event_Factory>;
  
  /// Almacen para eventos de llegada externa.
  Designar::ListStack<Event *> external_arrival_events;

  /// Almacen para eventos de llegada interna.
  Designar::ListStack<Event *> internal_arrival_events;

  /// Almacen para eventos de salida.
  Designar::ListStack<Event *> walkout_events;

  static void empty_stack(Designar::ListStack<Event *> &);

public:
  /// El destructor libera la memoria de los eventos almacenados.
  ~Event_Factory();

  Event * get_external_arrival_event();

  Event * get_internal_arrival_event();

  Event * get_walkout_event();

  void store_external_arrival_event(Event *);

  void store_internal_arrival_event(Event *);

  void store_walkout_event(Event *);
};

# endif // EVENT_FACTORY_H

