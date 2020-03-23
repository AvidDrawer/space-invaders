// Version 1: Has dots moving around. Shooting is possible too!

/*
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.h>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <vector>
#include <list>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

class space_ship
{
private:
	float ssx, ssy;
	//std::vector<float> missile;
	std::list<float> missile;
	int num_missile, missile_start;
public:
	space_ship()
	{
		ssx = 0.4f;
		ssy = -0.8f;
		num_missile = 0;
		missile_start = 0;
	}
	void change_ssx(float move)
	{
		ssx += move;
	}
	void launch_missile()
	{
		++num_missile;
		missile.push_back(ssx);
		missile.push_back(ssy);
	}
	void move_missiles(float disp)
	{
		std::list<float>::iterator it;
		for (it = missile.begin(); it != missile.end(); ++it)
			*(++it) += disp;
	}
	void remove_missile()
	{
		// collision and disappearance
		// reaching end and disappearing 
		// Needs check only on the first missile
		std::list<float>::iterator it;
		if (!missile.empty())
		{
			it = missile.begin();
			if (*(++it) >= 0.8f)
			{
				std::list<float>::iterator it = missile.begin();
				missile.erase(it);
				it = missile.begin();
				missile.erase(it);
				--num_missile;
			}
		}
	}
	void remove_missile(int counter)
	{
		std::list<float>::iterator it = missile.begin(), it2;
		// collision and disappearance
		for (int i = 0; i < counter; ++i)
		{
			++it; ++it;
		}
		it2 = it; ++it2; ++it2;
		missile.erase(it, it2);
		--num_missile;
	}
	int output_num_missile()
	{
		return num_missile;
	}
	float output_ssx()
	{
		return ssx;
	}
	float output_ssy()
	{
		return ssy;
	}
	std::vector<float> missile_return()
	{
		std::vector<float> temp;
		std::list<float>::iterator it = missile.begin();
		for (; it != missile.end(); ++it)
			temp.push_back(*it);
		return temp;
	}
};


int processInput(GLFWwindow* window, int press = 0)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS)
		press = 4;
	else if (glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS)
		press = 6;

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		++press;

	return press;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Space Invaders", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	char vertexshaderpath[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si.vs";
	char fragmentshaderpath[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si.fs";
	char vertexshaderpath_en[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si_en.vs";
	char fragmentshaderpath_en[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si_en.fs";

	Shader sh(vertexshaderpath, fragmentshaderpath);
	Shader shen(vertexshaderpath_en, fragmentshaderpath_en);
	// enemy ship locations
	float villan[] = { -0.5,0.8,1,
-0.4,0.8,1,
-0.3,0.8,1,
-0.2,0.8,1,
-0.1,0.8,1,
0,0.8,1,
0.1,0.8,1,
0.2,0.8,1,
0.3,0.8,1,
0.4,0.8,1,
0.5,0.8,1,
-0.5,0.7,1,
-0.4,0.7,1,
-0.3,0.7,1,
-0.2,0.7,1,
-0.1,0.7,1,
0,0.7,1,
0.1,0.7,1,
0.2,0.7,1,
0.3,0.7,1,
0.4,0.7,1,
0.5,0.7,1,
-0.5,0.6,1,
-0.4,0.6,1,
-0.3,0.6,1,
-0.2,0.6,1,
-0.1,0.6,1,
0,0.6,1,
0.1,0.6,1,
0.2,0.6,1,
0.3,0.6,1,
0.4,0.6,1,
0.5,0.6,1,
-0.5,0.5,1,
-0.4,0.5,1,
-0.3,0.5,1,
-0.2,0.5,1,
-0.1,0.5,1,
0,0.5,1,
0.1,0.5,1,
0.2,0.5,1,
0.3,0.5,1,
0.4,0.5,1,
0.5,0.5,1
	};

	space_ship s;
	float ss[] = { s.output_ssx(), s.output_ssy() };
	unsigned int vao, vbo, vbo_m, vao_m, vao_en, vbo_en;
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &vao_m);
	glGenVertexArrays(1, &vao_en);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vbo_m);
	glGenBuffers(1, &vbo_en);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ss), ss, GL_DYNAMIC_DRAW);
	glBindVertexArray(vao);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(vao_en);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_en);
	glBufferData(GL_ARRAY_BUFFER, sizeof(villan), villan, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glPointSize(6.0f);

	int press, size, vill_dir = 1, num_kill = 0; float vel = 0.5f, vill_vel = 0.2f, disp;
	float currFrame, lastFrame = 0.0f, deltaTime = 0.0f, lapse = 0.0f;
	std::vector<float> temp;
	while (!glfwWindowShouldClose(window))
	{
		currFrame = glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;

		//deltaTime = 0.001;
		disp = vel * deltaTime;

		glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (num_kill != 44)
		{
			press = processInput(window);
			if (press == 4)
			{
				if (ss[0] > -0.8f)
				{
					s.change_ssx(-disp);
					ss[0] = s.output_ssx();
				}
				//std::cout << ss[0] << " " << ss[1] << std::endl;
			}
			else if (press == 6)
			{
				if (ss[0] < 0.8f)
				{
					s.change_ssx(disp);
					ss[0] = s.output_ssx();
				}
				//std::cout << ss[0] << " " << ss[1] << std::endl;
			}
			else if (lapse + 0.2 < currFrame && press == 1)
			{
				s.launch_missile();
				lapse = currFrame;
			}
			else if (press == 5)
			{
				if (lapse + 0.3 < currFrame)
				{
					s.launch_missile();
					lapse = currFrame;
				}
				if (ss[0] > -0.8f)
				{
					s.change_ssx(-disp);
					ss[0] = s.output_ssx();
				}
			}
			else if (press == 7)
			{
				if (lapse + 0.3 < currFrame)
				{
					s.launch_missile();
					lapse = currFrame;
				}
				if (ss[0] < 0.8f)
				{
					s.change_ssx(disp);
					ss[0] = s.output_ssx();
				}
			}
			s.move_missiles(disp);

			sh.use();
			glBindVertexArray(vao);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, sizeof(ss), ss, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_POINTS, 0, 1);

			if (villan[0] >= -0.2)
				vill_dir = -1;
			else if (villan[0] <= -0.8)
				vill_dir = +1;
			for (int i = 0; i < 44; ++i)
				villan[3 * i] += vill_dir * vill_vel * deltaTime;

			temp = s.missile_return();

			std::vector<float>::iterator it; int run = 0;
			for (it = temp.begin(); it < temp.end(); ++it, ++run)
			{
				// find row first. next find column and then remove both
				++it;
				if (abs(*it - villan[1]) <= 0.01)
				{
					--it;
					for (int i = 0; i < 11; ++i)
					{
						if (abs(*it - villan[3 * i]) < 0.01)
						{
							if (villan[3 * i + 2])
							{
								villan[3 * i + 2] = 0;
								s.remove_missile(run);
								++num_kill;
								break;
							}
						}
					}
					++it;
				}
				else if (abs(*it - villan[34]) <= 0.01)
				{
					--it;
					for (int i = 0; i < 11; ++i)
					{
						if (abs(*it - villan[33 + 3 * i]) < 0.01)
						{
							if (villan[33 + 3 * i + 2])
							{
								villan[33 + 3 * i + 2] = 0;
								s.remove_missile(run);
								++num_kill;
								break;
							}
						}
					}
					++it;
				}

				else if (abs(*it - villan[67]) <= 0.01)
				{
					--it;
					for (int i = 0; i < 11; ++i)
					{
						if (abs(*it - villan[66 + 3 * i]) < 0.01)
						{
							if (villan[66 + 3 * i + 2])
							{
								villan[66 + 3 * i + 2] = 0;
								s.remove_missile(run);
								++num_kill;
								break;
							}
						}
					}
					++it;
				}
				else if (abs(*it - villan[100]) <= 0.01)
				{
					--it;
					for (int i = 0; i < 11; ++i)
					{
						if (abs(*it - villan[99 + 3 * i]) < 0.01)
						{
							if (villan[99 + 3 * i + 2])
							{
								villan[99 + 3 * i + 2] = 0;
								s.remove_missile(run);
								++num_kill;
								break;
							}
						}
					}
					++it;
				}

			}

			s.remove_missile();
			size = s.output_num_missile();

			if (size >= 1)
			{
				glBindVertexArray(vao_m);
				glBindBuffer(GL_ARRAY_BUFFER, vbo_m);
				glBufferData(GL_ARRAY_BUFFER, temp.size() * sizeof(float), &temp[0], GL_DYNAMIC_DRAW);

				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
				glEnableVertexAttribArray(0);

				glDrawArrays(GL_POINTS, 0, size);
			}

			shen.use();
			glBindVertexArray(vao_en);
			glBindBuffer(GL_ARRAY_BUFFER, vbo_en);
			glBufferData(GL_ARRAY_BUFFER, sizeof(villan), villan, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_POINTS, 0, 44);

			//std::vector<float>::iterator it = temp.begin();
			//for (; it < temp.end(); ++it)
			//{
			//	std::cout << *(it++) << " ";
			//	std::cout << *it << " ";
			//}


			//std::cout << std::endl;

			//std::cout << size << " " << size*sizeof(float) << " ";


			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	}


	//glfwTerminate();
	return 0;

}
*/