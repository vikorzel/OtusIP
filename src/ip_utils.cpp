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

template<typename ... Ts>
ip_storage iputils::filter( ip_storage& ips, Ts ... patterns ){
    const int n = sizeof ... (Ts);
    static_assert(n <= 4,"Too much patterns");
    ip_storage ret_ips;
    for( auto ip = ips.rbegin(); ip != ips.rend(); ip++ ){
        if( valid_ip( *ip, 0 , patterns... ) ){
            ret_ips.push_back(*ip);
        }
    }
    return ret_ips;
}

template<typename T>
bool iputils::valid_ip(ip_addr& addr, int num, T pattern){
    static_assert(std::is_integral<T>::value,"Pattern should be integral");
    return (addr[num]==std::to_string(static_cast<int>(pattern)));
}

template <typename T, typename ... Ts>
bool iputils::valid_ip(ip_addr& addr, int num, T pattern,  Ts ... another_patterns){
    static_assert(std::is_integral<T>::value,"Pattern should be integral");
    return (addr[num]==std::to_string(static_cast<int>(pattern)))&&valid_ip(addr,num+1, another_patterns...);
}

ip_storage iputils::filter_any( ip_storage& ips, int filter_pattern ){
    ip_storage ret;
    for( auto ip = ips.rbegin() ; ip != ips.rend() ; ip++ ){
        for(auto& part: *ip){
            if( part == std::to_string(filter_pattern) ){
                ret.push_back(*ip);
                break;
            }
        }
    }
    return ret;
}

auto iputils::compare_ip(const ip_addr& ip1, const ip_addr& ip2){
    if(ip1.size() > 4 || ip2.size() > 4){
        return std::make_tuple(CompareResults::UNKNOWN,CompareErrors::WRONG_IPSIZE);
    }
    for( size_t i = 0 ; i != ip1.size(); i++ ){
        if(        (ip1[i].size() == 3 && ip1[i] > "255")
                || (ip1[i].size() == 3 && ip1[i] > "255")
                || ip1[i].size() > 3 || ip1[i].size() < 1
                || ip2[i].size() > 3 || ip2[i].size() < 1)
        {
            return std::make_tuple(CompareResults::UNKNOWN, CompareErrors::WRONG_IPPART);
        }
        if(ip1[i].size() > ip2[i].size()){
            return std::make_tuple(CompareResults::GREATER, CompareErrors::OK);
        }

        if(ip1[i].size() < ip2[i].size()){
            return std::make_tuple(CompareResults::LOWER, CompareErrors::OK);
        }
        if(ip1[i] > ip2[i]){
            return std::make_tuple(CompareResults::GREATER, CompareErrors::OK);
        }
        if(ip1[i] < ip2[i]){
            return std::make_tuple(CompareResults::LOWER, CompareErrors::OK);
        }
    }
    return std::make_tuple(CompareResults::EQUAL, CompareErrors::OK);
}

CompareErrors iputils::sort_ip_vec(ip_storage& vec, int fst, int lst){
    int l = fst;
    int r = lst;
    std::vector<std::string> base = vec[(lst+fst)/2];
    CompareErrors err;
    CompareResults res;
    do{
        do{
            std::tie(res,err) = compare_ip(vec[l++], base);
            if(err != CompareErrors::OK){
                return err;
            }
        }while( res == CompareResults::LOWER);
        l--;
        std::cout<<std::endl;
        do{
            std::tie(res,err) = compare_ip(vec[r--], base);
            if(err != CompareErrors::OK){
                return err;
            }
        }while( res == CompareResults::GREATER);
        r++;
        if(l <= r){
            std::tie(res,err) = compare_ip(vec[l],vec[r]);
            if(res == CompareResults::GREATER){
                std::iter_swap(vec.begin()+l,vec.begin()+r);
            }
            l++;
            r--;
        }
    }while(l <= r);

    if(l < lst){
        err = sort_ip_vec(vec,l,lst);
        if(err != CompareErrors::OK) return err;
    }
    if(fst < r){
        err = sort_ip_vec(vec,fst,r);
        if(err != CompareErrors::OK) return err;
    }
    return CompareErrors::OK;
}
