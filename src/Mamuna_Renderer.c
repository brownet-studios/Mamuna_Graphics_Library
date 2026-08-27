#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <Mamuna.h>

#include <stdio.h>
#include <string.h>



typedef struct COMMAND {
	GLuint count;
	GLuint instanceCount;
	GLuint firstIndex;
	GLint baseVertex;
	GLuint baseInstance;
} COMMAND;

typedef struct Mamuna_Buffers {
	GLuint VBO;
	GLuint EBO;
	GLuint IBO;
	size_t capacityVBO;
	size_t countVBO;
	size_t capacityEBO;
	size_t countEBO;
	size_t capacityIBO;
	size_t countIBO;
} Mamuna_Buffers;

typedef struct Mamuna_PointersBuffers {
	VERTEX* pVBO;
	GLuint* pEBO;
	COMMAND* pIBO;
} Mamuna_PointersBuffers;

typedef struct Mamuna_Buffer {
	void* ptr;
	GLuint buffer;
	size_t capacity;
	size_t count;
	size_t bytesPerElement;
} Mamuna_Buffer;

typedef struct Mamuna_Shader {
	GLuint shader;
	Mamuna_Buffer** buffers;
	size_t countBuffer;
} Mamuna_Shader;

typedef struct Mamuna_Texture {
	GLuint texture;
	GLenum format;
	int w, h;
} Mamuna_Texture;

typedef struct Mamuna_Renderer {
	GLFWwindow* window;
	Mamuna_Shader* currentShader;
	Mamuna_Texture** currentTextures;
	Mamuna_PointersBuffers ptrs;
	Mamuna_Buffers buffers;
	GLbitfield flags;
	GLenum mode;
	GLuint VAO;
	GLuint FBO;
	GLuint windowTexture;
	GLuint RB;
	GLuint windowShader;
	size_t countMeshes;
	size_t currentW, currentH;
	size_t prevVertexes;
	size_t prevIndices;
	size_t baseInstance;
	size_t texturesCount;
	GLsync fence;
} Mamuna_Renderer;



void Mamuna_InitBuffers(Mamuna_Renderer* renderer){
	GLuint* p = &renderer->buffers.VBO;
	glCreateBuffers(3, p);
	
	size_t* sp = &renderer->buffers.capacityVBO;
	for(size_t i = 0; i < 3; ++i){
		*sp = 100;
		sp += 2;
	}

	sp = &renderer->buffers.countVBO;
	for(size_t i = 0; i < 3; ++i){
		*sp = 0;
		sp += 2;
	}

	
	renderer->flags = GL_DYNAMIC_STORAGE_BIT |
		GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT |
		GL_MAP_COHERENT_BIT;
	glNamedBufferStorage(renderer->buffers.VBO, sizeof(VERTEX) * renderer->buffers.capacityVBO,
			NULL, renderer->flags);
	glNamedBufferStorage(renderer->buffers.EBO, sizeof(GLuint) * renderer->buffers.capacityEBO,
			NULL, renderer->flags);
	glNamedBufferStorage(renderer->buffers.IBO, sizeof(COMMAND) * renderer->buffers.capacityIBO,
			NULL, renderer->flags);
	
	renderer->ptrs.pVBO = glMapNamedBufferRange(renderer->buffers.VBO, 0,
			sizeof(VERTEX) * renderer->buffers.capacityVBO, renderer->flags & ~GL_DYNAMIC_STORAGE_BIT);
	renderer->ptrs.pEBO = glMapNamedBufferRange(renderer->buffers.EBO, 0,
			sizeof(GLuint) * renderer->buffers.capacityEBO, renderer->flags & ~GL_DYNAMIC_STORAGE_BIT);
	renderer->ptrs.pIBO = glMapNamedBufferRange(renderer->buffers.IBO, 0,
			sizeof(COMMAND) * renderer->buffers.capacityIBO, renderer->flags & ~GL_DYNAMIC_STORAGE_BIT);

	VERTEX fullscreenQuad[] = {
		(VERTEX){{-1.f, -1.f, 0.f},{0.f},{0.f, 0.f}},
		(VERTEX){{ 1.f, -1.f, 0.f},{0.f},{1.f, 0.f}},
		(VERTEX){{-1.f,  1.f, 0.f},{0.f},{0.f, 1.f}},
		(VERTEX){{ 1.f,  1.f, 0.f},{0.f},{1.f, 1.f}},
	};

	GLuint fullscreenIndices[] = {
		0,1,2,
		2,1,3,
	};



}

