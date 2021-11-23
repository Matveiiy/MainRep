#ifndef VAO_CLASS_H
#define VAO_CLASS_H
#include<glad/glad.h>

class IBO
{
public:
	// ID reference of Elements Buffer Object
	GLuint ID;
	// Constructor that generates a Elements Buffer Object and links it to indices
	IBO () {}
	void Create(GLuint* indices, GLsizeiptr size, bool dynamic = false) { glGenBuffers(1, &ID); this->Bind(); glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW); }
	void Create(GLsizeiptr size, bool dynamic = false) { glGenBuffers(1, &ID); this->Bind(); glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, nullptr, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW); }

	inline void Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID); }
	inline void Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
	inline void Delete() { glDeleteBuffers(1, &ID); }
};
class VBO
{
public:
	GLuint ID;
	inline VBO():ID(0) {}
	void Create(GLfloat* vertices, GLsizeiptr size, bool dynamic = false) { glGenBuffers(1, &ID); this->Bind(); glBufferData(GL_ARRAY_BUFFER, size, vertices, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW); }
	void Create(GLsizeiptr size, bool dynamic = false) { glGenBuffers(1, &ID); this->Bind(); glBufferData(GL_ARRAY_BUFFER, size, nullptr, dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW); }

	void Unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }
	void Bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
	void Delete() { glDeleteBuffers(1, &ID); }
};
class VAO
{
public:
	GLuint ID;

	VAO():ID(0) {}
	inline void Create() { glGenVertexArrays(1, &ID); glBindVertexArray(ID); }
	void LinkAttrib(GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
	void Bind();
	void Unbind();
	void Delete();
};

#endif