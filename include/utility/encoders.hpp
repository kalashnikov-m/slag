#ifndef TO_HPP
#define TO_HPP

namespace cry
{

    struct swap_bytes
    {
        template <class T>
        constexpr T operator()(T x) const noexcept
        {
            switch (sizeof(T))
            {
            case 2:
                x = (x & 0x00ff) << 8 | (x & 0xff00) >> 8;
                break;
            case 4:
                x = ((x & 0x0000ff00) << 8) | ((x & 0x00ff0000) >> 8) | ((x & 0x000000ff) << 24) | ((x & 0xff000000) >> 24);
                break;
            case 8:
                x = (x & 0x000000ff00000000) >> 8 | (x & 0x00000000ff000000) << 8 | (x & 0x0000ff0000000000) >> 24 | (x & 0x0000000000ff0000) << 24 | (x & 0x00ff000000000000) >> 40 | (x & 0x000000000000ff00) << 40 | (x & 0xff00000000000000) >> 56 | (x & 0x00000000000000ff) << 56;
                break;
            default:
                break;
            };

            return x;
        }
    };

} // namespace cry

#endif
