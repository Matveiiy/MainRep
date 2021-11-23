//------- Ignore this ----------
#include<filesystem>
namespace fs = std::filesystem;
//------------------------------

#include<iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<stb/stb_image.h>
#include <functional>
#include "Common.h"
#include"Texture.h"
#include"shaderClass.h"
#include"VAO.h"



GLFWwindow* window;
namespace core {
	class BaseRenderer {
		const char* shader_vert = 
			"#version 330 core\n"
			"layout (location = 0) in vec3 pos;\n"\
			"layout (location = 1) in vec3 color;\n"\
			"out vec3 fcolor;\n"\
			"void main(){\n"\
			"	fcolor=color;\n"
			"	gl_Position=vec4(pos.x,pos.y, pos.z, 1);\n"\
			"}";
		const char* shader_frag =
			"#version 330 core\n"
			"out vec4 FragColor;\n"
			"in vec3 fcolor;\n"\
			"void main(){\n"\
			"	FragColor=vec4(fcolor.r, fcolor.g, fcolor.b, 1);\n"\
			"}";
	public:
		Shader _prim_program;
		VAO vao;VBO vbo;
		std::vector <float> line_data, triangle_data, point_data;
		int countl, countt, countp;
		~BaseRenderer() {}
		BaseRenderer() {}
		void Init() {
			_prim_program.FromText(shader_vert, shader_frag);
			line_data = std::vector<float>(1000, 0);
			triangle_data = std::vector<float>(1000, 0);
			point_data = std::vector<float>(1000, 0);
			vao.Create();
			vbo.Create(1000*sizeof(float), true);
			vao.LinkAttrib(0, 3, GL_FLOAT, 6 * sizeof(float), 0);
			vao.LinkAttrib(1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3*sizeof(float)));
			vao.Unbind();
			vbo.Unbind();
		}
		void Begin() {
			_prim_program.Bind();
			vao.Bind();
			vbo.Bind();
			countl = 0; countp = 0; countt = 0;
		}
		void DrawLine(float z, const Vec2& from, const Vec2& to, const Pixel& color = {255, 255, 255, 255}) {
			line_data[countl++] = from.x;
			line_data[countl++] = from.y;
			line_data[countl++] = z;
			line_data[countl++] = color.r / 255.0f;
			line_data[countl++] = color.g / 255.0f;
			line_data[countl++] = color.b / 255.0f;

			line_data[countl++] = to.x;
			line_data[countl++] = to.y;
			line_data[countl++] = z;
			line_data[countl++] = color.r / 255.0f;
			line_data[countl++] = color.g / 255.0f;
			line_data[countl++] = color.b / 255.0f;
		}
		void DrawTriangle(float z, const Vec2& pos1, const Vec2& pos2, const Vec2& pos3, const Pixel& color1, const Pixel& color2, const Pixel& color3) {
			triangle_data[countt++] = pos1.x;
			triangle_data[countt++] = pos1.y;
			triangle_data[countt++] = z;
			triangle_data[countt++] = color1.r / 255.0f;
			triangle_data[countt++] = color1.g / 255.0f;
			triangle_data[countt++] = color1.b / 255.0f;

			triangle_data[countt++] = pos2.x;
			triangle_data[countt++] = pos2.y;
			triangle_data[countt++] = z;
			triangle_data[countt++] = color2.r / 255.0f;
			triangle_data[countt++] = color2.g / 255.0f;
			triangle_data[countt++] = color2.b / 255.0f;

			triangle_data[countt++] = pos3.x;
			triangle_data[countt++] = pos3.y;
			triangle_data[countt++] = z;
			triangle_data[countt++] = color3.r / 255.0f;
			triangle_data[countt++] = color3.g / 255.0f;
			triangle_data[countt++] = color3.b / 255.0f;
			//TODO *(color1.a /255)
		}
		void DrawPoint(float z, const Vec2& pos,  const core::Pixel& p = { 255, 255, 255, 255 }) {
			point_data[countp++] = pos.x;
			point_data[countp++] = pos.y;
			point_data[countp++] = z;
			point_data[countp++] = p.r / 255.0f;
			point_data[countp++] = p.g / 255.0f;
			point_data[countp++] = p.b / 255.0f;
		}
		void End() {
			if (countl) {
				glBufferSubData(GL_ARRAY_BUFFER, 0, countl * sizeof(float), line_data.data());
				glDrawArrays(GL_LINES, 0, countl / 6);
			}
			if (countp) {
				glBufferSubData(GL_ARRAY_BUFFER, 0, countp * sizeof(float), point_data.data());
				glDrawArrays(GL_POINTS, 0, countp / 6);
			}
			if (countt) {
				glBufferSubData(GL_ARRAY_BUFFER, 0, countt * sizeof(float), triangle_data.data());
				glDrawArrays(GL_TRIANGLES, 0, countt / 6);
			}
		}
		void Close() { vbo.Delete(); vao.Delete();  }
		void SetPointSize(const GLfloat& size) { glPointSize(size); }
		inline void DrawTriangle(float z, const Vec2& pos1, const Vec2& pos2, const Vec2& pos3, const Pixel& color = { 255, 255, 255, 255 }) { DrawTriangle(z, pos1, pos2, pos3, color, color, color); }
	};
	bool fast_init() {
		// Initialize GLFW
		glfwInit();

		// Tell GLFW what version of OpenGL we are using 
		// In this case we are using OpenGL 3.3
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		// Tell GLFW we are using the CORE profile
		// So that means we only have the modern functions
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		// Create a GLFWwindow object of 800 by 800 pixels, naming it "YoutubeOpenGL"
		window = glfwCreateWindow(800, 800, "YoutubeOpenGL", NULL, NULL);
		// Error check if the window fails to create
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return 1;
		}
		// Introduce the window into the current context
		glfwMakeContextCurrent(window);

		//Load GLAD so it configures OpenGL
		if (!gladLoadGL()) return 1;
		glViewport(0, 0, 800, 800);
		return 0;
	}
	void fast_close() {
		glfwDestroyWindow(window);
		glfwTerminate();
	}
}
namespace GUI {
	class GUI {
	public:
		core::BaseRenderer& renderer;
		GUI(core::BaseRenderer& def):renderer(def) {}
		~GUI() {}
	};
}

int main()
{
	if (core::fast_init()) return 0;

	core::BaseRenderer r;
	r.Init();
	// Main while loop
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		OPENGL_CHECK();
		r.Begin();
		r.SetPointSize(5);
		r.DrawLine(0, { 0 ,0 }, { 1, 0 }, core::Pixel(50, 200, 255, 1));
		r.DrawPoint(0, {0.5, 0.5}, core::Pixel(255, 255, 255, 255));
		r.DrawTriangle(.0f, core::Vec2(), { -1 }, { 0, -1 }, core::Pixel(255, 255, 255, 255));
		r.End();
		OPENGL_CHECK();
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	r.Close();

	core::fast_close();
	return 0;
}