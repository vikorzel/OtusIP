#define BOOST_TEST_MODULE otusip_tests
#include "ip_utils.h"
#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE(test_suite_main)
BOOST_AUTO_TEST_CASE(iputils_valid)
{
    ip_storage storage = { {123,45,67,89}, {12,85,0,0}, {77,77,89,7}, {77,7,7,7} };
    ip_storage any = iputils::filter_any(storage,89);
    ip_storage single = iputils::filter(storage,12);
    ip_storage multi = iputils::filter(storage,77);
    ip_storage dbl_filter = iputils::filter(storage,77,77);

    BOOST_CHECK( single.size() == 1 );
    BOOST_CHECK( any.size() == 2 );
    BOOST_CHECK( multi.size() == 2);
    BOOST_CHECK( dbl_filter.size() == 1 );

    BOOST_CHECK(( single[0] == ip_addr{ 12,85,0,0 } ));
    BOOST_CHECK(( dbl_filter[0] == ip_addr{77,77,89,7} ));

    bool has_fst, has_snd;
    has_fst=has_snd=false;
    for( auto ip: any ){
        if( ip == ip_addr{123,45,67,89} ) has_fst = true;
        if( ip == ip_addr{77,77,89,7} ) has_snd = true;
    }
    BOOST_CHECK( has_fst && has_snd );

    has_fst=has_snd=false;
    for( auto ip: multi ){
        if( ip == ip_addr{77,7,7,7} ) has_fst = true;
        if( ip == ip_addr{77,77,89,7} ) has_snd = true;
    }



}
BOOST_AUTO_TEST_SUITE_END()
