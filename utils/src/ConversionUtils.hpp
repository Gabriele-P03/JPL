/**
 * @file 
 * This header file contains some function about type-conversion.
 *
 * Type Legend:
 * 
 * 	- cc: 		const char*
 * 	- int/i: 	integer
 * 	- uint: 	unsigned integer
 * 	- d: 		double
 * 	- f:		float
 * 	- b:		boolean
 * 	- bit:		boolean(bit)
 *
 * @date 2023-02-04
 * @copyright Copyright (c) 2023
 * @author Gabriele-P03
 */

#ifndef CONVERTION_UTILS_JPL
#define CONVERTION_UTILS_JPL

#include <string.h>
#include <jpl/exception/runtime/IllegalArgumentException.hpp>
#include "StringUtils.hpp"

namespace jpl
{

	namespace _utils
	{

		namespace _conversions
		{

			inline void ulong_long(unsigned long src, long *& dst){
				memcpy(dst, &src, sizeof(long));
			}
			/**
			 * Converts the given integer (src) into a const char
			 * 
			 * @param src 
			 * @param dst const char of size(int) already allocated
			 */
			inline void int_cc(int src, const char *&dst)
			{
				memcpy((void *)&dst[0], &src, sizeof(int));
			}
			/**
			 * Converts the given const char* (src) into a integer
			 * 
			 * @param src of sizeof(int)
			 * @param dst 
			 */
			inline void cc_int(const char *src, int &dst)
			{
				memcpy(&dst, src, sizeof(int));
			}

			/**
			 * Converts the given unsigned integer (src) into a const char
			 * 
			 * @param src 
			 * @param dst const char of size(unsigned int) already allocated
			 */
			inline void uint_cc(unsigned int src, const char *&dst)
			{
				memcpy((void *)&dst[0], &src, sizeof(unsigned int));
			}
			/**
			 * Converts the given const char* (src) into an unsigned integer
			 * 
			 * @param src of sizeof(unsigned int)
			 * @param dst 
			 */
			inline void cc_uint(const char *src, unsigned int &dst)
			{
				memcpy(&dst, src, sizeof(unsigned int));
			}

			/**
			 * Converts the given float (src) into a const char
			 * 
			 * @param src 
			 * @param dst const char of size(float) already allocated
			 */
			inline void f_cc(float src, const char *&dst)
			{
				memcpy((void *)&dst[0], &src, sizeof(float));
			}
			/**
			 * Converts the given const char* (src) into a float
			 * 
			 * @param src of sizeof(float)
			 * @param dst 
			 */
			inline void cc_f(const char *src, float &dst)
			{
				memcpy(&dst, src, sizeof(float));
			}

			/**
			 * Converts the given double (src) into a const char
			 * 
			 * @param src 
			 * @param dst const char of size(double) already allocated
			 */
			inline void d_cc(double src, const char *&dst)
			{
				memcpy((void *)&dst[0], &src, sizeof(double));
			}
			/**
			 * Converts the given const char* (src) into a double
			 * 
			 * @param src of sizeof(double)
			 * @param dst 
			 */
			inline void cc_d(const char *src, double &dst)
			{
				memcpy(&dst, src, sizeof(double));
			}

			/**
			 * Store into a char the given bit (seen as boolean value)
			 *
			 * @param src
			 * @param offset index of the digit into returned char
			 * @return char (if offset is greater than 6, it returns 0x00 )
			 */
			inline char b_cc(bool src, unsigned short offset)
			{
				if (offset > 6)
				{
					return 0x00;
				}

				char tmp = 0x00;
				tmp |= src;
				tmp = tmp << offset;
				return tmp;
			}
			/**
			 * It checks if a certain bit into the given src is set to 1
			 *
			 * @param src
			 * @param dst
			 * @param offset right shift which represents the index of the bit to check into src (if it is greater than 7, false is returned)
			 */
			inline bool cc_b(const char *src, bool &dst, unsigned short offset)
			{
				if (offset > 7)
					return false;

				return (((src[0] >> offset) << (sizeof(char) * 8 - 1 - offset)) == 0x01);
			}

			/**
			 * It retrieves two integers number from the given _src
			 *
			 * The first one is got from the 4 most left digits as well the second one (4 most right)
			 * @param _src
			 * @return int[2] the first one (left digits) and second one (right digits)
			 */
			inline int *split_cc_ii(const char _src)
			{
				return new int[2]{((_src >> 4) & 0xFF), (_src & 0x0F)};
			}

			/**
			 * It converts the src into an array of bits (seen as boolean array)
			 *
			 * @param src
			 * @param _size size of src
			 * @return boolean array (its size will be _size*8)
			 */
			inline bool *cc_bits(const char *src, unsigned int _size)
			{
				bool *buffer = new bool[_size * 8];
				for (unsigned int i = 0; i < _size; i++)
				{
					char current = src[i];
					for (unsigned int k = 0; k < 8; k++)
					{
						buffer[_size * 8 - 1 - (i * 8 + k)] = (((current >> k) & 0x01) == 0x01);
					}
				}
				return buffer;
			}
			/**
			 * It converts a bits array (seen as boolean array) into a const char array
			 * Each 8 bits (a byte) is stored into a char
			 *
			 * @param src
			 * @param _size size of src
			 * @return const char* (its size is _size/8). If _size mod 8 is not 0, it returns nullptr
			 */
			inline char *bits_cc(bool *src, unsigned int _size)
			{
				if (_size % 8 != 0)
				{
					return nullptr;
				}

				char *buffer = new char[_size / 8];
				for (unsigned int i = 0; i < _size / 8; i++)
				{

					char *current = &buffer[i];
					for (unsigned int k = 0; k < 8; k++)
					{
						*current |= src[i * 8 + k];
						*current << 1;
					}
				}
				return buffer;
			}

			/**
			 * @param value 
			 * @return true if value is equal to either 1 or true
			 * @return false if value is equal to either 0 or false
			 * @throw IllegalArgumentException if value was not a valid value
			 */
			inline bool str_bool(std::string value){
				if(value.compare("1") || jpl::_utils::_string::toLower(value).compare("true") == 0){
					return true;
				}else if(value.compare("0") || jpl::_utils::_string::toLower(value).compare("false") == 0){
					return false;
				}
				throw jpl::_exception::IllegalArgumentException(value + " could not be parsed as boolean value");
			}
		}
	}
}

#endif