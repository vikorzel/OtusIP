#pragma once
#include <string>
#include <tuple>
#include <iostream>
#include <vector>

using ip_addr = std::vector<int>;
using ip_storage = std::vector<ip_addr>;

struct iputils{
private:
    template<typename T>
    static bool valid_ip(ip_addr& addr, int num, T pattern){
        static_assert(std::is_integral<T>::value,"Pattern should be integral");
        return (addr[num]==static_cast<int>(pattern));
    }

    template <typename T, typename ... Ts>
    static bool valid_ip(ip_addr& addr, int num, T pattern,  Ts ... another_patterns){
        static_assert(std::is_integral<T>::value,"Pattern should be integral");
        return (addr[num]==static_cast<int>(pattern))&&valid_ip(addr,num+1, another_patterns...);
    }


public:
    template<typename ... Ts>
    static ip_storage filter( ip_storage& ips, Ts ... patterns ){
        const int n = sizeof ... (Ts);
        static_assert(n <= 4,"Too much patterns");
        ip_storage r;
        for(auto ip: ips){
            if(valid_ip(ip, 0, patterns...)){
                r.push_back(ip);
            }
        }
        return r;
    }

    static ip_storage filter_any( ip_storage& ips, int pattern );
    static void print_ip( ip_addr&, std::ostream& stream = std::cout);
};
