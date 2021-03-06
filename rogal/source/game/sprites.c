/*
* This file manages a linked list of sprites and makes sure no memory
* is leaked when sprites are removed and created.
*/

#include "shared.h"
#include <string.h> //for memset...

sprite_t *first;

/*
* Allocates a new sprite and returns a pointer to it.
*/
sprite_t *alloc_sprite(void) {

	sprite_t *s = malloc(sizeof(sprite_t));

	if (!s) {

		out_of_memory_error(__func__);
	}

	return s;
}

/*
* Frees a sprite and its data if necessary.
*/
void free_sprite(sprite_t *s) {

	if (s->object_data) {

		free(s->object_data);
	}

	free(s);
}

/*
* Returns the first element of sprites linked list.
*/
sprite_t *sprite_head(void) {

	return first;
}

/*
* Adds a new sprite at the end of the list.
*/
void add_sprite_to_list(sprite_t *s) {

	sprite_t *current;
	if (!first) {

		//this is the first sprite
		first = s;
		return;
	}

	current = first;

	//find the last sprite
	while (current->next != NULL) {

		current = current->next;
	}

	//make a link
	current->next = s;
	s->previous = current;
}

/*
* Creates a new, empty sprite.
*/
sprite_t *new_sprite(void) {

	sprite_t *s = alloc_sprite();

	memset(s, 0, sizeof(*s));

	add_sprite_to_list(s);

	s->current_frame = 1;
	s->scale_x = 1.f;
	s->scale_y = 1.f;
	Color3White(s->color);

	return s;
}

/*
* Deletes the requested sprite and frees the memory used by it.
*/
void delete_sprite(sprite_t *s) {

	if (!s) {

		d_printf(LOG_WARNING, "%s: tried to delete a null sprite\n", __func__);
		return;
	}

	//deleting the first sprite?
	if (s->previous == NULL) {

		first = s->next;
	}

	//patch up the list
	if (s->next) {

		s->next->previous = s->previous;
	}

	if (s->previous) {

		s->previous->next = s->next;
	}

	free_sprite(s);
}