void Mamuna_DestroyBuffers(Mamuna_Renderer* renderer){
	GLuint* p = &renderer->buffers.VBO;
	for(size_t i = 0; i < 3; ++i){
		glUnmapNamedBuffer(*(p + i));
	}

	glDeleteBuffers(3, p);


	size_t* sp = &renderer->buffers.capacityVBO;
	for(size_t i = 0; i < 3; ++i){
		*sp = 0;
		sp += 2;
	}

	sp = &renderer->buffers.countVBO;
	for(size_t i = 0; i < 3; ++i){
		*sp = 0;
		sp += 2;
	}

}

bool Mamuna_ShaderIsCompile(GLuint shader){
	int success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if(!success){
		int len;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		char buff[len];
		glGetShaderInfoLog(shader, len, NULL, buff);
		printf("%s\n", buff);
		return false;
	}
	return true;
}

bool Mamuna_ProgramIsLinked(GLuint prog){
	int success;
	glGetProgramiv(prog, GL_LINK_STATUS, &success);
	if(!success){
		int len;
		glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
		char buff[len];
		glGetProgramInfoLog(prog, len, NULL, buff);
		printf("%s\n", buff);
		return false;
	}

	return true;
}

Mamuna_Renderer* Mamuna_CreateRenderer(Mamuna_Window* window){
	if(!window) return NULL;
	Mamuna_Renderer* renderer = malloc(sizeof(Mamuna_Renderer));
	renderer->window = (GLFWwindow*)window;

	glfwMakeContextCurrent(renderer->window);

	if(!gladLoadGL(glfwGetProcAddress)){
		return NULL;
	}
	
	glCreateVertexArrays(1, &renderer->VAO);
	Mamuna_InitBuffers(renderer);

	glVertexArrayVertexBuffer(renderer->VAO, 0, renderer->buffers.VBO, 0, sizeof(VERTEX));
	glVertexArrayElementBuffer(renderer->VAO, renderer->buffers.EBO);

	glVertexArrayAttribFormat(renderer->VAO, 0, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, pos));
	glVertexArrayAttribFormat(renderer->VAO, 1, 3, GL_FLOAT, GL_FALSE, offsetof(VERTEX, normals));
	glVertexArrayAttribFormat(renderer->VAO, 2, 2, GL_FLOAT, GL_FALSE, offsetof(VERTEX, uv));

	glVertexArrayAttribBinding(renderer->VAO, 0, 0);
	glVertexArrayAttribBinding(renderer->VAO, 1, 0);
	glVertexArrayAttribBinding(renderer->VAO, 2, 0);

	glVertexArrayBindingDivisor(renderer->VAO, 0, 0);
	glVertexArrayBindingDivisor(renderer->VAO, 1, 0);
	glVertexArrayBindingDivisor(renderer->VAO, 2, 0);

	glEnableVertexArrayAttrib(renderer->VAO, 0);
	glEnableVertexArrayAttrib(renderer->VAO, 1);
	glEnableVertexArrayAttrib(renderer->VAO, 2);

	glCreateFramebuffers(1, &renderer->FBO);
	glCreateTextures(GL_TEXTURE_2D, 1, &renderer->windowTexture);
	
	int w, h;
	glfwGetFramebufferSize((GLFWwindow*)window, &w, &h);
	glViewport(0,0,w,h);
	glTextureStorage2D(renderer->windowTexture, 1, GL_RGBA8, w, h);

	glTextureParameteri(renderer->windowTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(renderer->windowTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(renderer->windowTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(renderer->windowTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);


	glCreateRenderbuffers(1, &renderer->RB);
	glNamedRenderbufferStorage(renderer->RB, GL_DEPTH24_STENCIL8, w, h);

	glNamedFramebufferTexture(renderer->FBO, GL_COLOR_ATTACHMENT0, renderer->windowTexture, 0);
	glNamedFramebufferRenderbuffer(renderer->FBO, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderer->RB);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glNamedFramebufferDrawBuffers(renderer->FBO, 1, drawBuffers);

	renderer->currentShader = NULL;
	renderer->currentTextures = NULL;
	renderer->texturesCount = 0;
	renderer->countMeshes = 0;
	renderer->mode = GL_TRIANGLES;

	renderer->prevVertexes = 0;
	renderer->prevIndices = 0;
	renderer->baseInstance = 0;

	renderer->currentW = w;
	renderer->currentH = h;

	renderer->fence = NULL;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return renderer;
}

void Mamuna_DestroyRenderer(Mamuna_Renderer* renderer){
	if(!renderer) return;
	renderer->window = NULL;
	glDeleteVertexArrays(1, &renderer->VAO);
	Mamuna_DestroyBuffers(renderer);
	glDeleteTextures(1, &renderer->windowTexture);
	glDeleteFramebuffers(1, &renderer->FBO);
	glDeleteRenderbuffers(1, &renderer->RB);
	glDeleteProgram(renderer->windowShader);

	free(renderer);
	renderer = NULL;
}

void Mamuna_FlushRenderer(Mamuna_Renderer* renderer){
	if(!renderer) return;
	if(renderer->countMeshes == 0){
		return;
	}


	
	glBindFramebuffer(GL_FRAMEBUFFER, renderer->FBO);
	glBindVertexArray(renderer->VAO);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, renderer->buffers.IBO);
	if(!renderer->currentShader){
		return;
	}
	
	glUseProgram(renderer->currentShader->shader);
	if(renderer->currentShader->countBuffer != 0){
		for(size_t i = 0; i < renderer->currentShader->countBuffer; ++i){
			glBindBufferBase(GL_SHADER_STORAGE_BUFFER, i,
					renderer->currentShader->buffers[i]->buffer);
		}
	}

	if(renderer->texturesCount != 0){
		for(size_t i = 0; i < renderer->texturesCount; ++i){
			glBindTextureUnit(i, renderer->currentTextures[i]->texture);
		}
	}

	glViewport(0,0,renderer->currentW, renderer->currentH);

	glMultiDrawElementsIndirect(renderer->mode, GL_UNSIGNED_INT, 0, renderer->countMeshes, sizeof(COMMAND));
	
	if(renderer->fence){
		glDeleteSync(renderer->fence);
	}

	renderer->fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);



	renderer->countMeshes = 0;
	renderer->prevVertexes = 0;
	renderer->prevIndices = 0;
	renderer->baseInstance = 0;

	size_t* sp = &renderer->buffers.countVBO;
	for(size_t i = 0; i < 3; ++i){
		*sp = 0;
		sp += 2;
	}



}

