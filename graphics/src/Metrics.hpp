/**
 * In this header file will be declared all variables which will
 * be used to store information about resolution and monitor size
 */

#ifndef METRICS_GRAPHICS_JPL
#define METRICS_GRAPHICS_JPL

#include "utils/Window.hpp"

#ifdef _WIN32
    #include <windows.h>
#else
#endif

namespace jpl{
    namespace _graphics{
        namespace _metrics{

            //Window width
            extern unsigned int width;
            //Window height
            extern unsigned int height;

            //Viewport width
            extern unsigned int viewportWidth;
            //Viewport height
            extern unsigned int viewportHeight;

            //Window xpos
            extern unsigned int x;
            //Window ypos
            extern unsigned int y;

            //Monitor width
            extern unsigned int monitorWidth;
            //Monitor height
            extern unsigned int monitorHeight;

            extern unsigned int getMonitorWidth();
            extern unsigned int getMonitorHeight();

            /**
             * Update width and height fields once window has been resized.
             * This function should be called by callback
             */
            extern void windowSizeCallback(GLFWwindow* window, int width, int height);
        
            /**
             * Update viewport's width and height fields once window has been resized.
             * This function should be called by callback
             */
            extern void windowViewportSizeCallback(GLFWwindow* window, int width, int height);

            /**
             * Update window's position once moved.
             * This function should be called by callback
             */
            extern void windowPositionCallback(GLFWwindow* window, int x, int y);

            inline float XPosToScreenCoords(float x){
                float halfW = ((float)jpl::_graphics::_metrics::width)/2;
                return (x-halfW)/halfW;
            }
            inline float YPosToScreenCoords(float y){
                float halfH = ((float)jpl::_graphics::_metrics::height)/2;
                return (y-halfH)/halfH;
            }
        }

    }
}

#endif