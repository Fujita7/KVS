/*****************************************************************************/
/**
 *  @file   PolygonRendererGLSL.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/DebugNew>
#include <kvs/Module>
#include <kvs/PolygonObject>
#include <kvs/Shader>
#include <kvs/ProgramObject>
#include <kvs/VertexBufferObjectManager>
#include <kvs/Deprecated>
#include <string>


namespace kvs
{

namespace glsl
{

/*===========================================================================*/
/**
 *  @brief  Polygon renderer class.
 */
/*===========================================================================*/
class PolygonRenderer : public kvs::PolygonRenderer
{
    kvsModule( kvs::glsl::PolygonRenderer, Renderer );
    kvsModuleBaseClass( kvs::PolygonRenderer );

private:
    std::string m_vert_file; ///< vertex shader file
    std::string m_frag_file; ///< fragment shader file
    size_t m_width; ///< window width
    size_t m_height; ///< window height
    const kvs::ObjectBase* m_object; ///< pointer to the rendering object
    float m_polygon_offset; ///< polygon offset
    kvs::Shader::ShadingModel* m_shading_model; ///< shading method
    kvs::ProgramObject m_shader_program; ///< shader program
    kvs::VertexBufferObjectManager m_vbo_manager; ///< vertex buffer object manager

public:
    PolygonRenderer();
    virtual ~PolygonRenderer();

    void exec( kvs::ObjectBase* object, kvs::Camera* camera, kvs::Light* light );

    const std::string& vertexShaderFile() const { return m_vert_file; }
    const std::string& fragmentShaderFile() const { return m_frag_file; }
    float polygonOffset() const { return m_polygon_offset; }

    void setVertexShaderFile( const std::string& vert_file ) { m_vert_file = vert_file; }
    void setFragmentShaderFile( const std::string& frag_file ) { m_frag_file = frag_file; }
    void setPolygonOffset( const float offset ) { m_polygon_offset = offset; }

    void setShaderFiles( const std::string& vert_file, const std::string& frag_file )
    {
        this->setVertexShaderFile( vert_file );
        this->setFragmentShaderFile( frag_file );
    }

    template <typename Model>
    void setShadingModel( const Model model )
    {
        if ( m_shading_model ) { delete m_shading_model; m_shading_model = NULL; }
        m_shading_model = new Model( model );
        if ( !m_shading_model )
        {
            kvsMessageError("Cannot create a specified shading model.");
        }
    }

    template <typename ShadingType>
    KVS_DEPRECATED( void setShader( const ShadingType shader ) ) { this->setShadingModel<ShadingType>( shader ); }

protected:
    kvs::Shader::ShadingModel& shadingModel() { return *m_shading_model; }
    kvs::ProgramObject& shader() { return m_shader_program; }
    kvs::VertexBufferObjectManager& vboManager() { return m_vbo_manager; }

    bool isWindowCreated() { return m_width == 0 && m_height == 0; }
    bool isWindowResized( size_t w, size_t h ) { return m_width != w || m_height != h; }
    bool isObjectChanged( const kvs::ObjectBase* o ) { return m_object != o; }
    void setWindowSize( size_t w, size_t h ) { m_width = w; m_height = h; }
    void attachObject( const kvs::ObjectBase* o ) { m_object = o; }

    void createShaderProgram();
    void updateShaderProgram();
    void setupShaderProgram();
    void createBufferObject( const kvs::PolygonObject* polygon );
    void updateBufferObject( const kvs::PolygonObject* polygon );
    void drawBufferObject( const kvs::PolygonObject* polygon );
};

} // end of namespace glsl

} // end of namespace kvs
