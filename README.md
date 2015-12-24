# lacadia
Rewrite of my 2D game lacadia (C# using monogame) into a 3D game in C using GLEW and GLFW

I'm writing a majority of the engine. Using GLEW and GLFW for windowing/opengl helper functions. Other than that everything is written with a lot of help from online stuff. :P

There's still so much to do. What I have so far:

1. Simple obj model loader (reads in obj models that are exported from MagicaVoxel)
2. Instanced Rendering *
3. Moveable Camera
4. Interfaces set up for spells/enemies/players and framework for how they interact
5. Probably some other stuff that I'm not remembering. :P

*This combined with the obj model loader will let me render a ton of models many times each witha different model matrix. Not really much I want to still do for rendering... just actually make the rest of the models!

Still left to do:

1. Bounding boxes
2. Collisions
3. Character Movement
4. Level Generation
5. Enemy AI
6. Character spells
7. All models
8. Animation
9. much more that I'm not remembering
