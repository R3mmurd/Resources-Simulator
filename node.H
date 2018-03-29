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

# ifndef NODE_H
# define NODE_H

# include <string>

# include <tree.H>

/** Clase que representa un recurso de atención (taquilla).
 *
 *  El recurso es representado como un nodo de grafo dirigido, por esta razón
 *  Se definen una serie de atributos el cual contiene un conjunto de punteros
 *  a nodos (denominados sucesores) que son los aquellos alcanzables por el nodo
 *  actual.
 */
class Node
{
public:
  /// Definición del tipo de nodo
  enum Type
  {
    External, // Nodo externo, admite llegadas externas e internas
    Internal, // Nodo interno, sólo admite llegadas internas
    Num_Types
  };

  /// Representa un arco de grafo dirigido.
  struct Target
  {
    Node * ptr_node;    // Puntero al nodo sucesor.
    double probability; // Probabilidad de llegar al nodo sucesor.
  };

  /** Comparador de arcos.
   *
   *  Los arcos se almacenan en un "set" con la finalidad de mantenerlos
   *  ordenados, la idea es que estén ordenados de menor a mayor probabilidad
   *  para que, al elegir un sucesor dado un número aleatorio uniforme entre
   *  0 y 1, se elija correctamente el sucesor correspondiente.
   *
   *  Este comparador es el que se le asigna al set para que mantenga los arcos
   *  ordenados por probabilidad de ser elegido.
   */
  struct TargetCmp
  {
    bool operator () (const Target & t1, const Target & t2)
    {
      return t1.probability < t2.probability;
    }
  };

  /// Estadísticas para el nodo.
  struct Statistics
  {
    unsigned long arrived;    // Número de clientes que llegaron al nodo.
    unsigned long served;     // Número de clientes que han sido servidos.
    double arrive_time_avg;   // Tiempo promedio entre llegadas.
    double service_time_avg;  // Tiempo promedio de servicio.
    double total_wait_time;   // Tiempo total de espera.
    double prev_event_time;   // Tiempo del evento previo.
    double empty_time;        // Tiempo vacío.
    double pond_use;          // Utilización ponderada.
    unsigned long init_queue; // Longitud de cola máxima.
    unsigned long max_queue;  // Longitud de cola máxima.

    Statistics()
      : arrived(0), served(0), arrive_time_avg(0.0), service_time_avg(0.0),
        total_wait_time(0.0), prev_event_time(0.0), empty_time(0.0),
        pond_use(0.0), init_queue(0), max_queue(0)
    {
      // Empty
    }
  };

private:
  /// Etiqueta, útil para darle nombre del nodo.
  std::string label;

  /// Tipo de nodo.
  Type type;

  /// Tiempo promedio entre llegadas.
  double time_between_arrivals;

  /// Tiempo promedio de servicio.
  double service_time;

  /// Cantidad de personas siendo atendidas.
  unsigned long use;

  /// Capacidad de atención.
  unsigned long capacity;

  /// Cola en la taquilla.
  unsigned long queue;

  /// Conjunto de nodos sucesores con sus respectivas probabilidad.
  Designar::TreeSet<Target, TargetCmp> targets;

  /// Estadísticas.
  Statistics _statistics;

public:
  Node();

  const std::string & get_label() const;

  void set_label(const std::string &);

  const Type & get_type() const;

  void set_type(const Type &);

  const double & get_time_between_arrivals() const;

  void set_time_between_arrivals(const double &);

  const double & get_service_time() const;

  void set_service_time(const double &);

  const unsigned long & get_use() const;

  void set_use(const unsigned long &);

  /// Incrementa en 1 el valor de uso.
  void inc_use();

  /// Decrementa en 1 el valor de uso.
  void dec_use();

  const unsigned long & get_capacity() const;

  void set_capacity(const unsigned long &);

  const unsigned long & get_queue() const;

  void set_queue(const unsigned long &);

  /// Incrementa en 1 el valor de la cola.
  void inc_queue();

  /// Decrementa en 1 el valor de la cola.
  void dec_queue();

  void add_target(Node *, const double &);

  /** Dado un valor de probabilidad p retorna el puntero al nodo sucesores
   *  correspondiente.
   */
  Node * get_target(const double &);

  Designar::TreeSet<Target, TargetCmp> & get_targets();

  /// Retorna true si el uso es igual a la capacidad.
  bool is_full() const;

  Statistics & statistics();
};

# endif // NODE_H

