/*****************************************************************************/
/**
 *  @file   CheckBoxGroup.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#ifndef KVS__GLUT__CHECK_BOX_GROUP_H_INCLUDE
#define KVS__GLUT__CHECK_BOX_GROUP_H_INCLUDE

#include <list>
#include <kvs/IgnoreUnusedVariable>
#include <kvs/glut/WidgetBase>
#include "CheckBox.h"
#include <kvs/Deprecated>


namespace kvs
{

class ScreenBase;

namespace glut
{

class CheckBox;

/*===========================================================================*/
/**
 *  @brief  Check box group class.
 */
/*===========================================================================*/
class CheckBoxGroup : public kvs::glut::WidgetBase
{
public:
    typedef kvs::glut::WidgetBase BaseClass;

protected:
    std::list<kvs::glut::CheckBox*> m_boxes; ///< check box list

public:
    KVS_DEPRECATED( CheckBoxGroup( kvs::ScreenBase* screen = 0 ) );

    virtual void pressed( kvs::glut::CheckBox* box ) { kvs::IgnoreUnusedVariable( box ); };
    virtual void pressed( int id ) { kvs::IgnoreUnusedVariable( id ); };
    virtual void released( kvs::glut::CheckBox* box ) { kvs::IgnoreUnusedVariable( box ); };
    virtual void released( int id ) { kvs::IgnoreUnusedVariable( id ); };
    virtual void screenUpdated() {};
    virtual void screenResized() {};

    const std::list<kvs::glut::CheckBox*>& checkBoxes() const { return m_boxes; }
    void add( kvs::glut::CheckBox* box );
    void remove( kvs::glut::CheckBox* box );
    void show();
    void hide();

private:
    void paintEvent();
    void resizeEvent( int width, int height );
    void mousePressEvent( kvs::MouseEvent* event );
    void mouseReleaseEvent( kvs::MouseEvent* event );
};

} // end of namespace glut

} // end of namespace kvs

#endif // KVS__GLUT__CHECK_BOX_GROUP_H_INCLUDE
