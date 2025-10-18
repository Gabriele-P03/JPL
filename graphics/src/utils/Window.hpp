
#ifndef WINDOWUTILS_GRAPHICS_JPL
#define WINDOWUTILS_GRAPHICS_JPL

#include <GLFW/glfw3.h>
#include <jpl/exception/runtime/IllegalArgumentException.hpp>
#include "Error.hpp"

namespace jpl{
    namespace _graphics{

        namespace _metrics{
            //Window width
            extern unsigned int monitorWidth;
            //Window height
            extern unsigned int monitorHeight;
        }

        namespace _states{
            /*
                True if window is in fullscreen.
                It is updated by setCurrentWindowToWindowMode() and setCurrentWindowToFullScreen()
            */
            extern bool fullscreen;
        }
        
        namespace _utils{

            /**
             * @throw RuntimeException if current window could not be retrieved
             * @return the current context window
             */
            extern GLFWwindow* getCurrentContext();

            /**
             * @throw RuntimeException if current window could not be retrieved
             * @throw RuntimeException if monitor could not be retrieved
             * @return monitor of current context window
             */
            extern GLFWmonitor* getCurrentMonitor();

            /**
             * @param w window
             * @throw IllegalArgumentException if w in null
             * @throw RuntimeException if monitor could not be retrieved
             * @return monitor where w is located
             */
            extern GLFWmonitor* getMonitorByWindow(GLFWwindow* w);

            /**
             * @throw RuntimeException if current window could not be retrieved
             * @throw RuntimeException if monitor could not be retrieved
             * @throw RuntimeException if vidmode could not be retrieved
             * @return vidmode of current monitor
             */
            extern const GLFWvidmode* getVidmodeCurrentMonitor();
            /**
             * @param m monitor
             * @throw IllegalArgumentException if m is null
             * @throw RuntimeException if vidmode could not be retrieved
             * @return vidmode of monitor m
             */
            extern const GLFWvidmode* getVidmodeByMonitor(GLFWmonitor* m);

            /**
             * Set current window to fullscreen mode
             * @throw RuntimeException if any GLFW error occurred
             */
            extern void setCurrentWindowToFullScreen();
            /**
             * Set current window to windowed mode
             * @throw RuntimeException if any GLFW error occurred
             */
            extern void setCurrentWindowToWindowMode(); 

            extern void destroyWindow(GLFWwindow* w); 
        }
    }
}

#endif