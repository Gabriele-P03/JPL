/**
 * @file speed.hpp
 * @author Gabriele-P03
 * @date 2025-05-25
 * 
 * In this header file will be declared inline functions useful for calculating speed and its components.
 * 
 * Direct and Inverse formula both calculate components using m/s
 */

#include <cmath>

namespace jpl{
    namespace _physic{
        namespace _kinematics{

            /**
             * @param d distance (m)
             * @param t time (s)
             * @return speed 
             */
            inline float speed_d_t(float d, float t) noexcept{
                return d/t;
            } 
            /**
             * @param s speed (m/s)
             * @param t time (s)
             * @param d0 distance already travelled (m)
             * @return distance 
            */
            inline float distance_s_t(float s, unsigned int t, float d0 = 0) noexcept{
                return s*t+d0;
            }
            /**
             * @param s speed (m/s)
             * @param t distance (m)
             * @param t0 time already spent (s)
             * @return time 
            */
            inline float time_s_d(float s, float d, float t0 = 0) noexcept{
                return d/s + t0;
            }

            /**
             * @param km_h speed calculated at km/h
             * @return speed calculated at m/s
             */
            inline float m_s(float km_h){
                return km_h/3.6;
            }
            /**
             * @param km_h speed calculated at m/s
             * @return speed calculated at km/h
             */
            inline float km_h(float m_s){
                return m_s*3.6;
            }
        } 
    }
}

