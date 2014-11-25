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

# ifndef SIMULATOR_H
# define SIMULATOR_H

# include <list>
# include <queue>

# include <event_queue.H>
# include <node.H>

/// Representa un simulador.
class Simulator
{
  /// Grafo dirigido de recursos.
  std::list<Node> graph;

  /// Cola de eventos.
  Event_Queue event_queue;  

  /// Semilla para el generador de números aleatorios.
  size_t seed;

  /// Generador de números aleatorios.
  std::mt19937 rng;

  /// Tiempo actual de simulación.
  double current_time;

  /// Tiempo final de simulación.
  double final_time;

  /// Cantidad de clientes iniciales en el simulador.
  size_t initial_clients;

  /** Lee el archivo que contiene los parámetros de simulación.
   *
   *  @param file_name Nombre del archivo a leer.
   *  @throe logic_error si el archivo no existe.
   */
  void read_graph(const std::string & file_name);

  /// Crea un evento de entrada para cada nodo externo.
  void init_queue();

  /** Extrae de la cola el próximo evento a ejecutarse y actualiza el tiempo de
   *  simulación actual.
   *
   *  @return puntero al evento extraído de la cola.
   */
  Event * get_next_event();

public:
  Simulator(const size_t &);

  /** Inicializa el simulador.
   *
   *  @param file_name Nombre del archivo con parámetros de simulación.
   */
  void init(const std::string & file_name);

  /// Realiza la ejecución del simulador.
  void exec();

  /// Construye una cadena con las estadísticas de cada uno de los nodos.
  std::string generate_statistics();

  /** Genera un archivo .dot para luego convertirlo en una visualización de
   *  grafo.
   *
   *  @param file_name Nombre del archivo que se va a construir.
   */
  void write_dot_from_graph(const std::string & file_name);
};

# endif /* SIMULATOR_H */

