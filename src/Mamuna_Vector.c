#include <Mamuna.h>
#include <string.h>

typedef struct Mamuna_Vector {
	size_t capacity;
	size_t count;
	size_t countToIncrease;
	size_t bytesPerElement;
	void* data;
} Mamuna_Vector;

Mamuna_Vector* Mamuna_VectorCreate(size_t bytesPerElement, size_t countToIncrease){
	Mamuna_Vector* vector = malloc(sizeof(Mamuna_Vector));

	vector->data = calloc(10, bytesPerElement);
	vector->capacity = countToIncrease;
	vector->count = 0;
	vector->countToIncrease = countToIncrease;
	vector->bytesPerElement = bytesPerElement;

	return vector;
}

void* Mamuna_VectorDestroy(Mamuna_Vector* vector, size_t* count){
	if(!vector || !count) return NULL;
	*count = vector->count;
	void* tmp = vector->data;
	free(vector);
	return tmp;
}

void* Mamuna_VectorGetAll(Mamuna_Vector* vector, size_t* count){
	if(!vector || !count) return NULL;
	*count = vector->count;
	return vector->data;
}

void Mamuna_VectorRealloc(Mamuna_Vector* vector, size_t nc){
	if(!vector) return;

	void* tmp = calloc(nc, vector->bytesPerElement);
	for(size_t i = 0; i < vector->count; ++i){
		memcpy(tmp + (i * vector->bytesPerElement),
				vector->data + (i * vector->bytesPerElement),
				vector->bytesPerElement);
	}

	free(vector->data);
	vector->data = tmp;
	vector->capacity = nc;
}

void Mamuna_VectorPushBack(Mamuna_Vector* vector, void* element){
	if(!vector || !element) return;
	if(vector->count + 1 >= vector->capacity){
		Mamuna_VectorRealloc(vector, vector->capacity + vector->countToIncrease);
	}
	if(vector->capacity > vector->countToIncrease * 2 && vector->count <= (vector->capacity - vector->countToIncrease * 2)){
		Mamuna_VectorRealloc(vector, vector->capacity - vector->countToIncrease);
	}

	memcpy(vector->data + (vector->count++ * vector->bytesPerElement),
			element, vector->bytesPerElement);
}

void* Mamuna_VectorPopBack(Mamuna_Vector* vector){
	if(!vector || vector->count == 0) return NULL;



	void* tmp = malloc(vector->bytesPerElement);
	memcpy(tmp,
			vector->data + (--vector->count * vector->bytesPerElement),
			vector->bytesPerElement);

	if(vector->capacity > vector->countToIncrease * 2 && vector->count <= (vector->capacity - vector->countToIncrease * 2)){
		Mamuna_VectorRealloc(vector, vector->capacity - vector->countToIncrease);
	}

	return tmp;
}

void Mamuna_VectorPushFront(Mamuna_Vector* vector, void* element){
	if(!vector || !element) return;

	if(vector->count + 1 >= vector->capacity){
		Mamuna_VectorRealloc(vector, vector->capacity + vector->countToIncrease);
	}
	if(vector->capacity > vector->countToIncrease * 2 && vector->count <= (vector->capacity - vector->countToIncrease * 2)){
		Mamuna_VectorRealloc(vector, vector->capacity - vector->countToIncrease);
	}
	
	for(size_t i = vector->count; i > 0; --i){
		memcpy(vector->data + (i * vector->bytesPerElement),
				vector->data + ((i - 1) * vector->bytesPerElement),
				vector->bytesPerElement);
	}

	memcpy(vector->data, element, vector->bytesPerElement);

	++vector->count;
}

void* Mamuna_VectorPopFront(Mamuna_Vector* vector){
	if(!vector || vector->count == 0) return NULL;

	void* tmp = malloc(vector->bytesPerElement);
	memcpy(tmp, vector->data, vector->bytesPerElement);
	for(size_t i = 1; i < vector->count; ++i){
		memcpy(vector->data + ((i - 1) * vector->bytesPerElement),
				vector->data + (i * vector->bytesPerElement),
				vector->bytesPerElement);
	}
	--vector->count;

	if(vector->capacity > vector->countToIncrease * 2 && vector->count <= (vector->capacity - vector->countToIncrease * 2)){
		Mamuna_VectorRealloc(vector, vector->capacity - vector->countToIncrease);
	}

	return tmp;
}

void* Mamuna_VectorFront(Mamuna_Vector* vector){
	return vector->data;
}

void* Mamuna_VectorBack(Mamuna_Vector* vector){
	return vector->data + ((vector->count - 1) * vector->bytesPerElement);
}
