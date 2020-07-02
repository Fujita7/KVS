/*****************************************************************************/
/**
 *  @file   ScreenBase.h
 *  @author Naohisa Sakamoto
 */
/*****************************************************************************/
#pragma once
#include <kvs/Timer>
#include <kvs/ScreenBase>


namespace kvs
{

class MouseEvent;
class WheelEvent;
class KeyEvent;

namespace glut
{

class Application;
class Timer;

/*===========================================================================*/
/**
 *  @brief  GLUT screen base class.
 */
/*===========================================================================*/
class ScreenBase : public kvs::ScreenBase
{
    using BaseClass = kvs::ScreenBase;

public:
    static ScreenBase* DownCast( kvs::ScreenBase* screen );
    static const ScreenBase* DownCast( const kvs::ScreenBase* screen );

private:
    int m_id; ///< window ID
    kvs::MouseEvent* m_mouse_event; ///< mouse event
    kvs::KeyEvent* m_key_event; ///< key event
    kvs::WheelEvent* m_wheel_event; ///< wheel event
    kvs::Timer m_elapse_time_counter; ///< elapse time counter for double click event
    bool m_is_fullscreen; ///< check flag whether the window is fullscreen

public:
    ScreenBase( kvs::glut::Application* application );
    virtual ~ScreenBase();

    int id() const { return m_id; }

    virtual void setEvent( kvs::EventListener* event, const std::string& name = "" );
    virtual void addEvent( kvs::EventListener* event, const std::string& name = "" );

    virtual void create();
    virtual void show();
    virtual void hide();
    virtual void showFullScreen();
    virtual void showNormal();
    virtual void popUp();
    virtual void pushDown();
    virtual void redraw();
    virtual void resize( int width, int height );
    virtual void draw();
    virtual kvs::ColorImage capture() const;
    virtual bool isFullScreen() const;
    virtual void enable() {}
    virtual void disable() {}
    virtual void reset() {}

    virtual void initializeEvent() {}
    virtual void paintEvent() {}
    virtual void resizeEvent( int width, int height ) {}
    virtual void mousePressEvent( kvs::MouseEvent* event ) {}
    virtual void mouseMoveEvent( kvs::MouseEvent* event ) {}
    virtual void mouseReleaseEvent( kvs::MouseEvent* event ) {}
    virtual void mouseDoubleClickEvent( kvs::MouseEvent* event ) {}
    virtual void wheelEvent( kvs::WheelEvent* event ) {}
    virtual void keyPressEvent( kvs::KeyEvent* event ) {}
    virtual void keyRepeatEvent( kvs::KeyEvent* event ) {}
    virtual void keyReleaseEvent( kvs::KeyEvent* event ) {}

private:
    // Callback functions for GLUT.
    friend void DisplayFunction();
    friend void ResizeFunction( int width, int height );
    friend void MouseFunction( int button, int state, int x, int y );
    friend void MouseMoveFunction( int x, int y );
    friend void KeyPressFunction( unsigned char key, int x, int y );
    friend void SpecialKeyPressFunction( int key, int x, int y );
    friend void KeyReleaseFunction( unsigned char key, int x, int y );
    friend void SpecialKeyReleaseFunction( int key, int x, int y );
};

} // end of namespace glut

} // end of namespace kvs
