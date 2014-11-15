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

class Event;

// Macro para vaciar una pila de eventos.
# define EMPTY_STORE(name)                                                     \
  while (name.front() != nullptr)                                           \
    delete name.pop();

// Macro para pedir la instancia de la clase.
# define EVENT_FACTORY Event_Factory::get_instance()

// Macro para pedir la construcción de un evento.
# define NEW_EVENT(type) EVENT_FACTORY.new_event(type)

// Macro para almacenar de un evento.
# define STORE_EVENT(evt) EVENT_FACTORY.store_event(evt)

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
class Event_Factory_Data
{
  /// Almacen para eventos de llegada externa.
  Event external_arrival_events;

  /// Almacen para eventos de llegada interna.
  Event internal_arrival_events;

  /// Almacen para eventos de salida.
  Event walkout_events;

public:
  /// El destructor libera la memoria de los eventos almacenados.
  ~Event_Factory_Data();

  /** Construye o extrae del almacén un evento del tipo deseado.
   *
   *  @param type Tipo de evento que se quiere construir.
   *  @return Puntero con memoria asignada del tipo de evento deseado.
   *  @throw domain_error si el tipo no corresponde a ninguno de los definidos.
   */
  Event * new_event(const Event::Type &);

  /** Coloca un evento en su almacen correspondiente.
   *
   *  @param ptr_event Puntero al evento que se va a almacenar.
   */
  void store_event(Event * ptr_event);
};

using Event_Factory = Singleton<Event_Factory_Data>;

# endif // EVENT_FACTORY_H

