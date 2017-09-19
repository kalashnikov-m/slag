#ifndef OS2IP_HPP
#define OS2IP_HPP

#include "basic_int.hpp"

namespace cry
{

    template <class T>
    struct OS2IP;

    template <class P>
    struct OS2IP<cry::basic_int<P>>
    {
        cry::basic_int<P> operator()(const std::vector<uint8_t>& octets) const noexcept
        {
            return operator()(octets.begin(), octets.end());
        }

        template <class OctetIterator>
        cry::basic_int<P> operator()(OctetIterator first, OctetIterator last) const noexcept
        {
            /////////////////////////////////
            // skiping zeros and whitespaces
            for (; first != last && *first == 0x00; ++first)
                ;

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
    struct IP2OS;

    template <>
    struct IP2OS<uint8_t>
    {
        template <class OctetIterator>
        OctetIterator operator()(uint8_t x, OctetIterator result) const noexcept
        {
            *result++ = x;

            return result;
        }
    };

    template <>
    struct IP2OS<uint16_t>
    {
        template <class OctetIterator>
        OctetIterator operator()(uint16_t x, OctetIterator result) const noexcept
        {
            *result++ = (x & 0xff00) >> 8;
            *result++ = (x & 0x00ff);

            return result;
        }
    };

    template <>
    struct IP2OS<uint32_t>
    {
        template <class OctetIterator>
        OctetIterator operator()(uint32_t x, OctetIterator result) const noexcept
        {
            *result++ = (x & 0xff000000) >> 24;
            *result++ = (x & 0x00ff0000) >> 16;
            *result++ = (x & 0x0000ff00) >> 8;
            *result++ = (x & 0x000000ff);

            return result;
        }
    };

    template <>
    struct IP2OS<uint64_t>
    {
        template <class OctetIterator>
        OctetIterator operator()(uint64_t x, OctetIterator result) const noexcept
        {
            *result++ = static_cast<uint8_t>((x & 0xff00000000000000) >> 56);
            *result++ = static_cast<uint8_t>((x & 0x00ff000000000000) >> 48);
            *result++ = static_cast<uint8_t>((x & 0x0000ff0000000000) >> 40);
            *result++ = static_cast<uint8_t>((x & 0x000000ff00000000) >> 32);
            *result++ = static_cast<uint8_t>((x & 0x00000000ff000000) >> 24);
            *result++ = static_cast<uint8_t>((x & 0x0000000000ff0000) >> 16);
            *result++ = static_cast<uint8_t>((x & 0x000000000000ff00) >> 8);
            *result++ = static_cast<uint8_t>((x & 0x00000000000000ff));

            return result;
        }
    };

    template <class P>
    struct IP2OS<basic_int<P>>
    {
        std::vector<uint8_t> operator()(const basic_int<P>& ip)
        {
            std::vector<uint8_t> octets;
            octets.reserve(ip.polynomial().size() * sizeof(P));

            operator()(ip, std::back_inserter(octets));

            return octets;
        }

        template <class OutputIterator>
        OutputIterator operator()(const basic_int<P>& ip, OutputIterator octetIt)
        {
            const auto& polynomial = ip.polynomial();

            for (auto xVal : polynomial)
            {
                // auto x = swap_bytes()(xVal);
                octetIt = IP2OS<P>()(xVal, octetIt);
            }

            return octetIt;
        }
    };

} // namespace cry

#endif