void Mamuna_SetRenderTargets(Mamuna_Renderer* renderer, size_t amount, Mamuna_Texture** textures){
	if(!renderer) return;
	if(renderer->countMeshes != 0){
		Mamuna_FlushRenderer(renderer);
	}
	
	if(amount == 0 || !textures){
		glNamedFramebufferTexture(renderer->FBO, GL_COLOR_ATTACHMENT0, renderer->windowTexture, 0);
		int w, h;
		glfwGetFramebufferSize((GLFWwindow*)renderer->window, &w, &h);
		renderer->currentW = w;
		renderer->currentH = h;
		glViewport(0,0,w,h);
		GLenum drawBuffers[] = {GL_COLOR_ATTACHMENT0};
		glNamedFramebufferDrawBuffers(renderer->FBO, 1, drawBuffers);
		return;
	}

	GLenum drawBuffers[amount];
	memset(drawBuffers, 0, amount * sizeof(GLenum));

	for(size_t i = 0; i < amount && i < GL_MAX_COLOR_ATTACHMENTS; ++i){
		Mamuna_Texture* tex = textures[i];
		if(!tex) break;
		if(i == 0){
			renderer->currentW = tex->w;
			renderer->currentH = tex->h;
		}
		glNamedFramebufferTexture(renderer->FBO, GL_COLOR_ATTACHMENT0 + i, tex->texture, 0);
		drawBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
	}
	
	glNamedFramebufferDrawBuffers(renderer->FBO, amount, drawBuffers);
	glViewport(0,0, renderer->currentW, renderer->currentH);
}

