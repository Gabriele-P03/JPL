/**
 * In this header file will be declared most of all method which can be
 * used to calculate mathematical error
 * 
 * @author Gabriele-P03
 */

#include <cmath>
#include <jpl/exception/runtime/IllegalArgumentException.hpp>

namespace jpl{
    namespace _math{
        namespace _error{

            /**
             * @param a approximation value of x
             * @param x
             * @return abosule error 
             */
            inline double absolute(double a, double x){
                return abs(a-x);
            }

            inline double relative(double a, double x){
                if(x == 0.0){
                    throw jpl::_exception::IllegalArgumentException("Division by 0 is not allowed");
                }
                return absolute(a,x)/abs(x);
            }
        }
    }
}