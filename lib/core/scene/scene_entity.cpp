
#include "scene.h"


void		PULSAR::scene::add_entity(PULSAR::ENTITY *entity)
{
	int id = this->entity_manager.add(entity);
	if (id > -1)
		entity->add_to_scene(this->id, id);
}

void		PULSAR::scene::remove_entity(PULSAR::ENTITY *entity)
{
	if (entity->get_local_id(this->id) == -1)
		return;

	this->entity_manager.remove(entity);
	entity->remove_from_scene(this->id);
}

void		PULSAR::scene::remove_entity(unsigned int entity_id)
{
	PULSAR::ENTITY *entity_ptr = this->entity_manager.get_elem(entity_id);
	if (!entity_ptr)
		return;

	if (entity_ptr->get_local_id(this->id) == -1)
		return;

	this->entity_manager.remove(entity_ptr);
	entity_ptr->remove_from_scene(this->id);
}

PULSAR::ENTITY		*PULSAR::scene::get_entity(unsigned int entity_id)
{
	return (this->entity_manager.get_elem(entity_id));
}
