/*
  Resources Simulator System.

  Author: Alejandro Mujica (aledrums@gmail.com)
*/

# ifndef SIMULATOR_H
# define SIMULATOR_H

# include <node.H>
# include <event.H>

# include <list.H>
//# include <queue.H>

/// Representa un simulador.
class Simulator
{
  /// Grafo dirigido de recursos.
  Designar::DLList<Node> net;

  /// Cola de eventos.
  Event_Queue event_queue;  

  /// Semilla para el generador de números aleatorios.
  size_t seed;

  /// Generador de números aleatorios.
  rng_t rng;

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
  void read_net(const std::string & file_name);

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

  ~Simulator();

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
  void write_dot_from_net(const std::string & file_name);
};

# endif /* SIMULATOR_H */

