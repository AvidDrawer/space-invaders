// Version 1.6: Code clean-up to support multiple levels and ease feature addition
// To do: 
// 1. Add another level once 1st is completed.
//				~ level 2/3 available. Add text to display game completed
// 2. Add text to display score. 
// 3. Add screen-saver post level completion
// 4. Add funky trajectory spaceship fall


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Shader.h>
#include <stb_image.h>

#include <vector>
#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>

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
		//sizes contains the size of each attribute
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

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
		press = 9;

	return press;
}

void frame_buffer_resize_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
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
	glfwSetFramebufferSizeCallback(window, frame_buffer_resize_callback);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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


	std::vector<int> ebo_indices, vao_attrib_sizes;
	std::vector<float> texture_container;
	// Standard box container to hold images
	//        x      y   tex-x   tex-y    
	texture_container = {
			-0.2f, -0.2f, 0.0f, 0.0f,
			 0.2f, -0.2f, 1.0f, 0.0f,
			 0.2f,  0.2f, 1.0f, 1.0f,
			-0.2f,  0.2f, 0.0f, 1.0f
	};
	ebo_indices = {
					0,1,2,
					0,2,3
	};

	vao_attrib_sizes = { 2,2 };

	const float rect_length = abs(texture_container[0] - texture_container[4]);
	const float rect_width = abs(texture_container[1] + texture_container[5]);
	const float scale_factor_ss = 0.25f, scale_factor_en = 0.23f;
	const float threshold = 0.005f;	
	const float left_end_limit = -0.8f, right_end_limit = 0.8f, top_end_limit = 0.8f, bot_end_limit = -0.8f;

	vao_ebo_draw space_ships(ebo_indices, texture_container, 2, vao_attrib_sizes);
	space_ships.setup();

	// VAO-VBO-EBO to initialize drawing
	space_ship space_ship_instance;
	float ss_coord[] = { space_ship_instance.output_ssx(), space_ship_instance.output_ssy() };
	unsigned int vbo_m, vao_m;
	glGenVertexArrays(1, &vao_m);
	glGenBuffers(1, &vbo_m);

	// Textures to show player ship and enemy ships
	char imagepath[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\textures\\enemy_ship.png";
	unsigned int tex, tex2, tex3;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); 
	unsigned char* data = stbi_load(imagepath, &width, &height, &nrChannels, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//spaceship7_si
	char imagepath2[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\textures\\player_ship.png";
	glGenTextures(1, &tex2);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(imagepath2, &width, &height, &nrChannels, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	char imagepath3[] = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\textures\\background.jfif";
	glGenTextures(1, &tex3);
	glBindTexture(GL_TEXTURE_2D, tex3);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	data = stbi_load(imagepath3, &width, &height, &nrChannels, 4);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	// Missile size
	glPointSize(3.0f);

	int key_press, num_ss_missile, en_move_dir = 1, num_kill = 0; 
	float currFrame, lastFrame = 0.0f, deltaTime = 0.0f, lapse = 0.0f;

	std::vector<float> player_fire, enemy_fire;
	
	// contains the lower most row of the enemy space ships. Used to decide which ship fires missile.
	std::list<int> exist_enemy;
	for (int i = 0; i < n_per_row; ++i)
		exist_enemy.push_back(i);

	glm::vec3 offset_vect;
	glm::mat4 model(1.0f);

	std::vector<bool> draw(total_num, 1);
	
	int num_enemy_missile = 0;

	int vect_index = 3 * total_num - 3, vect_index2 = 0, vect_index3 = 0;
	
	// value used to determine how much horizontal movement is acceptable while keeping all ship within the screen
	float en_row_length_norm = abs(villan[0] - villan[vect_index]);
	float nom_disp_per_frame, fps = 0;

	// game b/g rectangle
	std::vector<float>().swap(texture_container);
	texture_container = {
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 4.0f, 0.0f,
			 1.0f,  1.0f, 4.0f, 4.0f,
			-1.0f,  1.0f, 0.0f, 4.0f
	};
	vao_ebo_draw bg(ebo_indices, texture_container, 2, vao_attrib_sizes);
	bg.setup();

	// Game characteristics
	float return_fire_delta_time = 2.0f, last_return_fire_time = 1.0f;
	float ss_missile_reload_time = 0.4f;
	float nom_speed = 0.5f, en_ship_speed = 0.2f;
	float player_missile_vel_mult = 1.1f, en_missile_vel_mult = 1.3;
	int game_level = 1;
	float en_ship_vertical_divider = 0.25;
	bool en_ship_move_dwn = 0;
	while (!glfwWindowShouldClose(window))
	{
		currFrame = glfwGetTime();
		deltaTime = currFrame - lastFrame;
		lastFrame = currFrame;
		++fps;
		//deltaTime = 0.001;
		nom_disp_per_frame = nom_speed * deltaTime;

		if (currFrame - int(currFrame) <= 0.001f)
		{
			std::cout << " Frame draw time [ms]: " << 1000 / fps << " Limit draw time [ms]: " << float(1000) / 60 << "\n";
			fps = 0;
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		sh_ships.use();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex3);
		sh_ships.setMat4("model", model);
		sh_ships.setInt("texture1", 0);
		bg.draw();

		if (num_kill != total_num)
		{
			key_press = processInput(window);

			// Movement and firing of player ship based on input
			if (key_press == 4)
			{
				if (ss_coord[0] > left_end_limit)
				{
					space_ship_instance.change_ssx(-nom_disp_per_frame);
					ss_coord[0] = space_ship_instance.output_ssx();
				}
			}
			else if (key_press == 6)
			{
				if (ss_coord[0] < right_end_limit)
				{
					space_ship_instance.change_ssx(nom_disp_per_frame);
					ss_coord[0] = space_ship_instance.output_ssx();
				}
			}
			else if (lapse + ss_missile_reload_time < currFrame && key_press == 1)
			{
				space_ship_instance.launch_missile();
				lapse = currFrame;
			}
			else if (key_press == 5)
			{
				if (lapse + ss_missile_reload_time < currFrame)
				{
					space_ship_instance.launch_missile();
					lapse = currFrame;
				}
				if (ss_coord[0] > left_end_limit)
				{
					space_ship_instance.change_ssx(-nom_disp_per_frame);
					ss_coord[0] = space_ship_instance.output_ssx();
				}
			}
			else if (key_press == 7)
			{
				if (lapse + ss_missile_reload_time < currFrame)
				{
					space_ship_instance.launch_missile();
					lapse = currFrame;
				}
				if (ss_coord[0] < right_end_limit)
				{
					space_ship_instance.change_ssx(nom_disp_per_frame);
					ss_coord[0] = space_ship_instance.output_ssx();
				}
			}
			space_ship_instance.move_missiles(nom_disp_per_frame * player_missile_vel_mult);

			// Draw player ship
			sh_ships.use();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, tex2);
			offset_vect = glm::vec3(ss_coord[0], ss_coord[1], 0);
			glm::mat4 model(1.0f);
			model = glm::translate(model, offset_vect);
			model = glm::scale(model, glm::vec3(scale_factor_ss, scale_factor_ss, 1.0f));
			sh_ships.setMat4("model", model);
			sh_ships.setInt("texture1", 0);
			space_ships.draw();

			// Horizontal movement of enemy ships 
			if (villan[0] >= right_end_limit - en_row_length_norm)
			{
				en_move_dir = -1;
				en_ship_move_dwn = 1;
			}
			else if (villan[0] <= left_end_limit)
			{
				en_move_dir = +1;
				en_ship_move_dwn = 1;
			}
			for (int ind = 0; ind < n_rows * n_per_row * 3; ind += 3)
				villan[ind] += en_move_dir * en_ship_speed * deltaTime;

			if (en_ship_move_dwn)
			{
				for (int ind = 1; ind < total_num * 3; ind += 3)
					villan[ind] -= en_ship_speed * en_ship_vertical_divider;
				en_ship_move_dwn = 0;
			}

			player_fire = space_ship_instance.missile_return();

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

				// evaluate the position of the missile against all rows of enemy ships acc to above comment
				std::vector<bool> row_conditions;
				bool temp_var;
				for (int i = 0; i < n_rows; ++i)
				{
					vect_index = 3 * n_per_row * i + 1;
					temp_var = villan[vect_index] - (rect_width * scale_factor_en / 2) - threshold < *it && villan[vect_index] + (rect_width * scale_factor_en / 2) + threshold > * it;
					row_conditions.push_back(temp_var);
				}

				// Evaluate row condition. If true, the evaluate column condition, i.e., whether the missile is within the container width of an enemy ship
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
									space_ship_instance.remove_missile(run);
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

			space_ship_instance.remove_missile();
			num_ss_missile = space_ship_instance.output_num_missile();
			// Draw player fired missiles
			if (num_ss_missile >= 1)
			{
				sh_missiles.use();
				glBindVertexArray(vao_m);
				glBindBuffer(GL_ARRAY_BUFFER, vbo_m);
				glBufferData(GL_ARRAY_BUFFER, player_fire.size() * sizeof(float), &player_fire[0], GL_DYNAMIC_DRAW);

				glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GL_FLOAT), (void*)0);
				glEnableVertexAttribArray(0);

				glDrawArrays(GL_POINTS, 0, num_ss_missile);
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
					*(++it) -= nom_disp_per_frame * en_missile_vel_mult;

				// check collision with player ship
				for (it = enemy_fire.begin(); it < enemy_fire.end(); ++it)
				{
					if (*(it + 1) > bot_end_limit + rect_width * scale_factor_ss / 2 + threshold) //-0.75
						break;
					else
					{
						if (*it<ss_coord[0] + (rect_length * scale_factor_ss / 2) + threshold && *it>ss_coord[0] - (rect_length * scale_factor_ss / 2) - threshold)
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

			// check if enemy ships have reachd the level of player ship- death if they have
			for (int ind = total_num * 3 - 2, ctr = total_num - 1; ind > 0; ind -= 3, --ctr)
			{
				if (draw[ctr])
					if (villan[ind] <= space_ship_instance.output_ssy())
					{
						num_kill = total_num;
						break;
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
			key_press = processInput(window);
			if (key_press == 4 || key_press == 5)
			{
				if (ss_coord[0] > -0.8f)
				{
					space_ship_instance.change_ssx(-nom_disp_per_frame);
					ss_coord[0] = space_ship_instance.output_ssx();
				}
			}
			else if (key_press == 6 || key_press == 7)
			{
				if (ss_coord[0] < 0.8f)
				{
					space_ship_instance.change_ssx(nom_disp_per_frame);
					ss_coord[0] = space_ship_instance.output_ssx();
				}
			}

			// Draw only player ship
			sh_ships.use();
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, tex2);
			offset_vect = glm::vec3(ss_coord[0], ss_coord[1], 0);
			glm::mat4 model(1.0f);
			model = glm::translate(model, offset_vect);
			model = glm::scale(model, glm::vec3(scale_factor_ss, scale_factor_ss, 1.0f));
			sh_ships.setMat4("model", model);
			sh_ships.setInt("texture1", 1);
			space_ships.draw();

			if (key_press == 9)
			{
				// Increment game level
				++game_level;
				bool level_up = 1;
				if (game_level < 4)
				{
					// Do nothing
				}
				else // otherwise, reset back to level 1
				{
					game_level = 1;
					level_up = 0;
					return_fire_delta_time = 2.0f;
					ss_missile_reload_time = 0.4f;
					en_ship_speed = 0.2f;
					en_ship_vertical_divider = 0.25;
					player_missile_vel_mult = 1.1f, en_missile_vel_mult = 1.3;
				}

				// Update game characteristics
				return_fire_delta_time -= level_up * 0.4f;
				ss_missile_reload_time -= level_up * 0.05f;
				en_missile_vel_mult += level_up * 0.02f;
				player_missile_vel_mult += level_up * 0.02f;
				en_ship_vertical_divider += level_up * 0.06;
				en_ship_speed += level_up * 0.03f;

				// Reset values in preparation of new level
				n_rows = 0, n_per_row = 0, total_num = 0;
				num_kill = 0;
				std::vector<float>().swap(villan);
				std::vector<float>().swap(player_fire);
				std::vector<float>().swap(enemy_fire);
				std::list<int>().swap(exist_enemy);
				std::vector<bool>().swap(draw);
				space_ship_instance.init();

				// Read new level data
				std::string level_file_p1 = "D:\\C++\\OpenGL\\Tutorial\\Visual Studio\\space_invaders\\level_";
				std::string level_file_p2 = ".txt";
				std::string level_file_str = level_file_p1 + std::to_string(game_level) + level_file_p2;
				//char* level_file;

				lvl_reader.open(level_file_str.c_str(), std::ios::in);
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
				ss_coord[0] = space_ship_instance.output_ssx();
				ss_coord[1] = space_ship_instance.output_ssy();

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