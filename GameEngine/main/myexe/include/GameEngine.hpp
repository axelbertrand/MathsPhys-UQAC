#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>

class GameEngine {
    public:
        GameEngine(const std::string& title, unsigned int windowWidth = 800, unsigned int windowHeight = 600);
        ~GameEngine();

		static void framebuffer_size_callback(GLFWwindow* window, int width, int height);

        void run();
    private:
		void initGlfw();
		void initWindow(const std::string& title);
		void initGlad();

		void processInput();
        void update(double t);

		GLFWwindow* mWindow;
		const unsigned int WINDOW_WIDTH;
		const unsigned int WINDOW_HEIGHT;
};