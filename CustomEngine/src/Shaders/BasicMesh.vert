#version 460

// Uniforms for world tranform and view projection
uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;

// Attribute 0 is position, 1 is normal, 2 is tex coords
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// Any vertex outputs (other than position)
out vec2 fragTexCoord;

void main()
{
	// Convert position to homogenous coordinates
	vec4 pos = vec4(inPosition, 1.0);
	// Transform to position world space, then clip space
	gl_Position = pos * uWorldTransform * uViewProjection;

	// Pass along the texture coordinates to frag shader
	fragTexCoord = inTexCoord;
}