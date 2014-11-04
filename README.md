Resources Simulator

Discrete Events Simulator based on the simulator developed by Herbert Hoeger
(http://webdelprofesor.ula.ve/ingenieria/hhoeger/curso_sim.html) in the Pascal programming language. This system was developed in the C++ programming language.

* System requeriments:

  - Operating System: GNU/Linux.
  - Compiler: gcc 4.7 or greater, you might use clang.
  - GNU Scientific Library (GSL) http://www.gnu.org/software/gsl/

* Compiling

  - Command make allows to compile the project by default.

  - If you want to compile in debug mode, you can execute "make debug".

* Executing

  - The generated binary file is called "main". That receives 2 parameters:
    1- file_name: Name of the file that with simulating parameters.
    2- Initial seed for random number generator (optional).

  - For example: if you wanted to execute the program with a file called "input"
                 and a seed value 13, you should execute: ./main input 13

  - Program execution generates a .dot file for building graph visualizations by
    using graphviz.

* Input file format

  - Each line in file has one or more variables separated by empty space.

  - The first line contains: simulating time (real) and number of clients at the
    begin (integer).

  - The second line contains the number of nodes (resources) in the network
    (integer).

  - The n next lines contain information about each node. We assume that it'll
    exist the quantity of lines defined in the second line.

  - Each line describing a node are the following variables (in order):
    1- Label of node: a string without empty spaces.
    2- Type of node: 0 for external and 1 for internal.
    3- Average time between arrivals: Real number, only defined for external
       nodes (0).
    4- Average service time: Real number.
    5- Node capacity: Integer number.

  - Is considered that each node has an implicit number from 0 to n - 1 in the
    reading order.

  - The line after last node contains the number of arcs in the system, in other
    words, the number of defined targets in the system for some nodes.

  - The last m lines contain information about each target node. We assume that
    it'll exist the quantity of lines defined in the previous line.

  - Each line defines a relationship "node -> target"" with the following
    variables (in order):
    1- Number of node: Integer number. It refers to the number assigned
       implicitly on the line in which it is located.
    2- Number of target: Integer number. It refers to the number assigned
       implicitly on the line in which it is located.
    3- Probability: Real number that define probability for the node is reached.


- Author: Alejandro J. Mujica
- email: aledrums@gmail.com

