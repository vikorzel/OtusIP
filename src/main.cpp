#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>
#include "ip_utils.h"

std::vector<std::string> split(const std::string &str, char d)
{
    std::vector<std::string> r;
    r.reserve(4);
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {
        r.push_back(str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }
    r.push_back(str.substr(start));

    return r;
}
int main(){    
    try
    {
        ip_storage ip_pool;
        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        auto err = iputils::sort_ip_vec(ip_pool,0,ip_pool.size()-1);
        if(err == CompareErrors::OK){
            for(auto ip = ip_pool.rbegin(); ip != ip_pool.rend(); ++ip)
            {
                for(ip_addr::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
                {                   
                    std::cout << (ip_part != ip->cbegin() ? "." : "") << *ip_part;
                }
                std::cout << std::endl;
            }
        }else{
            std::cout<<"ErrNo: "<< static_cast<std::underlying_type<CompareErrors>::type>(err)<<std::endl;
            return -1;
        }
        /*
        auto filtered_ips = iputils::filter_any(ip_pool,46); //iputils::filter(ip_pool,1,2);
        for(auto& ip: filtered_ips){
            for(auto part = ip.cbegin(); part != ip.cend(); part++){                
                std::cout<<(part != ip.cbegin())?",":""<<*part;
            }
            std::cout<<std::endl;
        }
        */
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
