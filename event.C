/*
  Resources Simulator System.

  Author: Alejandro Mujica (aledrums@gmail.com)
*/

# include <limits>

# include <event.H>
# include <event_factory.H>
# include <simulator.H>

bool EventCmp::operator () (Event * e1, Event * e2) const
{
  return e1->get_time() < e2->get_time();
}

Event::Event()
  : time(-1.0), ptr_node(nullptr)
{
  // Empty
}

Event::~Event()
{
  // Empty
}

const double & Event::get_time() const
{
  return time;
}

void Event::set_time(const double & _time)
{
  time = _time;
}

Node * Event::get_ptr_node()
{
  return ptr_node;
}

void Event::set_ptr_node(Node * _ptr_node)
{
  ptr_node = _ptr_node;
}

void Event::perform(const double & current_time, Event_Queue *, rng_t &)
{
  Node * ptr_node = get_ptr_node();

  Node::Statistics & statistics = ptr_node->statistics();

  statistics.total_wait_time +=
    ptr_node->get_queue() * (current_time - statistics.prev_event_time);

  statistics.pond_use +=
    ptr_node->get_use() * (current_time - statistics.prev_event_time);
}

void Arrival_Event::perform(const double & current_time,
                            Event_Queue * ptr_queue, rng_t & rng)
{
  // Llama al evento perform de la clase Event.
  Event::perform(current_time, ptr_queue, rng);

  Node * ptr_node = get_ptr_node();

  Node::Statistics & statistics = ptr_node->statistics();

  statistics.arrived++;

  if (ptr_node->is_full()) // El nodo está totalmente ocupado, a cola.
    {
      ptr_node->inc_queue();
      statistics.max_queue = std::max<size_t>(statistics.max_queue,
                                              ptr_node->get_queue());
    }
  else // Hay disponibilidad de atención.
    {
      if (ptr_node->get_use() == 0) // El nodo esta sin atender a nadie.
        statistics.empty_time += current_time - statistics.prev_event_time;
      
      // Pasa a ser atendido de inmediato, genero su salida.
      Event * ptr_walkout_event =
	Event_Factory::get_instance().get_walkout_event();
      
      ptr_walkout_event->set_ptr_node(ptr_node);
      
      expo_dist_t expo(1.0 / ptr_node->get_service_time());
      
      ptr_walkout_event->set_time(current_time + expo(rng));
      ptr_queue->insert(ptr_walkout_event);
      ptr_node->inc_use();
    }
  
  statistics.prev_event_time = current_time;
}

void External_Arrival_Event::perform(const double & current_time,
                                     Event_Queue * ptr_queue, rng_t & rng)
{
  // Llama al evento perform de la clase Arrival_Event.
  Arrival_Event::perform(current_time, ptr_queue, rng);

  Node * ptr_node = get_ptr_node();

  /* Como fue una llegada externa se genera la próxima llegada externa sobre
     este mismo evento para reutilizar el espacio de memoria.
  */
  expo_dist_t expo(1.0 / ptr_node->get_time_between_arrivals());
  
  set_time(current_time + expo(rng));
  
  ptr_queue->insert(this);
}

void Internal_Arrival_Event::perform(const double & current_time,
                                     Event_Queue * ptr_queue, rng_t & rng)
{
  // Llama al evento perform de la clase Arrival_Event.
  Arrival_Event::perform(current_time, ptr_queue, rng);

  /* Como fue una llegada interna almaceno el espacio de memoria para
     reutilizarlo luego.
  */
  Event_Factory::get_instance().store_internal_arrival_event(this);
}

void Walkout_Event::perform(const double & current_time,
                            Event_Queue * ptr_queue, rng_t & rng)
{
  // Llama al evento perform de la clase Event.
  Event::perform(current_time, ptr_queue, rng);

  Node * ptr_node = get_ptr_node();

  Node::Statistics & statistics = ptr_node->statistics();

  // Selecciono nodo sucesor aleatorio.
  std::uniform_real_distribution<double> unif(0.0, 1.0);
  double p = unif(rng);

  Node * ptr_tgt_node = ptr_node->get_target(p);

  if (ptr_tgt_node != nullptr) // Si hubo un sucesor
    {
      // Creo el evento de llegada interna
      Event * ptr_internal_arrival_event =
	Event_Factory::get_instance().get_internal_arrival_event();
      ptr_internal_arrival_event->set_time(current_time);
      ptr_internal_arrival_event->set_ptr_node(ptr_tgt_node);
      
      ptr_queue->insert(ptr_internal_arrival_event);
    }

  statistics.served++;

  if (ptr_node->get_queue() > 0)
    {
      /* Si hay elementos en cola decremento y genero salida reutilizando la
         memoria.
      */
      ptr_node->dec_queue();

      expo_dist_t expo(1.0 / ptr_node->get_service_time());

      set_time(current_time + expo(rng));
      ptr_queue->insert(this);
    }
  else // Si no había nadie en cola decremento uso y almaceno la memoria.
    {
      ptr_node->dec_use();
      Event_Factory::get_instance().store_walkout_event(this);
    }

  statistics.prev_event_time = current_time;
}

