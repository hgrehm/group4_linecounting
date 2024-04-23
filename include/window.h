#pragma once
#include "core.h"
#include "cube.h"
#include "shader.h"
#include "camera.h"
#include "input.h"
#include "enums.h"

/*
 * Window.h
 */

class Window{
public:
	// Window properties
	static int width;
	static int height;
	static const char* window_title;

	// Helper objects
	static Input* input;
	static Shader* shader_program;

	// Objects to draw - TODO replace with Scene object later
	static Cube* cube;


	// INITIALIZATION ----------------------------------------------------------
	static GLFWwindow* create_window(int width, int height);
	static void setup_callbacks(GLFWwindow* window);
	static void setup_scene();

	// DESTRUCTOR
	static void clean_up();
	
	// TODO callbacks
	static void resize_callback(GLFWwindow* window, int width, int height);
	static void display_callback(GLFWwindow* window);
	static void idle_callback();
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

	// Helper to let client poll for events
	static std::vector<int> get_input_actions();
};