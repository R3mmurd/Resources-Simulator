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

# include <iostream>
# include <fstream>
# include <sstream>
# include <stdexcept>
# include <map>

# include <simulator.H>
# include <event_factory.H>

// Lectura del archivo que describe el simulador
void Simulator::read_graph(const std::string & file_name)
{
  std::ifstream file(file_name.c_str());

  if (not file)
    throw std::logic_error("Cannot open file");

  // Primera línea: tiempo de simulación y número de clientes al inicio.
  file >> final_time >> initial_clients;

  size_t num_nodes;

  // Segunda línea: número de nodos (taquillas).
  file >> num_nodes;

  // Arreglo para tener los punteros a los nodos asociados a un entero.
  Node ** node_array = new Node * [num_nodes];

  /* Se leen las "num_nodes" siguientes líneas del archivo.
     Se asume que existen la cantidad de nodos declaradas en la línea anterior.

     Los nodos descritos en el archivo se asumen en un número que va desde
     0 hasta num_nodes - 1, por lo tanto cada nodo leído será almacenado en
     la posición del arreglo que corresponda según su lectura, para eso nos
     valemos de la variable i.
  */
  for (size_t i = 0; i < num_nodes; ++i)
    {
      std::string label;
      int type;
      double t1 = 0.0;
      double t2;
      size_t cap;

      // Leo las dos primeras variables de la línea: etiqueta y tipo de nodo.
      file >> label >> type;

      // Si el nodo es externo leo el tiempo promedio entre llegadas.
      if (Node::Type(type) == Node::External)
        file >> t1;

      // Luego para cualquiera de los tipos leo el tiempo promedio de servicio.
      file >> t2 >> cap;

      /* Los tiempos entre llegadas y los tiempos de servicio se generan con
         mediante la distribución exponencial. El parámetro t es el tiempo
         medio. std::exponential_distribution requiere es la variable lambda,
         por lo tanto debemos efectuar la operación lambda = 1 / t a cada tiempo
         medio.
      */

      double lambda_t1 = t1 == 0.0 ? 0.0 : 1 / t1;
      double lambda_t2 = 1 / t2;

      Node node;

      node.set_label(label);
      node.set_type(Node::Type(type));
      node.set_time_between_arrivals(lambda_t1);
      node.set_service_time(lambda_t2);
      node.set_capacity(cap);

      // Inserto el nodo en el grafo (al final de la lista).
      graph.push_back(node);
      // Agrego al arreglo la dirección de memoria del nodo insertado.
      node_array[i] = &graph.back();
    }

  size_t num_arcs;

  // Siguiente línea después del último nodo: cantidad de arcos del grafo.
  file >> num_arcs;

  /* Se asume que se describen "num_arcs" arcos o definiciones de sucesores a
     los nodos.
   */
  for (size_t i = 0; i < num_arcs; ++i)
    {
      size_t s, t; // Posiciones de los nodos fuente y sucesor
      double p;    // Probabilidad de que de s vaya a t.

      /* Lectura en orden: posición i-ésima del nodo fuente en el arreglo,
                           posición i-ésima del nodo sucesor en el arreglo,
                           probabilidad de elegir al nodo en la simulación.
      */
      file >> s >> t >> p;

      // Al nodo fuente (s) le añado como sucesor al nodo t con probabilidad p.
      node_array[s]->add_target(node_array[t], p);
    }

  // Libero la memoria utilizada por el arreglo.
  delete [] node_array;

  // Cierro el archivo.
  file.close();

  // Reparte los clientes iniciales equitativamente en los nodos
  std::list<Node>::iterator it = graph.begin();

  for (size_t c = 0; c < initial_clients; ++c)
    {
      it->inc_queue();
      it->statistics().init_queue++;
      it->statistics().arrived++;

      ++it;

      if (it == graph.end())
        it = graph.begin();
    }
}

void Simulator::init_queue()
{
  for (Node & node : graph)
    {
      // Si el nodo no es de entrada lo ignoro
      if (node.get_type() != Node::External)
        continue;

      Event * ptr_event = NEW_EVENT(Event::External_Arrival);
      ptr_event->set_ptr_node(&node);

      expo_dist_t expo;

      ptr_event->set_time(
        expo(rng, expo_dist_t::param_type(node.get_time_between_arrivals()))
      );

      event_queue.push(ptr_event);
    }
}

Event * Simulator::get_next_event()
{
  Event * ptr_event = event_queue.pop();
  current_time = ptr_event->get_time();

  return ptr_event;
}

