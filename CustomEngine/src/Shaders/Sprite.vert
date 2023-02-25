#version 460

uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;

// Attribute 0 is position, 1 is texture coordinate
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;

out vec2 fragTexCoord;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = pos * uWorldTransform * uViewProjection;
	
	// Pass along the texture coordinate to the fragment shader
	fragTexCoord = inTexCoord;
}