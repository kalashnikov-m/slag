#ifndef MGF_H
#define MGF_H

namespace cry
{

    namespace rsa
    {
        template <class Digest>
        struct mgf1
        {
            template <class InputIterator, class OutputIterator>
            void operator()(InputIterator first, InputIterator last, OutputIterator result, size_t maskLen) const
            {

                if (maskLen > 0xffffffff)
                {
                    throw std::runtime_error("mask too long");
                }

                const size_t hLen = Digest::size;
                Digest digest;

                size_t i    = 0;
                size_t rest = maskLen;
                for (; i < maskLen / hLen; ++i)
                {

                    std::vector<uint8_t> mgfSeed(first, last);

                    mgfSeed.push_back(static_cast<uint8_t>((i & 0xFF000000) >> 24));
                    mgfSeed.push_back(static_cast<uint8_t>((i & 0x00FF0000) >> 16));
                    mgfSeed.push_back(static_cast<uint8_t>((i & 0x0000FF00) >> 8));
                    mgfSeed.push_back(static_cast<uint8_t>((i & 0x000000FF) >> 0));

                    digest(mgfSeed.begin(), mgfSeed.end(), result);

                    result += hLen;
                    rest -= hLen;
                }

                {
                    std::vector<uint8_t> mgfSeed(first, last);

                    mgfSeed.push_back(static_cast<uint8_t>((i & 0xFF000000) >> 24));
                    mgfSeed.push_back(static_cast<uint8_t>((i & 0x00FF0000) >> 16));
                    mgfSeed.push_back(static_cast<uint8_t>((i & 0x0000FF00) >> 8));
                    mgfSeed.push_back(static_cast<uint8_t>((i & 0x000000FF) >> 0));

                    std::vector<uint8_t> hash(hLen);
                    digest(mgfSeed.begin(), mgfSeed.end(), hash.begin());
                    std::copy_n(hash.begin(), rest, result);
                }
            }
        };
    }
}

#endif