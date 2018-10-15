#include <string>
#include <tuple>

struct iputils{
    using interr = std::tuple<uint32_t,int>;
    using strerr = std::tuple<std::string, int>;
    static interr strtoint(const std::string&);
    static strerr getaddr(const std::string&);
};