Simulator::Simulator(const size_t & _seed)
  : seed(_seed), rng(seed), current_time(0.0), final_time(0.0)
{
  // Empty
}

void Simulator::init(const std::string & file_name)
{
  read_graph(file_name);
  init_queue();
}

void Simulator::exec()
{
  current_time = 0.0;

  Event * ptr_event = get_next_event();

  size_t i = 0;

  while (current_time < final_time)
    {
      ptr_event->perform(current_time, &event_queue, rng);
      ptr_event = get_next_event();
    }

  STORE_EVENT(ptr_event);

  for (Node & node : graph)
    {
      Node::Statistics & statistics = node.statistics();

      if (node.get_use() == 0)
        statistics.empty_time += final_time - current_time;
      else
        statistics.pond_use += node.get_use() * (final_time - current_time);

      statistics.total_wait_time +=
        node.get_queue() * (final_time - current_time);
    }
}

std::string Simulator::generate_statistics()
{
  std::stringstream sstr;

  sstr << "Semilla para números aleatorios: " << seed << "\n";
  sstr << "Tiempo de simulación: " << final_time << "\n\n";
 
  for (Node & node : graph)
    {
      sstr << "Nodo: " << node.get_label() << "\n";
      sstr << "Llegaron: " << node.statistics().arrived << "\n";
      sstr << "Servidos: " << node.statistics().served << "\n";
      sstr << "En Servicio: " << node.get_use() << "\n";
      sstr << "Longitud de Cola: " << node.get_queue() << "\n";
      sstr << "Cola Máxima: " << node.statistics().max_queue << "\n";
      sstr << "Cola Inicial: " << node.statistics().init_queue << "\n";
      sstr << "Tiempo Promedio de Espera: "
           << node.statistics().total_wait_time /
              node.statistics().arrived << "\n";
      sstr << "Longitud Promedio de Cola: "
           << node.statistics().total_wait_time / final_time << "\n";
      sstr << "Tiempo Vacío: " << node.statistics().empty_time << "\n";
      sstr << "Ocupación Promedio: "
           << node.statistics().pond_use / final_time << "\n\n";
    }

  return sstr.str();
}

void Simulator::write_dot_from_graph(const std::string & file_name)
{
  std::ofstream file(file_name.c_str());

  // Escritura de cabecera del archivo dot.
  file << "/* Generated automatically by write_dot_from_graph on\n"
       << "   Resources Simulator System\n\n"
       << "   Author: Alejandro J. Mujica\n"
       << "   Email: aledrums at gmail dot com\n"
       << "*/\n\n";

  // Declaración de que se va a construir un digrafo en el .dot.
  file << "digraph\n"
       << "{\n"
          // Especifica que construya el grafo de izquierda a derecha.
       << "  rankdir = LR;\n\n" 
       << "  // Nodes\n";

  // Para mapear la dirección de memoria de un nodo con un entero.
  std::map<Node *, size_t> map_nodes;

  size_t i = 0;

  for (Node & node : graph)
    {
      // Escribe los nodos en el dot y los identifica con un número entero.
      file << "  " << i << "["
           << "label = \"" << node.get_label() << "\\n"
           << "Type: " 
           << (node.get_type() == Node::Internal ? "Internal" : "External")
           << "\\nTime between arrivals: "
           << 1 / node.get_time_between_arrivals()
           << "\\nService time: " << 1 / node.get_service_time() << "\\n"
           << "Capacity: " << node.get_capacity() << "\\n\\n"
           << "Use: " << node.get_use() << "\\nQueue: " << node.get_queue()
           << "\"];\n";

      // Mapea la dirección del nodo con el entero asignado.
      map_nodes[&node] = i;
      ++i;
    }

  file << "\n  // Arcs\n";

  // Recorro los nodos del grafo
  for (Node & node : graph)
    {
      /* Obtiene del mapa el número que se le asignó a la dirección del nodo
         fuente.
      */
      size_t s = map_nodes[&node];

      // Para el nodo actual recorro sus arcos.
      for (const Node::Target & target : node.get_targets())
        {
          Node * ptr_tgt_node = target.ptr_node;

          /* Obtiene del mapa el número que se le asignó a la dirección del nodo
             sucesor.
          */
          size_t t = map_nodes[ptr_tgt_node];

          // Escribe el arco especificando que debe conectar el nodo s con t.
          file << "  " << s << "->" << t << "["
               << "label = \"p = " << target.probability << "\"];\n";
        }
    }

  file << "}\n";
}

