#define BOOST_TEST_MODULE otusip_tests
#include "ip_utils.h"
#include <boost/test/unit_test.hpp>
BOOST_AUTO_TEST_SUITE(test_suite_main)
BOOST_AUTO_TEST_CASE(iputils_valid)
{
    std::string addr;
    int err_n;
    uint32_t addr_i;
    std::tie(addr, err_n) = iputils::getaddr("192.168.50.1\tanother\text");
    BOOST_CHECK(addr=="192.168.50.1");
    BOOST_CHECK(err_n==0);
    std::tie(addr_i,err_n) = iputils::strtoint(addr);
    BOOST_CHECK(addr_i==3232248321);
    BOOST_CHECK(err_n==0);
    std::tie(addr, err_n) = iputils::getaddr("192.168.50.1anotherext");
    BOOST_CHECK(err_n==-1);
    std::tie(addr, err_n) = iputils::getaddr("8.800.555.35.35\tanothertext");
    BOOST_CHECK(err_n==-2);
    std::tie(addr_i,err_n) = iputils::strtoint("https://g.gg");
    BOOST_CHECK(err_n==-1);
    std::tie(addr_i,err_n) = iputils::strtoint("8.800.555.35.35");
    BOOST_CHECK(err_n==-2);
    std::tie(addr_i,err_n) = iputils::strtoint("0.0.0.0.0.0");
    BOOST_CHECK(err_n==-3);

}
BOOST_AUTO_TEST_SUITE_END()
