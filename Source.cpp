// Version 5: Code clean-up to support multiple levels and ease feature addition
// To do: 
// !! Get all the definitions in one place, ensure sensible naming and finally, more comments please! 
//				~ more or less done
// 1. Add another level once 1st is completed.
//				~ level 2 available. Add more levels and a text to display game completed
// 2. Add text to display score. 
// 3. Add screen-saver post level completion
// 4. Add funky trajectory spaceship fall
// 5. Make speed changes to enemy missile fire, frequency and reduce overall fire rate of player

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.h>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <vector>
#include <list>
#include <fstream>

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
		init();
	}
	void init()
	{
		ssx = 0.2f;
		ssy = -0.8f;
		num_missile = 0;
		missile_start = 0;
		std::list<float>().swap(missile);
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

class vao_ebo_draw
{
private:
	unsigned int vao, vbo, ebo;
	int num_vertices, num_attr;
	std::vector<float> vbo_data;
	std::vector<int> ebo_data, attr_size, attr_num_cumsum;
public:
	vao_ebo_draw(std::vector<int> external_ebo_data, std::vector<float> external_vbo_data, int n_attr, std::vector<int> sizes)
	{
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);
		
		int cumsum = 0;
		num_vertices = external_ebo_data.size();
		num_attr = n_attr;

		vbo_data = external_vbo_data;
		ebo_data = external_ebo_data;
		
		for (int i = 0; i < num_attr; ++i)
		{
			attr_num_cumsum.push_back(cumsum);
			cumsum += sizes[i];
			attr_size.push_back(sizes[i]);	
		}
		attr_num_cumsum.push_back(cumsum);
	}
	void setup()
	{
		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vbo_data.size()*sizeof(GL_FLOAT), &vbo_data[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_data.size()*sizeof(GL_INT), &ebo_data[0], GL_DYNAMIC_DRAW);
		for (int i = 0; i < num_attr; ++i)
		{
			glVertexAttribPointer(i, attr_size[i], GL_FLOAT, GL_FALSE, attr_num_cumsum[num_attr] * sizeof(GL_FLOAT), (void*)(attr_num_cumsum[i] * sizeof(GL_FLOAT)));
			glEnableVertexAttribArray(i);
		}
	}
	void draw()
	{
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, num_vertices, GL_UNSIGNED_INT, 0);
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

	GLFWwindow* window = glfwCreateWindow(600, 450, "Space Invaders", NULL, NULL);
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

	// Shaders
	char vertexshaderpath[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si.vs";
	char fragmentshaderpath[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si.fs";
	char vertexshaderpath_en[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si_en_v2.vs";
	char fragmentshaderpath_en[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\si_en_v2.fs";

	Shader sh_missiles(vertexshaderpath, fragmentshaderpath);
	Shader sh_ships(vertexshaderpath_en, fragmentshaderpath_en);
	
	// enemy ship locations - File read 
	char level_file[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\level_1.txt";
	int n_rows = 0, n_per_row = 0, total_num = 0;
	float coord_extract;
	std::vector<float> villan;
	std::ifstream lvl_reader;
	lvl_reader.open(level_file, std::ios::in);
	if (lvl_reader.is_open())
	{
		lvl_reader >> n_rows >> n_per_row;
		total_num = n_rows * n_per_row;
		while (true)
		{
			lvl_reader >> coord_extract;
			villan.push_back(coord_extract);
			if (lvl_reader.eof())
				break;
		}
		lvl_reader.close();
	}

	// Standard box container to hold images
	//        x      y   tex-x   tex-y    
	const float rect[] = {
			-0.2f, -0.2f, 0.0f, 0.0f,
			 0.2f, -0.2f, 1.0f, 0.0f,
			 0.2f,  0.2f, 1.0f, 1.0f,
			-0.2f,  0.2f, 0.0f, 1.0f
	};
	float rect_length = abs(rect[0] - rect[4]);
	float rect_width  = abs(rect[1] + rect[5]);
	float scale_factor_ss = 0.25f, scale_factor_en = 0.15f;
	float threshold = 0.005f;
	const float left_end_limit = -0.8f, right_end_limit = 0.8f, top_end_limit = 0.8f, bot_end_limit = -0.8f;

	int ind[] = {
		0,1,2,
		0,2,3
	};

	int sizes[] = { 2,2 };
	std::vector<int> sin(sizes,sizes+2);
	std::vector<float> f(rect, rect + 16);
	std::vector<int> in(ind, ind + 6);

	vao_ebo_draw space_ships(in, f, 2, sin);
	space_ships.setup();

	// VAO-VBO-EBO to initialize drawing
	space_ship s;
	float ss[] = {s.output_ssx(), s.output_ssy()};
	unsigned int vbo_m, vao_m;
	glGenVertexArrays(1, &vao_m);
	glGenBuffers(1, &vbo_m);
	
	// Textures to show player ship and enemy ships
	char imagepath[] = "C:\\Users\\sparks\\Pictures\\Saved Pictures\\spaceship6_si.png";	
	unsigned int tex, tex2;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	int width, height, nrChannels;
	GLenum format;
	//stbi_set_flip_vertically_on_load(true); 
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true); 
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
	
	std::vector<float> player_fire, enemy_fire;
	std::list<int> exist_enemy;

	glm::vec3 offset_vect;
	glm::mat4 model(1.0f);
	
	std::vector<bool> draw(total_num, 1);
	for (int i = 0; i < n_per_row; ++i)
		exist_enemy.push_back(i);
	int num_enemy_missile = 0;

	float return_fire_delta_time = 2.0f, last_return_fire_time = 1.0f;
	float ss_missile_reload_time = 0.2f;
	int vect_index = 3 * total_num - 3, vect_index2 = 0, vect_index3 = 0;
	float en_row_length_norm = abs(villan[0] - villan[vect_index]);
	float fps = 0;
	while (!glfwWindowShouldClose(window))
	{
		currFrame = glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;
		++fps;
		//deltaTime = 0.001;
		disp = vel * deltaTime;
		
		if (currFrame - int(currFrame) <= 0.001f)
		{
			std::cout << "Number of frame per sec: "<<fps << " Frame draw time [s]: " << 1000 / fps << "\n"; 
			fps = 0;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		if (num_kill != total_num)
		{
			press = processInput(window);

			// Movement and firing of player ship based on input
			if (press == 4)
			{
				if (ss[0] > left_end_limit)
				{
					s.change_ssx(-disp);
					ss[0] = s.output_ssx();
				}
			}
			else if (press == 6)
			{
				if (ss[0] < right_end_limit)
				{
					s.change_ssx(disp);
					ss[0] = s.output_ssx();
				}
			}
			else if (lapse + ss_missile_reload_time < currFrame && press == 1)
			{
				s.launch_missile();
				lapse = currFrame;
			}
			else if (press == 5)
			{
				if (lapse + ss_missile_reload_time < currFrame)
				{
					s.launch_missile();
					lapse = currFrame;
				}
				if (ss[0] > left_end_limit)
				{
					s.change_ssx(-disp);
					ss[0] = s.output_ssx();
				}
			}
			else if (press == 7)
			{
				if (lapse + ss_missile_reload_time < currFrame)
				{
					s.launch_missile();
					lapse = currFrame;
				}
				if (ss[0] < right_end_limit)
				{
					s.change_ssx(disp);
					ss[0] = s.output_ssx();
				}
			}
			s.move_missiles(disp);

			// Draw player ship
			sh_ships.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex2);
			offset_vect = glm::vec3(ss[0], ss[1], 0);
			glm::mat4 model(1.0f);
			model = glm::translate(model, offset_vect);
			model = glm::scale(model, glm::vec3(scale_factor_ss, scale_factor_ss, 1.0f));
			sh_ships.setMat4("model", model);
			sh_ships.setInt("texture1", 0);
			space_ships.draw();

			// Horizontal movement of enemy ships 
			if (villan[0] >= right_end_limit-en_row_length_norm)
				vill_dir = -1;
			else if (villan[0] <= left_end_limit)
				vill_dir = +1;
			for (int ind = 0; ind < n_rows * n_per_row * 3; ind += 3) 
				villan[ind] += vill_dir * vill_vel * deltaTime;

			player_fire = s.missile_return();

			// Player fire interaction with enemy ship
			std::vector<float>::iterator it; int run = 0;
			for (it = player_fire.begin(); it < player_fire.end(); ++it, ++run)
			{
				// find row first. next find column and then remove both
				// providing a cushion of 0.05 for the chk 
				++it;

				// Compiling the row crossing conditions-
				// Note: Villan contains the enemy ship coordinates in 3D! x-y-z <assuming 11 ships per row as example>
				//villan[1] - threshold_vert < *it && villan[1] + threshold_vert > * it
				//villan[34] - threshold_vert < *it && villan[34] + threshold_vert > * it
				//villan[67] - threshold_vert < *it && villan[67] + threshold_vert > * it
				//villan[100] - threshold_vert< *it && villan[100] + threshold_vert > * it
				// ...

				//Column conditions
				//villan[3 * i] - threshold_hor < *it && villan[3 * i] + threshold_hor > * it
				//villan[33 + 3 * i] - threshold_hor < *it && villan[33 + 3 * i] + threshold_hor > * it
				//villan[66 + 3 * i] - threshold_hor < *it && villan[66 + 3 * i] + threshold_hor > * it
				//villan[99 + 3 * i] - threshold_hor < *it && villan[99 + 3 * i] + threshold_hor > * it
				// ...
				
				std::vector<bool> row_conditions;
				bool temp_var;
				for (int i = 0; i < n_rows; ++i)
				{
					vect_index = 3 * n_per_row * i + 1;
					temp_var = villan[vect_index] - (rect_width * scale_factor_en / 2) - threshold < *it && villan[vect_index] + (rect_width * scale_factor_en / 2) + threshold > * it;
					row_conditions.push_back(temp_var);
				}

				for (int k = 0; k < n_rows; ++k)
				{
					if (row_conditions[k])
					{
						--it;
						for (int i = 0; i < n_per_row; ++i)
						{
							vect_index = 3 * n_per_row * k + 3 * i;
							vect_index2 = n_per_row * k + i;
							if (villan[vect_index] - (rect_length * scale_factor_en / 2) - threshold < *it && villan[vect_index] + (rect_length * scale_factor_en / 2) + threshold >* it)
							{
								if (draw[vect_index2])
								{
									draw[vect_index2] = 0;
									s.remove_missile(run);
									exist_enemy.remove(i);
									++num_kill;
									break;
								}
							}
						}
						++it;
						break;
					}
				}
			}

			s.remove_missile();
			size = s.output_num_missile();
			// Draw player fired missiles
			if (size >= 1)
			{
				sh_missiles.use();
				glBindVertexArray(vao_m);
				glBindBuffer(GL_ARRAY_BUFFER, vbo_m);
				glBufferData(GL_ARRAY_BUFFER, player_fire.size() * sizeof(float), &player_fire[0], GL_DYNAMIC_DRAW);

				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
				glEnableVertexAttribArray(0);

				glDrawArrays(GL_POINTS, 0, size);
			}

			// Initialize enemy fired missiles			
			if (currFrame - last_return_fire_time > return_fire_delta_time)
			{
				last_return_fire_time = currFrame;
				size_t rnd = exist_enemy.size();
				if (rnd > 0)
				{
					int val = rand() % rnd;
					++num_enemy_missile;

					std::list<int>::iterator iter = exist_enemy.begin();

					for (int i = 0; i < val; ++i)
						++iter;

					// fire missile from val
					vect_index = (n_rows - 1) * n_per_row * 3 + *iter * 3;
					vect_index2 = vect_index + 1;
					enemy_fire.push_back(villan[vect_index]);
					enemy_fire.push_back(villan[vect_index2]);
				}
			}

			// Enemy missile interaction
			if (num_enemy_missile)
			{
				//move enemy missiles
				std::vector<float>::iterator it, it2;
				for (it = enemy_fire.begin(); it < enemy_fire.end(); ++it)
					*(++it) -= disp;

				// check collision with player ship
				for (it = enemy_fire.begin(); it < enemy_fire.end(); ++it)
				{
					if (*(it + 1) > bot_end_limit + rect_width * scale_factor_ss / 2 + threshold) //-0.75
						break;
					else
					{
						if (*it<ss[0] + (rect_length * scale_factor_ss / 2) + threshold && *it>ss[0] - (rect_length * scale_factor_ss / 2) - threshold)
						{
							std::cout << "Collision. Lost\n";
							num_kill = total_num;
						}
					}
					++it;
				}

				//remove missile below player ship
				it = enemy_fire.begin();
				it2 = it; it2 += 2;
				if (*(it + 1) < bot_end_limit)
				{
					enemy_fire.erase(it, it2);
					--num_enemy_missile;
				}
			}

			// draw enemy missiles
			if (num_enemy_missile)
			{
				sh_missiles.use();
				glBindVertexArray(vao_m);
				glBindBuffer(GL_ARRAY_BUFFER, vbo_m);
				glBufferData(GL_ARRAY_BUFFER, enemy_fire.size() * sizeof(float), &enemy_fire[0], GL_DYNAMIC_DRAW);

				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
				glEnableVertexAttribArray(0);

				glDrawArrays(GL_POINTS, 0, num_enemy_missile);
			}

			// draw enemy ships
			sh_ships.use();			
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex);
			for (int i = 0; i < total_num; ++i)
				if (draw[i])
				{
					vect_index = i * 3, vect_index2 = vect_index + 1, vect_index3 = vect_index + 2;
					offset_vect = glm::vec3(villan[vect_index], villan[vect_index2], villan[vect_index3]);
					glm::mat4 model(1.0f);
					model = glm::translate(model, offset_vect);
					model = glm::scale(model, glm::vec3(scale_factor_en, scale_factor_en, 1.0f));
					sh_ships.setMat4("model", model);
					sh_ships.setInt("texture1", 0);
					space_ships.draw();
				}
			glBindVertexArray(0);
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
			}
			else if (press == 6 || press == 7)
			{
				if (ss[0] < 0.8f)
				{
					s.change_ssx(disp);
					ss[0] = s.output_ssx();
				}
			}
			
			// Draw only player ship
			sh_ships.use();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex2);
			offset_vect = glm::vec3(ss[0], ss[1], 0);
			glm::mat4 model(1.0f);
			model = glm::translate(model, offset_vect);
			model = glm::scale(model, glm::vec3(scale_factor_ss, scale_factor_ss, 1.0f));
			sh_ships.setMat4("model", model);
			sh_ships.setInt("texture1", 1);
			space_ships.draw();

			if (press == 1)
			{
				// Reset values in preparation of new level
				n_rows = 0, n_per_row = 0, total_num = 0;
				num_kill = 0;
				std::vector<float>().swap(villan);
				std::vector<float>().swap(player_fire);
				std::vector<float>().swap(enemy_fire);
				std::list<int>().swap(exist_enemy);
				std::vector<bool>().swap(draw);
				s.init();

				// Read new level data
				char level_file[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\level_2.txt";
				lvl_reader.open(level_file, std::ios::in);
				if (lvl_reader.is_open())
				{
					lvl_reader >> n_rows >> n_per_row;
					total_num = n_rows * n_per_row;
					while (true)
					{
						lvl_reader >> coord_extract;
						villan.push_back(coord_extract);
						if (lvl_reader.eof())
							break;
					}
					lvl_reader.close();
				}

				// Initialize values for new level
				for (int i = 0; i < n_per_row; ++i)
					exist_enemy.push_back(i);
				num_enemy_missile = 0;
				last_return_fire_time = currFrame + 2.0f;
				draw = std::vector<bool>(total_num, 1);
				ss[0] = s.output_ssx();
				ss[1] = s.output_ssy();
				
				vect_index = 3 * total_num - 3;
				en_row_length_norm = abs(villan[0] - villan[vect_index]);
			}
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}


	glfwTerminate();
	return 0;

}