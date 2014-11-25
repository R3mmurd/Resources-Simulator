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

# include <slink.H>

# include <node.H>

using rng_t = std::mt19937;

using expo_dist_t = std::exponential_distribution<double>;

class Event_Queue;

/** Define un evento genérico para simular. Deriva de NCSlink para ser
 *  representado como un nodo de lista simplemente enlazada no circular.
 */
class Event : public NCSlink
{
public:
  /** Tipo enumerado para usar un constructor que inicializa un evento especial
   *  con tiempo tendiendo a infinito.
   */ 
  enum Ctor
  {
    Sentinel_Ctor
  };

  /// Definición de tipos de evento.
  enum Type
  {
    Internal_Arrival, // Llegada externa
    External_Arrival, // Llegada interna
    Walkout,          // Salida
    Num_Types
  };

private:
  /// Tiempo en el cual ocurrirá el evento.
  double time;

  /// Puntero al nodo sobre el cual ocurrirá el evento.
  Node * ptr_node;

public:
  /// Construye un evento normal
  Event();

  /// Construye un evento especial con tiempo tendiendo a infinito.
  Event(Ctor);

  virtual ~Event();

  const double & get_time() const;

  void set_time(const double &);

  Node * get_ptr_node();

  void set_ptr_node(Node *);

  Event * front();

  Event * pop();

  virtual void perform(const double &, Event_Queue *, rng_t &);

  virtual Type get_type() const;
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
  void perform(const double &, Event_Queue *, rng_t &);

  /// Retorna el tipo External_Arrival.
  Type get_type() const override;
};

/// Representa un evento de llegada interna.
class Internal_Arrival_Event : public Arrival_Event
{
public:

  /** Sobrecarga del comportamiento del evento en la simulación. Invoca el
   *  método perform de la clase padre y luego ejecuta las acciones propias
   *  de un evento de llegada interna.
   */
  virtual void perform(const double &, Event_Queue *, rng_t &);

  /// Retorna el tipo Internal_Arrival.
  Type get_type() const override;
};

/// Representa un evento de salida.
class Walkout_Event : public Event
{
public:
  /** Sobrecarga del comportamiento del evento en la simulación. Invoca el
   *  método perform de la clase padre y luego ejecuta las acciones propias
   *  de un evento de llegada interna.
   */
  virtual void perform(const double &, Event_Queue *, rng_t &);

  /// Retorna el tipo Walkout.
  Type get_type() const override;
};

# endif // EVENT_H