void Mamuna_ClearRenderer(Mamuna_Renderer* renderer, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
	if(!renderer) return;
	if(renderer->countMeshes != 0){
		Mamuna_FlushRenderer(renderer);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, renderer->FBO);

	glClearColor((float)r / 255.f, (float)g / 255.f, (float)b / 255.f, (float)a / 255.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
			GL_STENCIL_BUFFER_BIT);



}

void Mamuna_RenderPresent(Mamuna_Renderer* renderer){
	if(!renderer) return;
	if(renderer->countMeshes != 0){
		Mamuna_FlushRenderer(renderer);
	}

	if(renderer->fence){
		GLenum result = 0;

		do{
			result = glClientWaitSync(renderer->fence, GL_SYNC_FLUSH_COMMANDS_BIT, 100000);
		}while(result == GL_TIMEOUT_EXPIRED);
		glDeleteSync(renderer->fence);
		renderer->fence = NULL;
	}

	
	int w,h;
	glfwGetFramebufferSize((GLFWwindow*)renderer->window, &w, &h);

	glBlitNamedFramebuffer(renderer->FBO, 0,
			0, 0,
			renderer->currentW, renderer->currentH,
			0, 0,
			w, h,
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
			GL_STENCIL_BUFFER_BIT,
			GL_NEAREST
			);

	if(renderer->fence){
		glDeleteSync(renderer->fence);
	}

	renderer->fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

	glfwSwapBuffers(renderer->window);


	renderer->currentTextures = NULL;
	renderer->texturesCount = 0;
	renderer->currentShader = NULL;
	renderer->currentTextures = NULL;
	renderer->prevVertexes = 0;
	renderer->prevIndices = 0;
	renderer->baseInstance = 0;
	renderer->mode = GL_TRIANGLES;

	size_t* sp = &renderer->buffers.countVBO;
	for(size_t i = 0; i < 3; ++i){
		*sp = 0;
		sp += 2;
	}


}

void Mamuna_SetShader(Mamuna_Renderer* renderer, Mamuna_Shader* shader){
	if(!renderer || !shader) return;
	if(renderer->countMeshes != 0){
		Mamuna_FlushRenderer(renderer);
	}
	renderer->currentShader = shader;
}

void Mamuna_SetTextures(Mamuna_Renderer* renderer, size_t amount, Mamuna_Texture** textures){
	if(!renderer) return;

	if(renderer->countMeshes != 0){
		Mamuna_FlushRenderer(renderer);
	}

	if(amount == 0 || !textures){
		GLint64 max;
		glGetInteger64v(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &max);
		for(GLint64 i = 0; i < max; ++i){
			glBindTextureUnit(i, 0);
		}
		renderer->texturesCount = 0;
		renderer->currentTextures = textures;

		return;
	}


	renderer->texturesCount = amount;
	renderer->currentTextures = textures;

}

