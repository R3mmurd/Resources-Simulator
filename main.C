/*
  Resources Simulator System.

  Author: Alejandro Mujica (aledrums@gmail.com)
*/

# include <cstdlib>

# include <iostream>
# include <chrono>

# include <simulator.H>

int main (int argc, char * argv[])
{
  if (argc < 2)
    {
      std::cout << "usage: " << argv[0] << " file [seed]\n";
      return 1;
    }

  std::string file_name = argv[1];

  // Si no se pasa una semilla como parámetro, se "aleatoriza".
  size_t seed = argc < 3
    ? std::chrono::system_clock::now().time_since_epoch().count() % rng_t::max()
		       : std::atoi(argv[2]) % rng_t::max();

  // Construyo el simulador con semilla seed.
  Simulator simulator(seed);

  // Inicializo el simulador con el grafo descrito en el archivo dado.
  simulator.init(file_name);

  // Manda a crear el archivo resources_graph.dot con la descripción del grafo.
  simulator.write_dot_from_net("resources_net.dot");

  // Efectúo la ejecución de la simulación.
  simulator.exec();
  
  // Escribe las estadísticas en la salida estándar.
  std::cout << simulator.generate_statistics() << std::endl;

  return 0;
}
