#version 430

// grid view projection matrix
uniform mat4 gVP;

const vec3 Pos[4] = vec3[4](
    vec3(-1.0, 0.0, -1.0),
    vec3( 1.0, 0.0, -1.0),
    vec3( 1.0, 0.0,  1.0),
    vec3(-1.0, 0.0,  1.0)
);

const int Indices[6] = int[6](0, 2, 1, 2, 0, 3);

void main() {
    int Index = Indices[gl_VertexID];
    vec4 vPos = vec4(Pos[Index], 1.0);
    gl_Position = gVP * vPos;
}