void Mamuna_SetModeDraw(Mamuna_Renderer* renderer, MAMUNA_TYPE_DRAW type, float pointSize){
	if(!renderer) return;
	if(renderer->countMeshes != 0){
		Mamuna_FlushRenderer(renderer);
	}

	switch(type){
		case MAMUNA_POINT:
			renderer->mode = GL_POINTS;
			glPointSize(pointSize);
			break;
		case MAMUNA_LINE:
			renderer->mode = GL_LINES;
			break;
		case MAMUNA_TRIANGLES:
			renderer->mode = GL_TRIANGLES;
			break;
	}
}

void Mamuna_DrawVertexes(Mamuna_Renderer* renderer, size_t countVertexes,
		VERTEX* vertexes, size_t countIndices, uint32_t* indices,
		size_t instances){
	if(!renderer) return;

	if(renderer->fence){
		GLenum result = 0;

		do{
			result = glClientWaitSync(renderer->fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000);
		}while(result == GL_TIMEOUT_EXPIRED);
		glDeleteSync(renderer->fence);
		renderer->fence = NULL;
	}

	if(renderer->buffers.countVBO + countVertexes >=
			renderer->buffers.capacityVBO){
		size_t i;
		for(i = renderer->buffers.capacityVBO; i <= renderer->buffers.countVBO + countVertexes; i += 100){
			;
		}

		GLuint tmpBuffer;
		glCreateBuffers(1, &tmpBuffer);
		glNamedBufferStorage(tmpBuffer, sizeof(VERTEX) * i, NULL, renderer->flags);
		VERTEX* tmpPtr = glMapNamedBufferRange(tmpBuffer, 0, sizeof(VERTEX) * i,
				renderer->flags & ~GL_DYNAMIC_STORAGE_BIT);

		memcpy(tmpPtr, renderer->ptrs.pVBO, sizeof(VERTEX) * renderer->buffers.countVBO);
		glUnmapNamedBuffer(renderer->buffers.VBO);
		glDeleteBuffers(1, &renderer->buffers.VBO);
		renderer->buffers.VBO = tmpBuffer;
		renderer->ptrs.pVBO = tmpPtr;
		glVertexArrayVertexBuffer(renderer->VAO, 0, renderer->buffers.VBO, 0, sizeof(VERTEX));

		renderer->buffers.capacityVBO = i;
	
	}

	if(renderer->buffers.countEBO + countIndices >=
			renderer->buffers.capacityEBO){
		size_t i;
		for(i = renderer->buffers.capacityEBO; i <= renderer->buffers.countEBO + countIndices; i += 100){
			;
		}

		GLuint tmpBuffer;
		glCreateBuffers(1, &tmpBuffer);
		glNamedBufferStorage(tmpBuffer, sizeof(GLuint) * i, NULL, renderer->flags);
		GLuint* tmpPtr = glMapNamedBufferRange(tmpBuffer, 0, sizeof(GLuint) * i,
				renderer->flags & ~GL_DYNAMIC_STORAGE_BIT);

		memcpy(tmpPtr, renderer->ptrs.pEBO, sizeof(GLuint) * renderer->buffers.countEBO);
		glUnmapNamedBuffer(renderer->buffers.EBO);
		glDeleteBuffers(1, &renderer->buffers.EBO);
		renderer->buffers.EBO = tmpBuffer;
		renderer->ptrs.pEBO = tmpPtr;

		glVertexArrayElementBuffer(renderer->VAO, renderer->buffers.EBO);

		renderer->buffers.capacityEBO = i;
	}

	if(renderer->buffers.countIBO + 1 >=
			renderer->buffers.capacityIBO){
		size_t i;
		for(i = renderer->buffers.capacityIBO; i <= renderer->buffers.countIBO + 1; i += 100){
			;
		}

		GLuint tmpBuffer;
		glCreateBuffers(1, &tmpBuffer);
		glNamedBufferStorage(tmpBuffer, sizeof(COMMAND) * i, NULL, renderer->flags);
		COMMAND* tmpPtr = glMapNamedBufferRange(tmpBuffer, 0, sizeof(COMMAND) * i,
				renderer->flags & ~GL_DYNAMIC_STORAGE_BIT);

		memcpy(tmpPtr, renderer->ptrs.pIBO, sizeof(COMMAND) * renderer->buffers.countIBO);
		glUnmapNamedBuffer(renderer->buffers.IBO);
		glDeleteBuffers(1, &renderer->buffers.IBO);
		renderer->buffers.IBO = tmpBuffer;
		renderer->ptrs.pIBO = tmpPtr;

		renderer->buffers.capacityIBO = i;

	}

	for(size_t i = 0; i < countVertexes; ++i){
		renderer->ptrs.pVBO[renderer->buffers.countVBO + i] = vertexes[i];
	}

	renderer->buffers.countVBO += countVertexes;

	for(size_t i = 0; i < countIndices; ++i){
		renderer->ptrs.pEBO[renderer->buffers.countEBO + i] = (GLuint)indices[i];
	}

	renderer->buffers.countEBO += countIndices;

	COMMAND command = {
		countIndices,
		instances,
		renderer->prevIndices,
		renderer->prevVertexes,
		renderer->baseInstance,
	};

	renderer->ptrs.pIBO[renderer->buffers.countIBO++] = command;

	renderer->prevIndices += countIndices;
	renderer->prevVertexes += countVertexes;
	renderer->baseInstance += instances;

	++renderer->countMeshes;
} 

