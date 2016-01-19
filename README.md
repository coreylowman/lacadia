# lacadia
Rewrite of my 2D game lacadia (C# using monogame) into a 3D game in C using GLEW and GLFW

I'm writing a majority of the engine. Using GLEW and GLFW for windowing/opengl helper functions, and lodepng for loading/using pngs. Other than that everything is written with a lot of help from online stuff.

### external/
Contains all external libraries that are used.

### shaders/
Contains the shaders that are used with the game. These are copied into the output directory so the executable has them

### src/
Contains the source of the game
##### src/enemies/
Contains the code for the enemies of the game
##### src/game/
Contains the code for the game util structures and functions such as game_object, game_world, spells, effects, etc.
##### src/players/
Contains the code for the various playable characters in the game
##### src/util/
Contains the code for various utility structures such as camera, array_list, set, vec3, mat4, object_model, etc.

### main.c
The main entry point to the program. Handles setting up glfw, glew, shaders, camera, handles the input, and handles calling the various update functions for game_world, etc.
