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

    ip_storage test_vec;
    CompareErrors err;
    test_vec.reserve(3);


    test_vec.push_back(ip_addr{"8","8","8","8"});
    test_vec.push_back(ip_addr{"192","168","50","1"});


    err = iputils::sort_ip_vec(test_vec,0,1);
    BOOST_CHECK(err == CompareErrors::OK);
    BOOST_CHECK(test_vec[1][0]=="192");
    BOOST_CHECK(test_vec[0][0]=="8");
    BOOST_CHECK(test_vec.size() == 2);

    test_vec.push_back(ip_addr{"256","168","50","1"});
    err = iputils::sort_ip_vec(test_vec,0,2);
    BOOST_CHECK(err==CompareErrors::WRONG_IPPART);
    test_vec.pop_back();
    test_vec.pop_back();
    test_vec.pop_back();

    test_vec.push_back(ip_addr{"2561","168","50","1","3"});
    err = iputils::sort_ip_vec(test_vec,0,0);
    BOOST_CHECK(err==CompareErrors::WRONG_IPSIZE);
}
BOOST_AUTO_TEST_SUITE_END()
