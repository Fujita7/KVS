/*****************************************************************************/
/**
 *  @file   shader.vert
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#version 120
#include "qualifire.h"

// Output parameters to fragment shader.
VertOut vec3 position;
VertOut vec3 normal;

// Uniform variables (OpenGL variables).
uniform mat4 ModelViewMatrix; // model-view matrix
uniform mat4 ModelViewProjectionMatrix; // model-view projection matrix
uniform mat3 NormalMatrix; // normal matrix
uniform float offset; // offset in clip coordinate


/*===========================================================================*/
/**
 *  @brief  Main function of vertex shader.
 */
/*===========================================================================*/
void main()
{
    gl_Position = ModelViewProjectionMatrix * gl_Vertex;
    gl_Position.z -= offset;
    gl_FrontColor = gl_Color;

    position = ( ModelViewMatrix * gl_Vertex ).xyz;
    normal = NormalMatrix * gl_Normal;
}
