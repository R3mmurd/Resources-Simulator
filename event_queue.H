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

# ifndef EVENT_QUEUE_H
# define EVENT_QUEUE_H

# include <event.H>

/** Mantiene los eventos pendientes para ejecutar en un simulador.
 *
 *  Los eventos se mantienen ordenados por tiempo de menor a mayor y siempre
 *  se saca el primer evento en cola para garantizar la causalidad de la
 *  simulación.
 */
class Event_Queue
{
  /** Evento especial que tiene tiempo tendiendo a infinito para marcar el final
   *  de la cola y solamente validar tiempo y no tener que hacer validaciones de
   *  puntero nulo al final de la cola.
   */
  static Event sentinel;

  /// Puntero al evento especial. El último evento de la cola apunta a éste.
  static Event * ptr_sentinel;

  /// Cabecera de la cola.
  Event head;

public:
  Event_Queue();

  /// Constructor copia deshabilitado.
  Event_Queue(const Event_Queue &) = delete;

  ~Event_Queue();

  /** Inserta ordenadamente (por tiempo) un evento en la cola.
   *
   *  @param ptr_event Puntero al evento que se insertará,
   */
  void push(Event * ptr_event);

  /// Consulta el primer elemento en cola.
  Event * top();

  /** Elimina el primer elemento en cola.
   *
   *  @return El evento eliminado.
   */
  Event * pop();

  /// Verifica si la cola está vacía.
  bool empty();

  /// Deja la cola vacía.
  void clear();

  /// Operador de asignación deshabilitado.
  Event_Queue & operator = (const Event_Queue &) = delete;
};

# endif // EVENT_QUEUE_H

