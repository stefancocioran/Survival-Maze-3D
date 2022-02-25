#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_color;
layout(location = 3) in vec2 v_texture;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;
uniform vec3 object_color;
uniform float explosion;

// Output
out vec3 frag_position;
out vec3 frag_normal;
out vec3 frag_color;
out vec2 frag_texture;
out float noise;
out float frag_explosion;

float rand3D(in vec3 co){
    return fract(sin(dot(co.xyz ,vec3(12.9898,78.233,144.7272))) * 43758.5453);
}


float nfunct(vec3 p) {

  float w = 72.0;
  float t = -.5;

  for (float f = 1.0 ; f <= 10.0 ; f++ ){
    float power = pow( 2.0, f );
    t += abs(rand3D( vec3(power * p)) / power );
  }

  return t;
}


void main()
{
    // Send output to fragment shader
    frag_position = v_position;
    frag_normal = v_normal;
    frag_color = object_color;
    frag_texture = v_texture;
    frag_explosion = explosion;

    vec3 newPosition = v_position;
    if (explosion == 3) {
        // Get 3d noise
        noise = 10.0 *  -.10 * nfunct(.5 * v_normal + Time);
        float r = 5.0 * rand3D(0.05 * v_position + vec3(2.0 * Time));
        // Compose both noises
        float displacement = -noise + r;
        
        // Move the position along the normal and transform it
        newPosition = v_position + v_normal * displacement;
        frag_position = newPosition;
        frag_normal = v_normal + vec3(sin(Time), 0, cos(Time)); 
    }

    if (explosion == 1) {
        frag_color /= 1.5;
    } else if(explosion == 2) {
        frag_color /= 3;
    }

    // Compute gl_Position
    gl_Position = Projection * View * Model * vec4(newPosition, 1);
}
