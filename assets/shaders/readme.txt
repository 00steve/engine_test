engine Shaders

1.0 - About
2.0 - Implimentation
2.1 - Variables


1.0 About
--------------------------





2.0 Implimentation
--------------------------
To add custom shaders to engine, create a vertex shader and a fragment shader as two sperate text (.txt) files in this directory. They should have the same base name, with the extension _vertex.txt and _fragment.txt. Look at the shaders provided with engine as examples.

2.1 Variables
--------------------------
The following variables are and can be set by the engine, so if your shader includes uniform variables with the following names, it can use the variables set by the engine
:fogColor		:vec3	-the color that everything should fade to as distance increases 
:fogDistance	:vec	-the distance that everything should fade away at
:ambientLightColor	:vec3	-the color of the default light applied to everything
:ambientLightAngle:vec3	-the angle of the ambient light