Mamuna_Shader* Mamuna_CreateShader(Mamuna_Renderer* renderer,
		const char* vs, const char* fs){
	if(!renderer) return NULL;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vs, NULL);
	glCompileShader(vertexShader);
	if(!Mamuna_ShaderIsCompile(vertexShader)){
		printf("vs\n");
		glDeleteShader(vertexShader);
		return NULL;
	}

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fs, NULL);
	glCompileShader(fragmentShader);
	if(!Mamuna_ShaderIsCompile(fragmentShader)){
		printf("fs\n");
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		return NULL;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	if(!Mamuna_ProgramIsLinked(program)){
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		glDeleteProgram(program);
		return NULL;
	}

	Mamuna_Shader* shader = calloc(1, sizeof(Mamuna_Shader));
	shader->shader = program;

	return shader;
}

void Mamuna_DestroyShader(Mamuna_Shader* shader){
	if(!shader) return;
	if(shader->buffers){
		free(shader->buffers);
	}

	glDeleteProgram(shader->shader);
	free(shader);
	shader = NULL;
}

Mamuna_Buffer* Mamuna_BufferCreate(Mamuna_Renderer* renderer, Mamuna_Shader* shader, const char* nameBuffer,
		size_t bytesPerElement){

	if(!renderer || !shader) return NULL;
	size_t index = glGetProgramResourceIndex(shader->shader, GL_SHADER_STORAGE_BLOCK, nameBuffer);
	if(index == GL_INVALID_INDEX){
		return NULL;
	}
	glShaderStorageBlockBinding(shader->shader, index, shader->countBuffer);

	GLuint SSBO;
	glCreateBuffers(1, &SSBO);
	
	glNamedBufferStorage(SSBO, bytesPerElement * 10, NULL, renderer->flags | GL_MAP_READ_BIT);
	void* ptr = glMapNamedBufferRange(SSBO, 0, bytesPerElement * 10, (renderer->flags | GL_MAP_READ_BIT) & ~GL_DYNAMIC_STORAGE_BIT);

	Mamuna_Buffer* buffer = calloc(1, sizeof(Mamuna_Buffer));
	buffer->ptr = ptr;
	buffer->buffer = SSBO;
	buffer->capacity = 10;
	buffer->bytesPerElement = bytesPerElement;

	if(!shader->buffers){
		shader->buffers = malloc(sizeof(Mamuna_Buffer*) * (shader->countBuffer + 1));
	} else {
		shader->buffers = realloc(shader->buffers, sizeof(Mamuna_Buffer*) * (shader->countBuffer + 1));
	}

	shader->buffers[shader->countBuffer++] = buffer;

	return buffer;
}

