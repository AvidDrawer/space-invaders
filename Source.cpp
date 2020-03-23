// Version 5: Code clean-up to support multiple levels and ease feature addition

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
	int num_missile,missile_start;
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
		// reaching end and disappearing 
		// Needs check only on the first missile
		std::list<float>::iterator it;
		if (!missile.empty())
		{
			it = missile.begin();
			if (*(++it) >= 0.8f)
			{
				it = missile.begin();
				missile.erase(it);
				it = missile.begin();
				missile.erase(it);
				--num_missile;
			}
		}
	}
	void remove_missile(int counter)
	{
		std::list<float>::iterator it = missile.begin(),it2;
		// collision and disappearance
		for (int i = 0; i < counter; ++i)
		{
			++it; ++it;
		}
		it2 = it; ++it2; ++it2;
		missile.erase(it,it2);
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

class enemy_missile
{
private:
	std::vector<float> missile_loc;
public:
	enemy_missile()
	{

	}
	int shoot_missile()
	{
		return rand() % 10;
	}
	void add_missile(float x, float y)
	{
		missile_loc.push_back(x);
		missile_loc.push_back(y);
	}
};

int processInput(GLFWwindow* window,int press=0)
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
	char vertexshaderpath_en[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si_en_v2.vs";
	char fragmentshaderpath_en[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si_en_v2.fs";

	Shader sh(vertexshaderpath, fragmentshaderpath);
	Shader shen(vertexshaderpath_en, fragmentshaderpath_en);
	// enemy ship locations
	float villan[] = { 
		-0.5f,0.8f,0.0f,
-0.4f,0.8f,0.0f,
-0.3f,0.8f,0.0f,
-0.2f,0.8f,0.0f,
-0.1f,0.8f,0.0f,
0.0f,0.8f,0.0f,
0.1f,0.8f,0.0f,
0.2f,0.8f,0.0f,
0.3f,0.8f,0.0f,
0.4f,0.8f,0.0f,
0.5f,0.8f,0.0f,
-0.5f,0.7f,0.0f,
-0.4f,0.7f,0.0f,
-0.3f,0.7f,0.0f,
-0.2f,0.7f,0.0f,
-0.1f,0.7f,0.0f,
0.0f,0.7f,0.0f,
0.1f,0.7f,0.0f,
0.2f,0.7f,0.0f,
0.3f,0.7f,0.0f,
0.4f,0.7f,0.0f,
0.5f,0.7f,0.0f,
-0.5f,0.6f,0.0f,
-0.4f,0.6f,0.0f,
-0.3f,0.6f,0.0f,
-0.2f,0.6f,0.0f,
-0.1f,0.6f,0.0f,
0.0f,0.6f,0.0f,
0.1f,0.6f,0.0f,
0.2f,0.6f,0.0f,
0.3f,0.6f,0.0f,
0.4f,0.6f,0.0f,
0.5f,0.6f,0.0f,
-0.5f,0.5f,0.0f,
-0.4f,0.5f,0.0f,
-0.3f,0.5f,0.0f,
-0.2f,0.5f,0.0f,
-0.1f,0.5f,0.0f,
0.0f,0.5f,0.0f,
0.1f,0.5f,0.0f,
0.2f,0.5f,0.0f,
0.3f,0.5f,0.0f,
0.4f,0.5f,0.0f,
0.5f,0.5f,0.0f
	};

	//        x      y   tex-x   tex-y    
	float rect[] = {
			-0.2f, -0.2f, 0.0f, 0.0f,
			 0.2f, -0.2f, 1.0f, 0.0f,
			 0.2f,  0.2f, 1.0f, 1.0f,
			-0.2f,  0.2f, 0.0f, 1.0f
	};

	unsigned int ind[] = {
		0,1,2,
		0,2,3
	};

	space_ship s;
	float ss[] = {s.output_ssx(), s.output_ssy()};
	unsigned int vao, vbo, ebo, vbo_m, vao_m, vao_en, vbo_en, ebo_en;
	glGenVertexArrays(1, &vao);
	glGenVertexArrays(1, &vao_m);
	glGenVertexArrays(1, &vao_en);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &vbo_m);
	glGenBuffers(1, &ebo);
	glGenBuffers(1, &vbo_en);
	glGenBuffers(1, &ebo_en);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindVertexArray(vao_en);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_en);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rect), rect, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_en);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ind), ind, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	char imagepath[] = "C:\\Users\\sparks\\Pictures\\Saved Pictures\\spaceship10_si.png";	
	unsigned int tex, tex2;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	GLenum format;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	char imagepath2[] = "C:\\Users\\sparks\\Pictures\\Saved Pictures\\spaceship7_si.png";
	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	//int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	data = stbi_load(imagepath2, &width, &height, &nrChannels, 0);
	if (data)
	{
		if (nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glPointSize(6.0f);

	int press, size, vill_dir = 1, num_kill = 0; float vel = 0.5f, vill_vel = 0.2f, disp;
	float currFrame, lastFrame = 0.0f, deltaTime = 0.0f, lapse = 0.0f;
	std::vector<float> temp;

	glm::vec3 test;
	glm::mat4 model(1.0f);
	
	std::vector<bool> draw(44, 1);
	std::vector<float> enemy_fire;

	std::list<int> exist_enemy;
	for (int i = 0; i < 11; ++i)
		exist_enemy.push_back(i);
	std::list<int>::iterator ee_it = exist_enemy.begin();
	int num_enemy_missile = 0;
	float return_fire_delta_time = 2.0f, last_return_fire_time = 1.0f;
	while (!glfwWindowShouldClose(window))
	{
		currFrame = glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;

		//deltaTime = 0.001;
		disp = vel * deltaTime;
		
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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
				if (lapse + 0.2 < currFrame)
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
				if (lapse + 0.2 < currFrame)
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

			shen.use();
			glBindVertexArray(vao);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex2);
			test = glm::vec3(ss[0], ss[1], 0);
			glm::mat4 model(1.0f);
			model = glm::translate(model, test);
			model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
			shen.setMat4("model", model);
			//shen.setFloat("col", villan[i * 4 + 3]);
			shen.setInt("texture1", 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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
				// limiting the vertical chk to minimize #chks
				// providing a cushion for the horizontal chk of 0.01
				++it;
				if (villan[1] - 0.02 < *it && villan[1] - 0.01 > * it)
				{
					--it;
					for (int i = 0; i < 11; ++i)
					{
						if (villan[3 * i] - 0.03 < *it && villan[3 * i] + 0.03 > * it)
						{
							if (draw[i])
							{
								//villan[4 * i + 3] = 0;
								draw[i] = 0;
								s.remove_missile(run);
								++num_kill;
								break;
							}
						}
					}
					++it;
				}
				else if (villan[34] - 0.02 < *it && villan[34] - 0.01 > * it)
				{
					--it;
					for (int i = 0; i < 11; ++i)
					{
						if (villan[33 + 3 * i] - 0.03 < *it && villan[33 + 3 * i] + 0.03 > * it)
						{
							if (draw[11 + i])
							{
								//villan[34 + 3 * i + 2] = 0;
								draw[11 + i] = 0;
								s.remove_missile(run);
								++num_kill;
								break;
							}
						}
					}
					++it;
				}
				else if (villan[67] - 0.02 < *it && villan[67] - 0.01 > * it)
				{
					--it;
					for (int i = 0; i < 11; ++i)
					{
						if (villan[66 + 3 * i] - 0.03 < *it && villan[66 + 3 * i] + 0.03 > * it)
						{
							if (draw[22 + i])
							{
								//villan[67 + 3 * i + 2] = 0;
								draw[22 + i] = 0;
								s.remove_missile(run);
								++num_kill;
								break;
							}
						}
					}
					++it;
				}
				else if (villan[100]-0.02< *it && villan[100] - 0.01 > * it)
				{
					--it;
					for (int i = 0; i < 11; ++i)
					{
						if (villan[99 + 3 * i] - 0.03 < *it && villan[99 + 3 * i] + 0.03 > * it)
						{
							if (draw[33 + i])
							{
								//villan[100 + 3 * i + 2] = 0;
								draw[33 + i] = 0;
								exist_enemy.remove(i);
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
			// Draw player fired missiles
			if (size >= 1)
			{
				sh.use();
				glBindVertexArray(vao_m);
				glBindBuffer(GL_ARRAY_BUFFER, vbo_m);
				glBufferData(GL_ARRAY_BUFFER, temp.size() * sizeof(float), &temp[0], GL_DYNAMIC_DRAW);

				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
				glEnableVertexAttribArray(0);

				glDrawArrays(GL_POINTS, 0, size);
			}

			// Find enemy fired missiles			
			if (currFrame - last_return_fire_time > return_fire_delta_time)
			{
				last_return_fire_time = currFrame;
				int rnd = exist_enemy.size();
				if (rnd > 0)
				{
					int val = rand() % rnd;
					++num_enemy_missile;

					std::list<int>::iterator iter = exist_enemy.begin();

					for (int i = 0; i < val; ++i)
						++iter;

					// fire missile from val
					enemy_fire.push_back(villan[33 * 3 + *iter * 3]);
					enemy_fire.push_back(villan[33 * 3 + *iter * 3 + 1]);
				}
			}

			// mmove enemy missiles
			if (enemy_fire.size())
			{
				//move enemy missiles
				std::vector<float>::iterator it, it2;
				for (it = enemy_fire.begin(); it < enemy_fire.end(); ++it)
					*(++it) -= disp/1.3;

				// check collision with player ship
				for (it = enemy_fire.begin(); it < enemy_fire.end(); ++it)
				{
					if (*(it + 1) > -0.75)
						break;
					else
					{
						if (*it<ss[0] + 0.06 && *it>ss[0] - 0.06)
						{
							std::cout << "Collision. Lost\n";
							num_kill = 44;
						}
					}
					++it;
				}

				//remove missile below spaceship
				it = enemy_fire.begin();
				it2 = it; it2 += 2;
				if (*(it + 1) < -0.8)
					enemy_fire.erase(it, it2);
			}

			// draw said missiles
			if (enemy_fire.size()/2 >= 1)
			{
				sh.use();
				glBindVertexArray(vao_m);
				glBindBuffer(GL_ARRAY_BUFFER, vbo_m);
				glBufferData(GL_ARRAY_BUFFER, enemy_fire.size() * sizeof(float), &enemy_fire[0], GL_DYNAMIC_DRAW);

				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
				glEnableVertexAttribArray(0);

				glDrawArrays(GL_POINTS, 0, enemy_fire.size()/2);
			}

			shen.use();			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);
			glBindVertexArray(vao_en);
			for (int i = 0; i < 44; ++i)
				if (draw[i])
				{
					test = glm::vec3(villan[i * 3], villan[i * 3 + 1], villan[i * 3 + 2]);
					glm::mat4 model(1.0f);
					model = glm::translate(model, test);
					model = glm::scale(model, glm::vec3(0.2f, 0.2f, 1.0f));
					shen.setMat4("model", model);
					//shen.setFloat("col", villan[i * 4 + 3]);
					shen.setInt("texture1", 0);
					glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				}
		}
		else
		{
			press = processInput(window);
			if (press == 4 || press == 5)
			{
				if (ss[0] > -0.8f)
				{
					s.change_ssx(-disp);
					ss[0] = s.output_ssx();
				}
				//std::cout << ss[0] << " " << ss[1] << std::endl;
			}
			else if (press == 6 || press == 7)
			{
				if (ss[0] < 0.8f)
				{
					s.change_ssx(disp);
					ss[0] = s.output_ssx();
				}
				//std::cout << ss[0] << " " << ss[1] << std::endl;
			}
			
			shen.use();
			glBindVertexArray(vao);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex2);
			test = glm::vec3(ss[0], ss[1], 0);
			glm::mat4 model(1.0f);
			model = glm::translate(model, test);
			model = glm::scale(model, glm::vec3(0.25f, 0.25f, 1.0f));
			shen.setMat4("model", model);
			//shen.setFloat("col", villan[i * 4 + 3]);
			shen.setInt("texture1", 1);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	//glfwTerminate();
	return 0;

}