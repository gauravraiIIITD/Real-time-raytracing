#version 330 core
layout (location = 0) in vec3 vVertex;

void main(){  
    gl_Position = vec4(vVertex.x, vVertex.y, vVertex.z, 1.0);
}