void Mamuna_BufferAlignToOtherShader(Mamuna_Buffer* buffer, Mamuna_Shader* shader, const char* nameBuffer){
	if(!buffer || !shader) return;
	size_t index = glGetProgramResourceIndex(shader->shader, GL_SHADER_STORAGE_BLOCK, nameBuffer);
	if(index == GL_INVALID_INDEX){
		return ;
	}
	glShaderStorageBlockBinding(shader->shader, index, shader->countBuffer);
	shader->buffers = realloc(shader->buffers, sizeof(Mamuna_Buffer*) * (shader->countBuffer + 1));
	shader->buffers[shader->countBuffer++] = buffer;

}

void Mamuna_BufferDestroy(Mamuna_Buffer* buffer){
	if(!buffer) return;
	glUnmapBuffer(buffer->buffer);
	glDeleteBuffers(1, &buffer->buffer);

	free(buffer);
	buffer = NULL;
}


void Mamuna_BufferSetElement(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer, size_t index, void* element){
	if(!renderer || !buffer) return;

	if(renderer->fence){
		GLenum result = 0;

		do{
			result = glClientWaitSync(renderer->fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1000000);
		}while(result == GL_TIMEOUT_EXPIRED);
		glDeleteSync(renderer->fence);
		renderer->fence = NULL;
	}

	if(index >= buffer->capacity){
		GLuint tmp;
		glCreateBuffers(1, &tmp);
		glNamedBufferStorage(tmp, buffer->bytesPerElement * (buffer->capacity + 10), NULL, renderer->flags | GL_MAP_READ_BIT);
		glCopyNamedBufferSubData(buffer->buffer, tmp, 0, 0, buffer->bytesPerElement * buffer->capacity);
		
		glUnmapNamedBuffer(buffer->buffer);
		glDeleteBuffers(1, &buffer->buffer);

		buffer->buffer = tmp;
		buffer->capacity += 10;
		buffer->ptr = glMapNamedBufferRange(buffer->buffer, 0,
				buffer->bytesPerElement * buffer->capacity, (renderer->flags | GL_MAP_READ_BIT) & ~GL_DYNAMIC_STORAGE_BIT);


	}
	if(!buffer->ptr){

		return;
	}

	index = index % buffer->capacity;
	memcpy(buffer->ptr + (buffer->bytesPerElement * index),
			element, buffer->bytesPerElement);
}

void* Mamuna_BufferGetElement(Mamuna_Buffer* buffer, size_t index){
	return buffer->ptr + (buffer->bytesPerElement * index);
}

void Mamuna_BufferClear(Mamuna_Renderer* renderer, Mamuna_Buffer* buffer){
	if(!renderer || !buffer) return;
	glNamedBufferStorage(buffer->buffer, buffer->bytesPerElement * buffer->capacity,
			NULL, renderer->flags | GL_MAP_READ_BIT);
}

