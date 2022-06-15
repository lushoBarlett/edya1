#include "contact_book.h"

#include <stdlib.h>
#include <assert.h>

/*
 * PRIME: número primo grande
 * MAX_LOAD: porcentaje de la tabla que debe estar
 * lleno para que se aumente su tamaño
 * INIT_SIZE: tamaño inicial de la tabla
 */
#define PRIME 1000000007
#define MAX_LOAD 0.75f
#define INIT_SIZE 4

/*
 * Tipo de las celdas de la tabla hash
 *
 * Empty: está libre
 * Occupied: está ocupada
 * Tombstone: estuvo ocupada pero ya no
 */
typedef enum BucketType {
	Empty, Occupied, Tombstone
} BucketType;

/*
 * Un Bucket es una celda de la tabla hash
 * contiene un tipo y un contacto
 * 
 * type: el tipo de la celda
 * contact: el contacto que contiene, está liberado si
 * la celda no está ocupada
 */
struct Bucket {
	BucketType type;
	Contact contact;
};

/*
 * Inicializa las celdas de la tabla hash como vacías
 * con la memoria de los contactos liberada
 */
static void initialize(ContactBook* contacts) {
	for (size_t i = 0; i < contacts->storage_size; i++) {
		contacts->table[i].type = Empty;
		contacts->table[i].contact = contact_freed();
	}
}

ContactBook contacts_make() {
	HashTable table = malloc(sizeof(*table) * INIT_SIZE);
	assert(table);

	ContactBook contacts = (ContactBook){0, 0, INIT_SIZE, table};
	initialize(&contacts);

	return contacts;
};

ContactArray contacts_array(const ContactBook* contacts) {
	ContactArray array = contact_array_make();

	for (size_t i = 0; i < contacts->storage_size; i++) {
		if (contacts->table[i].type == Occupied) {
			Contact copy = contact_copy(&contacts->table[i].contact);
			contact_array_push_back(&array, &copy);
		}
	}

	return array;
}

/*
 * Libera la memoria de las celdas y las marca como vacías
 */
static void free_buckets(ContactBook* contacts) {
	for (size_t i = 0; i < contacts->storage_size; i++) {
		if (contacts->table[i].type == Occupied)
			contact_free(&contacts->table[i].contact);

		contacts->table[i].type = Empty;
	}
}

/*
 * Inserta un array de contactos en la tabla hash,
 * se espera que no haya repetidos
 */
static void insert_batch(ContactBook* contacts, ContactArray* elements) {
	for (size_t i = 0; i < elements->size; i++)
		contacts_insert(contacts, &elements->array[i]);
}

/*
 * Duplica el tamaño de la tabla hash y reinserta todos los elementos
 */
static void rehash(ContactBook* contacts) {
	ContactArray array = contacts_array(contacts);
	free_buckets(contacts);

	contacts->size = 0;
	contacts->inserted = 0;
	contacts->storage_size *= 2;
	contacts->table = realloc(
		contacts->table, sizeof(*contacts->table) * contacts->storage_size);
	assert(contacts->table);

	initialize(contacts);
	insert_batch(contacts, &array);
	contact_array_free(&array);
}

/*
 * Retorna el hash de un contacto comprimido al tamaño de la tabla hash
 */
static size_t hash(const ContactBook* contacts, const Contact* contact) {
	size_t hv = contact_hash(contact);
	return (hv % PRIME) % contacts->storage_size;
}

void contacts_insert(ContactBook* contacts, Contact* contact) {
	size_t hv = hash(contacts, contact);

	while (contacts->table[hv].type == Occupied)
		hv = (hv + 1) == contacts->storage_size ? 0 : hv + 1;

	contacts->size++;
	contacts->inserted += contacts->table[hv].type == Empty;
	contacts->table[hv] = (struct Bucket){Occupied, contact_move(contact)};

	float load = (float)contacts->inserted / (float)contacts->storage_size;
	if (load >= MAX_LOAD)
		rehash(contacts);
}

/*
 * Predicado para determinar si un contacto se encuentra en una celda,
 * sólo compara nombre y apellido
 */
static bool is_here(const HashTable bucket, const Contact* contact) {
	return bucket->type == Occupied &&
	       !contact_comp_name(&bucket->contact, contact) &&
	       !contact_comp_surname(&bucket->contact, contact);
}

// se invalida despues de hacer rehashing
const Contact* contacts_find(const ContactBook* contacts, const Contact* contact) {
	size_t hv = hash(contacts, contact);

	while (contacts->table[hv].type != Empty && !is_here(&contacts->table[hv], contact))
		hv = (hv + 1) == contacts->storage_size ? 0 : hv + 1;

	return contacts->table[hv].type == Occupied ? &contacts->table[hv].contact : NULL;
}

void contacts_erase(ContactBook* contacts, const Contact* contact) {
	size_t hv = hash(contacts, contact);

	while (contacts->table[hv].type != Empty && !is_here(&contacts->table[hv], contact))
		hv = (hv + 1) == contacts->storage_size ? 0 : hv + 1;

	if (contacts->table[hv].type == Occupied) {
		contacts->size--;
		contacts->table[hv].type = Tombstone;
		contact_free(&contacts->table[hv].contact);
	}
}

void contacts_replace(ContactBook* contacts, Contact* contact) {
	contacts_erase(contacts, contact);
	contacts_insert(contacts, contact);
}

void contacts_free(ContactBook* contacts) {
	free_buckets(contacts);
	free(contacts->table);
	*contacts = (ContactBook){0, 0, 0, NULL};
}