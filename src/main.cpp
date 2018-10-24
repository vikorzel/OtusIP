#include <iostream>
#include <set>
#include <tuple>
#include <vector>
#include <algorithm>
#include <thread>
#include <chrono>

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
enum class CompareErrors{
    OK,
    WRONG_IPSIZE,
    WRONG_IPPART
};

enum class CompareResults{
    GREATER,
    LOWER,
    EQUAL,
    UNKNOWN
};


auto compare_ip(const std::vector<std::string>& ip1, const std::vector<std::string>& ip2){
    if(ip1.size() != 4 || ip2.size() != 4){
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

    for(int j = 0 ; j != 4; j++){
        std::cout<<ip1[j]<<".";
    }
    std::cout<<" EQ ";
    for(int j = 0 ; j != 4; j++){
        std::cout<<ip2[j]<<".";
    }
    std::cout<<std::endl;
    return std::make_tuple(CompareResults::EQUAL, CompareErrors::OK);
}

auto sort_ip_vec(std::vector<std::vector<std::string>>& vec, int fst, int lst){
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
using namespace std::literals::chrono_literals;
using ip_addr = std::vector<std::string>;
using ip_storage = std::vector<ip_addr>;


int main(){
    try
    {

        std::vector<std::vector<std::string>> ip_pool;

        for(std::string line; std::getline(std::cin, line);)
        {
            std::vector<std::string> v = split(line, '\t');
            ip_pool.push_back(split(v.at(0), '.'));
        }

        auto err = sort_ip_vec(ip_pool,0,ip_pool.size()-1);


        // TODO reverse lexicographically sort
        if(err == CompareErrors::OK){
            for(auto ip = ip_pool.rbegin(); ip != ip_pool.rend(); ++ip)
            {
                for(std::vector<std::string>::const_iterator ip_part = ip->cbegin(); ip_part != ip->cend(); ++ip_part)
                {
                    if (ip_part != ip->cbegin())
                    {
                        std::cout << ".";

                    }
                    std::cout << *ip_part;
                }
                std::cout << std::endl;
            }
        }else{
            std::cout<<"ErrNo: "<< static_cast<std::underlying_type<CompareErrors>::type>(err)<<std::endl;
        }
        // 222.173.235.246
        // 222.130.177.64
        // 222.82.198.61
        // ...
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first byte and output
        // ip = filter(1)

        // 1.231.69.33
        // 1.87.203.225
        // 1.70.44.170
        // 1.29.168.152
        // 1.1.234.8

        // TODO filter by first and second bytes and output
        // ip = filter(46, 70)

        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76

        // TODO filter by any byte and output
        // ip = filter_any(46)

        // 186.204.34.46
        // 186.46.222.194
        // 185.46.87.231
        // 185.46.86.132
        // 185.46.86.131
        // 185.46.86.131
        // 185.46.86.22
        // 185.46.85.204
        // 185.46.85.78
        // 68.46.218.208
        // 46.251.197.23
        // 46.223.254.56
        // 46.223.254.56
        // 46.182.19.219
        // 46.161.63.66
        // 46.161.61.51
        // 46.161.60.92
        // 46.161.60.35
        // 46.161.58.202
        // 46.161.56.241
        // 46.161.56.203
        // 46.161.56.174
        // 46.161.56.106
        // 46.161.56.106
        // 46.101.163.119
        // 46.101.127.145
        // 46.70.225.39
        // 46.70.147.26
        // 46.70.113.73
        // 46.70.29.76
        // 46.55.46.98
        // 46.49.43.85
        // 39.46.86.85
        // 5.189.203.46
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
