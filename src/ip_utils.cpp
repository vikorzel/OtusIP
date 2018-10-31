#include <iostream>
#include "ip_utils.h"


ip_storage iputils::filter_any( ip_storage& ips, int filter_pattern ){
    ip_storage ret;
    for( auto ip = ips.cbegin() ; ip != ips.cend() ; ip++ ){
        for(auto& part: *ip){
            if( part == filter_pattern ){
                ret.push_back(*ip);
                break;
            }
        }
    }
    return ret;
}

void iputils::print_ip( ip_addr& addr, std::ostream& stream){
    for( auto ip_part = addr.cbegin() ; ip_part != addr.cend(); ++ip_part ){
        if( ip_part != addr.cbegin() ) stream<<".";
        stream<<(*ip_part);
    }

}
