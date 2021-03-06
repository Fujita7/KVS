/*****************************************************************************/
/**
 *  @file   multiple_tetrahedra1.vert
 *  @author Jun Nishimura
 */
/*****************************************************************************/
varying vec4 position_in;
varying vec2 random_index_in;
varying float value_in;
varying vec3 normal_in;

attribute vec2 random_index;
attribute float value;

void main( void )
{
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;

    position_in = gl_ModelViewMatrix * gl_Vertex;
    random_index_in = random_index;
    value_in = value;
    normal_in = gl_NormalMatrix * gl_Normal;
}
