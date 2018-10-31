#include "ip_utils.h"
#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>



void add_val(std::vector<int>& vec, std::string val){
    vec.push_back(std::stoi(val));
}

void add_val(std::vector<std::string>& vec, std::string val){
    vec.push_back(val);
}

template <typename T>
std::vector<T> split(const std::string &str, char d)
{
    std::vector<T> r;
    r.reserve(4);
    std::string::size_type start = 0;
    std::string::size_type stop = str.find_first_of(d);
    while(stop != std::string::npos)
    {        
        add_val(r,str.substr(start, stop - start));
        start = stop + 1;
        stop = str.find_first_of(d, start);
    }            
    add_val(r,str.substr(start));
    return r;
}

std::vector<std::string> split_input(const std::string &str){
    return split<std::string>(str,'\t');
}

std::vector<int> split_ip(const std::string &str){
    return split<int>(str,'.');
}


int main(){    
    try
    {
        ip_storage ip_pool;
        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split_input(line);
            ip_pool.push_back(split_ip(v.at(0)));
        }
        std::sort(ip_pool.begin(), ip_pool.end(), std::greater<std::vector<int>>());
        for( auto& ip: ip_pool ){
            iputils::print_ip(ip);
            std::cout<<std::endl;
        }
        /*auto filtered = iputils::filter(ip_pool,5,8);
        for( auto ip: filtered ){
            iputils::print_ip(ip);
            std::cout<<std::endl;
        }*/
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}
