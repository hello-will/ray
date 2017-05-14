/**
 * @file    singleton.hpp
 * @version 1.0
 * @author  hello_will@yeah.net
 * @date    2016/09/21 11:19:55
**/

#ifndef __BASE_SINGLETON_HPP_20160921112023_F1A552D1_EE56_4FD5_A2B4_890F2149CDE6__
#define __BASE_SINGLETON_HPP_20160921112023_F1A552D1_EE56_4FD5_A2B4_890F2149CDE6__

#include "non_copyable.hpp"

namespace ray
{
    template <typename __type>
    class singleton : public non_copyable
    {
    public:
        /* get instance */
        static __type*& geti( void )
        {
            static __type *s_inst = nullptr;

            return s_inst;
        }

        /* set instance */
        static void seti( __type *inst )
        {
            geti() = inst;
        }
    };
} /* namespace ray */

#endif /* #ifndef __BASE_SINGLETON_HPP_20160921112023_F1A552D1_EE56_4FD5_A2B4_890F2149CDE6__ */