Mamuna_Texture* Mamuna_CreateTexture(int w, int h, bool linear, int channels){
	if(channels <= 0 || channels > 4) channels = 4;
	Mamuna_Texture* Texture = malloc(sizeof(Mamuna_Texture));
	glCreateTextures(GL_TEXTURE_2D, 1, &Texture->texture);
	switch(channels){
		case 1: 
			glTextureStorage2D(Texture->texture, 1, GL_R8, w, h);
			Texture->format = GL_R8;
			break;
		case 2:
			glTextureStorage2D(Texture->texture, 1, GL_RG8, w, h);
			Texture->format = GL_RG8;
			break;
		case 3:
			glTextureStorage2D(Texture->texture, 1, GL_RGB8, w, h);
			Texture->format = GL_RGB8;
			break;
		case 4:
			glTextureStorage2D(Texture->texture, 1, GL_RGBA8, w, h);
			Texture->format = GL_RGBA8;
			break;
	}
	if(linear){
		glTextureParameteri(Texture->texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(Texture->texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else{
		glTextureParameteri(Texture->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(Texture->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glTextureParameteri(Texture->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(Texture->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	

	Texture->w = w;
	Texture->h = h;

	return Texture;
}

Mamuna_Texture* Mamuna_CreateTextureFromImage(Mamuna_Renderer* renderer, Mamuna_Image* image, bool linear){
	if(!renderer || !image) return NULL;
	int channels = Mamuna_GetImageChannels(image);
	if(channels <= 0 || channels > 4) channels = 4;
	int w, h;
	Mamuna_GetImageSize(image, &w, &h);
	Mamuna_Texture* sTexture = malloc(sizeof(Mamuna_Texture));


	glCreateTextures(GL_TEXTURE_2D, 1, &sTexture->texture);

	switch(channels){
		case 1: 
			glTextureStorage2D(sTexture->texture, 1, GL_R8, w, h);
			sTexture->format = GL_R8;
			break;
		case 2:
			glTextureStorage2D(sTexture->texture, 1, GL_RG8, w, h);
			sTexture->format = GL_RG8;
			break;
		case 3:
			glTextureStorage2D(sTexture->texture, 1, GL_RGB8, w, h);
			sTexture->format = GL_RGB8;
			break;
		case 4:
			glTextureStorage2D(sTexture->texture, 1, GL_RGBA8, w, h);
			sTexture->format = GL_RGBA8;
			break;
	}
	if(linear){
		glTextureParameteri(sTexture->texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(sTexture->texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else{
		glTextureParameteri(sTexture->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(sTexture->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glTextureParameteri(sTexture->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(sTexture->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	unsigned char* pixels = Mamuna_GetImagePixels(image);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	switch(channels){
		case 1:
			glTextureSubImage2D(sTexture->texture, 0, 0, 0, w, h, GL_RED, GL_UNSIGNED_BYTE, pixels);
			break;
		case 2:
			glTextureSubImage2D(sTexture->texture, 0, 0, 0, w, h, GL_RG, GL_UNSIGNED_BYTE, pixels);
			break;
		case 3:
			glTextureSubImage2D(sTexture->texture, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			break;
		case 4:
			glTextureSubImage2D(sTexture->texture, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
			break;
	}

	sTexture->w = w;
	sTexture->h = h;

	return sTexture;
}

void Mamuna_DestroyTexture(Mamuna_Texture* texture){
	if(!texture) return;
	glDeleteTextures(1, &texture->texture);
	free(texture);
}

Mamuna_Texture* Mamuna_CopyTexture(Mamuna_Texture* texture, bool linear){
	if(!texture) return NULL;

	Mamuna_Texture* copy = malloc(sizeof(Mamuna_Texture));
	copy->format = texture->format;
	copy->w = texture->w;
	copy->h = texture->h;

	glCreateTextures(GL_TEXTURE_2D, 1, &copy->texture);
	glTextureStorage2D(copy->texture, 1, copy->format, copy->w, copy->h);

	if(linear){
		glTextureParameteri(copy->texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(copy->texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	} else{
		glTextureParameteri(copy->texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(copy->texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	}

	glTextureParameteri(copy->texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(copy->texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glCopyImageSubData(texture->texture, GL_TEXTURE_2D, 0, 0, 0, 0,copy->texture, GL_TEXTURE_2D, 0, 0, 0, 0,
			copy->w, copy->h, 1);

	return copy;
}

void Mamuna_GetTextureSize(Mamuna_Texture* texture, int* w, int* h){
	*w = texture->w;
	*h = texture->h;
}
