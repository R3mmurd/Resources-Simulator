/*
  Resources Simulator System.

  Author: Alejandro Mujica (aledrums@gmail.com)
*/

# ifndef EVENT_FACTORY_H
# define EVENT_FACTORY_H

# include <singleton.H>
# include <stack.H>

class Event;

/** Fábrica de eventos implementada como un singleton.
 *
 *  Está implementada como fábrica y almacén, de modo que los punteros que no
 *  se estén utilizando se almacenen aquí para ser reutilizados cuando se pida
 *  nueva memoria, de esa manera se reducen los llamados a new y delete.
 *
 *  Valiéndonos de que Event se implementa como un nodo de lista simplemente
 *  enlazada, se coloca una cabecera por cada tipo de nodo para enlazarlos y
 *  mantenerlos allí para ser reutilizados o para ser liberados en la
 *  destrucción del objeto fábrica.
 */
class Event_Factory : public Designar::Singleton<Event_Factory>
{
  friend class Designar::Singleton<Event_Factory>;
  
  /// Almacen para eventos de llegada externa.
  Designar::ListStack<Event *> external_arrival_events;

  /// Almacen para eventos de llegada interna.
  Designar::ListStack<Event *> internal_arrival_events;

  /// Almacen para eventos de salida.
  Designar::ListStack<Event *> walkout_events;

  static void empty_stack(Designar::ListStack<Event *> &);

public:
  /// El destructor libera la memoria de los eventos almacenados.
  ~Event_Factory();

  Event * get_external_arrival_event();

  Event * get_internal_arrival_event();

  Event * get_walkout_event();

  void store_external_arrival_event(Event *);

  void store_internal_arrival_event(Event *);

  void store_walkout_event(Event *);
};

# endif // EVENT_FACTORY_H

