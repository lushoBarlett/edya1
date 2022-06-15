#include "action.h"

Action action_freed() {
	return (Action){Error, contact_freed(), contact_freed()};
}

bool action_is_freed(const Action* action) {
	return contact_is_freed(&action->added) && contact_is_freed(&action->deleted);
}

Action action_copy(const Action* action) {
	return (Action){
		action->type,
		!contact_is_freed(&action->added) ? contact_copy(&action->added) : action->added,
		!contact_is_freed(&action->deleted) ? contact_copy(&action->deleted) : action->deleted
	};
}

Action action_move(Action* action) {
	Action move = (Action){
		action->type,
		!contact_is_freed(&action->added) ? contact_move(&action->added) : action->added,
		!contact_is_freed(&action->deleted) ? contact_move(&action->deleted) : action->deleted
	};

	*action = action_freed();
	return move;
}

Action action_insertion(Contact* insertion) {
	return (Action){
		Insert,
		contact_move(insertion),
		contact_freed()
	};
}

Action action_erasure(Contact* erasure) {
	return (Action){
		Erase,
		contact_freed(),
		contact_move(erasure)
	};
}

Action action_replacement(Contact* insertion, Contact* erasure) {
	return (Action){
		Replace,
		contact_move(insertion),
		contact_move(erasure)
	};
}

void action_free(Action* action) {
	action->type = Error,
	contact_free(&action->added);
	contact_free(&action->deleted);
}