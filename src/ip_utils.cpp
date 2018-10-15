#include "ip_utils.h"
#include <iostream>
std::tuple<uint32_t,int> iputils::strtoint(const std::string& in_addr){
    uint32_t ret = 0;
    int element_num=0;
    int val=0;
    for(const char& c: in_addr){
        if(c=='.'){
            element_num++;
            if(element_num > 3) return std::make_tuple(0,-3); //Too much elements
            ret=(ret<<8)|val;
            val=0;
            continue;
        }
        int num = c-'0';
        if( num >= 0 && num <=9  ){
            val = (val * 10)+num;
            if(val > 0xFF) return std::make_tuple(0,-2); //Wrong ip element
            continue;
        }
        return std::make_tuple(0,-1); //Wrong char
    }
    if(val > 0xFF) return std::make_tuple(0,-2); //Wrong ip element
    ret=(ret<<8)|val;
    return std::make_tuple(ret,0);

}
std::tuple<std::string, int> iputils::getaddr(const std::string& rcvd_str){
    size_t end_of_addr=rcvd_str.find_first_of('\t');
    if(end_of_addr != std::string::npos){
        std::string addr = rcvd_str.substr(0,end_of_addr);
        int err;
        uint32_t addr_i;
        std::tie(addr_i,err) = strtoint(addr);
        if(err != 0){
            return std::make_tuple(std::string(),-2);
        }
        return std::make_tuple(rcvd_str.substr(0,end_of_addr),0);
    }else{
        return std::make_tuple(std::string(),-1);
    }
}
