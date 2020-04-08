# space-invaders
Using OpenGL and GLFW to recreate the classic game. #practice

---First Game---

The aim of the program is to emulate a decision making system for neural networks. A secondary goal is to keep the code as 
light as possible, how small can the code be while still being easy to extend? The development of this game presents a first 
step in this direction. This makes using C++ and Opengl quite ideal.

As a 2-D game, it is quite simple- use arrow keys to move the attacking ship left and right. Use spacebar to fire missiles. Avoid
enemy fired missiles and prevent them from reaching the bottom of the screen. The graphics used is also just as simple. Basically 
a container is used to display images. Movement of the container simulates movement of various sprites in the game. The vertex shader 
simply takes in the coordinates of the container and the texture mapping (passes it to fragment shader). The fragment shader maps the
required image to the container.

The core idea is to hit all the enemy ships as soon as possible. As levels progress, the game difficulty increases. This can be achieved
by increasing the rate of fire of the enemy ships, faster downward movement, special ships that break away from the herd, aiming at the 
player and so on... 

I wanted to encapsulate the drawing and use it as a separate function that is repeatedly called. All the drawing take place by using the 
offset on a single container placed at the origin of the global CS. This meant a single object can be used for drawing but restricted 
encapsulation for the player and enemy ships. This resulted in most of the code being placed in the main function. Fortunately, the 
simplicity of the game meant that this method would certainly extend easily to as many levels as desired. However, I expect there to be
intergration troubles when the control functions need to be added to simulate an AI-based player. 

The Visual Studio environment is used to set up the game. Learnopengl.com provides a good description on how this can be done. Once 
everything is linked, simply run the source.cpp file and have fun :)

Potential improvements-
1. Add more levels- potentially an easy script to build more levels
2. Display score, #lives
3. Add screen-saver post level completion
4. Add funky trajectory spaceship fall
5. Use a different set of textures that respond better to window resizing and look prettier


![Screenshot](https://github.com/AvidDrawer/space-invaders/blob/master/game-preview/Space_invaders.png)

[Screen capture](https://raw.githubusercontent.com/AvidDrawer/space-invaders/master/game-preview/Space_invaders-gameplay.mp4) of the game in operation. Given the present implementation more or less captures the essence of the game, the updates to
transform it into a true game will be left on the backburner- I'll eventually get to it, just not yet.
