#version 330

// Input
in vec3 frag_color;
in vec3 frag_normal;
in vec2 frag_texture;
in vec3 frag_position;
in float frag_explosion;
in float noise;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec3 out_texture;
layout(location = 3) out vec4 out_position;

void main()
{
    //out_color = vec4(frag_color, 1);
    out_normal = vec4(frag_normal, 1);
    out_texture = vec3(frag_texture, 1);
    out_position = vec4(frag_position, 1);

    if  (frag_explosion == 3) {
        vec3 color = vec3(frag_texture * ( 1. - 2. * noise), 0.0);
        out_color = vec4(color.rgb, 1.0);
    } else {
        out_color = vec4(frag_color, 1);
    }
}
