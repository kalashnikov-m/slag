#ifndef OS2IP_HPP
#define OS2IP_HPP

#include "basic_int.hpp"

namespace cry
{

    template <class T>
    struct os2ip;

    template <class P>
    struct os2ip<cry::basic_int<P>>
    {
        cry::basic_int<P> operator()(const std::vector<uint8_t>& octets) const noexcept
        {
            return operator()(octets.begin(), octets.end());
        }

        template <class OctetIterator>
        cry::basic_int<P> operator()(OctetIterator first, OctetIterator last) const noexcept
        {
            std::reverse_iterator<OctetIterator> rfirst(last), rend(first);

            auto noctets = std::distance(rfirst, rend);
            auto nwords  = noctets / sizeof(P);
            if (noctets % sizeof(P))
            {
                ++nwords;
            }

            std::vector<P> dst(nwords);
            auto result = dst.rbegin();

            P word(0);
            size_t cnt(0);
            for (; rfirst != rend; ++rfirst)
            {
                word = (static_cast<P>(*rfirst) << cnt * 8) | word;
                ++cnt;
                if (cnt == sizeof(P))
                {
                    *result++ = word;
                    word      = 0;
                    cnt       = 0;
                }
            }

            if (word && cnt)
            {
                *result++ = word;
            }

            cry::basic_int<P> out(dst);

            return out;
        }
    };

    template <class T>
    struct ip2os;

    template <class P>
    struct ip2os<basic_int<P>>
    {	
        template <class OutputIterator>
        OutputIterator operator()(const basic_int<P>& ip, OutputIterator oct)
        {

            std::vector<uint8_t> dst(ip);

            oct = std::copy(dst.begin(), dst.end(), oct);

            return oct;
        }
    };

} // namespace cry

#endif
