/*****************************************************************************/
/**
 *  @file   ProgramObject.cpp
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#include "ProgramObject.h"
#include <kvs/DebugNew>
#include <kvs/Exception>
#include <kvs/OpenGL>
#include <kvs/Platform>
#include <vector>


namespace kvs
{

/*===========================================================================*/
/**
 *  @brief  Constructs a new ProgramObject class.
 */
/*===========================================================================*/
ProgramObject::ProgramObject():
    m_id( 0 ),
    m_geom_input_type( 0 ),
    m_geom_output_type( 0 ),
    m_geom_output_vertices( 0 ),
    m_is_bound( false )
{
}

/*===========================================================================*/
/**
 *  @brief  Destroys the ProgramObject class.
 */
/*===========================================================================*/
ProgramObject::~ProgramObject()
{
    this->release();
}

/*===========================================================================*/
/**
 *  @brief  Returns the program object ID.
 *  @return program object ID
 */
/*===========================================================================*/
GLuint ProgramObject::id() const
{
    return m_id;
}

/*===========================================================================*/
/**
 *  @brief  Returns the information log for a program object.
 *  @return information log
 */
/*===========================================================================*/
std::string ProgramObject::log() const
{
    KVS_ASSERT( this->isCreated() );
    GLint length = 512;
    std::vector<char> buffer( length );
    KVS_GL_CALL( glGetProgramInfoLog( m_id, length, NULL, &buffer[0] ) );
    return std::string( buffer.begin(), buffer.end() );
}

/*===========================================================================*/
/**
 *  @brief  Creates a empty program object.
 */
/*===========================================================================*/
void ProgramObject::create()
{
    this->createID();
}

/*===========================================================================*/
/**
 *  @brief  Clears the program object.
 */
/*===========================================================================*/
void ProgramObject::release()
{
    this->deleteID();
}

/*===========================================================================*/
/**
 *  @brief  Attaches a shader object to a program object.
 *  @param  shader [in] shader object that is to be attached
 */
/*===========================================================================*/
void ProgramObject::attach( const kvs::ShaderObject& shader ) const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glAttachShader( m_id, shader.id() ) );
}

/*===========================================================================*/
/**
 *  @brief  Detaches the shader object from the program object.
 *  @param  shader [in] shader object
 */
/*===========================================================================*/
void ProgramObject::detach( const kvs::ShaderObject& shader ) const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glDetachShader( m_id, shader.id() ) );
}

/*===========================================================================*/
/**
 *  @brief  Detaches the shader objects from the program object.
 */
/*===========================================================================*/
void ProgramObject::detach() const
{
    KVS_ASSERT( this->isCreated() );

    GLint nshaders = 0; // number of attached shaders
    KVS_GL_CALL( glGetProgramiv( m_id, GL_ATTACHED_SHADERS, &nshaders ) );

    if ( nshaders > 0 )
    {
        std::vector<GLuint> ids( nshaders );
        KVS_GL_CALL( glGetAttachedShaders( m_id, nshaders, nullptr, ids.data() ) );

        for ( const auto& id : ids )
        {
            KVS_GL_CALL( glDetachShader( m_id, id ) );

            GLboolean result = GL_FALSE;
            KVS_GL_CALL( result = glIsShader( id ) );
            if ( result == GL_TRUE )
            {
                KVS_GL_CALL( glDeleteShader( id ) );
            }
        }
    }
}

/*===========================================================================*/
/**
 *  @brief  Links the program object.
 *  @return true if the program object is linked successfully
 */
/*===========================================================================*/
bool ProgramObject::link() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glLinkProgram( m_id ) );
    return this->isLinked();
}

/*===========================================================================*/
/**
 *  @brief  Builds shader source codes.
 *  @param  vert_src [in] vertex shader source
 *  @param  frag_src [in] fragment shader source
 */
