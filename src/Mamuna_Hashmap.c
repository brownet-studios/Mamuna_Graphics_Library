#include <Mamuna.h>

#include <stdlib.h>
#include <string.h>

typedef enum Mamuna_FLAG {
	EMPTY,
	DELETED,
	USED,
} Mamuna_FLAG;

typedef struct Mamuna_Node {
	Mamuna_FLAG flag;
	char* id;
	void* ptr;
} Mamuna_Node;

typedef struct Mamuna_Hashmap {
	Mamuna_Node* nodes;
	void* data;
	size_t count;
	size_t capacity;
	size_t bytesPerElement;
	size_t countToIncrease;
} Mamuna_Hashmap;

Mamuna_Hashmap* Mamuna_HashmapCreate(size_t bytesPerElement, size_t countToIncrease){
	Mamuna_Hashmap* map = malloc(sizeof(Mamuna_Hashmap));

	map->nodes = calloc(countToIncrease, sizeof(Mamuna_Node));
	map->data = calloc(countToIncrease, bytesPerElement);

	map->count = 0;
	map->capacity = countToIncrease;
	map->bytesPerElement = bytesPerElement;
	map->countToIncrease = countToIncrease;

	return map;
}

void* Mamuna_HashmapDestroy(Mamuna_Hashmap* hashmap, size_t* count){
	if(!hashmap || !count) return NULL;

	*count = hashmap->count;

	for(size_t i = 0; i < hashmap->capacity; ++i){
		Mamuna_Node* tmp = &hashmap->nodes[i];
		if(tmp->flag != USED) continue;
		free(tmp->id);
		printf("%zu\n", i);
	}

	free(hashmap->nodes);
	void* data = hashmap->data;
	free(hashmap);

	return data;
}

void* Mamuna_HashmapGetAll(Mamuna_Hashmap* hashmap, size_t* count){
	if(!hashmap || !count) return NULL;
	*count = hashmap->count;
	return hashmap->data;
}

size_t hash(const char* id){
	size_t h = 102313;

	char c;
	while((c = *(id++)) != '\0' && c != EOF){
		h = h * 33 + c;
	}

	return h;
}

void Mamuna_HashmapRealloc(Mamuna_Hashmap* hashmap, size_t nc){
	hashmap->data = realloc(hashmap->data, hashmap->bytesPerElement * nc);
	Mamuna_Node* tmpNodes = calloc(nc, sizeof(Mamuna_Node));

	for(size_t i = 0; i < hashmap->capacity; ++i){
		Mamuna_Node* tmpA = &hashmap->nodes[i];
		if(tmpA->flag != USED) continue;

		size_t index = hash(tmpA->id) % nc;
		Mamuna_Node* tmpB = &tmpNodes[index];
		while(tmpB->flag != EMPTY){
			index = ++index % nc;
			tmpB = &tmpNodes[index];
		}

		tmpB->id = tmpA->id;
		tmpB->ptr = tmpA->ptr;
		tmpB->flag = tmpA->flag;
	}

	free(hashmap->nodes);
	hashmap->nodes = tmpNodes;
	hashmap->capacity = nc;
}

void Mamuna_HashmapAddElement(Mamuna_Hashmap* hashmap, const char* id, void* element){
	if(!id || !hashmap || !element) return;

	if(hashmap->count + 1 >= hashmap->capacity){
		Mamuna_HashmapRealloc(hashmap, hashmap->capacity + hashmap->countToIncrease);
	}
	if(hashmap->capacity > hashmap->countToIncrease * 2 && hashmap->count <= (hashmap->capacity - hashmap->countToIncrease * 2)){
		Mamuna_HashmapRealloc(hashmap, hashmap->capacity - hashmap->countToIncrease);
	}

	size_t index = hash(id) % hashmap->capacity;
	size_t start = index;
	
	Mamuna_Node* tmp = &hashmap->nodes[index];
	Mamuna_Node* fd = NULL;
	while(tmp->flag != EMPTY){
		if(tmp->flag == DELETED && !fd){
			fd = tmp;
		}

		if(tmp->flag == USED && strcmp(tmp->id, id) == 0){
			memcpy(tmp->ptr, element, hashmap->bytesPerElement);
			return;
		}

		index = ++index % hashmap->capacity;
		if(index == start){
			if(fd) break;
			return;
		}

		tmp = &hashmap->nodes[index];
	}

	if(fd) tmp = fd;

	tmp->flag = USED;
	tmp->ptr = hashmap->data + (hashmap->count++ * hashmap->bytesPerElement);
	tmp->id = strdup(id);
	memcpy(tmp->ptr, element, hashmap->bytesPerElement);
}

Mamuna_Node* Mamuna_HashmapGetNode(Mamuna_Hashmap* hashmap, const char* id){
	if(!hashmap || !id) return NULL;

	size_t index = hash(id) % hashmap->capacity;
	size_t start = index;
	
	Mamuna_Node* tmp = &hashmap->nodes[index];
	while(tmp->flag != EMPTY){
		if(tmp->flag == USED && strcmp(tmp->id, id) == 0){
			return tmp;
		}

		index = ++index % hashmap->capacity;
		if(start == index){
			break;
		}

		tmp = &hashmap->nodes[index];
	}

	return NULL;
}

void* Mamuna_HashmapRemoveElement(Mamuna_Hashmap* hashmap, const char* id){
	Mamuna_Node* tmp = Mamuna_HashmapGetNode(hashmap, id);
	if(!tmp) return NULL;

	void* data = malloc(hashmap->bytesPerElement);
	memcpy(data, tmp->ptr, hashmap->bytesPerElement);
	
	size_t i;
	for(i = 0; i < hashmap->count; ++i){
		if((hashmap->data + i * hashmap->bytesPerElement) == tmp->ptr){
			break;
		}
	}

	size_t delta = hashmap->count - 1 - i;
	
	for(i = 0; i < hashmap->capacity; ++i){
		Mamuna_Node* tmpNode = &hashmap->nodes[i];
		if(tmpNode->ptr == (tmp->ptr + delta * hashmap->bytesPerElement)){
			tmpNode->ptr = tmp->ptr;
		}
	}

	memcpy(tmp->ptr, tmp->ptr + delta * hashmap->bytesPerElement, hashmap->bytesPerElement);
	memset(tmp->ptr + delta * hashmap->bytesPerElement, 0, hashmap->bytesPerElement);


	--hashmap->count;

	if(hashmap->capacity > hashmap->countToIncrease * 2 && hashmap->count <= (hashmap->capacity - hashmap->countToIncrease * 2)){
		Mamuna_HashmapRealloc(hashmap, hashmap->capacity - hashmap->countToIncrease);
	}
	
	tmp->flag = DELETED;
	free(tmp->id);

	return data;
}

void* Mamuna_HashmapGetElement(Mamuna_Hashmap* hashmap, const char* id){
	Mamuna_Node* tmp = Mamuna_HashmapGetNode(hashmap, id);
	if(!tmp) return NULL;

	return tmp->ptr;
}
