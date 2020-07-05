# Resources Simulator

Discrete Events Simulator based on the [simulator developed by Herbert Hoeger](http://webdelprofesor.ula.ve/ingenieria/hhoeger/curso_sim.html)
in the Pascal programming language. This system was developed in the C++ programming language.

## System requirements

- Operating System: GNU/Linux.
- Compiler: gcc 4.7 or greater, you might use clang.

## Compilation

- To compile the project by default execute the command line:

  ```bash
  make
  ```

- If you want to compile in debug mode, you can execute

  ```bash
  make debug
  ```

## Usage

```bash
./main input_file [seed]
```

Where:

- file_name: Name of the file that with simulating parameters.
- seed (optional): Initial seed for random number generator.

## Input

The input of the program is a file with the format described below:

- Each line in file has one or more variables separated by empty space.

- The first line contains: simulating time (real) and number of clients at the
  begin (integer).

- The second line contains the number of nodes (resources) in the network
  (integer).

- The n next lines contain information about each node. We assume that it'll
  exist the quantity of lines defined in the second line.

- Each line describing a node are the following variables (in order):
  - Label of node: a string without empty spaces.
  - Type of node: 0 for external and 1 for internal.
  - Average time between arrivals: Real number, only defined for external
    nodes (0).
  - Average service time: Real number.
  - Node capacity: Integer number.

- Is considered that each node has an implicit number from 0 to n - 1 in the
  reading order.

- The line after last node contains the number of arcs in the system, in other
  words, the number of defined targets in the system for some nodes.

- The last m lines contain information about each target node. We assume that
  it'll exist the quantity of lines defined in the previous line.

- Each line defines a relationship "node -> target" with the following
  variables (in order):
  - Number of node: Integer number. It refers to the number assigned
    implicitly on the line in which it is located.
  - Number of target: Integer number. It refers to the number assigned
    implicitly on the line in which it is located.
  - Probability: Real number that define the probability of reaching target.

## Output

Program execution generates a .dot file for building graph visualizations
by using [Graphviz](https://graphviz.org/).
