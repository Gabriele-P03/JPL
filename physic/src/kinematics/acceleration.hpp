/**
 * @file speed.hpp
 * @author Gabriele-P03
 * @date 2025-05-25
 * 
 * In this header file will be declared inline functions useful for calculating acceleration and its components.
 * 
 * Direct and Inverse formula both calculate components using m/s^2
 */

#include <cmath>

namespace jpl{
    namespace _physic{
        namespace _kinematics{

            /**
             * @param s speed (m)
             * @param t time (s)
             * @return acceleration 
             */
            inline float acceleration_s_t(float s, float t) noexcept{
                return s/t;
            } 

            /**
             * @param a acceleration (m/s^s)
             * @param t time (s)
             * @param v0 speed at time 0
             * @return speed 
            */
            inline float speed_a_t(float a, unsigned int t, float v0 = 0) noexcept{
                return a*t+v0;
            }
            /**
             * @param a acceleration (m/s^2)
             * @param t speed (m/s)
             * @param t0 time already spent (s)
             * @return time 
            */
            inline float time_a_v(float a, float v, float t0 = 0) noexcept{
                return v/a + t0;
            }

            /**
             * @brief Equation of Motion (a*t*t)/2 + v*t + d0 
             *
             * @param a acceleration (m/s^2)
             * @param v speed (m/s)
             * @param t time (s)
             * @param d0 distance already travelled 
             * @return distance
             */
            inline float distance_a_v_t(float a, float s, float t, float d0){
                return (a*t*t)/2 + s*t + d0;
            }
        } 
    }
}

