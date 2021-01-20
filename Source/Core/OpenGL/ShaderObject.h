/*****************************************************************************/
/**
 *  @file   ShaderObject.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/ShaderSource>
#include <kvs/GL>
#include <kvs/Deprecated>


namespace kvs
{

/*===========================================================================*/
/**
 *  Shader object class.
 */
/*===========================================================================*/
class ShaderObject
{
private:
    GLuint m_id; ///< shader ID
    GLenum m_type; ///< shader type (GL_VERTEX_SHADER, GL_GEOMETRY_SHADER or GL_FRAGMENT_SHADER)

public:
    ShaderObject( const GLenum type );
    virtual ~ShaderObject();

    GLuint id() const { return m_id; }
    std::string log() const;
    std::string source() const;
    void setSource( const kvs::ShaderSource& source ) const;

    void create();
    void release();
    bool compile() const;
    bool compile( const kvs::ShaderSource& source ) const;

    bool isCreated() const;
    bool isValid() const;
    bool isCompiled() const;

protected:
    ShaderObject();
    void createID();
    void deleteID();

public:
    KVS_DEPRECATED( bool create( const kvs::ShaderSource& source ) ) { this->create(); return this->compile( source ); }
    KVS_DEPRECATED( void clear() ) { this->release(); }
};

} // end of namespace kvs
