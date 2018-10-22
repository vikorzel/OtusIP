#include <iostream>
#include <set>
#include "ip_utils.h"
#include <tuple>

int main(){
    auto cmp = [](const std::string& str1, const std::string& str2) -> bool{
        int err;
        uint32_t s1_i, s2_i;
        std::tie(s1_i,err)=iputils::strtoint(str1);
        std::tie(s2_i,err)=iputils::strtoint(str2);
        return s1_i>s2_i;
    };

    auto addrs = std::set<std::string,decltype(cmp)>(cmp);

    for(std::string line; std::getline(std::cin, line);){
        std::string addr;
        int err_code;
        std::tie(addr,err_code) = iputils::getaddr(line);
        if(err_code == 0){
            addrs.insert(std::move(addr));
        }
    }
    for(const std::string& addr: addrs){
        std::cout<<addr<<std::endl;
    }

    return 0;
}
