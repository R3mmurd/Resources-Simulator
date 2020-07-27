/*
  Resources Simulator System.

  Author: Alejandro Mujica (aledrums@gmail.com)
*/

# include <cassert>
# include <stdexcept>

# include <node.H>

Node::Node()
  : label(""), type(Num_Types), time_between_arrivals(0.0),
    service_time(0.0), use(0), capacity(0), queue(0)
{
  // Empty
}

const std::string & Node::get_label() const
{
  return label;
}

void Node::set_label(const std::string & _label)
{
  label = _label;
}

const Node::Type & Node::get_type() const
{
  return type;
}

void Node::set_type(const Type & _type)
{
  type = _type;
}

const double & Node::get_time_between_arrivals() const
{
  return time_between_arrivals;
}

void Node::set_time_between_arrivals(const double & _time_between_arrivals)
{
  time_between_arrivals = _time_between_arrivals;
}

const double & Node::get_service_time() const
{
  return service_time;
}

void Node::set_service_time(const double & _service_time)
{
  service_time = _service_time;
}

const unsigned long & Node::get_use() const
{
  return use;
}

void Node::set_use(const unsigned long & _use)
{
  use = _use;
}

void Node::inc_use()
{
  ++use;
}

void Node::dec_use()
{
  if (use == 0)
    throw std::underflow_error("use is empty");

  --use;
}

const unsigned long & Node::get_capacity() const
{
  return capacity;
}

void Node::set_capacity(const unsigned long & _capacity)
{
  capacity = _capacity;
}

const unsigned long & Node::get_queue() const
{
  return queue;
}

void Node::set_queue(const unsigned long & _queue)
{
  queue = _queue;
}

void Node::inc_queue()
{
  ++queue;
}

void Node::dec_queue()
{
  if (queue == 0)
    throw std::underflow_error("queue is empty");

  --queue;
}

void Node::add_target(Node * ptr_node, const double & probability)
{
  Node::Target target = { ptr_node, probability };

  targets.insert(target);
}

Node * Node::get_target(const double & p)
{
  // Precondición de que p esté entre 0 y 1.
  assert(p >= 0.0 and p < 1.0);

  if (targets.is_empty())
    return nullptr;

  /* Como los arcos están ordenados por probabilidad, el primero que se consiga
     que cumpla con la condición p < probabilidad es el sucesor que queremos.
  */
  for (const Node::Target & target : targets)
    if (p < target.probability)
      return target.ptr_node;

  return nullptr;
}

Designar::TreeSet<Node::Target, Node::TargetCmp> & Node::get_targets()
{
  return targets;
}

bool Node::is_full() const
{
  return use == capacity;
}

Node::Statistics & Node::statistics()
{
  return _statistics;
}
