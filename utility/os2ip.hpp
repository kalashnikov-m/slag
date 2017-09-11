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
        template <class OctetInputIterator>
        cry::basic_int<P> operator()(OctetInputIterator first, OctetInputIterator last) const noexcept
        {
            std::reverse_iterator<OctetInputIterator> rfirst(last), rend(first);

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

} // namespace cry

#endif