/*===========================================================================*/
void ProgramObject::build(
    const kvs::ShaderSource& vert_src,
    const kvs::ShaderSource& frag_src )
{
    kvs::VertexShader vert( vert_src );
    if ( !vert.compile() )
    {
        kvsMessageError() << "Vertex shader compile failed.\n" << vert.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Vertex shader compile failed" );
    }

    kvs::FragmentShader frag( frag_src );
    if ( !frag.compile() )
    {
        kvsMessageError() << "Fragment shader compile failed.\n" << frag.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Fragment shader compile failed" );
    }

    this->create();
    this->attach( vert );
    this->attach( frag );
    if ( !this->link() )
    {
        kvsMessageError() << "Program object link failed.\n" << this->log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Program object link failed" );
    }
}

/*===========================================================================*/
/**
 *  @brief  Builds shader source codes.
 *  @param  vert_src [in] vertex shader source
 *  @param  geom_src [in] geometry shader source
 *  @param  frag_src [in] fragment shader source
 */
/*===========================================================================*/
void ProgramObject::build(
    const kvs::ShaderSource& vert_src,
    const kvs::ShaderSource& geom_src,
    const kvs::ShaderSource& frag_src )
{
    kvs::VertexShader vert( vert_src );
    if ( !vert.compile() )
    {
        kvsMessageError() << "Vertex shader compile failed.\n" << vert.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Vertex shader compile failed" );
    }

    kvs::GeometryShader geom( geom_src );
    if ( !geom.compile() )
    {
        kvsMessageError() << "Geometry shader compile failed.\n" << geom.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Geometry shader compile failed" );
    }

    kvs::FragmentShader frag( frag_src );
    if ( !frag.compile() )
    {
        kvsMessageError() << "Fragment shader compile failed.\n" << frag.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Fragment shader compile failed" );
    }

    this->create();
    this->attach( vert );
    this->attach( geom );
    this->attach( frag );

    // Set parameters for the geometry shader.
    this->setParameter( GL_GEOMETRY_INPUT_TYPE_EXT, m_geom_input_type );
    this->setParameter( GL_GEOMETRY_OUTPUT_TYPE_EXT, m_geom_output_type );
    const GLint max_output_vertices = kvs::OpenGL::Integer( GL_MAX_GEOMETRY_OUTPUT_VERTICES_EXT );
    if ( max_output_vertices < m_geom_output_vertices )
    {
        kvsMessageError()
            << "Geometry shader GL_GEOMETRY_VERTICES_OUT require = "
            << m_geom_output_vertices << " > max = " << max_output_vertices << std::endl;
        KVS_THROW( kvs::OpenGLException, "Geometry shader cannot be attached" );
    }
    this->setParameter( GL_GEOMETRY_VERTICES_OUT_EXT, m_geom_output_vertices );

    if ( !this->link() )
    {
        kvsMessageError() << "Program object link failed.\n" << this->log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Program object link failed" );
    }
}

/*===========================================================================*/
/**
 *  @brief  Installs the program object as part of current rendering state.
 */
/*===========================================================================*/
void ProgramObject::bind() const
{
    KVS_ASSERT( this->isCreated() );
    KVS_GL_CALL( glUseProgram( m_id ) );
    m_is_bound = true;
}

/*===========================================================================*/
/**
 *  @brief  Uninstalls the program object as part of current rendering state.
 */
/*===========================================================================*/
void ProgramObject::unbind() const
{
    KVS_ASSERT( this->isBound() );
    KVS_GL_CALL( glUseProgram( 0 ) );
    m_is_bound = false;
}

/*===========================================================================*/
/**
 *  @brief  Determines if the program object is compiled.
 *  @return true if the program program is compiled
 */
/*===========================================================================*/
bool ProgramObject::isCreated() const
{
    return m_id > 0;
}

/*===========================================================================*/
/**
 *  @brief  Determins if a name corresponds to a program object.
 *  @return true if the program object ID is the name of a program object previously created
 */
/*===========================================================================*/
bool ProgramObject::isValid() const
{
    GLboolean result = GL_FALSE;
    KVS_GL_CALL( result = glIsProgram( m_id ) );
    return result == GL_TRUE;
}

/*===========================================================================*/
/**
 *  @brief  Determines if the program object is bound.
 *  @return true if the program program is bound
 */
/*===========================================================================*/
bool ProgramObject::isBound() const
{
    return m_is_bound;
}

