#include <string>
#include <tuple>
#include <vector>

using ip_addr = std::vector<std::string>;
using ip_storage = std::vector<ip_addr>;

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

struct iputils{
    using interr = std::tuple<uint32_t,int>;
    using strerr = std::tuple<std::string, int>;    

    static interr strtoint(const std::string&);
    static strerr getaddr(const std::string&);    

    static CompareErrors sort_ip_vec(ip_storage& vec, int fst, int lst);
    static ip_storage filter_any( ip_storage& ips, int filter_pattern );

    template<typename ... Ts>
    static ip_storage filter( ip_storage& ips, Ts ... patterns );
private:
    static auto compare_ip(const ip_addr& ip1, const ip_addr& ip2);

    template<typename T>
    static bool valid_ip(ip_addr& addr, int num, T pattern);

    template <typename T, typename ... Ts>
    static bool valid_ip(ip_addr& addr, int num, T pattern,  Ts ... another_patterns);

};