/*===========================================================================*/
/**
 *  @brief  Determines if the program object is linked.
 *  @return true if the program program is linked
 */
/*===========================================================================*/
bool ProgramObject::isLinked() const
{
    GLint error = 0;
    KVS_GL_CALL( glGetProgramiv( m_id, GL_LINK_STATUS, &error ) );
    return error != GL_FALSE;
}

/*===========================================================================*/
/**
 *  @brief  Returns the location of a uniform variable.
 *  @param  name [in] name of the uniform variable
 *  @return location of a specific uniform variable
 */
/*===========================================================================*/
GLint ProgramObject::uniformLocation( const GLchar *name )
{
    GLint result = 0;
    KVS_GL_CALL( result = glGetUniformLocation( m_id, name ) );
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Returns the location of a attribute variable.
 *  @param  name [in] name of the attribute variable
 *  @return location of a specific attribute variable
 */
/*===========================================================================*/
GLint ProgramObject::attributeLocation( const GLchar *name )
{
//    return glGetAttribLocation( m_id, name );
    GLint result = 0;
    KVS_GL_CALL( result = glGetAttribLocation( m_id, name ) );
    return result;
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const GLint value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform1i( location, value ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 2D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector2i& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform2iv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 3D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector3i& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform3iv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 4D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector4i& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform4iv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const GLfloat value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform1f( location, value ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 2D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector2f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform2fv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 3D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector3f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform3fv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 4D vector
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Vector4f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniform4fv( location, 1, &value[0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 2x2 matrix
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Matrix22f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniformMatrix2fv( location, 1, GL_TRUE, &value[0][0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 3x3 matrix
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Matrix33f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniformMatrix3fv( location, 1, GL_TRUE, &value[0][0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  value [in] 4x4 matrix
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::Matrix44f& value )
{
    GLint location = this->uniformLocation( name );
    KVS_GL_CALL( glUniformMatrix4fv( location, 1, GL_TRUE, &value[0][0] ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies a uniform variable array.
 *  @param  name [in] name of the uniform variable array
 *  @param  values [in] value array
 *  @param  dim [in] dimension of the uniform variable array
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::ValueArray<GLint>& values, const int dim )
{
    GLint location = this->uniformLocation( name );
    GLsizei count = values.size() / dim;
    switch ( dim )
    {
    case 1: { KVS_GL_CALL( glUniform1iv( location, count, values.data() ) ); break; }
    case 2: { KVS_GL_CALL( glUniform2iv( location, count, values.data() ) ); break; }
    case 3: { KVS_GL_CALL( glUniform3iv( location, count, values.data() ) ); break; }
    case 4: { KVS_GL_CALL( glUniform4iv( location, count, values.data() ) ); break; }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Specifies a uniform variable array.
 *  @param  name [in] name of the uniform variable array
 *  @param  values [in] value array
 *  @param  dim [in] dimension of the uniform variable array
 */
/*===========================================================================*/
void ProgramObject::setUniform( const GLchar* name, const kvs::ValueArray<GLfloat>& values, const int dim )
{
    GLint location = this->uniformLocation( name );
    GLsizei count = values.size() / dim;
    switch ( dim )
    {
    case 1: { KVS_GL_CALL( glUniform1fv( location, count, values.data() ) ); break; }
    case 2: { KVS_GL_CALL( glUniform2fv( location, count, values.data() ) ); break; }
    case 3: { KVS_GL_CALL( glUniform3fv( location, count, values.data() ) ); break; }
    case 4: { KVS_GL_CALL( glUniform4fv( location, count, values.data() ) ); break; }
    default: break;
    }
}

/*===========================================================================*/
/**
 *  @brief  Sets an input type for geometry shader.
 *  @param  type [in] input type
 */
/*===========================================================================*/
void ProgramObject::setGeometryInputType( const GLint type )
{
    m_geom_input_type = type;
}

/*===========================================================================*/
/**
 *  @brief  Sets an output type for geometry shader.
 *  @param  type [in] output type
 */
/*===========================================================================*/
void ProgramObject::setGeometryOutputType( const GLint type )
{
    m_geom_output_type = type;
}

/*===========================================================================*/
/**
 *  @brief  Sets number of vertices for geometry shader.
 *  @param  value [in] number of vertices
 */
/*===========================================================================*/
void ProgramObject::setGeometryOutputVertices( const GLint value )
{
    m_geom_output_vertices = value;
}

/*===========================================================================*/
/**
 *  @brief  Creates a program object.
 */
/*===========================================================================*/
void ProgramObject::createID()
{
    if ( !this->isValid() )
    {
        KVS_GL_CALL( m_id = glCreateProgram() );
    }
}

/*===========================================================================*/
/**
 *  @brief  Deletes a program object.
 */
/*===========================================================================*/
void ProgramObject::deleteID()
{
    if ( this->isValid() )
    {
        this->detach();
        KVS_GL_CALL( glDeleteProgram( m_id ) );
    }
    m_id = 0;
}

/*===========================================================================*/
/**
 *  @brief  Sets a prameter to a program object.
 *  @param  pname [in] name of the parameter
 *  @param  value [in] value of the parameter
 */
/*===========================================================================*/
void ProgramObject::setParameter( GLenum pname, GLint value )
{
    KVS_ASSERT( this->isCreated() );

#if defined( KVS_PLATFORM_MACOSX ) && !defined( KVS_SUPPORT_OSMESA )
#if defined( GL_EXT_geometry_shader4 )
    KVS_GL_CALL( glProgramParameteriEXT( this->id(), pname, value ) );
#elif defined( GL_ARB_geometry_shader4 )
    KVS_GL_CALL( glProgramParameteriARB( this->id(), pname, value ) );
#elif defined( GL_ARB_get_program_binary )
    KVS_GL_CALL( glProgramParameteri( this->id(), pname, value ) );
#endif
#else
#if defined( GL_ARB_get_program_binary )
    KVS_GL_CALL( glProgramParameteri( this->id(), pname, value ) );
#elif defined( GL_ARB_geometry_shader4 )
    KVS_GL_CALL( glProgramParameteriARB( this->id(), pname, value ) );
#elif defined( GL_EXT_geometry_shader4 )
    KVS_GL_CALL( glProgramParameteriEXT( this->id(), pname, value ) );
#endif
#endif
}

/*===========================================================================*/
/**
 *  @brief  Constructs a program object binder.
 *  @param  po [in] program object
 */
/*===========================================================================*/
ProgramObject::Binder::Binder( const ProgramObject& po ) :
    m_po( po )
{
    KVS_ASSERT( po.isCreated() );
    po.bind();
}

/*===========================================================================*/
/**
 *  @brief  Destroys a program object binder.
 */
/*===========================================================================*/
ProgramObject::Binder::~Binder()
{
    KVS_ASSERT( m_po.isCreated() );
    KVS_GL_CALL( glUseProgram( 0 ) );
}


// DEPRECATED
bool ProgramObject::link(
    const kvs::VertexShader &vertex_shader,
    const kvs::FragmentShader &fragment_shader )
{
    this->create();
    this->attach( fragment_shader );
    this->attach( vertex_shader );
    return this->link();
}

// DEPRECATED
void ProgramObject::create(
    const kvs::ShaderSource& vertex_source,
    const kvs::ShaderSource& fragment_source )
{
    // Vertex shader.
    kvs::VertexShader vertex_shader( vertex_source );
    if ( !vertex_shader.compile() )
    {
        kvsMessageError() << "Vertex shader compile failed.\n" << vertex_shader.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Vertex shader compile failed" );
    }

    // Fragment shader.
    kvs::FragmentShader fragment_shader( fragment_source );
    if ( !fragment_shader.compile() )
    {
        kvsMessageError() << "Fragment shader compile failed.\n" << fragment_shader.log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Fragment shader compile failed" );
    }

    // Link the shaders.
    if ( !this->link( vertex_shader, fragment_shader ) )
    {
        kvsMessageError() << "Program object link failed.\n" << this->log() << std::endl;
        KVS_THROW( kvs::OpenGLException, "Program object link failed" );
    }
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const GLint v0 )
{
    KVS_GL_CALL( glUniform1i( this->uniformLocation( name ), v0 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const GLint v0,
    const GLint v1 )
{
    KVS_GL_CALL( glUniform2i( this->uniformLocation( name ), v0, v1 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 *  @param  v2 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const GLint v0,
    const GLint v1,
    const GLint v2 )
{
    KVS_GL_CALL( glUniform3i( this->uniformLocation( name ), v0, v1, v2 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 *  @param  v2 [in] value for the uniform variable
 *  @param  v3 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const GLint v0,
    const GLint v1,
    const GLint v2,
    const GLint v3 )
{
    KVS_GL_CALL( glUniform4i( this->uniformLocation( name ), v0, v1, v2, v3 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (2 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const kvs::Vector2i& v )
{
    const GLint v0 = static_cast<GLint>( v.x() );
    const GLint v1 = static_cast<GLint>( v.y() );
    KVS_GL_CALL( glUniform2i( this->uniformLocation( name ), v0, v1 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (3 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const kvs::Vector3i& v )
{
    const GLint v0 = static_cast<GLint>( v.x() );
    const GLint v1 = static_cast<GLint>( v.y() );
    const GLint v2 = static_cast<GLint>( v.z() );
    KVS_GL_CALL( glUniform3i( this->uniformLocation( name ), v0, v1, v2 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as integer.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (4 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuei(
    const GLchar* name,
    const kvs::Vector4i& v )
{
    const GLint v0 = static_cast<GLint>( v.x() );
    const GLint v1 = static_cast<GLint>( v.y() );
    const GLint v2 = static_cast<GLint>( v.z() );
    const GLint v3 = static_cast<GLint>( v.w() );
    KVS_GL_CALL( glUniform4i( this->uniformLocation( name ), v0, v1, v2, v3 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const GLfloat v0 )
{
    KVS_GL_CALL( glUniform1f( this->uniformLocation( name ), v0 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const GLfloat v0,
    const GLfloat v1 )
{
    KVS_GL_CALL( glUniform2f( this->uniformLocation( name ), v0, v1 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 *  @param  v2 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const GLfloat v0,
    const GLfloat v1,
    const GLfloat v2 )
{
    KVS_GL_CALL( glUniform3f( this->uniformLocation( name ), v0, v1, v2 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the value of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v0 [in] value for the uniform variable
 *  @param  v1 [in] value for the uniform variable
 *  @param  v2 [in] value for the uniform variable
 *  @param  v3 [in] value for the uniform variable
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const GLfloat v0,
    const GLfloat v1,
    const GLfloat v2,
    const GLfloat v3 )
{
    KVS_GL_CALL( glUniform4f( this->uniformLocation( name ), v0, v1, v2, v3 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (2 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const kvs::Vector2f& v )
{
    const GLfloat v0 = static_cast<GLfloat>( v.x() );
    const GLfloat v1 = static_cast<GLfloat>( v.y() );
    KVS_GL_CALL( glUniform2f( this->uniformLocation( name ), v0, v1 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (3 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const kvs::Vector3f& v )
{
    const GLfloat v0 = static_cast<GLfloat>( v.x() );
    const GLfloat v1 = static_cast<GLfloat>( v.y() );
    const GLfloat v2 = static_cast<GLfloat>( v.z() );
    KVS_GL_CALL( glUniform3f( this->uniformLocation( name ), v0, v1, v2 ) );
}

/*===========================================================================*/
/**
 *  @brief  Specifies the values of a uniform variable as floating point.
 *  @param  name [in] name of the uniform variable
 *  @param  v [in] vector value (4 components)
 */
/*===========================================================================*/
void ProgramObject::setUniformValuef(
    const GLchar* name,
    const kvs::Vector4f& v )
{
    const GLfloat v0 = static_cast<GLfloat>( v.x() );
    const GLfloat v1 = static_cast<GLfloat>( v.y() );
    const GLfloat v2 = static_cast<GLfloat>( v.z() );
    const GLfloat v3 = static_cast<GLfloat>( v.w() );
    KVS_GL_CALL( glUniform4f( this->uniformLocation( name ), v0, v1, v2, v3 ) );
}

} // end of namespace kvs
