
#include "gtest/gtest.h"

#include "algorithm.hpp"

#include "basic_integer.hpp"
#include "rsa/emsa_pkcs1.hpp"
#include "utility/os2ip.hpp"
#include "rsa/rsaes_oaep.hpp"
#include "rsa/rsaes_pkcs1.hpp"
#include "rsa/rsassa_pkcs1.hpp"
#include "rsa/rsassa_pss.hpp"
#include "digest/sha1.hpp"

using namespace std;
using namespace cry;
using namespace cry::rsa;

class Test_Rsa : public ::testing::Test
{
};

TEST(Test_Rsa, Encrypt_EME_PKCS1_1024)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const std::vector<uint8_t>& rand, const std::vector<uint8_t>& m) {
        std::vector<uint8_t> C(128);
        auto it = m.begin(), end = m.end();
        for (; *it == 0 && it != end; ++it)
            ;

        rsa::rsaes_pkcs1<>::encrypt(it, end, C.begin(), e, n, 1024);

        std::vector<uint8_t> D(128);
        auto d_end = rsa::rsaes_pkcs1<>::decrypt(C.begin(), C.end(), D.begin(), d, n, 1024);
        std::vector<uint8_t> decrypt(D.begin(), d_end);
        auto d1 = OS2IP<bigint_t>()(m);
        auto d2 = OS2IP<bigint_t>()(decrypt);
        EXPECT_EQ(d1, d2);
    };

    bigint_t n("bcb47b2e0dafcba81ff2a2b5cb115ca7e757184c9d72bcdcda707a146b3b4e29989ddc660bd694865b932b71ca24a335cf4d339c719183e6222e4c9ea6875acd528a49ba21863fe08147c3a47e41990b51a03f77d22137f8d74c43a5a45f4e9e18a2d15db051dc89385db9cf8374b63a8cc88113710e6d8179075b7dc79ee76b");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("383a6f19e1ea27fd08c7fbc3bfa684bd6329888c0bbe4c98625e7181f411cfd0853144a3039404dda41bce2e31d588ec57c0e148146f0fa65b39008ba5835f829ba35ae2f155d61b8a12581b99c927fd2f22252c5e73cba4a610db3973e019ee0f95130d4319ed413432f2e5e20d5215cdd27c2164206b3f80edee51938a25c1");

    {
        bigint_t Msg("1248f62a4389f42f7b4bb131053d6c88a994db2075b912ccbe3ea7dc611714f14e075c104858f2f6e6cfd6abdedf015a821d03608bf4eba3169a6725ec422cd9069498b5515a9608ae7cc30e3d2ecfc1db6825f3e996ce9a5092926bc1cf61aa42d7f240e6f7aa0edb38bf81aa929d66bb5d890018");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }

    {
        bigint_t Msg("58439043890584359034808043850384584385048043850983450843");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }

    {
        bigint_t Msg("584385904380583580928590829384590283905834095825882543252577893475934757943759843795877437598437583495783");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }

    {
        bigint_t Msg("1248f62a4389f42f7b4bb131053d6c88a994db2075b912ccbe3ea7dc611714f14e075c104858f2f6e6cfd6abdedf015a821d03608bf4eba3169a6725ec422cd9069498b5515a9608ae7cc30e3d2ecfc1db6825f3e996ce9a5092926bc1cf61aa42d7f240e6f7aa0edb38bf81aa929d66bb5d890018");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }

    {
        bigint_t Msg("5874385438590843589043850834590438905834859038405804305834589034850834850438059843");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }

    {
        bigint_t Msg("5889043859084358904385098345843858348904380598435843850834850438580435");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }

    {
        bigint_t Msg("47543af58094850348503");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }

    {
        bigint_t Msg("584385903489058358904385835803480584385903890840");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }

    {
        bigint_t Msg("58943859043859038905890258902845902384182483248234802480320");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }

    {
        bigint_t Msg("85038590438590438905834859038458043");

        auto m = I2OSP<bigint_t>()(Msg);
        test(n, e, d, std::vector<uint8_t>(), m);
    }
}

TEST(Test_Rsa, Encrypt_OAEP_SHA__1_1024)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const std::vector<uint8_t>& seed, const std::vector<uint8_t>& m, const bigint_t& cipher) {
        std::vector<uint8_t> C(128);
        rsa::rsaes_oaep<sha1>::encrypt(m.begin(), m.end(), C.begin(), e, n, 1024, seed);
        EXPECT_EQ(cipher, OS2IP<bigint_t>()(C));

        std::vector<uint8_t> D(128);
        auto d_end = rsa::rsaes_oaep<sha1>::decrypt(C.begin(), C.end(), D.begin(), d, n, 1024);

        std::vector<uint8_t> decrypted(D.begin(), d_end);
        EXPECT_EQ(m, decrypted);
    };

    //# RSA modulus n :
    bigint_t n("bb f8 2f 09 06 82 ce 9c 23 38 ac 2b 9d a8 71 f7"
               "36 8d 07 ee d4 10 43 a4 40 d6 b6 f0 74 54 f5 1f"
               "b8 df ba af 03 5c 02 ab 61 ea 48 ce eb 6f cd 48"
               "76 ed 52 0d 60 e1 ec 46 19 71 9d 8a 5b 8b 80 7f"
               "af b8 e0 a3 df c7 37 72 3e e6 b4 b7 d9 3a 25 84"
               "ee 6a 64 9d 06 09 53 74 88 34 b2 45 45 98 39 4e"
               "e0 aa b1 2d 7b 61 a5 1f 52 7a 9a 41 f6 c1 68 7f"
               "e2 53 72 98 ca 2a 8f 59 46 f8 e5 fd 09 1d bd cb");

    //# RSA public exponent e :
    bigint_t e("11");

    //# Prime p :
    bigint_t p("ee cf ae 81 b1 b9 b3 c9 08 81 0b 10 a1 b5 60 01"
               "99 eb 9f 44 ae f4 fd a4 93 b8 1a 9e 3d 84 f6 32"
               "12 4e f0 23 6e 5d 1e 3b 7e 28 fa e7 aa 04 0a 2d"
               "5b 25 21 76 45 9d 1f 39 75 41 ba 2a 58 fb 65 99");

    //# Prime q :
    bigint_t q("c9 7f b1 f0 27 f4 53 f6 34 12 33 ea aa d1 d9 35"
               "3f 6c 42 d0 88 66 b1 d0 5a 0f 20 35 02 8b 9d 86"
               "98 40 b4 16 66 b4 2e 92 ea 0d a3 b4 32 04 b5 cf"
               "ce 33 52 52 4d 04 16 a5 a4 41 e7 00 af 46 15 03");

    bigint_t d;
    {
        bigint_t phi = (p - 1) * (q - 1);
        cry::mod_inverse(d, e, phi);
    }

    //# Message M to be encrypted :
    // bigint_t M("d4 36 e9 95 69 fd 32 a7 c8 a0 5b bc 90 d3 2c 49");
    std::vector<uint8_t> M = {0xd4, 0x36, 0xe9, 0x95, 0x69, 0xfd, 0x32, 0xa7, 0xc8, 0xa0, 0x5b, 0xbc, 0x90, 0xd3, 0x2c, 0x49};

    //# seed:
    std::vector<uint8_t> seed = {0xaa, 0xfd, 0x12, 0xf6, 0x59, 0xca, 0xe6, 0x34, 0x89, 0xb4, 0x79, 0xe5, 0x07, 0x6d, 0xde, 0xc2, 0xf0, 0x6c, 0xb5, 0x8f};

    bigint_t cipher("12 53 E0 4D C0 A5 39 7B B4 4A 7A B8 7E 9B F2 A0 39 A3 3D "
                    "1E 99 6F C8 2A 94 CC D3 00 74 C9 5D F7 63 72 20 17 06 9E "
                    "52 68 DA 5D 1C 0B 4F 87 2C F6 53 C1 1D F8 23 14 A6 79 68 "
                    "DF EA E2 8D EF 04 BB 6D 84 B1 C3 1D 65 4A 19 70 E5 78 3B "
                    "D6 EB 96 A0 24 C2 CA 2F 4A 90 FE 9F 2E F5 C9 C1 40 E5 BB "
                    "48 DA 95 36 AD 87 00 C8 4F C9 13 0A DE A7 4E 55 8D 51 A7 "
                    "4D DF 85 D8 B5 0D E9 68 38 D6 06 3E 09 55");

    test(n, e, d, seed, M, cipher);
}

TEST(Test_Rsa, SigGen_SHA__1_RSA_PSS_SHA1)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S, const std::vector<uint8_t>& saltVal) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(1024 / 8);

        rsassa_pss<sha1>::sign(plain.begin(), plain.end(), signature.begin(), n, d, 1024, saltVal);
        // auto x = OS2IP<bigint_t>()(signature);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pss<sha1>::verify(plain.begin(), plain.end(), signature.begin(), signature.end(), n, e, 1024);
        EXPECT_EQ(f, true);
    };

    const std::vector<uint8_t> SaltVal = {0x6f, 0x28, 0x41, 0x16, 0x6a, 0x64, 0x47, 0x1d, 0x4f, 0x0b, 0x8e, 0xd0, 0xdb, 0xb7, 0xdb, 0x32, 0x16, 0x1d, 0xa1, 0x3b};
    bigint_t n("bcb47b2e0dafcba81ff2a2b5cb115ca7e757184c9d72bcdcda707a146b3b4e29989ddc660bd694865b932b71ca24a335cf4d339c719183e6222e4c9ea6875acd528a49ba21863fe08147c3a47e41990b51a03f77d22137f8d74c43a5a45f4e9e18a2d15db051dc89385db9cf8374b63a8cc88113710e6d8179075b7dc79ee76b");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("383a6f19e1ea27fd08c7fbc3bfa684bd6329888c0bbe4c98625e7181f411cfd0853144a3039404dda41bce2e31d588ec57c0e148146f0fa65b39008ba5835f829ba35ae2f155d61b8a12581b99c927fd2f22252c5e73cba4a610db3973e019ee0f95130d4319ed413432f2e5e20d5215cdd27c2164206b3f80edee51938a25c1");

    { // +
        bigint_t Msg(
            "1248f62a4389f42f7b4bb131053d6c88a994db2075b912ccbe3ea7dc611714f14e075c104858f2f6e6cfd6abdedf015a821d03608bf4eba3169a6725ec422cd9069498b5515a9608ae7cc30e3d2ecfc1db6825f3e996ce9a5092926bc1cf61aa42d7f240e6f7aa0edb38bf81aa929d66bb5d890018088458720d72d569247b0c");
        bigint_t S("682cf53c1145d22a50caa9eb1a9ba70670c5915e0fdfde6457a765de2a8fe12de9794172a78d14e668d498acedad616504bb1764d094607070080592c3a69c343d982bd77865873d35e24822caf43443cc10249af6a1e26ef344f28b9ef6f14e09ad839748e5148bcceb0fd2aa63709cb48975cbf9c7b49abc66a1dc6cb5b31a");

        test(n, e, d, Msg, S, SaltVal);
    }

    {
        bigint_t Msg(
            "9968809a557bb4f892039ff2b6a0efcd06523624bc3b9ad359a7cf143c4942e874c797b9d37a563d436fe19d5db1aad738caa2617f87f50fc7fcf4361fc85212e89a9465e7f4c361982f64c8c5c0aa5258b9e94f6e934e8dac2ace7cd6095c909de85fe7b973632c384d0ebb165556050d28f236aee70e16b13a432d8a94c62b");
        bigint_t S("8f5ea7037367e0db75670504085790acd6d97d96f51e76df916a0c2e4cd66e1ab51c4cd8e2c3e4ef781f638ad65dc49c8d6d7f6930f80b6ae199ea283a8924925a50edab79bb3f34861ffa8b2f96fdf9f8cad3d3f8f025478c81f316da61b0d6a7f71b9068efdfb33c21983a922f4669280d8e84f963ff885ef56dd3f50381db");

        test(n, e, d, Msg, S, static_cast<std::vector<uint8_t>>(SaltVal));
    }

    {
        bigint_t Msg(
            "f9be76e9b029c4933260249dd76c5067ccd483d4b9b338fda00d34270963c6f35c854ed58ea8fcc7ffb8da3fa3f00d5e61a7586ab86de17ea8563880d0969554d44e614f01a6f8ef341caec9f71c10c2eed06c82723993267b7fdd35c3856ed628c1b840524b41719733a6231b18e1fc3cf6c7052d40d45de02f2b2f2a59d9e1");
        bigint_t S("4f7ff112a7937707d529b5602e1d017b79739b13234a725dc36487e05ee4f5e072270b6712b5f71e47747dd4b9b289df31f91fc97db39ba14694ff894b42932555d01dfd4146a1672338cc60521e4b5988fd22896b512faaf5888dedcd5f9662319e9f752c99c341be08ac2bd0346845ccb74d0a7d4d8165aa7613b66847702f");

        test(n, e, d, Msg, S, static_cast<std::vector<uint8_t>>(SaltVal));
    }

    { // +
        bigint_t Msg(
            "5eec1f64a799c1bb29b9aa6635b0bb93d1b461c9f6316a71fc6c1794526672e67203e104146bfcfaa2b19c08edb334a61e4165c5ef81cc581b911f13126f7872f216f0feb3ea70cca156c44610b4f12d4b6dc9dfcd61bcd6b489f17272da96f8c66d74daebdec6390ff24ba8067c9a334081b6a0f89ae2f4af9ab639d971737f");
        bigint_t S("344c602f68bd9529af10a80a1a3c25e24cce3d27e6baacc1b998e8c0b634e1689a97d2eb6004ad08b651a2dec53c8a6bae51de694f072add1f90758abb6f37471c7379498f6a63b49ff112a99d4de5855e9b1d1783f75c919730e9fc294017bc96ba82cc009797353881ea25c4fac9233a01189ac4825aa52bf01fa300e630d5");

        test(n, e, d, Msg, S, static_cast<std::vector<uint8_t>>(SaltVal));
    }

    {
        bigint_t Msg(
            "bd75511e451602958995e50efaef130d9f5eef3b097eb8af88ae5a6d9fb98a2cb6b6115d0f2c6868fb7a86394e76f6fb10552c2375cb19b10f72fdb243707d0afa55f57098420fba3250955bfc2f832a000e616516bbcff5100c62f3357dae6cf11aa461caa01f1296dfb97deb62e321d8523507d2980203f14230385d2846da");
        bigint_t S("0dbb7954b411ac1c425d778875dedda9add836f7283040797b5899b993e28f8e843909c28baf4169a0344f5eed769b22c567bab97a1816a634d0fe0e7860674cc6273ec2232d01ce93ea2cd03f42ceb4c4bda7a40dc1a5c89a84ae7afddb729956a83fc626a99f4be5d5daf2ef47f4eccaa103acf92fb5ee5e1b4f8cd8d4648a");

        test(n, e, d, Msg, S, static_cast<std::vector<uint8_t>>(SaltVal));
    }

    {
        bigint_t Msg(
            "ca77e662077ed5cb63dd9b10177bd66062276828d1cfd8e3bdf2f6321557be2f5e007ffef75a3da6a2b6531f40fc5caddc9e546cebccfb2bd2e77a7da937490e3fe050345e124dc650817531a1155ec827e21c5c405aab0d5bc18d5fbae2b3917601a12ac310ced47526496dc32ca25c4bda082157e2af305f6a7500cfc0c45f");
        bigint_t S("462d92b7fbac49dab2fef13caeaf9bd11bb5053a8aef554a05e1ab6ce095678212b8e1fbea0396b18aae3d1275da8520abc123f88c3b5c056271d4415642ca4abc2030de600a48280115151c84a7d9c7f17769e0926450efe3fce62fb63db53082a3ed21f7299433aed078cfb7be3b48bcd6c13f11e3ea0f4946a139a941303d");

        test(n, e, d, Msg, S, static_cast<std::vector<uint8_t>>(SaltVal));
    }

    { // +
        bigint_t Msg(
            "638260c0a519c45faba299d0ca2e80bf50dfde8d6a42e04645dfbcd4740f3a72920e74632851d9e3d01a785e9b497ce0b175f2cd373bd3d276d63e1b39f005c676b86b9831352cef9edabef8865ad722ebbe2fd3efb48759f22aea23fb1b333159a9cfc98a6dc46c5b0bf2085109996f48e11c085a5405c4020248548138cc9a");
        bigint_t S("015c922938f67dd570f960ab30a4fc72b93df76aa76973c513055793807aecc314cb0ef1f7de401e609904ed20cab642ba4664ed8b9e5823705bb27a201cdda9455fa9752c69067c7473064e0e399e87ca5f9d28798ee7bfda1815c3c30e83da411c33938001e6cebc388bd13ee206ea93f9fd458b1a8300ab8aaa79bd22d182");

        test(n, e, d, Msg, S, static_cast<std::vector<uint8_t>>(SaltVal));
    }

    { // +
        bigint_t Msg(
            "c6ac689ac52fa1ea2276f7aade8b1b54ddd5aaa047e9b2f92fa3732d72766268e2007463a4e198d9d7ce853b76e1b7a48033b40f56ad94c8641bc0200382fa659eb49e11142127425ef0f46a724f4d486707a117d95311058ff233e65b7b0f2a42f16e0cdd97550dff05cca1997ce328da77f71f9a823298f84f51e74207aa9f");
        bigint_t S("8f7e7bfe45448c203ed2b7b8b68e3aabef2ca3f7c2861042d591fc964649ecda06da1b0f77d230c46b0d3fed02ceaff91336abd57be295f4165527ce85655634fc77e9d1b34e0a826aa1c687818fa2e1ef319efb6d1de108f140a30fe942aad25bcaf2bc2a8ca4ac083a10c1188cad40ab723b5fcf47cc709fef4bbc71fed7b4");

        test(n, e, d, Msg, S, static_cast<std::vector<uint8_t>>(SaltVal));
    }

    {
        bigint_t Msg(
            "7523cb2f9d75b6068f785c4482dcbcbf80d05ab75e22da14f1a8cdf07fdecb5574d0c248563b8d06fbefc98dfa77e45e5ffc7682a367d5cbdbc6e45c2ff12b6b8e6d2d46a090f594fce6f3de9cd4064e8d7bdab6ebdf74d30d490bd4fdea1a162bc0b33262be1639fa27823e503b4b1573152d5b20d4e0b6f1f9ec5ed110bfdd");
        bigint_t S("31527f8b5b289a67acc9c525eea485b40beeedbb777d68201caa54ebc4a08c15b4bc5fe85de9f695f21c95b3617fc4c3e3f8aa872f5ebfbf2a4f617fa39bb3959076f3e61d41005ee8306c8d8df6f4cba0d27430d981e841e2dd9a5db01cd6f481e5cedc3255d94eab10394efcfdb16ee90d25d02c37ad41b0708e916f5ae0b5");

        test(n, e, d, Msg, S, static_cast<std::vector<uint8_t>>(SaltVal));
    }

    {
        bigint_t Msg(
            "0320f03a9c681fd97dcc1ab0dc3b6642075d599aa5c97f283f793dcbff7ab42fb7e8e859241a2280751a931f07583948739f073bca4740371fad249046e0ce02dc9d5d45877e52e17f6e39444e608c12d441d4bc8d2a115417db5ca0ee65cb6d7b2695254dfa76a4a6b0554839bb2547322a094677dbbb272deb2e227f707217");
        bigint_t S("823156702ac0caa27fc3461b7a2c0d71411e5c45708184b26975faee5551c3854a9ad68f9c1327c43ccf46d229a44345b0a08d11ffdb6f6180a240ac68c895daae1086d60e95e1f3db3931ee36a4f751de5699fa5374d1959b78de0f479ab070c220256803864aa12302dea96aa0732ac4c40d799bd306087bd43ef6b36868f0");

        test(n, e, d, Msg, S, static_cast<std::vector<uint8_t>>(SaltVal));
    }
}

TEST(Test_Rsa, SigGen_SHA__1_RSA_PKCS_1024)
{
    // [mod = 1024]
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(128);

        rsassa_pkcs1<sha1>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 1024);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha1>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1024);
        EXPECT_EQ(f, true);
    };

    {
        bigint_t n("c8a2069182394a2ab7c3f4190c15589c56a2d4bc42dca675b34cc950e24663"
                   "048441e8aa593b2bc59e198b8c257e882120c62336e5cc745012c7ffb063ee"
                   "be53f3c6504cba6cfe51baa3b6d1074b2f398171f4b1982f4d65caf882ea4d"
                   "56f32ab57d0c44e6ad4e9cf57a4339eb6962406e350c1b15397183fbf1f035"
                   "3c9fc991");
        bigint_t e("00000000000000000000000000000000000000000000000000000000000000"
                   "00000000000000000000000000000000000000000000000000000000000000"
                   "00000000000000000000000000000000000000000000000000000000000000"
                   "00000000000000000000000000000000000000000000000000000000000000"
                   "00010001");
        bigint_t d("5dfcb111072d29565ba1db3ec48f57645d9d8804ed598a4d470268a89067a2"
                   "c921dff24ba2e37a3ce834555000dc868ee6588b7493303528b1b3a94f0b71"
                   "730cf1e86fca5aeedc3afa16f65c0189d810ddcd81049ebbd0391868c50ede"
                   "c958b3a2aaeff6a575897e2f20a3ab5455c1bfa55010ac51a7799b1ff84836"
                   "44a3d425");

        {
            // SHAAlg = sha1
            bigint_t Msg("e8312742ae23c456ef28a23142c4490895832765dadce02afe5be5d31b"
                         "0048fbeee2cf218b1747ad4fd81a2e17e124e6af17c3888e6d2d40c008"
                         "07f423a233cad62ce9eaefb709856c94af166dba08e7a06965d7fc0d8e"
                         "5cb26559c460e47bc088589d2242c9b3e62da4896fab199e144ec136db"
                         "8d84ab84bcba04ca3b90c8e5");
            bigint_t S("28928e19eb86f9c00070a59edf6bf8433a45df495cd1c73613c2129840f4"
                       "8c4a2c24f11df79bc5c0782bcedde97dbbb2acc6e512d19f085027cd5750"
                       "38453d04905413e947e6e1dddbeb3535cdb3d8971fe0200506941056f212"
                       "43503c83eadde053ed866c0e0250beddd927a08212aa8ac0efd61631ef89"
                       "d8d049efb36bb35f");

            test(n, e, d, Msg, S);
        }

        {
            // SHAAlg = sha1
            bigint_t Msg("4c95073dac19d0256eaadff3505910e431dd50018136afeaf690b7d1"
                         "8069fcc980f6f54135c30acb769bee23a7a72f6ce6d90cbc858c86db"
                         "bd64ba48a07c6d7d50c0e9746f97086ad6c68ee38a91bbeeeb2221aa"
                         "2f2fb4090fd820d4c0ce5ff025ba8adf43ddef89f5f3653de15edcf3"
                         "aa8038d4686960fc55b2917ec8a8f9a8");
            bigint_t S("53ab600a41c71393a271b0f32f521963087e56ebd7ad040e4ee8aa7c45"
                       "0ad18ac3c6a05d4ae8913e763cfe9623bd9cb1eb4bed1a38200500fa7d"
                       "f3d95dea485f032a0ab0c6589678f9e8391b5c2b1392997ac9f82f1d16"
                       "8878916aace9ac7455808056af8155231a29f42904b7ab87a5d71ed639"
                       "5ee0a9d024b0ca3d01fd7150");

            test(n, e, d, Msg, S);
        }

        {
            // SHAAlg = sha1
            bigint_t Msg("e075ad4b0f9b5b20376e467a1a35e308793ba38ed983d03887b8b82e"
                         "da630e68b8618dc45b93de5555d7bcfed23756401e61f5516757de6e"
                         "c3687a71755fb4a66cfaa3db0c9e69b631485b4c71c762eea229a046"
                         "9c7357a440950792ba9cd7ae022a36b9a923c2ebd2aa69897f4cceba"
                         "0e7aee97033d03810725a9b731833f27");
            bigint_t S("642609ce084f479271df596480252e2f892b3e7982dff95994c3eeda78"
                       "7f80f3f6198bbce33ec5515378d4b571d7186078b75b43aed11d342547"
                       "386c5696eb3799a0b28475e54cd4ca7d036dcd8a11f5e10806f7d3b8cc"
                       "4fcb3e93e857be958344a34e126809c15b3d33661cf57bf5c338f07acc"
                       "ed60f14019335c152d86b3b2");

            test(n, e, d, Msg, S);
        }

        {
            // SHAAlg = sha1
            bigint_t Msg("18500155d2e0587d152698c07ba44d4f04aa9a900b77ce6678a137b2"
                         "38b73b1aea24a409db563cf635209aea735d3b3c18d7d59fa167e760"
                         "b85d95e8aa21b3881b1b2076f9d15512ae4f3d6e9acc480ec08abbec"
                         "bffe4abf0531e87d3f66de1f13fd1aa41297ca58e87b2a56d6399a4c"
                         "638df47e4e851c0ec6e6d97addcde366");
            bigint_t S("42f3c3c75f65ad42057bfac13103837bf9f8427c6ebc22a3adf7b8e47a"
                       "6857f1cb17d2a533c0a913dd9a8bdc1786222360cbd7e64b45fcf54f5d"
                       "a2f34230ab4806a087f8be47f35c4e8fee2e6aa2919a56679ce2a528a4"
                       "4bf818620d5b00b9ab0e1c8d2d722b53d3a8cca35a990ed25536ea6533"
                       "5e8253a54a68a64a373e0ed7");

            test(n, e, d, Msg, S);
        }

        {
            // SHAAlg = sha1
            bigint_t Msg("f7f79f9df2760fc83c73c7ccea7eae482dcfa5e02acf05e105db4828"
                         "3f440640439a24ca3b2a482228c58f3f32c383db3c4847d4bcc615d3"
                         "cac3eb2b77dd80045f0b7db88225ea7d4fa7e64502b29ce23053726e"
                         "a00883ea5d80502509a3b2df74d2142f6e70de22d9a134a50251e1a5"
                         "31798e747e9d386fe79ae1dea09e851b");
            bigint_t S("ac2ae66bca1ec12a66a2909fe2148a1d492d1edd00063b8b33af74760d"
                       "c4056718fd5041d4dfee12bec7081ab1bab2d0eb2712f334509f6889b1"
                       "9d75d1fd0fc61bf12976109c3614c46051e2a401b20880d6e64ad6a47f"
                       "23939803d138aa0a44bc41ba63030746622248771431dff97e8a856f0b"
                       "61d114f813911ee229655155");

            test(n, e, d, Msg, S);
        }

        {
            // SHAAlg = sha1
            bigint_t Msg("099bf17f16bcfd4c19b34fecb4b3233c9f9f98718f67b3065d95a5f8"
                         "642351362b9009534433987f73ce86b513669736b65295350c934fd4"
                         "0663e24f3a1037778a0bcd63003cb962fd99eb3393f7b2792f208369"
                         "7b25f6c682f6110f162fc9f76e35c615148267ddff3d06cffb0e7dee"
                         "5230e874a5c8adc41b75baa0be280e9c");
            bigint_t S("3a2b7571619272b81d3562a11c644502894421583e02879f5a7759fb64"
                       "ec2ab8105f7d11947c8e4bfca87219e52987aad3b81cbd483166ed7815"
                       "2af24460c908879f34c870573127e3448c8fbf43028350c975bbc3a999"
                       "196a3e9961228a2bb15b4985e95bba970ac4ad2ac5b42ac51dbc6509ef"
                       "fc13396693980fc89ba44c7b");

            test(n, e, d, Msg, S);
        }

        {
            // SHAAlg = sha1
            bigint_t Msg("fb40a73dc82f167f9c2bf98a991ea82fdb0141dbad44871afd70f05a"
                         "0e0bf9f26dbcbd6226afc6dc373b230445c2baf58ed9e0841fa927c8"
                         "479577da4b1e61d95b03af31c5ac401d69c8136b6d36a1803221709b"
                         "8670e55e1b5d5a8a3763700aae5ea6330eee2b4a191cf146784003d8"
                         "ad2218a94a5f68e3600ebef23ba4cf8c");
            bigint_t S("b10322602c284f4079e509faf3f40a3d2af3abef9f09171fdd16469d67"
                       "9bb9adc7e2acb1addb0bd5b38b5c4d986b43c79b9724f61e99b5b30363"
                       "0b62d0d8d5f76577fe7ea387710b43789ee1b35b614b691f0a27b73baf"
                       "6bc3f28ec210b9d3e4c5a2729cc1203b74ef70e315cfe5d06e040aee6b"
                       "3d22d91d6e229f690a966dd9");

            test(n, e, d, Msg, S);
        }

        {
            // SHAAlg = sha1
            bigint_t Msg("97e74960dbd981d46aadc021a6cf181ddde6e4cfcb4b638260c0a519"
                         "c45faba299d0ca2e80bf50dfde8d6a42e04645dfbcd4740f3a72920e"
                         "74632851d9e3d01a785e9b497ce0b175f2cd373bd3d276d63e1b39f0"
                         "05c676b86b9831352cef9edabef8865ad722ebbe2fd3efb48759f22a"
                         "ea23fb1b333159a9cfc98a6dc46c5b0b");
            bigint_t S("60ebc9e4e2e2b4fa6d31c57d0b86835e8d201d21c274cf5452cdd7ef28"
                       "57dc780dde3526f3658c4f2c8710eaae4870d275997e5cbb268e3bd251"
                       "f543b8828feb85c211c858e47a74cb122dc17f26fe92b4afeecbf1e20b"
                       "ea75c794c0482aa6532e87955dba249f0fa6562bdf8f4ccd8a63da69d1"
                       "f337523f65206fb8eb163173");

            test(n, e, d, Msg, S);
        }

        {
            // SHAAlg = sha1
            bigint_t Msg("95d04624b998938dd0a5ba6d7042aa88a2674dad438a0d31abb7979d"
                         "8de3dea41e7e63587a47b59d436433dd8bb219fdf45abb9015a50b4b"
                         "201161b9c2a47c304b80c4040fb8d1fa0c623100cded661b8eb52fa0"
                         "a0d509a70f3cf4bd83047ad964ffee924192f28e73c63b3efd9c99c8"
                         "b7a13145acc30d2dc063d80f96abe286");
            bigint_t S("859cc4fcd1b88ccda695b12311cf8bdca3b4c135faa11f9053dc10f4bf"
                       "12e5f2179be6ab5ad90f8d115f5df795a77340e20662809fa732b92560"
                       "adcffdb0ddb72d33811e94f854330680f2b238300995a9113a469afd9e"
                       "756f649208d2942febffb22e832279063ec5b57ab542d9bbc56e82cdc6"
                       "a03b00d10d45801575e949e1");

            test(n, e, d, Msg, S);
        }

        {
            // SHAAlg = sha1
            bigint_t Msg("207102f598ec280045be67592f5bba25ba2e2b56e0d2397cbe857cde"
                         "52da8cca83ae1e29615c7056af35e8319f2af86fdccc4434cd7707e3"
                         "19c9b2356659d78867a6467a154e76b73c81260f3ab443cc039a0d42"
                         "695076a79bd8ca25ebc8952ed443c2103b2900c9f58b6a1c8a6266e4"
                         "3880cda93bc64d714c980cd8688e8e63");
            bigint_t S("77f0f2a04848fe90a8eb35ab5d94cae843db61024d0167289eea92e5d1"
                       "e10a526e420f2d334f1bf2aa7ea4e14a93a68dba60fd2ede58b794dcbd"
                       "37dcb1967877d6b67da3fdf2c0c7433e47134dde00c9c4d4072e43361a"
                       "767a527675d8bda7d5921bd483c9551950739e9b2be027df3015b61f75"
                       "1ac1d9f37bea3214d3c8dc96");

            test(n, e, d, Msg, S);
        }
    }
}

TEST(Test_Rsa, SigGen_SHA224_RSA_PKCS_1024)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(128);

        rsassa_pkcs1<sha224>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 1024);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha224>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1024);
        EXPECT_EQ(f, true);
    };

    bigint_t n("c8a2069182394a2ab7c3f4190c15589c56a2d4bc42dca675b34cc950e2466304"
               "8441e8aa593b2bc59e198b8c257e882120c62336e5cc745012c7ffb063eebe53"
               "f3c6504cba6cfe51baa3b6d1074b2f398171f4b1982f4d65caf882ea4d56f32a"
               "b57d0c44e6ad4e9cf57a4339eb6962406e350c1b15397183fbf1f0353c9fc99"
               "1");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000"
               "000000000000000000000000000000000000000000000000000000000001000"
               "1");
    bigint_t d("5dfcb111072d29565ba1db3ec48f57645d9d8804ed598a4d470268a89067a2c9"
               "21dff24ba2e37a3ce834555000dc868ee6588b7493303528b1b3a94f0b71730c"
               "f1e86fca5aeedc3afa16f65c0189d810ddcd81049ebbd0391868c50edec958b3"
               "a2aaeff6a575897e2f20a3ab5455c1bfa55010ac51a7799b1ff8483644a3d42"
               "5");

    {
        bigint_t Msg("e567a39ae4e5ef9b6801ea0561b72a5d4b5f385f0532fc9fe10a7570f8"
                     "69ae05c0bdedd6e0e22d4542e9ce826a188cac0731ae39c8f87f9771ef"
                     "02132e64e2fb27ada8ff54b330dd93ad5e3ef82e0dda646248e35994bd"
                     "a10cf46e5abc98aa7443c03cddeb5ee2ab82d60100b102963189797027"
                     "5f119d05daa2220a4a0defba");
        bigint_t S("5aa5033381bdd0acce332dd314daf008acaa9e835f832979891d1bda2b55"
                   "d5eae35c479c06cac5bf33f432c8c0a5549d1d1b29c5e2589024d27800a0"
                   "c235a61532c203cbc406ac6ecf63f52ae771b97c08e4b108ec916900e5a1"
                   "1b1d48cca86ca5a5a799ed32e99c815cef04cf8eb55223bfd4d9c3449264"
                   "b60061bc3684bc82");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("db9eae66bcacb01ba8029fbac5fef13156aa1a768c3dff259ee5ae6b1c"
                     "c34c2280374e3d065fed67bddad3e7ded21bc75d397835bf52b4292b74"
                     "5d48c30bbb0935df04168c99d3f54d83d7e7bf8fc8108a9ba0d983f307"
                     "93c7170f5d07bf2147221d38b7f9624c0c7f8312820f3de2550357f82a"
                     "8227e0aa99572c708de96825");
        bigint_t S("1ee03bd2a0440ef89c0f19fa03c855074e16f6ed426ca35bb76313887896"
                   "9a814be2f16f3dbbf73d2fad78d42a8ff8aaa342f5fcb08d2dde74525ba5"
                   "b2c65ef4e33d61774545886f7f217eb681a54b42ef7df66ce943227021c1"
                   "b5709b7a554a64141c1fa3609ad25fa8317068369b8990b8439468810440"
                   "66746c1203b57acb");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("2e22656e48856240b0f655d2022d69ac731a1c63151ba36e57728a35cb"
                     "f69c6b2d08029ea0c9af0c3a8bfbb9c3858dbebaca7fe4997cbd715b3c"
                     "ae423ae6171abd7b3c3af25f4832f8c5eef6c1675134167a21e5dce107"
                     "2395a1443a2a959501e763596bc7aef559f28d78b2843c7bb690124c8f"
                     "65b34b290f879344216bddc8");
        bigint_t S("c3a66796c87319c01a8355102562029797d695de50b46bfa39c6d9a4c064"
                   "2b6de80272481b8785b0ff3ba4571bb45cd6aad9c5fc1383c79f6b7a13c4"
                   "e67f594f81a81157de6247b896345de1aec94adcccf126599132846ea139"
                   "8585499be4e759a1f7757d318ccf41378b374275ad4cbcd1077476cbb06e"
                   "a42b36289b0f80bc");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("cf785002292f1ef9645f2f2cdc0455b29d0ba1efc91fde0a3ac05fa169"
                     "95fbbc8e03550009fe6126011f23ab4bf7bac88da49ea4a3a126c941e3"
                     "4dd4da21b571c173ec878f1eefb0c4aeececb1464c969535f19706c9bc"
                     "d9926853ffa7dc2153b0ef9eff8138d081f5341259d5991404fce6232e"
                     "b93faa247578f98896704648");
        bigint_t S("59ed09ae9cb743146781a1aa58c608fe107e2f6a1767d774a84eeb00f845"
                   "86e4bd0efe7f201be1cfc4c67b73441fde816b91530270c62a34d86e5c8c"
                   "dad4f0d0b90b53aed9bd31c7b43e1d7517f413c8b10232ab7d7a270f250f"
                   "09817721611f6cc96137e8067e8cf77a7ac25bfa8f8f157c4eddf051da69"
                   "be6284e877aa435d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("302219bc6c07e628f352a45091719bb0e655875fb97691388f187e377c"
                     "cd27253a3c29787820dd7574948b97cc8dda3492a88b5e9e2667082834"
                     "7669967072a692aa7bab60e70c596eac2ee72d79ec82209306373306ea"
                     "134debdf8928e7841361f4791ccb645c1e0923f7085c3cb7e2b99e60b3"
                     "1f2100a327c4225ddd3843b7");
        bigint_t S("4415ec9cc208f5c981528821637bcb0ebbbfa08418ff935667efcfc088c3"
                   "941b978280bb1c246762883559461334b0ef26eca0e5d4ab72059fb5dd59"
                   "aff3ce5d0b4913e19797561116ce3934bf5f362b88fae1bc9ebda447ec32"
                   "ff31d26eec3951eb87f08412bd3c3b7a23b44d73c6c2eb816859f67becf8"
                   "df118c382c917d42");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("1fd68998e4c34ded43d2e51b1680a6b3fdfdd576033b6a0d7f4413fb39"
                     "d27ccfce19ce9e56e876b043f0277a8c6322cc0cc2b54c25f00123a88a"
                     "01f6f97bc8186eca0afef9b7def30707c20ca79ce05fef4890709be31d"
                     "ecf89bcc9a018c1a8ec79146a016590f1c55bdbfbbae5068d26f386eca"
                     "b03d45d037207ccd982efb99");
        bigint_t S("63b946b4f8e61f3d5d09ccf379cc8b6e4b65070a96144e13f1f68c17f916"
                   "45ddd70b32a976a5288f7484573c46687a82eba49bc4216c9328d6ce8afe"
                   "ddff5b123ec0f495d95be89a695d0378241b005302fb85a32429121d2009"
                   "97d759c47f9fdd4e29c86dea2a7f2dda2aec22ad48f67f6f3fdd3791ba84"
                   "1fc971403d86ced7");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("90d0d6a410f8aa868b64128c039cc698a3b2e36700744969a0e741d0b9"
                     "f164e73a56b8a60fe61b83a1f4e58a9d09c9b0f7dafc652e789968d1c7"
                     "45df42310fa7ba9b99e98d987deeb0ddea3478f68ec1a437b9b79ef4bf"
                     "29fef59e2d5024e0321789c3941278986ebd0969b01bc9de71b3af6186"
                     "d85281532e5894e16a32a131");
        bigint_t S("bca2b39e5621f6f20073775067b0b7fcdf3bac402416f2c335b7296ebff5"
                   "14ff21e896e8aeaa4701ce131db5d932a16dc0ceae235d819b379a42c91d"
                   "9f9e9db2e8650701f2e6961035a89b3394a65fc2826a30e7cafd0d8726b1"
                   "89102e9a9c8e1aae152808aa2d1a6b8740b6ffd862366cd07174513929d8"
                   "b65702046447977f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("644a3de6100dd8a36c841446b9376b345d478a329450a66f629313c5cc"
                     "55133c4782ecd071963d74ffbd9156f63935651f054711b4b8105160ab"
                     "94eb675e66f02018c185acced5e9e2465fb4897d7c5dca45bfffe43281"
                     "5d22b778230b8d8c6fd854e58f2b7d490dfd67a5e8d8c8e925474f1986"
                     "8c88a19a90ffeaf620a82ac1");
        bigint_t S("481157d65c91a09b871bc9f704ac2c31b1e6b097e0d49e291e037c11e707"
                   "f48ac166558e9e8c114ee444c4d649e7e2cf7374269b036fea99bb3566d1"
                   "34d7ae5dd4141f7326884746d396be3207da2d9f56862585993db42ce282"
                   "aa2f048d12bccff12f14b9d1ac4e33d371fd4c0de1563c978826468e6a37"
                   "99f64d3985d4689e");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("29e9cb3482e9eda5e6d714a2dc730d91bdfb2d42b14fe82ed953a0fcce"
                     "95bb4a4b30f8a926f003e985b647984e1e03c5df60a009007442c9157a"
                     "9db599580a94c7a5d9449ac6e9fcf85677d52acc348990579a7a930d7f"
                     "f6da45ebb9c14127b30711160cc6b3c866a97f4d6259c3c76538aac39c"
                     "5262ca3d917d321030983a2f");
        bigint_t S("3c23c9e5ab29b8102e03932934616481013789e19dbe6604ee9bf0a64ccb"
                   "5e3b73fe747ce8b8f1efce2a9907e04199eb211ceb90be284ba71374a942"
                   "23a07ea9a739ca72faa7ca4da29bc1c044af61e4c563b6663af2050e2a00"
                   "04fe9e883951300a25a45ffc3bab8884d9f0f2279a26cce7142dd6776862"
                   "ab63ad636c9acc95");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("467e8ea634f7995dc46c11b8ab0b7508894681e81c3502c3b335e897e6"
                     "d69df885f49557ce232784e3519b727ba6843bd7af5063f8bc1d610f86"
                     "ce5b35155e325ce175be8538395b34df67a421fca27e31330b59a41011"
                     "b290a58bdc8e740401b38f5564c2fd7ae89f609ed607d578db7f1cda50"
                     "8af987be1fd946a25ab9346d");
        bigint_t S("7fb485a62f837a4765308ed005a61fb821a9ddfa6bab4e76548342e8c2a8"
                   "53ba1ab720cc37e3e3661408a36bead4ae0207c505b8cd9fa67f0e73ce29"
                   "bf66d1873c98f9b4849e4ed98727b206eabb9fa4cbcac1eb52e20c3d9b12"
                   "443dd1e746a755babeb340870e660112f2ce186539e943ee7fb335adbee0"
                   "45d66a3016e8c439");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA384_RSA_PKCS_1024)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(128);

        rsassa_pkcs1<sha384>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 1024);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha384>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1024);
        EXPECT_EQ(f, true);
    };

    bigint_t n("c8a2069182394a2ab7c3f4190c15589c56a2d4bc42dca675b34cc950e2466304"
               "8441e8aa593b2bc59e198b8c257e882120c62336e5cc745012c7ffb063eebe53"
               "f3c6504cba6cfe51baa3b6d1074b2f398171f4b1982f4d65caf882ea4d56f32a"
               "b57d0c44e6ad4e9cf57a4339eb6962406e350c1b15397183fbf1f0353c9fc99"
               "1");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000"
               "000000000000000000000000000000000000000000000000000000000001000"
               "1");
    bigint_t d("5dfcb111072d29565ba1db3ec48f57645d9d8804ed598a4d470268a89067a2c9"
               "21dff24ba2e37a3ce834555000dc868ee6588b7493303528b1b3a94f0b71730c"
               "f1e86fca5aeedc3afa16f65c0189d810ddcd81049ebbd0391868c50edec958b3"
               "a2aaeff6a575897e2f20a3ab5455c1bfa55010ac51a7799b1ff8483644a3d42"
               "5");

    {
        bigint_t Msg("e567a39ae4e5ef9b6801ea0561b72a5d4b5f385f0532fc9fe10a7570f8"
                     "69ae05c0bdedd6e0e22d4542e9ce826a188cac0731ae39c8f87f9771ef"
                     "02132e64e2fb27ada8ff54b330dd93ad5e3ef82e0dda646248e35994bd"
                     "a10cf46e5abc98aa7443c03cddeb5ee2ab82d60100b102963189797027"
                     "5f119d05daa2220a4a0defba");
        bigint_t S("1689a8523919ac77cc997ebc59cb908872d88b2855a309ead2779b888b22"
                   "b4232da9b93bb19b32c1db77ad738c6e43361e9eb6b1a37c49a8f3c7c7ae"
                   "7e784d19a62138741293e49b1831c0c3617eb43c56706d83314953470636"
                   "441086419ab9e6fd1ec4f9d5cc6544815d1e02ed96a3ae64c6998b2cf238"
                   "e79a12164352d12a");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("db9eae66bcacb01ba8029fbac5fef13156aa1a768c3dff259ee5ae6b1c"
                     "c34c2280374e3d065fed67bddad3e7ded21bc75d397835bf52b4292b74"
                     "5d48c30bbb0935df04168c99d3f54d83d7e7bf8fc8108a9ba0d983f307"
                     "93c7170f5d07bf2147221d38b7f9624c0c7f8312820f3de2550357f82a"
                     "8227e0aa99572c708de96825");
        bigint_t S("7b286c61b67110e232533fd9d1ac890b52ced71c9029d355c3f34c90c5fc"
                   "e068807e424fe948554ec3946f8d4453596acd29e2d44b45e3460239c4cd"
                   "92426bbce7d1b0f8f93141fd9f2f80a506947bf5018650fa0babdbb2dc19"
                   "39622ea8790a1d165b01c92ad91541e2aceb8a775b5ca4bf225b8b03d99a"
                   "2405bdb7b2fdbdbe");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("2e22656e48856240b0f655d2022d69ac731a1c63151ba36e57728a35cb"
                     "f69c6b2d08029ea0c9af0c3a8bfbb9c3858dbebaca7fe4997cbd715b3c"
                     "ae423ae6171abd7b3c3af25f4832f8c5eef6c1675134167a21e5dce107"
                     "2395a1443a2a959501e763596bc7aef559f28d78b2843c7bb690124c8f"
                     "65b34b290f879344216bddc8");
        bigint_t S("98a7f89db0932e94cb1d38bb72a85ce4143757c449f85cf891826c743072"
                   "48aa6a0a32c45a10ce4d21bd889b6716d8b323d191952571707eb73313d0"
                   "af7324a5aa497b3d045d44877d4d8ace8bf1cdf0345dba34d0c1a79935ef"
                   "94dcc60dfa980a4b1e40b6d30113d557789118fc1eea5f5c3f9f49acc161"
                   "9643685e6dde5827");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("cf785002292f1ef9645f2f2cdc0455b29d0ba1efc91fde0a3ac05fa169"
                     "95fbbc8e03550009fe6126011f23ab4bf7bac88da49ea4a3a126c941e3"
                     "4dd4da21b571c173ec878f1eefb0c4aeececb1464c969535f19706c9bc"
                     "d9926853ffa7dc2153b0ef9eff8138d081f5341259d5991404fce6232e"
                     "b93faa247578f98896704648");
        bigint_t S("584abd1f5e146b2b01b12f1c6ebbbd5873980282c63c90385c2f7040b344"
                   "64353bce9ddd9b573d334f7666098e7a7220a4539f4b4eea2fe0320f97cd"
                   "cbd6b3dbdf76c396a88e8bc73c37a832a86bc1453ad9f9fb7de135442d27"
                   "080bb2203917a8cef60bc3a55ef73e2ad8ce880bb5e5ba6365a7b354b3e6"
                   "38ee095d22197ecd");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("302219bc6c07e628f352a45091719bb0e655875fb97691388f187e377c"
                     "cd27253a3c29787820dd7574948b97cc8dda3492a88b5e9e2667082834"
                     "7669967072a692aa7bab60e70c596eac2ee72d79ec82209306373306ea"
                     "134debdf8928e7841361f4791ccb645c1e0923f7085c3cb7e2b99e60b3"
                     "1f2100a327c4225ddd3843b7");
        bigint_t S("766e1aa607e79968eec237369b4c9140aa6bfb8714b3d97d058a5464482d"
                   "ed41f5036f852112242a3ef9e97e5c31b02622c72f7f37dd47b7eca8b947"
                   "ef55d3375bec1618b09fcd982e488486a885165f189aa02715100bf4e09f"
                   "eadfc6c2767145131ca87ea0a20f475d6d44b6298443e9dc49fc70b318f0"
                   "4188daf5ec487ecc");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("1fd68998e4c34ded43d2e51b1680a6b3fdfdd576033b6a0d7f4413fb39"
                     "d27ccfce19ce9e56e876b043f0277a8c6322cc0cc2b54c25f00123a88a"
                     "01f6f97bc8186eca0afef9b7def30707c20ca79ce05fef4890709be31d"
                     "ecf89bcc9a018c1a8ec79146a016590f1c55bdbfbbae5068d26f386eca"
                     "b03d45d037207ccd982efb99");
        bigint_t S("84f034124c8280ccc4f97e9a326d57e03ab28fbd84562a5f4bc0ae66a888"
                   "b64c4c9aa272ef567240dfd633560a79c83974f628444df337de9d4e26d1"
                   "0964c78322de3053f00543c0b135cf952bdb23c3bf02c8c6b8d3acc9033f"
                   "b5ccf266394f4c8cdc4a44824d7dd38697600dcb62247f6a1b69ac14de06"
                   "2d7c66f18fe62d7b");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("90d0d6a410f8aa868b64128c039cc698a3b2e36700744969a0e741d0b9"
                     "f164e73a56b8a60fe61b83a1f4e58a9d09c9b0f7dafc652e789968d1c7"
                     "45df42310fa7ba9b99e98d987deeb0ddea3478f68ec1a437b9b79ef4bf"
                     "29fef59e2d5024e0321789c3941278986ebd0969b01bc9de71b3af6186"
                     "d85281532e5894e16a32a131");
        bigint_t S("787b005e95f9642ba27e5f4d365275bbbbac72bd1cfe292ea78016700aae"
                   "7af3c9e0e20bcfa54dc008c65a3b91c6e17c67d6229086c5a6f677e6ded9"
                   "6b88f6e8541c63f664a38f4cba641aa177176f9a3e45a67ffc7608d24f4b"
                   "44065e09c2744b7be4d349afb0e8f2e7c5106816a46c745e61a572bbd5e9"
                   "8cb6b8d34c6c682f");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("644a3de6100dd8a36c841446b9376b345d478a329450a66f629313c5cc"
                     "55133c4782ecd071963d74ffbd9156f63935651f054711b4b8105160ab"
                     "94eb675e66f02018c185acced5e9e2465fb4897d7c5dca45bfffe43281"
                     "5d22b778230b8d8c6fd854e58f2b7d490dfd67a5e8d8c8e925474f1986"
                     "8c88a19a90ffeaf620a82ac1");
        bigint_t S("599b511fc8bfcae44054a16c299b5f3c64b74c760fd182292b31f52204c6"
                   "27c853f6fa2a753216026d312574f6adde4ae829680d28dc1c3253ffa96f"
                   "56d81e1c4b5dafce91e809d9f58da72a94e29dcad52e6759a16a06304e75"
                   "e641cce626154edeef364c62aea5284ba40270e9050e81cf5a7dfda67a70"
                   "22fb4dd5514e3143");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("29e9cb3482e9eda5e6d714a2dc730d91bdfb2d42b14fe82ed953a0fcce"
                     "95bb4a4b30f8a926f003e985b647984e1e03c5df60a009007442c9157a"
                     "9db599580a94c7a5d9449ac6e9fcf85677d52acc348990579a7a930d7f"
                     "f6da45ebb9c14127b30711160cc6b3c866a97f4d6259c3c76538aac39c"
                     "5262ca3d917d321030983a2f");
        bigint_t S("798638df8cc53062b8a312e906527ce8e3a6b2b6456992c599e82cafcf1d"
                   "dfe6671674202aa61fcb9e83dc1f6bf93d2edb7651dcf869fed987477c26"
                   "d0a2a7a2c94dce4fe29849c26d477f782a40f2ce1f4050b5bf3fe3e70d00"
                   "efd284dcb9f52990149ec8c5b7d229db841e32741a74fd36e105e67c1814"
                   "c08ea9aa44f2ab26");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("467e8ea634f7995dc46c11b8ab0b7508894681e81c3502c3b335e897e6"
                     "d69df885f49557ce232784e3519b727ba6843bd7af5063f8bc1d610f86"
                     "ce5b35155e325ce175be8538395b34df67a421fca27e31330b59a41011"
                     "b290a58bdc8e740401b38f5564c2fd7ae89f609ed607d578db7f1cda50"
                     "8af987be1fd946a25ab9346d");
        bigint_t S("c700283557dafd1f8834ec0b7a7ec733719cb0be6edb19f1064ee94c75b8"
                   "d9d49458fa184f7d6dfa5a6da2d2ebeef0650d8af35447823c83a7737824"
                   "509a7425339caf99101cd1fe3db883fb98d172127e30e38d1d6f9e365493"
                   "7cd68cbb4ea228c816064fa8ca0950c7e7b6ad25045574a6a4063b63f074"
                   "66b2cb5d7311b7cf");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA512_RSA_PKCS_1024)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(128);

        rsassa_pkcs1<sha512>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 1024);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha512>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1024);
        EXPECT_EQ(f, true);
    };

    bigint_t n("c8a2069182394a2ab7c3f4190c15589c56a2d4bc42dca675b34cc950e2466304"
               "8441e8aa593b2bc59e198b8c257e882120c62336e5cc745012c7ffb063eebe53"
               "f3c6504cba6cfe51baa3b6d1074b2f398171f4b1982f4d65caf882ea4d56f32a"
               "b57d0c44e6ad4e9cf57a4339eb6962406e350c1b15397183fbf1f0353c9fc99"
               "1");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000"
               "000000000000000000000000000000000000000000000000000000000001000"
               "1");
    bigint_t d("5dfcb111072d29565ba1db3ec48f57645d9d8804ed598a4d470268a89067a2c9"
               "21dff24ba2e37a3ce834555000dc868ee6588b7493303528b1b3a94f0b71730c"
               "f1e86fca5aeedc3afa16f65c0189d810ddcd81049ebbd0391868c50edec958b3"
               "a2aaeff6a575897e2f20a3ab5455c1bfa55010ac51a7799b1ff8483644a3d42"
               "5");

    {
        bigint_t Msg("e567a39ae4e5ef9b6801ea0561b72a5d4b5f385f0532fc9fe10a7570f8"
                     "69ae05c0bdedd6e0e22d4542e9ce826a188cac0731ae39c8f87f9771ef"
                     "02132e64e2fb27ada8ff54b330dd93ad5e3ef82e0dda646248e35994bd"
                     "a10cf46e5abc98aa7443c03cddeb5ee2ab82d60100b102963189797027"
                     "5f119d05daa2220a4a0defba");
        bigint_t S(" bf3ff2c69675f1b8ed421021801fb4ce29a757f7f8869ce436d0d75ab74"
                   "9efc8b903d9f9cb214686147f12f3335fa936689c192f310ae3c5d75493f"
                   "44b24bc1cd3501584aaa5004b65a8716d1eda7240ad8a529d5a0cf169f40"
                   "54b450e076ee0d41a0011c557aa69a84a8104c909201d60fe39c79e68434"
                   "7ef4d144ea18f7a4e");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("db9eae66bcacb01ba8029fbac5fef13156aa1a768c3dff259ee5ae6b1c"
                     "c34c2280374e3d065fed67bddad3e7ded21bc75d397835bf52b4292b74"
                     "5d48c30bbb0935df04168c99d3f54d83d7e7bf8fc8108a9ba0d983f307"
                     "93c7170f5d07bf2147221d38b7f9624c0c7f8312820f3de2550357f82a"
                     "8227e0aa99572c708de96825");
        bigint_t S("398cbaa07bc4b3432f0ad0ef54b2402c2d25c0c46f7229fbfcba4c141cd0"
                   "db76aea42c2436757b16f7397c570e891cd4e6f0a052d0a910c8537d4eff"
                   "b2d07f06890581dde8bad7f77f8781bdc052c0caa50d94d95897508d6848"
                   "f5e26e5975f1a63c03f45bd5479bac0a40a3e4a1adfef847c3e9e9f7612d"
                   "5034cc688f019b86");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("2e22656e48856240b0f655d2022d69ac731a1c63151ba36e57728a35cb"
                     "f69c6b2d08029ea0c9af0c3a8bfbb9c3858dbebaca7fe4997cbd715b3c"
                     "ae423ae6171abd7b3c3af25f4832f8c5eef6c1675134167a21e5dce107"
                     "2395a1443a2a959501e763596bc7aef559f28d78b2843c7bb690124c8f"
                     "65b34b290f879344216bddc8");
        bigint_t S("4af9876c2a520a79282226d953505b166a67f361545a65a980abfe43e8d0"
                   "9bce6d89892e556313351cc3312e214517ea6d62c80c734eb276074e6467"
                   "5e53289ca41840b4d7c9b225183b8162a6f0761bf1a71341b9482cc15ad5"
                   "16bda050da93382567998f844529131b1b560294060b36171a4e7118a556"
                   "9beff9bea078385a");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("cf785002292f1ef9645f2f2cdc0455b29d0ba1efc91fde0a3ac05fa169"
                     "95fbbc8e03550009fe6126011f23ab4bf7bac88da49ea4a3a126c941e3"
                     "4dd4da21b571c173ec878f1eefb0c4aeececb1464c969535f19706c9bc"
                     "d9926853ffa7dc2153b0ef9eff8138d081f5341259d5991404fce6232e"
                     "b93faa247578f98896704648");
        bigint_t S("5a9b01a6d3e08d45d4c359502ec613489b733cd5a9c385e2d3e775949963"
                   "41bfe3475e1594e6161c12378ff2e2928373e247ab4c0aafecff220339b0"
                   "f063473195edf91725ae5e114714dd75de8d2f6eb7865912394c849d04fc"
                   "dbf97921ac1ca79c7859849e1ba09f396c2b291f9b1f11a5864baa11f09f"
                   "5c24164ee2422768");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("302219bc6c07e628f352a45091719bb0e655875fb97691388f187e377c"
                     "cd27253a3c29787820dd7574948b97cc8dda3492a88b5e9e2667082834"
                     "7669967072a692aa7bab60e70c596eac2ee72d79ec82209306373306ea"
                     "134debdf8928e7841361f4791ccb645c1e0923f7085c3cb7e2b99e60b3"
                     "1f2100a327c4225ddd3843b7");
        bigint_t S("5b5a22bf6be45cf311c78cbce08d9c499b239985e3ce980da04974c97d16"
                   "a21c0c38666460546a9ef141cbbe06e48d1ae8931f9247fa42114220be91"
                   "acf8d86e21ee3f36bed644b1c9c4e967099bfd190676e0b12978e9b0c996"
                   "afa07065b07701bdca9e6c0ca278b88894542457f0fc02b938525ad6b530"
                   "0fb52c9cfc5b8dfb");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("1fd68998e4c34ded43d2e51b1680a6b3fdfdd576033b6a0d7f4413fb39"
                     "d27ccfce19ce9e56e876b043f0277a8c6322cc0cc2b54c25f00123a88a"
                     "01f6f97bc8186eca0afef9b7def30707c20ca79ce05fef4890709be31d"
                     "ecf89bcc9a018c1a8ec79146a016590f1c55bdbfbbae5068d26f386eca"
                     "b03d45d037207ccd982efb99");
        bigint_t S("85ae6a4f4f6d0b8deabcb65fe319a51403512b745abef4fd306a2fbe0085"
                   "80da8a1e976713713bcce3b2a56ca2910743adee058fd6e3dd0cc0174c15"
                   "ed1d8e1355161e920493621daef96c3f74105e2b65b9621ce7a1924a649c"
                   "13083ebdff4cd20ca1f72596c763d03bb539cfe45a48bd161574882ef608"
                   "54a60ce4cf38d1fd");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("90d0d6a410f8aa868b64128c039cc698a3b2e36700744969a0e741d0b9"
                     "f164e73a56b8a60fe61b83a1f4e58a9d09c9b0f7dafc652e789968d1c7"
                     "45df42310fa7ba9b99e98d987deeb0ddea3478f68ec1a437b9b79ef4bf"
                     "29fef59e2d5024e0321789c3941278986ebd0969b01bc9de71b3af6186"
                     "d85281532e5894e16a32a131");
        bigint_t S("4895d5bddaf911a9ff22731726e4d68a4d544721baeb82bea8444797aabd"
                   "45f99f9a72471737a5f6db09c93ed07728f45fde8110e1f93d4b63d08d4a"
                   "87398f469ca5aea8267afe9fc571b7eacac0425166b9a0464cfc64d7fee8"
                   "7aed80f7c2bb825a03799106070533a97344c8cd63d5bb8f6707198fe996"
                   "55a589272a0e5af8");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("644a3de6100dd8a36c841446b9376b345d478a329450a66f629313c5cc"
                     "55133c4782ecd071963d74ffbd9156f63935651f054711b4b8105160ab"
                     "94eb675e66f02018c185acced5e9e2465fb4897d7c5dca45bfffe43281"
                     "5d22b778230b8d8c6fd854e58f2b7d490dfd67a5e8d8c8e925474f1986"
                     "8c88a19a90ffeaf620a82ac1");
        bigint_t S("9000dea9922c03ffd3a1ca689dd4b98c368a128f5153b2895ce995f1771a"
                   "0816a09fd8b493c460f29e49e1ab6a3867eec293f5d1fbb77fbb77c4a28b"
                   "b5b9e5d5fa06550edf365b94ed4daa3ab981a33d7c0015bbed2572f3c5b0"
                   "9f4f1922e5626732c8e6b5d31e5fef75d5a9026ca5784dba571ab72e3cf7"
                   "0df4ebfa6ad7b69c");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("29e9cb3482e9eda5e6d714a2dc730d91bdfb2d42b14fe82ed953a0fcce"
                     "95bb4a4b30f8a926f003e985b647984e1e03c5df60a009007442c9157a"
                     "9db599580a94c7a5d9449ac6e9fcf85677d52acc348990579a7a930d7f"
                     "f6da45ebb9c14127b30711160cc6b3c866a97f4d6259c3c76538aac39c"
                     "5262ca3d917d321030983a2f");
        bigint_t S("6244f168e5f8efceb00f33e5f4eaa8212b1b416ba7f1f95948e476755d53"
                   "2ddeed5bb84902df7750f775dc53a151737b39923dbfed374616f944b5f7"
                   "3d63e02d9007ca1e9d3333e77f200d8b021912af9122f861740d0bdd1300"
                   "75d9f112c99fd613f9a2036762dd6237b5c91b33fecc366324536195acf5"
                   "552147add0ec1c8a");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("467e8ea634f7995dc46c11b8ab0b7508894681e81c3502c3b335e897e6"
                     "d69df885f49557ce232784e3519b727ba6843bd7af5063f8bc1d610f86"
                     "ce5b35155e325ce175be8538395b34df67a421fca27e31330b59a41011"
                     "b290a58bdc8e740401b38f5564c2fd7ae89f609ed607d578db7f1cda50"
                     "8af987be1fd946a25ab9346d");
        bigint_t S("5d5cf180bdac8ae14a50c4e8b57b88ba68b1c3527d34a63fd390f33b8b02"
                   "5a75f7ce824f1e20de814991364a97c2ab9e6c88cd196abf6c4a0ac66013"
                   "a73db9e94a988a4665a31cd4e0731808c88ec1457c481047f3e32b08bff6"
                   "560cbe85cec212239dc8b25467debe536a8ab1a951afcdcb2cfc411afd5c"
                   "189f4b0eabeec3f7");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA256_RSA_PKCS_1024)
{
    // [mod = 1024]
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(128);

        rsassa_pkcs1<sha256>::sign<>(plain.begin(), plain.end(), signature.begin(), d, n, 1024);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha256>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1024);
        EXPECT_EQ(f, true);
    };

    bigint_t n("c8a2069182394a2ab7c3f4190c15589c56a2d4bc42dca675b34cc950e24663048441e8aa593b2bc59e198b8c257e882120c62336e5cc745012c7ffb063eebe53f3c6504cba6cfe51baa3b6d1074b2f398171f4b1982f4d65caf882ea4d56f32ab57d0c44e6ad4e9cf57a4339eb6962406e350c1b15397183fbf1f0353c9fc991");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("5dfcb111072d29565ba1db3ec48f57645d9d8804ed598a4d470268a89067a2c921dff24ba2e37a3ce834555000dc868ee6588b7493303528b1b3a94f0b71730cf1e86fca5aeedc3afa16f65c0189d810ddcd81049ebbd0391868c50edec958b3a2aaeff6a575897e2f20a3ab5455c1bfa55010ac51a7799b1ff8483644a3d425");

    {
        bigint_t Msg("e567a39ae4e5ef9b6801ea0561b72a5d4b5f385f0532fc9fe10a7570f8"
                     "69ae05c0bdedd6e0e22d4542e9ce826a188cac0731ae39c8f87f9771ef"
                     "02132e64e2fb27ada8ff54b330dd93ad5e3ef82e0dda646248e35994bd"
                     "a10cf46e5abc98aa7443c03cddeb5ee2ab82d60100b102963189797027"
                     "5f119d05daa2220a4a0defba");
        bigint_t S("0e7cdd121e40323ca6115d1ec6d1f9561738455f0e9e1cd858e8b566ae2d"
                   "a5e8ee63d8f15c3cdd88027e13406db609369c88ca99b34fa156c7ee62bc"
                   "5a3923bb5a1edabd45c1a422aafcbb47e0947f35cfef87970b4b713162b2"
                   "1916cafb8c864a3e5b9ffc989401d4eae992312a32c5bc88abbb45f99ac8"
                   "85b54d6b8e61b6ec");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("db9eae66bcacb01ba8029fbac5fef13156aa1a768c3dff259ee5ae6b1c"
                     "c34c2280374e3d065fed67bddad3e7ded21bc75d397835bf52b4292b74"
                     "5d48c30bbb0935df04168c99d3f54d83d7e7bf8fc8108a9ba0d983f307"
                     "93c7170f5d07bf2147221d38b7f9624c0c7f8312820f3de2550357f82a"
                     "8227e0aa99572c708de96825");
        bigint_t S("5a616faeb00680f5c4ef633205040b497b5e5e226e4a8f493b1ec2a26fe7"
                   "a0971fed4e1b8f188eba8161266fe558eab539f903c0cd8ed18d21af77e3"
                   "0cb264c6214a175588ed542b5fbdaae99e924094e8c8a22366441d866126"
                   "433aa45cd37773b9dc3dc179ec8cf51efb8bbdedcd229cd2dc2334f5bf2b"
                   "0b00374350e6147d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("2e22656e48856240b0f655d2022d69ac731a1c63151ba36e57728a35cb"
                     "f69c6b2d08029ea0c9af0c3a8bfbb9c3858dbebaca7fe4997cbd715b3c"
                     "ae423ae6171abd7b3c3af25f4832f8c5eef6c1675134167a21e5dce107"
                     "2395a1443a2a959501e763596bc7aef559f28d78b2843c7bb690124c8f"
                     "65b34b290f879344216bddc8");
        bigint_t S("c24dd39dc4306bb0b06d548091714820f14a3938437ee76d5302f7006213"
                   "d388041b79db6125c337ce41d2086f2536026f0fffd20aa9dacffe4e601d"
                   "fe51310729ad050c4f396b0223492a63d9a011f70cebf8850f61f0ca2ea9"
                   "4800f51e35bbb077e99beaf12fc2caf1744700eb1e027bbb450308e16743"
                   "7272433ee7a765fb");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("cf785002292f1ef9645f2f2cdc0455b29d0ba1efc91fde0a3ac05fa169"
                     "95fbbc8e03550009fe6126011f23ab4bf7bac88da49ea4a3a126c941e3"
                     "4dd4da21b571c173ec878f1eefb0c4aeececb1464c969535f19706c9bc"
                     "d9926853ffa7dc2153b0ef9eff8138d081f5341259d5991404fce6232e"
                     "b93faa247578f98896704648");
        bigint_t S("b34c8e11cbfd2927bcbf442a6afbbd6ea5c20f5e47cf56c57dca93e10bda"
                   "bb20b958997b216b92d09fb92cfc3445346cc9e63721b73db06dc9e36727"
                   "b8daa5f16fa4959c977f6eefcd2fc9790c5e01f31f8190360d380f433252"
                   "6472e32b25e54781bdde98016bc25ef8697d5d73a6b7bba06fd3d8b9ebe3"
                   "c657b86f07028ced");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("302219bc6c07e628f352a45091719bb0e655875fb97691388f187e377c"
                     "cd27253a3c29787820dd7574948b97cc8dda3492a88b5e9e2667082834"
                     "7669967072a692aa7bab60e70c596eac2ee72d79ec82209306373306ea"
                     "134debdf8928e7841361f4791ccb645c1e0923f7085c3cb7e2b99e60b3"
                     "1f2100a327c4225ddd3843b7");
        bigint_t S("68edf786f166f9aa932816d70b7786e1d88128da0ce1212bba5b56fc8f8d"
                   "c21d54b5bd44401af2119c4a4f03cdbf75615cf08f3764fba03aed856ecd"
                   "01caa15ffd6a3604a612c81218f3c395b931fd5fe78031b674369e49b185"
                   "de6c00b6effdd8d10aab7bfc6671900c58c6882850de5a37b87c15caab18"
                   "b8cd6bf8e5132020");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("1fd68998e4c34ded43d2e51b1680a6b3fdfdd576033b6a0d7f4413fb39"
                     "d27ccfce19ce9e56e876b043f0277a8c6322cc0cc2b54c25f00123a88a"
                     "01f6f97bc8186eca0afef9b7def30707c20ca79ce05fef4890709be31d"
                     "ecf89bcc9a018c1a8ec79146a016590f1c55bdbfbbae5068d26f386eca"
                     "b03d45d037207ccd982efb99");
        bigint_t S("0687bca5c51bbddddb68d21a912b7d3f89a54517b7f3c6ea5da386b5fb6b"
                   "e8c7a172d6528b2dfe9cdbcb3f4cca1099a1003a69176eb3292a1571b845"
                   "1693e1554c6ec5e9d336771378718a9e822f02f783b9cee3c0f8ff265453"
                   "9c495478ba68cca6a6683303a104230e4dde1c35e194eba1ea50e7fe690c"
                   "b19d2b1118a87be8");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("90d0d6a410f8aa868b64128c039cc698a3b2e36700744969a0e741d0b9"
                     "f164e73a56b8a60fe61b83a1f4e58a9d09c9b0f7dafc652e789968d1c7"
                     "45df42310fa7ba9b99e98d987deeb0ddea3478f68ec1a437b9b79ef4bf"
                     "29fef59e2d5024e0321789c3941278986ebd0969b01bc9de71b3af6186"
                     "d85281532e5894e16a32a131");
        bigint_t S("13049092c675809b386459eccbe1c540a1a8e9cb674da9d11349f2c50e55"
                   "9387f019154216370752c8ae0a7c4b6e031f414f7a32a8ebf6f4f87d4692"
                   "caa44e535565e0c0deb9471411126f6dfed6dd860edd1e97468b7e144f8e"
                   "a6e0b5063d3ebea598b47ef9d6292ab0c05f5621d20bcb389d550bee84d1"
                   "90d79c7d8820a97d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("644a3de6100dd8a36c841446b9376b345d478a329450a66f629313c5cc"
                     "55133c4782ecd071963d74ffbd9156f63935651f054711b4b8105160ab"
                     "94eb675e66f02018c185acced5e9e2465fb4897d7c5dca45bfffe43281"
                     "5d22b778230b8d8c6fd854e58f2b7d490dfd67a5e8d8c8e925474f1986"
                     "8c88a19a90ffeaf620a82ac1");
        bigint_t S("43e64b245e65eada15e66dbcf12332ae80637917e65a68f9f9de45be6ce1"
                   "854a582634c139892d5aa29187801c7ca47af5ded85e5b0a32e825d706f6"
                   "eb0b2dffa2f80f69fecaf87aad0919765cc2cb2042e124eda7cd157bedd3"
                   "21cc12100f8ceb3b90b68da13e5aa65c2c3ec184c6abbbf86dcbffdf1ce3"
                   "6f1b5563e8b56044");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("29e9cb3482e9eda5e6d714a2dc730d91bdfb2d42b14fe82ed953a0fcce"
                     "95bb4a4b30f8a926f003e985b647984e1e03c5df60a009007442c9157a"
                     "9db599580a94c7a5d9449ac6e9fcf85677d52acc348990579a7a930d7f"
                     "f6da45ebb9c14127b30711160cc6b3c866a97f4d6259c3c76538aac39c"
                     "5262ca3d917d321030983a2f");
        bigint_t S("57b17157b4eaaf0a9bdcb9abe4b299a728e6df5f8e03d688037d5e1ed5c9"
                   "a66c20ac739e1c3516a4cf78155f52ed7d054b5c5fed534b80dd3fc92fc0"
                   "920eae695f2fb9626cada584d572927a00d612aa690b7f6051dd581cce47"
                   "48eab9b4d886addaa32c4ed7d7221862354556eb68197b05bebda90c3e01"
                   "e00e27c5ef547c83");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg("467e8ea634f7995dc46c11b8ab0b7508894681e81c3502c3b335e897e6"
                     "d69df885f49557ce232784e3519b727ba6843bd7af5063f8bc1d610f86"
                     "ce5b35155e325ce175be8538395b34df67a421fca27e31330b59a41011"
                     "b290a58bdc8e740401b38f5564c2fd7ae89f609ed607d578db7f1cda50"
                     "8af987be1fd946a25ab9346d");
        bigint_t S("2b1ffb370d518a82646d86828db1fc7e8bfe73ee878da120fa92737c9174"
                   "688995f2255b29e83b28c244cc563c9b33efd3f9f9e1638e2c16e24f2eae"
                   "19435696b2f4d1cf73064fc1cfccb2278c01f0979e7de7463bf8417bd698"
                   "6fbf1d34d382a978ce799582442afcc92b4fe743216b6f151f6a561d979c"
                   "f683cab6af2ff4c5");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA__1_RSA_PKCS_1536)
{
    // [mod = 1536]
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(1536 / 8);

        rsassa_pkcs1<sha1>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 1536);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha1>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1536);
        EXPECT_EQ(f, true);
    };

    //[mod = 1536]

    bigint_t n("a2fbfe82fccf847724640571bd393ca2dbe4e9a3ad85cbdad9151faf27b7127d37147cb7fdd3508d68b9a8b5b914cacc863b3e180665bad32c0c1ec8f9d87740fa5891de8f54085eb0424d2589f53adb20ac92eec98d9ce5378e9875ee7890f99b3828662471b24ba2caad362487030169dd127c8e6e7bf3c8f01f7ff5e09d3e91189a"
               "c6e37355dda7526dd5b63c6b32cc9f5b86251fdc9db8ba9e676f848a44e59c095550d4f4480295ae327ac7fb877a2835a216cc06c3786682d31b6dff19");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("068b7f86339042c5c50a963461d0a35877e39aa545c1d674989c924be698a1afb10f4bcdd8adcc718d47cd5854d11aaeb9623625615662f8f96f182a626c18a7dd95c26fd14a22d6e895950301a5fd21d10f550c2d40585155188f13474b04a94cee8df7e6b889c4e9b68145886eba209577ff4ad3072c72219b72b0803b308fb2e4d9"
               "a8bbd370088c4de857827136612f6e7731d56900318c433eda485a4798ef7be50a530f1f3f173fc251ee7192c3bceb598c109872fca098b9f9d5cc0001");

    {
        bigint_t Msg(
            "f4461d25951ea6965bbe2b16f1f9c7a2714e875f7ab12281838fe46b6f0cec17618f97b9d1803c9fe0501a5ce8ba8e2558498a7bbf1b420b26e428d2a3351b842bffdb2fa853c11b4b6badf624ee3f2df0704a9d3238856b854784303096e75afb9ed2cf411d9dd34718cc2bcc1c4182e1cab00dfb93546b47bfe0cf3db4b960");
        bigint_t S("0b23b64dc869d805a910eb63f4cddb682232c007b2e35d98f34a0a4d3ea8806723921920835b06792fe015925f7a8fef1fc4447ee4ab4a74fadccf4b9f24c1948aab43715824a0e28e61c20fc630e07d123fd3448825dddcd719baf06e54f481b133412a29a921a68ddb2ec2a9f4a5e61c7298eb1b183db926a13920045a4df086"
                   "d6d612abb39ed32c39aeee1daeb352df547b9fc3ba2ffe3f3e053459469f846c11679e505f07c020e505953ff518b2e7eee015c7c50cb877ab827c4b8b0609");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "460a92b19bb8c0bdf66aa46547df517eddb733d22d2af772cffaa54ec9a60f0210e16ff34c424c9bdc3d9d211a52312de0844efa4b0d99adbe5035e52a0f2f938dde1a0cb9d8f65c579057dd2b4bc3099a6c5b8d692fbd9ec79a2a908eb1e4a5c6eada236be087a96f4b18c20330cdbb1355a1b20ff82df0f1c0ca797cf4f6e0");
        bigint_t S("9aefedbd779aca14709410c5bad0ffeb1b7d94627042c096d33f114a12428a49431cc19c9ac32f28cf1b2dc441c090d3ec215a428a107d743200f2d255fc362cd9937fed7f526e520af21935d174a1516ba33533bf8de2986095d37d684ad42f440cc57b3a8b794bed5638a577a60bfe8c1cfa96d73f155c7d039712f4daf3679e"
                   "658f61437dfeb69be2586c6aa7cf7aa62838f670ad3b2aa375a49807357582d3eb52c959721ba6792a38e237cb3e47904aad3691bac8ca8d1be95f42429e79");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a737ccb2c336163e224d776dde7ef3033a510b4fd82e8fb12c0ab4593046665159d3dfbeca9b7963c91cc0666f56cc57a3e8ce6e4976e814b64d42a5674a5a75f3f89b113ff832c7f1a61d6827f9844c68ab4d97db9aebbbb66f763efa7d01be5eec28328816e8728331b4fa6e2fc4ea71e2b152f6643fad15b679ab4fa70434");
        bigint_t S("374359025e79a451956e52de8cac9c2b246e97962e5eefcfd3d93dc7fa88bf3fb70e2c3fa6d102558f5e2737cb0c2ba438ce852acf9d2fa98606630d0825cf1bfdacd39abeb6a687840e5f988d5c8658e8fe22d474c7a5c196897ba4005e7b9500863714cdae8e54a3dc75b0adb802278eab96a76c14ce0ae302129c8b6f782782"
                   "0afb94f3abe9d2f84f03731f05a24a03330f84755d99bd3df6a46fc67c4db4728d036c6d44089c12c00e2b330dcdf350ffbcaba7dd8297ca1cefa6946b3c3b");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d14aae46a4c4a9617145adfce65478f147b21824b68e505cb1f83d51dc98390601a0005855ff12d7fdd858d6d03209be5ff1cfa83205262a55e10570142ec18f02c12e1740cb07e7fb50904637db792fcb6f06ebadc06de670a380bb0bc48b91dc9780aaed91513975979e74a2b790eb95160518af29ed370b28f3584bc061b2");
        bigint_t S("158c1a3c66dd1f18aee57f857f693f69b7a44cd446fe7515939740def8c94abf1f803d41b838871908abf271e12ad7c0734f789c449778088a6798683846fdda8344de2c1565e160eca2bba476b4d59001b506afe5bc8b28a319a5902898a64d0e23eefabe3258be86d2994d64b02584f0bac1a52fed2da6335899d24eee2d02b8"
                   "4d68bffc3de9bccd2ce0239f95f7aff05c154968ff2322ec2d3151f10f77e870443435c16d6cada073d59722aff392927de28dad315cd0a5102b5147dcd855");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "88836b5a0a3200f8571635f5d0991e20d826cb5d93de28f8ed8d2239dfdc0b39eec840f3f452099e2278c03b4665366a4ae55281fd1bfbdf1776387e77e196c8ae3acbacc10698f02b63f4c9223d66a91bc5b358901fb946015b9b2039a71ea1d2c353abfbb5779679bd17c8ec8b778554b03509d3532ad4b5259146ad976b6b");
        bigint_t S("1af30b7601793c480e7ab8833f2c0220392c8a967506fd68c841670cb60513b01e119429a111d702a90f46478255d0095bf191f76665b4eafaf2efffd21b0dd36d0f0fe7a1d8b2abb9925be63a0d30ccc5b93e1c8cf64bb15b33571b4af04f751e79650e525723eb7f6d24f2e574e991a40582e24ef62a6ae87a860b4910aa5fd5"
                   "6551caaf7b5c9525ba1d00370e7fd34dfcd2c50beef90be5606b0eee7a6fe3c929da9f8a7dd1337454758a4a70d1045101d2573950309d4eb115e8a086ff21");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "fd31e83f7af727138a634e96505cbc803be96bb03697c5b71f1cb99a7b390dd468e5f69bd2a7e664ea44a5a9d49aa3eaa0cc8df27c9f1e5b28243854aa9833a2428875e5a038421d5546d66c888bb11eea4233e6ec6c686a2f7325023b0d54bd75638ca64b73e545b2b20a68d2f68f23ff948515ecc54ced1b4fa92d9861182e");
        bigint_t S("9b99a6e43c58b8336d357706c0bcf8b6aa65c9b1d806f93297aa0811b0535632fdb544086211a83040e5db9f8b75270044d8aa6dd68373c6848d5bad3ac7c6dbe00b62f1be8188b2d107b637537e76a9166e9f1cdd27c6f0bd919ef12ae2b36a8f9ac55eec950d2de819db689ed6cc624e850ffdcfc2811a59ce75c27ba1178ebb"
                   "2867a3a0d82546b4841350bd758177f08636de24d053ac3cbb249793203be65dfb49ce4cc54ae972cac77fad725828853ed5f39a898ecf2b4828c2b09707ee");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "f15cb6c5705a8d0e6f1092cd09ffd12ec8574888bbbd9c3ec84b0be344d8a2a1c17ab0db274a7a11076730042b3e201b72082a06db9374a1df465f1b0460b957af803492396a16474608334f52af184d8da48a49e9df2f9b710e8a0bdcdd2d3662752934ce42ca77d5fa81f6a6db228bc2344f9a0e2634f426ac76fbf5d96506");
        bigint_t S("86c9998ebef634c24205306608a50eaf0078eae3df0662c9897b2893333599c5b0185a86a21afc932cf66b8f7fea28719548095b2c76f12ab92451842e5f5afc32666364b5ec0a974bebbf1a1ef8655a5ab150023c355c3b6aecfd995fbefd657196e56c6e354df9180ea673a47789188c71eeaa04a391226d641be62a08799cbe"
                   "86c5cb3a6184e69d1d97c8de1dfaa07c5a1d224e26c3b0b5cd2d2277271a45ff38201c3cad98c7ee89151eb2f27801ce4e9adb9ab07b24999da423174d4575");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "ad354218cf73e07d261929339a4556da534509e317a33badd35e76ee777b455507bcea6eefb04ec598ec3e4e3897d304ee2c32d10e7b76aebaf1eb0bb21a7d716c64706668265191bee02c1ef9c5ff6427766fc53111bad6696aedc0629857e704b43dc9dfddcf28c07f5b6b82d5c14227ad2789286ced6f0851e3d72ffebafe");
        bigint_t S("72d7ba63e6b42de149413f08c7d49b5ce52c52447a31ced6c8766cf5a2b889ee826464591036e992207bf0ed0912fab8263f9a2727b66ba28226df526f369e6747995b9deb46ae1200b349e4b090a62458e03e432d4ea1abde5f6839a94776fd7311094a209ccf2d284ff95421930627d6f4a3b5bfe2deb7425ceb4a3efe8e82a1"
                   "84d137cd0a8db22d12f2621e1b33e0c13f46d85c4f3fc8a9138a69b1a9f137947fa68915ae3ceb842fd54615ec1e0224069fdf9443b224744c31ed1fdc9b1b");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "89de252d69eb32a94b80ffdc2bbb04e8f0f015e996419c2b8c6e269a5bee2179893ee357bd79ef3f23297430125c17ccedfbf2544758ddd44a06ec9b59ed39eef49de3fedce35617789acd1978c4162cb8da95b067608c7368bb9a27f847811392a52c080f52f097127e1281a31b896f1a3406e33f80326faf3736258224814e");
        bigint_t S("784f31890d7da137ba05ebc0d092e86ed6b40a96a0d24264f0a5ca6a8d863145acfcdfad099e0ca523e008bbffbceb8abae26046937e796e0e73b4ccaa07d9da317f1923912b2380a0966c347f942eb412e4103700975c9b3705120b9fac11305f29d514955d40b7c94bde537bf9e6963a74a7770ae353265e9f72614eafa4df3f"
                   "f91223c27e83709c15ec425fe897f6220c21b6127ab9cc8870bd9aaea9ae001b6422b7d6140899f2c5ef6e08c83ed3d62b0b4c310d2bdf6f9ab0168bb24dd0");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "14581fc3d8c6ea6a14c9fb216c86bb412d218744341bb0c20cdfc6a0569716846e44718b4b0e2f3fa7d874c36018b2511fe66fe73753493cd0fb37f944de1126c96b53e251de1e58aecc7feefafcf4bf36792c4c31e50f6eab10fe90bbca5c1a6ef54d30b1debd9d71cda50a00da8974bb0346ddea7c0a41e746a4cb8303fe49");
        bigint_t S("3aa8c0a814b9c7925f52f7e71bef180ffe91edfa68ec9957102ab94e51d7c3a07dbb69de143c89998e2183d935db37bcf72fed636196d749977228b613d4043bb5cf59011981a1e8bdaab5e3a2dd46e6932a5f78ae4191e032b3e11e5826ad13fec05ad982f45d1652e1960d77ad27bf9c469142790f64ef95d2088c0acfc7bb3f"
                   "d34098359e11a890cd8ff5084c7e0426e4c099f41ce9b2f509940c81d9abcf56aa144b88bf875d92b8ccda731925b02fc76775c5f41c557295e93e1390792e");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA224_RSA_PKCS_1536)
{
    // [mod = 1536]
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(1536 / 8);

        rsassa_pkcs1<sha224>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 1536);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha224>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1536);
        EXPECT_EQ(f, true);
    };

    //[mod = 1536]

    bigint_t n("a2fbfe82fccf847724640571bd393ca2dbe4e9a3ad85cbdad9151faf27b7127d37147cb7fdd3508d68b9a8b5b914cacc863b3e180665bad32c0c1ec8f9d87740fa5891de8f54085eb0424d2589f53adb20ac92eec98d9ce5378e9875ee7890f99b3828662471b24ba2caad362487030169dd127c8e6e7bf3c8f01f7ff5e09d3e91189a"
               "c6e37355dda7526dd5b63c6b32cc9f5b86251fdc9db8ba9e676f848a44e59c095550d4f4480295ae327ac7fb877a2835a216cc06c3786682d31b6dff19");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("068b7f86339042c5c50a963461d0a35877e39aa545c1d674989c924be698a1afb10f4bcdd8adcc718d47cd5854d11aaeb9623625615662f8f96f182a626c18a7dd95c26fd14a22d6e895950301a5fd21d10f550c2d40585155188f13474b04a94cee8df7e6b889c4e9b68145886eba209577ff4ad3072c72219b72b0803b308fb2e4d9"
               "a8bbd370088c4de857827136612f6e7731d56900318c433eda485a4798ef7be50a530f1f3f173fc251ee7192c3bceb598c109872fca098b9f9d5cc0001");

    {
        bigint_t Msg(
            "f4461d25951ea6965bbe2b16f1f9c7a2714e875f7ab12281838fe46b6f0cec17618f97b9d1803c9fe0501a5ce8ba8e2558498a7bbf1b420b26e428d2a3351b842bffdb2fa853c11b4b6badf624ee3f2df0704a9d3238856b854784303096e75afb9ed2cf411d9dd34718cc2bcc1c4182e1cab00dfb93546b47bfe0cf3db4b960");
        bigint_t S("7772129848c4967906da15a7b0044f07f150655e133983113851cf4d3cf5f0259b67988d3819a51475067929e146f0c31b3ca55b82715e6d18c06c78e13aebcd3c1be5e9f428f3aae7626c9b81881e559112e0e4a4084a3367ee285bed9155179b1eaef032aa2e01e79624b808adbf248f71673458a5a44eedb5816bc3d12e5d3c"
                   "b6bdc1f7f7fea8c8a9d077db8896ed51a342a67e5200701acb7aac81f7de33d4234b88144fb0dac3e8a357b2fa55e11e70171019c0661344b72f93d73bdbbd");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "460a92b19bb8c0bdf66aa46547df517eddb733d22d2af772cffaa54ec9a60f0210e16ff34c424c9bdc3d9d211a52312de0844efa4b0d99adbe5035e52a0f2f938dde1a0cb9d8f65c579057dd2b4bc3099a6c5b8d692fbd9ec79a2a908eb1e4a5c6eada236be087a96f4b18c20330cdbb1355a1b20ff82df0f1c0ca797cf4f6e0");
        bigint_t S("31a60196e3a4668d34a588e45042eec590105337f32fab966631c1beda84f1aa3439ee87d3a237e7e2c456dda48199afc0107e66ccb7c409a848b9ec3949be87d3fef92d0da5ecb0492457b21d598cd1c2db6a0419dacb7eee384c85eafcc9f28e04bf18c19bfbafb6fec7a5c3c3b557872895223620785893d9d2533c769ba4cd"
                   "36347b8701ab599da6f4322532c70f77fa01b80f2201c836b067ef4c8f766c5eb12c57cc30d4afce8bed30734a1b028db9d26de529b399bdf4baf22e078df9");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "b12c24b0a33a8ff0a141bbf14f650331803c7ffd9e9983e54da2696c4b2991049a39a539e2ee222c118a144344c6211fea66c8ce2610eb42765e8b029332d420984a596b6514a0e546c3e178d0a20be40ca808fcd84d4212899d66b0d58b6889f187c7aef65312058912abf8bba2cb6a2e2bc6ef7af8903cce8680dcdbdb5525");
        bigint_t S("52d07eb734ff3191ae0239e0938349b0249ed4de2aad93aa26bd065d1fd6932f7da689529929b02b329fe2f5ef7298189327636200c7eaf27aefd6916f597234322a255e086a772b7b26e269284d4205af6b90bdff0ae940138abed68fa4bce217d385059577384fb009b11e195e355ec25b81d1b8496f6443b4f8dc26edd95b99"
                   "ec3e2abb3797a6d398fe8739646fcda33c81437349d49809036c2064f4d89739fb97e4bea45172ce64a4f8392ce4f530b1f4522a10c8dfc8f7d6e1b91cdcfb");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "662ffccab13472140db866fa562f49ba20f0e976584339e429bb55481dc6c19f8582f11d1ad20fdae4b2f8c95bc788e9cd3a17bfffc743031711f09ab9398ec471ae9b97514768acdb8f1f182160059d2ab3fd2298b64e61881fa5e0bac05e22f25cff1e9dba05f0b8f7e4cfee20493f0001a2876a37e066448d7b026685b721");
        bigint_t S("03f94f4852da9e297bc4f343e7e03f5a1227e6c05f223359afc3cbd2eddfd4877429f4046571ce669bc7022187c5fcb8126cdcdcc1e25801f12898ab75c8891e4893f1d8d589747884359762bc7d96724f33799a8c774674a1d677a4c22df214e3db25fa1f09d486dcc0bf217d410a86f238f45fc80a0f5fc3370e01ca51655e68"
                   "ba14b88469a90c2b71db2291cc409fda548d2741fde484797e3790228e9fa5e84cb240b7059452cbe7dab64d638f9d589498019086f7c3a27e2f1f235774f3");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "fb91d32d2cbd4ffb4046a36de1bc73a6777bfa09a622aa4f174612d67c67053cce22f92dd9a9ad79fb710f6a292a5c5432f4896d48d0e2dfbc54f11c09a4a172149da3dfb25311266bbe51227eb03d5b240a80d71930433359ba0af2276dd81454699aeabea3c22a1796333fe8365de2d526a9ee222a177076df25fedfaaf44b");
        bigint_t S("11e023512d47599449fe547a6d92f11c40cf0fdb59535d44746e4cdd666ebdaa87dff752a0a744638a5df8f6b2a4a02789927ed87c5577bbb935678b1c034fbb569bf3f30e66c9a794a68499266e73ce1645c275a067a0d6db010a6d987c0174137e080374b193f98d34335bcba133d27df21fdf0303e28cb8e2863427e7edb651"
                   "a9df5200cf354c00d369d9ce1804f7b304b5997d6e587e71cc3f9f83963e1d54a9b0089ddaaa8990077cb509a55c89270c4dc17112f7738f1ef62198d014bd");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a70a6fc8945a7ce5a81892f9b16d809faeece60ad67d5598dc99b2a972bddbb10c188d8ce687861248c4e6ec09dbdba652dd7d04a4d7f79a1c05e897f64409e251c60f683109d3b4ca0ab0c0fcb62c0a1aa1dbebeffa812403bf77ba1a2d5005fe7bbb037ec7abf2b751b61d92e7bbf68ea0c865c0cd3b60182d27bce560f942");
        bigint_t S("8c14cd62325c489d3bd29fcfdecf1dff118d6247cf65c5e4c37523c77e1fcfb3c665b51c3b7c6e79b05cf1cb31854cf427d5991341701ac594abad6290ba127b93aaf05d9ea05ad8b816bfb28333826c4a302959c1ba5a093cc0319b3c71c4430263bc249d5572a6f93f5d12d6556a27aec1374d13634684d62f819fdc3a8b7bd3"
                   "a9585b38a63ac46ba9fd460d83c80acb75ade91346bce396300e6a7ce6ad18161c0a1439e558e09a513b839293541a0bbbd4b63b4a381eed075cb88d959d95");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "e84488e1d4b6d35f82767514d46d044eae850f73b260014d4e4b48a6f96db715b9442917e953686139144d75f63428748cff6133491a60906370ab073d6e4e80e673806be282830f2e3bcc1f7e09648fc99c8ea5079aaa533a9159c4ab31e23576b8d50648adee1168d13b3af45381476e43ddf37749a73dd9979b06cb1c3bdb");
        bigint_t S("5127c85058dc71a4a07592c85a86f3fead0c49d6f61b16cfa7ff17017b877453a75baeac9a9e83efa2e1f649eedc8e32414db78dcf39414d7f4a4537a476d3948adc047c3776ecf3166a8a1a65daff3f56df7866abfc5a6735ad755589032eab3216853fd69c3337694ff1e5a5737e3d73fe5d8fccb91d9aedbca20c4833914667"
                   "be4371cbc99442560a08b97816fc087aa5ec155205b4410721d0038e2ebc4808a215fa000c308fec9f2a84259eb3189f33f7afb262e734b13267740451d8b1");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "cc8cf669edf786837bfbb48dea4743c4d22527e4504eff9caa03e03f315954bc6cd5a2d07238507a989ce7b04cc0f65041a15959cd4abb5b8fec5a2c54310add3bce7dec1858c1bb17580a41a2a1b20084e17e374e772358f98409bcce454e9683a00e26a7fce22f4056a6aab29180bce20e1de69c7d3e7c32f37458663c27cf");
        bigint_t S("6772d4802912fdd4d321da4c15c3b75ad4220a88353549880010ebf43e3d35f671ef38173e9d4fbc2904d2fdae527817c9310b94ebefdc75929f602bcc188b039ce3d966eb844717496949664d1fcdd18b810481ecfaab6540b25aa571a342b9f5824d5f4d946893381e22a55f82b855a764c11dd780f6532d376e23163cde9e1b"
                   "a2594f5e1b602b1a69ada4edeb0c0a94a064d93b73018909fee5a3d21dbde8917e4803ddd18f0f80ea6497fb73d17124615a7a8920a66b854c270e0ef23bcf");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "9cf9206031fb19bd87fc50aa4d3b1bf3b86e66a331aa060f4b824a16f773d3a8fb80a685a0198047ab416aa38ff08cece1b646d80ca27d87466233c1c3ab5b8028dcebae42de322ff84e6c227ede9464ccf879564fa514e1deae19b7ef01ccdb0b5516637f11d87d556a7fb082ccd964617c27a5607b7324faad237ee53acfc1");
        bigint_t S("42a5d83fa6dcf0aefa64c3d651c39a088f4339952a6f4a32ce192a8dd6e34736936b025eb9e9ff35cdce3a4d6994b0d1e0a74649c612776dcadff5fe3d73c174f492cde03d16292abf082cb7b795795ff927e2b4c8cd89f43f24eab746c825a615b154b5c5ffb30d86ffb1c3d3b8b207bc1d29d312307529b9634783c1ec59c07f"
                   "45af4d5b5accde4a7c51a90488b3930d7c3c84ade6c5e1a875e2f384ccc9c85c9c847b6b53e34779562039d7e3ddb8338d19494609361dc36c6caf6b48c726");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "41fbaee4805d64635b050113023350daccb08964cf0d837dad2f25bfb00c1e6e1aafd5a0b682a3a1b3cc4195b2797c5f1e1a30c2bc2c73c6839d115ead8c39470e517533a5bddf2771a29322859660ff5ff5c39c7625153c121e032a60293fa0c6e775e6749681be3c90df00a31462823849293a2480d0a703bae36cf26a983f");
        bigint_t S("5f9e4625c58b861fddcc558f3de7d85c3643fc312e8d04d0e19ac4ea1305dea1b2b015e8cd5b8ae6c348f9e88dd51e569d9f0b8a365bfeb3d493102fb7a530f9cb69ce3385ab5cd0470fde3d451db82eb1f5f226d39f9cc5cae5e3ac6c7075b7170b1f2f3ddf49877511ee178e46ba5de75108a0c9f8bad1488823873d4949c70f"
                   "dfa1dffc4444401fdbd134d7a326edf0d0b11b41b217f7c388263f50237008e0d78fa022a14953acced92f1dc5d2475c958d81a5c1653b255fb566f8449f00");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA256_RSA_PKCS_1536)
{
    // [mod = 1536]
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(1536 / 8);

        rsassa_pkcs1<sha256>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 1536);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha256>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1536);
        EXPECT_EQ(f, true);
    };

    //[mod = 1536]

    bigint_t n("a2fbfe82fccf847724640571bd393ca2dbe4e9a3ad85cbdad9151faf27b7127d37147cb7fdd3508d68b9a8b5b914cacc863b3e180665bad32c0c1ec8f9d87740fa5891de8f54085eb0424d2589f53adb20ac92eec98d9ce5378e9875ee7890f99b3828662471b24ba2caad362487030169dd127c8e6e7bf3c8f01f7ff5e09d3e91189a"
               "c6e37355dda7526dd5b63c6b32cc9f5b86251fdc9db8ba9e676f848a44e59c095550d4f4480295ae327ac7fb877a2835a216cc06c3786682d31b6dff19");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("068b7f86339042c5c50a963461d0a35877e39aa545c1d674989c924be698a1afb10f4bcdd8adcc718d47cd5854d11aaeb9623625615662f8f96f182a626c18a7dd95c26fd14a22d6e895950301a5fd21d10f550c2d40585155188f13474b04a94cee8df7e6b889c4e9b68145886eba209577ff4ad3072c72219b72b0803b308fb2e4d9"
               "a8bbd370088c4de857827136612f6e7731d56900318c433eda485a4798ef7be50a530f1f3f173fc251ee7192c3bceb598c109872fca098b9f9d5cc0001");

    {
        bigint_t Msg(
            "37d7fee3d31934912167e1ffb35b4b2f159e84bd02513d65d8abdbc44ae3cd6355501cf175cf43d0186542d45325ee47d4548b3018cc9cd4de9efa7edbfb247caf55d66a18c1ddb365a029542e7e24e3433a42d0e267d9ec2df33427da06e39e2d2d54d523b8a840f161bf09ac2e950d4a98d228c9ec9cb2aef2b049b1cdb1d1");
        bigint_t S("092a257b50a4e4c8c7914e0d19c0088778e5cb4a815fd3a4eb59ccf3e71233934f9bcba2e03465f2c12d2100e722c703acc6bdb8626cac09f964f9f4ba527d9c49a6d6a2136873666bdcba0a297a3c38fe48731fce5a7276015c4d750de6fc550718544767c86df16b048cbd8ab8a878b6bab25ad642dd57102f323471e2fecb84"
                   "f40be00c79f75db6bf01ab070fb2874cf11a3d4e81e15f878b0797b534743e9666c93f782f0a11539c0154f406c710f1f5e24b490d7f468691ca28730a4802");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "15750e411ed6b1487e9bfae96aaf758f6c059ce1668af84fd45889eb512c687b5d1aabcc5d7f8949e338bfcb301131b3f780d5376494cdf95d28d3de085cda1d8d46f2961de46d66335540b81949acddafb71da068ce52d3952dbe2ee6562e5a2d60ad4866eda4c7f55c46562e402b361a103bc47820ce0603cf1284f302ac59");
        bigint_t S("45f6202a029a0b5751ca6aa81f8712da94646b05a44901f8f972834fbd8745e0716888e26fa9319eaf30951387bf9370a3e005c970dae2958e8c3aef1e4bba802af877140b39c467595147215f44a5726ffe4be17004d61d05f39505023a2942cd8c9bc0a2a8ef8b8abcc4bf02c694b5b2d90cc5f27aeff2f3078eb7dad4d0c292"
                   "d43801cdaeb8c59c5e97a6beb5fe03e320f9ffd5cddea1448d3d308b6d8bde90fefc1083c50f78e8f2e27c7c2a1b54d5dfd89d9a76a40b6dbe118d57336a31");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "369d20e16d4a6574d4f9ee511106bc494268f7b5eed982b73b38d3c2622b025c61081f22f0f6c90db54ffcf5bb492b5321d68f4b0fc8beeaee397facd92add414e6a7a902aba26fdd42eb0e7b8d6fc75f79e56935c73063793a725e99597af82678bd93404dda4ca985eb231eef558382c89f694be218327904a6c8280ba9fa8");
        bigint_t S("2352b15d1c536146660f89d7a6b54adc9d68c0d9444fa527321e19f5444f4118c32b8345339bb6ba35c852dd575db7e3695fbfefd42aaca046b6ce657137c9ed4d010a15faef7f60bd4e6befcabb4139964af96f6be6a9911dce31cac26b9ec7eb21be01b9068c8b03483b47874a6a3c15f08534ada689a9b4bb641b9dd8a12592"
                   "b8b10c7f437c6ce714c3097ee4049ce19203dcf5e0eef76c0f933bed0e624f8c4a2d357d2379c4e3a1d382def0dfbec85d48c6314bbd6918a583c4c0dc37eb");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "1f75ef4371178356081e4bab44fc3c706a706f6eecb580e0ebf93485fa105ef4256a03303242bc7c26195c700b63d34df180528e33edb0bffbbc0d920841b530af0f46cd2023167feb5ecf50c3395c9281b409726c8fe5d0b7bd6b4e4b1662a7c74c0243c520580860d5ec72f3e5a0e622f4db431f02e7bb26e498e14af52f02");
        bigint_t S("643a46e16763ecbcd7287b66a93c897d2e84d3e4eda24f2e518f04814d3d9d5aac3365d9541f5edcd165049b22a9066fdc9525ae6dd5e542830253f00864ce8b40c9389aa2857453f36996957e0ba5ec2618d47a6f514e2e0451684289baa89704a33f35f242c6f94ed286d0ac7301c35f4361e7b6df1232e787d081dab1ad98bb"
                   "bdcfacde5996138d52ad8c98843d3ad2458c1cb0143de60585e666c8a82843c750e8d28c41b001ae73c842527e60d54f1dc20320f90a663c701f2bab8696e9");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "7f07b5c76d5b9e9d77d3b03b69de2f21cea9228b06e5075098cac60ac95ce4776dedf91ffde36437eabb3dc6314e8b063053b70db72641ac63c8b2982726d2c72c9e1a1ddbbf6c48385002e97194a5f4b6cd1f594cfff2e4ddf64a61138a31056522020930772098099be7eb5239e3b30b1e40b7ebf882beaf5e77afb31169f7");
        bigint_t S("8814eb974e33027b3ed47ca9cbe7135e4aab75547be3b661bec48dd15e13bf3797681e9a96c83a409d46796decfa3155255af5d534d660596066f87611408c93c90a403e048f0ffc44d4b079a96000684c998fdc82f662e82df711b057ec7de01b5f42f348c0fb445d3fa8d8373d0ad9a5371804d999e77b6a6977fba65a6eca76"
                   "c4a84af4ff3805020d6ed3dc5f75e1ae88972dd70c1c00956f39cd8b22e5fc2a613aa364ce45cdfcdf02cee909ab0c54ea0eb1997a8bc97d399fa36243a162");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "57678a5baa2b4a39f8c33f62a3977fa9f9516116b64a27f4ca41e1265d538eba112783c1876a115d247b621b1567a483901769ee5dc84e9896bcdbf6ab2fab4aa88dcd74bbe2690bf192a7ac404acc92ca36a76760752b5a45ca5d7235947122a6002f1d4e7d9c6be570d7bd2c2941fe2e16e02ac637066361d22d420568266b");
        bigint_t S("5658d6bace6f80ae25ff1cddd8643bfe197b7699edb35a2035fe325d01e97cffa894a2e93c22b77de799531f0812b4931224a7a5fbde302b11397fd8804ff48471beb3c50ff90abf95d80c3b138d5ab3a79108fe5ff7c89d1abe4bb94cd4875feaf4384f5c3c32ff3118b20048d2473368bcfa5b3075606dca2e54ff6e27f00457"
                   "d6bc5b8f7c6c86675d47fa032e7c395eda977b841cf2e72067d1285113c5081f8d6b14659ee1bf49c78214efdfd866ed4d2c0f72ca661a59844a9f83c3966f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "15b1e07d9a1bfe53c7becfed15c721ef602b641d33dd0c29464437c845ec900bd37c73edcfe85229d6e7574e09a84a1410c0b6f15f6847e8b7e577a2f04769bff756156e13caa63b6cba2029cd574601a9dabbe95cbb6738b4241c89b0a57a0f87291028ec29c334911f74a7ea3edd5d3360bc2eb44034c344e970a70c6e89a1");
        bigint_t S("4a5b9af9c12d4041775070e6f2b1d8b5a3c7b4966ece2cdf6031e9af8a8607ad5d3a7903263c9467a3e099d2d937f8111164ee8ad3b0b8d4b7862b2678df2bb765dd03375591d90265d136edab69518b33bbed389c7746869e598f82965851d16b3d8b7c4f83d073c58ef3360112e5a775dfd0a3433ffe72df030145969acaf07f"
                   "fb9874304c1ff20e4df02b9436cc03dbb485955eeb7b6a02665f61edcc22fd908a0ea8fa61b9ae52a346a691828c9ccbe8d12f389f6aff37bbd9cd0ffb8c1b");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "50b0b8dab5921fbb12b835222008164e6de95b04ff58e03d3a39cb1c04eac922261e9ba5f5e9d27e3317d60330c22d353424fa3a21a9c40d55487974ce14b332910e397e4c3ec9b53a02154c47a50b08753359717e8c3184bbb849a8447a27e359289b4e00b98dc6f020f8e5aed93730f6c180925c2aae0a332f43a0ae45ad9d");
        bigint_t S("2cd9c7f555f84e77aaa544ab5fce57a019519e094d1c762832fbf940259833f126531f4758278d3b24cb438816cc9c0c1b803d90533914c20e7329d92c0cbbb87bd4c16d1f1f0b9abee539b7aff350c1a4d65d29e80f7765ba09f20d8fabdd1f7ad7feaf4191157dc7617b17b0325b1d9fd51d2b7e0be5d0c00b15787107949076"
                   "89cad46bfa7be95bc7ad92de5eadd5d9716898cdc309c26e8ec7967b7137de73a5742c1a4b55ada7700d470ecf8727b27d3be9d6c06ef44f6a19671b53d0fb");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "5511c2b0d2dfd25992ad78735f326382ff2b2a5359f48e32da55005e179e63bab4f612c584f025ccf15ff05f9f6ccd014ce51c23fbd8875451618bb23fc033a4e1ae27cba6f2e635b22f50d48000672fcc465658b0c1b31e0dde01832cd07971a5e20aa8800c9efb9bc49fbc4b16b97546f7e2d8c1d798f9d5756abb27f7ba88");
        bigint_t S("117b987b4c79fe569bb0031cb63ec610b9fc4da8ae05b20ca2fd0608c6c8967baf7fd34fc45ef9e4532013a0a3502e608544a63a5e218109e58568ccc2bedce3b48c0fdf4748b0ba4b4bf569d8e9c50231bcb0caba31e32b2cf8d57edda8bf9770705fa072cfcbf0355979080d8e7291179246f1be59a4d7786e8dbc548598d8fc"
                   "b3ab293c95b9c897d4fe285ce6863f1eb90e953fea9a36d6a5b359f08d35a8854925940aa06fec66176549e79dc99054be00f0bd9211782de7a3392740684d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "67567351452fd3aefce4671f63c1d5036dc1c0bffb0c3f62f4be8e8598ea047611df45954ba9b9fd06db3d672a8c184c5aa0304acb497ec957ccbfdecd896c708c36ed99506b0a4e4b63a24378019f092c112979eed11a5688afb22e5e558d81ffe3d126fa7bace9f303fe79f52652820a9c131609acfd51b0efd13df2564df3");
        bigint_t S("2bc0a09493a57b56a5ec1e3b4733450e06bc64644b3a9c3af86b30866022702f1d018b4f56254e74fd997877db8a8683b2fe1571f494ace3a7648fd940b957b0b7e98383b9d42fa5b48528df653489ecbf7f92af7385da03af27475db872c6273f2cc79762b05ee0ab2aefbb682a0d415614de31370aaa2634480f5ca7ba0e503b"
                   "45d44c31f56402fd58bacf34440a26bcb0f8dc187d7761e4cfcc17c78adcaf0c54a10ff6977def477befeb9a85aac64555369c11319b0c97cbfc5e4741dd0f");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA384_RSA_PKCS_1536)
{
    // [mod = 1536]
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(1536 / 8);

        rsassa_pkcs1<sha384>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 1536);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha384>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1536);
        EXPECT_EQ(f, true);
    };

    //[mod = 1536]

    bigint_t n("a2fbfe82fccf847724640571bd393ca2dbe4e9a3ad85cbdad9151faf27b7127d37147cb7fdd3508d68b9a8b5b914cacc863b3e180665bad32c0c1ec8f9d87740fa5891de8f54085eb0424d2589f53adb20ac92eec98d9ce5378e9875ee7890f99b3828662471b24ba2caad362487030169dd127c8e6e7bf3c8f01f7ff5e09d3e91189a"
               "c6e37355dda7526dd5b63c6b32cc9f5b86251fdc9db8ba9e676f848a44e59c095550d4f4480295ae327ac7fb877a2835a216cc06c3786682d31b6dff19");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("068b7f86339042c5c50a963461d0a35877e39aa545c1d674989c924be698a1afb10f4bcdd8adcc718d47cd5854d11aaeb9623625615662f8f96f182a626c18a7dd95c26fd14a22d6e895950301a5fd21d10f550c2d40585155188f13474b04a94cee8df7e6b889c4e9b68145886eba209577ff4ad3072c72219b72b0803b308fb2e4d9"
               "a8bbd370088c4de857827136612f6e7731d56900318c433eda485a4798ef7be50a530f1f3f173fc251ee7192c3bceb598c109872fca098b9f9d5cc0001");

    {
        bigint_t Msg(
            "37d7fee3d31934912167e1ffb35b4b2f159e84bd02513d65d8abdbc44ae3cd6355501cf175cf43d0186542d45325ee47d4548b3018cc9cd4de9efa7edbfb247caf55d66a18c1ddb365a029542e7e24e3433a42d0e267d9ec2df33427da06e39e2d2d54d523b8a840f161bf09ac2e950d4a98d228c9ec9cb2aef2b049b1cdb1d1");
        bigint_t S("7fbe7bf0b9eef0dc7634606cedf55fdac9e6d654d6e7e1e83a776d8e4f585f59fad12ddfb1d0805679497ddddb834b248cbbf760a352ae53453bc3026fe7694bf153c4e12ba07eb5bc0f9c65038bb5d72368bbaf23ddd4c1da6a9203cb51cb7e2b9efb5b5bafa927885b49f479708aae9f770eea2e5f8ad4050edd686cab35569d"
                   "27c00d45c21b5aecf2d946484044318c8909115c233c9af8527a9d9db40b8dcba3281877751845b4c2775968b04ed813a7a03d99d90fc1408078bd60aab480");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "15750e411ed6b1487e9bfae96aaf758f6c059ce1668af84fd45889eb512c687b5d1aabcc5d7f8949e338bfcb301131b3f780d5376494cdf95d28d3de085cda1d8d46f2961de46d66335540b81949acddafb71da068ce52d3952dbe2ee6562e5a2d60ad4866eda4c7f55c46562e402b361a103bc47820ce0603cf1284f302ac59");
        bigint_t S("3832d9372ccbc0e70c203e46ebc7df8b47de57a539a34490d101279b99017754e973d92d07359c3d8694704b6614e97291be4ebe74db19a96749261f5bb0ab7f9ac42b0418ba8d2418f498f9266e5afd43255f55e04ff713de11f4a43028a2ca1275c7c78bf4d36e224de0c5501848fa08efcbc7488018efec4a492892cddfd33b"
                   "960386f1f856253a11a0e526ceba417ffade4abddb4e5479e623bd3fa247d98d2001b5518f756545d0690ec91304deb0e1f3b854e3e1273200bf67735cca15");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "369d20e16d4a6574d4f9ee511106bc494268f7b5eed982b73b38d3c2622b025c61081f22f0f6c90db54ffcf5bb492b5321d68f4b0fc8beeaee397facd92add414e6a7a902aba26fdd42eb0e7b8d6fc75f79e56935c73063793a725e99597af82678bd93404dda4ca985eb231eef558382c89f694be218327904a6c8280ba9fa8");
        bigint_t S("0e296e4821f1ba34981000bc92e5dac9bb0c9b5854352adb8b3f5b2ce79d31ec16b56a9bd1e1f33850a3100bec1dfa5871c2c18a833499c7346b526ca79f8f5512745989d086314438d466ec57b407bcf6e9d708e7858f1416195912a44c661f4da3569ad855d749bdcf35de353b10c05e13894387d04c7c241f0e5eee579bb156"
                   "4e19ba61d8ba69c0058e637f719fc867c514685270272e73e6d7862f888b974ea9d5f252142c9205375c9a375d731f100c187847b290e9b25bb6f882b11114");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "1f75ef4371178356081e4bab44fc3c706a706f6eecb580e0ebf93485fa105ef4256a03303242bc7c26195c700b63d34df180528e33edb0bffbbc0d920841b530af0f46cd2023167feb5ecf50c3395c9281b409726c8fe5d0b7bd6b4e4b1662a7c74c0243c520580860d5ec72f3e5a0e622f4db431f02e7bb26e498e14af52f02");
        bigint_t S("4bc782031a3288672715669846984e87428e2262c066c4510237491fa35f7ca5c900f1bb5c0e5391489c51a140d9d2a13b297ceaf14b1b4b04aba0f6943801976d66370334dce54d08df0813a5d9eb5ecea6e9cfd7b229c10a4440b7ad29b1a4e96148fb60fa0117c703a76b9bc484af00a28989f840a24b8ef28d5bf4c98d0b8e"
                   "00ef56ef5f574fae05da3dd7a447061121d804448e3e7dad8ef95a4f2d7174f54048a2b42add71eb9130de8f83d7d8f49b6d8029cd13b2e6658918ca50fe49");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "82037ec6b8b2c72ef7cf1c67e480dfdb90a2496f9d5eaeb7a3fd0265d1fd9d08d8643bf30454477aace05965cd1efd9ef27db62d575942664dcd2061e0c950abb8645a3b3d7391a8ff5a99995422277c43cc087ba0e565ab795b4587b18f539eebabe288a9362e1eb5e6e4ebc1b5e5dd4a364740c22a9deded4f79bf8c782f47");
        bigint_t S("8ad5ada53357fb45e8202adf818eeb8d65507670b037307c94e98f4e66caa2ea4d7c6679cd4a3575af81c334da1b340c99f6473e4d5984eda3ff0768c2233d8a4b11a7b4b0101368bd50f6e01c5fd8a0fd6261a4eda88c9a7201cc55aae7019047eb89cf446a1a2221be64bdaa9a0ec1d78c52f88e9a01f3f510d10874c02db74b"
                   "73e0cf542ebdc32326ae61e0db599434d2e2391dc33bad2abc66dbbef2e26c83e25f144c373f3e23c7fac8ce898c62c0b1607ccdd731c31d8e298c137fa382");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "06a21cb4062d114a1ca2d0f79cd174b4a82749f100303b371a5a8e0c76fccf172c960a84c75b3daa55ec0debbf67b8dc57a04ba6b0ae15adb2aa5b6fedb34dae2e3539d1506bff885523c5afb6c2eb75f68741bd7d1478c4a42ad7040db42f59350711634c683e2b44ce5f0928eaf7c5af6595fc11694ec519626a957b065e60");
        bigint_t S("82da9373d2e50e49e91d56e376af44e44d0028344cf6ed03af1b68c745b7e424106cd0e0057f42988b834db9ef82c6081cfe0cc3b4ba3bb4375f4d17780b7f988d56a82784772cf63a561ed4e80902440a8534590fa569eec3fd7afc13adb28a8b111c177fc6f54011ca8d00cead4e24c1ed7c8e7c41c32c6b6fcc1797cb1c61e7"
                   "e8ed25658c7f6f32ca95ba0844dea6846ed4deb08c0d8ac5b570d52bfe0f8bb110a588e5bf1ed9506d68553a621625aae2e48f2021cb35e7c385d435516f21");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a9c872524df7da7de23b47cb7735677f5bd00a7fe7c68efa9f679a4d7a9610287d143cec8e015258889f17658b5ade6e9605b6945767e26141f689ace0e2afbc695564b096fa3b1db0bcfefaedd0a0b9fdf573b7e0fa9bd6fa81c36869040f50961551630aea7f1584186551489f97a8b927aaae2a89a3a1f291b04cbe03162b");
        bigint_t S("09935b301ccc3849efac6d00373190026e53c07c514a91df1b8541843562024393c9a60b965139e640c3ebca7dcb7a862aabc1c296e7a1697821ac17c4dc719cb72931ab1381511e80b9bba3c5d639049842621bb7889fd7fa87e52af360bfa0cb5ca385425ed3f9a6975efd8bd725560e07b2231730e6b9ee1b9c873df9820e02"
                   "29776aa6ddbca030f3f96a60c7c09dbbb8b334711eeb07e3de9a6f409639468ed1ea71dda850262e6917a2eb9c7ffc991721898d7209717ec1e5950a7b65d2");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "e33f32f8b487703a3860f3c5a2451efb19cdf5625fc8df56b18ba9fbef88cf76fd22f75d69bfbf6b4e1eb87978ebb2a9c8903be5da4f6288e68dc15ae471bac2f4ae0037fc9d59f8938fa3fd1e61152d3851c8d1580f7649ad1881fd4d3f37978f8c5470b54ac847cf5cbb39dcd0cfed31b7a5101f33a201d0d1f3cd7219ce5d");
        bigint_t S("7bcdfe9f7add3841b717954f7d95ee90de9881b8cd86dc7c15528d2539b194fbf2a2e41995864935c396380961e0200e3f8c532d81ea269aeae763036be53d6be992d5bcf307e79af39f518fb689472e2dd56d71ed9703033465c0d6d8514f56ad58ca3747d88a84d9ec3e5e27f1083e66dbac7e7e1817c55d88e8eb59b9ea4740"
                   "ead9ae0c7aaa6f3105887938be675b3c64787c9b12e97f2a67f9368ae2746ecb13041271ff11024b2d3effd9aa50603f4b9530bbeac8953e74f640afa9ece4");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "95622ab710f99332c2b812c2297a24c2c8097ee2193ea26fc969b1d524f28062cf41ed4cb5dc74f643cffd6fd851f83b77a4ac57c03dcc771e254c92419f72a0cf0b30a3560ab1e486e3327f9d261321228f2e11d62c676de8c284b7686e4090fe889ac2dccc7384f1bc58fa5f409dfc3090ab1844539ada8e09034a81a60fb7");
        bigint_t S("4b4681480ecb8ee514a28b08609026e10e73c7795341a5860229111f5eb4e6e343a0cc335c4fc16c56d3dd104b62b8a9527769659f0593fe795c1a63b1e942517f6ac5b0c93ecc071a88671ec5310d0eeef8e735ca3788886099827658ecda30506722be056da09a1a1bd66c7f6cd0dba7a6427750c33d301c497eb61fae990509"
                   "92cc30aba5b8159a44cd8be36d738c0a6fd464d733af37b9915398a13c6769490bf32e1ed2b3e7d085eadb8b6b59532e85dfa53d679d3ed64ea7bb71cda3a0");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "243c80bcda3b5ce186260bbf694b75313f81e9dd6ab85f664cd400e4f5bce891fead2ac4b4d514247a1db51568469d773837e49063956e4687956c920c0e263107d3f99d5e3f9b3b2e11a7d6697a554cc4758887d0570faf66a7249e492f39950753f52d20a279906aabe7e8b4122518aae3deabf925ca09b3b330f1f447a923");
        bigint_t S("5b2695b38d1ab396c73dc2517e4a80b1b961eb58f9097aa969b5f55bbe6ef105879c5226d4305b113be48f333e04ef03fedfd2f311bda7495ba478d0ad62543aed3f00c29e419ab572b889e8894e7fbd2b4f34ff8b77590ea6c287bed27e58f807eff8be4ed66011d50c85d1d2b6a7babe5323c9a1a4fe53f7e968c11941713d54"
                   "b2a97c2145bc585e1de14e6753b40127642a0d08c29f4fa07b16e2d6eecf87343430c68bab4da1f948fb307287eb6d59db67ae295204ed8deaf5bab03b0620");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA512_RSA_PKCS_1536)
{
    // [mod = 1536]
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(1536 / 8);

        rsassa_pkcs1<sha512>::sign<>(plain.begin(), plain.end(), signature.begin(), d, n, 1536);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha512>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 1536);
        EXPECT_EQ(f, true);
    };

    //[mod = 1536]

    bigint_t n("a2fbfe82fccf847724640571bd393ca2dbe4e9a3ad85cbdad9151faf27b7127d37147cb7fdd3508d68b9a8b5b914cacc863b3e180665bad32c0c1ec8f9d87740fa5891de8f54085eb0424d2589f53adb20ac92eec98d9ce5378e9875ee7890f99b3828662471b24ba2caad362487030169dd127c8e6e7bf3c8f01f7ff5e09d3e91189a"
               "c6e37355dda7526dd5b63c6b32cc9f5b86251fdc9db8ba9e676f848a44e59c095550d4f4480295ae327ac7fb877a2835a216cc06c3786682d31b6dff19");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("068b7f86339042c5c50a963461d0a35877e39aa545c1d674989c924be698a1afb10f4bcdd8adcc718d47cd5854d11aaeb9623625615662f8f96f182a626c18a7dd95c26fd14a22d6e895950301a5fd21d10f550c2d40585155188f13474b04a94cee8df7e6b889c4e9b68145886eba209577ff4ad3072c72219b72b0803b308fb2e4d9"
               "a8bbd370088c4de857827136612f6e7731d56900318c433eda485a4798ef7be50a530f1f3f173fc251ee7192c3bceb598c109872fca098b9f9d5cc0001");

    {
        bigint_t Msg(
            "281573d668f14475977c9fb6204369392d944a5abf405f10fe9f2f15ced5e148a254ac4f95a28e80b75d4102a47c6b2f5cd7153d4eea5e1eb6d570a429a8b4e96ede42ee90279c82316b9b66210ba9ae306a316a40ac54869ebddeef929578f734bb1ffb8208d443b0ae9529bf798d7e51548829bc0ea9115d5e5f3e2f25953b");
        bigint_t S("4dccb5f51c588328ad02fe48c30c5c9b4d8705e0a4ee15e63fb9441b74ae28f618da184e7d278c088bf62d2dcf6ad1dfb601e55ffbf2d9210e8df3fae104c8e6aff88196d3395706a346754c91104a609046069e5b23a3a58c2d2dd1158d75d91c7f482f8e8eaefcc93bb72688d8d5d6e8ac6794fb56cd3c499e1d2b30664d6bbd"
                   "e84a65fbbe5e2bad5a569a047102b7299f42883a85c146e221e8993ab929b2339bdf21dbadbd5bba92ea9765628c81173fc42abe207aa956c01771b1685d92");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "c4095a74b43fec6a10af4a84630d6caa89143016646daa58d0d3bb9dd875e5302a9b5d41c88d62e601b1ce5fa0f3eb9e63698707ae374b7481b1487ca8c0d3f5318793b72346d8a36d7e2ee41d563375d23da5e374d1bf5522ffc6c96c6cdfa0c314235d2d32f5aa3e5f939c43fdec37c0d6b2351d01af039c065c0d8e41b03b");
        bigint_t S("50dfbeb36cd26a3f2568326b99d9ca4c7bdefc58579a2ddce9d780389932930134041f43571f7644b5c761bebac9613ce5c78ec79114a733ab8d8781fe3c892600090a294e3c9ed9915ded2518eb64d79a488ea636626fea4a23d90d56752ed42094886e1780a46c8fd838fda557bdbcaa2ba3f60d9102fae05b611d91f746ec05"
                   "bca5237a88ee1da970d2c9cd9457cace65f69d646eb7b64219e25da839cd048d5f65c7a08af468c83b53c2c9fd2e9ad7f710c009718670ec22670a65d4a054");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "4e0ea4cd4106880242667f17586ae8bdffe94c2c1f7def683fe4a5c3f6a6e7a3a3f826855ee15f422f9bf0917d3caacec99627fa9c5b5e02bd857444a8f4d64f3c5984eab454b059d8d987db74d84f770a1db2f8c4755937ac12bd981f303ad77ff9660529a6a0cda7e8ecfc2686fe412a4794308012098d755427c845a23ec4");
        bigint_t S("208c20d4e21709016862d7f9fde9966aa1888626b1baef877e56ad28921fd41f4f5345996af0a0eb26097c54c1924b536b232312b65b3a61a0b25ffe852128d048d4939447ce495c5a6d344ca847b6190187768245a25b5447581c4fb2d897411e6b8fc73fdc0a7c7d0326cd41cee8b590b7ff7ff77336726351ebe5e0aa64f1a2"
                   "200acd27afe9bcf9d9ce3aa66856a57fb8de6006a36f7d0eff0627af3fc8ca9a7aeb4439883280bcbe070a0578a651a06a44816c9907045f7e2d7d88939c5d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "2de2a6e5f415badfa2749d7b42743b01cfb3100f3ce64831b71225b12f00a2b114eb56765e7b9a07d0f7eef77eb6997f8dd690d03a3199a61d8db021af13aad10b03180ff1a56e941083654485d59d4b9f3d876be5b482945ac444b6fff97cd39a92b558b67b7eadeb80c80b5fbb8f8e8d4ebd8b45584b23f421a918269860e0");
        bigint_t S("33a76bdf4cb67dbe60a385e5203d3d38a70bf11042d958d8f03fca072b158503c2e4d19dfbd7608a5a1aaaa93f292fee637697e6243510b0363ddba3d4c1f877c230bad3832efdce6a87f65a897e636698a9f1c8f464e5db0b5e87ee0ea42c0421933b213985de4e357363fc3af52c34a29bb8d5f12ad7c767d4d95bda4e33ea4f"
                   "fa3f041f3e73496dc4f466e9a123ef2cbd907d181036bf7f10d442f3ea781aa12ef24aa85af4e5283940a7349401eb984fe1ae11b9ffd54986a4a8a05a31ad");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "66614022324dc6ea068c1afaefc4dac998c520541b11fcc0179c224103d0df3ac1d452a6760ed8b69ae7c8bb4bce6147e77f008fd9859614b3fd5a837e9ebbd94df699ca19f6b464c8b165f08f618c197aabc9504924ef6e7e13b59bb479af6f324e7d58c00e433cba74595f628543867ad1e35d13f6ed16a0659c534e69a346");
        bigint_t S("10d9924ec0584a45429b136f70e49667c0debec0a0c3d3efe282bdc8e7d43d3c1669ac97f3a97d9f3d89bc81dc42e3057f40ea2916b1bc1a6011cf633ba2dedefc6068d5cc7e02afdcbe52ef0eaadbdeeadecaf45b86c4639c8aa90e2b854a8f244572f482dfbf4474aa17895dcee685fac68252d8c164f053a5581d377728ca5b"
                   "52351b9f11ecdf5a1af3861f862ba0afd6fdb341bba6d65da55fdd6d887ffa7c238159230f846221f2c3247c0df50e5c123af2b83fba89f73ca67740ffb643");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "32a73e45bfd575a2df076774b6e3f3aabf031dccb64910ecb5725fd99dee9095b7efdb099bc12ff851597632b7301a0a2252c80948b174e45ec09dac7d3dd2454a4e7519d6d0bf0dc3ec04b5a9eb38c3b74bb3c69e123111aa621df8324e0f4333ec93dc38ab2ccb7de7986453bb56bfc61766697e125059e381672685c84f77");
        bigint_t S("1246dd0d02b896b6b2adf65b5b12c0333abe6048d5b62c116bbcb0d0974cdd303860b64516f99903f92dbeb368db19e92ed24dfa1ed23362a8ade55239ddb13fe98e09919bad116e5c95269c62b198672f27f52969e254edb27e9e3bea0cbb18cf98abfe11d8be73e2a210436be7d2eb1b6a14c0fb28c5677b9d4cc2229affcec9"
                   "9d09daf983c0f96c402e3924fdd2a26c5e5b274f181da4e43b574181609fea05a627384f5a1614a7618c6b54d59b218468e10c3ddad17cbff0bc3e53fc042a");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "78e2c1f94635d3eb3f1be98bf869dc62a3121c2ed9e397359770ddda864e826e7f82c4484e3a6e18b6bbfd78b69b00c40b30c52cf64831073f1af06a7a7dcffa895865fe625af9b913eba0f60362a6e47a782f2463fc1e2151c936a0c1f67f989f4cec430bfd3a755c921b2d65269469f8d2a43fa968262ad464f8a2e21f5def");
        bigint_t S("1866aee1787699e0f5188007e67e2db923cba09635d295fb72d0a19cf07fb8735e90f7967750627569120bbba482ea7aebe052b99e9968605b971c5121fc54cc9b19cb578831aefc8e9227b61cf8c1bbf123a6a19f855bc4964220b838be527c72cceac357eb8a902efde32268ba1fa7946b7b2ae8d460da6b85f40a216edd43c6"
                   "03a2c7d670dc81dae728ba45764382c4db1d073f8acb0d7ed0e87f237053378f4ca36953b588eb58c638a051671dee3bf1338895ef61776f88c3eabcf7e581");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "c63e4c2d1b90a18f14894fff97c3958f0276dcd51d9d48364d548d650a85042692c3fe58df765832d7394fcdc48096e2037ea6d6004656f36230abf9827ef1d5223f0ab05c74072bd872f5e94287f94ffbe0a0870faa70cb6878c3e041e5d5e0dd7a1b3d78d2d6037efbe95dfb20e75b526571f23313bf45c7f57c222d02d7d0");
        bigint_t S("9302513e38915438f6282db46cefdf35d87a0de59ac4d4963c2178100440873dfedf38604320b862d41d93f2d1cb41e067944d48c2fc865dd9a6777ee28c16169148676d7dec190ed1c1b5a28aa3af3f0c64f3d5c1643d13488baf244ff97437055a7ac17257423404e31edc927162e6d0874fa1b661527887e56ec479817ceef9"
                   "d77c7f4995ff63e9c36befc17eba8906dfcd564495106d62226d644c8c3b01535201f5aca8872bcb7bb22fa5738cbe2df5bcd08ce25982485facf67d316090");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "21e9a566b5c780b65e3a1c42df89f79d4c0b5900cadf379b96dc572d49cbb5d35ad4261de4373b67a93d6867586d65020b02d564149d264ff938b8b3bd5b4180619270b8ab1ed0c826ebf0186ac2c1e19dc03f3f3a07538711b3f8da90109efa400acb5d92e40880dc2dfb7e9ff0d1e10b2cdfb8c58388eae73661c9495193ea");
        bigint_t S("8471e6b45db165c41bc9dd6fc6fc9febce68b53774df1078118097436eb3227c9d3d02a54ab6e140e6bfaca1dbbfb2aafa791b922a41d7d13a7c3a6f7a6429d0012a036b0d99e9560347d43be0ca8cad62c4c942d753d548ab138d66b7a64484cc6409ac02c91a8ade51bc135236c0a483370192fa1f08ae2b3fb1d9f7978f7552"
                   "a941f18cd59eb5f3115871787a3dd6d59cec465721c8812a9c9602fa0a7416d5d80909f14233868784395471bc99839458e9a03e54d4a710cc3bfdaa275b14");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a19edcee04d171ec1c94426a08dd7cba642bb7dcbc211445467488e0d0087753f75685839a11a95a8e0983c8934fef07879ada24dae954bb426ae702b09b270882af8b3ffa337b39eff1aaafc215a6c7948eae96b6b7c3e461370130a82630f4aa71972b0c7f100fa71462277d76ca81f2cfdb3d39d3894b0ca28074a0f0ba9f");
        bigint_t S("89f174835086d4096ee4bd71d5afc2a893751385d833577cc3b9aecddec1058c5ed7612e1a117f197d82ff1bae9a4d2bf238dae5c27c1bdcee9e3023e30ca9feaf12b302514b2e5748edf349a4a97429c48fba8cf5ab5e4d2b9d10b89882f74017166c9c09a78d4001c8279038eb64b5aef6473b459e3f321d49067eb487f13438"
                   "f86248c39c89b8371a4c4cce59da132995e81c0a84f4954b467c183f31d140d8ac7db72a08cae7e11722a6e63e0e0dee9bc9da00179026840d3fc7ad3f131a");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA__1_RSA_PKCS_2048)
{
    // [mod = 2048]
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(2048 / 8);

        rsassa_pkcs1<sha1>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 2048);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha1>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 2048);
        EXPECT_EQ(f, true);
    };

    bigint_t n("e0b14b99cd61cd3db9c2076668841324fa3174f33ce66ffd514394d34178d29a49493276b6777233e7d46a3e68bc7ca7e899e901d54f6dee0749c3e48ddf68685867ee2ae66df88eb563f6db137a9f6b175a112e0eda8368e88e45efe1ce14bc6016d52639627066af1872c72f60b9161c1d237eeb34b0f841b3f0896f9fe0e16b0f74"
               "352d101292cc464a7e7861bbeb86f6df6151cb265417c66c565ed8974bd8fc984d5ddfd4eb91a3d5234ce1b5467f3ade375f802ec07293f1236efa3068bc91b158551c875c5dc0a9d6fa321bf9421f08deac910e35c1c28549ee8eed8330cf70595ff70b94b49907e27698a9d911f7ac0706afcb1a4a39feb38b0a8049");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("1dbca92e4245c2d57bfba76210cc06029b502753b7c821a32b799fbd33c98b49db10226b1eac0143c8574ef652833b96374d034ef84daa5559c693f3f028d49716b82e87a3f682f25424563bd9409dcf9d08110500f73f74076f28e75e0199b1f29fa2f70b9a31190dec54e872a740e7a1b1e38c3d11bca8267deb842cef4262237ac8"
               "75725068f32563b478aca8d6a99f34cb8876b97145b2e8529ec8adea83ead4ec63e3ff2d17a2ffefb05c902ca7a92168378c89f75c928fc4f0707e43487a4f47df70cae87e24272c136d3e98cf59066d41a3d038857d073d8b4d2c27b8f0ea6bfa50d263091a4a18c63f446bc9a61e8c4a688347b2435ec8e72eddaea7");

    {
        bigint_t Msg(
            "544be0a4044edcdc1240f2182109f826d784ea613486dfd5221d3ba44d1525a934c13c5a81f885f3955da8d168e35d1b909121a89f832d1db232a85647f51c084fc727a4854a737efd0ce72e00091f3617721ae666ad337d3b9d5391e72364b1cde50948b84e8cc472d618f8928328bf95af3fe3300dda3de5e7a21dd5a9e7ec");
        bigint_t S("d0d03ccb3b30b7c9c4d6eeee2ec26d069246e019fb8fa2f3a9b72c9bbe231d93ce053df805a045e2ef6bd8d08bfb0c36922e5a6f10b947b2607f596b6cbd3c9eefef56f5396805e8b28b1ca182c78c0b12b9796aa856af69c35504f8acc7afa74bc0f77a1d61da94944057a9ee72d2f0a96cbaa2f64676f5318b71e56f519d0da1"
                   "ce8f42db0ebe5045fcc726e39fb0032f2287918f9190f3fb3d4de542030441f6736c6205a2bcd2450eb411085311c7320baa4268fd2fd8bcc8ebfddbb60740cff0b3b00f618777ebcfb3468f309d923c957c8170727a5458ac2c9070f93cfc37d31cf9f1a35d0cc3abf25af8dc9e1590ce59ab39d01cf0c154ab8d0635c5e9");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "8a66370970925dda75609a710ad1fa75d04e523c508f8cd6ecbd3f4d15d3832bbf4a033d12f55397632a84d86222f6af95a26e9e05db68fe3790af445d762a82cee0f931b49bc12d04174301b4c8346528cc4805edce9184a92a07c1525268599f70426dbc4d4da54271ff74668c5b9e71140af87030ac6f2cd5704c5c2ee2b7");
        bigint_t S("16da730c7ee6f3f9750aa3120f734f7800f2cbf7247c180f1a490eeef5dc637dd2da577cb2f41464adeadabcbc7324779a2b118c0c61c2829b55797de6a3b092aeea444d2b4080f15614e9da6eea17552a8e955fcec1873cd20bfed8ed2d63a950b159a0ac109df370ddea773a7149f1893cda82d84ace35b65b3fa9f7e4cebec8"
                   "e219ba9a12fac5cf5b2449cb6d563d8a646f1d649dc9abeac829a3ce8f9823cb70877193690855389557adbdbede90df0c274986f09796abc2a2b16f7c8c6eb5b3a42b0958c3da16bc363ed45de4ff4357dd8c20421e4c3007f9b98c86a80e7ad0106aea059acb71ed2d81eb6f0dd758bdd9d508cc465e23404aeb08e4ddc5");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a8f41af433ff314d93e0d1e0bc05c427cb682697f6b705ddc3b32a6779d63526d438f092fdb3ddefb94511cfe1b745d9127c2bc8a5dcfeb7e2525d9b70325911108ecd5a8c5beea3e9d0228ae40f16df9d070aa78d997067bdf9b69f3a586094c4c05952c63f8bf28b47ad19ae555cc1d2d16e3c94358375be72dc7abd907137");
        bigint_t S("2d69d08bb9b99a173a69a3221c28958161f5defd5592e3ef5cf0ef21f04ef7e199344457b363206bc2785197489aa83d0b330618b5d58edc9ffca4268a1058c4b1d1fdb39ea2a1afcf2024da3c9e1f9820e65721db4a0accc77254291239c83b46fd3af6ab14aa2e8c4e140f751744bfa731d2bebc33d30388f140e8111ce1f7a0"
                   "90005d78b48feaedc0059897e3384a3a31270eecaabf455f60b8377619cdd79484c292562a461ea96774e9883253d611dd4e7b26af83d19d740cd41f26d28eae5d20a85d16b374b991b03141ba87c75da6d23818f509a2547f2c3499eeb48dd48363f3f80bf15f2e2328116940a0c3f4d4ca6726c92814bbfd94ebade849ba");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "9044d199a837832558657fdf67c8ea224f41317eef7f54ba84228519d620f1ddaf68de0a91c6f66ca807e2dbfe5d1bbeee05fdcedb261e1d22fddd564c373e46899ef59b5af8c0ec8d2cfafa6ea76e363cfe6983dad5133cc7fffe7950e7008e042e3ed569958aaeed1eefc31f6e89911f769a245352db215c71100a49f7dfbe");
        bigint_t S("176fbaea69c599caf946fc657b1fc958565ef3195931843dcfe5031b15d4c7a96c005da15f6f6550e5c426ec91c871b1c8dd0868cfd1de3b7901ce6c16fa2d39b1caeb3a479741893e8165910b46f07a9acb29d3a7b82ee6a1e06bf44155a8fc025480fe495eabce19e6728c5f3d812b32a7a14dfe79925351082406e14c7f591d"
                   "931f1c514980e964691a51e2044ebb82c40eaace62ff74eb66b2dbfc15a08ea48c7a2a110fc402b7a68cf039c8eaaf437d5642a3511d36bc12f96cb5b6cff47c5614ef7cc2be1f139047a7c5dbfb1d7018fb8857216662d3f5af3f7e1cc696e9b1ec958295218448e19625a1e65ae92be6d60fde3d2e6a0b3093efcc7754c9");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "b796c34f1d4e1dfb3010282bee33eb9da90e73f7ff186b0c0d4eed5be0d598d82ce1e3f041d5b0c73aac25c47897b74f4fe787d9fe2ef21de5a163a6f2c16c925d3b1695950c75417ae87f84ccdd1e72e3348344a198bea1a67a8b1dc2b36f00b06b0448013a91f1568ede73d24d478dcfbb0e41d09df620b79f0dc739f3c9aa");
        bigint_t S("9e324f827ca17e50fb0cd17b8f714d7da1928e1c586a83d437e9891c786644638af225e92841ca1df6d94e6eed65b5f076cae3aeb97ac96f003adddef6cebe80de99c584849a1c1832813cbb63fc619079252ad4d8881ef72e630ea06fb3b0e1b51bfa379b1decfb92e964cdc68809afb9d53b3c43dad119ba67b5ec1ace1093fe"
                   "2e144d498b81125fa69b51814fa3c1230f0b42f9d7763b1447ec57bd0d93a96a8928bb7f4560be95fc100dc6430674107511f7f466a18fe102a7bb0c3536ee9d3a7c45631153cccf774fa65f20d07a5255a0902d4dd3c91cda03c0778dd7278beeab6afb93cbce0c9fd6b202d57d811dd7ee50331f7243168df107597d25a6");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "4b6308be5b3d99ee6785bcce2566047ad96adf7680e26b364e878be45b613b5d9ae8207d0a6a639c30acc058c4d4da95ab86d2c3254e1760483e80bdadd89804692e397a3e475c0213c52fe3a2c568ca7107d913034f45444f0a73da71358049a07b793e609d2a27f81730f52701562013331eec82ac9dec4c358cc411419d32");
        bigint_t S("ac402ca58c3a5e42bafb2135503a176174cf1bb86d5bc4a5d7f6b3e41251c0a6179df8696bf418ac2ebb52682a8b5f3cd23f0e1afb58c8b1996d486b90bfdf6d7c1e5e25575ae5b370ea3437d54a2f98f96d99ac601fbb93dc81c731fa10a6158baaf040fe57184fa79af7a4ea3606b08b329ca455f35cdf5cae9a9791e8d70f03"
                   "ed8226031d7de352ec4b999e113c23e9ec16cce9ab1baf49ca090abf009389561d25013963f93d182c1b8e3ab6d8c563e6f8ebba125281e93a1f5b84f370ebb51b03e36212697b32b7c272f6e4dc0be86364020b659b1dcb3604322af2d5122e3b6b68cb26ec8abb4ead01f9b13d8a1a7db29331b1ce9b560158b4d89bb961");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "693b3235e6576d1f9d17b9d200b9b289306a1f03c0ffd2d7bce5d008e95bb2d94fde4be2f5e58b64eccfa3da8e1a6ad008d500f81658b17b0e321c8d20639444dcccc6aaf4e39c1144ab62a60fe428498c7e679ca154e75aee17dfeb0d885f3518ebebb46cb868cdad933d5f0b7eb29cc34e9b17e85b3b1d637c63493933da53");
        bigint_t S("54b78bec4855765b4c51925778abcc0275b6e9f55419bb54ad66f0d8e7f3cf68cd3d5b299e634b2b26bb78fe9eaeb6f8826cece71562f9760554c9b2dade54922a367a937dd9c99e60df11a5daf4b10f6e756961f2bbff96d5ca3933b3bab13c338a46469c8b5118e8e2b06b57f0487c23a5d145c3308989d8e469690ebfb14019"
                   "89b5677d1b9262f0a62d38753041ca6eb09069d4a679eb68d4d9ee7b64fc1e1c8260fcbaeeaa83c88ea603109e76c885e8ba1ee99ba1fdf37a7932398341f59771efca036bd9921957ec7b972125eecf2319f0ce39d4de21fd78e98574794deadd8f55bb8c03f47592d6877a1fa0b66d6d8009ecfe58a71fc07e021505e8ad");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "c7960decb793f6d8414918eae60716868bee1c7dcdf9ce30943b7ff9eb68a4c60f978807c54641cbf19fe22565faf2d5f7a21d24d6dc2a101e252da9a9bcfebb2960e9ad735ba7a91ebf8dfe4df004c2c96239bb53a41680389e70db251782723d7a9aa669d912bcef32823a47f134942ab7d808eb5a79c70a2db05507811832");
        bigint_t S("de64be44ee64131b4efcc32982ed33b52e58f3169139d002f9b61160dfed8cff955acdcf28165a997959491d9b20d03e4e297289a68efb1e9ec3476ae7253ffce743ba4abe64c1c71310d6a812b75eaf2c18e6745da146da87e85280ed8cc01f4330d51b1e77231e25021b7712793f6e875eb7233b11c9c003da5978f6af3dc51c"
                   "ce8249ec53bcaefb3a52c9b9ef9392a89051bb0bfa67cf3275318d35ca44dd71a5d67809be05c054ac97c5a6f65f73b7fe142495260c882cf9f5d5b455b22612021b036e1197803dfb1b45c522b69156dc549974b865c2d66d3871cdb7cbb12ab5bb30931a9710b2ffef326b3954872a56eb4e6274ba4bc40a1c19ca154cd0");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "6278d8bc32dc6b039f138c9260c8c90d98607fafaa824cea4fd7c74de60e6fab0b24e27a6ae25079d5a72e9695672f0efef470d6477cf23a98534ec463f3a45ff842d5bc5413f2f53aeef4708614ad07bdbefe65e8a64bb3a3c82c75716a65ae525a194b98773abef67244eadad62b3ffc46912637063d9f99bec928b67a9ae2");
        bigint_t S("98ff167b985e030585d170aa38a1f85fbfdede90cb57f5240876aa8ae669177b917dbf8fc00852bf85116cbb9002f465dc09b340d3bf1a7f96f87fe7843d78e3224af76108d238260598b2eee93055593b714b0f3f422779155668ddcc2078c74cf935a298619c661a9fd313bdd5a65ac4edde4625142905029402a3bfbc111e16"
                   "cf403552abf2af3bd11554e50d5809cc13af82ae894113dd04c65605de2216bbf66a08400566451f4254eeb02294f8f86be0600f35349d919a26272471c25943b53147cdd102d268ffc7b3cf98738d837823831109adb4593aa38650622ce8b64e94db3ed35000fe3c8c6cd068009fd707d23b5f94d0b3382e207a98e255b2");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "e823b2f062bb10a41f3ab2a0c4bd110b2dc2846f0f3a066adbe039a6e5c8ab0ac53b5832fdc2711ddd815c26a4c6fc36e8e232373838a4ccff93bd3fabbdf5bb0f4d52bb06c02ec25acb3c4de4f0c605f450383af3c0e28d461efaec76e6e0c48e00a671c5dcd0fa5dc158fbcb62f6e218b39e5e87fa49157829f8968c6bf68e");
        bigint_t S("86f4b5c9b7db9cf7d09caae1ad6ab3043ce39d14c56c6e4fd8701a8eefd66a1792c89cc105f3657298b293894e999fb30e30079aebb1d4fa0f5c6fcd5096f50574592696a559d0f228d18d4ae8f75b508b714ffb3393a4e560b0d004bc4a0129387babee466e0c9b1a223f3a87450de0e4200733af92745c880a155f6d9cabba81"
                   "6181078fe4cd360ba25a45954f4b52356435964789f9503f889799e55a3f3c46fa32c5195c84956e56913cad423dc4c8a6ed7f17f1825bbdc53dd33d3c3b2612da4ce5e89622d62aa0205ee667a9dee276b91dc7bc67a8c1b379930aea8c8d26c4773fde955e4afc371197dcd8cfd6c619c2f0f57c0b2fa08ea73cc18c156e");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA224_RSA_PKCS_2048)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(2048 / 8);

        rsassa_pkcs1<sha224>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 2048);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha224>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 2048);
        EXPECT_EQ(f, true);
    };

    bigint_t n("e0b14b99cd61cd3db9c2076668841324fa3174f33ce66ffd514394d34178d29a49493276b6777233e7d46a3e68bc7ca7e899e901d54f6dee0749c3e48ddf68685867ee2ae66df88eb563f6db137a9f6b175a112e0eda8368e88e45efe1ce14bc6016d52639627066af1872c72f60b9161c1d237eeb34b0f841b3f0896f9fe0e16b0f74"
               "352d101292cc464a7e7861bbeb86f6df6151cb265417c66c565ed8974bd8fc984d5ddfd4eb91a3d5234ce1b5467f3ade375f802ec07293f1236efa3068bc91b158551c875c5dc0a9d6fa321bf9421f08deac910e35c1c28549ee8eed8330cf70595ff70b94b49907e27698a9d911f7ac0706afcb1a4a39feb38b0a8049");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("1dbca92e4245c2d57bfba76210cc06029b502753b7c821a32b799fbd33c98b49db10226b1eac0143c8574ef652833b96374d034ef84daa5559c693f3f028d49716b82e87a3f682f25424563bd9409dcf9d08110500f73f74076f28e75e0199b1f29fa2f70b9a31190dec54e872a740e7a1b1e38c3d11bca8267deb842cef4262237ac8"
               "75725068f32563b478aca8d6a99f34cb8876b97145b2e8529ec8adea83ead4ec63e3ff2d17a2ffefb05c902ca7a92168378c89f75c928fc4f0707e43487a4f47df70cae87e24272c136d3e98cf59066d41a3d038857d073d8b4d2c27b8f0ea6bfa50d263091a4a18c63f446bc9a61e8c4a688347b2435ec8e72eddaea7");

    {
        bigint_t Msg(
            "79bcffbfd6bcf638934b38e47a1b821dc97cafe1da757f820313989ebc01ca52ff5997abf5baf35dce9b48b8f0debdd755a8b81b2e71a1d8cd57ea4dc1b84cda43ff536dd1be1c3e18fe5ebc17d3a7c68233e81f6407341c0983c5a01bb3404a0b5739edb2f1fa41391c80d8361fc75317c248d5c461bfb8803e317f101b2e0c");
        bigint_t S("5cbc1d2c696e7c5c0a538db35a793959008564c43d9aa8ed20816b66ef77124eca7584631308d0fd7383be62eaf799b5e67e8874cc9d88d507e1bd4fb9fd7517adebe5d583b075040ce3db2affcf77ee0162be2e575413f455841cb6ea4a30595daee45e3042b0b9d8f9ee700df3f1898219777c21ef3695af95628ae64260dd2c"
                   "b7ee6270fb06f52ea1aea72e1a26a26f2e7cee560ae0cb8be323113c3f19c97cb5a3e61b998a68432aa2d1f8c8c00ac92b0f35344710ae1d6d79f379fbb3dba41b46b9c814eb3a25ca64a3ff86af613d163f941a897676652e7c3f6769fd964b862dc58cc2e652d0a404e94853fb83937c862c1df2df9fd297f058bf660d15");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "8f27e8fbea155d20335395dd11bc9572d42e4c6009d5cace4bacae2b5c40dd1574b87a79d81212fcafaf169a11128f8d62e50d71de3ce0a5741e8bf4d8427675d416e3afdf052301f0fa0a429420a0a43fed19dedf5e088ab784cfcb6b3f5a34574a0de52577faf79f3c581bc16f307d134e5aef36c2a843bce561f83dc1f11a");
        bigint_t S("9a61223b98303c51dccc54d5fc837c6fec1720fa22ff69c277be46c388430272e3c8030d61b1c4946d329fd2aa408749a3ace2a6e1efe58b84edafec5deaa036341b40ac5310d12326fb65631cecb25c13fceecbabd4a87cea8f185751309d707c920ce87a096a1b942182e209f5d7ed55a18a58cd4f2b5249ee723ae7c858fc53"
                   "0e70730f25448fd641a35d314f5c511dca411f8df85b74a9ee8669095f5aebb4687efe052afeedd53eb1e6b1757f71c5622cc03dec368660ed50b11f2870587e277c48e554470769fa496efe5d161ef3e6d04003f2829f7affa6aab9ea54cf9e5bb794bae48fe2f92e1294f01aaef40e1e2a5bc99c14b992c1deb66afa7f5f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "cfd6f7246114ab011c28dc654ac7bec465172e92d86499a006141aa88ddde227cc35dd6aa30a93ec79b44cdf84287709446e4cd15155b19a57655a4f5f15353f503e052cb5d04c9ec0c46f7d8f5115dfaf63d01fdf49ff2f16adf32fc7ca503e63d9b69020cca1c0f93f8e68f5ac60f0adde01a308a84c084ad62b6f4d64fcca");
        bigint_t S("2c32eed41bf7a1afd0d567063fb253fb8e6b2c9f21689c1c07a1b13c858ea085b932e3ce60bcb7af3eed96523a6d77fb92f996d2eb0357bf7cfc2532a6f584cb8be0b3cb545ce6a0a972ff83137d940100caa4301370c02dfce781079926ca6a38929d618a8dc7e51b4142d6cd335cc08d5c384a9a7de9bc2fa121d901ed342b47"
                   "0ac8407598366bba7e122ba993b3ebf3df5257096c56303eaa9f2fcc4daddbc135b76e2bff1112675982dcfe483fc9b1d6eaf19dbd0e52cfa2bdf58dfffa2b2ed38433a5b947beb40b83433266914a168deba84b4c522b88f6861d406dea78f1212d137ef1d17544de967ac5bbb25dc9d4464cdf2d0bc70c1b280ec83a5bb4");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d737bd2d037ec4be9e03781a6755bde384d3fd2bd2b91d947a5bb0b5c5ab6bc03101d962c41c50eba06424afd8711aac82d1206d0d063b2ed5cfc040f5a9c7af5c94dd23241af9041f8eb2e1b292ae13e6db5e9dbc39e857c1d1654ce3ad4d6083782f36cb82a650420b88789dbd605fd97a1f8586580e5866ea07a036c2e51a");
        bigint_t S("bc61e594357a5cccfde0bf6c99fa1402e2222e7f1a416ca365d6715343b481d70ca6f2533af85826bf57f9ab3f173dab433f34868c9a9a7eda30921bf8c9892b7b26f0b64d01b5262f878c4d28af4d0ec0452bd2235ecbc5023ee0ad524fdc691fbd5cbee2750726000156554fb0dcd69358ad01a82636c90299c059b9f15c74ec"
                   "6c4db2c754742c39ef6dc67eaff256fcd1058c3892e3e4c3fca89323675e9212a86bccfc81c2b6878fb1bdce1d2255d39457a387b5514f2a25099deaaf0413c5304f5dfc6ce163458c7f261cf8cc31244db7fb7646413542c543de6dacfce8b02d95b904046a9417b8dd17688cadb882077d663fea2ec470672f7abfb0d90d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "05320d0d47199301120046f64d46d274acda40f21d087749639202690003134b5940c8271560aee6c6bd84dacb4b41ad7529589768763ad444661c568595fbb7a89bde7190e05c5127b25d29489425c6ddd98e21447294d9f80517e16d3fdd763b6ad7557f42a52a7e045b4b8cc986a62d126af979a48e41c5967b747fa8c072");
        bigint_t S("ba9944e9574b763dc2e86d2189cb850d1b3fea01e03f615c1c56e0c632d436724861485328d67592c59483bdfd8f1c5887cb51a0660bb81f158b6fc073c363bc31f9fae11c06dc5e5057f5708866e7a486d0083c2a39a2a86982930229d02da7b669edad764f8224a511ed519953f5e22dda9533914352f9eb12f4580c025d94ab"
                   "520ce80b14c61a4a4bb23a0bb89c092079c14b0399d841a998644f8cb1396ad54792f4e38a6df84d32bec940ff01e117506c0661b08dfb0b53f929f43f286d3ad94f5e47dec5d6c394abd475cf543bf40daf42a6ba493e95241c1efe91503c6eb9ffef95958a1a3f5406e7f22f543ae4726e3fea490f44ca09c8f82aa5e28d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a032ba951de84ef9d6519bbbcdf35bc4181ff2f5e5db96bbbb20b3e403468eb1c6bc282cb8fc65bb76be6df60c6c53b6bad7f56ef82823bfb7190346ea65b3b9457c3132447c78ca705dec6ade6bf8def8faf62b7f554f23d1f7e33668feab8ba4dc9c50ca50cf4e4c95e0c04bd994655af0bed4854a631092221c5c86f9bc50");
        bigint_t S("bff7b6c06d464ad3e5747641e5d66ddc7e5f623a735d6d0437851fdc0ca710d10c7e138266cf9a4ce95e41838e29678fcb04d7a3fbfa884e540dc7c4db38412eb95c68800910dbf91f13a9e129f8f82ac928cca1102eb625d9ca1f8383e561cfd36348879b0bd79ce6d325872de49aba9d6e84478c11cee47daa3471cf0f1993a8"
                   "38a338b688d05e9aae426a0281cf31ca7b6300251437b0b407057ad84157443384f186cee795677931b21ef34068318431d30b986ac80c75d5c48301455105892f0544288c90ad7bad6ca78acb82a48a9df9c9a6dc986de7dbe29b3c962a0165f0d47d2ca32976cf5e17cd77882385c95c943d5d38b0a6a37d169275ac7bd1");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "e871e6cb03936cb8fcab5a87027e77b23aea33b9b4123b679ebb4a56b7f642b507007b49ce665bb2ba6c27f05cb01825dd0bb29cedb8510bfdb80515ae749f1389a50c14f071e22254d639c8a94cbcd117a60051f33a14eaed4159488b8193eed629413553fc2a9134b13917d09a8a3c5185c5e0ace0ab8bd720eef6366346cd");
        bigint_t S("71586262d9940bf8a372a64a31df832aae189933ffa5d03b636e89a33fc1cb4388e98158943cd97c678de28d4f9766780b963f112fdd6def391bcc98ee8239d2e8ea4a9812e38456c444dab314b19032c41b20efe647191d74853f0a10daa9a999a2c770db365c4c903cd2e3b2274c0583dacc404a722eb86dc456c475f2a6524a"
                   "ffd016b7d7c24d4f8c83e651ea448b3b556aee7a33b35c3549efb961caf462d49db7dc8ed5c71a7d7d655a36a409808be865950aa5bf5d85b4b520356c9c002a1c9398432067b161a51675938bfbb4ded9bc4b32401fe2953593c3a77b7415b2a4ed1945f8417040b97697fb4149e8fced862142cfacdb096b4421ad5afdf0");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "af9a8bcf29570b646bd2141e06ea57f3e85fe3cc258a00ddf74b5b320342648bdd42f56acf02346ad616bad2ee1ef56a8f3eca8e78ce7f80ab52d903b9ac81253f90ac5cb139fbd4565aa0089458c3906eb624394bd1015bf9cd3a67f1cfb3c0dd6cfacc57622a4af4860fe681fe8a0a8cb037f827fa4e16e8a4c7883413c204");
        bigint_t S("07259c366fda803169ca35c963deba60af14dc1beb87685b3b078b781ad8af7b7b11a3414559d4e6a67e335bbc69ca8edd5ff7e71716d86b1e5bad37ca020d380dcb5e2d9b7040751015a061753230b1550e41fc306056efaa43473dda95d2afb52c45daeba2c25bb446828b7b6215e192f35f775c047430c51f591e2ea77f65dd"
                   "9438597314677a027c99479f2d2e4175a2eedc1ae481cc9e840dcd62bf46dabd501388245415453adcd57c428c315f6bb210c323df5004dff3fb1f7c8b00eec7416012479177903ada20b1669a13eee4359c12b8d24972aeee73d41593a25fb8a3ad12f8662c1be87f37eef65b9796bcb9fe538f0fd719e886ec9686352a87");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "7d56334f1003356b971e13847159487b1228040a56c724ef157e624df824c94f8be26d0e933958b5a146c7e3e74597504b55fab4ba7a6ee4bfe8f03e547f6c48f892799401ec43e73415c89993d1483f76eaf688dd2ff46505bd7e6d397d667febeec0a90325de4877127f711c044390bc6f624f68ee9eba0f8476a050279d3c");
        bigint_t S("4681bdfbe70c05f779556f880a293a73e54526ac6f5231f45cbc015e7ca03767a291165e7cfcdfac82955c23a61ed30608a57ec0e1f2e61fb300289c5fe2401be5b08f20a37fa06354a5702ac40011553f16d90a1429c851558d0710313c8236859f882d46100dee4a3091b5742c747b8414e5e2ad6767bd557505e9352af40f18"
                   "7cec49ffbebc34c4ef33e6428dcbf3d4de34c4b782ac016203b7c04604ca5e7dd23bb78cfddbc261b396806444f1f91b1e8a5a397cb477fb510b6170b9bd8cc511e619e4805291232cbc8a0267ad94c26f603ec2f73d3807764a9a114e2042a787d62846f458b76bcb728cc9c9fdf67859718a4e6910acf06b6c222be867bc");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "f96c50dfae376c4f4dd0849edded04d74b995ef3bc35fefc22bc7f730d4b1ff0c5e070089561517369e11d6e9c20225f0b5055b86e1f431de8f14bc9dc6d0b2e033c18dbbb0df2a3a2dd37fc647516a5d134078c330eb9eeded54cc008b7c14edb09af22c9433727204b390edef6c8f1c6ccdc36b30b47b72defeb1f3a312997");
        bigint_t S("5ae33e70f8bc701a97ff7391dde8a1b498177330d4ea94c98f4d109dcce74588e9baeb6eec783d68c10be45a9a895253d24b289087c2a901e213a21c88e7987c4a62a5f3b6b6e7c28bd1adb12df175dbdec22f48e45d441cccb1781878e5e531d726e3d8658da8727a2283dd9f7e78a8b46e5653128d95254978f52099e6cb230c"
                   "c449893041fef63a48e499038574d1fcc3e1e26b6c85f38aa1cb14bec8c7600c0b7f391e006c9d5db224718c548c19c3667c43b5f4ce6514ba41bec0c2d9b270bdd5c911d92e2734fd11d0387a3b6500cb3530d87c2a647b1c7c05a4447a4139707aae7eb37c05fd966ecdca8b4c8d11b141cd16c4d47ba7520bc4d16c1fba");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA256_RSA_PKCS_2048)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(2048 / 8);

        rsassa_pkcs1<sha256>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 2048);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha256>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 2048);
        EXPECT_EQ(f, true);
    };

    bigint_t n("e0b14b99cd61cd3db9c2076668841324fa3174f33ce66ffd514394d34178d29a49493276b6777233e7d46a3e68bc7ca7e899e901d54f6dee0749c3e48ddf68685867ee2ae66df88eb563f6db137a9f6b175a112e0eda8368e88e45efe1ce14bc6016d52639627066af1872c72f60b9161c1d237eeb34b0f841b3f0896f9fe0e16b0f74"
               "352d101292cc464a7e7861bbeb86f6df6151cb265417c66c565ed8974bd8fc984d5ddfd4eb91a3d5234ce1b5467f3ade375f802ec07293f1236efa3068bc91b158551c875c5dc0a9d6fa321bf9421f08deac910e35c1c28549ee8eed8330cf70595ff70b94b49907e27698a9d911f7ac0706afcb1a4a39feb38b0a8049");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("1dbca92e4245c2d57bfba76210cc06029b502753b7c821a32b799fbd33c98b49db10226b1eac0143c8574ef652833b96374d034ef84daa5559c693f3f028d49716b82e87a3f682f25424563bd9409dcf9d08110500f73f74076f28e75e0199b1f29fa2f70b9a31190dec54e872a740e7a1b1e38c3d11bca8267deb842cef4262237ac8"
               "75725068f32563b478aca8d6a99f34cb8876b97145b2e8529ec8adea83ead4ec63e3ff2d17a2ffefb05c902ca7a92168378c89f75c928fc4f0707e43487a4f47df70cae87e24272c136d3e98cf59066d41a3d038857d073d8b4d2c27b8f0ea6bfa50d263091a4a18c63f446bc9a61e8c4a688347b2435ec8e72eddaea7");

    {
        bigint_t Msg(
            "6504921a97cd57aa8f3863dc32e1f2d0b57aff63106e59f6afc3f9726b459388bae16b3e224f6aa7f4f471f13606eda6e1f1ac2b4df9ef8de921c07c2f4c8598d7a3d6ec4b368cb85ce61a74338221118a303e821c0f277b591af6795f50c40226127a2efacce4662fd7076c109eb59b18005e7165f6294a6976436ee397774e");
        bigint_t S("335ffadc0b1b8bd2b1eb670dd246e76dcccdc955a1687a15f74aa3e1596ebd43e607c640525f89dda95809cfd065f1be4e4a249477d24f400d4d4c9438a0af95b26b28b416e42aa950e2a52851b52132048f1b1ce944322fc99c1aabb49b7fae4c2f0fef674b50adee3bbb5c6c33822b608e4b9577275ca20c710af9fc41b1c01d"
                   "9c0ff6f0d8324dc08e1a76e232d8feaa06c73bbf64053bea35f1c528b2722764822ef1ff06246e75a9a22a10da4ea84fc2441bea24b35506f8447fcf69093c5d21ab0305cce2c7ea9ffac357c664b491fc55f2919ec490c38accbab378c252ac2df3845acff575ec7524cd2f586cca1497c74f24b299d6d6254c8cdb1d227d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "1a53a9d6312918f3db6bd5766da981854166ddf2b613ebc8793f50230308fd4563d50ee73e47f2cd146ccd9dc4361af1532c95abe96058891ca144f971f1a22e203a0a73183c84bf43caee8a0b369ee01d0a48b46df821767a1444e8d341552ddae96522c8f216f2f1ed00e54a9053792866bdb44ac58eb81a011a9830df5e6a");
        bigint_t S("7b17b13a5ce4485ca3b979be3b14d77532fcbac14e460a54515a6aa7d213ef1410cadd009ce60bfec45bf494514338913a2792bb6086a4cc21dbe852c2b57a3ff5931039973cdacbba96e2ee77fb8ec6c87c14ea10762e2aa9279ae5192ca1c87ededbedfb305fb006a51602e6bf4dc43b994f419c979b4f30719827a348948214"
                   "ee71565361f668dbf81ef6e94297a3ccd68357fccca6dda237911c938d6e71787b3d4085c66c7ca2aad2db16274df23ad19211aa6380be75cf0b19257af4df5af1e62010dffd23b9742f5369e008224ee40331f2387422095f6e5afe0e125f855de220648a845cbde542ff7f99d926868819aad924b3919654483bbc6d8dd8");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "ddf5750aba4be69c944d71896361f210f961ee6b38f96e5481500ec76fea510ac6cd37eba6e21249059f95d55ce32e1a9f2196bb73c1a6fc9cc36c84b4a67e6223888e4d4b8e3f30038637e647cb75412d69e5521b8232e438f1dc4a202c8a5bdc6fa1ba1117155b774c80428de2718e8c8106b79904bd143e2087a01b461efd");
        bigint_t S("9e32925845931c2baafbaca8df109cccd922ba72e6f4b838a76198b1f3173e9069ccf75d0c96d6e2644ade92b29043f6bdbdcfc8b0b2794b4ee16b1a8ef8eb2dd2fa3da4bacf7659e43e70d69c132bf9cb1ba8584bf3c1e0a7c8803566177742c977e447d357fb0331dd3112e7d3c0f1b5772a7cf5e1e368cb58b1a18889cff913"
                   "6bd56b5cc3fe6bcfc5a9f26dd60715a807f3c7ca236c89ddb36ebafde62b35a624284121f56d81f5a4d215b7a36976eed2c78a3c44cc864c6a5c01f4ef9f54efc4beb947b96e9aeb50af6221dea96d620baaad235e9ba6a003fdd39f31388a4c3579dddc0a51e879e1286ac142601d2e5725c1ea9d44aa896a12d29047aa3e");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "f866ae4a8c37ce71c0e047ecdf0b6bd07f8b11adbd92f363ca245536a7cbe8242c8ded754f1b9d95a34bc91f1fc8be3a21086b28b27fc61c0c136b4c5acf8648bedbf48a98aca54e067845a4a7ed1bc3a52231a0f2ceb5bfd1b89f49996e9460b53a0aaff052ae45f04f02a3ffe04b1312c0aa3c03e589f422fb3855ec10d813");
        bigint_t S("bd4d7db1d17c3f56062a1dd4307d939ddd403cbf04b0866603541fa8de37ce80bf06d796efdead6963f603d81e376dac20454c4964c1e6fa347c8dd7684215580779be97f94c8279efd0169ea0cfdd282d3538bafe067ff5ad9e624cbb24f42f1811287fda0903afd3b516875a8ebdbc81b4a63286a35bd66156f657f110b9909e"
                   "983043133cbf0d0fd1cddb4a3e1fe0962e412af91682e44262869524c92557f5d8fb385fa16e87304aaf534e90d256684600381770b77629d1a7e8f4cbc9f956a1e0d2e41bf57207c677abf6f38094ed0e6ad4810d49eb09b51ed29ad6087f72ee019a4f99a816d142c155fe91c93c38f928fb1558e3743d545ec3cf8ff4b8");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d737bd2729d1835f243983fbc152a19a2e659a5d211ff3006dd92f4ad31d45a81a65511561258a3cb50c74e1df87577d7f88ef285f8565bc01c37e387ad41362835fc1b1ce1fe169962eba087a84f6e18bba5307a4149b6cfabfd64af357bec23cad73c416c6702ef027be383b4f670f7a862ec098d57d587f569f49e6ae2f10");
        bigint_t S("581e5c157c6b960933bafa5d7fee2746596db490a57761ade4aea0f76ef658d832b603e8cb9e03a853be53e02d4deaec2e7764e14415173516935db1b868c8f7952eef47c0cc57f011853b6a77ec99b56a01c0f4a1b083f123e3dee8ee1312949789b3a410908f2afcaf81a18ea6dc685301d471f91b1d730693329616a4aa460c"
                   "53825c3e24469571de2b55fe7b9e2ff5031b2d3e2aced2432fa85c4ec2ef0356c23ba81dc589a6aaa470d15555636337014cee7c6c76f5ddf35e1aa0aaf342faf80d4674692b902c8465f60a8f55d6b3c2d3ea5a6bcc24ccf3fb3df7eb0e50c845314d26ef5ed8141b73f79ce64d25d0e647d66c875503c921317892d169fb");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "10e3b453719469227b51da0c41b5cf0fd94b32b6b3960b3c2855aff5560433ea2ca627c3c1c281ebeaacd8ee0b05d3262d85e09651f829160487c4b13439294b42baea86585a9ee7fd581e35e761f5221054112e0c48bfc799d0d6b5787e31b57ba56364e063a75fdddf16a97cd01d4da93f2d44898db4980eb74dcac9d93372");
        bigint_t S("dd2ea03939bc68d1d40835a099aaa3ee57a4f11e30ad4485089074d345e57363bd6d3c17b86cf35d0629ea91730a0c8ee364ac80a016cfddb6875e1bff463ae284b84959e52046945ee916256582facae719c33c6e892a3a7aaaacdbc300c76486633c8720e6207beb6401f1258997a0bd33c752ea4799ec99685ac1fb9066cae4"
                   "5fc861608cd74aadb367fdebd28fcf88f52ce4942f74c8a53e809fe4953434d6484113501a40605a11c460bd9cf290e37efe33b161b96b63c2270dc1c3669100d1085afd738cf8723cc9f232f679ffee07e1d536a7d56bd2248f37610fa582671d6414440caffd10aa3d09b95fad2c2c41bf0ca33699cf0749110f817a4f63");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d130b2fd9a7a1f118a5d9b6c4bebc0d4d44fdbec8c613766b2779f74fc7d1e7f7e48091cce273f3c66bbb0a249091c9beace1de9491268005f005075bcf58cb36fd739f026a8235f965b40a71de67d95a698bd0dcead1f474520803876c0424d6a864b5fe92650e4e3e453620fa96a2ad256c3426258e5a32b7d38a47205c8b7");
        bigint_t S("2b6c07656b398f3b3387627f51aaa51e8f905ef07d7bd82b355396bf32865227f5bf3e0cd3e8e1aa44841e4028e61c58cbe839fb8b7582652c37f7fbf8b51fab980e76635c38aca98b9927dc1e5f17633339d6e903be5bbc65fd7a7a928ddf459bef84a6370c3258dcc810cfe272274d06a1dbed89bf121be3c1699fc129d55d3d"
                   "eca9d7b10de41bd7f47944e26d0e94be9a708b10b3a729ef9ae59ac986de71fe4522284873a19bc177804ecb9acad651055e7dc4f5d29590ebc6d6f965a51dc5b4589699e01ac42aeb579032d5954f160cdea380b0f3c486124bf8ed8884c6861388ff7afaa2fb0b07cc999048135378a16e006d83a98837d259568e89823d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "811fb8028f9c8ce411dcb6ee9f6b9db60928b9ad8da65da14dd3d48600d7b075e73f5ff5025bff7a85e4732b86e2b7e8f8cd708cdcf3c86fb61978303e6526b1597cf26247e74cc31ebb08247779b4af673329a9b1f060f5395287dabc765406bf1c9ad2c5081ac7c5a89b61dbf63d143ea00a8d7266ce3abfc4280ce1d2e27c");
        bigint_t S("c33f9dd337fc09da689d1be3055719701581c3c1b41ff7e574db6976ff7b9911c5a4382c9fac346b8dd6ef150f5d194853df3be384acb03b681aab5c8844e31d39658f731417ac942b88b2bba674e01eb151f35add8fd0e65c4738b7150e97c20e577bb320d10f872800df2df55095ac67b2fe4b51805a7c6dab3c288eda94c916"
                   "99a7f85929a3f98ec92008bf884d7037affe0638a91c540c351e2cd41b3b26f38c4ae7033fab2115f840a6df4c08173c7052499ae817052c068c6c34942d47a0adefd40eedbc7cee3f4898c044f05790334be9423c5231eb5696260ab8f7d6fc6e5b1b86a7e362541e20513294b9d5206440eee2198976584b5840fa68e5a1");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "cedfd1c0126a62aea3e7a30a7309c258c69ab7c2f028276760fb88f5dd4a877a10aea67743d62a37687d345e5d992ec56ed568851c4203521c490f0ba3e5f08ee10f06d1cb798d2f1e7ec1ffa159466baa96c880e3c7df814e7835818765ea304aea37f634ba523eced49215c7b3d4f381c51dd8d01f819bfe6f437f12323d08");
        bigint_t S("18c454b50814f00da88589f533b87f8ea5195bedbd3e1b2fcce3faca0ab768d71d930a28b29d24044c231c0f24041800e762cd7271c545cdf7f9913f86caa5ea493d1a994902f9ecc1c0daf1c571c2aec78c4ff75e873fef3d6861432f227dd5de635d2315c93e10bbb3cbab1e981ab547b53383143153c0a4a3ce2d7576d89e68"
                   "8327f16636f3ee38f6dbddc0ed42af36b08247d9c91283ee2c52811010fb0a10a3de69b6e42fa11ad44f76f4766b9d6fd0ea179790cd37ee5aa22b62b8010a242ea15c2d0f9cd7564f1645b65cc438093944d60eb0dc42afcd35b16c54c9628070e5eb93826003c84ae66573b84a623a288997bc57a87a3034a392a4e54b80");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "7ad20fc0e2886691319e5c41a6e4d438e30b0bcfa95a7ba8e3a20040e6743ac7f791322f0da24083d92a2c7a04b8934cb8618cc4616d8c96b55187f4f2de585c66b763b4350879985775fcf3e05143cc84113f635a005f7de034b2fc40c04cb2d12bea77e121d979b26913d4c217ac634f036cd51b79be036d32c8805883dc72");
        bigint_t S("3f48d2f019745b091588182451539c3b1c761016a8d3bf386b3ef046e75c23628be14aeee1db6bb876485a2be2503b39603ed9e07fe35f61cf2051bd8e4ffb281ee960b171efc49f61be17356343aac7d260a1f2b29a77cc81dff36d5d5e75dcb11151501c621e0fa1883ceee2b5befb8b95274f0b3b3f0af0c2282a3527a20034"
                   "6f27c7a6755a5a4f5be9424967e1ed09a312d0d910156dd8df5f6f209ab5bc05b86f02c3d3bd4526bfc0f92e3542399592b5ff47792e3fc8b11e367496d7af6013afff2c433f91f9ae479ec70116e0663d53f8e4de053a0da5c93463271564768a7c0df098d744758630659b84964d61aa940ee0aa3aeda96e8a14dbe0f813");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA384_RSA_PKCS_2048)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(2048 / 8);

        rsassa_pkcs1<sha384>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 2048);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha384>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 2048);
        EXPECT_EQ(f, true);
    };

    bigint_t n("e0b14b99cd61cd3db9c2076668841324fa3174f33ce66ffd514394d34178d29a49493276b6777233e7d46a3e68bc7ca7e899e901d54f6dee0749c3e48ddf68685867ee2ae66df88eb563f6db137a9f6b175a112e0eda8368e88e45efe1ce14bc6016d52639627066af1872c72f60b9161c1d237eeb34b0f841b3f0896f9fe0e16b0f74"
               "352d101292cc464a7e7861bbeb86f6df6151cb265417c66c565ed8974bd8fc984d5ddfd4eb91a3d5234ce1b5467f3ade375f802ec07293f1236efa3068bc91b158551c875c5dc0a9d6fa321bf9421f08deac910e35c1c28549ee8eed8330cf70595ff70b94b49907e27698a9d911f7ac0706afcb1a4a39feb38b0a8049");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("1dbca92e4245c2d57bfba76210cc06029b502753b7c821a32b799fbd33c98b49db10226b1eac0143c8574ef652833b96374d034ef84daa5559c693f3f028d49716b82e87a3f682f25424563bd9409dcf9d08110500f73f74076f28e75e0199b1f29fa2f70b9a31190dec54e872a740e7a1b1e38c3d11bca8267deb842cef4262237ac8"
               "75725068f32563b478aca8d6a99f34cb8876b97145b2e8529ec8adea83ead4ec63e3ff2d17a2ffefb05c902ca7a92168378c89f75c928fc4f0707e43487a4f47df70cae87e24272c136d3e98cf59066d41a3d038857d073d8b4d2c27b8f0ea6bfa50d263091a4a18c63f446bc9a61e8c4a688347b2435ec8e72eddaea7");

    {
        bigint_t Msg(
            "db2e9fbefd9832d29e6015b976734ff15a5c1177a9153b38e61d22c8a6ca9056ade10de054645da32eab4ad6eab6a4977bc28526771ad951bd301e2f5bec6911af44aab6cc0d30be1e1170615261026170edec3d4a5123a81af24f39674acf642880fda92c3cb30bb9b90ec4741c71378004cd26ea622025458b3c4f918bca37");
        bigint_t S("211398c068e60ae49ee2b8b7cd96171373f4664cf45b62415d9bcf119184c03274aec2b3f04c598043d1d8642d53edad68d0e0bc33fdc65ed69334ab52303c839cbbaf0586508007dcf4ad5d297f568039dc16a1e6c08108a0050d5dcf47523e4868e4be72f20d789b06bcb92484efa2fcd903ae280f42a509dadbe62dce1c6d3a"
                   "ef6e4ff78a745c8500ff0572748abb1a220495c1c103a72c940206347c16ae6082dd1e4624525aa8ed8f118d09be2b8535d042b29ea3e2c6ab3c990b4b59501c3bb9d602c7813a55f4efc129f4dfd0fabe0cf6b8c8c1d9126c5440cbec5405722927389b3ca80a0872711388b5b099cce6effedfbd9befc9646ea8a258fdd0");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "6574a1c157e1d5f399daeddd816c9410b0c80862f3961ee4678b8514e13e4e548c37ac9bd449f96b8323fa032a2164d5c3689f3a9431c9655765fe95976bd78f5d3d304c0cc22fef8a5882faa0afe7c0a4def21e55b00b66063fb69cffc517e520aeb53cc7f27e67f4a10b29abc026ba9a538974fcbbe9cfbd42338fdff5163c");
        bigint_t S("03f393bf6a20677a92565076e7c628a10490eb41b4509aa6ef99acb0ab889ccaeab98617c59dbe499231396e9daff7a2bbaeb3e3a10258d4f605b80c41d0166c5f0a175a7b42476c03e57f84426f4de98d74829d40c01453999cebb996c41756d47f0b79fbfdff2faab111a5c7f07f8f760e44d9538fd1e3b3c4307a79158e7dff"
                   "4eb0f0e567bfa0b67d1387310ac4f390674357332e85847c26fdaac1dfe7c95585a604bf70e3d4a33649c6c4861dde05ff11cb5e60ae590657d3f455e6e60ec55d1752493768c92ec954f25b780dac2246a50f87cffc11ce9f78c0c1339d9a7bd9cb7e747ebbfc5b6669f4f46944e897695171727b89550e2f9dff74b3de7b");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "9767cc530ca77270c9734593cd5fd811ea852df6c97bd79d2e825c7d7d00dd467e31f92d5d19ee83f5bd018d4d9221906336c72b295e20e9ce106e173e12acd3eca27066cf6716d4aaa81bb191bcc735a9c10f932f91b987bcf2c12ec70dfc1b6b22fba76a793288739c59a6a0c3132a4390ced7e2fcea9d80921d6b1edb1646");
        bigint_t S("5f2a2b0588373371b1a9b446a2a75a10f2143e7a4d0c0b766e217b7b938eefcf8b1a46ae742f9a390c3b02f627f9287fa7126dc1e571579b87a6beb860a012e73efb7df853bb31dc056f68373a91af044592b2939fd2f4950178bf83de1f0affe6066dff573c3a1ee6d413b8dff77200cad63a29baf63999258a49e55c713281ea"
                   "71e60ad2078cd4fecf6bcbdf5fb2121a32191543a16e51de7d6bb0ca0c897a8d893f00f642111393148f6acd417bcf702b318dc4c4010ab1e5710a5b94f411bc5cb54ea3bf211588f2b7889cb0c3af9545e9a1d5326062eca6bd015f4fc383db371cf6407eb46b501b19a862ed1ef9e943c9eaa2aebc9b6f203e39a9e692fa");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "974cd6e98a4c88f9274b47c8da1483923bf4213dea137c47bd03aafe1e9a00efaefdd8a02ffe9182a0a8d821a917269e3a8e8a5f70c3a905b87d039e3661781c68090a070657e46e2d099c23bd8db528001668648cefa39a6a10539c77efdfbc0b98f484d90088a716961936de1c3617d3b541b4073f73f01ce2205fa9e93db6");
        bigint_t S("9c36a3f2750370dde85811caae4d361a7499f8ee1ead65803fa861100086ee0f5d96cdb33202916c34b7d86128a023003bfb55eb43a19693fb32c17393b77777e34fbec701d6d0daff8f89ec5c336f68e5f680da08bce0e8e567d86601c2ce076c603ab292de50c97857fe34d59f4105c6ced054e5982397cd9cbbf08ceb53bdfb"
                   "90b6d75f231a601098827186df27980736ce97e40974c91011691c2dde453760467b094bdeaedfb0350c1eff96f236af768dea38c65f520cf0b36512df938b31f9f74351b221405b1cd04efa7634ac7e966029f45f77afb64bd7d2be4e5922a150d3bff36f52a155ba4ad168036a440d5060f2ad6e323914bb475a46a5d35d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "cde7aa5cc4c1185b2aa569af9c4cc6ed261e3ff108ecb52ebae1f7422db62bcd54267084e3a2c18157451cdd360ae481983d8539e62f560be22ec3c6e7b4abdd72df0776e221cdbf7c391481b26278f7a5cac0e0e845bed3874377d34800842330fab85b49a803bca36996c199bb48f655ad390d9797b52fb1085b90d7007e46");
        bigint_t S("2dbb212c33cb755b5346a04e428ffcaee7978fcb2a58254d7dd94b6d266ffd3deb9bf587cfb78f902fd5cb1d9bd47bea3759a4d89e00aac35f76ac88f291b71ad7805a7e42d828a999f7ce971697df2e04724a35cf3f5ada29e35dbb4b742c31f3537baf8b6d3bb5068a15d7ee611450c10fba365860dfdf83bf648d48d9af7dcc"
                   "2f90254b65b50617036a93f7528a1bb848c6b4e5e319a8fb08e602a7373a9dc4b337652a04b548fdb1d3153820c547bb4d3f8ac1b3258ddd89d7d6d3742dc5ad5c6f455e4bb7e8df6b640b27f1e9e42e839bde6033beea394cdaeb576863122d1fd5c5abd6eb367194af71ccae198f2a4677988c5913aa3ec2942fad989590");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a48b30a2dd6d2629476cb424ac7b0fc014d29d9939930b7f3bad9a7a9500b90b50c3d9b576237dbcca2d902db6eefc1e85d97837d269a17240074d1e6c2d1c2a20cc50a0caebde702c91c46f54e69e9fcf9574b11a74d7c859672c804b82472f7e6d3c6bbf0d71ccd560f8f26fa91cca5ace3a0ef7fed30bed96dea71b4663a2");
        bigint_t S("16ecba74f9249efb948dd42bde29cc44e2597ded40a345c5fdf03c485080d2865138e937c090b93794b2187b6d7ecbe5166ab311478ea93fcc7143eae4106154bf2d494652163ccf1b4df00b638cd01b05746f282b1bbcf12d9174330e665676620d40907119299769e8f61e56e714be237ca861197807fde2feaeace31d0fec0b"
                   "3088542053f7d54e1c30a069b3967ceeb2db7a1f9f6d0b673ad279a185d8730e4ebe402ad20280bd7e7f286ed87a25c6c1cb3c66e5cd048170c9759dba3f49c3adcf68c777f3cc9acb4b17b27ab7a36a4d6b5f1fdf8df91813c6378aee60559d85a6ee7e9fc8afaf3118dade7d7a0a8f7449a098e8293b59359d967310cfb6");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "e25879fb75fbb80255b7abf6536cc05c71642a6c9635341586e8d541105a855a8ae3500d7a36294e9688809ceff51786691f116e6e184c8de382f0375bf448608ba0d160ff3391f30bfdaa8403522fabae3036f4f9212fe2e9637433d3472166cfd7257b6abb99fa9b5e5136f94cb1d32adfc2869f1a851c7ed588bee01ab673");
        bigint_t S("d3439a1390fa1848a7f9d54078538d3516cf17b535d1c86038362c21f8aa593cfd0ec2e33aa68600e1c03e4bbb8f2fd174b82fb83059fb8836283cf4ba288d1ecd7b48c9c49385aa9d84eb89c304f50ab1d1f65765b7641247a6c21b4d66a3a54cf2af11cbbfc10755eef49c3df81b9b53bca992e88bb98de1e073299b81dfd97e"
                   "1aaeb24612d5b73c0a97f2d0bb1e2bf7eb452bd2cc51ddfcfe47d6690a001e51405a7e93d8731bef1093dd21d1e320a6aa011e973e7e985e2de59a000939e201b0f1098444f422b97817fc60154ba58a27ec3a82e27d998b9b3ab74cc4f95101635cce10c0a8d4379878183ec7a1b6f6cae94c72e756c415becc2c211b260b");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "648de8c418551dbbb731a50e33943671085e3d6c2a0273a33be8b15a0210053819f9054be5c1634dcb8dc226a12bc8b163f675c328fab36a54592e720eee652ef52f02750e5866ea01c6898086a0013e1abd6040f6c2c3ff418ba8d285cd00b460f986477c634e19c0a3a126c83b089f937ba7ea7ca2ccea6c46c8804df6fe52");
        bigint_t S("34de98ac564092d4adcca85986e83402e65982813aae6d50f2a85af7fd22eaa61d475326bcc6798a5f34b4736c8bc0bf591c1091d86a09bff46199aed1ef78db34e575c1ce4778de14eac3a1392a62c4b0854dcf5b3320856cb34c4237034efa1e551ff26f93a86ac0a474de6dfd70cc49000383992e6557bde5c4468628d7f1cb"
                   "0ba987ec39cd595757a48c25510461fca56308c161a0a79079fc714f0611e0f3fff11c2b36a8f504ca903d71645c08a9923185353b454e78843e2ae692da656c7cdb45a80668843e9154b88073fa04b947cfdb6f5b97e6742790f8d485750148d3b6d0f7f8d99a7a8cccd21c4c8ef3757b321e3c05109aee90a4c168ef109f");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "bb70b663d157c330c79bc603ff28c35d41b371a6a4054122b2979b22c7606b77baebdc2cbfac63758c9400e2bbe3c1d8703e87423b010c8440d625b0958745fdeb10437571f95dedf5e27c3a4c60baa2f5072191f9c8b58d157eb56c1bc31fc5960872a4ade5b7824aadc2747b46b6a21837c7fba60344277bb6d4bd23bd00b7");
        bigint_t S("a9f9443f14a80195bd6325606d675c4acc6d383777eba9440b9557cdc5d99d221732be50540410714b40343bc908b058aa94db2426afca198b01188fc2ce530e7552b3fa3c1dd90dcfddcb5582303861433a465aac610410e45cf2a699c7b364ac8d72493e538426c454b694ac2dbbdda92fc5ad0b600dbf02166b1e2b21ea470d"
                   "7df34359925c29d6284922fba05b7b1373dc3ff1a4e8e0e8cd68efeadfe7bf5289a4608aaed23090da391d30fd2dabccb7e938e128bbb37bb4abb468c2c2eae597de1b964c40c691d694d27fbefbbc2876329260d02cc63ee711ef475582ee4de3b079c1805bd8c90130bf13c7e52e955e58878e075d7f2bffc75e9badc882");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "627accf64851e9f19bc86ee8698ae687b950b9e2635ea61525506d27f216cc358d3eadcc57719e9666418139d622857c64b555ac384d8f3e03ac77b54a0dedc602689cba5cf17515e2e15c75c37a20bc6b45b171579f8d6428737c91f6497d47e680008affd755b539336564d79ca090f78836fc685c8ad0bb30e4849dd26500");
        bigint_t S("b533cf67e44e2da3c4acd7cd33af394d26ce2effa55d7b82d31f38afa71982217f66420ca77fb94a0f55d5c18c5e0182c3ea64f4fdaa3f62b6e45482532969ddb8a98acba36217f33211567ee84656f2d689440bfcaef4367e7bbb2d7b59b9371786813e6a262d97a6354a5d704d05b26d92f8b53f4404b482b787aabf20ea512f"
                   "10223fdc97601619aa9629dafbf3d0611431abd82dc904478e119a972a7b4df0497a2e4d40a0565587401e842a319e06f101b2074e3516d07bb78733611adf2105e3ca30d24be1c925af3d377eddd0400c273d673f6b03ccecdb5e32dd894b8544af974ccba58a2124fbb7152add4182b553d66aee70afcb2193d4db48a0f7");
        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA512_RSA_PKCS_2048)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(2048 / 8);

        rsassa_pkcs1<sha512>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 2048);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha512>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 2048);
        EXPECT_EQ(f, true);
    };

    bigint_t n("e0b14b99cd61cd3db9c2076668841324fa3174f33ce66ffd514394d34178d29a49493276b6777233e7d46a3e68bc7ca7e899e901d54f6dee0749c3e48ddf68685867ee2ae66df88eb563f6db137a9f6b175a112e0eda8368e88e45efe1ce14bc6016d52639627066af1872c72f60b9161c1d237eeb34b0f841b3f0896f9fe0e16b0f74"
               "352d101292cc464a7e7861bbeb86f6df6151cb265417c66c565ed8974bd8fc984d5ddfd4eb91a3d5234ce1b5467f3ade375f802ec07293f1236efa3068bc91b158551c875c5dc0a9d6fa321bf9421f08deac910e35c1c28549ee8eed8330cf70595ff70b94b49907e27698a9d911f7ac0706afcb1a4a39feb38b0a8049");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("1dbca92e4245c2d57bfba76210cc06029b502753b7c821a32b799fbd33c98b49db10226b1eac0143c8574ef652833b96374d034ef84daa5559c693f3f028d49716b82e87a3f682f25424563bd9409dcf9d08110500f73f74076f28e75e0199b1f29fa2f70b9a31190dec54e872a740e7a1b1e38c3d11bca8267deb842cef4262237ac8"
               "75725068f32563b478aca8d6a99f34cb8876b97145b2e8529ec8adea83ead4ec63e3ff2d17a2ffefb05c902ca7a92168378c89f75c928fc4f0707e43487a4f47df70cae87e24272c136d3e98cf59066d41a3d038857d073d8b4d2c27b8f0ea6bfa50d263091a4a18c63f446bc9a61e8c4a688347b2435ec8e72eddaea7");

    {
        bigint_t Msg(
            "e4dba4692a6628b501f776fec7fe973d655154268f669bfd47c624aba3be5311d158619c588ab71aa0ac9accb52f0dbc488df350f77c7520ce67a3050d1e5e722bcd75081c2b0e64d0f3483cfc981eaa1c358fc7b9c2fb7ce78ed19513e96717fb2129d4feb1f63c96b4c77623a092b0ea306eb35da2f7ba9d23f4843d8837a8");
        bigint_t S("dfe58c84ec3ee5f11b265e8e7d99b416d8e7b166a38d2b0f9027be73887fad28947994a2bc227dafcb272d2d410af31afe16b96f51c9ddf62b417d03e2af63ea4a58d41e4649712177c85788d837fadd223c4ea7f635237a93b181fe1c0fd3bf2d8a7997cc9bb6ca099a0a36c4b9e91aa780a3d5edbc283cc316a153a101fc8c33"
                   "d0035e6c1e1aca731ea765a1e5fe1406ef7ba8bb8f335b8d2e6ce0b9fee2f5416e4536b280fdc40860c17e9da6bb361128ee53754bf68f54c2878a4ac4d349703066a6fb96220e15285fcb41b398b4567c0c167762eb6ceb4f0537c7fb7103487c78a98093209771fc4f4a9821cc8eb11f15a0e35c54f6d5d31d8e8646c229");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "0ff815e8e1ccf66c1a09c3b1875953c44571542729e5d8395e4110f2505556d3c9d0711d5a111542fd39aa99f8854e5505db1db8afa2b084ffd7e3b3585ddc67a0954924a02fe83dae404d8c8994e066844525cee78c40e992d8b8b3e31d7befda6f22647bcf7746744534f3002f0ee00bd8f2d2974c8f515e6b1459b93d3b54");
        bigint_t S("19c45bc7c6b4445a2e4eab511de011820ae0a0483e957b922fc303b2b3b94b31205c0eb821a8f33b3ee53d16116c6a7be99595b2c16cbb1162f147e7530e48c60861411b595d4e7d1a1e852389eb98ca6856e99cbf121377db96961b4a55478f54e1cc0ecca349d298dd55b7dbfd9a1e97a1e248ed9c3e4879189925b1846010e5"
                   "67269376a5321476cdb6fa6ba3d358ccf737964457765419245e53bfd006753eb64054e9f83eef419a7cb99ed4bebf31823e3f8b6be81833ae164774b044788b4fdd6170ac499ba60b87e8b1515d4d1fa751fb4f74ea674290a1a8f24c0d769d5d8aa60ed2e4f79fa7e2aedb42e5f18811b90d96041dcaf41e25b7b2e3e998");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "646919bee4a47d978d4be19f1806de5ef849a98433d68a877183908e523d848e054d1bb217da6f0188afb03b243f170310e61c43a472e9cd78e20e3ec26e7628dfc79a702f9ff4f4266cb771a069bda575dec1b04ea2cec0b7def7ed75134962195ffebed5fcf3ba8f095d0b348db78a4fb9ff92da6d21a953feb4631337e484");
        bigint_t S("084fcf4ce66c15436dbaf057e9de7dbe99a20fdf1ac491a920580a6e3b304629211577daa07a38888eb65fe9507e91990783e05f0d9820afc37623ecf42241c66a8bc7ad0e0f1f43b321fecf4a9d76106d6040588d13b58aef68037f73500a8d34a97117ea41c0d2ab51425eb62d863a82ed56e7082589cfa3d55facab67590ed6"
                   "03fa8fe89553af279f36bf32d7e7f81f24f17cf066ba66d090130a7ccdc44848a23b691837f51f589ce3076bbd53088e80958681d4868b0f957edaf95e175dae09cecbbdd39e01cae14395690a61af5d7c4ae882092e840204823a0710e80f81c4011f10a6044c79a07374b75f13843ded1f7ee3fd9abb85914f1a25b3dad5");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "32d9829c40eec978d8db1b744f7784b937d955ab7817d3c8a3532cd282f9af30f4423b92f7fdf8d4078e4625a737f3683d96b1da226fc7f5faac069541a2ff5a2796734d9e800bc6b10e498d17ecbf3f08437ff34f80d21ea1562db42d06609074de11165bff69177e640df4c820500741e7a73e26d05d093f5987776dac3b9c");
        bigint_t S("92f7509366d0d79e4a350ceac7597481b3c4044a7658f2aa65165fdcfa0ba95c9083156c9b9cbc220fbaa694bc5ddd5405976be45845fe9a2facd2bc1f1a37913541a918ff75a8ef26506654ebfe0501ad46bd57e386926136c38be16d004b1c8c3da23c58632ccba96dd1857405e4712488c2c84db20f82711212222def48ea20"
                   "efe09e5b476e28a60170cfdb175c7463443f8291e894eebe95df13b0f5ed0d504958813ca990d2e32e969c58543f77ef52dc6f8ea539107a4dee46735cc49903664ca10a902634733f275a0a1f145a9bb912ffb4af550ec08ada902f152164c92e7117f2f2649549f59e1ef5398b515403e3f3748eae0b98a0782e30a9c69a");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "049b80ff6c1c1445d472a70b470c404ca0ea2e2a4c52ef3e51c5502c8f31d67a46685da2d9d1ff65d25d376325bd46e9b9d3d60a2466def1b719c575ba9ecaeef827a3477f1d6455a115a3673e00da1f46a10c807945ac4771aeba7fe3502f5022c09b18e4690dbd49813341193bc2964d3e18b4d25721ea896a777192afcd47");
        bigint_t S("844074eeba279c98b763fbff66f7324bdce04b27acca85e4d5abe514605185bad21207b62a427fce1d9ac2b2f0d0260d45160a2a822abac1da0a966836521ae6564f38e996538b4af300de69c4aa0a5721e8c89f20320e4e09ad1f99b6a0ba3fa2b41330b84a890c30272f6312b2cbad7d38336faf92a8c4234a0814ef662f62ca"
                   "3ebd492f805457613e04e241ad9516c1641296334228cccf24562d69ab45352d30c68dfc7cb7e1159bcb5c0becbdb3ffcecb446d0ea2507ebebb6d9c8d9388d6afbb67c5f35df343ad4c23e0b65be11e0f8e117fa26a6abdcfe215f8b0466d31d6168611b71dbcb999233b50dee5414e54d444c3d3c423dc8c58813a571058");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d0431397e560edbba3e0792f37915fd296e3602cb34a2ef6f5e481dce85c6781330edbc4ca6c9a4264a0321b30735cfac36d8b4f553dc5df2e5cb27f5beef31ca5e9b4ffc478854fcfa265d8f727200a75aadc45c115ee54053af237d1b61a25101bc6d13b4e55edcc2d3becf675540d93e379955e9a505b62c5688ab27d85a3");
        bigint_t S("e08879c12338d9eaa5e8648351ba516941fdb52d6578a0ba0cd88e1637d041ece35bb3a4d6c5e899ca3ecc1b74a92354a70940f0f83ffe25a9afca901feb6ed4dcce64faa100ddf043e514426f4f1e6aa9423247bf1e7ce96158ae8f1eb1961ffae1d232c13504e99e09f0da7513a2620e88d05acdfbb9644748df12475d01af7a"
                   "17f35a6971d4e401f62a903018f29c1b76fb8bf7166e19d774d318c67c834c4e8b0cc0b4d36a54586169f43ddb1f269e62be363222bc1a1d1fe21b488afd539b5f8e87ded67979e6b1f18f8e542587aeead07c91b435c933e5acd33ecbc7da8b2094c50f512fa9ade68c3c40ec9649b0a92c720760e5afb7b6fd94d6c53ace");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "293bf13d84db45e8cc67b4a0d6f259213e365323adcd232f58a555b0c9ae634de2d5d6761f36ae3d1b8d046856eeeec435b7a3bc4ac2df29d6df896c8c8194bb98b723b8e05ef0488a96eea2be722da6adf4fe71d238592e77ef7b2e7c0fd33c54c91f2a50bcd3b02068ba7797e5df70aed4ae6acc9757e444bd731f1fc0ea78");
        bigint_t S("c10f115b8c930964d893552075c50f9c606800f446d4d80226ffb15025e7a0ed9cd329ae7346546647538a1421acd82c0afb345cc59099f30b94f812d5f5d45bc870d6a7a50533dbc97e4488fed26f3061a654559b18a7bafb3b90170020c0967fa5a11d1359b561c0edd562fbeddce8f697abd19cd3f58b9b0a3f5fc131bb523a"
                   "c2149ce95f3c5ffc2cda3c988069d2675d1ffec29973d5bd7b61c64dcc17367e50afd555ea78738b75491eec3e1f0b78fa323e06bb9518742602f341856d3575f18ce35f4e4298e124e4537a3de4203704be90dcfb841225cd5be4ac694c144e238345c30a3ca23accb7114ffc0bb89a146bf192a6d78b3a324c8a418b7af9");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "91a914707b2c0e33585e56042b50889b1f8ad9e8b2a15b49190c7d00e033d1e4d2c91356185c6677a52ae099d13749f3439c20981522dfaa330f81ca044040d44d78a3c952f8b80d3e509ed3c47447d507127c48b723220f7dc1bb2f9a383083cb5fcdb16f298d5ceb77989b4e300aaa3194f6e0e2cdabe1c1c73515e9422759");
        bigint_t S("77d1c6cf1286cf21350e3d331e8fa58addcf796dc680af1a0d4260a3b4df256bf04870a852129fb4a950c732da2f7bb06b414d52c7c442b3af8eb0a76c3b701efa66d57b2f047de994ee209dffcc6291587f0de4f695da12300d5f74d0dae002114e0be8babe21dbb031ea3c2aabe922fc822bcc973ea1c4b0dc0e4f85f83db4d0"
                   "2cddd238e68785e8a31db70b83e1adfe291eb63d934ec9c7b93d0b369a59215d2421aab831bfd3b9a6b1c518853492d87c577290c446b5a942bd4d2bc76d7e8450eecb0debb5073838a0fd521854cc1127d5ea514938b499d2289c1e891e1cf0bb466e11e310324a566c51b75072323403dfc5eb5bfd0db538fc450ecbc925");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "76ee8ec93353c62749c49c66846ce128cf177cc3f9e85494b94e7196f0345e322f78d159799f8dd14ac788ccd7dc49bf980f957104844a539c4b8b0f2a921d634b1911302be6a4ce903963a31ac293f4c912cffabf50f339472d390d1719a286d2b89875d18fd1c723047612d285856e68077486b2b217efc4dac3b83d32b442");
        bigint_t S("2689f68d42cbde98b3627fe809380598fb1aea001366c9adf82e7349515ae1889a85d16e2ddfcc51d83699fa2b6da580c1c5f32a53208dc0835ae745dd8add5238a4842413a107bd4a39bfaf546b3c994cf50e29add6db3b5138c34fc7894b910b00a5d5ec5c87db8af03e22997d515d1430ce3b47543a24ad957e192c996bb0bd"
                   "388fd65823f0cbf40d46776ed647989e7a5d6a6ce4d433054d3c6873aa04a4bd2fc20a92d68b90e412e44c44914a76fd12664034c207622107c51e92987e51ffd381605b246f016fb78452d7bdd73891172d3f88ef2e2da253a5800b469091d9a43a947e85c8da2855630630efa63b418f5345623e3c258f2e93db3d094a52");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d9eb32d99c9e6a8001a30107c29ba5bb0bd6f31189192f8cc59e39e1b05dbc5a7e8aa978b85fe4dc8d1b3d676a4ba1a1ca4891574e01d48910fa69833595c049c1912a38afd08deb425d5c5c96adc64b7a252ee482dddc9c57a4f25c5244a1e149079c63a979cd74c570482c6e4a7d83c476b1fe4b36af8e42a2bab530f1c7ea");
        bigint_t S("d2b2e8701e9268f050fedd0040ed468d3e5a5a5e20de157608aa70dc7059af94a6dba349545d3b9c65e64e74c555cbc0ec462d496cadf45c4ed240a2a1d6cd021837ee857ceb0e5aa0c52bbd9cd70a495a597a45e5630cae62b6957ee7f95f772fb8821fe781f7a3ec7a41f7db5caaecdad9af8ab3c9b058291d6b9c98d50b2c47"
                   "7afe7baf769c424f60e39a62c4d63a8dd99b7017ae267191561c5e2955764c8932ed9b4b75924d6ea6f8b62272eaefbe0bbccb59a43511abe49e2ab486b7f6171f0db8a8770cfda0d2339be9e78cb741c2dd0de3d6c272c1a61fd5de72fcd8618ed984d891ee5d3002ebcef8af237af441484aa229cfea3ae2d53f22277ce4");
        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA__1_RSA_PKCS_3072)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(3072 / 8);

        rsassa_pkcs1<sha1>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 3072);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha1>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 3072);
        EXPECT_EQ(f, true);
    };

    bigint_t n("c755df3cd383466596520290b6f7afbe8b949eb5f9e449ef4e34e397b4d0a93257ad93a83b2d177ea37eeeab1ae175ccd81156ec1381072b30473f613f1b918d1b39653ba6cdd832e4429acba2fa05e44cb296981ff7161f17a5535b2adfe0fa8a56b092f35dce1fbd4365e13970befed80b8d9f413297db07bcf491e5fe236dae0172"
               "e05147f7a85a3ec11a074a91aabda90e94949eecea765444ae30ef629ac682efcaa1272ee17a2116019910323f00c95842cabb019cb0948bbb362ea57efa99a78b9785658edcda6c29884a10f3cf289197d022aceb2cdbe681ff5c436ddea48a380b6b79fe2bb88f43c1922b3cc13df4baf7e6761f29d35b47c1adaea89594c4c7fde4"
               "eba855e8be1fee172af4b35cb732e39af61e582ddd60d93e06c74b0d560d015a02e5c4d4c33cd68b50cf69089fec3e19ebcdb45828e96f5d176584fd3827adf87c5b9174583a2373243c24d99ba202e0d4849e7ba073a6081330eb5b50254113fe3e4207a355c371f24607276eb7a884f2ccdfa8313d293d5e1d");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("222a4af8a935151e08d1761c992ba34ce8ae18b4ce87ad0f6deb5d3ded911d0ae2a1becee513a1b5042f57976ea449954a4c508666826538e70db324871541d17a62d041d4e16fa6ab5e6a1b308c2371e19e7376cff5c0ee23d6a38e9aeee3e7f5498dfaa5e94450c6d6f43191ef8be0f0a52c49293ad371c865ffd238e621eae4d9dd"
               "376adf07a8ec8cd87a8e58ded631ab35f34bf4e05d005a89aa047ba73e297b9c3f71f71e0f29e85d55f946e021d1cff0c783e961099aef5ef2bfc2e77cea58902d910279228addd532dc417e7c64f394419a3d70dae19bae780bf932c502ed817dd7bf3c9dae31c9f4156f8029643a20054393c849b32dac3931695ceeb700c006caa8"
               "caf201cfeaeeafb0f4bac89416c50f14c93aac5e3efcdc9409e491450bc3ffbaae46b5647b7a9718ef0b32d52403e26679515ae70a5a9ac35851344602d8d424b6c556b64eddb9111df66e6d8c82c4b9734eb986403957ffe415af0d13d3aea4734ec77b03e359bca2cfebc3e6cc96e46b3cb80bbc04205af3e1");

    {
        bigint_t Msg(
            "05287f294cee9accc2b125f23bc3b723308aaf951283254cde872185f24d0a551f31618eff33afe28843bb4be2bc01aa762e60ae486013f78695e27b610f99291bd2bacd61aa6f34dc6fc2900388101d33aebb0501f22082cf02c9429da8e30f2293f2af1b5ac309e4bea28a4530cec78e932a64e6423534e0553ca14f20cf4f");
        bigint_t S("0caec9762370ec3ffebdcfa2e7d0d44c0b5ab46845214324910270bac7620982ca236b54ef5179fa6854fa7698515b7c90e3b44f72971be690b876b4636a6a459cd3aa8c15525fac4c2f3a73341d4ace08381b4a9b08f5f67553e23c0893ef556072ae7eae1ff15a2b381861a8f40c281ffb884fd27d78e5b01888958f21972d38"
                   "fb542c5c0e9764fbb8ae66b88d8eee091b19e723fb9f302b71cf14658ed24a981c53dff9976fdf7663c34d7d579519d97ea2e5c719b79a9336597d922986013404e021be31ae6bd3a295fdd95efdaae201bba0645b86a678b282e1c785fb36f920d81456ab0ffa34a0247065694c43ecd5d90eba1aab268b268ff5d8789ce894a9"
                   "93b606627577534d39c7de7790a1997f749057a3a5bdc69a34575c34cfe6d1ae2ae52d33b1cefa635b694633a2082ca4a2b8617c4313cdcef0b821dde64303767f66393f50bb9af756346a30a44137790a1164863c4960ef61d3249c4585b7f1cda08e8ff9675cd848df40896f413e6b2d4c6303927ec495e9665b3479e0");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "8cd701d80169be848d529297ebba170fac3994e8713dd19a5dac2ae4cacae1c83b5cb9d7920ec0ae84126cc1490ae9579828b2d6d2935f417e0dbdfff5d424de5ec50557ddc7c3140867c4af9bc0c7bd6c9e780ba1e341272029642247a84795de5a0ee2495e6fbc029bc2ea47a5584710e40e0e44f322542c4645d62810f1f5");
        bigint_t S("2ba6809b0be2549b8e8ff2da7dbe3912cf5671498b41b49e372bf58fa0d814b0de5f98b1dce703149a6dcb7f409006f9f4c8da6748e50a5dd8cff8db0c54c5cebef4de1b823bcabca3ec3ae78b2050a60b0f813b1a8e7b67bc09fa1cd98dd8436110e15193f17436f2c0a1f81fe94ae2084a38264eae7eb991a4470e86f8fc84e2"
                   "01e0eff0c308404098f37615992f5116f9d7d92c758520c9c8723128a22464bb9b964b3ee93a80a745bce7abf5e97db3f6fbe0cdf57ef9cfef97c1b4b1a432b376c27e3ddfad4b2aa9d2b27d0c1ef23cc27259fbb272e7131d13e0494f6709eb509dba04a8091ba25735844c54b168c65c55e0befa35d6be44ca192855bdfb5a07"
                   "d7cceca5d85ebfdb38a2ab7c9938d4cbb7d74bb2c29f1d9cece5cfc7a63ae560b7058f3c737234e4538ce057dd13278f1fd1ee27efd9a6c74eba4039dab491ffb6f72548b18d606abcd205f35a457bb2c8c388204d95bbf9d669c1ecd50665dae997e431bef462b9c64fae5fee525b5b150d4dba9ea7cf779b4cc09cc3b7");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "b055da1d4e7ba932d0fb59cf30745fa82554c4b0a548cb30a8f7d41630ef809086df16dac60825559dee6bd85253f64a0365897132374105806e7809b73dea5b0e931689a992e8e6604494ddc189c5865854f7772e1721dd8f7c98cb105630d1d1d7a4d10003380e9e649c8686001482ab88d66ba1d2b3df5f89fa0443249a1c");
        bigint_t S("0607adbd2f690a83532d4d63fa4886ce1eb769842a8f29b15c37db6275c8eb7ae9e3fd335718e6bcc1d9cd5b6ff5becfd3f99b4da003e9dc5b1aa4e892910cc3f21a9333f28208a9c774e812774b6b36cb5963728f142ff37cea9a59f75da871a6629e8a1846071bb010cd021225e4186ccefe5b5dff9182143b351de0a5c2cd0f"
                   "39de75a5df6249488202eef83c4c5a68a092c5f78e67b5dfa67c1268c1e12c6f1860ca54450e028c41f47ab3abed6b36c6895bdc5580cb923d077eb8fee5cae2da0d0942256777bb43a57dcf75c7366a7f7be6adeb2eee9cfa8f7af134f467a155444885d9afdf97e64477ad38c41aa099a993ddf61adc8dca65b44464c16804cd"
                   "a566bb5b3ead7dd65046cbd686ad7dd0a3a60c31f4c2c26aa63cb52eee17b67b423da75b78949515620a534efaac691e662a3d38540bb9351d1f9f98fa160566110df0d6f85ad9b593271bbdab7a2137e7cd67955ee4529f239d2ea544877352d60783a6316bfa1ad1ce04202d6cf6c387f0d365385923cafd32c29aef15");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "0c75e97351b4445af5fe02896dc5609bfb15c79f7f1946bd90e1401dcb6262046bbfd8401bc7c1c708bee09b396094f023230a9178bdb18041e6eedc2c5f12082733890a2d3d533160eab9fb59403835bc1fc2640fc72664cc9bcc4fb425a5a167596299321ee4e2280af472746d35b3ed9256e0eb418cb25988403d0cbc0514");
        bigint_t S("4b6c7279346e264a3a2a2e0f175a4e8e47154cc44ddf07133bd193c336cae59ad91fca75a19ad27ff5b7ab2c7a91160d5f0a67be95eae7bd9b333658953277ad7ca90195848562b7a78646827f0e572ccb4f16c86f859255f7a42917a4d93e2f75ff4ec2df8877429238ded1afb8a4e2cad8c86eaeffeaaa13f438d8502eb2fe45"
                   "35c828d46d852ccbfa83581826d6cefb2412fecba925cc2dd34af55ab79d0740758cb08662c4d80e6f0da3e430aae5bba2bfdfd8c06763b40cf8b4b8c743a90304897a758f36edcfa2da8060a08fea4192aa55dd7640b2c3552142489e3787faca63d586e55a001ea56fd3a4ecdbde46f2bdb56210f3d93eba38e55b8bddf1e30c"
                   "df3f18a6ca2ab081bd45b4582e8c30bd2e129100d8def76f6bc572d4d71623cb3a20d760ec2fabb76134c32a2a3c0ab86f8c22e796532774f911f70f554c4eb62d10f3cd3740b3902143e12e430e6a89a30c7f55724f1596c47cab69a66e1175a541418ce5ab4e7808608da589db31ac3e341d1f2ef893c62f7f02152a36");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "71e0a316b65c8994f0d5f02ff0a1619da3d03e079f6cf86bfcff833f655607576e85d908282ba98ef4f7fdab946bde9188e0d00297cb08b6ac3edafde3b271a5e403c21e70bcc2dc3995d7fdf73a8d2cb84e8bc577b0c539f1fdf3b5b45641fe796348a0b3973d4768b6686d4095d835dc22569768a397be7443b6aeffcae12a");
        bigint_t S("1685a8503142af078d38ee62c6b9b2f232a4b01801cae2f7bdf589a220f390276665c58e54ed3a19b662bd6bafb2f24c9798efb56a7534e66a0079b1d95291a8bc922827659367b6baa7b57aa17dbf4daa9a3b2b2f61fc23aebd40765d58000c019f27b25fbd645b020d4bed5f068f47d5d37f911575b6480ca7038b892356e589"
                   "3ff92f4664c8908aa5ad1439e968b561edbb4e236d338e7354ec909d740cf1625f382fdb91d079a5ebb1e290945d380c8b19da23d39695f817aac5b86a67c54b0391936e6a9cc9920a289f262468420d20ccd002ceebe262c3d947d7b3e35d75573eab92f3ecb4f29d9ca4a4d40366de8fe191de77209372bd3de2540fc88d68d5"
                   "dabc22ac02891e1737e32a6352a2590d869e9c26d3f84112c674d271071f60df90e7578f1d5dc9628c8a172e07f4c59d4fc37fce1577510114457ca2fec73d9d3aa23dfabaaef60e0f6da42a4c862685296c31a766f6c49af7beba508bf8d1c640b12e70ab63d1ea0c5add9a1a96bf83b1873f3d4a97961d224318a5e0ff");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "ed16a259d569c3e791424d47bd20d879e5a685f23f89499d99ad8820908be66003d65a47c75e84c59c344014600f7b4183ee45e8bbff283ff9c0b8559b646c9ce378aca6a8c62b2c4b7a7754ed83cf33df3c34915a4cc2ed71cb41c143a6f8087544c1fce0696324865b1b8ecb447c5ae2d87a788537c1b1e9154becf11831aa");
        bigint_t S("5af92406a5a36af9fc49084cd0a94486c4ad8e5c011be3f77db454d612bd4e71325ac8387651c8528607a5f261000aa3cb720b61196e3177ca908d76f84d18fba7355245557bdc22cf54562aa29a5c1400aab781fb3577a19e4193fc139e8676e6b9ff92065722a03bddee85d8c87084a5416b555369fe323ebea53b392abc4322"
                   "3caaa746f2bbd7fd99ed3b4c9b5bfcf5a892e62ee3b8bd21562c3ac0b47c0c0b3783d52c56dbdc96af40b02310c22ece6464a8dad3a0355d34d31fd6753f2dbaf4a95703e302618bd5c90ea46edcf2ae6751d05ac76d4e4dc1ec222bbd969c6f062758638747ba7ff0fa4561a913e3a6f1a9713f65ddeb4fe317e2de2edc2edbd2"
                   "be0bdf6ea8f3da3c264f82511ad62dece581a4e42c7ebd2613dbf5a80ecc668bb8b5f50a276d56a3e470e8d2dfe95d3ec0056c19d3ee27b55891bf57580e2947707e3188b53ebc743b49483dd37e3dc2582ac94768a1697dd249157ee1a6a0e44431267600e64689063b3e8a4fc5d16673c835919b4e2832132f2e05d474");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "8670556cc21c6d08816d1294afae31d54de9795318ff190b65f7cb64509711641531be24bec0b981ff34fe155e09e566961ea75c3ec51ef9a616cc44028a1f86cac5f3b960b2593ef4d597cea5dd70b3e879a5382e6dbb5ec1e5ffd0ef09c8196bd1fc15f01bd5a527fbe5a7cb5258260ba27acd302079a27f0b6bf1613d9285");
        bigint_t S("3e5c918b7abc858d646e783f4b3f5b298293164499dd72d116d4dddad3a50ce684b2b566fcd9aa923e9ee80b6288ba4506239aafc1d0d56aea4011df8dead986a13202f7635a48b7ac4bf7770ab842c04e58c8d5655cbb7864578eb76d4f2ac4b371474688a7db50846e669ebdee8f8609f91c26773a7de9a31b096735c50955a1"
                   "c5926eff5e03c764898e7af476524c622d3afcbbbc1c837c33808258c757fd398f6b0a4e656cad30dc951c426cafbbe11cacea2ec2064b825abfcede83f5f57f42d48d4648e39500a74d8e4cfd6006c4653bdc92312b8b2d656d5821e4f403327235712c936777d09c5e1332f6142711e5af2771b0ba6439b9f495e1fc9b6788e1"
                   "c28216c20e01e7af15da0d13c7a8bdb278b1d6da5eaa20db00e2c35f32126398cc8b8f266a2492561567a37ccd8e2e02830d04ab25221f3a38e22beff11a049852b469cd2686f9826680ca745ba1dd46811d97b249246a35d9afcc0c9e93ece9f7bbbb260c1a8814242ffee3abb963fcbf5dcebdbbc7de8294b7e9306ec5");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "ad847bce1d8c9b0bce7ff0c0d92005d7ced087de56a404b717bbf391d8cff517e6095ae6b61ae745cb87d610da441ff62d85f6e545d195c1c3583b5c898675a31cd89c154cef9f7a18e302d1ccd9278a6cd8cdf555d8d6fb221c4d0ccdc2a7581699494229324b117b3a7ce9f506a6fb547013f8d0e24fcbfbb03087edab149e");
        bigint_t S("3ce924a194236d9032e3a8db36da07202c378352a24ea0bf4ae708368304b0c1595b6cc74c2cb35f0697e4b58da6b0e98ea7bda4e584ebcde10665caf0eb47f6547cbe6c4ac26eb14fabaccb686114e8f2a3dd7650a3b28fe3996618d2ed8bcbb7df1e5d2591242ccde7e5010b99ff78ae25cf8c2741d13a558c313d42fd3fcbdf"
                   "0f5f60a044afcbe5102a062df5a52a0cf6cbcba6965c186af711d2b782c456004b9301b931a8933457aeb89968be7746f4f5fc6b32c832be6f007e00a36ffa6a0e210068be478e2b7a156764bb4c3f4664a4139c6e6ec9e22778f28a331ccb001fd68fc5768fa332ac70551432bcbb88b8ba4741b50256146c83e6fb8f323b7a86"
                   "2ec4e1bd4e11d19f1ad697b69caba10195587afae5e095e20105a5ca9a8eea10653cc19474778599b8755739dc2bb9ffbc0f3054c26591c88312d6168cc9ccae4a7e1c4ef5eda71525ac9189943c7c289c698e7f167fc1f97d9c4b66b52675cbbcb55184c05ab17d38cc5d63eca4399eb6c880a2c51e4ec4670aead99cdd");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "438d4da5823088c9d634bcacf3cc60b8295e571493e1c426928a1f5679a789158acb5c1e222b07fc304819cdebce9b45c5bca3f69ac626ce2ef80ee1d69b8c68755cb00aaaab375b4862a4a307a792e07913480e3f148ea57c8fbf648aca204dec8a70f18478c0ed06c985b5ad1eec902da96de4e892a3b464995b67fdfcebcb");
        bigint_t S("24baa7f4f5a3e7e541c2bfa41924a48e22355618fae7655b1978a911545df542fdebd076916aee40f3ea393664b45217a568e21033e464cb4abb0a52db709c1e605dc5831a7fc45231c074f0f5742947f8bc9121d0d88892d02f55e86cce6c5275f42714766c0baa736d498d028123d0337de4bd27d1f8187658ab89d28d9afaa4"
                   "c47b64eb79dcef8eb1eb50f357be140a091c448e4153abe5e279fb5afec80244f3f8736aaacd76f41403b8c6a8915be94442bef7b5e2dc9fc77191a0907a02d3270d0ec13c2c3e8b4aac2f050fafadf26844331817e08f18616491e9e96fb4efb6b817ef5e8332053f49c62419ed6fd0f6818904a9388f1173eda8ee48aaf349c3"
                   "5a98c1d0f55d404e641343fb41ee4b18d77a83dc104eb6e75bcfc03f6eaa194c4b95b4a51a6357fe56b36e3d2b1635a33ca1bcc7f2673dfc60d5ebe25c7f147f0fed67a9639247841db91f20d8f33c08e7e6f6771661a33fcf70593e52914b0c468152d35c6c18285dfcc7811291ea6e0a2989ea68803616f6a2043f36f1");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a4db6371c8ea007f2f4771cd9d373ed6741d88467c35eb55e385a95483110485644feb3082dad9f3d06afb5a7c0491b0756e96142be6a3cf961311e4793c5ee9e8b8d11608a6619c16ded437aa2724073daa269ce9a66d5071666dd98170dc1e37005e4ae1b4601f0989448eaf06bf84d5b7800641dd13807e64045a241547cd");
        bigint_t S("97768ccffe282bdd63b10ce17b0f78eefb368929fa827e4cbbabe914ed71819630cc16d1261a5486f73f65935e1dc3254f7e03f93890849305c5796105bdc764cc31dd9389ffc328d1cbb4f4154c48ada7525c2d6b9807980241714a0d852aefdc40fe10ba46f89e5b5b4e1deb23ca18cdf732d382cdcfd936bfff05385bab3dee"
                   "8f6f904915ac418baa4eff5a2626547f5dba29c5209b0abfa378992e4353ff42c0d68f892502a0606158c6f41d565cf8541f19dc75f9c23c57b470f1c05d3ec801b6b524cb9451a678f04b7452026534344b502d8789cf6ba5540f1ec6729ea110eb5cf2d84d5c7979c71585eb8d3c208a33d9ec7589398013678c699019bd01bd"
                   "3cd5e1dde94c8be4724f23ba0f6c770b56c9a10af046e661209379cbe0cefc4aa6088152f8575ce7fbd84d941e2b1e8d6ee21158c43b411347a46fe9b870b6eac10e16dca2519aaa6d77e861e25c26f071a8312f5eafe6df55666312aaa5f65e5c81bb712d024a0372a74c9ee8f5f4614f1bf85782d6cfe4f810dc913d2c");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA224_RSA_PKCS_3072)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(3072 / 8);

        rsassa_pkcs1<sha224>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 3072);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha224>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 3072);
        EXPECT_EQ(f, true);
    };

    bigint_t n("c755df3cd383466596520290b6f7afbe8b949eb5f9e449ef4e34e397b4d0a93257ad93a83b2d177ea37eeeab1ae175ccd81156ec1381072b30473f613f1b918d1b39653ba6cdd832e4429acba2fa05e44cb296981ff7161f17a5535b2adfe0fa8a56b092f35dce1fbd4365e13970befed80b8d9f413297db07bcf491e5fe236dae0172"
               "e05147f7a85a3ec11a074a91aabda90e94949eecea765444ae30ef629ac682efcaa1272ee17a2116019910323f00c95842cabb019cb0948bbb362ea57efa99a78b9785658edcda6c29884a10f3cf289197d022aceb2cdbe681ff5c436ddea48a380b6b79fe2bb88f43c1922b3cc13df4baf7e6761f29d35b47c1adaea89594c4c7fde4"
               "eba855e8be1fee172af4b35cb732e39af61e582ddd60d93e06c74b0d560d015a02e5c4d4c33cd68b50cf69089fec3e19ebcdb45828e96f5d176584fd3827adf87c5b9174583a2373243c24d99ba202e0d4849e7ba073a6081330eb5b50254113fe3e4207a355c371f24607276eb7a884f2ccdfa8313d293d5e1d");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("222a4af8a935151e08d1761c992ba34ce8ae18b4ce87ad0f6deb5d3ded911d0ae2a1becee513a1b5042f57976ea449954a4c508666826538e70db324871541d17a62d041d4e16fa6ab5e6a1b308c2371e19e7376cff5c0ee23d6a38e9aeee3e7f5498dfaa5e94450c6d6f43191ef8be0f0a52c49293ad371c865ffd238e621eae4d9dd"
               "376adf07a8ec8cd87a8e58ded631ab35f34bf4e05d005a89aa047ba73e297b9c3f71f71e0f29e85d55f946e021d1cff0c783e961099aef5ef2bfc2e77cea58902d910279228addd532dc417e7c64f394419a3d70dae19bae780bf932c502ed817dd7bf3c9dae31c9f4156f8029643a20054393c849b32dac3931695ceeb700c006caa8"
               "caf201cfeaeeafb0f4bac89416c50f14c93aac5e3efcdc9409e491450bc3ffbaae46b5647b7a9718ef0b32d52403e26679515ae70a5a9ac35851344602d8d424b6c556b64eddb9111df66e6d8c82c4b9734eb986403957ffe415af0d13d3aea4734ec77b03e359bca2cfebc3e6cc96e46b3cb80bbc04205af3e1");

    {
        bigint_t Msg(
            "8cca7fc0ca7689c0ba19855c7676fb8d65682c255e77510510a92491a19aa16195cf0e39f248393ca154015c18aa171536b4065b6e2e71d128f09c3b64082f36a10c3627fdaf6b719d91b8eb4f5cf8916e0cfe79078ed809398d5468e289c4327829ff7034d2a8c77678b3ef65b42467d5171a676fbf0d170b2754fe6143eabd");
        bigint_t S("5fb3a33769af2790501d8bf8149d720ae2355e2c19b9250ef3efac5398ab644c0feaf8a51d09fe25ad9e871f36927e129ac23ff70de519937fb2da6371dd617bb9e6f5c0ae8e3399d8ce503515301570950e31849d67985da08c6d088bd7d13074c67f2163e457b66f5f7b5111e848204c78bd3bd3fc04d5462b0efef47915f862"
                   "c26ca28fa30bc9e791f41630c6f8f484f27acefb14c894df87c52284beff19374a40fbfec8f5979de9ab89f9807840e6f055f7ba495c38bb074cec97664121b9ce8bb4bf8fb3714a728a1b2297625ba2d5afcc79edac68362ef12e86e41be9332171aac36687422b6c5466dfcc599d5d3712608dc8abc41d4660541dfcb66da3ca"
                   "1887e956775f6af163a0eb2d0e51fa85003e08af58418c8b0f6ea0df765f070c800cf5403a54b74ebe2ef6f101b632680868fd59573801012f79119079a2dc11111f32496f3e28f70f1c4b9dadf0d4dcb1e0d3f1455c8d8a6c24880e55836c38aa89d2f6c225c09a8669078d029a06b82e32425f3299dd8d5ca354327c01");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "de5029fd9216787444f727ab156674394d5a0777f09624e5922e25ca6b47281bae1e059e174c834fc348cb03452d1c4bf6bdc19b8ca640acef05d51e48291e625a9918d715584a3368674025588c3b394f5227fbb651cd5fc5af4a609ae7c0e889c94bf0b4c337e8db5e7059750b39c9ff2021df941e33c7be356ed88c0a7574");
        bigint_t S("36da648a4a59e33b40a3ca62c80fd0dc061d47749421c82d52413f117e8e690579cbe583fc7d893fa338aed419d567628ed27262b54caaaf718c6d83134d428ef8051f8d65c4434777d6363c627753803bcab14e263318b8d72ee2bdbeeb6681e274735aabc0648632b81f05530b799e219f8612ca35ea11990dabe98397694fe5"
                   "a407316280beb8263a29dd3ddc81355e4d7138b826b8154876265e09affd3c1533d6e33a77718206241c55dbd5c8fa4654cb23bc338128f6e575b72b2c381c36387c21cc04fd9416582a0db994298aed27d213cfdea3f0fa13c75da4f51316806d98c66a8e5c0f6f0c7f41076fdc9790d87929de2bcd2d179805fd48d42ead72ae"
                   "76c534fcd443e4afe4185d88f1712114f0ca46a6b227fb6dad9d7a8c410515545acec91ecec726df1d3297da780c5b80cb3692390e78ed83af206b48cd452661e9f278a656e8b09e585d87e4187330e83d0e3c30c2468aacf8793c78c57f2885d506033fbe93ad856a4db1ed257cd1614b6ff2e5b11aeb56d42fddd98825");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "ee3e12fa06c74248c2e344877c92e5db1738c8efe5f366e6c82c5495986cc5efc5714fc03e82f29f91ff561d533c5443c28fb83ca9a8454e8421fa0fe55694d51677546e00dc02114b983295c750c6e10b4eb4a45cd10a46d0a72f1eb2fe6be52978a96a157d7c883ad56155a076449d399a458fc515bbb5f3e6792ac90c5107");
        bigint_t S("0c57bb639a1bcecc84594878c9df7dba1a0cfe13ea6a6f0b48f0a817dd13a47e977a1d21503a469dd905bc35d0022232ef23259542ac860ce30cc5835c196b742911c3d84e90101b9b200b73da4f995da01b7f2d4fc572075859af2c86d0000f376935a0bf183668236bc5d3d2bef74d8ad9a935ab7171929145d1093fbbc03e6b"
                   "951a37640c4c71f1d2eab81a2c1c6c8bfff0864c1a8d7df64b776da1dee48b8ec843a732e4e5e55481a2a4c1db66188ae585af7dc9b55e1ad834c8d57ecdc5080204319618885792810e8bd79da22e1388cf49bc84cd69cbfb4aedeb9a615644da158c3486aa4a4fd98284096a1e48008dd6aebd5392252615f4b663df30dba393"
                   "e17d37ae84b9e1f937e60ee8c5e6ca8108c2112a7590f2d32b366523b18b0dc23dcf1d0754b5f569b2c85c49afcc741fef0ece5ecf27035d1b0e32d05f74dba53dd39346e45833135ab7627935164db6f5329094cd319bf27d6e8b84ece2eb7ce96da47ed75ba20a8e5f717086bbc29f3fc41c5e2117702b5948076aeadb");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "680060fa6a12201453f4e4ce55a106293b4557c7f2abbefd49f9222bf59b8a48d87d83c1837dc384f9200b57b583526fef951f3e1c84378cb47578c9377805a272bba3d2fffe633b0b4682355bd68d26915bc853ac67d1f5cf208190433a1380be2288d4e67c496011d00f0e4f800075aafe1badac3bd5612263042520c7fd0a");
        bigint_t S("bee1202471df2aa417750e9f0111ad6b17e6069dcbe6155aa676812062bd0dc62671302059eaf2b9f7fb1889e750ff2a8119f0a4c72ecc1158f2ffca293cf7fccd4a4a49d8825b75dd3bdec89360e3a4dd01557bc3a4deda139ceb602820ef8a3db0e2b7cbfd2653052aee96fc0028769d6011fb349ac278788c513eb135d51038"
                   "4477dad8ffb2fc547a002c0cdb6a5625635e4424e995288dad1e5f4e9aa4a3fd061821ea7c60f44110f72c92ee753b4e8453009908346db1a3b9dfa16081e57dd1b281a7b7b384ced2389a25645ac7a7cca4f1b7899b02d9c4353316760d79e51cb0237aeca74e533fdf3f5f1df867eaa58e8f61bb46af04dc8afa35dfbf544942"
                   "8b5fb96c6ebb580688d3f94fe424ba17076be801b5cb99759896ba048fb5af13180c18baa933bbfce5ba493b9d753c2eb1fc6942818116a98acdd94765189f07f50ef8e1b3c60cb0ca4861949624c40933485fc52a61248c05d9aa23840eecae56d3501ef442b97ca66e37c102afe753303aca6586bfde75bc917a31db6e");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "c0efa9d7abc69dd36ca1f456935e66404d082513e54a79058b2aa75355f418d78373b7dd07679b334534e67f17d8edd5141f9989121a0d42e9cd88507a288d40f03942ad469f17590a42a77c6041e35b492c6d1d257442da771f9cf6e9c017ad90a712ee7055586d433300f4768391f975966a76283f61f5bd89abeaa9a5fc46");
        bigint_t S("bde6e60ed2758b9715ae15f321eb3badf1a47f043de93e7a913c56b830fde23cf25b7a056af98be57dfcec626feb75cbe369621c442a3548b88b50979a1d70f0bdd6172997adcc5f48062d28f63e08ed8f395a8c579c2ac224ad277adc5ca564d8b9f8a200055458db6f7e9f2f5e8dae8eb8fdad869e104da5b7bfa30162432744"
                   "4e527685a135a01baf7de1fd59c755b32d3347578f784b0ee0ff292c0b7cb6caea3eded7a000d3259a55e010805dfa9d9db65ec082b0d412911a34b482cac50e5adc98e2ff9979869dedbd56f77d62a8407e61c2ba6ba4d60f82b215e090c8983bb5c606c800bee7ff425699bb9ffa549255df4024fc5f3981c483bf0ca19baced"
                   "8e885ab6ba26c7bf8fce5a998313f76ea6b96cd0d5d2943f934eb77dd49fddb53d5938a1ab80b6f228c7721dfab591650a8f2e0dd571b09fa237f65ce80399849eee339c6b3ec976bab7d6394e9508db0105c18f3108ceb8b3d4a98cb115773d369ce971644ba86efd6e173f8b4a52d1e3f67588a1907e4c76304e33ea7c");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "7a596c55a05a4e3991ed9c779db9b309a79d40f470e8a81c18cda75ce11137d63707b388edbe3c21ca795c12445fe9e78b742cd8c9c8439ba8efc47d16b0a3b1450156e8f5343622f56b29c3e7517ee9450c18fda16dcf88316a13488acfd284ac309c9c3f2206e41cab4cb20ff63ab6db4b4c8a7f3f148ab828dce6c7d48df6");
        bigint_t S("8bf66f32bb27f14569500055694a9b70938b7e3fa6bfd41d6772e74e4cbe6a7b52327543f2d861f2d39c9cb15a79008e10ff95b60d13fb76ec5e1ae402515f3e830177ca9b359178f729fe133f27df97e164bda5457b3c392f75b5fe38e8178f0b74dd81f2f322055764c76e129dd53eb3ad781dad05b22724fbe86d521f82cc2a"
                   "b5ada5491b5430cf60c250dacfd7c230c87a5e17762b4d66c8e413dd4b2b6a6c505cba0d954eb483fb8d59e65137adefd7fd4dd7507e89b1fa7c230cf5477e944c46d8b63f9fee270c93ccd8a1ebeb6a267b8d91fa296f8d5d19157f0c7f165171f05581f29a584f8670dbf9b77dc74e77ab9a6f1a3ed3fd8b9795bcab3eba0d04"
                   "9bf36946829b48234ce24b89dc560fca830c0c8d0fd8d46ee5ae179d67f450771c4a282c8b0af305c6e73e64d4e4ccaf2fb8e1a105193e550222eb4af772b893f18097c606b303c6a34b04abe9ca7f30a4e2362c96b2ae1f8179368921ed391a10cf1bb1ea9542d9bc215d36709d3de11cbcf6b41224131dcb7974055967");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "38214b5023b4d5287f39ba5a8498942b3656216fe70be99ffd16212cf58fd013a87d30b2cd4957d9c0731625828422890044712e982ce77dd91e327a8a54f353ee971e3847b1a36e99244e9fb01ceb7564a3af671c14d3d291d2824bd539f53493d5dd8d98d2010d92b4287118493915ebdba1c17ca9f5c5ae99d848dc2720dd");
        bigint_t S("65655169aa2e21d4f8344a31d3ec6f8dfdcaff88e0a50bda626423edefc469928283e2c1d72d77a58ce9c0a1faf37b7d31f9b31d7436932505bb6600e124d5d457352877481989191934f70eb27d929f83f8c8081c9599a6a0d57587e1ee1cbd4e9e60afe8b22047297f9d37418e4eaf8885d33d598177e757c1ced78b8aad0bf9"
                   "bf5ba4acc71cd1f9abafbe506a231bcc361b6d5e766baede75e2c7b47076574c5b485e5fe0c77ee6c34b48fd8afc3b168b8fe4ce9704f00c41cf7920c197d0223f562f9379f07ea28c2bf82e18f23d648a810fff1ff44c95f2b247cc571b60e329ced3a8736776e60499a91775cc8d889e655250bff9eaea77e71d8c1579cf34e2"
                   "8bb929db9a739f919bca00dbb9881f58d00ad2b85ad4d3b975fe6bea0a74625fe2d497cabeeb8de568f2ad2f56707173860d7701f6994f10604d2d2ece117e45fb52bf4c0b4a91d3480ece56955367a5a348f159254040e2ec95e853e22b4a5a7d1ba905555331c587b10ea50e39f92aa6d6b5fed0361fa23cdb12b4d71a");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "e3c2a239be80bc2d5b855b89dd765f39d7af83a169da9ad3acc018065060d344843e0b6216fea835dc31c403526f693884a69c5a2fb19a3f232d617eeb887223b05ad6a75bf95f1ffa4830b365cf7f5221119aa4841928401a57b22048aa62512fbe0aefd39867517d6fa8815ebe2aef74978666b23acd3e5be612d4afef2937");
        bigint_t S("82f364da3f75a3b80a8ea5bb6882090e5fc6628c94a3f08c0ed529d7a7e2990c3e301d9569a15afe212420827976e4559f55564fa6355ee5d54e117b25daed068fe2c0a9a4e3930342765fb1f5423ac2e053bef4d63f599a9178da7ecbc430cefdcc503ce92853ab6c9875654102509960f01238d6b1dc08816085144ebff1fb6a"
                   "51088ea481428970fc2514d103732e27dc2366ffa6a10cea63263355f3e814e9a31f133a83b678e5483740eabd0219a28e22134b7010f02fd92c7d4fb6949e09d78b2be39b42fa2636ddb176dd9abc6f36e78b2da0207106423fb7e2344d74afbba457f3eaa3002fd4abfdc12e16a555423d8666f92dcfeeae4d3bbc3ea5564f2f"
                   "850030094e357e4f9a7cee9ddf570bcc9ddfe07cd7f7720bc5c921a70dba0dd5dc38770947acea77c4126d9699af6ace8ca7b9689043f5435352544f458d150393b84c6711e85fbb69faa21c18d96b9e39d60dde929842740c263f05054ccd8b184786bba426dbc1b01823c6fbdddeb7e6bea8cb554e991ea98cd8768b69");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "7b411e6d7c492539b50bdb619c7192c134c7779c3d1d6a29a7ec0d4e4dfc1babe084fcda219496f554b06559f1bdceaa9c22b01857273de0f7a00815d4d05ea0c588da864c8f34690d465a52858cc2adf8e0fe95abc59152546b0120cf8b2d321293e1f76303a89cc917e1d2c0a615d1304a889da0c10e88a79320f8a4f52f6a");
        bigint_t S("3e66129baebc98bb8d72ea27e3fed779f0d31731f4dca2a6e0a4f66cb0db7d8cb75b7813476228b331aa5a67496d840ce555fca7fc540ce2a364a6bf9cab3c505a52cc3eda03bf49d3032362c30aae0b058ed3401ac32d84402c3a499f4b4c272d6dd2756208306b809e16dcd204253379cfca6074368d504487dfb503954b7518"
                   "9e3e47ea60b5edc32f28e9043633b3ec965044f3ba6728b73213005c06b5c2311743702149298ac03043dea9ad88b7813e2a981d3ffdcd6c10b1848258e15d76fcc96fc508109931e18698336fcf15837f155b4fea07c37bef4bd9be74c1f9cbb62352d5f42f07d6df782f2078f704b020a75b895d1543d60ddc2aba119a1bd23a"
                   "8d2bfe9514f939aaa3716a866d6d1303967b8d13c8d23336b0edd2976cbc803d00b4a925866b2d4799c51072066fb04eb0740e749aff725791494f28f38ad9248fe1c62481ef9ab02a8328f3c95db2c963f8c6da71760203cea0315120e65123f03573d4cf1716a951ab8808fea8c0ade4f00078e9c60b8644c166bff193");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a6114f4aa84d14cbee61628328cf031b29ae0286eaa0cb93d2048ebe346de44c1a52b597410759a664ebfd00bde449c2ca0840f04d9d6709964429de04717aa3ca1ebb47efc1dd01997bca2c8a920054eb217f91480ee613dd3dd9ff1003a57a3ede2702ba63ea0513778fac96c272f814618a11e6db91db2d5765c95e0caff6");
        bigint_t S("27fcf85727c0d1694bd85c3a637b098cf71d486866c654a469408b50088f208d72b2da92720637ec8807cbb8024e04efe569f6070d7615ea3bbdcd4e088cc31b1eb56be4ce853cfaad3258bb95f352a5805e1a3ccb7219eaf96dbfb362ac1d5c3f523c8644e46c3cdddedf8afefda527c8094932e1299bdfe98dcbef535e26d203"
                   "4cc66c0ded6cdaa599bdd1bd6890359087ce30c473b589c08670f5611733e09c028be916bbf52064c4630f491a3b383bd0fd5414c35f99e9c8b2a7ebf702632b53368b1a0f6f74dff5637a22d70b7536dec2b1ab1b9084461640afdfdc1ede94749203dc5f899e17f522e63a5f26adbc280d3d4ec8c04a6407f6fbcfc3cd1a060a"
                   "2a64fd3355c317a6721755838981736f1867c7e0c6cabc828cf2615f62c8d5d7978361051acf13f0a3471919e9cac0801c3f92fb1dd6ce37524e879971e2b69b690d4b95e9120055b8fb06b91c01fb241eb1630b4b0c75f481e249bd7e8b57cc4096aaca11492849c7116ae121495cb37fed86e4b8ddd152952e1ae0f824");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA256_RSA_PKCS_3072)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(3072 / 8);

        rsassa_pkcs1<sha256>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 3072);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha256>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 3072);
        EXPECT_EQ(f, true);
    };

    bigint_t n("c755df3cd383466596520290b6f7afbe8b949eb5f9e449ef4e34e397b4d0a93257ad93a83b2d177ea37eeeab1ae175ccd81156ec1381072b30473f613f1b918d1b39653ba6cdd832e4429acba2fa05e44cb296981ff7161f17a5535b2adfe0fa8a56b092f35dce1fbd4365e13970befed80b8d9f413297db07bcf491e5fe236dae0172"
               "e05147f7a85a3ec11a074a91aabda90e94949eecea765444ae30ef629ac682efcaa1272ee17a2116019910323f00c95842cabb019cb0948bbb362ea57efa99a78b9785658edcda6c29884a10f3cf289197d022aceb2cdbe681ff5c436ddea48a380b6b79fe2bb88f43c1922b3cc13df4baf7e6761f29d35b47c1adaea89594c4c7fde4"
               "eba855e8be1fee172af4b35cb732e39af61e582ddd60d93e06c74b0d560d015a02e5c4d4c33cd68b50cf69089fec3e19ebcdb45828e96f5d176584fd3827adf87c5b9174583a2373243c24d99ba202e0d4849e7ba073a6081330eb5b50254113fe3e4207a355c371f24607276eb7a884f2ccdfa8313d293d5e1d");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("222a4af8a935151e08d1761c992ba34ce8ae18b4ce87ad0f6deb5d3ded911d0ae2a1becee513a1b5042f57976ea449954a4c508666826538e70db324871541d17a62d041d4e16fa6ab5e6a1b308c2371e19e7376cff5c0ee23d6a38e9aeee3e7f5498dfaa5e94450c6d6f43191ef8be0f0a52c49293ad371c865ffd238e621eae4d9dd"
               "376adf07a8ec8cd87a8e58ded631ab35f34bf4e05d005a89aa047ba73e297b9c3f71f71e0f29e85d55f946e021d1cff0c783e961099aef5ef2bfc2e77cea58902d910279228addd532dc417e7c64f394419a3d70dae19bae780bf932c502ed817dd7bf3c9dae31c9f4156f8029643a20054393c849b32dac3931695ceeb700c006caa8"
               "caf201cfeaeeafb0f4bac89416c50f14c93aac5e3efcdc9409e491450bc3ffbaae46b5647b7a9718ef0b32d52403e26679515ae70a5a9ac35851344602d8d424b6c556b64eddb9111df66e6d8c82c4b9734eb986403957ffe415af0d13d3aea4734ec77b03e359bca2cfebc3e6cc96e46b3cb80bbc04205af3e1");

    {
        bigint_t Msg(
            "5dc2b5a9d8d72492b8a4bd0bc45e2e18ba62b21a4c27355b6871b9e8bcc8f89f7a294a8858fbca69dc44b494d61d12042e6498a8dfb0ccff448a6ae593da06ada79ff36f02e364a312efd1efb3bb9c3ef6a8f5122071fb1bf65f230838bdde9d6c8c7606dc78396be20adac4631e14ef9a9890ff175309d8075aaef9b55bc898");
        bigint_t S("654ff18089b8778a5f63eb4d743cf5bd0fe68a7575e0043e0007cf0133909eed03ef0472ed3e50d8ed880259aac0a3406314b96ab60ba023576755e56484d550bbb7e02a0fa02e3b6907b6a7dc8e7264cea4e975e1205561796d19611c5c018c3a64bda31e4c8d7839e6da1f57656e44a5428226198b4a52997746a82415e3c8f4"
                   "ee84d9fa8094149a4e765f525258fab720fecf6dd00550b141029d6e3b9ccdf1bcdbb3622ab97661180f283606377e7dde80c6abb073db6810ee4056d4e0b379394164adef8e22fdb32cb2f42e2bd2031b710c40d2f1e727b9218162468fd773767a9d4821942dd3937a672c03c0beeee7c1400c9e2c204fd86cb862e68e78c18f"
                   "702e5e10dc9ea1c3833bd209739d47db37036f96ad69380faa26f33e400ff849597c82d3f44b517d04ceab5490436c375409a43fa01624be3a1477a1b33ce984b021c9b3d86f9cb633a7da4e2f7f25467b4daefac4120d59398e4aab6c9ab8a511a853d66c6db91855bc9100017d058387cf68b9e6df390f3ba1a981a231");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "8c89c71dd8a2c2976c21d6ea05dd4dcadb40b31f81a1defd0ccd2cd677e70cee4673c0106aad6180651f2330ae72da365785bd7eedbc6abca09b2ba5a393785fd61bbd36a3035d902505f623909ee765cd1c0f4464f7d8189f6474d6ed28678684aa46f74a4dbeb1f6fcd72b72077a3e842d32daaefa2ff8535390cf1a4fe24b");
        bigint_t S("9973a5c2e9dc3ee2f54a94c8c79c5bffd644cacb9607d392b01c4cb157c0a6b8d94fac5045dc47798a2e9ed5b1a4305618b714e4d98e8ee8c16e3b0c4a2aa799ed715a631ddf71d0ad9299b3b56dd7f07e464c28c96559469137727d4e3388d526342a9a926c50416b5e1cd434d80ba4844b76df434ac487cf32519e76f5a3f213"
                   "49be54000ddd1e1d9d70d14193c1dd0a6be6e493a941cee49263718571cfc012173549f1e91f6c5e90a78e817d69fd7a154e372fd056ef803b294f89d2fbbfe6f42ea911a53ab8902da65397cfb6a1be35f64ca77787f6cbf64257420b581c2dc93fc41d27fa98836a02b30c64c81f1b11c023dfe7a4e7c5d7a1de278067cd4fb5"
                   "b5bbbf9e0b3d252b4ea63e265644f07c114ca3106f5d4429c61fabeece4688fdc860dd7af7a7d8c4891403ab07f2d6980788dad2c07da2abc63bd5693200c2ddf9e406fe565222c95ac66e0a4fc7de7ec48efb661533201f24301025693731009b4d8e734ea62f5107d77d98f137cbd1c410f2178df4eb65d42a06cb8000");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "96cf462a9446d4ec4a70af8c1644c5d0f8d5b07cf42437c5b83918087f7205bc351aeca6dc3d7b4b2ebbe4ab849cdd2cf6a3ebd5c426e1bcf51da3c23d78307d24b6e07665b3ea9594c128d904fb1f4ce8d63a37b8f95692d74f83be725a0977abcaaa04f74e27b0c4aa067e600a91191695220a9a469e2da7f74066e6bf9890");
        bigint_t S("9d739577385c3124ad11db386fd1818657af9a0af06eeb54de027000effb4ca87d64d9cdcfe80f9bc4d370a5ad1ff676437f26d100adf719c807396f93bf3b3511e2191cc042584157f08606a07946cf640f1e0b94022ac3b613b63473ca76d4d3c401be1a1b6aef01a1118f0051c2f79353c5afdb01481c98ba9837c9376f94e7"
                   "2cb0030319160f05cb390576bae902f345ded052466285c0398b2834e2ca854e98aaa3ede2ebc569ae6fb11189925b50d93309a989fe1f93decc3e785faaa750d92aad9501a530cf60beb94c6eccd67509978f333fc26c760fdda487dd1549a71454f996dfd36f9317d758f7d359194f005b4fa4754671f55b879be301400a7756"
                   "bb07ab8d4fa9b7fa3f289473d6740376a3ca0eca54d5c64a492f7110a201c2de5f89ea9d058d2b38621e43ba96d72770b99835e20eea4a6fcd25bee2c5bd03e31169b924df990cb9d1374b9b87afb798d8703073bf570c091a4e1c43a67efb3b81be5b77866f13ada510aebc827514bcd55efa69b3b580e7af5eede53c34");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "9a126e437b5e015eea141ca1a88020f2d5d6cc2cb3db196faee262ba99fac8bb3b66d5e50a4d60c436b3b08de9a8e4facebf9154d0c2aed124a4ad2189b7eb44ce1f4a9e95871c4d581344baa995976ebe0389282c29052cf39b9cddbbeee28e5d271596a57400a2571a1f506faf04f9ad89f9cd5176e72624ad7716b7057642");
        bigint_t S("70e10e29b7ea17f9e69b8b8d2dfb1afbbde324fe8e64205e260f362d6708838b17302e7d55d83e3d66b87fa81dfd641fd9d41499a1a1de9615362e1207084464eec60251528c0af12e784de39ea5bb3105c203e432d0efe1cc7f5908cfd7cd5780d6511fdd18de339ef6f46d26fc98db604b0e3a8eec6c276cf07bbc65a738e82e"
                   "b89cc5468c91462d8e40e1ffed9b165abdf308e8a49a3eae289fbbd2d8d52132a0392f5a01bd046622425c8254010dc0240f76ef1458ef456ae76ca3639f608723993213e9c5bd04f0e750fa9fe534ae97dc3fc2e2541d11ab91b626fc726c85f2b519af38a4b75952b05672b4fc0a9357d31c62908f4df63e87c9121f5866f186"
                   "57b708caceb9af633a8f517673d5512dc3ea5e3d2917e6517c8853d53eca760433f9cdebf3ba210c866d88d02e02ae21567930ec0656c4814c9262a901429d206478b7e2a1f40a3c159f8dce30c305739a32bdf752facd76716d04bcbac983beaf0897798d669f6fa1bfbd29288765230975572e095aa24459ab956d721f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "06c2f886ff1775dfc39ea830559ecc96ecbb0574ee1bf4bb1cc889690a551950d3c5b1190aa8a988f4877c02f482d74c4638ef95f0e718b7f6d494baceeea89913aef5497e4106f811e9269395a1748a470039d7cf5f783d713ec7cd87e45eaf2f466e1f412ab4b6848c4deb2141ccd957da8a4371cb3dd5f5cdccac1f806219");
        bigint_t S("ad511de3f429bca205b5c616b9a34c87ba106bd887ed5765dfa00e15f0360cc439c6f825716042e8af57ed1617848477d6b1e06ffeea8fb708cf66d2a978b1150a644b2d685712ddd1e92547092a25adfd05a26fcb8197e73de412fd697aaa9fbfb61a13d96ae1fe0e43d3579d6b18358169c9b038b5302ff67e63b067773d9e71"
                   "d7b433e2066a8a41541ee7c0084ad127c06d6df52b12546bf25d9dada97208af936299b233f899efd96ec7befe62450bf058e83b948654d172e80bbe66ad2a9125e3b4b27078a5c9dbeae5039d1ccd376a99f27eb01c974756c7f7d00a3aa5eaa111cd11fd452771d015be3ff481815547c472d333cf0a1741893ae46ea714f6ca"
                   "d3770ff9b75977110f2bf2e4006e7533d92b631add30a8ae12a2730aae767be0f34bc2c5cca96bcc3c388ea528c8b2814eafc7a422db194de4547f34d41f207559141628ddc5a2d117252f01bff97e5323732cace93b6e0e6edbd6cb804428c7d0787302684adab40dadf647e62b56ae03c6ab4a6babd1863d2c09a7a5d1");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d5b58bd311ca3173ba6ead0effa14ad21f38dc552c8ea4e410777318cd7512a9092fe287c91ec0277d8a2b1b3db5a9e65256660ef8481f7362843edeff4d47ddfcaa72cf0a199a3b61825566046a652447694fee8fde49355ae159f67f79bc8fb60f85d15a2386566e3e7314df284533085add1c7bb6ead3ff760c86d5633a66");
        bigint_t S("baab3a4d1322dc8a11846abb0a360bb8de78fa17b59677543d00de62e661adf86160496fbfba90d7b88439fde23cbeea41d24ca5ee9b3ef39368e03b240e14bbbf13bf1e727fc7ce0b60ef1f05cd2aa4a81e88a046a3ac0400e6f84756aa9016f6b03f1d1ef21e11ca33b5d80e5839c2bf6310863bf5794d4128275e01cdd5388a"
                   "306ed4d37a5ab84cdd798c0331047ad606518d0f0871e9c45266e4379e6c53d929a025ce8910268c99ce5414d1cd040f69abecdfb12cb4c8cd651d27ebd1b2a3b66ebcd7bdf9fa97e3dae41519d81a23667e302eece8805e0c8a9b0fd6409ffa3b55b9818cc0ad006992844bfa5078375e8711a023d61610836f4cd3ea8bfa6871"
                   "722e96549864bef6c6d50a62d363e551d9a080a654ded270d6b145b2693c784137903481fec551281d11a835197bc428229c952c844ff80329c7b4469969acfc2c942c39329243173003a38e13dc915cb3a55ad54e79a703417c171a98633d3c463db66b34bb96f7565d7cc20893e7037921f80ebb58856c8d23b3cea7d7");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "4835f68708de7caea9f587dd69125f12daca4f33dbd54353c7709632ec0125023ffaa8714dff42e3754b56ff55509d6e829d75df7c057c4fe1fe5e87e047ebe6f1ce6d60c3d038c7764282490b70753b19e48315c189fabde310d8089293e07ec678ed5d519987e115a1b4c373b910ff076520db92a8e425376fabbceb2d2b52");
        bigint_t S("c0f823fef8053f76b565df98a37ef1cc9d8413bc806cd9d211d7d32e47db3a43605a3a5d933929a08ff3edb26ba67c89ff03ba9be1bbaa14c3eb50ab84e1a5991a7799e125cead5f361381f4b3be0f299f04b08bd1281fc1e0c8a0975d6908e5d5543320f3b7ed27c07b952b7f0d242bd4a377be28d27e3bc45829915792db51f8"
                   "3b11f5b27f12290d399e86c23dde510d473437368825a4b0c38e6b118e482de54d7147f6be4f1daf8a9b51dde4a3d2d07931f7cb5554b14e0212a33d772fed8d07aedbe16ebd9db4162131b07233762fc0ee345d03c928b5e68bec906ddd603b0d1e2caf7a5ec7db4252a92207b6c098174a2c803437ecd7c8005a0844b2e8680e"
                   "63f41cb1fb65819aa923f7928f0dcabe04350965ce0f8fae9667d11f651a43a35059264fe630b38161a4706cc3d6f4f5db5790810ad5cf7319124b04b3434d782eec544d5de0e8d2726e51d7e17ecc2c9828b207944756d59ee247d19dd8f34771b03ab9e556f068e6486fa59545436c047756704657faef57243abbbc0b");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "da338b0dd6c48d2862bff80b8a14d13c0b9e558fb5f7829b1390d96e4e0163804f9df2e6244aa9c8a590be1557ef7f7590769380dc91832da023798dfdd447b9f7adaa09d7e2d086cdee19a2dae51b27ed06e86e4d9093d136d8f062c7979cde1c883ddcb7330b4a2403901adc9fab396b77dbc1ce9dba6f1fd7c85774f26ad6");
        bigint_t S("a864050be16b5452f9d45b3dd39fc931f738f9175befd256846a1271f067d83109209afadfb5d82836389555007cddf25e8af7fa5e49f60c0336178550f74559b55fad8160290597fa1d7996bbfb038e107f764ea797d5af7fb1fee34f06a78b25a2e532b9413e011fbfc4552f6d17e2dd4068fccded5125ad3e55ac1c853a8c06"
                   "3010ca1312ac33d3dceb8ef22013ae55c46724f8539fc1bb8fa413d6b993378b6648ba4d52a933712aa3b44ea2ea897f829bc97db63eabf9d7de169b4bc0bc4e67c48035df593bee31b9b047f04d47e5996052dcc78e25bb3f2c7b45e63a05c267ff7671db2a01b145a198326727f0f4b395b1364f54613a32099ee8571d69f715"
                   "832cea04b36951129f45e3259626b7f54b09fe5520db4e07facd2686f0a8590c6bf6a05e87ab7732e00e161e3d825568f2fcb1cee514f670eb25bbc3a8ec838943e5a1836ec32b2257289b3ab05f99b246ff24cc53c7f5ad8a6021dbee0c36b754b7c005e447f7792872bd602ca1f2c80a9150e54b27d11ddba52ea0a9bd");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "0de4f920eb750402796b81a963521d234cd1336c13dc353e552a4d2a33ea44e855b2a2ec2eb817398244197a2665cf4f08e42ee56f7662c983356ffe0f51184d860300dc44c30f0217bb175afe7bb71630ee8096608d573a40d21a7444f08721a8c15919b400b3043fb8c27072fc9f21ced972a87089dd3894e998b4592e580c");
        bigint_t S("44c175e10abf86836af7ddfadc8af5cf446d61176bd251e4f5c2770bee835e6177c6bde9f51f802d4e92558ab3bda8d3ade34a4c9ea3ac439cf5a5fbac53b4c426a9644567fe6698cc1444ae533197358629557b18a077043087b0133091fff1cf8f00813b8224197174557a9c8c0d15dc408cf371d8ed098b3cc9f5c22fd588ae"
                   "56bfd1862c3f457d2abd2e9ac8436345fd688adf0f6089693bb0d35661a1127b4a65d8099805f64246a1925f4d0dc6b60193ff10740335d77d5d62fb23e0d7d07520af4b12d224098da47f789167297d191db59da3e3adb185774ed83c6d5924eda40bb62335e3313dec8f5ab3c86f85091976747f8dd824b287b56fce16b4e250"
                   "7baa5138771a0db95022177b1acdf7377ec8ab2dfbd77e218cb546ee3b82eafa18ca04066b503435e891010001a0fbfd8ed9bdb7f771624a957199998ead0ee6eb7c4f37df5e34ecb7d00d308417dc38a6cf9810668070d6bf2c6e797a6a2e9a7579f491bc9c81d98687228ba7368ca67b5a5a4d3e877c34f98c1b5bb05f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "21682959e6957153bd00a1b40719b79059aba326c80eabb6649f0e92026de7763f35551cc191d90cc06cffb2c5b941b2b26428800389b42b53e5fe79228a1c39d5d645d5fea53527dec6d38d29ac1240086615e164d29655620d88d32a2dfb3bed3341aa263b4f4b20f66570b85af2e16498b273744f19dd98fff4c8a3c04e25");
        bigint_t S("46217e7b3ab09e7faebed8b896fca4c136a2bb2c7986b4b6f6e9c1c08aa5c08d500111066cb225e215420694e93cbd11363e3baa0d6dcede40b494fa781d04f4def3c4ea26b54ea8e34a72d0af6ac11e9638790cd3c00b8feadec25a6a16bd39537c46dbf21b68334427b2782db33a361ec6c4208b09cbc8209d7c22664560cb91"
                   "b2005bc2185371dc6dd47eed011bd2da60e34dd430fc9f32668227905c09bb1f2bf556a60c831098c9fe083e0086499fe75b92c2f81811d1dff29e8dd0a873fcb69641f33767c0f0eb2a9eeb86bd02ec388e7f0c3fa3759d1a58c2192d4fa99065d1544a35912db42b20ae1f6d52d777754e20dfbf0f060a5bae05c1c119d024c9"
                   "e512f544ed17e3ee600648ca2e5672c38e3f5a66e50de503a7faf9feba3854be609825c33f6b04af1668d1763470b6932d8fc42e0b0659aeedbd9cb2ac16b5a0b164d02ac0e8ccd17d642677fd223dc7c80b28b329dbf507c6f9375331c5be0fd1bcea43d41e3f9fc5415680367b12abe067820fe45db3f6fb55d36c9c7a");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA384_RSA_PKCS_3072)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(3072 / 8);

        rsassa_pkcs1<sha384>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 3072);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha384>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 3072);
        EXPECT_EQ(f, true);
    };

    bigint_t n("c755df3cd383466596520290b6f7afbe8b949eb5f9e449ef4e34e397b4d0a93257ad93a83b2d177ea37eeeab1ae175ccd81156ec1381072b30473f613f1b918d1b39653ba6cdd832e4429acba2fa05e44cb296981ff7161f17a5535b2adfe0fa8a56b092f35dce1fbd4365e13970befed80b8d9f413297db07bcf491e5fe236dae0172"
               "e05147f7a85a3ec11a074a91aabda90e94949eecea765444ae30ef629ac682efcaa1272ee17a2116019910323f00c95842cabb019cb0948bbb362ea57efa99a78b9785658edcda6c29884a10f3cf289197d022aceb2cdbe681ff5c436ddea48a380b6b79fe2bb88f43c1922b3cc13df4baf7e6761f29d35b47c1adaea89594c4c7fde4"
               "eba855e8be1fee172af4b35cb732e39af61e582ddd60d93e06c74b0d560d015a02e5c4d4c33cd68b50cf69089fec3e19ebcdb45828e96f5d176584fd3827adf87c5b9174583a2373243c24d99ba202e0d4849e7ba073a6081330eb5b50254113fe3e4207a355c371f24607276eb7a884f2ccdfa8313d293d5e1d");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("222a4af8a935151e08d1761c992ba34ce8ae18b4ce87ad0f6deb5d3ded911d0ae2a1becee513a1b5042f57976ea449954a4c508666826538e70db324871541d17a62d041d4e16fa6ab5e6a1b308c2371e19e7376cff5c0ee23d6a38e9aeee3e7f5498dfaa5e94450c6d6f43191ef8be0f0a52c49293ad371c865ffd238e621eae4d9dd"
               "376adf07a8ec8cd87a8e58ded631ab35f34bf4e05d005a89aa047ba73e297b9c3f71f71e0f29e85d55f946e021d1cff0c783e961099aef5ef2bfc2e77cea58902d910279228addd532dc417e7c64f394419a3d70dae19bae780bf932c502ed817dd7bf3c9dae31c9f4156f8029643a20054393c849b32dac3931695ceeb700c006caa8"
               "caf201cfeaeeafb0f4bac89416c50f14c93aac5e3efcdc9409e491450bc3ffbaae46b5647b7a9718ef0b32d52403e26679515ae70a5a9ac35851344602d8d424b6c556b64eddb9111df66e6d8c82c4b9734eb986403957ffe415af0d13d3aea4734ec77b03e359bca2cfebc3e6cc96e46b3cb80bbc04205af3e1");

    {
        bigint_t Msg(
            "c530d8dbcf28eddd459b5f6f19bc5c929ab639df51fb6d7629d8e138d44e6736691d8768522182a365e191c551666e333c491585aa10cc7fa1bb61430b99b3c65b607381737d2c69c5a98120b9bdb76d4fc773ea065dc4fcfc31630856e63ae4e222248cfbe5f417ae19b8e91b6a3f291d57d8762d895dd5836470ee7fdd0579");
        bigint_t S("827d9ef38d99808d1f12d98fe4b45e93d2c9df343ec37e21497715043760789a5854950e292b48aa526244faaad5d894eea3695a1a1c777f85f8455c6bd06e39a7c45d49a3ea1d5122cd114a746d15f525e1344d3e4971a3ea395bd650d5fc3b2b42dbc175cfb651b3cdc39bd572ea50839782e0d1a969b0eea7e7366061c338fb"
                   "6e3eca00a5bca5d794ea005e7e4ee3c8e90d1df15feba8ddfe20a302995d41c022f8b257e99df2edab7d0f9c01bf9139a18207063a837a79bd4485dd9169b38a218c202d1f19f93b22459a5b3b3d818518d66a2298d821456a798dc7e74366e3cd7c0f4b514eda7ee311a8f32ae6aa088d8e9ab92bfd0bcd96f34bb03caece5291"
                   "31335419db1caa97894374cbbb27a9c0f662764a26a249d56541263135aab39fac9c5d35da0e279b7e0cbdab2bfcaef2d8938c8227352bb2e07f43571c11a489517ad62ce68e374d0d8cfb60e2887c3be9e783354f846081c3a7bf89196459ab02245d890ab0eaa457875aa5208d7f8a7e246b408d3c12f2b150e9f0974f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "20c328fe9add7f432a4f075829da07b53b695037dc51737d3cd731934df333cd1a53fcf65aa31baa450ca501a6fae26e322347e618c5a444d92e9fec5a8261ae38b98fee5be77c02cec09ddccd5b3de920367138a87b4fdf38815b9ba69642b621fccb2ec4310a136c73d731c7847eefaeabb877b5fbda1700d74de758b63ead");
        bigint_t S("6613f0059d4adbe9161557a9bb69b26ccee0bbda9644c082f25dc427c0ee7ff5e365f144b5bcb3ca856c5a33cde8a448af076bb578d82f436fad1b9332b616dc153346b3febdbcca6d4d626a3215e2eda7d28c99c23ae9b7e54fadfe995afcc58c2c808ea61401bba3d6e04402ac05c97831a590e704c8eb1a0b3dbe69839dfed5"
                   "64e9e6e393e5e8f27aed7a0f4662d6ae6ca8d9b5b303e1aa1414a879d99a02a2245eedacb8a6dd6851ea2608361866f9a65a07b2db08edad620db12d3be0f7ed5cf7aa6b79eb9383e14f0a7a0e7943a57fef64322687fdeb6f46e737f345643bb7f4e15f919332fe2036d3f13f71d72fb3a0b3bb19403d5fa34a5de6c75dc67b5e"
                   "f663c382607b5a6b2e017a0dcc38f2bd27457355d0331270d05cd0ee21c41ab7419b8a9cf3d2cf400f1bdf4ee28175f55a63aa5d5e48a7482c0b1f671493c3e20a5548484b88c244fb30486c882f2cd2c8343ca1e9acc12abfc6870099d1e61e7501080cb1087ba0f51c050c864f35353e673b8ce801646e0433513c8916");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "4052ba56f8de36c5456a7a7fc2349fcde3ed5f0fdb0f1caebd702351e1183354894f3976c4fb5ed1932943104bea8bdc3a609b978d189d4d719acbe7793feaa2404314c04f1bbb7e4109eb4525edcf24235e777e63f9d8002af915d64609fd1fad56d49e61e756b29631f98817e6738fe60f7c2c225ad5b02eb58987101b73b1");
        bigint_t S("695dd776cad9a55228f6ba2bb455d78cab87576a786ca07d008a1456db291de582bd33bd56f5ce35cb506e9867a8e6a389c0ca486bbbf64de833ac48e24650f0a7161142306bf823a0df886cbdd2276e8cb97244c71489dbb61acaf71977ff1ab86caaf8a9fb67bc2a242ce3001696dc45cb9c881c81771233a4f4cccbde0f4c9f"
                   "af96ff410758c19a740a625a78e54b1194be892d9cdb1e1864888463dec2c266c33a78df8277bf74881916f430f48b1a089140f05425cf450cff2deca4ab418f695ef4c7681d5e0294ee5105e1503a4eb07ed3e331e3bb4cdcce51264697003b4307cbaf2383c30b4a97fe9765c6537f42815deeadd04b694851efbbd2259d3564"
                   "509bbe5ced1d176102631eec96fc3d9a30b3e5317f2ee98b4f4193c1a1e55993c1975e05120c32988ddb805600f822b463871b82234c09e04181c5b853ccf59659a722fa0de3b2718e5eb676ee80137c9b61700e9f3103e8c7957c3443060ab871752499bc91daa85ca655dcc49fe450eb8480c466bfe7d579707ee74d20");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "4a7956ab59fc034dbd327204ccaf400ba1ae0b9b6448d3bdcceaa65ee1057ca107076079388bd07691cc67df1060b9029c237fc84571f48cce987e2d688c9162c1ddd04e3de776558349f52d3c98dd381754fe11111ad165f31c14041ccfb53780e3f155f178bd919e8f6516d9be73561d29d29e4fad33ee445eb0098434afc8");
        bigint_t S("3b9316e5b2c81382652f63de8866684b0f9f693d20cd26c2782cf57edffa966a775f7e075be414124b78a74da843427ddb6a04d6a327bc7b3036dbccd1c2b12e635c4627e4a8064c77f1ae06b58881b6144b1d3f2313ade755e31681929f4868e1ba90140bfe28603754ab0c84ebac8d7503ae149224594e38f9410c69770b1c13"
                   "c53339f5c3577380d8f8759b40a9b559cedafd8977c8d44f9c4f37997a6075d62814d7fc28eefe12fb49193c15fb3124134861f27abda01c09296b038998159c85735326550a214c6b3a5f2f245309170075203d00676a11fb3788b4d665889209894037c6dfcfc3f6232b9ebff516b785945ea012450cad2c396819f09c9bf8cb"
                   "766fde7d07d426c41b493c19caa1475aac4ad7f6629aea297327c42fb646ba4111477e723e58e789b57929b89cab53b95bb54b22e81f8bd7d3441bb53ee0b22ad122403cec31ca4d4f40fff541b0464cf5d4ec3dfe1e9b071bc4a6a9b11e5cd63c7f4516ed604129e1bd80a962ace74c9e4919c672b81bda0be11f3f2d22");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "694ba0a1cf223f98ad7b9f7e2fa16b9e7deb2eac8f41c46343dd0f6cad4feb82aec463d4c88725aa30c8334c259ddc208146eb128eb51e381a4ed9b6ceeb1ce0bb23b39f36ccac16e6be0e851bdac86d614be8f5560c0d8cb8aa1fd0c24c3daeb9f0f7cbc2f7be23f6e498f1b86119ce46838e1c8f4e7ba5457c4880327a3735");
        bigint_t S("2afa4db1e41af844e7ddc5f0ff402f3ea1c676902ca787c376c8952089776fe07f8d62a7b2b10510705663f32e4db0ebd365d169505d5d9bbbc89b37b74c24ae3b9c68112f62420983b5badbb238385f691a6795c926dfa86a259d5b073946f69ee99e683d642690fcd40f5a8e4f647c81f85262d26a2b4593e4c2cc452c3ff8ea"
                   "9a965eef93e318456b3ff6daf3ae5b10b40ad9a4e54191b395e7ea9c275bc31f92637d6d51117b42b57a94c6d049325ef5b2dffad0eb06f5e3c61b796c55c740ae09cd87bc6301dde55480bba60cd2667e8e564cb56ead71b478e82565380c8aef27b41ed3b826027625264cf042c5497cab09e957bd02464239d29b2ced84f926"
                   "bc169b1a6917e9076f6597e2a55804620cbbc596c68083ce53985ed7bbf8777d41c70eddc6dc952dd10029797fd081027594340d58625a30fa294c07b40f45df177a6e0d9f505e47ae77e036de3c52471ecb184954572f198b7c9b81956ef0169c8375023e97998b93dcd9340a39997af69b083310cb61eede3b7d7dc9e1");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a4c802d77e0432d516c59309b1919fd8b2890307554bf4c5a018c6ec9732bb84b088198986957fedd18678e1969b51519bcf28ad1ebb46efc6b8afdada44f67fb1a756066ad6ffabdd97ae07d0e680916f970864f63e9f43b14cfbda267a0446916be2d8908c5b1620405c03c83ad5a11429cab3459ba8c7471bd574e39d8ae1");
        bigint_t S("3a74cdbe007249e26eb41ab83a180a0be12472b57bd65295b73f8856b3179fd4551807c1e279f60a3868fce219895af9e2dbd71fe96830494a2a6ce263b2b13bffff9c30f332b054d4d2c1381c0bbc05bd21662fd324fb05ca8795fbfe444ccc127058af7313a94547df305b4b502e24e94b35cff919075b295899ada6dca61540"
                   "d793ea673465db0c1fc10c7ca2eabc253659e9c0f63a53c79cfebde5111b2e6e02a10d04b58f50b098cabc1a17b4badeb25d8fbfa2344555314549ba10bc9a79c456c96d88b2163336d6a5ab25cefbd2af3ecc804bb99f8ce341c214d6ca8bdee3d53c310bf46a6c94d15a74f3379e57b2f416ab552866078c0e6dc3df594cf172"
                   "4b119dc3705873be61a6574655e3949eaef7bf1c959de5641d07ab3d4b0fddd8a6706a25d32b80744646d3de3875ff9f7639d4bd398b4120fde22b98231957d2ee3351e226919bdf596223f15cc74f1234cd9d18cb682d641d6ffcda6de3d4ab0822ab841af591cd328e7e18b4f0f49c90412405fbf9a93df7384da3eec0");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "18a02d44366d91f9b94142a4c34319c69f2453be0355f09b92775a2fa51347644b8be0d48f502393f956864f57c5dd9cbcf27d89ca8da2772d1c0e2b68a7f321d4b51323e578261bb0457c26aa47e3e9b373cb2853bea894438e98e52f6f4629ba080e5cc34d6238e6f66c4e462ff4568a9185c42651cb9cdcb7408682d20825");
        bigint_t S("2525c6e9e052ba2279e553c525a942a040d7adc8c58fce7521093ec9d8b35ad14cb34abd68c3e544a1df38a6299ea4b0c54ff53799455b65c5bfc17d7d2b519eacf80534e817c96c7ad86fabf634e06ea5f9138a1b958430384f2ecbcffeacc3f5cad2fc951732a45dbfc175ecf48b16c18fe1e856c6214bab33aba7dafe6d7111"
                   "9717450678ea0e68cd213b9c4d6eda077cb79875cf7d5e80a86f905c7278e29376ac658f90754b73e16aa235ca891a9a164988f64d7156731a64171e364b68a5cb2bc9bf42b95c86ba9b971c9551ad315976400df60228c515174eb9c22c5e2309dea7baf60d89049033027e7e31daa52c958f5fe7ee2fc5f4fc17711a641099c0"
                   "b655f2026db4b4462ea1a533c2f28a525b8c0b1d11a7a411e7f363d391d6cc35e11a5c0f44d11cf1be6e6189ef7e535c2ce18e4a24d483eeffb9308d01b7a874f5df0b9823bb96b35004d044f0681e2614807d5f4c5e44dd3e826297bc5e77f507de35503e1e1fc6a9fad0990e83cc3666e6ad70748a13c0182cd1b79cb1");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d7e5971e27df2b1c3fa8e3fcc7d4550bad2a757c25a1b9ffee887a96d7049ea483bf744769dfdfe81e35b711d7b76c7e2a31ec761437c75c405e918186ac893cb34c773d8886db1e05ef433be7ddfdc46f1eee1065eac2345d1546d53989113aeff46aa58cbe66e4793e5123fd2d4f04045b43f21fa0f0b2cb165f3af68580c7");
        bigint_t S("bcad4a658073dda8cd4a8850fc64d4637e2018415c61c684d785812e635920c59d9dacebe7186f10282b8a26d92166c75ade2194cabb0fc1afb2df212a9bce1a3e5ad4a5f735a2eb03c9422232e7aef4d69c3eceb2dfb9e06f336922d02aa45a9de30bb409887968b6a95f797cc6a1da14bfba6805359aacdf5c5c0c66439c2f86"
                   "89793e054478d260df80d2617658389394601f2ad20cb502e8ebccdc6a6cee597541863f39d1cc480ae5656281b441214bc629402865b2cf62b21a34972c529cb91e1deb331460bd2d1e87bcbb5d1f021a850cd5a166f74ebef6f97a9ebc0b8e709c13e7dd822be44463dcd3cb60b1a34b7f0972a42b545fd9f5e7bc14c7b98790"
                   "2689a63695c5221996129c4ef88d3d346d594055ed76150ad6761742441f98d76f2cc4f6f4afab8da48be18ac2e9770307e55762e47db1be470e21f66d21062d5e3c6875a23ca11ec4c928dd57f9253e766910347c74314c99f17897b59693d55d3130a301353e781895e2482eb9a2865615e3bc0f5f7c223d064acd5cd2");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "6efdd76650fc2cb2d18b8bb3b7698449aba8b729e2ba958dfdcf662b2e5f90649d0216bf0a885f95b346390e78ae1d4c3d23b5e5900b9b978b256437fc1cddebc45d94997d269a9f60e088c565afc06fab47f5181b01eefb492e86139aabf846acb659def2ff66389f0280c0c695f51dbfbd81a8ccc61523fc93d6a8503269df");
        bigint_t S("51ffd4d03f1c19d234d4cce5c93d6550c051e011f16ab5b2d727963f24b829ca50fc0a5b283b74f25e58be40109eefa5cb6968e9af5ce1f3447da30f636ea8d81c002b90cd4bd40730e968d23b9b6d7b6e9d67f8044361bd4065b520eb392fe7d987b7735faf72cc82431863db43135e5b478ce5b40b0dff81ccf1d9999e3b2c9b"
                   "c54aa63dc922e2a690df44b4a77b7e0beab27debf700d6691b78e32a4bde4f3be671d0ed2b64b5d39b5f2320dbaea1e95dbe7ee8144801b95bc42e1ce7f983881349bda38f84ceb3e799ffd397c03330cb5eeb6f1c032d73ec0ba9e4e1dbbe7315227115be902a97b774ec3e87a1b1a0f82c1c095cf13650e65ed186e28ef40bed"
                   "c08be2c0fd46b049cc0c39c7acb2803afff4f46fdbe073386887e9d6615dee8acaa81165de79fdfeff68d945a90b6dc681dd366a24d12af416806d97384ba165657697b29ee4e3edf92fdb7375ab03e485a72073627e5334016b3a7f40f16aa80dde27493a6abcb9cbb91b487ca3dbb21c9f02e28f7e9650f4eb568b3c9c");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "ce9f1236befbba4c9664eab4b45094ae2133aa6e17f2cc2ac09b237241538dc0f790fa5000881631fee9289d17ca398641e17a71bcf7e087df9a11e9181283452f96e2a7cabfb391363c384d64c8938232592d06ec9a59f3139c35fd5dc8d102fc1994e32388db93c7c7e26da331e8cb7e028ed4b0c38195e3095854175fc020");
        bigint_t S("4e1438410023bf9695d471353720e2a700e23687bc0cfb5f76c990319a98e6d58c49aa89bc3a0860ae13eedcb69687bb3a9b4b69cf2f5a1beb06dee4512e4c9c0aec10db12ce34e97e29cf2e9c51ac74440f24f2b83e76000537b7764cc403455838f791d579631823bc9c58d9d99489d2bbe718d80b50356cc5022901e7880f79"
                   "b60d53911932cf77f47130b234446778b52ad14522c3a1da356d8da46b244cf3ef4ec53dc1d3b7b36527b4a5298732aaa78444ce022271b45c9315e5b4ab4d95170619442f6152cd8329f5dabe61a28e4105582dce523e220eed5977f7ccab3620d6e505be5eac384dbec79146861e9c664ad2982c3509076dc67be9c492046911"
                   "a33d79663c4cc2ccf2d4c526200eff605217e44cf35c8f9c5569ec3904eff5a528a2ff6e65f4676f8801a8ed77d476aeec8e5fc5dabec33b7befe4f6bbb17dd75a9a6f0e88a0c0667ed0397c52adfe3213f286c3e3bfd9d7af189c8c2b2978b2837b2b7a59bb9b946a98e7cdd771e489e3a6ead1b959959301a2d7119050");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA512_RSA_PKCS_3072)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(3072 / 8);

        rsassa_pkcs1<sha512>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 3072);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha512>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 3072);
        EXPECT_EQ(f, true);
    };

    bigint_t n("c755df3cd383466596520290b6f7afbe8b949eb5f9e449ef4e34e397b4d0a93257ad93a83b2d177ea37eeeab1ae175ccd81156ec1381072b30473f613f1b918d1b39653ba6cdd832e4429acba2fa05e44cb296981ff7161f17a5535b2adfe0fa8a56b092f35dce1fbd4365e13970befed80b8d9f413297db07bcf491e5fe236dae0172"
               "e05147f7a85a3ec11a074a91aabda90e94949eecea765444ae30ef629ac682efcaa1272ee17a2116019910323f00c95842cabb019cb0948bbb362ea57efa99a78b9785658edcda6c29884a10f3cf289197d022aceb2cdbe681ff5c436ddea48a380b6b79fe2bb88f43c1922b3cc13df4baf7e6761f29d35b47c1adaea89594c4c7fde4"
               "eba855e8be1fee172af4b35cb732e39af61e582ddd60d93e06c74b0d560d015a02e5c4d4c33cd68b50cf69089fec3e19ebcdb45828e96f5d176584fd3827adf87c5b9174583a2373243c24d99ba202e0d4849e7ba073a6081330eb5b50254113fe3e4207a355c371f24607276eb7a884f2ccdfa8313d293d5e1d");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("222a4af8a935151e08d1761c992ba34ce8ae18b4ce87ad0f6deb5d3ded911d0ae2a1becee513a1b5042f57976ea449954a4c508666826538e70db324871541d17a62d041d4e16fa6ab5e6a1b308c2371e19e7376cff5c0ee23d6a38e9aeee3e7f5498dfaa5e94450c6d6f43191ef8be0f0a52c49293ad371c865ffd238e621eae4d9dd"
               "376adf07a8ec8cd87a8e58ded631ab35f34bf4e05d005a89aa047ba73e297b9c3f71f71e0f29e85d55f946e021d1cff0c783e961099aef5ef2bfc2e77cea58902d910279228addd532dc417e7c64f394419a3d70dae19bae780bf932c502ed817dd7bf3c9dae31c9f4156f8029643a20054393c849b32dac3931695ceeb700c006caa8"
               "caf201cfeaeeafb0f4bac89416c50f14c93aac5e3efcdc9409e491450bc3ffbaae46b5647b7a9718ef0b32d52403e26679515ae70a5a9ac35851344602d8d424b6c556b64eddb9111df66e6d8c82c4b9734eb986403957ffe415af0d13d3aea4734ec77b03e359bca2cfebc3e6cc96e46b3cb80bbc04205af3e1");

    {
        bigint_t Msg(
            "50c2197ada262ec4a5050804b3b0d19585ec4212d7dc01608a282eefe258383cf181f69b5324f3331c53094a0f1531c3110c99e4dd55f75df8b01e86e8e1cee9a156d10040094340fba8325658f467b09e67823f89194d7b42e44aa88ec68e584688d232079dc8f12a4dabb0f7131a64154326aa45efaac3510cdbdc3ed11f93");
        bigint_t S("8c110bdb2869fbe144f9ea82a6a06555378eb5c5ff5473b6d15b54e5c0111442af4dcb429276440299c424fda9fdd8fda7bbd4eeeef66a9c1652c1a68844d691ef91a2a5c7f618af88b8471361aa91a7df89dbefaeeb2e596301c614c358270b397eb94b609c58397bd7ca6348f994df52ec749204dabcbcaf015dead4399df118"
                   "ccf2fca1e02f20a4f16cb4ef04e2289bf2e906d9b4acae7db719f58a97990db75aa36c5a5d5f522379f7f52c7a999305719eab0c58f8532cdaeabe8349249499901f4777295b655ad0193c8842cfe5b975c1c0cee62fe2e9300bb377fb3b7462e5937f0bd642321596819365379cb6bf51a3f0d3f448859df3921608f80f886cd3"
                   "4e582ad5a947b554abc2d4e25c3250a1922da7063c6fbb69973bffcd538817434679f99ed4ae0a22098621f7906bef15c8560fee31594096849194b18dba3f7fe9fa8902668b3225f11acad8b10f57b0fd5f5dcc6d74cac0a040ea6f5474c7a79fc465e81f853874787d64796240997c85a7ba9646ff995f5accdce85e78");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "ad97dd6e421faa1461cdc3a36bc8c01ee7d26eec2e2925db67aa5664b63c6431b16cb5dc145508fab5c30958ba982284096e958f007dc81267bfdf6fa25390592b20339740ba1904d9891fdbdd288331950bf6c36165f2f2aa484d0c170e0fcd04f981263016863cb5672eb43e40892e74f2b90fb9a74b84198dc8c47d378695");
        bigint_t S("8caf3ec11af85cd2a161773e82f3b3e7cc38ea3781227d8dc8049d07ed01e41e9db73966e7c707412bbfcf1c0792614da97ae9c39156b4da1f0c7fc9163dc585596adaded142acdf646f4ed7f27d2b9cd04325efd45cd2d1c86fb26c1e5ca73bbdc8b76e1a5f6465ccfbf9e5e528c9cbb341206834f235b94f97aa70c3a944dc6f"
                   "fa15db164ea9a114079fcedf38b344a1cf16b20de1bdc97e32674f3116e0c129a8c2ea51bb227ecaeac35e6b100c1f1b7b1fd65b4eba15b629032af21f05c33f1791cca63690e05bd9bd04bf2f87c8ca33cfe5674eec3280dfe0523c90eea83622295a3fa362389135c9b9092bd48d1f3b90ba108d782eaa42c2cd2342e964d649"
                   "f4c820c23b732b1abf6676252a32f46dbb4410fa8105bd4016f32d2527772d4ab2f0e0e169edf91440b3952dd5cceca0425d877de6a56f0b899f5cd20579b185d6d6642beaaa98f5a715a0556ed9e7c7fe0a19adfc36494e5cdf606b7d856796dfe2035f589c3344c3ec9befd74ed00f2845a36d6e7d7d904c530e379dea");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "8728b76733676e20a76c7fd31e959cf7842db351db407266ddae0b36e37f34270576724083e9989764d08a0d5c1b4738f34927a1e4366130c334cfe4e35ef6c777294122f73c8dedc682ea89117da0b2fadd71aff6511ecb739d2d09bfede8142568a9755909550816aa27b10562af4be1111ae1d939b3213510aa0d3dd314f4");
        bigint_t S("56a6d09254c656c35eacd50b69acdafa06935341c59216cbf2eea84a678abcab22fa17109512a46a30e1721c6d02cf103ea31ab154dbea1f9a59385dc415744636f017ee1ee0304f37e034b5f397815d58b386b63882db8d06c1ad5cdbdb834433b5349e5da719f56bc7a9ff58f3d05aedb810704ed34f8aed26546bb9417a89a9"
                   "94532cbc11632fae26593e090f6f9035d4a2df06584175264d6ef004e34b4a98ff81126579b805352aac5e5a6a3574bcf88f2fae077adf7e66a5bb8e8d45d28eecf89ce0253826806f7bd0f27a5591e8482101c244e5d518d0285ae286819576cc00f487582f5fef5045bc17e245f9f13a4e6c6dd7b92554daf948009886132719"
                   "cd7fe3b39d8020396e98f0b19c2e40cc1e905d306d74217972eee1d37676238f21efa4ce41d1afba1768e3976b909bc5f1b2e218bd36959ca3610795c6e7ad31752587627cb379ce9bff20d9501bf5f1aa5e5c16f101df44a6c8845fd7b3c89879b623a77dd1271b586b0982cd67388c3b1fe4a1a84f5dcc48538e19da03");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "bc7c3781e2f83a53149fc95c1a6efa27def23d376866ec4b0c1c23e0e21ed1f677140f17c268b1965aa91b15e62d5749d4fb64024ee7d06569ff897ca026f0f282ff2f17a70dcc2ae8187fd8cfd241004dbaa6b9ab416c96c32b5429703930c543053e88782db49928b39cafc0a4e2d3b1f8ac6669bae96583692dec3a9dbcf9");
        bigint_t S("3b07f69f1196cd797249b4de7aa4eba49e0cca5dd79633385740b65acaabee0b164e755ce5dd2a3ae78f017a8e63485c0cf462d2d5dd7ca82bc6412c5ac6f6859aa4482a4a8db4c4922c35d8572eb5946060f16c8b4ceaddcecf9c83c5adf18aea527631bf54671cdd482cde94ea98f3400b452e6beef0637d4451580255388286"
                   "d940fbcb26592daacf30452b886aae029798fc834a9629e425475a1c3fa74ca5020b126c300ac51e1c1415b6800f9eb9becc83b7d5a561d867fa0afc85ab55e8ffa2ebe1177bb47bb8a2ba38579636843c3f09643a84725c203154183af437b37cde023b89ab2a1ed997263991d13473bcc6dfb02526fdd7f41ccb579d5b761122"
                   "19607854e63d7b02ceee6c36435eb2bee7ae236caa572f3323450c2dfc15ad9077cd79b47ffb45618e2483f4ab5b970179a28dfc5e1f0a25dbc45f6e2b95d3752cb73c31f60212ee763242b1bee98524630924665a917b84646c0bf773829c204341b17416dbd0281c22d33f2ca9b16ab2946dede71f8b25668c22fc1572");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "f73951605d5b351846ab6279b2faf86df326ed67bce1b9497bb35386e8425d83ded29b446b08aa4423f3efa4b94dcd88d0fd1bbf0ee1d5a272cb113ee8c935b51baa68e18bbd463bdfdcd8fb7ca9c3815618bff4062e4b291e08a0c6b91ee3a756c99e80e60371c4236e05c62499421f2e7ec24fce4f85a72472e66682cacd22");
        bigint_t S("a5b9a39ee692971ec6d6bc5dd461c54dd118b5c2e604711caea4b40b258919ba97045c13cd767ec0a94254113e729469201ae2b1bdfb53dd5ebb59e9640a32ecdbcf8ad4729ee7e3efb91592fe3847ff7fcc6176a808db4c7fdbe4318afa7243bf7c9531ecc315c9eab2d4aa30df61e6ba4c1473dbef58b486bf38ac9363a61f66"
                   "0110992829c0b6d87ee25c82210f235194587f96a067c3104026439ff5a7a8f64823d35d76633af2c4c33290133b06c4c1ca4d4db7f090cba1eb30da6a77d4bf35b74738f135a60f0fd6af9848a0d51a868deb6f222cd05cca8007e3deac0a2946c7de43f194a987f643c6dc91fd5ae4672a38014c891d3e67db3219bca8206e77"
                   "b7d3bd2fd84032e80c280b2e9482b4e6e6c2da9e132732cc47b2b633b2888b6a6f1f96ff3579f0df2c4503d825daff3cd0f516984b2a9269e51549558a8fe766fe816a62b7c462adf265bcef1f89f2e281e363768bba819d22674ad8881fb783cd395f4611a881a1a469b4135560bc694e67445c4984b9fbec9c1e32c6f0");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "58d1b97a7b9eef3e425004bad5be8324236debcad4b44b537f1d0f481e0d1ad3bfa815c67cef06deac2b244a5b8fea67f8196ace4eb9e490b6b597c08a4cd732dc9a608368bbea59c833fdece78d07c6b843acc728de5d6b515e2c50f0dcdef2196e42b9294bc237fd3038aa610203c9cbb459bd4a6bde6db9e076fe3806589a");
        bigint_t S("6ae883471ac6a97b9d4a99f41e20601529cf4c06cc6a4d746fb3cb791b0d9504efe292e8e1bcd2907225f03f9fe5ceb41974f01802f840e619b1e2c13338b55abbc2417845b02cf5ef1332644272df1fcc0ad97be9fc09dc29f9fe866c1eb2eb2b27147cb647186678cef1429a3b0e67005c4855119c360ded5e4864b4a80f0fea"
                   "80caf33e4fc27b1abc81963b06139b06f38c089f79f2a1e9dc24970147442a2a1c064ec013d5d5177e8354e1e610282a7513d2d04177a5b9223d300158bedb35d579c91af4150841f7be2184a32813d4b2df0705c7bc0c86b624daa3ca395646745432500919c6f8431eb6eb8f481f949cb2c5be2d9ffc124165afcacb4082f97f"
                   "518f9834c77b8584fab019e1cc1002d86a8c3b5bcf2bccb3157103fc594326858e10cf8c90df787351fd59e1eb7c31dcfb932cd5deb54e066d86f53107210071d9ce167711a1ae4fccf33bbda60377fc9756f034486d1eae1f2df3ab10c37ad2beae18ed4a89905ff8609a220879145b569f201edf18b02edf67fb8f9fbc");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "529477c33067fa8b2db77cf463020f1d86011c1675e59d450ee28d0947ea9d9e7abb2941011b6a940362ed438d3a44e8e5a3fb2c1b3ace2fe0ba182b9bc38f54bada2f0246b27411bba9cc8816964c7a2fc2a22a49344b72e8320666e3048834b9fb714f28b27fccfc94010c269e8f9b0d66f958295d7dd6cd513d034436f007");
        bigint_t S("649c7c6c5ef860d78eccf523d0c7cfc3dcac9dfa91abf4691e9ba892485bf9df5cf73db22fbe176a1b4e8ebb7aef617f6e32325f927c0a5f44c41304202b70d072306c3648f506a80ee9610692678812a777c305c956f7d73af5c9e1aae19b6b9b5c4b5e7f6a2df4ddafc4da2cea6d0c2143c92d313210b15ffbf20d3cc26a108a"
                   "a4379ed04cf6b60283b0a0541af1332549122b6c041943a157d7cd3df03a242aa2b4482281cfc272344729a6316c342847554b6a10f7d257c30c0b19f75d7949a754b187cc29a6593ff494274fcec173bf2a187413b296c17fa27ff87fd713ac30dfcb864d07ed2749639382666a41e278c8106dea0797a63c0a185977e86ad72e"
                   "09afd841d79e4d7ce69f1994b18933632d7eba2cb768a1431b0cf60cb22940d4be6581d26661136ce6db4b14c7860852b3212e0baf8a600add573212d4f7df386d0f86b12209f4cc4ede6742892435b47ff27e98dead1c2d4bcd8ffc93efd1bad3f10e3ddc53c1ea57768b4295520f81c867aa6a5e8e681f705c116bde7b");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "bbf033767a3714f3e2a037af7e9a1fb189818a4e5770f79cf1576629004d5fecec27357b44498c7f61bc19654681a957cb25656bfb96541aba3521f3ab2c1f9050028aca736d1029c5678f9ec6832175e4825685e721c0aa8c36b57570fb502130aab9c9cf4aa6d958c822c3c4498ef31dbfe504e4ba4434b182a9151962fa86");
        bigint_t S("ad8c9bfd4cf25a482ef90ada72a0f0858de561d14c47c25a4693a2176a976c43518f0afffaf16a58b7e190aae5d596cc6350578fcba70ec5bd4977a10b54fc05c3027fa96bc0727294bd67b2ad7c1f965f76c8e85808021c7b24f54165f3dce007b153ad376ad819bd30d90f6035463bdff22eb8c11f80aebb413f78e59bd322d3"
                   "b9ebb2c13668cd94bfd00ec183c1d1be05ad2faf376b92dd6aef8b151509f5406dc507a817e0a42792fa5af4279294e70effbc7892e8c1bbd614d040cec24dff252495d85b2170206b69eaac65c3d99d4dbdd4bbc584daccdcf2dfe8eef7a0c447115d30270f4afebda47e6479f6095ad3e204a0117143b5901ba809f1a6f73a0a"
                   "23e9cbb1d7f0826059095b0954c57f18209f223f662286c9b2c098b2baa855e2df4124276b8585132b6f9e902d99a65353b30b9639df2b4a471ea53dcd1d14d4b60df94387089b2e2ab73a686bfe0465c80cc28a3db0f16c1acc5dfd493c6b0d01a3818917a76c210baf2e766d665a6bd155e902ee4fd82c65003b3cbda9");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "eb8e907ccfc298b66863b2b1dd0ae3ec054848a0853b59f2395e6f6cd6a584aecdca3a6172e11f6d5d33306c10499c5edb47ca9efd7c9c150c36f6ad97db7a0809359673d00ec115bffd016cb17fb1fb7049813cd5ddbedc305cd427e99b11c26878ab98336f051656682e9b08d3aa2a8bf77663548746b16c7711c4160dcf42");
        bigint_t S("2f17b94ee519fa45fcd2e83085f034398cd14c88a0b3514819e059dfaf79a61ee1fb453438c65bbbf923c8fe1860c9d0676102536081eb5aaaf5fb08c8c895fe4d5fa3750f98ef9c1206c76c843e7785e90455adc60718f2d44b2836d1dd8e82381a0bed7d48a71f7ca4d92b4be730364fa6ee861395e17a615aa210597a898021"
                   "36838f48bd9917ca4b94c6793b3067e22aa746df17311dcc6627cfc5f9b3cfc02829f49502df3559c80555fdcdc96504d8cef19ec6e825bcfa26c38274b7ea69ccc575fe47d5aec12ab45657f458386bee8dea5dc823729c5f4d3fa82d385df98045defdbf6ab984820b039ca1bcf5da955134808190f1ca4dd60e0d4382fee155"
                   "b86eb680a099c870dac75fbad0310d0743e735c329b4ceb608a448afc1a6139bbde7cf828817f79cbdc08ba871cbbe7073061ca337253bd19e4798541b1fec54032f291045bea57a8aee43cb6694774426f21ebe049d57c4b8be7e1dc395357fa1112ff49df86af49d0a96f6d24f164116f54cd2556497edb26842c4ab1e");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "373a8c725c3a7c1658d05dc68ea2d2edc91b25f9d3b381bd4370611496ac48c8811ab3b286044b22c6d6608116e093345458d2c4fc9e8a3cf6c417d3b24e80cb77366199f8342846293c841550471b665f567cda81b454b8f37ce490155cb21ec1987f70db527a4b2908b3cc655ed24b911e09848b7d47f9e1a0af92e65e4887");
        bigint_t S("33353a5e6f8062b183d4ddb18e718f19064b633ff8e3cead3ca6dc9af875110242bc20187b288a4c21d644188398fc3404195bffbda41adec3218b18ea8d68e98406cf6a91c203a48be073fa337b4c0e94f7cb8991d0c0ad047d147b1b662c15bbae49695af8d861d19e573103754d5a5789d7a84c41790b52f816e24b0f52c77a"
                   "da15deb52b1b463241d734a4cf5d4b031d8473502054f5291d7a867ac0b2d7a086602014da3a14f4d25379823c03344475f18bb17b67326ded6b93b725bbdf7e7f8df5c6e7b865bb26ce989ba75a93873d3075625d882491e7bed188af04232f27d4650375529432d7bf4a79f83e0d0e1b1ebaed4852b8fd317819ae8e871a2fac"
                   "daae876f6c8927db122e724ae82f17ba571baee771e634f8f9fab80e202bc51d3696f9f8838686e81fe0cbd97b56bd5c0947428a8a248ed2f55fc43940fd80cbe87c4963bd31138c3ca40e07494cef295e285297ea9a3d6d63475d1c8dae9f820e5d6d2d761e248adfb77b6a9ff6d74eba607690cb67fa78a3dbad573774");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA__1_RSA_PKCS_4096)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(4096 / 8);

        rsassa_pkcs1<sha1>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 4096);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha1>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 4096);
        EXPECT_EQ(f, true);
    };

    bigint_t n("d25c8f8c97b7e5a6a98de78f19ef18077cf952e64622e4142e6ba5c8ee265ea9b34b9e5cb2c1ac0307547faa70126229b32085988c0974a03198c89f8305be4bdc445ee577c60b7a7fdfd5cf7283f0a1d0f2d65b1ba2fbfb76c5039ee06a944d40ae3843c9a4d60571db4cd16e28ce5b9fb9fa83fa3cf8cfc1e87103612568ecdb1a5c"
               "50fbd0911b645983b629ba72bc2d228207b49e00bdae1b906aa15ea2d23f8c0e124c9e7de1b3cd3b9bc6a017c4de3f1d6548dacad6d4abd5f4e179a2a5e782fb8f818cf8f307f84f7f3bcd81059e73192036aca02f805070eded8b5de01037f2b34b41e86bb8c44760492cde4bc3b080d5b0624bfa1002bd6a75d0513b07c354e8edf8"
               "a6f57dc14290eb8850659788098b9971fc0492c3ced7c3478689444e771c9434fb624eb4243f740c3ef9c3f3f084bfb59f44a46a1d3b1438e661ba3ba08d525255224816348b4d696475ccac3871fc7836385f7078412cc58e36747d248bb1ab123fa33256c10d1102f77a60643f4247ac1cdfdf4d8d51deea80795964f801f97c4f95"
               "5eb181136bb39abecd3549541ccc9d29e34a63241a4548e836c4d2159968596130a222608ae0b09d9ed30cbf04d83083457f60e8a686a452baa2b08c177ade1a3cbe7bc0d6ed846c24c4ad5bcaff3817fdf3381e64c39c18dd4f54eb82fc23d3139843228489399c156565df75f3808cbdfaa53594bac5");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("28a6fc92c5eb22f54f44fe1fd87a2d219d542d6214e1bf43eb28a0743119b727825fb5ade259ad1d34b86ea7885af51f6bdf7811dfd61ff4a941065c551cb206c4fd6d7d58e52be8b37795412eb732b482c5bb0906513bf2ffa3d92c45e032e2af023c1b55878658ad051d5647bc990c79d49e060d40e530791fab5f64d1aef23ef15c"
               "2fc64a2b10a916e76e37a8316e0e26e20c0d2a8fa81852795931f030f50294d54f52013b007817cc52315354e495b28ac2bd0e932e6b10fe1b01cbd8d3c8e6b628a8778ed0a094ab004295573eb7bd3478d6380c08c1fa49a8a1445292ef6f707697af97f3e154051b256907588aebbc2a0f991f67a6da70b2772c17d63fd8d12b94ab"
               "4e0faf9330a4b4bfdcc6d9137262cff60ec003e61182f201bd07c7d3acad67cf2e780e29a11238785d4802ffa8136bfb4d3558b48e8cb77bd63ffd9bbeac0f51d297f009a7b603a388603c59d7d68a02e2e027bc7249177b185e567b7a092986e8008101d4f58e917c5e9ecef56d5acc00767e0d74b49c2d3bd29ca290e8de04f6e161"
               "d5e48af3253a1710d1f5b7a50f888a1417b12c221ff25d9b106128829894cb16eed84333a96abb2ac310b4a72bd207ead3dc2560939307a74f3ba3b8aea576cd846ca963cf337fb41b506bd2320036e3b469af5a1db5a2d7419dfc762441da11275c8825d96d4ce2703a4e183fd20d4a9fb85da6a07b8d");

    {
        bigint_t Msg(
            "80f161f9e7ae9465dc66e5401a778ef14194c5e6d7bae1288c65bf33c20364044ca9e373760581c3469418780214b968f23d951567ac1e3afd2069a41ce8d743c59cf4818b0c9fd2df766c8d91fc46fa945459ac02c22f115f431847220bd03171325ccb2dab35ba2e75a828d0157dceae43d752ade45bd43d3894c4db5596f6");
        bigint_t S("25c247f99a2ff5d698ed3dbe4dde8eb4913230e13039b2313dcd0cbfa8174f8251e1fb17aeeae360da0cbfcdc708751e8424d47e692e48ea06d4615f7bd8b71cd2ac1dc943d5ec2c35e1585c8b9757026a2e103e22ff6c2bbb70b051e83ac93d810892a38055f03ff90d497278738713aeb671fbb6b21cc23097c5d5d0d49c497d"
                   "dcec611778d905ea635ecf9c500b351d0c09aed6a59eb33bc461081841ba28e0eac4331e0222a8e8e71b0e5c06addaf46969fb836b872aedba9aff01f2c531b8186edea77367df9c015fdff69edca843ef24a816a0bdd464ebc7f7b20ddec2b74f9e856476e2d9dcf6efa4c352439470b85cb1d4aa21719a035dde4eee22eaf674"
                   "3874eb7ecd93334edc82957cdb8e1d6f56f731b6b0d5019fcff92cc1db18f6bafff146a9e9bfb5242f63418c716b5081ebf0c819bee15168e47eed4028a20ffaf9a762148384fa76e6c22f1cedc9e182588ee887dfe668c30ac4f6f27c4c3ff3e2dd73bf7ebe1399b5addbc6b5a1ebe2d97134fb26825649128879006fc3dd2067"
                   "b6c681abb678d186fd97f23231a798afb640721ad9b84fe5f51e712a5155e7a4333458eb145b67ab48a85b5eeee2d1caeb763a37018cd7fd5e7cd41f8965b25bb309908fd7532b5e636a85c7603759c0707ab58de6eb2ec46e9ce1eb5cff0861c1d31236735b01e9d6b83e0a2ad42a47aac91a0d0146db13cd4479c3b0");
        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "9f993b53072492ae16d9109965f9c86b41ae08c80ce4df18e15bdd3777fa1a6e486779a3f2d42eccdd1c7eaa1e0ec6b39b08bf5070c900489280c57cb228eed665184b7c81d61649ca2dc17dce7ff3b30d5ccd145d5271cb8d07c5b8498674693dd736467ed2520b2534617c3df35454dfcfb87e9b2a39871a19655402bbb310");
        bigint_t S("2ae6d937589efc4a871378fbf6463378963b3290002fd961c8fd52faf1909f86469b738a8f644fb3e9cf414486771b6947226123517046d8fff8d9262a99d3ae94012b14349995904677d387e4a12b334bea6bafb0a255588cb764867cdf9fd626b0c42f76e4dd30c52a691209a830fce1434455212053bf6ad91d2c7bf80dc79c"
                   "1c4b0157e83205867a00048758d7d136d98391ef20748fed0aa98f6b7b88e0db4981f0e9bbca300a50314e1cafb1f26580495a2bb3f0f05860c6dc6e7089c03bb58e29ac61a9084827570b95d90f568670d89e9173a32d4e54dd8047b8bc04d21a6bfbb948a96c64f74dd649bf3c911fab9b02697b3f7475f45ce017c696da06a2"
                   "e955e28152fea9502c59093704da6d7df963cbd94c42df9ff7fba89e7b113f1e3229887c0b26f7694d1cf694993fa7902caa92f0ec304796488cc93f789ff28f6d122c642a07984a6f71543c8edde6c6122bd1621de349ea2b67be2e87ec0142f13e47266eb5f04dd122c7377bcb6ceff922dd74872f091818070fffef24618b27"
                   "b12827cebed74103de9b9e0091ff2276ad2b2df0d1906b7228dd4c8b1fb22e75dea87d6cb59ae6eacbad6ddee0de44968e8b2450df327c885767e219601c91702caee6bad1c28ddc47abba7723fead6992432685665dcf8a69c0f5dd2275ff551f13c922f908c9ca647fa985f05c4d890e9bec9821c040d999e9e94d2b");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "fa4787c0dc22ae64e275ca1310802bab56c45d58c267fd15f3dbcc6fd1bb028c7bf5e56d812e997054623a19facb28110605a26fc86f9737c1d4b820594bd29bd55b5b1083634b6b37392dd7fb1275c903a21441df844a4df4dbbbce2d3ac67eeec112324a89695d7543cad67c1bb9db33e3da556f6dbe5d7638fd00b44f853f");
        bigint_t S("87d32441d514098201ab4c4336599292deb59dcc536e35af55032beedfb6dfe9f55fbd1c9eb088cee1dcf2487d355dbf0c392a98541ca3b416ce2ab176b3f7b031a27969f7a45928db902e1b70677a1706e23385cfeca18f1028aba3f084104e4548d1f1c4c42ba2587f59278962364c67da15ec5ed0863c91bff4ef5b6562bc74"
                   "197befb7a1870e9eadb90002ee274490266f7dd7ee832b818204c2d902cebb03502b3a4b43644bca905519ab789c3b01da27065060afe3945d372878d4e3dd954a7fcf75bc0772133b6c22313b2f38623caf36349e9ab290e95db7810254a0e6aa57db5412b9b10d08850a51cfcffcc3f0013a524d12c5f6d431f5c679fd2f9840"
                   "4d7db8d3f67b65a06d5f57e4aceb8bbb060fd054a79bdbf1142b33a458eabe165e5789f075b698c23abd044eb31f4eeaf29c5f8255a157e81ffcf57bb856173ff93bc51c889da065f14f8308e3587bf01ed3a7289059db19b7ea43d0c69194bb8b40bdfab94dccf80b6fc0903d3ba73d4d6b2ba0d48acb5a7342cc2e1471499163"
                   "2c8faaf2c257f9310fbed2295970365e02dd6b2849ee134f0702b86d350a32029722ccc2c77c76c43793253a2ffa0ac83c07532ba956132c1cf03a6dcaf128e498eb175be1e5a70d1573b1d4cc93bb924cc763a62d5fcb06c3522642433b36e866fb6c62808710ec2dfcdde7d212b31bb53cbc79de49132c60a90d2585");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "c82118dd81f0fe7132d2b47cd1fa71e3fa9f63b3da4eab144b84aa78407c7b186446c0a1fe19a22a18ea136ae03d56284e3cc2f20706c312824921be6913858f42e539354e0a2f68b1264521d412c70b08e735346c59470910472f815a8f1e98a92709ff396aad1247afd10a7520cb746fca45e2cff041e03ea55b29dc9a4e1b");
        bigint_t S("6aa3b2eefb2ce0eba604222fcd9306900db019877bf5b0324330fc784fcbbbfe93eb01f5be964dfd2318238850e5197d43a973823fc4b9d596860d4ff4bc4a1f403facf99275062e41c7ca860baef4527944d869a513a06d960f61e272fae7a899f5a133a02cd20f45aa9abb7f372cd69121cb9f3c0c512494d8f97ad3c9cc1d3a"
                   "1aa3cb29c7d075218a56be1f01516450e41043df5b182a599dddadca1eae1cb2d7aec909840894402e8b6a0b0b56677111688164e8df904c0eb06cc4a379fc0f25fac324dc3d3d4ef6bb2a59b50aeba815e49dd74ee19e79afb15b07cb09151aeae0dec21c052de0ee9d85870bb8e7269478edc83204c3008871821622aee87636"
                   "018a628bad4e6cd9da74ca2a1bc900c1ad34622e1ddf4d13656b84936692a969759ea95c26daae786c729e90d3ef2a7672fedcb3572a93dfea2735ee38d033fadd375245c0a2cd0b60687a9620aa64105f4131bb067606742b5cf016966ce8079b317225238edbafbcfdee2a82ce19274bc33d0a0bd8b3bfd69987f95a4b4a21ed"
                   "b3d70dbcdd6860355fd62e9876602403eee8acc79cb43a7a58362d648daff05611f97e85f3d351365e28fc4dfb328c4ecf6c0c88613d5bcc21ebdccaad2c0c62d4eabf968b34036617e1925fd908b0bf649bf2f202f7e5fbed6c8f4e9605afaaece19c1ecbde5a5304121fb537debbab9721b0b9b9964d8ab2b8cf989f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "430dcab25161982d766e3839cc3da0c41bbe0ded95b1500cc4c768f2c0055672a56a2d2044d16958191981827e82c0951d5a50daabda98f3d44bc81b716cb1af0118dbee5724d1fd9087c02d48bd40810e6d726b14494bd72b352f6aefacc20c3f60fba4f1217a22c3001bc394b4cb90d43d9a2939c712c0b3d0c768a38d3090");
        bigint_t S("42681ac1828fc86403b38e24e2022491b8aec6c75c80fb3f6a96d8dc7df5e2a5f8b9bbb3ad6879cd813a71eb2b843b234d5fe76e2085f8d82e085e4cbd1a05a152c806629226f5de8534773c15b566e538c272c8ead0b6adb98623f868f54b765c7b0981ef9136ec14b42d798e1a041b2b8e7ea74b1d4ba56ba64a926419c5c504"
                   "d6365d7f64f3ce6a3c689f67c4fe1704fe2373903ce63c07aba67562bc2be87a4883e20be5b0a3ccaeb90a97db921ad5e68cc7dd4c9cfb5ea04d895e32a040a841b34b78844f2f6853b3855bf489c10919646a9d79c2c4690ff7e32185c13dcca66e214787dd5806b492aab746f35ea9458d7faabb35ad45ec766d430096a20e51"
                   "68ede4c636e8df4a2b99bcbed397c476c8db9abb1135f1b95fb022fba03a6bb357ad7c02ee2e6fc3bdec125425185ce4af50c5adb7175eac5f769378bdc95f1f1a06d24199dd7f846588176172e42e331a4186765b2ea9f9c8afd72a95ff35ddfed7f3c80e2e7a2b5b6d5e63ad09fda5759548f78eb1252f3185f023ca3bddff2c"
                   "12cd52386c4cb0444e2f0bb25c62102e4e4defaf216670f953602435d138d1c2f3e6cea5de735d9d753b1e073659aed8dd292cc15ed56d6169d916d18fe47d31073f38672575eea480e4c6430dc460447dbddea2c08d92a06fa849589563ede7b37e16bad433ebf9e2e5c3c36165732305a27a3a9acd5999c1f0b92507");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "edd8d609976e9973817577460629c749612eb3d21e30488bcfc7c976009d492f6dd27872729fbedcc23a86f65cfadd6fbbbc3ba94d85a0782769e326f7ab47ba3a255079afead5ae0b2b55a7ee578f0016864c24d3188010ba99915a9ee8f772bc9905293697066fc30e8c61a4880583da568f4996310eddf1217b1cdd9e7fe1");
        bigint_t S("69c816e55d6b3b3bb442adc908a98a0e07500af74f9b63f88f4e3127440a43268b66b6f5284695b9ba22ed1a476f206c29f15539f879bd451ffd40fa465cb8f306e179c63a5e9ddc5c6b715d3ca8cb5cf97fdb0de4a376e0e8553d4e0f865e907606cb1ddd93ef3c39fee31de3e669874494aa6f9c56532d3a7a899c6b6cba958e"
                   "8602ff27a69bd1d06a819c6e6470f0fafeaa26361e0ea0f60f70b94a15d71f8994a93c65060178b078c144b5363c8f72c22e5073832b35e0857d6a440b3af20d2769459d05e43f820494c8add81b30dfd936a3db8eefdcc3fbf0887608fdde38e9adb300a57b27f9c44764cef49b49cd6da0e9acdfd5d76feaa8acd12db571d537"
                   "ccbe5dca7aaf73cfdcba93c63913c156d2f3f5a253f7d8baf1c40d9904b4b44b36755442e42c82ba54838d7703bd6256100da463948af5a990fc0680f2dfc7b8741d85afda74445cd1e0e4e319ce7ae6173acb29865e8d94f2e0519ed3826f58714237dbcfac731652a120b900b81017663d3d54f610d493833f482b6bdad2d46b"
                   "5d11f56c48c205b1c15851f7228f1fca4fb320c0bc4b0a285103079a3337084ac4e14b6ab2fdc39d4b447f5222a80caa33f015fb69f688ce3a6d052b57fedca12dcb176d2a656e6c9b319e399aa55dca9512d09d2b5c0d5fd765081334ed5fe27dad1e04323dd62796bcc6e8ab9eeec1149c20aea04ea39894e1499588");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "827686813a7c22dfc81ca7643feae6eb1b425b53d38c60dcb2bf9fd4e224cd74ca5d96059aa5d34623590312edb8f93e83e7d2c112372a1732ab90e704b536e37d41edef01f4518634b6415a0d6e933b2c0c053a6dcd760ebdba56b72c735b61442a758a99f20567a23a4a84c7941bacbbbcdc080cd5ab8af9f5da8d83e4a2b5");
        bigint_t S("9b028d8d6cbd7ebf0e70d06f147943e0b36dfa5ca7c5143f981b1e76ceac8898839f107b5bfcd85cd9af5c27a27cd1d45ebc28dfac534de658b387d8c4ccc5a0d46de36df955bbfb3685b264156be333fff4d8ef8a34bd206f80ccccf6faa55548316938f68ad75fddb552c4b72cf0fb1728090a1ebf2fccc1cb94c3ada65d78dc"
                   "076b5c78b6bb997bef8cb5cee6b138318881e56286f2b9d51fecf1da448ed9e4b03228e9fbb658d6bddb043eb941431fe0908afb80a90ada63cf92e16cc1af65ba8c4eeb910c32383dbfa140330605035ed1797880c8348cfb613525e0c54f82e6ae83854e0e764dbd1b502f19aa372f0dd481b1423869f1fc21cff325d0c643de"
                   "b71266c991c175a15a2d3a0c6b04394f9dd9354e921546091995981fb3fbfc1631067c51560f10fec097d27a587432380db3bcb677c0c21a1762260f7fae5f6dcbe55e1dcc5d60f83ab3181100a2834861e2c3e5db2e4faee1a3da48fe94bba52f13835aef2ecfd9be1c3e8f0e8668bc05462d7220465783e25bb1ed20eaacebc4"
                   "27c209d2126d36b6509350f2313f0c2f2478c243555a3197d7fa9fde28b0dfb4341859392ca3cf8fd6ba6f81b579d67713cf9e9fefbf22c60d0b99f71228570b617172c8fd2d02e8b14037a1b6130e7c35126ff5df195033cb22058848bc7893b06d216b30b271a174b8e733d7563cf4b2d1b61f00bfa796d650f28349");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "f08ffa1716c6a085ee1951aefc9147f5e9be1d99ccf13586a9ab577137549a23e730881521907d3ed1455029225a354a8903e29206d0f141a72686fc4df0311fb2921654ab1e4b65abcb814e89a3e797837b4be0822882bea0f00bc5e73fefe65b9bebfc3bdad1a185932c46b8d22383dd10a0abc862b1a49cf02ef58da99c21");
        bigint_t S("cae090d065e0378374d9433f4cebaf7bcebca82a0b79ec21313c4906bf0fe1377aa32f49bb671db09607eadfa3c4d302cd4dffb1c14ea2f21fa2cd6b9e8e96a5867dd9dc0a5cf4a01cfda814b13650bab33cd0bf34a3a4b87c89833453f247475a7bad065256267d726fa413faf46d595277e68c70de02b73e9e4abe5894de7513"
                   "2e2398de941f6e051da9eafc66cabcf596e287775ea9b50042dc365304d110ad3311b06e0146096db49b935531c1f132d64ab42a84128d3389b031cfcab154d88e281f27791f17ce181d24cb64549f0a93ee760f19905c4da8095a12cbb01b8162bad9976e0765142654e7581627173bea35d9afd3682f0c2581a37c4720d110e3"
                   "68c558c220ea4ddde558e859bdb53b091baad2de07e101f56adb0a1a240f697358e8fa7b745de43e0f76e5ffd5da1dd3cf63c8136b001d1019034e688a960206e30e04bbeb37e70705504a8e6802cb92407d55ba800f7e935ccf28ede9053dd3ec564609b02b34402b64ecf013b40752a2f28945e37b9339176a6d86495899d4af"
                   "fbecc8d1812d536fc1c48b7f3345241d5d5d18e7f5a2423ff3749790459cb47d71eff579cdaa76d2117b531ccffb481904e711019ba50c044b4b8986571bd5c3e0c9ff2856993c2e59535e5b6cb070bd227dcf50149ae215c2f4290f4318720eb33d50d1a13d961bd1d907851a794d211799a6fcff7dda92abf97b22e8");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "e12929d730aaa1bbdfcde32c1d98307ec40fcaf3dca8a0d161fe2db745c99e30b90e3389a993e7e08596537e47bed067b252dc2ed7a9bf0edee26e3703d5af66df487eb8e967afc03c6ed6d79ba964b3b29e123c9e89839ab00824936e185814db40d20df90c41828bbd33e01f3f338d4c5b35c2392e359d5eda1a728f7be063");
        bigint_t S("7f08e819e3cdab1a45c3ee5cfd9c302307d79b831c7f4cdf0378ae6ffdf84d8900b8fe8cb89c3536a1219c1214255437da9ac5e2ed679d5677e370f301fd2c0c9f5327ea2494608337595e4e22ed07c8aa4bfc1600a368a3497508911f236c4c1bd7c2d2aefb744215acb2ecaabe6fd0c4bb326fa93775b438a8d5314330c24345"
                   "fdce856572cf91021b6f75d407c4c4341473470fd84c4587037c59e02eb009287c0e51444e397876e2bf5cda80a01bbdd5c7a60c6020c36f3799c89fd2b6cf5dcb94a33c7e04ed21d6ca38144716bfbd96b9f510c5d701e56a0bc23d555dc5cc53e80806905204d867eed6aceb40bdf663ab2383dfdd1db48d6cbbe0005f69b24f"
                   "19f9cdc9af34f7e5dc18f9cf0fa34eb4eeb6732cc05c342e33c4cf7995c6478ef2f64f018dfa0f085b1a12b179c3cb5825633189bb4affc0a001a7c6b8bf4e4a047b637a065974adbcac3cefbb4210acd73afde18c79dfdb1fffdf916b30a80b3ff82d39fc4deaecdce05ca4fcbba7d5c71981710ade126986fe48fe4b118f9952"
                   "015667be3fed473c362d273ec3d3d9d260dc398d892bebe1c13af33cfe323c7b13710e1652fb8ee5cb4c0653ab2f47d1bfdd217e30d082b9a27cc1f1785c9826beb53817b5c763e217aa06c72319bb77b47ab30a0c58d80eec4b4bceea9478ae1a35277c61ac42b0ac4b9700d14e85203708b75680239a6d50bc9aaa7f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "766e0bd60285d58beaff18f45cdc2135c177329cfbb8e07aeb8cba3137922b8f4a18a25a9c45ef2c3397f1816a916e0ff1ad42561598bc77419e4549c56342aad7d53c13d75e3e70d85e7f2e5bd841955d6cf9589585b06af7d3a9147cfc4445258e22293e398afce03b0c21b91b646e0f9aa1cb03e786fb131230b9e3c3f907");
        bigint_t S("d17c50b5b0526a1e412d793b364a3cc690d40b5ad0ebf627ad469ee01af986c51fcd10452ebd90a4657aea9f90da2c9a9ca720f082b012a0bd9b7b032e53ff0cdd627586025728227f9894da05d74d381379d3235833fdc028b9b30dc7818ca2720826ea4404f655f10298b62fe5cae187417e9773f2b2ec8af17ba3bc09837dfa"
                   "7767ae48e57f4ecf07dbe3058d25f8acd4ee784bae85435fed18297996c6a78c9616a6a920e524ae3d61bda1254609ede681b807a7529db1d5d64ab7427a4029d7f8b131ef7072f66a302213ed13ce451da0c5b98f90e276d5cf675c9fefde457df91c897f77426db94633e345f3eac00c823b621ec6eed45a0fcd72470b4cee83"
                   "909beaae4eb41636ff14822dd74417b0447316ab33d1a4cc7a7bf0253fbf812a17fad5fa63ebd41eaaf7abafb8f41b096831def95ce6eca029114914573b8eebfa96a296b7e6bcc9fad83bdc1d6ecf3e6b22ce9f3dc87a68ae918b7daf98ee9c4e4d7ada2320f19e9637254c72c29df2722cd9f472f421e77419486fce49d5dc7f"
                   "4e14224e312a28769f853a2b2be73dae99bd4ae4d482af32afacc581a9f349e74fb1729eb4706ac213493e845fc37e03e4f234da3fba03c620b234889a3cada98fe1cc0b5f5e108e5c0364d71913fa7c0b42215788df409250f3cd72e822fd66351ab2d960b6c43bb36a5c8f3e627a3e8062802ee56f8ff7936a52ccf9");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA224_RSA_PKCS_4096)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(4096 / 8);

        rsassa_pkcs1<sha224>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 4096);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha224>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 4096);
        EXPECT_EQ(f, true);
    };

    bigint_t n("d25c8f8c97b7e5a6a98de78f19ef18077cf952e64622e4142e6ba5c8ee265ea9b34b9e5cb2c1ac0307547faa70126229b32085988c0974a03198c89f8305be4bdc445ee577c60b7a7fdfd5cf7283f0a1d0f2d65b1ba2fbfb76c5039ee06a944d40ae3843c9a4d60571db4cd16e28ce5b9fb9fa83fa3cf8cfc1e87103612568ecdb1a5c"
               "50fbd0911b645983b629ba72bc2d228207b49e00bdae1b906aa15ea2d23f8c0e124c9e7de1b3cd3b9bc6a017c4de3f1d6548dacad6d4abd5f4e179a2a5e782fb8f818cf8f307f84f7f3bcd81059e73192036aca02f805070eded8b5de01037f2b34b41e86bb8c44760492cde4bc3b080d5b0624bfa1002bd6a75d0513b07c354e8edf8"
               "a6f57dc14290eb8850659788098b9971fc0492c3ced7c3478689444e771c9434fb624eb4243f740c3ef9c3f3f084bfb59f44a46a1d3b1438e661ba3ba08d525255224816348b4d696475ccac3871fc7836385f7078412cc58e36747d248bb1ab123fa33256c10d1102f77a60643f4247ac1cdfdf4d8d51deea80795964f801f97c4f95"
               "5eb181136bb39abecd3549541ccc9d29e34a63241a4548e836c4d2159968596130a222608ae0b09d9ed30cbf04d83083457f60e8a686a452baa2b08c177ade1a3cbe7bc0d6ed846c24c4ad5bcaff3817fdf3381e64c39c18dd4f54eb82fc23d3139843228489399c156565df75f3808cbdfaa53594bac5");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("28a6fc92c5eb22f54f44fe1fd87a2d219d542d6214e1bf43eb28a0743119b727825fb5ade259ad1d34b86ea7885af51f6bdf7811dfd61ff4a941065c551cb206c4fd6d7d58e52be8b37795412eb732b482c5bb0906513bf2ffa3d92c45e032e2af023c1b55878658ad051d5647bc990c79d49e060d40e530791fab5f64d1aef23ef15c"
               "2fc64a2b10a916e76e37a8316e0e26e20c0d2a8fa81852795931f030f50294d54f52013b007817cc52315354e495b28ac2bd0e932e6b10fe1b01cbd8d3c8e6b628a8778ed0a094ab004295573eb7bd3478d6380c08c1fa49a8a1445292ef6f707697af97f3e154051b256907588aebbc2a0f991f67a6da70b2772c17d63fd8d12b94ab"
               "4e0faf9330a4b4bfdcc6d9137262cff60ec003e61182f201bd07c7d3acad67cf2e780e29a11238785d4802ffa8136bfb4d3558b48e8cb77bd63ffd9bbeac0f51d297f009a7b603a388603c59d7d68a02e2e027bc7249177b185e567b7a092986e8008101d4f58e917c5e9ecef56d5acc00767e0d74b49c2d3bd29ca290e8de04f6e161"
               "d5e48af3253a1710d1f5b7a50f888a1417b12c221ff25d9b106128829894cb16eed84333a96abb2ac310b4a72bd207ead3dc2560939307a74f3ba3b8aea576cd846ca963cf337fb41b506bd2320036e3b469af5a1db5a2d7419dfc762441da11275c8825d96d4ce2703a4e183fd20d4a9fb85da6a07b8d");

    {
        bigint_t Msg(
            "bda79b564e1ef644c729add43ca108bbdb0558991611382c059213c30578e7f6a62d4895c425a7def3b7d4f3212bc28b76148be77fe0b3088cadb11f565eab45e5ee826cb1050e9508c9ab44aff7f431cc32ce41039ba9b4e0a102a0751b2c9e449451c35fe5b4d2cd0a38af302e2617a5082b8fec6aa09ac9170e2fa9d6f26a");
        bigint_t S("83acbc8e1f8217b7a2f36b57c62429ed7a31f7934fb19836560a6ba115174c4d587d73564a7d0d49dd40cfc46c52e59e46ded247d9068ad8ede7dc4e7acedd6c22316d5e6c4ef57efc986f75236889e71b958ca5c658a6d1d90be87edfa941816da92533c85db67f4626202b7dd0f5dc9b1793a74186057cba6f277c1897612c13"
                   "b513905898871e276051ae295a77f708713246f72c7f0afb8ffd39cd37f5a125e0dcd768e30869f838cda6005858e7fbe1dbaad21d575d5c6376b7698a3afd3a52a349b3d88667e0c2974dcd559470fef4777168484c2177239dec43f2c2bd2f5682cd89150515922907160539901090fdb5280f8b1070a56b3bf3266b9f90f6e4"
                   "77b11b3de9c731d77d2ecbdf5a9235d3e5ef0abb44573daa1d24ef96a65ea40f0d2ee5c4118f358662035d0e9f0eaca3c2fd02175a260754910836521d72994ea6d84f35edd7cd0c61de9404fe6b2647ea6e6ecdfcbcddad2879da89b0161d972862bf2c249cda8ef2f36fa5eeaaef14f2d43f29d83a2a6352eede58296baf0505"
                   "a4ec5f15931ca4df7ebe3d84fd9b6462dc6ece7abf37ec73f404838e296a8829b27d75d2fb43bddc2a4962ce7f057106d77eeddc64d116b0f4964a0c7abe52d517f6e51a99a7a7bcc88ebb420eef2e4123a1f5b678e8657fc00fc7e97ac6dde190c0f14887425c30edc77ba6f7512f41fb3e2c0b0175d7b75573e3d332");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "6a25de6760a962e601aaeed7b1521159a355c25e91d4acafb080c931e30232662e9b36366e07a9f048a87e8c60e09933cbbfde01c6de8e09a88ad399152599c075caa00ea430cf9bebd1cc4a304bc94f6906a1ef571723877064add01d09fe6125fb457669eb7defd36efd9c895a4bb7667332b2aa471434f056815e61760752");
        bigint_t S("61c807ee465fd1d825ab5d99724b659bd9583fec75cd164539772e74652bc61e53e4b81021ec44698ae913ea0b6d2370258ff5eb2894e4661c250cf87d81299473a46e8f93014b116c9f7fa0f4b89f56e89b40aae7c41443c28bee1741ce586e6f2bcb91715447af4e7dc64ae843176f3729ac4eedb88c88e46301a082ef656bba"
                   "d3bae14869715e317ce0ff1e6df00285aebdb5ea2a1d1d564fcc67eadb2364b7b73a948292607a89c821332b19d7a48e0ee29eb23f019159310eb613f33e4a26c33ba66043cb84efead979dc2773fff33c6e2d105b8cc0692bc706a502361dc7918afc5e84393a703880b80eb9424d661e8be14827a67f49c3de2cf006ed260b9c"
                   "dc5208e623cc3f6e4db25b3e9bfc7be6c0d7f9eb3dddb9183129bb97c339fb759936c97710ca4f5e1cc524d03ff9b03a5045d15d951e739c910efd9ea2edf7e42794faa188bb13797dd4757934b4529524a328957d655a85a3a888e8d05f6d9da9ad454dcfb19da64ad35a73fa7cf8ca827ca8a3086744306eb680acb430d60fff"
                   "150494ce1b1702c10b53dbe49a2aa501916a82c7ee72c2fff95f063d00cf572bcc94ec0dfc8d38f6cf6437a1802a0b78c45abe654c527dfb97047c9ed1cafa37a9c554997778072475e517f765e71df8af222295dc5515fefee1a283a171333df9cd37c9d3ef697ad61fb421b5a10e368dcb98cacebc9a34a64264fcf3");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "7310ae61cc71833c740f261db8d36bd631c2d72e987e0d86da108abd6fe49252e8156ac8c8ecc84f95a388a02f6ea02960bc566ab3b9b19f8a182d9f6b11b2049ba5c78cee2904bfdd02c5d2d9ae26ccdf042be74dbc3e39f431df936e6697aab1472d74e5321dd573b6467b5f9be307d722bc6d0c8d9e9d40e2086320ed278b");
        bigint_t S("9fd63405dae6a2441ba008461a2085eaf031ee30eb710f9b35fb469c3d2025f79d48f295110f7c34da4aa67459c7732a3e55ada08adb96f679c636a989aac7d5e906776fdf27e86c071f2563ec888c373fd9d36b88c0e10c2b6e17ca4de602ebc076ab6d8545b56b1e34ec7fb3f81293d27aad82c61e52939d41ee0232fb3b5541"
                   "14548fda2badafa6f07191ccec84b60a4c1117557eb45fcb763bbdfacf81b4b9cd980c0fc533f1d1afcd6fa807a26092e87004172492b1f33ba43ccf7d3e17c3f4becc7483da9c7bc0b66e85a3e9ea7334e8bb4e9b37e1cae13fcac8c290f1174524057e56775817368ced1353f1bcfd6ffdfb05ed5ae839d7ce222aa602888995"
                   "17ed8d3280090eff7b8e8a80b8e64365680576c8e38d3bbfa895aea72de300af919c7bb9fc867fd3122527bc3484c7509b0c889c836e713665362ea5ce7e6259b4edd336a9d1e77c54da3cb22cbb25b4e747b4f37ff1c9896234be8e34d0d2536db4e9b72e7127126e993ff03182afb0f0dbc2b4ff590e5194933a556f4c93866a"
                   "de236cab405e0b5f75d4c92ccf8f2d0d690bee78321b5d0a813bf382bda32710fe8982f07fd6362c0d47fb623bf5360abd1b1d7f326ff0b65e92a75944c9c55891b5ad039da8c60ae03a584b98f48cafa9bf06ac70dc54b01c05a615e01a6102b2124cf212ae930bf0be4d4bfbad6f24922f0b79d64265763a472592df");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "a526a9fe9096013c48593662c6562332d003f379152bdaee79ee035eae09c9060457294182987763357251e4c8b5d2c155e3071f7bf0a731b146fe5dd90c2b9a19a88b1de6827534610b2872e7ff010ce3e7f283e7e12f83fca32a0c36bec2cf8b20b7dbe4dd6278752fa3534abb1adc23159aed8287daac020e6eb2eaa6f66d");
        bigint_t S("c76a2cbbc47fb190d4573eaff8ba3015adafc543f96265178048b76e8735f4a478c764e49251958d8686fc4a814ad317e6202e44e365027892db59988aa6b3e700d18dc648a44b01bb267cd3ee637a633ebdb6cda742564def52988d6e23de4f851aa4bca4f23fdf1de705c8588256a8e15e02b855321fd1bbf543f560e0c64f3a"
                   "297d384fcf6008c75573830a5fd8264d9b8990d7eba392d27fcb41d70f8ba18d48938a899a1a27ce2aa860ca7bff3cc412ba871b05c1461aac8c3abe9c3ee54b44babe0968538c4e324536f159c3a3e16010a9314c747f1ae325f0dfeef5c603012adc0ffc5f718120c229fd462c6cdb9dde8022f274285f86d44bd6eafaae4877"
                   "274ae417efb1bff6ff781ca1ce8e4ca5e0878565ffe91c2cf217582d75fa66b994615e7fc2eebea6668e2f4b2361c6fc3c14196f76cfe6bbca6678c7bd011ec8bbe1881f553938f0a5a8596a59eee88600b6cebfa2ab9e8fce27208d0a5f528f035fd9eb578dc6a5e4aea2cc6b1b5376cc914273656f7f268b695d02ccca6075b2"
                   "d95682b78895a4f9967d6a4e0a52a84a74f2d0df95380739187a534e3f7cb9bbd4331636306e83b8c87585987934d4ea7e96ed35df8ced393940b2bf6a31ff1c972409e80078f5dec8a56616e17151c60e5eef2de57a701e41fa0fde22d18fd5ec5d7e771bf9dc123aa847413470d1cf8cf1433fb7f3c7fe90c351bdd1");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "9e7f3f09fdda085e2ef1fe08ca900f4ecd4bcb90e3b7c622e2da3e5b97dffee8c50e82666bfc5c3146ffd77697e5d99026e60e9187d6421e9ca00f815befb9f9d12e565467b332c0653771dfb48af619e88484367e3f232c6183c635b3822a25cbc601fb7a6750b69381166aaae52e921b0f76a84f5931f33a6eaaa128001b15");
        bigint_t S("ba3f84960eaa90b68006c74f70dfd6ad8c529399cbc6b4f101b16ab90278962539dc415f62d26c2899e39d34d2770e53747d816e3ffde2c4821737240d73a429e832e0464b05099437704f9c1a50e80c66dfcc0cef4dfc810eb700d559818af80ed6aea540aa917b6a83e163ce461298ad853dc2e6b0127e16b71acb6278511cd1"
                   "7b2c9320dff8e7f3c18e64cabbda7e1bd25dc1aa4972017a24e0cb2c5502f2f5df339a831f3386b54c5c8c8d5ac70e17c32c8a56dc927771e9cfd63413bd8fecc08cd3bb06bfe5233c1dc8640c4c1a22a5918617af4573d079452a4ac053509f763ce3e2d1b1ed3fd37f47a115318c16623869f299ddaa63c132a7b30fe8ffe060"
                   "45efd70ed825adf3ec8e0a4917949a9b17e41cb1e375678d4be9151262920fdef1d75a95f9e7ca0779025bcd5b4fcc0aaec1b50275ed0ef544adbabc90d3ba774cfde6f5e3b8477fce50130f6352f31e4708bc146655670d3c7a0812b21d8cb79da3461c60f83bb8131ab7c1325cce5484fdf362721fde71ca50452c0f64075009"
                   "3fcfdee46db962e89a5e3b35a9c8a5b533f02ad193de622183a91161e06fd71ddf60ee0798406a19e464ee1f5043e8cad26c7e01be6c65ed3e87d4cd7226dd378bf150fb15869269d5b1d73d29b9683386d77da37a6855a76807e1b32e9a02f3b4cda2c1f88d0cd75c47d9dee84320643e0012aabe8be925f093a5a028");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "4d3179b3fea6c9ffb10c574c1b15a56fd3cf1294616e9624984cb2ed272ea77d6ac9e7ce8685c929b4808044ddd64eae6820f56c503a6fe5794adefb44fdc67b61147ab7e420d4719b8929fb9fd57c9d2a27f237a80686271160fe6d75cf1e0dd91a1bb541ff53a00e303aba8a9b9805c2eef23b945b45b50899fb8be42b17fd");
        bigint_t S("26dba345f2b1a20d82a6c19576ec3c2c4811b23234e28d3cc8a7ac0e930c953c23860a57d5f4457de6c7a3293500a11cff78bc86d4ea6c50b7ba00080259527a9d0fefd6048c9d85aea4b861d05c4c28deab10ba2655b1775092c44c8f83f9f58eaadf29e8658df83ab14057e77d31a32cc140d4774940ad11040b4f00f4262af1"
                   "de68bcde0127737ffec92915a0e50d04af7ecc083862235fcecde903d472c58fcdbb8ac4d50ef34aaa9ecda0772390a2ae47cef24563531d56df485160fd9bc158b1596f47aee8470d9b19e6991de3c1b022f0a9dde7853dc4ec9a816ec5410b5b1acbd220ef9e4424092bf14caac0e271922555b93cb0b0e2ce07f37995f6da0a"
                   "44cf56da3989b02d94fdee7d0be140b8ed25a9cb3900c501aa5567580e95f2e9ec611c706448bc868bc7cce31449672b8ec13e2d6b2ebb42f1421877ee371248ba220fd76d22a691dc1ebd84a21ff9d538fac9461db5ff404cbe2bc6b27dbda9d0a8b7ccceb6af1421a4d4601f1964cd178ecb212e551bf39c42dac015e596cb3e"
                   "0a51bfe9652783cd9930aad1309d220e31af6cbb5a15ee9983ef09ced622514508cc5bbf63bfd732e99b1ab23575ebf27f9a6d227758c9ec4881bea44159a53cd4e1420cbf743f648303330332867c10c08c159b68272fd4b1d69967e279c90d0fe19c0702d26617e8ab1447ff60570513aec5e0096082df666cdecd70");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "25388d8e792160b27e0713a7ac8d9048cb6e744613115a62915e5ce0ccd1d738a9cc6f0e8c325364c031c20f195fcf67d7096143cec77d25c454c8b11b9d4764b0186a820257495097dd2d849b3052facb30eba891a667d47020f384a3fd403701c486d224bac9ddcab35bef7b28a2676d8a1b5da4d69fd95d6ae542ad174919");
        bigint_t S("a19707efc8042dd0cb5d3d2cb862a93bb9799ec03c07d10a1d694bea364b53bb1766e48a1f25a22e9f7cc9b6d103db0dd1560b718710094b69ff61e9433e4ae1039600895f471750925b13e59f9aa394f8fa6a4d7505884ef6028f687cff7ad9d6a5bc9a788a7a43067b829496c54180fe0de8d7ce03edf4d300d07dfa1afe266d"
                   "20924839ea636651124d5c0ac0aa9bd026d017c7138ebff17b6c9d0b1583aa6943b4e96cf1912ea93693ec9fbcf5cee6edf081bf7a0daa0acd185b645c76ac2c38d26132631ddd2b3ef5326dd4e6298895a0e1d096bf277b25ca739712c166cc4119e29bea72b2e87b5ab0e13b170007a3a440225164ff2fc7d1f0da655ca19c8b"
                   "088d2f438067992d591d13a35acf9c54fbef1eb45528517d87b76f3b180178ff94368198f02d3a0f210dad5d4c91124ca5f5e7753cf65245016f51e22deca3087b98b00ed3801473d349af290cd93498b2b89ff06f26a9b92f94e2cdeb7643eafd20ffce2c4338515188b73ac2da53ffa4c11eb37205232209f13bd4f0c9f51694"
                   "c6722eb2fdedd70b4cdd4a469e9a16717fa56f3673da3d408a89dff532dd0e4eefac9707f18a058615e45594e727c25829bc4253f6b627aea39d4fde65a24d40d572bf8d013479d56d67d1cb66fe8b0b73e0e5b3f8fc7e5383ecc5d551eef6e7cece73775cb111ab67861fc11221b08218f772d29ed09aa4b0f3947ed6");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "24c68bada498647a77b0a751533fd08eaf312f14770d3c4a031d2a634ad0bfe3535c625eeda77683a068e65ae38901ca8fd1895e94bf0649ebb960a7cc66ad052058482439ee23fac1785efaf09d189502fa8501e90c40393f50e3457ea8d6bb73016486f03314e6c22836b8e241bcf8f0f54b84dcbc9222e72bdbc5ca60df0c");
        bigint_t S("a60b21235ab86e6afd0a72ab6cc5314c006cf8f08dcfd37ef1f9755848094b63a824b3679caeff188a349b18f251c87f369d7da9f11bb9e19b1edad3e3fceb7d931fdad46f54c42f8c06c17def94f9619a359b01228c8b9018c301ea4d497c42e0a28c413595fe446e6ca241d226187308dbac5c25506dc4c031af3ebc00fa958f"
                   "697908e41a6260a18c3d19c078a78d80541e545bb3ec67bd0eb45c66dbafb9f4aefe6d8cd991610444f56272325924095f8154bc251c4e6e0673e5f27031543c01df0b39eedf49f1866222785850a350607124dafde1fb94f19613fb0f66df886912994961dd02b41a27f08b67bd37dbd5bdd7ff68e51fb3876ea605c447248072"
                   "e4b410d977e421dee2c5937d9f09cd1c6c5c688dfc48ce44f3c0bb238650ab5c0a7c4cd852d9d5108ad8497a1cdd083d80797b5ef87af61d8d6ba0db9c1b610b0d863e9dff5a310b6d1fb1f7ef40b18c070fd23c6c7d11b1a6f8729271fc9e9d08ac5f2f7d8863e9c6f51444b99be4d2053ca3aff15a3a6d2b80a8c682ba1ccb9f"
                   "ecf97ddc7ef1993c38d5244f1591700ac15b8586ecf23033a86b90d4c6a086142643cad66b402c54438fc7763c3ab10324f3b513bd53acc217febcdb57a496cffe484ae12f1c59f7ce0e40f80d5f98d1267a8164f570ba0e841e32191e9ddc015d1342bac61101f26aaa2c47dbf9d2d626a1af53bc58665d6e0c5b6285");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "83b158c3906eb04a8c0a71879004ae9465d6865d527a9e49312dcf1dc8731b1d575be6746e0462d501604c63be38d6ce9e70e4bfbb8d7a77617475b367747ad2c0872d394dd5a3f7f57a19a5f889e6908051ae1b7adfd95ea3c44313e61835b3e5be95dcd6ea59f690e14116e500c62233004e373678a14720a3a007ec643508");
        bigint_t S("3d2a6a4dfa224779f30b267a78c22ca4bf08ce92e07ad3c1a46ea78bc585af1b2c9c8ad227cd0cb48d483c75500bf14e965cc4b62b1bb00d4b562a8de5404ca07be7e79d29f1489a4bff67530138cddb93333cca6e9c96ce1c6c0da801da3608110140955a96b22b7407067515c8e9a17010e5dff29963b3edad6fca1b1b12a88c"
                   "fdb8b42fff9c57db14a9e334f5ca2204b16aa79caeb817a7ed2efe306120f799e01448a20fd8e6a8b759708c858d293f0dd23bd94f51f48c1f7edb0f1cfba3704acca4086b7c53c3051f424893f2ea50cc5168460ea8f4e01121a868eab15394e017a223adf68e6dbde880b14770d4bb6a1bb1bb10b901460856f94808e4b42769"
                   "239ce81e535e4d59db02a64c3e22007b1021f26ace4abeca666f111c8baac1847af881452ae0c20453759195fe663f054fb0b5cf8a3878e3b5f3b563e09386e5b1f7dbec53340b713317759b4ed203be9bb36fefaad13add45570c52afceaebb5556851a59dd04420b39cd87f1879563980104e3b5a25293bf96352c3d182d667a"
                   "fb94d30056a609291efc312ebca1b53208bd648ebd09da08ece61ef50f581dfd4932acc6beef1f6da4687a65920a472371074abbc109d424ce4ef39c0d5304561c766e90420797b00e5a6c57251e6d1f6e93eb99c2079e55a6e4e32d8bde20ed7b0b75b70aee520b72bf667d3ca33ea70c8f82b2358ddd610f798df2a3");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "b1fbda789a339b4163acf22b0db30e83449616a091050368d0beb016e56a343aabc16f002df5e3b6194ec5897111548f84a76a0b1dfb51ecfa24d7967f44a5e5acb0cc6f4d51a10a704acb8f1cda9add40849b20ea6e6953b908c50395ede9569d0e1a4d610d7d4652e72bc577d687736176d0676026f2a5f7b1a2e48e2ea995");
        bigint_t S("a277641fc48ab328df9162d39289280bbc0adb4b770e856e3123e970063e2a2ea6c8e1e457c632b21e63dea5439a84b911590ec34cd998b889640a26428e6a02ba2e72d2bc27c2e78ee039f6df22bef638ea1af5a9ccb89be8ada7c12cb5620fef079d24ee84b036be7a5fe3d178bbdd502e3484ab404e31c89e8440a6d92f9fa9"
                   "5dd8b456c19af96b9dd999266217d04bbbe6a170f8faf1f7a384694bc6f664f5bd21d4f40f0c7c329c5ed45e19d5c5445556daa2c96e756f5368eefbd3a39f104e3d2e1fbc72556f0a438c565aaaef93624ffbdcf96c5bd60794c66da15de69aa5024991e35685b20e0aa118fe09418907c792cbdb6793ea9f04bb2a8175674f82"
                   "9baad7a793e0ceca9554c53bcf6a364dca1df3e89bc1b093067e6e7582fcf8aa14e79bcad5182a5ee49efedea4a9e40e0a99602770a2809d284ca803e476af0c34af4d7ac45ba5d38be6aaa161ce0765476187b1566c1d23788cf6df8ef4384781d3bf53c336b2924f8f9a4fea771cf6f0caf4a3b538371c010c12b9db0e075537"
                   "ff30910310e8d0617b16e40f5a010c269c12c92d160d01b36740780a7514bf75aa31fcf7d7c212cd7efb367d052648582dbfc74bd7b570f26cc462fb8c26d9cc465e87958ca30ff4a916e436cfe61b6900fecf0e1f1b4b342782cabf97479d554ee5fbec96549c8d0a79adec4c33847fef05426fdf45f563fc0c21fa5a");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA256_RSA_PKCS_4096)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(4096 / 8);

        rsassa_pkcs1<sha256>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 4096);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha256>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 4096);
        EXPECT_EQ(f, true);
    };

    bigint_t n("d25c8f8c97b7e5a6a98de78f19ef18077cf952e64622e4142e6ba5c8ee265ea9b34b9e5cb2c1ac0307547faa70126229b32085988c0974a03198c89f8305be4bdc445ee577c60b7a7fdfd5cf7283f0a1d0f2d65b1ba2fbfb76c5039ee06a944d40ae3843c9a4d60571db4cd16e28ce5b9fb9fa83fa3cf8cfc1e87103612568ecdb1a5c"
               "50fbd0911b645983b629ba72bc2d228207b49e00bdae1b906aa15ea2d23f8c0e124c9e7de1b3cd3b9bc6a017c4de3f1d6548dacad6d4abd5f4e179a2a5e782fb8f818cf8f307f84f7f3bcd81059e73192036aca02f805070eded8b5de01037f2b34b41e86bb8c44760492cde4bc3b080d5b0624bfa1002bd6a75d0513b07c354e8edf8"
               "a6f57dc14290eb8850659788098b9971fc0492c3ced7c3478689444e771c9434fb624eb4243f740c3ef9c3f3f084bfb59f44a46a1d3b1438e661ba3ba08d525255224816348b4d696475ccac3871fc7836385f7078412cc58e36747d248bb1ab123fa33256c10d1102f77a60643f4247ac1cdfdf4d8d51deea80795964f801f97c4f95"
               "5eb181136bb39abecd3549541ccc9d29e34a63241a4548e836c4d2159968596130a222608ae0b09d9ed30cbf04d83083457f60e8a686a452baa2b08c177ade1a3cbe7bc0d6ed846c24c4ad5bcaff3817fdf3381e64c39c18dd4f54eb82fc23d3139843228489399c156565df75f3808cbdfaa53594bac5");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("28a6fc92c5eb22f54f44fe1fd87a2d219d542d6214e1bf43eb28a0743119b727825fb5ade259ad1d34b86ea7885af51f6bdf7811dfd61ff4a941065c551cb206c4fd6d7d58e52be8b37795412eb732b482c5bb0906513bf2ffa3d92c45e032e2af023c1b55878658ad051d5647bc990c79d49e060d40e530791fab5f64d1aef23ef15c"
               "2fc64a2b10a916e76e37a8316e0e26e20c0d2a8fa81852795931f030f50294d54f52013b007817cc52315354e495b28ac2bd0e932e6b10fe1b01cbd8d3c8e6b628a8778ed0a094ab004295573eb7bd3478d6380c08c1fa49a8a1445292ef6f707697af97f3e154051b256907588aebbc2a0f991f67a6da70b2772c17d63fd8d12b94ab"
               "4e0faf9330a4b4bfdcc6d9137262cff60ec003e61182f201bd07c7d3acad67cf2e780e29a11238785d4802ffa8136bfb4d3558b48e8cb77bd63ffd9bbeac0f51d297f009a7b603a388603c59d7d68a02e2e027bc7249177b185e567b7a092986e8008101d4f58e917c5e9ecef56d5acc00767e0d74b49c2d3bd29ca290e8de04f6e161"
               "d5e48af3253a1710d1f5b7a50f888a1417b12c221ff25d9b106128829894cb16eed84333a96abb2ac310b4a72bd207ead3dc2560939307a74f3ba3b8aea576cd846ca963cf337fb41b506bd2320036e3b469af5a1db5a2d7419dfc762441da11275c8825d96d4ce2703a4e183fd20d4a9fb85da6a07b8d");

    {
        bigint_t Msg(
            "8b90a79fec955a7db26ba6737d3ed0455603d09b95c454f8013808eb3a97158954ace71ac52c341381463a45637b4853cfb07f699682da2b19d96cecb2c0e9214f74681560ba67def476108f1e7abbaf9e9cb3eabd0b3d0779e3d61ae2febdfbc40eb1f686e8fcc45223b0ba77e231410beef386d4088573697dcfdd94fa98ff");
        bigint_t S("d058fc5d239b0f975fe10f3c3a6f36b0e9be32c9bd2e5c81d10d38c6d462bf08d70b1d8ce85cecbefa18f52672c9de7b152c93a89436c0eb5ede1aa78b17d07f3eecb628be117a23b5be3d5bdc48b6d50b864f4899129c3e917201963e063ed6ea428cf92b4ce358dcdde32f18e768edbf40c43caced67cdf97ca1133f8ba158b9"
                   "b050ebff5bde22752b464e5740c8c6d990c44b291ddf652317a336ccef6e0e8a5f0811319c6d213fbf44ba637f8f32341eac526ea90638378956bbab749d75148d6a408b4f6c8974921bcd2274be693732e1ae3083d17fdc89653e4c22e846d32f9c18853ff03c7f3cc176175ebcccebcb760d995ab3f3c557b2b36b6363822357"
                   "29fc1dfbd88fa9c766daa351e0b0a4ae0d8ed63a01a45640361a0bb4056aae074e8ddf096e50bb2f49b64b20dfd475a1c5fd3860e5283bcf3dfabb952c201c0d7fecf79595267bd455f825b67f597238793b069512b65a5cf79a2fa6d2680e8004af7fbf65efdcf5912fc2d321c7b2bc969548c3da960cecd94631f75456d234ca"
                   "3812a2ccad439a292dfde9fc98c1bc3e17262fdb0e167130fa8935efc84fab777d757f0dcebc76e85c8a4988bba63348af0c383a37b434ab600f80f295053e07cccc84575f8292645c2dbbf60616efa473767d47086710eb1fc2362e9784d07c314ba2d48a9ec4616e256ccc9dfb66d5444f5d0ede21c7bcf67c514683");
    }

    {
        bigint_t Msg(
            "7c5b1ab78792af899cf3edf237da4adea82ab4d4664a831ea76dac2b2d17373163ddf316a86c6891ebcc0d8d78e6bbce114462d64119fb0aed81bd112c09947d5972cfad66b218f1723caaf09e81be471db746738bd9def180ef00a3648f777685a41642666ea9f25bca2ce452b43b93f40aca73ed92161c873fdbab7eb0c865");
        bigint_t S("8f8a6ec7cdf51e4cd4ff008e6e0086ee3925c1d1ccb7d5381f24ed7f5ba0bb466321b4ad6eb0e0e8064a73bc748f811b239228966bc3b6edffcc7ef669a6efaf80bb7391697533caaf68a78b15206747b4c26d76f2ce7d5df098d66ad9346aecfacb13b9e7ea32d5bc78ba9296f9319269ad382e4017960b2e2e448ddcb5964faa"
                   "51395cd45a0738792861ea78791f36a5f353c6a7ccbf50b8dacdbfabc031806d603545b077b948f5e6a51269d8ced53257fbd2c9522184888f4dca8e45c41ab4c0100e0f0170a7fd52785e8b2d050e70735bfc5816733aea65de86dd4f385a208b8002b0e9463d3439c4424e0c828e66ed98476637d8ed82978770313a7f5b66cb"
                   "a976b9640e2c22805c1132dcfca2df0bfa54679c2d5ec6c5dd28e3438122d6fccb6282b9ee798357d4e3ae6b8398e6f2f067b8095b09f1cab886d8d4b2e34610a6a36cee34b1b0be034bac400f9a3daa6fb43a01a11b16ee98c74c55c37bee765fd5d5095b75b5caded753e3d51873ae74df873d2d837f8fc61a27e2f73413ee26"
                   "2e214ae27e80bab78ae779cf1eb7c71a4634068040fe37d32250aaa0ee061a1b9b095ecb78f57bac7258577a4ed86ef98023e2a88d33fdc07a2d14aee48f9ecf9983eed38b2f11268baf0c7382c3b02a8a2d41729b8598b8ae74eac63530c04420a03df1d973e594af5eb952296ac49ceb4b1a61a5aed094d72c251aa4");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "6a41d3cb08570bfd2a07f683fd9c4b8ffc786b49bfa1dc71137dfa4550dae9a5e731ce753b2c3f43b265f8290b71cff5f21dea88d1f72f7cea11fc0aa301182a202ceafb1745e966e01054fecc6c232bb8903d88e295eb89fb358d617dd28c233eb98f2746a9f2afdd8f74cae9942f797759ce119881fae06e71151386532bd7");
        bigint_t S("c232d219f65b663d5f7c64bc0f0f11c5264fdd1e71917bb1b25a37c505c649da3bb8a3a164788a783ab75f0eb1f22ed0f67877faf59d0e89bc731ae4a66b58ff149091066e5015e0c438e98cb5b270949ddba2abdfd3f38b443f0b82a3b4132b8b9ed677a2653b30e0cf381f87b2ae3390a898341b0bf29640a22b04fa94b37ffa"
                   "6df62c352c8a47b04d35ed0d6f99c0fc9ea340dab3a84e6f653627605867b4b1dbd3d70d6ea218e9dff57020ef44e1afe6b665967de08a447811e98fcfc0b60a513ff18a8e03df63303ed05245d483887da73be1c6325a56a6d0309311ed2efc9c4ae46f6e1bf4ba243bb23b894195480379b20ce0287f4aa578f1e9d8d10ad6cf"
                   "50de3d6b2e32c60305b4e33fed63fef8b9f03eaa5399e543a2675100a63b7b14b2d1eb3343556f84da005b5671ca09c9cc6b2d1c4928152b3cc67457b9a35beb82f1fe7e9fed1770b343c68a5ba1b6d6c839995a8e6c5da60c6039511467b05a8275eb3d32f39a72e00a17b2b094ffc5994aedbd80f1074e224f84dad99164a44b"
                   "a911b7d7feb685411088129400f2f10adc606628bdc372b48eb81c4d5ae274fc9fa47427c2171628238edd00f1c897ceb0273e4fd539c8b1b5c43ff20150ae78a27b35992554cd62f88f66fb386514634ff419db192b61d7cde12c2a5a086fbfe4b2e7f49a206093be776e3e7d16d88743fa486f9224d04e5a8e5bc598");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "15c24ee3a9398f6e5e57c07a9eadbfbc583e11f85285d421c1baf4c21752a29c1cc50290409324c02f8936f2a740f83586e870c62843de34eff4f2c323fa3d0efd78e6f1feb79ea6856dd52ea1402ef3248631abf94f417fb7a5cd6d7512caebe16360cd8cace368888c06f835b24b3f58a0be6f6dafe34a3f54ed88b5a13079");
        bigint_t S("af4e570f2fd101a04035c61d550b6aae1afd7328ecafa0f73d3a81272e791eaddf0801c50aa2bf18e20323aff9fe8d44d32310586d66f6b88ff85f2a6e2ce84f41979ddf34620d097074d5fc8c5ddbcae26a2a027441ea508edbfe44e9e44d7b101a3600570da7409e0f62570b4d690ba7ae82a9af678d210d4c70b29b6526d35e"
                   "e1bd528def9f63b2fdc6993dea732b8779cc6a1dda0a89ef30022486571f4b6235743626c99edbfed13b32122dd51ec45bedb2e876599ebca0553853e6ee32a818ff6bac03d4b774ee01d879f22b7f1f308b11e9b9e6e60717ee22bfd406a4005feb488bfd6d24cd18038848b5badf580c9fd1dd8b930823332c5e7ce85ccd1aa1"
                   "2345489d843c325979cf82147daefaae0d7bd5c251057ad0536ca43c2021baa421ea3fee0c0376d55e89136e95c713b182f8893bfc47f0b86a2fda14664c4e3063c05ce43ddd0b83a0ae792e07e3a7ad222f9975be8fc51b610ea3b127b59f7d88f43b45b9aafbae10f038f2ab22bff08b622997d34b12d69f5768352141d5560d"
                   "915c08e216dbd93614410db1335d7c03ed2383ea467aa37c2161724e2c1d2f96bf082c9feee522edd849f49ce17afdfdaa16d2600d70484c26d17d1eaf1231afef056b650c8ddfafdcc93aba5538d2b032f477ba4043341a4db3b0c6a57aab586590eccaa8c349a68662c30b9ee727429c3ac4164190cf53cdddf78c91");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d10062df2fd6492a8e3ec9c1bb1a4b3e7d69de41826eceb59a0f7855237b5c6ec40f74e65f7bc57d402033539bbd99ab354b19ee6125b1f32e6c0f7ddfaaa7059bb502ba66d759c7502b62d4ec4a6acf0427a4ec8b38c1ead3d7c09901e70899bdbab4642e322e8e2527ff4c9809e53453c72643eaef2524f21633af707aa7b1");
        bigint_t S("74805c6bb30998ca55ea3d61408ae96c4ca87219b22c4d32c653e67d4b96f641ce19f574712e18f17a4ee8c022ab7ede8e0a2e974e5dcf81e82eae3d3e935aad88f3f51a9d7a3136a7baf05cbe3c30cbcba28e7ffccbae8a607799f8efad1abc1ff876f9cb3becc475fd6ac8fddad9037c7cebf5a47b09688f51a51b48f087d6e7"
                   "dcc6dae5584c535a07ec0022aa316d7e39272544baf17968a9951ea9c22474764d6e7d6a622ef5683f5ee47af6735744fb5908a9cfae7825875afac7686ffab3c42bcef73f850f8749c140c679e809ab1ed407aeff2853b03afd594d8521bdb55dce91f5340077322d81c92e6ebdd8be0ca001b69b444dc54ff4120d7f1f0bf561"
                   "6061f7e3a99d6ac42a2f5e1b73d03c5ddee04740d793ae35b7fdac52154cd9dd0e78d1bcb5935be84c7104763f00c2b9a8bfee9d7f35ce2e62be17f56f9ac0d3645abfb379edfe4f295f2c5cd56412b895b204329c186f369330cb9645c5c50eaab1523d0296fc8325b3437898f7d3ce6e40a487f78a7e32a77323ea478201e5e1"
                   "823ab899189e96e0319346f7262b524e0fc2cb949f6a275d69aff5574096c5f81c9c3129b53bb8a9836e7d788b51515913590cf422d7663545d192f03770d9eb9c838ebd6c141765a91135473c8915e2913374bf774d61b1143f10756a0ab61be6c200bf2a7096e7bbb96334b49b8de2544a584ed988e404910c4c059f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "be8d5a3415984f55ee920a683297caf84840540e2a69a10dbf38d4db655b6f41cc90c5950b6bb396ed548b978ccf51e022002a511b6e11486f07ccc52e641ecc5d4f3f0b008289bd78b274ce0fccc440d27f01cd9ffa28ce7ef7a05b34f86fb416d0aedf1c663a9336b6ad55e3188de1ffc5c3cbdc974e66951038e59f9ec9c0");
        bigint_t S("cec601dd36bbf049cde7b7f2bad094aa17e0bc8858ac383d26412e0e17cf9786641fed6e221064ebdd2ba50fd2016b0b496f29701a6e5f1eaaf7fd2e0c9e7e87b60273bc94523fd8ada4b76f4ca8bc72430af4dee31a32c6f0e8af0d0e3cc5e87000ca77da5992e4edbfc0730055a6519e62ea8b1f1576dad86c389adab82f11d9"
                   "c99ab68d102552a159181294f69750fdbbabbd95984db70e43a191bc20a35c68a2a613abda5497e4d31eb36638e845f4d4f4c86d119ac0eb870c095449802ed3137cec010df5f9380c2e19016b8bbccd117f6463be57122ce393ef6614f58eac82db37ffe1e7d4713c8cfd0d2b962823207efe73d93b6f5af0121b0e85b62be68e"
                   "d498d0f574dc4bef93a33315d9a2ab0ea7017f4e95f56fdd89bc85f1cba96618114afbd3a1ffef32159e9fe0272c4e07e01c76fd23159a74ac715816a64531120d3a777e5ba48e303b4721540a55ed98c4f745c4bb1d414c94094e2be21db69a76f3206fd15f730e55be47e06606909b8bd95d63d32273171151630f5f184c767b"
                   "5bd42c061e461d861a3aa6d028102757bc1136b4694a4216dd8fb6d8740192202e0fcf09d8e703e68278f4f0aafe1996409b787825343e1203946837c9c21fd6837d81e104a1be6034a372d0ded0fe340d26e07ab5633da3c8e12f9b4ed31093198a5511cf5b3c0dea04b564299737bca0b7bb94e7e5a0ab14d1238c14");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "d7d91606f1ecf5576c6fd5c1528f398866590cb912da386aa1857443aed55d3edc33c9aac81958763c784caca6579a3cc8bd40fbb0d2daebeb4170bdf6e09394f593a80ca76e837b9a1938779b792d98718c747ecb955816767a361ad36a8fd789c25a3377329feeed1c41281b3c1c24c98e4f4b496cdb74aaf76e622fb9798e");
        bigint_t S("718082723f43869432433c910c2d400adaaaa40b9b3b251480a4f01dd001c31e93166aa8a45476c00c10188a538f8a533a5b601a2880957aed4c4bd69f68c08a3b86524cedb6fcb9bb46509ba39249953671dc6d1f9f04f219802be5c106d519573e16fd596c6734e173c524ab4d2304c73d9fb78f3496e8bb42f0bf0a27555231"
                   "2396b9b893b5fe3b5ef1c9fa8c22d1b9863ff0c0b4221a7ba91c1849ed2d571e68c5e2ebf1fc8942aff57b9fd78b1491c615717f1ec42d9ec0fe84100bbafa54e7d9f0ee48f18c5fe2dec2d02bc789772175d5c6f1fcb56588675f02356e8366cfeffb4e560e1e6d1867e93f02d4e68cdefdaca4e1122fa9c9b52b90286aab9c2d"
                   "1357257870aa507bb7f94eecc3fb36319c3e7e34620a74c305ff1ab0cafc0198ff4e19f8c8cd5a1c6faded98bc2167ea92a3af157b9cdb4546ee3c2739b8dce011d3da83e9e13652755e79bfc6e28c45906f327b78a7e7ab9d24256cd036280ee4ac8930a36bce28ce8f060510c0d116da8900b6064440f02624ea31376ba87eac"
                   "1562dbd61d1562448f3e720609ab614b477297a68d19d4f975b44bc402c4d86c53f3dc76052353ab228879be3b25f7acd93e52facdfb7f298886f0205ee8925ed45967dc12cc24677fae8f55f36a7b7715570b3f9e88b30e185fa68268d066075eee9f132526fb2cd50eaa75fbbc3e5660ba031db92dc7c4540020fa54");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "5e8e725d0c19527bb365f34977cdd39aee693fd8eb8dc7a79493eec5bbdb26346b0ffb9cc0ac8eebfcbe453ed8ebc10418b2106062f524d050b5840dc41e42327cc32d1fe62a4720358b7acab4d586265af23e129f4cf4e38f90324f1db2ab8bb93170b0259c67638455fab5b4950c48ddb031b44a9a1e2878aa5e556d9f4787");
        bigint_t S("53c551c730cda7be2812bb9e2e13d8c849833b421dbefaf3bc0afb90f0c80cfd45abc383a0f38525b93d4051851e48c6faaeb8a912ce4b07e4d8a9ad374d065f3219aa0af0d9411ffeaa90d55d22194b4be2807eb44f891624b80ca2acf050d00a6dee14cc7653908f5a788441b246b89c7c84453e858191c19531ae6740236367"
                   "28fdcdc005ce72d51fddc954d01b19b9b62a922f2542d81832216efb8b4b0f8d75b38712ca50b29c599ea852a5466a776b47da0f16d509682faab2177ab14c46b0723000c70e45b610ac7625db6da98db2ac596233cca2c2a311f2d4abfc256ec93ff2b90d97fc196f01f1b51274e795962c5dc18818f4e888fe0450f7eab13e32"
                   "9589c7b260a9970711ddc7fd990ab061179480c054fad2433e66009af9fe5b33f2c07217ca031f39235f5c1670881b51c5e335b5c5f79f24833c1e60e6de778502b0ceda9c882d835bb2cc40639303d8be61294bf6752211017e9c26fa6def5b9529a31bcb3473ab3f48bc86b5361c79873bc7173f2bcd726a0babcd5a92dcfb4c"
                   "aacb9a9a5f0707a28b7ea80546f529690115642bd8e60afdd652f145f51ce14bd005de2813d1584850c6ade075284156ecc301541d38aad9303cf9275f3b9efb93736150041b8c7201464af14c9488a2c157a952c858a4512862e106404fe3aef9e61f118942a6f37e1bf119bce0cf364dc99b513017d2111baf77a476");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "10130471dc383f0a7871acb6d18743a430312f61bc8974ed41bdd5142f07f5fbe6dfbcfa306ffca90f47553eba525616e208e49139e6e023351b04a811850ef816db21bc182c68804388fc0918fd08e8b2c52b4f4ebd1d240a199654ed566b6a2871fa609b57c9b49b9c6abff36fa24328ceca4302f67417471e52bfca645478");
        bigint_t S("636d07a32c64f2e3d3d2e23700206cc4c0e882c4154411f2aa54029fd33ff7bc060db61512a6338736b6c044a200bce79b70ed563811da93786da5b87083b6df7038e0ce49b791e35cc6fe36ffc517f4fce9ce55a2f926d66f8166e56c552faf485f0d0c241681f968d649df926fa83abd1fe017e19a470e562cfdb72551b5bfdf"
                   "94d3ec07c1cd95963d436a2b4440293d95cd340618a1f5e26abf85980ff46f9112a45fc09bbac286de98ed8206fd52875350b4f57ae764952bcdb0bc22e4263f173a58bed2bf5edab12a80969b7f4d0d3223c9799ecce4d8052a008f483c59f01ec28b34a9f8864375a9d9ab7d1939c1f9baadceac17136c3d54edb316a02d7c86"
                   "02e69fc35337a24926d9cd3a63b5b460c653c62c803a68d0b7244c8b542c7510203b794ba8b1478aad294c22c1cb82dbc2e1c46dccf1af4c024ec94b395c02d2d371386ae87e9d5cb0a3ef29aa66036216e07c571eb1f1ed5f18a1a4300e9ef7166877163eb49b680be0066ddaeec3477c7462c195c7707e745f9f511341851e81"
                   "2a760099a88590fd0922739018b9c0cfbf413912c7337fc5a5e617b5aec5eb2bfce56b6c040d337d52af7e0c2c34ed020e42c5a2ca802a51f04ec684826686911b0f81580520e397cab9c524d4a650bfb1213de0a425dfbbb27c824a919f9ace8e704c5743afc7f0c063b0407a9c78a2abd413adac3034d34396b2da53");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "eec08f01fb4c104de953caf6ae000a9fb42a1bfa3629c839eee81cc5ed3c6c33ca749874fe328d4b8f790e04ec06b56418e434dc0b3be10156697241055304cec9fd63df575985180b5d5746d158bd504586d36b19e27a8cb73b1023288a7e06358378abcc6a009ba5282c23249e2a4d8239810f28c799c15b897ed41e89e1c2");
        bigint_t S("946cdfecd27bb3b5447b66089fb08a17e840460fb34dfb55271908f811209193fcbe277f360b0c7f2a430adef93f1b30bd11ed2cca35da0c1444804eb9ef23b5753a52171cb3ab05e00dd853c18036e5dc268f52edb03de49ab9452b7c99f6b3dbb04b0e3292ff739e539885a2053523b83e93326e467c762c26fe4c44d6ea9c09"
                   "2e75130869cfa12f19cf8dd8177359c6caa663f33c43e44c58ae0a1fb7c8fdfa7e58ebe2c7807405022a9a1b7472fda86491e4f5fdcc87d71bf2b8cdcb6df183bf71371df5b3489fbae38d6a697a91dd5425d88555353ca65307ebebf8464d9dcd32fdf88eb29531d5a568edea1792da2d17150954bdeabfb3d0fe8f3b9bb7cafc"
                   "48d4431526ac6e7d0bed958f2d2ef43a709bec9758398eb53a371f300a45b147680eb492383f44252ddcb62691b159532b3808c90c094c69a9f7130f4ba35e8a7cfb376b2a46012f32b643a3bfb62a696ec955183b4f857dfdbde81ec19c0c9ebd8659629637220b03e108b8c4efe771f0fc8625590d59101049c176dba62032f2"
                   "5115e0557d84d94b343a008e7d4211ad23fbd5d07eb3f58f09ee3f4605fdbc124677c53f4f3c93faa801139aa81a122e2cdd35215efc8046f475a11c51a124c1cce680fbf7d479ccb5236609bdbc45ecea5dc18c285d181a686655e32dd47793d84d7645d89f054e1cd8c1b06504918dbdd469d29a0222aee92cf2d44c");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA384_RSA_PKCS_4096)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(4096 / 8);

        rsassa_pkcs1<sha384>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 4096);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha384>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 4096);
        EXPECT_EQ(f, true);
    };

    bigint_t n("d25c8f8c97b7e5a6a98de78f19ef18077cf952e64622e4142e6ba5c8ee265ea9b34b9e5cb2c1ac0307547faa70126229b32085988c0974a03198c89f8305be4bdc445ee577c60b7a7fdfd5cf7283f0a1d0f2d65b1ba2fbfb76c5039ee06a944d40ae3843c9a4d60571db4cd16e28ce5b9fb9fa83fa3cf8cfc1e87103612568ecdb1a5c"
               "50fbd0911b645983b629ba72bc2d228207b49e00bdae1b906aa15ea2d23f8c0e124c9e7de1b3cd3b9bc6a017c4de3f1d6548dacad6d4abd5f4e179a2a5e782fb8f818cf8f307f84f7f3bcd81059e73192036aca02f805070eded8b5de01037f2b34b41e86bb8c44760492cde4bc3b080d5b0624bfa1002bd6a75d0513b07c354e8edf8"
               "a6f57dc14290eb8850659788098b9971fc0492c3ced7c3478689444e771c9434fb624eb4243f740c3ef9c3f3f084bfb59f44a46a1d3b1438e661ba3ba08d525255224816348b4d696475ccac3871fc7836385f7078412cc58e36747d248bb1ab123fa33256c10d1102f77a60643f4247ac1cdfdf4d8d51deea80795964f801f97c4f95"
               "5eb181136bb39abecd3549541ccc9d29e34a63241a4548e836c4d2159968596130a222608ae0b09d9ed30cbf04d83083457f60e8a686a452baa2b08c177ade1a3cbe7bc0d6ed846c24c4ad5bcaff3817fdf3381e64c39c18dd4f54eb82fc23d3139843228489399c156565df75f3808cbdfaa53594bac5");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("28a6fc92c5eb22f54f44fe1fd87a2d219d542d6214e1bf43eb28a0743119b727825fb5ade259ad1d34b86ea7885af51f6bdf7811dfd61ff4a941065c551cb206c4fd6d7d58e52be8b37795412eb732b482c5bb0906513bf2ffa3d92c45e032e2af023c1b55878658ad051d5647bc990c79d49e060d40e530791fab5f64d1aef23ef15c"
               "2fc64a2b10a916e76e37a8316e0e26e20c0d2a8fa81852795931f030f50294d54f52013b007817cc52315354e495b28ac2bd0e932e6b10fe1b01cbd8d3c8e6b628a8778ed0a094ab004295573eb7bd3478d6380c08c1fa49a8a1445292ef6f707697af97f3e154051b256907588aebbc2a0f991f67a6da70b2772c17d63fd8d12b94ab"
               "4e0faf9330a4b4bfdcc6d9137262cff60ec003e61182f201bd07c7d3acad67cf2e780e29a11238785d4802ffa8136bfb4d3558b48e8cb77bd63ffd9bbeac0f51d297f009a7b603a388603c59d7d68a02e2e027bc7249177b185e567b7a092986e8008101d4f58e917c5e9ecef56d5acc00767e0d74b49c2d3bd29ca290e8de04f6e161"
               "d5e48af3253a1710d1f5b7a50f888a1417b12c221ff25d9b106128829894cb16eed84333a96abb2ac310b4a72bd207ead3dc2560939307a74f3ba3b8aea576cd846ca963cf337fb41b506bd2320036e3b469af5a1db5a2d7419dfc762441da11275c8825d96d4ce2703a4e183fd20d4a9fb85da6a07b8d");

    {
        bigint_t Msg(
            "7247df014b08f109b778c850bf0763e7026bf056b595ed32e8e38df20b975dfeb0882709931716a1d7651c0492c391368c6a1d50446dd71b34ebaedd56890973e71faaa06bb2d8a5f1a3bd1298250ddb52f4e8aee84502843ff80177d91526d39e76306fddc17d3e8e9a7321da161effdd721a18aa05b4968f734c659b24fe45");
        bigint_t S("7f6726e4edfbc085332e7171c879954e35a34c223fbbda2cab55bd33f8f584e0ee722d294a79a334f33956110f04d8674e9a1f02bfd878ee0383381c8cd8997bb83b604c737220ae56e7875a3e65dca77ad47509b56a00a84058239721d7ca67eb03d2fb538bc37456a585c7491ff67dfc4bed1ac067efb6984246d69837c2b7a3"
                   "4dee6125fb4171b5732a23a9833251884280a3df5e219e6f84169d4b0b16236ebbcd68d3a94ab7378751030a2f0647dd61e0b26f075a5126b92b6029b8757596018e6f390fd9de0371a11c0d7ec3a62e2cf27fb020c1ad9874d7267debd953b90d75c0bc73964653f8ccfb3d80e951e6484d4ecd61a6a8885f4fea130cffc12dd4"
                   "058c9a8e58600c5fef8458e5f185186ecd0874341b23a1a0d2138b4894ffd2e0e1aac7c0a23023c9f6144256fc88d2028eeb6e5ecc2e6724c789ef59008276e25ff2591a4c7b9d379c1ee3eb452d8e76e9df75bd83404d93343f0777d68ac1b9a29cd6003eece08cf2213fef5669336a1cbb5e17af34e0bb2250971b7d4f845c5a"
                   "232361abb6acabce07031d8eace9861e8ae6af3b7cf6dad904ab65d288d27b482824cf9262b13527d38e04f302ec16559fdf05e198e9b2fea57cd2bdf8f6767ec138d6ad3fcab0888d4561efe41c2a1949e869a37b25b2e9c0307d7e7d4c822053e81d0a6a925d9cf8f1dee2d4ba78368abb4d8ca5d39164da55b47f5a");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "3f1ad56f3073857039717eecd2bf9000f5773db9cb3879cb8479ae86861c0305cb46dea9846fa7c603b5d09bd7761288391df97ba73ca2accedcc48aad11edf4f8d10eadba053a0a3fe1abee317106f80175dffb886f80afeffae6a6cc5d0d818d606e631a5fc5a6b69c17cd26d9f4ba1627fa22b8f99a3dae7c1fc22a2beb12");
        bigint_t S("1ee50009e32b7c4425299952250a26622b39a4e6b09645701025abaac727238a36137f8c719c62f5e54e26cb84533599d7acc86ecc7dc0e730a0fc3523607041e297504ae26a7b9b8457b84c6f47f1f207ac207509784ad70a2961ba1785453ca8628ce900fb26d3c5e81a6208d6947e79b1c07508ee131916af33f05053e4cb7a"
                   "51c2cdbbd6fd75bada9f1005e56fa85a41ca785db71e0ba0eeda40b5212757a46b84ba497bc6621455640659b5e9e251ff029acff10788b93809142f77210c0e6cbc51f750dfaede151ecbc2cc43a000ff4e7006b677a91d4138ba01acb23ed186f6c047b268854306b5bc7a42878793be830060e1f585329ed00f1ddfe69a56b9"
                   "08e71a24b8a3c4085ecbcb21c0039799264f4d7aac0b1432a02476d7648b2a109fd63424bfe3fb0da4a19c3010bff5230e2846cbd2d51f7f2066728412521f6841f9724d3e66c5e99a767d9f1cd2aa5cb6a6e8eae4b5644e2c169a98239e8c6f4f62b83847024dd61f5a2a3f856d25d35ee0a1baa3311c42a0f501cf3db261af32"
                   "27f46e75a6bf423d6a57f89b0ebec9c57ae717f02a85578b619275d14669323cf139ca9e8c37d647427fc9a970a5bc533d0b19018f95d8c9186945a827a8470a2bbed9c17e9f8141d64dbd5e8755ffd77f9287a17aa138c9c8b7697e95e3ba8d86cd27a3c989f06321e492d0e7dbd1f9308fab0befca1548be50a22b38");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "6e5f71b6e9fbb5ea92743004e2ebd344e8c6ba6349a654278599c5367b928828301659dfcc37521be5c6544900c5a89c551be65fd38b168a64e7169372813c86aae33b5800c04b79f32a84a812048b0bbfbb717f3331d82dc2fa585e60cb2190652fe1c88d7ce82b43068ac81f92c4b9ad0e236a707fe238263a523e4e577f6b");
        bigint_t S("47236d742e5305cca31ba94b07cd8f678ff6ba40cfb66d31e5e9305f309f4a5972b70b96694b5b652afbabe898efdebfc87596ea5569134d8a6bec9298e5d2cf71a30f57ac3f1a2155e163bd321fa6739a223ed300d92b177f266098068cfe88a27ded8f410bf9eb620b398e231c9b48b6fe0fd75bea2d80b3e23d54882c2fea63"
                   "befc699beb404edc29bd6d5a52348a39c98b927cb54afdcca99a27c9205ba3f9256e1678b2e67d3f64f321836947e9098b1f16694b840c2dd7e0973650208a43334541001d072f620d14f3ae76e2d6851b5bbc1b151642b080b9b413a3a7b751617a2e70dd4481ea19c128969771c939eda2de30511794028a484a3b6b193bc036"
                   "01f35a7a00d64f96f47ab9a84a43f770919013b395ffb54761ac64d48ed81609d2d2baae7cbb7efd19c67dbc5956c4636a7aa9e121d15244ce08fc4f73cc5b8e676beca4a8f7f81236c0984461f98ba3fe90e2bbf8f5cf4c6534358f8aea6f461335eb59c201f72836e127da4332d045b8e715e01fc8d6e8873115e3a75608a8c5"
                   "0fba1319121ee93417826c3df3edeaeabedcb62c22b757c708f502186367ca80d015d42932dde2b889e135012a7a4307d782c06ee7414e0a6188feec3f616b58647d783558eeae8f6851b4655f49387626ee6b9695759ace1ad889b753efb30c4f8af0a6cc492e02d1843a3e09ce3ab6554d9731e8c1e5d43ad3b71e7d");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "413973543769eefd2713c24e100cfd6c8c562905a4a2c6eee813f80bc0457c70fd8ee0c5b384f4ef4bbf7cedd4e027ff9ad1c50efa4fb44874a69b0b02bc5b04a85143984d858482c99be4be3ffcb6a6ad7f095cc1e72067efae22a1c00113506d0b0393ac768500ca2f638bfa6a90a9ede75eb6a273f19480b1f496a21a8a9c");
        bigint_t S("62b3d624aac87816b595c6da931a6caaeb9a329a96000c8cafaab7bfbfc3740718692bccbff955a0353c6eaca1093b8524d5f3c59e784da8d573dd151ce4a9ddf5f327925f4dc64e736929d8cd41bd20bfad57ac31b85271b8c5a96e9ceb02e0a009b24a2f6669117bd86a552f4dd24561a7f62d894ed96b4844b933f4e02f76e1"
                   "a6d98b6f1d5f3dba231ecd7cf0ac094d6b342886931177d76aff990036ec7dde988c1b20f471233c9fb0a38f417cdd8395c903706409ca96e215c4fb229053c86e2457a6c072a15649ac1c1ea6075a4bcd4dae3a1b238ae3d33cf178792c3d6bd56eda2a6ec4ab7130ce15e80399f77bb765719c1dcd50e4282f848beb3659a776"
                   "33ad10bf279456bcde1c040a003524435ba86a6eb3ca40cd1ad1fa7167193c584c1487b67ee9ed3f6f6c4aca561fa1328c21e5c177fc9877401fc762c52fe448aa06b4bc5ea6631140f9543b6bc45b25e4196af5e45829255217ae534bd36ef52ce09bced4923768f13c415c3eea41ab767e38ec6c65f482083d5c30dc45333599"
                   "cf4b332c675e3c5e0f16da059c195bffa5f87e1bccfeac1a7c58944a72e41e0b1ba2b978cb256d187e90435a570784bcc7680c0f791d4adf41eff3a87a05aa61238d0ad4abf710e3faa61ac76993f77e525f14112bf0db508b55f0e8c8de0d3c60d510dbebce13053f4ea207c3ef407ad4e353cbce07c73f932fd0693a");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "cb72fa190800bbd41a49acdda9dc6f5e085487de7687bc139d00c0f75fb0121ed2e02346896ab83016298faeccae45feca34775635d628c0c4006228822420b3cfae6c53a839c5992903c3ca509efe3404e92bc2563f88cf06ee7b3849eb75b5c9c41d69eed5abb75cbf9158bd1661aac05aeb4b6129a5e76cb2514e3c6dd9ca");
        bigint_t S("b61fcbd9060adb9b4882356a3a624864264a2130bebad9688fc2efed8e8383610b7e5cbe5091a604200cb0d4bc90b3ea87aa5f1738932afa68b82770ccc29f56bb0e905885821df1f55bba01f03e49ddb776e962e43951b2e4bfcaa33dfc8cfa5053282d60ebd2140a9e76a097ca646350fa417bdb09988d8c18fb1add9b204ae6"
                   "399a7885032335eed46769aee10bf901e81ee36d96ecdd8ca61561a1a28173170aa22462d54778c800a48835fb03d2029b790715ffcb1e8a126766e0320ac1e29fe03354a1e1e6605e2e32b27d2387d6eb35e15b92fe86812b829c24843a5778951888f9b82d62f2e980a8a763e076dca1efc044af51f184e8c7d4b0b2ccbb766e"
                   "a531d414f7a01c27f711f0ba1cf865bb9d923ada18eec68e25cd42a2fd1b0e9a776b5a3ac3780548b6c85751c7704ad3dddf74ce0cfcd583903923d5cf8194b85d98477b880bbf8a12b4c922b7bfb98244cb016420f6f666a406278c69785869c0cd3fce37edcbeb18b6e32cdbb648bde83a5f349d0a61df9aecf07926056d9f58"
                   "418c77eacbb538b472601a03b76c72d5c8fb588ff0ccc1d9fd926bd3f51e4989b1c120d4f0b6300491c81c5c106681c571329d0b6cc5190b35700f3937d2541752aaa4a31232a963da1ef4dac081d88817c634285824abcd84a1d7dcd88d0eb1035c9953272b4a9ed464962fbf04b289b7333f3345a178a9c2de62a6f8");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "cc2962fadce79e25ffd2230167e359685ad755293528fda3b13b6d9265db9a288ce635600946d38fa8234a3ba7575e3175239dd2b92087ae9678f2542d55ad67649a64f5e6d2c3056c65de6f3a12d1f1ec99a37e72b871d95a59c8634d0a5232ddadb5627e71b8b0d101a84429fbfacf48a331d669d9024e8448b668cde1d1ae");
        bigint_t S("2369100f007fb2f7c44511996094e3baf0b833c457a25cca52852da351895cc1aa149ba2a1c478133fdbde6bdd78934cbac9ad5f3536537f592b9be623b38ab91d72e157e9603e2dae2c05cb4e24de38db95a6f1c78bd25217acddd04639febd08b32e39b72615f372dd8a529422684f773f2764943582daaa924b26622fd89641"
                   "92a4ea4b76665ad7d8ca90b6f535171b4dd9d68d34fc86b62be6592630599f61eae9715804002b7061063f4d56a2e39c618ec0cf2e1af8fa06bab48af761e36374205c40d11dec239c180054ceca8979aa19f858409ea33b8e9c9d67647fd02f3c312220c41d40c086f7c4c3009e2c1c058d63a76ff5a885a2e990cd5dbbc24a0b"
                   "ad29a7e42b816910ee6ba5d4925965b45f1588facad78ce8df850f0e70ba83eb485ee3db12e6b8e26423b2c92ae76537837cf7def7e5ff3789bc0d2782b97d4f2546a62ddbf954f287c91ffa260090d3baaa50b5d4d04cc86ba47b91076872f34ddb99912bc6425f11c50f2fd43850a6afb76a84a11dd3ed680e86a2a7ce903178"
                   "4fb5e62a8d8df3426df3f61de4b908a76abcfeb02164585086bac45892114328e3dcff423b519709afd49b460d2963ba79f20ce00b38eebdb4b942c74095b181330a0aeca5ef7f66846987a325defd0412be4b0df2773dd5314569a8584a1e7dd67a6458cac46d296c7fd8b8e155a74d072aa6c977bb5ff865d9bcdb28");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "3fba35335b2bd9a14032da98b11ba7d548ab4363b753a66a54a86148d51880e257d5b43ca616b248b87fd6fc7add0ca7d017f085d66234916db3429af37a2399518da0423b62a7d9b382f047cf3adf6dbd94adb80486c3f60c686668e1ab991834c93f6ca37881fc3a236fa1399aeace1709c124c8271a403caf14d82814aa06");
        bigint_t S("8fe6e0d37cbc2187f7f5bdc53e56d71aaa6b246cf3b63d88644a130fe49727a2779863330b1463b3fb583176547c444b3b399f880c1d6c13c7e4f8334efad28979ccee440f265802f0c20dac7bbba45c1d0b7095924e02a1ae5a6f2e1d0820a81478ee11ac8c6be4d62f3143ba4c1afe5a1178734f5bdc8c3b43fd877397263166"
                   "865997f0e798c3129cb3635405b85dc2e2c43b4ffc882e2d9284173de239f6eef893d7da0883f8a552a5755a1e9b95a854bdb831882a3f0ca3a8d5c00a15c2c3db53cea22bba9a5a95749a335b510c444605be6df3aa27cb070827faba753cbc97fb2e17638fda4a6cd39cb52a16b6265bd54dd7d70c7f157cb75b2bb481ff7187"
                   "d4db341161845b9f9f110efc87d66bc49f8ff8cabc92b931c0d3e34e8a88021bc60380343a59c9900dd1b96458609c4a519ae0662502c514f36fa34004db83aa94fbc97c432ae320a06a186ad8e897a368f01881a9fd64189f84ae5237f23650abdbc2fe1cffde4eb6d1815f23da8339bfeb19a3f614a44c292b741faedc84d561"
                   "4cbbe1c72076c88cfa8e8efe49b12b1750620306215794c6346a3a786013eb0c9cfc47112d337b87a542e2e68022625fafcd43695c9e4db6cdb3bf9929f3ef293fb4d34b013ad3389c3e52562755c11ff559ef8201038cca38662045360a7cb09c9623579b66be7034884a525508a2f3ecc2921731fc4e8205a7dc00e3");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "c16d33b03f3070a88af282ad090ea51b75e70387dfd9d553f570cc7f81d8afc2329545da9f74752089c2ac5f60bb7fe6094592dcdfdfb8d4f8d45ba1a9b41f6c4c04c2044035477432d353db9bffd7788a1fef05a4d40a46692d4ce1b91a3070962372d52b592ccd0d8d3fc9c2c1f4f980dce3a936a214d0d1e265e6b8f12bf7");
        bigint_t S("09b1d3d8fe2e575996773b74e3481734c14390725c8b0231f446a7bb0fae65678ae9254931e68828260ad3a0201fe93606405869f9cceca5ed7e365595e9c8965a3ec15fb7ef5091cf3270d2f817d16de2152bb74cc0ffd1fd758d3678809d0694054e5c7f8321f6a266ed91534126ebfceb6e102912acb6dabdf61154a238987d"
                   "6a83c0140f4e16eafffea7091d25e5516e6985fc780f465c2136cd29935695f6e061824ef3b04e3fcdc43a6484ae96b5013cdb8f598317329c058b91f86d3c0e0497acea161eecdb2df768258f9ca461ec9b4b6009a557e3d717be4fbd200788a5c7267d052f156d2a6610ba2702e861b9ba6eae7dcac247758b8c8d8f3e33de76"
                   "bd56b707020bcf744ede533923df96580638c924ea59c60b6bd244f00295300f47791d2ea7d3fa7dd580756e9189385e9a71d1e1b6a1ec0329bd993a4099fd50f1d4562eca6c7efe3cd56db68eb14b96fa981f9f43af9c8e348c101fd81b5e46812da7b21e1b99521248bfebdb6be773383a326aadaddd80e40f46f34f963ae364"
                   "afbba2c04aa93fc392966355a3d8b81c516186d9f23dd2b84d9c5d54c1fc6730dddc213601a6fbdd7b93a21c9b8ecfb97f4bc80b4f6c9a92cee605aa4716d08f9c0fd42cf8cc9a8344b7924fb9f775d9ceedf9f8d651831c6263b3f9852afea6c27eceba78fc7285f4df7a67d1ea1fac3be07e3bfe351d6e2e285dbf1f");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "b2d61258c81edc56f85138a0404635574ec0160596380280a1ddfd12a8da17d661d7e36519c22bb203cfe233d0bd5a427f5cb8c2774ca405d30745edfd216c527ecf0fa414a7fad205a6c16ab477f90172f2a7d596e4f9b1d00e3411ba70ee87b0c77d5de254af7ca7dfbc8c15fc785dce8dcd861c88694c41fab4c5084c053d");
        bigint_t S("7b15a0ecd91f3a801962dd8c495b12567f2f6ef4e319bc1b7f05c79ad557fb121647b07a95bf0facf649be8b4f94303d7d1926af9ef26f8bd802d69683fd590ec9215ebd0526a4a50865c6bd494c18af20e0846e7be8419df19dfe482f26dd9d02766058a08648d0cfb273e4ffbe904f3bc9ffc92e74cd4b8e2669914d1d4d1280"
                   "2d971c92cade5f8e83f6bfc7b518fec3933fe2762dd79efc9656bd7f92ad2c046d4b7d9aec085d4a8ed4cb40b653df8d7d06b8b6f51d773375fe108bd4411cb773694c6f61bd7e416de0b4022ededcd2cad291408203e25a6bde32d4e5aaca0b2898a41639101479433ed34d80deaaf04d073bbd6d0b2c4b320caf5c3b7a1cf4e0"
                   "8256d1c907e91a5cf20e9461ccc39c973910abd1a3c9437d20bbefd8d00c6aa5a5feb6f778986fe8a644cb413020b945a8922da9362dc15bb7545d21bd50411558b57dcc16a19d195137cc678904700518be66d08968e1197dbe5d0c66e6d077145ce8641eaead2e6ea57b93063c412c339422ddcdd818cc725a290b5e49a6b1a6"
                   "4d83676b52fe61797a34556cbf95f0be375462ca72d6f596ac4749abbb446779aebe51a53b7a34cfa0b0e46e55707e1e651d4ae70bcd7c6d5328cdcaeef9e88fae9f6a9be609c064ba193329cd82bc3ded547899f2e8d426bcf10be8ff2d828fb4968005a73114ac001e8fb7f01ce3522fd46b566f1e5776539956dfa6");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "fbb1affe25a7b28fe7427aa69a89cb87bc0fb68c940d63d319b30e3411c050767651a0c21df619579a0b7e805e467bc3228c2d5c8909e5ea007f2967e9defc5dfa166057df2918962784b2065d8fc571758378f16b7c43ab26cffabaa29d076a4ea8f6a7b36638b557542021e18d32f77d9b9d35828d0680534987168324f804");
        bigint_t S("6960078bc7804d107316a6c4be9d9ffa5acded0052a2f252a01dddae1a4247e9341524643fbb2ebc936394a8521e97810fa81d14fad784c589e745de3724cc5c26020d8e21aff4d106fac224ffeec5094b60707528a46eed9019d2dcaaee7ef1af2aa414cb6f8b72e2365729baf82d3d8c1203edb3a74c06bbb2b55dcab021c1ff"
                   "27ac3827f8786e422de82b35b245efeb0b47ef55ecdcc3094cc5f5c21da9cf816341b90fdc36f8ccd5fd8ff78e4aa8b10a4a9b038ac09ae0761c300fa498b051ae1f129e0f0aa1c0fec92c31609a8ac6c4ea19908c9e733d272842b1a97dcbafab516d9fe1592cd3a8f60180a9d12c033387c39e18edd4fa3cf0e85c3f021b12ae"
                   "428718c69bc5a4d3d19b7f4d5b3ab89cf449c4f708db6a1e0123563d59bde0a92d45eaf652a8e4d39c0f4d18b08cf2fcd239983dac1b738a05f55c3c992fe8d297080c1dbed109378c2d5dd85e201d41f79943cfa08a8b0719e72a85d4e4a8935544722f1cb3bb4fd5ce6c21010d6197dfc2227bd74f89b93c82186d118aed142d"
                   "9e1812b2b7d23d7001a327b49fb3b335fdcf6c6eee5f565f07337480ea8f6e38deb1a9633f4d4fb8d327f6c95372f7986a931c49148c982fca8584ecf6e57449453337ed4c95a4185c409320fd96fabfd3c37ee0fe686f20da2fa451f2f3baa2394bef7acbb06d0adbf60ff8baa37ed1b0eb3e322433139bcfcd80ec86");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SigGen_SHA512_RSA_PKCS_4096)
{
    // [mod = 4096]
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& Msg, const bigint_t& S) {
        std::vector<uint8_t> plain = I2OSP<bigint_t>()(Msg);
        std::vector<uint8_t> signature(4096 / 8);

        rsassa_pkcs1<sha512>::sign(plain.begin(), plain.end(), signature.begin(), d, n, 4096);
        EXPECT_EQ(OS2IP<bigint_t>()(signature), S);

        bool f = rsassa_pkcs1<sha512>::verify(signature.begin(), signature.end(), plain.begin(), plain.end(), e, n, 4096);
        EXPECT_EQ(f, true);
    };

    bigint_t n("d25c8f8c97b7e5a6a98de78f19ef18077cf952e64622e4142e6ba5c8ee265ea9b34b9e5cb2c1ac0307547faa70126229b32085988c0974a03198c89f8305be4bdc445ee577c60b7a7fdfd5cf7283f0a1d0f2d65b1ba2fbfb76c5039ee06a944d40ae3843c9a4d60571db4cd16e28ce5b9fb9fa83fa3cf8cfc1e87103612568ecdb1a5c"
               "50fbd0911b645983b629ba72bc2d228207b49e00bdae1b906aa15ea2d23f8c0e124c9e7de1b3cd3b9bc6a017c4de3f1d6548dacad6d4abd5f4e179a2a5e782fb8f818cf8f307f84f7f3bcd81059e73192036aca02f805070eded8b5de01037f2b34b41e86bb8c44760492cde4bc3b080d5b0624bfa1002bd6a75d0513b07c354e8edf8"
               "a6f57dc14290eb8850659788098b9971fc0492c3ced7c3478689444e771c9434fb624eb4243f740c3ef9c3f3f084bfb59f44a46a1d3b1438e661ba3ba08d525255224816348b4d696475ccac3871fc7836385f7078412cc58e36747d248bb1ab123fa33256c10d1102f77a60643f4247ac1cdfdf4d8d51deea80795964f801f97c4f95"
               "5eb181136bb39abecd3549541ccc9d29e34a63241a4548e836c4d2159968596130a222608ae0b09d9ed30cbf04d83083457f60e8a686a452baa2b08c177ade1a3cbe7bc0d6ed846c24c4ad5bcaff3817fdf3381e64c39c18dd4f54eb82fc23d3139843228489399c156565df75f3808cbdfaa53594bac5");
    bigint_t e("0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
               "0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000010001");
    bigint_t d("28a6fc92c5eb22f54f44fe1fd87a2d219d542d6214e1bf43eb28a0743119b727825fb5ade259ad1d34b86ea7885af51f6bdf7811dfd61ff4a941065c551cb206c4fd6d7d58e52be8b37795412eb732b482c5bb0906513bf2ffa3d92c45e032e2af023c1b55878658ad051d5647bc990c79d49e060d40e530791fab5f64d1aef23ef15c"
               "2fc64a2b10a916e76e37a8316e0e26e20c0d2a8fa81852795931f030f50294d54f52013b007817cc52315354e495b28ac2bd0e932e6b10fe1b01cbd8d3c8e6b628a8778ed0a094ab004295573eb7bd3478d6380c08c1fa49a8a1445292ef6f707697af97f3e154051b256907588aebbc2a0f991f67a6da70b2772c17d63fd8d12b94ab"
               "4e0faf9330a4b4bfdcc6d9137262cff60ec003e61182f201bd07c7d3acad67cf2e780e29a11238785d4802ffa8136bfb4d3558b48e8cb77bd63ffd9bbeac0f51d297f009a7b603a388603c59d7d68a02e2e027bc7249177b185e567b7a092986e8008101d4f58e917c5e9ecef56d5acc00767e0d74b49c2d3bd29ca290e8de04f6e161"
               "d5e48af3253a1710d1f5b7a50f888a1417b12c221ff25d9b106128829894cb16eed84333a96abb2ac310b4a72bd207ead3dc2560939307a74f3ba3b8aea576cd846ca963cf337fb41b506bd2320036e3b469af5a1db5a2d7419dfc762441da11275c8825d96d4ce2703a4e183fd20d4a9fb85da6a07b8d");

    {
        bigint_t Msg(
            "0aa7729d265c746f66b75246cff95b073cdf630253c85d2eb7192db67fe1b7b5e8b93c3e46efe6a041e3a1e83db02da71a0f71f51e7913b52f8c451b46b0e26c20fab9053751299ff0cfeed116693ad687d307912d9eacce30a4afcb6aa5dbf71c3292f9dfca64f4dd44e00a406ca91febf1cddf687f5302222813ced508cbde");
        bigint_t S("b47ae7224429db36d6253878be84087faf59b5ff8896649afb2aeda0e02bf843b5c591945bf7d4dba0aa2b09a4a52ba358f4d80b5d6f97d8f636e03fe986f37b38a2e8297a48bca4ff3759d21ea6958af1ea7a08efbd61c0b82fde2def4264744b86cacf2603f5fc4828d2db7da947ba91058f704212bc6d1075112cf0dd9d8911"
                   "f8dfc6f0cf5ad31987095b5d09e520b562af6223cb35dac8c0a6a302ba9e28f797b55299f36f82fe41692857ec7d73bc24604db0ace106a2e68f6d1f9f274cd5f56cd802a4f2683a013d2d3f6992bff578310d81fc1aae7a01a61b8a9df77b020ebfd191ab749c2e3b853f284bb76c83b86ad0978f760266ec5e497f2dbbfd73eb"
                   "c693140406a0516765b83e4ca7a820fa44b9057af4b702c0069142e2933b7e9699ac099101c2c509b7825d8aef59a6432f38a4cc80e0b3c4034bb992ecce690d2e809627ac13a078b4f1a0855d6e63b2abdfe307653244aa9b23934ec898cffec904645407cae728fa6cd36f3f4a27a01be502df5f5ebd8a7f4d0d21a49086fcc2"
                   "e44c62ad5441ad9fc73a15d5759eeb13483998792f126f40abbf9b775ae24545a11cdc842edd0f23955dfe752481fca340dbd75bb8902d3a73314ba112e0d4b0ffab3fa9a51697b5efe12e53f7e6ba7322f5194146ae2da2691d14884470a07ef37bd90cb2082c2ecdb1d92062fe2b1c808ec5c07dda12f66d9ab85983");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "e853030dcd5fe68b1c59844a548f85a184a8801a182d6f1af34860c5becb6d1c7f73ed686d6fecdc031cd97653137f269d65373220255bd93027fd378908d968e36e89f0c691f85aa0fb1920158cd9cb96c0525b353541e9b767ba8bebbe1a783bcc3e6d81cff24f1c7eac1142027bd897423a5dff630993b18ffe8b8ef61794");
        bigint_t S("9c1be5e857dda58ac54ad9a148038407ebea778650fb48b9345fcac30ac5d17bdda862bf384c2eacc957a20b5250769a3fe3d32f0f1e3e27574b657d3593c51000ed3633f7fc6e2c0ec7b4b148cf34b2f01c80fee1bfb23664ed5e5678ebbb2937921e9f976473325585ba49a7d4af055fb58e350a4d98a2b9e4e03f561b093903"
                   "58df758fcd3af7aaaccdbeee96a7552877c0890ae435934d47cda981ada606bbe767c7f6a3904d7132ec5906edbb3d2f766f963aac2ff6dbd49986d3a5507666110392fc84bb82b40ad0df6b936059b9045eb2c4a0cd4bbac71e35b0f7994c1def664b7af84ba5e6ccfac1424be3102e15d985890ae44fb06b1b7f45ca18f200c8"
                   "acb9d98ba114a050912b703f3cf99ed1655b5f631b26fb75d8db6dcbe145d9aadb7903217ef2bd97fab5a0948fecd8fcd22b36ff8ac0d04d8ff17d518bc2286a80e362bfdecc41ae4054ed9b694af8313a2ff8d909d50cf140942da5e77f851e988ea5eeb65bfe6bd793edaa6add6362dd4a62f6395af888085144efa253dd2ac4"
                   "382972ed40b81f06aa994e4d02000d558e3993b28d77aa66141b7bbbbe4a162bb4f49dd5d3827d750ca79a2bb9fc350c757890d3e80b6d3db102ffa201d79006c5c75aa83b710ec321a2ee2711451195248d5c3e7d3b3aac26a67199a4e85952b5905958377a88b20da16fda986d50be8623491e32a07bd18ad89de4c6");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "56b11c9474a886b054aec059977116b88d8eff7efb49fe301b82c6c2e5e5a1688d6012a94ddab9b5642278f85f25d4ef97659e3bfba6d2f40feabf3f35a068232c192b98d63ba0e13e79c41cd90357180c398266ef66bdb351abf055fc39d82339c3ad8a973402dde99deced8e7166daf5f4f4285ff18715e7aba12ac017b911");
        bigint_t S("c8d66bf1f90237d93b174603c01d7996357afc51ab965e247664b33caa662b567fbced0b66ae00f8116867b0d3664770509a7a2192c5bf5c3832d0a0a19fb3c803a3cbe7933346f92e9b5792658709a92dbbb8d928ed314b5a27daee92c8d927909a15b6c805ad87afa3a8d666eac3496ad81d3250b825380e203084eef1098fda"
                   "aba364594418714e7276544f244cb755cf095a0a1a085289fb97ef860e2deabd9d8322c2f2445cbbc6e162be347998d1fc86767bfb4ac17abf33e54d222c054a3f6b339b01fb826694016d8a50f2739a8774223b4a688198e371455714f4dbf98c0d5edc45862ccdb1aef6d86f0ec4c6e7acc106bfe8565ab211a7ae3f806ce98f"
                   "ccc8b3e15dd937228fb75aa98a52597a8bffee05a4eafc92979a2fdc00e23474957d2275f10940fe2faa0564fb187dac74c3dedc6666a8f184c4b0bfa59198d1a4b0b46bae75282c4a8706e20f49e635123c580dacf69b285a46bebdb7dc2e63223732f8bd1aa1ee9ebf979a958d19c46cd8b0e99ed37135b16a6e95b8e478de5b"
                   "577afa918d128fa012bf13915228d4882f7547f70895254944841c75c85a5559ac9446c75d6df8f92349f692f7a41935e96dd9006daea17c36a06a0b010a48deb184fd4fdfcd31762e7f1d540e0e0405f85de95a039d3bd12176a75a0d778e00e53d37fc47581170f5aed3bd44dfaa2b705e9132e96f13a7d3f792eaa0");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "6e5e789c4b97962250ff3ae8310b522b03064eb145053d5c201e32feeed5ed6ffad7b7dd86eb8e64132582dedc5c5ffda4df8c97b16433401941a21e3cdff2f9926be692a7ce153663e04c928fd82ec995081dc487c75eca63ae77509607dc12be82cb62b42a75c0ca985eac516606b85fe7c9e1cf15041f88cb793b0335f5e1");
        bigint_t S("a05368256ba63f31c4317f2768f8a5b490c044b2ee15fec78a0a85d4f7c1a988a5c6e946040bf2d862e970dfe37b67f0221f85b04b9e607245bd0b21ce5bdaf8b138bb451205fc38b60b942881ff20ce077f2aa070e48bf71a21ce8a18c8b48f09bf5937220e8f0ed0d6cc2ddff2cda195e041ee319b43210834ece6d9030237df"
                   "5bea8d93a81c4d62f7ff48bc8726a3e32fe7b63450d2d7bb564a8ac80c89b588348bebbaa43c9a62341b194b4cf3f44cf2d7a7dd8eeac77c867894acd9c9aa34eac02e7e81eac0638933ce19913d7a0f6ab9c89d5ffe74a9e779a80cb948805b37a0f952d3bc9b508af0cf9d0c7d9276b29358bde046d65a664b48aff1d0b641a7"
                   "89b5d5f9d0bf661ae611d13ce7153b849ce071628e9f13c1edb092169b26f13b3a6fb0929fa4a033ebd345655022cf3982ab22301a472b3ddada69dff2d51defdfd72c91b4d3dba9d9a8801f03cfab64fd0e63ffe2952bc42ee10253d82d705e04097954bc6201c57ab0d1e0c60591875fe2407cfb1d6717de1c442cabe2f4e81e"
                   "939947b486044c0244f04660da5533fb4e8c2568bda7e504ff7b3a0e217d83a6dfa3bf2396a1ce92225b8d8374f2c67f5ad944e4cda7641060d0ae3e6e459325bfe6047b5f63969de5a0004c5d9a18301f9862a9988e9d1a7507b3d00e7520717d693db5544edd8341413b03e4a6072efb8053e6ff0a2aad3e351d3d04");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "806363d4e2894f3d68f35c78cdc332630763a64ac223d5ad0c246f44527656418b7d68d5b39998f57b05445e3f00c3d7382b3db6f333bfa501af8ea6f14cbc557b27fa6dfe7aad8e2d7b443e0481c82d0269b4bd923808c1ca019985570aab10b82ac1a5ab75b490e51c031f068e6aeecce3b667877bd78acde237ea6ef74af8");
        bigint_t S("7591a4f620dcf5a43310af89f8a8d7ded94294d204dcb159eeddcbcb1c7ea6675101d408464d0e4f8abf3c50a82b3d092430467bd08292b57ba9870220f53d3b6ac989ad5ed3249d237bbc76b6479f8c45c2bc9c25460c54c913c242c0932211dc4abfe206dce6302c033885e32366baea3a6b2e1582bccbfbc9e45882ab58425b"
                   "17449a53e5e0b10d1287c58ac2c44424f4188bfe1de498e29ba2a1f2a8f28e565b997946fb359b7e320e4da6ee64e48547b1b7946919e91fac8f4f631a8de9c1029e87c0503cb18a43a8a8f5d9c3dc59d10a41ac65d4d93c1e7f5c0ac27ea2ffecc17881ffb6d63e4afe8b9c94610661b30ce34d4ec732b9cf886b08f6f38ea9aa"
                   "094188067b656657eab22fc90e234e64e31dd0558de5891cd49cde67c5d6b1e307e23a35226ba09682f194eea06325c9e0a733c9758bca951cccf5730b71987ef564f92057a1985e45d3fe20bec7d96aceeaac031caaf4504dfcfd8c7c12b9a580844851dd54aedad307b7a6fc79e87a9431faffc9353e497820697e7631600c7c"
                   "faf851f844981cdd2b2048c78044e0af699d8b0b614cda9a3032f834112f2ac17bf8636314c1c2c5d2c38606127f57d2ffe8ad7a33cb02abae23c2eb3f3b0852826691729b16cfc6b2cc45d4177782ed1c1de61768b436d5b8fa6501cfa97ec65f5426b63e9e82ec3780cc5fe0cefef8f395a4935da39cffded65f9d37");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "1c8b88e34c5469ce1c6fb2625b0247fcc07930972cc0fa9d30b55afccfdd147d85551b52880cfaed4d3072194ee665430b3ef3184e66e256b41e43db696b520333909f30b23a6baae6fb55aaf1f936b11ee8491e23358f00a3847129f75da8842e225aa8524585acedb7a13dcca481ef035d7bd13296c84406d12d0d021adba8");
        bigint_t S("9eb10c1a31066c3eda46d2a6102af5232b92a9e164c2792303454c9c9919d2eb34a896010cf2230f8138bca6869b4625e95c92cf2920b8c897749a1586e9c4e24e7a975f44968216b4ee88a398e49e3a234dc32f16ff998b8cab805ee9876b2ac6390abf53e6bceb2a8b1e6c91a550584208b53c23a13a8d5739343740fc667a8c"
                   "9883993f9e77b2bdcd8123f1389bd7b3ec363c5f84b5b65dc183529b19e27c3b77db8468d97aded8ef878a112e0fd099ad1a634a3ae84972b95dad1fa7c5c46aae1e6c5f2bf1b57265a2ec4aba9cea60c738810f628ae076c32d1d12cfac0a2704af40162010acbf819bf20c2a5282d76c05a09ee5812db5007ee43930dec8d928"
                   "a90d675f4223a77491ce2b396bc0a0d33d359051c957e19e411cb3577e9550f2e3ce637954551231088658b11b3e92835f44acf92df0f0481aa8b65216c649201284b85cda544eb9c2f08599f257adf3d3f12fd3032f0ac83cc5ba94537c4a55952e7df234e13e8d0c6513046d7fc5c7f2b2efbecf4c19f207486fdb5940d39b31"
                   "e0dfc17143c74206c519f3e799606122688604c58eae05ca3b5f2a7a8a280e712e6968d4d225fad844dda555543d2e5fd9f86abeee66a9b98e26f90da6313de0376ffa4714ec1a82f74132c1fa60c6bc63d48f6a31d5bbd49b8a66de5dfec94b354c60123f9063e85ae54e75957170117f384cb72fdd96c04871225d80");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "f708072854534169081799b3714803beb67f8409ae4d39cdfed09a2f828300b9ef5a650bb60f6bce2bb6a0eae69801f9693ebdfb04cc3b4bf6915dd2d5db077a3010dbe572fa47600963c212c8e252798af45218194e72694e72ff19798165740250f21f51bddc05ec448693dfcdff2861c8cbc7b47ce7104d9c076e351c2291");
        bigint_t S("9a1e8c94597a186ae778ebb2473182b32dbbbe5729a574ad55346c156c5a377ac7302e09859368258139a46312b10aee2cdfd53f213ab42c65ff9b317a1414a071886b3066bcd3328aade60e8e6afa5c28d7d6ed193059c2af4c8617efdd8fbee7dd180c9297783f568f0fd1590490d0132a87b27c4b8be188eb513fbc12a419ea"
                   "c894b254b1b862254047bf582d2faf8c1cbba9465be5e71b7afd3e20bbbe933b044b33e9f06aa313beb4558d02ffaafd4825225037098d567b1120c042a4f5db6349d42bf710f0add3ed28091f0afb34695567baa8170a836fa0a9499de9f2877063b3e34c92cb723e8a1b76aa8e26568938aa5e51e7d0edf666535e5285ba1dc8"
                   "3b59d3fb9a0ee969cf4269aa94f7102f52080ff3985adc62d4f637ab7cebf7914e00141b27687241befc726e7765be8aac4b4902d592e0ad05ef54ce2c278d01e7e0b8de1cc1c48d912965f90be1e5170b754a283c571c88688f2d296c4cb5117372654e82fd9c871cf33bd8a041f48d484777c023db71ab2a0eb336c3a76d1102"
                   "1b7772dd9711d08d7e812e9f1373affae8d5b2e2f6386de3e38f93dfa7094b1cd625efdec3835b82992a3f50e64ebd6fb0c4ec4106f088986f98f1161cd4a8152c6b12c4ee2b3c115f3fe874c0a8d728a18cc761044c2b66aa09193be723d9d841e773347cf2346424089b21a1439a7cc3ae539cd45e99ef6fe285ef80");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "c810b157fac10e2fe1fad7b4f03337b5c75de758066ba2bc5f11d9dcf0cba7b7cf2207aee62b452d0150b28d25529c39248f7d106e81730b2a88f02085e9b3664a16f779b430ea2080ca602d45f0c9a5086760210b37a674e7670f8eda08fba16275e7820bd8e55185f0495cf912a6419d6ef9538794233fbf379df0973067af");
        bigint_t S("a7ab2b0a42a60f303f11b8f14ffcd35e7357dcf5e4e3e01ba32b8266c6c3dd59298b059142bbfb9f18f22ba1c638bff000299fbfc38ab5c5fa6122d476e214bf8b0fa96297c1a63e134e1283490f16e362ee885d135a58a450067f9dc08270063ff9be625e99037a562f3a47012988790c23842695d4db077b7aaec633094fb206"
                   "56adec1c733f49e4eb121dfe91c8ec9d0db80f786f91fab87d6dcf68bca9d3fe9bd18d562f37db5f861850422a64df5a69d7e6c7b12d8ad3d7600e6cf259ccd33e7e66c3b8ca2eeeb1d4c03d0264068437b040c90f2edbf3982d227ccde89207f29c8a9114a0544ac9e2b64991621631a9145cb6ac0b637d495add900eb1592586"
                   "00167d71f28ee249466ad43961b1ea563743fd5c700d83afda3d8ce64c1727c2d4563ce9cd146687c2f8f8c2f402d903780cb77e9d9538bbef0d0c13f86044fc690a180227de71fe429b99ff919f4214aaac666f8b0bf18941e75a61d6ce214469b9757a4856b2da118626add8d2fa4d0912468f6796908c4bffc287db9b14e957"
                   "190370ba72ade18ad1c2cecbb1d9ec77f92be526ca3320851ef576197fc69702c45fb6594d4f9fdd27aa7623fd2812fed0e26214114327f85ab740272f0af3e4e07885c130f9406c0e39d917ba55b4bab001572c0cbecdd6ade69467bebceb0473d3bc1379bd8fdefc255daac9cb88465552b281d525b7c8ee85acb9f8");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "431f3238d891886ec8471401ad04daf24e00884969955963b03b398e11b62ad33a8ee6135dc0c65630b3e25f396d7a84480795bc455ee7fa2b0cf8b9559783d8f9e71ca9f2fe1645c2e3a422d7df3ee70dec477e756bec6b55154f5d6f34e50386131c211fb342e385bfa2710fc2f0c3a598e7b781233061333f86a2e8f718fc");
        bigint_t S("a36086eef46617b01648cfeb43aa676ba1d0f32bdfbab194ff17e685c476a434111f029d75051e00206e7cd12419737d6803fdccbb2eb76bba3286e334dfbdd0f83babf29a92241d97e13b8205d2e8a33a8106c16d9f98a67ab9b132ae0c821d79dcf824e1e28194767ab8b2fde70813b6701f29fcb1470675207498215a6f0a46"
                   "71bddbbd1a5697e11ceeb61384b1e494eadce1d35e153b5a7eff8be54aeb17212118ca8166ca81f7a374272cfa580b7c2bf8ecde65b42860ad9463316deec871adf5851864b538efab894a8c8370f0bf3afa9c3087cd7e955586d3ad08b90278810e7db57d0aa811146f0ab90698d1ac08a25358938fee57e25ea0620bb40455e6"
                   "cd2c688c5de20cadb5491a19956ecff1cba23cea6c991765e9f749635dd24871d24a3a475300d6e08924d037b7aa8dd71046e9cbe8857081eafb66aaabb007b6ae395151bc9bbc31ccf72ca756eba01d48c2e24d8422c6ec82d4df1b953739c4846741b81568aaa59509e81109b4665adc1a57b46169ef9bcd912d3f108eca2753"
                   "45b45b2aa571357864ab1212be27d139547bbba26c409681f632bbce553be94de50e8fc47e685dca76252b1094b2785d5a82c5333aefa24523e1e809a75f8c79b182f734bc9359fa1da421c2ebf6b18f0cd79ec97ab1cfff1bd1833d90b8aa482a57a29b6f886db5d8c14128aedfdc17a16a4449f5e2b9c61223b2a5cf");

        test(n, e, d, Msg, S);
    }

    {
        bigint_t Msg(
            "c2d662d5ea51560c4e4a3c25e137c4dff571f009aede2445b7cd7c0d332161f3f7b25f2df6f03150fcca1e5ca0ce89f97491c3007e51233decd9597403a5ffa1594771844409df5d92d4a0f57a50c9ddd34dfffa846289423cd3a9c063b82dde505c41e3bce487bb76316af75907af147c6e4c00a8587eda0f8516f93aa41331");
        bigint_t S("a023835c19f3b9e66ec2afaf46820cb34fbe196919e29107a47294669f50690b699c19f08b85da515d4485f1fbc7cb9541e4329a2aecfe764f41a34103850b2748f75de6829545894e3b0d4fdcb047017d30dd3aa8b49b38d21d41e6d89fef60b67d6e956ff0247eaff157c6f079dd520cdbf0915dc08dc6c01a78a5fa3ddb8b8d"
                   "0acda861341a002b739fe290f14dc41df97490fc1e2d60ab2169301dabfd215f0357b0832a390f97ea96ee98f73a53e7cc6340217abafe5948a0a83190708404e0328fa156e366bfe2d4a9ac75531e458d91d151786c36bef7f0ce2d5ce5b6b0ce4f0d6410fd897342d492c1a7f8185013f723d746a61d2b97aba1e3d534dd63d0"
                   "ad21a3fc73d97e480a7be367c549784315aea37c3a2f4a3172a5b4c4c6cb15a38d4f2d276c643ef4ade2b7ca6aa34b248849720f1bfef5f9a11f7dd279ae2185e4195691647acbace1ddd38581db67d37a839c2d6c5b62f0fa798d35f92eedcc2caf8a7d5623884f2f2631d82b0633a8c3a42baa8ccbcc7db05a342715788fe88c"
                   "23652306fe025060d5c4ce305e33f12b8fec51d1d0300a7c0091e9166cc6a9e0f24b93fc93e5c712197781072c040acd5feb83cc49118d50aba69b54b3e51131a64c120a7989f05d7bf4543ed3cb0cb9048af363125d75fa25da39260eabe49004f51bac9a9feeb5818b514b09327caef7489f9c2c2467004947e48fac");

        test(n, e, d, Msg, S);
    }
}

TEST(Test_Rsa, SP800_56B_Section_7_1_2RSADP_Decryption_Operation_Primitive_Component_Test_Vectors)
{
    auto test = [](const bigint_t& n, const bigint_t& e, const bigint_t& d, const bigint_t& c, const bigint_t& k) {
        bigint_t K = cry::pow_mod(c, d, n);
        bigint_t C = cry::pow_mod(k, e, n);

        EXPECT_EQ(k, K);
        EXPECT_EQ(c, C);
    };

    {
        bigint_t n("d0b750c8554b64c7a9d34d068e020fb52fea1b39c47971a359f0eec5da0437ea3fc94597d8dbff5444f6ce5a3293ac89b1eebb3f712b3ad6a06386e6401985e19898715b1ea32ac03456fe1796d31ed4af389f4f675c23c421a125491e740fdac4322ec2d46ec945ddc349227b492191c9049145fb2f8c2998c486a840eac4d3");
        bigint_t e("859e499b8a186c8ee6196954170eb8068593f0d764150a6d2e5d3fea7d9d0d33ac553eecd5c3f27a310115d283e49377820195c8e67781b6f112a625b14b747fa4cc13d06eba0917246c775f5c732865701ae9349ea8729cde0bbade38204e63359a46e672a8d0a2fd530069");
        bigint_t d("27b7119a09edb827c13418c820b522a1ee08de0e4bb28106db6bb91498a3b361ab293af83fefcdd8a6bd2134ca4afacf64a0e33c014f48f47530f8847cc9185cbedec0d9238c8f1d5498f71c7c0cff48dc213421742e34350ca94007753cc0e5a783264cf49ff644ffea94253cfe86859acd2a2276ca4e7215f8ebaa2f188f51");
        bigint_t c("6cf87c6a65925df6719eef5f1262edc6f8a0a0a0d21c535c64580745d9a268a95b50ff3be24ba8b649ca47c3a760b71ddc3903f36aa1d98e87c53b3370be784bffcb5bc180dea2acc15bb12e681c889b89b8f3de78050019dcdbb68c051b04b880f0f8c4e855321ffed89767fc9d4a8a27a5d82ba450b2478c21e11843c2f539");
        bigint_t k("5c7bce723cf4da053e503147242c60678c67e8c22467f0336b6d5c31f14088cb3d6cefb648db132cb32e95092f3d9bcd1cab51e68bd3a892ab359cdff556785ae06708633d39a0618f9d6d70f6bdeb6b777e7dd9acc41f19560c71a68479c8a07b14fb9a4c765fd292ae56dd2f2143b62649cc70fb604fdc5cc1ade6e29de235");

        // Result = Pass
        test(n, e, d, c, k);
    }

    {
        bigint_t n("c8883f0573e9a5f512fb65ed0adf2649b03237edf4faa6b259f89d7ed59d30d6fe052ef8dd68c9d672c9ac4ddb1edcf0102b8214dbd8478c7c87dfca0ad26cf5a9f73701e8525e059d1be2c568b42e39d0549bb772ed43135a39c79071b76e6604a05523be68ffa9b1c28f37f30bdb0bf78ddd85daff7054c9ab471bb483a065");
        bigint_t e("a9e9df5a55fe9eeccd16fd651c2d7f13a9942e7418052b4ae1b98f8ca3f3e828532a453289bd47b363738f866debf04222abeecac1e11f980b6f115f097f4540aa7735b993f17f55083caeb6a80f80d092c59d2f895f783fab56a353b58a8c4316eacf3012c77e6fbfdb4be7");
        bigint_t d("032ed7f38db0a3f026aa85f35c8835d8add52e62f631d5a99a6b6fe407c3711dc429cb5846b40fe560fd82b18a83bad21209382584e635ec21b38299d1651797246d69cbc0e23e674ce70a905109583a5b2ec87b0de762594c813f89c06d3ee2a19bfa0ffd78c26bb29d426f7647020434341b4735a2972b6aa653c662d710e7");
        bigint_t c("49099483331a355f9e1e9446231f1f36d9a7cba76b83c06c6ec752dbf112342d5513e8285f4c7afb8b166e69a3e6f72991ce6a8d7247f163eb2d8419c00b1524a2990ba14983f23dc36c71530c337c55de75ca6dca538a64887552c28c4e5837d9a2e4391a4605efef28844e858f168269a3d58f9e2ff138eda6a3207c64632f");
        bigint_t k("c847d03e2e5fa8ae543e35e47143bc6d0c172889418e2161398171222cd9aeac4c70f4e1e96425fe555fd492b8a5b45ae9a0af6ba22df257b6b354705819c9018c724032d54da56821f945d8fab1eada5107494a4e72d5ce98fccf03a8af05331f2a7121a0a9201494d1ce896a104323fd475d3e33d570d6cc426ea7f2b0646f");

        // Result = Pass
        test(n, e, d, c, k);
    }

    {
        bigint_t n("bd74c62141cc4cf8228224d7e2de0d52b51e67e2d43676249470304d30d662ea3d6e5225101317369ae1de6bab944cde823d182cc0177f1905ca843611a2f67b9c32f975feccc8e9422b05d8831f0a013d8f9c68b1b8c25e5925d516414d34d57d84f38311bbebc7f1faf6548de9968e871f8e7e16b1fae5545d3f3cf04121b7");
        bigint_t e("c08b122dd36a9965916287d08d8e2ece0500d4f1e37b45e29cb056efcd1449220d602e7cf13ec97b6c4f17a7e57af9c8ec65cb9864b7a4c83290855c8ffc55bc1da64e43b9ec4e9f266dce489d14aa8cda67cd50b638d8454a6854741126c4a07cc716330a37576e5021ca2f");
        bigint_t d("0217f64f638419c8741b4b98a0678e40cb0e0c703a8c823224d88be40ea4d2bb8183233c9be2760ccff280a51ba714646344885212569ee9aaa80e610f2de30f42ef26a55bbb2bdbf31b9238036cb387be57067116e97386e9aac9cf2695e266873ef2944a4b010804408b039bd543a7f622983efdf56ec4ebb151ad80d5f4cf");
        bigint_t c("97c4c0f9b86bf131e695932e891fddd63c46e87373bd6d6261fdd10f19dd5e21f362d4f131d82041e2216b2b074e5657f56c8471faa05ada4377029efd77b6a7bbfa3b30183cab7c9db278c7e59dae7dfee078aa130ee6b148c8ed42e646b086b833fd9cb5be1b49deeec3eb6a356caf2149cc04062f9b5038989ae11a71d426");
        bigint_t k("3ed4d56d770a47b5a90c22d75fdc71b31a1fd54435de4375a06002cf13db50a012c5f5e5d94b7cfc85b5b3d9f5701d5d3aa75b5900ee0b09091c67d8ca33871228e041ae16138b1343bc3003af4aa21ff701d623dc40fcc0bad9848ec41be3a68ef8f9fdfbdbbd6df98f571a9d3fb8fad7c1e00ae3e72501a1182463b9971298");

        // Result = Pass
        test(n, e, d, c, k);
    }

    {
        bigint_t n("8ae197e5bb30172ff378eb37f08709aa87b73802a027617fe959439399b3a0ae58ee46ec6e9fc3f29fa03ec08db9d16371dec03e9a6422d539cb4deb2dee52fd4dc1978f72cce77726886ce81c62b880f29623758aa615c65e500101d0980b2fba68e466a2545aeae7a3bfa6a549ce9a62ad7f171385feccbdfa902494d78443");
        bigint_t e("f7dfd3273f5ec1a26fcc9d7df3c448f4c9ff6408bc35a26f6897875b2213ac6a6ccfc061ce46d6df12ec86f376377b51c9b4feccafbb8f3df628fd05af868a7d066aed834cdb9d9e3a3a5b60ded9799b40fa0edbbcc80b1808182f5eaeb77735213231178f3637cea7b9b0b7");
        bigint_t d("2cd9eb9d91370fa049e778dc7b2eed16032a396d96d391f58df1c269ca2b354451bd13ffb929df047e356617f6d7394ca30622651d84823e7400805ea20d4eac617146eb8b11d0e53048c74ec02037e8b58946d95f10505dcb9dc468e5ade82fdec0edfafe09dd0bd2d13a07dbe96b741554b4806f4beeaa50562a1040e024af");
        bigint_t c("3b888227ed25d10630057877e11ee54576a1d645402dbf2cec426ba350f8e59e65f199e98f8cb703255bd8302dd8e4ca675333d88e2ff2e2da4ec903c9caba3f1fe64d172b44ea5b4d588640b51c82b26d05cfeb9a365745fff5984cc25b9218734fe4da4ab532b4b0e2c9ce015386a6919ff4a7edabb4fe631d22a9c41335d0");
        // Result = Pass
        bigint_t k("19e5d33241cbc43c21d1c3eb21b9a48b2db1930c78b38321487212f82859cc5b989b77ee23c49de771ab9603ccf0631abeecabb90b4c7178b86db5aee994ed427fdabddf89def7643d0903379f1bb69f575d09472648d7050a77e8c06909dcd51b2edc194d7a9d632b210e883d2710a155f15b3cbb156cbdd7135024f671901c");

        test(n, e, d, c, k);
    }

    {
        bigint_t n("b101bd9c2458804386e23d1b7a521430569b4f49029e87db288892159834d827bb2083272bb85bd5934e7ef6d458e40a8549ff338a7e6d84789207b15f824e39ebaefd2076f55c69121e9a90aab7c662734b50b4473c3342f7c57fdb6b891c2697eefe0917d20db468defef1d7acbec6f88390ccc6344b48b273a7d51081c4a5");
        bigint_t e("b6c13349b9671c7bbce53881ac1112b0b83be497f42b1776c84acd0e9983b2ede9391126d81ddf249fc542fecf52269df27e64bf995a026b71ff631d3dfa8c84a14ebef82079ff92946188eba0c2a04b54c1f324c073081f38380719ab64359dddcb826624975b1d37a43bd7");
        bigint_t d("22107d6ec7bdf09c9f688448bfae94cdf7268bd72afc96e4ad9d7501b04afe69df7b4274fe90b8f35e1cbf03b71a097489c619d2b87ef0024a6f2c2f1bf7f827c891bf7e42bf1f9a3b973ff0305f5de643092da60d9f039b51902e1b07990d5f3e1f360de4084e92f9bbfe95a51105dd58c266384c5ec815d380eea8dfb248af");
        bigint_t c("400e4f3d5498a2b5d695618f4599a80639e875b049d1af692d3d678c20875d6a888ec6f4d9094cbbb71b571b5b288bb237e69217ddb71bc91d3622666d0f721e8f1569666d8efcf148d1fadc11149529177f1e6ee38510ce607350294049b3909acac1fcd8cfb93fd0e0d53a3105ce5121daa31c24e2975f401d783d5fdd27a8");
        // Result = Pass
        bigint_t k("53fe9fdbc8de44c4a0c073f4ed9ba8b2769174fa1c29814528221c223e4dcd42435f3ca8db857a8cc056dd8155369c45d5d6339ace54b77ae4ccdd8180ec0a251c5599cdc5187c5a055ba1cd97e2ac6f43d6a85bce3646dcd2e0342a6d3a165febfae367b79c719ede2940345c55ae9d6106d4f94be9b03cb561f3759304792b");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("ab04b223f82347bab2547e2cc0abb18ffda653a5284916d90a1fb4c24315a18c778709b18a71abe1a385b305d25813308467d2c29263ca468d5c12e519f8749f267d17df5398b473fbfac9f5b4e98cf5c90e5a65d1a4561c69d82616f1a379bf2c1d0f8477e5339d925082a0bab01f8823d4e46ace967cc526985e03c0e52a4f");
        bigint_t e("e23bc7155efb1817eae0633b99b178ec507dfce68c2ce2631a625123ebe9cf0b8c142464ed1766b69d7eb5c50bc8c63238b615ebfce42d087ed87c6cb3cd7d83d561dd350f9aee914cb73edcbfcb307e1cf03ea2be449e902d3c15d582c446d9e92eecb5e5fbca71054a857f");
        bigint_t d("060a93b479b6b56f7655fccf0770321733cedc646711244f08d12b56456e360b879c4007df0d93f35b57b7c51cd1b4f624631d5c3d656d6c67866001f5668a318bdc5259381db72eb50995c95cfdc45fbc3902bc65a1ce9dc92d1622e087db9f6abbee353371d700d5c2efcce47dd044bf5061853e729cdc28bd407837d93bb7");
        bigint_t c("1649ac3e49126af973c4065224bc36b46be21855f63775687aae4695563d77a67333ec836eccc260f903d7357ea78c6e06b1c9e45ad5420d6757f96fe720045b3e1191d7a29727dc7f802fc21a0db2f8744864f87197e27a399238e8df12a613418aa4242f9b23d5cfe216e13f1531b87d4ef07884a6dc19b8060ae7843ae436");
        // Result = Pass
        bigint_t k("89219746564489c208be153f337de445c517ef25060a5557f49bfb639085c2130cd8130df14cae49d9541e0418afe3d889356977270c9f39bca1944ada92fc115fd83078317e18c0d927d1d5a7825524e5f5b7d1b5d40a28187e05197f66888310fc2d23dba232c352758cf2f16b20911c96363475edb33caee52e16750fbe61");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("9d6c06ee50f4528b0d44ab99a87d27f180a73e8fd98512b11cd795fd417173cf44a55f050f5c1df9a80a4917230b4c9a2d4c76e36e4c845abe717e8384e0fe312429ead5a02d0ed98b37c85d034dc88f42d756546215df2bd5c739c1d43b3a37841caa7ade44be1e089eb0bd7cd7205e66cf7539d8ea1126d21b9a6411430c2b");
        bigint_t e("6367395890cd978cfd461fe82f3bb22f074c6c33ec4ccf395799ab830fbcc73196c8d52f036b4737cc82a3bc596fb56ccb664c98abd4fc3f21682bf7d1f58dc7ec16c3c10da1827e071d840349c281b57f7d11c7f67432f04da27baff2beee40a9f9dd3f39ec1201fcfaa04b");
        bigint_t d("16de900b71da7850b6c774035beecac0861b29ca03903020b2cf8d573f160a36ed73ac4e28c57a17f460d2ca400a48a239cc2aca97903c9eaae57a1c6d39148a9465795ed87116cd82445eb4998ed913cd3b01c19e08cf91a212a03479fdc1e4db717d0ddf04bd35c17da598f0b7d0dee5a5379ead70e939fdb12f99572bd4fb");
        bigint_t c("8f9157d36a2f306d6524e7cef4a036d25380db51ca7ef9f430a5bb47ae093a0fc5b4a1dd92e38fe35173bc072196e03606cad0fb340cea508b48b4b45e15c07d789fd37061e4458fdd24ab32b692b459bb505141099a19358942eba7ba5cd57fb13a34b35e9b1d18101ffc783bfe7be47afaa64bd0924c4c3379a8494b4640bd");
        // Result = Pass
        bigint_t k("2db5ade74b4c168f894c1588f61754b6d5db8e4b62cd95ed9d13c9795d8cd8187d125c02d688086110c4de61bb7d9346c656cc89effc6cc945812e696b201fa8e4571ee74aabab931cf034a77a1f77c2649fe75ecebb0f772a765fd833589e4f337262d1a15a966d3072b760aec506f268f1e359ef611de5a7e4fd1241f40583");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("a5be019172ef33f82061aae0e88aa81fae292b3e1c0b33220c5822dbc9acdbec2eb7b0fbfae1e9160c47277a85e047c629686cb40be50f2a676bfa702c773cd24ce80a6c07f3c2d9d65029abe0c876cdf3b9f6ca1b4ce8edb74aeb3c63dcd1448de20ee97063a26450bf4db8b83a4f16b9e3ea146aba454b72ba1cca615752bd");
        bigint_t e("c06312d850e541be4b24c664811e80cb0a8d4ed6b21f1533d55477d1b8ac8e4b25441b52f75a90c23e7c81b42da33e9f7d6a02b8c172a8341aec8222fa2f9540a64c79a0e8837c0b02c82f75aa0a529bb715f2aa7bbe5d63f9e346a354be5b4aaf95a52abdc71ec6ed171581");
        bigint_t d("22595b2c5ad55e4a96dce4733064e1338ceabf00a2fe2df5ae8afc7990ace25a036732c06f3170844868bc854d8408ac49856bca8314b54b2fefefe0393920da3c4db3e62d8969da7a42f89d93c79954170082db7a44c25780ed7ed21c6475d125877ea1954310bf91031ba2ae6422a15e9b67868224bf6050839cc0ea331f3d");
        bigint_t c("067aa5f930e6e5f25b5fcb93c57d9cda00d8a6aeeb25d420e1a281f030b58a53cdcfa7f34697fb38f745e9c6814b2a47783b1c6a5ce55d2bcbbf9956f48ba0464f33bcf38cf8412cc9cb652007fb5223df117aaf71f92ba99e6263e67766e5eef46e8c00ea50e05637bbe7a93fd53ef89d22678612a86522c15186aa9f9081d2");
        // Result = Pass
        bigint_t k("384e297946360d798abdaf2a71ae3b5d5222c7c874260879c4db0d663ae185ae2ccea35170e23707a380dd4230de9b52e0b2aea273545663bf816886f1b528f251ebfffc778c19dff5ed151a469490aa9ec7238d2e681ac33b8418eac5056045bcd90f1490ba6c52493c3af831b1a3851166bff56e85994ae1531c16b14bec95");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("aae790ce66165e1431df2b6234497b11994653c3f70e1724123b5adfb993daafbee368707554d7392314340abe052b6f7f28a6e4528919ed77ee2d0ec7e58ba3503c61d41274af784ea38f9a5ede616cccf011da8c4d7d46103dc29d166f1424a1a19d79e6a3acae603de797f1caac0fd3ecf246c40376fceca0931b164044d5");
        bigint_t e("c690888a046c216cf843d63b0a71eb930063bbdb884d4c9efd372339bf2bef0fd77793eda3d89508c83719acd72725df7a456c5acfaa54ad27fafcf461e92981322977a38543f3af1c1cd8027e4a4f35b16e1657cfc7ed6ab932fffb0dfd75374195e5fc3f30a36b0eb2ab17");
        bigint_t d("194c170c8407fe3c9aa81d193f92d933bf49a05af50332b1f6deaa442d85a04580716af33136655881d4b2e50aaed19c5d22c28f755c834bf8c045eaadc0376ee4f0e22779196bf0029fbf0501589338d993c626167b5e576d20b533a150470ab6c0e4bb30a091321a8f63fd2c8a2cfe1e9a9e09ae43a14ba6066823139b56a7");
        bigint_t c("888c45bc9f8faa8893f2013c8917badc88bd25c588c62b92cc3d3c3d7297888649397299e23b41ebea6c1965b6962bc420032840923ffb6b6020b75d5b04ce0ea35a8503bc64a42de43a720ea413e01da6c0056cc679ca886bdce7a018330eea3ef6654355913a162a66f37e7b1643afc1fca4924dfb01af967ab250d2acaee3");
        // Result = Pass
        bigint_t k("0b6730cdf0f4e8106edfef36437d3a03a2a62f56a26dae5440c487db56684016e617a4301cded9f91da454f4cb498ade5061f42d6af2be01a27474e332e308029d66ab392963e2f92550132bafc1ccb624d6e5e36cb334e50d0473afb3a6a600757f8caa92f83599c8f6a7fc4df348ec0418e6baa529a9b2ebcebe754839c505");

        test(n, e, d, c, k);
    }

    {
        bigint_t n("cc5984a7f60abe9f99ada8fd1de66b1b63a6f58938d1ea58f3981d883f91030fa97a93d54d79d614ad47a283dad02eee1640351b77b83930cb9b5554587ccc0742a29558d36fb5a8fdd19f257de4e5100852bc9e48ddf901651578c4473c1d6f76acf6ad75033a8e08277a91cd4f0589fed41181946f243cd8370969832b467d");
        bigint_t e("9e06619b72712729e743275562fcc12a8305eb92b4243ea28343d4c2e1238afe94021cab7a0d78bbe1e875fad56837189c46d04e7efb4f72aa81b931a73b40c4c6b9d18c70882d52769649e271e724f08604a0e63d4f5df1fd0608dcdf41292cd1de964de30857a70c90e7a7");
        bigint_t d("01eb4e4d4074de353dbe9fdaaa98e157e91d09cd36b9274892ad521239021c739abdc5273453f04173a3041e44b53aef72d2c6b375661cee61d158eaa87b3b537145e9aafe21da40e666139c40622366b46bf817bbc0b44b677270467f0ac9edb4b55e023cff5fa6eea870dabba8da7d783a03e1c47c9b04d664c0eb1b4a8e7b");
        bigint_t c("76193e175efef1136d123601a69754dea902ff61e15c287efee8192ea606c07267574b95f122f6b282f62177a191a6280f0169e7fb09313275c6b6ad2592fb223d29ddc6ac0becbe95217be1ffa2f3c0ce386de3f25f7f56de0e55653b9a04a3ded71c31f8807c3c8dd96bc82892e4acccef30de2d26bbf1200920dc51e486fd");
        // Result = Pass
        bigint_t k("2e7fe1ec2c3eb972af1a84dae5a401d13b51ee6de63a792a928a60c790202d44384ee8299174be64ceebb013008921c4ed08a891d2cdc99b7ecfa80a412f41355e0ca6515ff74678e80b979edc6cbbe7dfdbe5593e6bb2abe8f2d1fffe15042a1ed4b3c1c7cb8b34fcd48beb4329b99c461513bc018cf270ceef4dfd7a351914");

        test(n, e, d, c, k);
    }

    {
        bigint_t n("bc73128135abff03786ca76b4ca58692c53511eda689bbbc3e90bd23591c416ecdb7e22833cedfa1de94ef047b6d17a8c5f0eb084ba9677ce38039b1b10c72c87f4de26186bda671c855c8798e4b16b6619c012e4679f1d04d90a15da8db1973b1d4bf7b6d94cabe1130f19b34e2a616d8d59684406b23dffebec41a0262247d");
        bigint_t e("65a07403091e668a7511be81a6e583ed9677699b25069a079cc133ab7860c946f4c4ae7d46497d33070a9a9684a27fdaad4e4380ededa4695e681519359b24ca418181e0c075641c050086f878be7a1b3b3b7138db33b94f0f232d38dc78a8c73db2538038b98aaf25e886f9");
        bigint_t d("5595768e334e32a7eff447d468d79df392ac43dc4f62a41192d6b411b967be4ae2063700210bb61da026e375394c8baec73442db7e9854594c0768a3342bc4506ff41c239a5b794b32de5059d7572946e52dbee6be391110c029e1b96a8675af94c6025345f0498a993703298557c21130eb59bd84e5110303e72a2681aebc8b");
        bigint_t c("7731404da6919efa8d4f47953621a82b8d1232b4b883533ba66fa63febf36758cb0d291e94a4cb4632c8a80efd3c0edcd531f2217155e184f40c3a3dc1fd9bbe91d19999c337b49ef32c8943cd794a4a237d6c9d3e2c9f7d725803dbf5309906f206f887d8554149f80254fe4fe244bda07d09f0c711756d49da6ae32f914978");
        // Result = Pass
        bigint_t k("60231d6657fa2bc2482bd081b6a33e666bbccba3f19063b9659609cffedff6b01ab63f519ee527bbd264838a6e349289cdcab2551997de43c7445517c277795707aaf68ca4c5b4446ee60215383959ead444734710b7ce7690734c5222afa0b48612114dbbbd39a84be14d448beb59a73c0fb404e10d8517ab369ff31181fac1");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("eab017d025220d1c3bf6e89ee591aaca4d5a96a3b7f8ccc1932bb98344ef442fb41a1c82029de53dec19d1f448fa4b9b4615992259e3a85cab1a9495ec13458cf2059caa2263255521f80859fb4062aaa6dec660b6bc3685e8fcc296aa9f2a56ea46e65dca824e83f0a6f4000e5a743d6d366bd8134c886a8e466743e19a94b1");
        bigint_t e("760e3c5173bef4fb2b3e4b103ad8082249c14baa80e85fe0bcf8a253dd36dfd2744533a593165b021872f5ce857da213ecab759e432beeb23cae1ad0836581105496558519fa901451856d68bf52a57db2d87a34341c4c871bf58078144f57425c3fc3dfa090fbb680e17e4b");
        bigint_t d("1ad0e0fadb7de0801c4d7123599442d00bc9cb31cf15ab37637e2c99df0969534eb9966cb0376c6644abb2f6071970db6fd63be5ac412da37477cabe18dbca529d7afcea827ab810a288f8f7b93cf5555df671e05f1e450977154f41b5a4c6bcd55d119f24ed838b40a0d51ef4ca8427b36978a00af2d1cef55434fbabe7d32f");
        bigint_t c("d4c44c9e1cb75421fa1e53c27ea0e8f165d1e68832f905ad8016d7f99624fdda3c9b9a35cd3a08508e03caf03e87c2beee63a4ef130bab24dac68b8fbbfaa65851aa823f693d7b825981b50193ef55a07931f1ed73e144033f9427ae8917bdcfbcf8ed021748157f03a0fc3ea462095fae41b3c87a0439105986939c2802a807");
        // Result = Pass
        bigint_t k("4595b4ca7bece728eca173bf2659e22f757342d7c9934a3e0740a0fa9629c4e06493ac3923accef944d81c67fe3311af7e94105f7c5e54a0924478818ab8a6b6e167f5da90ed550831d3d300f0edf95a4ca6b4b4b319441152cd741004e25f6eaa44f7c7eb95d67129a2774e8393607508b8a74866b440122919f8e35cb7e5b3");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("de6486187b72e8c5ea8fd6599e268315dd277be512b6f88c9a593a65b8fa1573d5c297df5182fa9754911c0df37246407869770fecfb1a8d447e5eeea618c13abda8f15c61586420f9aa1446126cf1fd4b96bb48841f9931e337e236e2d7bdfdd91c02b0b2abb5f2965dacdd45bd1a969b215bb31916857e3144ad72c7030ea7");
        bigint_t e("d4ea9c310623c90f1c04c6c2fe9c0adcf6e6ebaa083c4cd3b8f7e0bf7a2245fe79b02ec78147e3275d82222141bb2da8857889646ac721582fe23999ea86b5a2c843f6674bf9cb1c6a0bd0e13ff33c19f0cbb72ff70654d2a64c460a1188a26a4beb414075682a595c90939b");
        bigint_t d("024110ab2212a6ffb81008ccdb18c9cf2398c4e1cf8277aa59c162d69a60585624fc50e3e80dd9d60f4ddd7168b55f3cb533dc2f96f6d4720432dd6e9e508c6534d6ac3b4f07246347a40d3af8dfbc5bf46998810357125871733e4eed010c632f53b28b4226e09b73e0cdef172a0a7cbe72320b0f1a8aace8149c697486d853");
        bigint_t c("c03c2f116dbe4a1b79605b462037b2f15147d0e7c04a1e3f37adde802cdc610999bf7ab0088434aaeda0c0ab3910b14d2ce56cb66bffd97552195fae8b061077e03920814d8b9cfb5a3958b3a82c2a7fc97e55db543948d3396289245336ec9e3cb308cc655aebd766340da89213834dcf1a759ea57fd22094904229abf45d45");
        // Result = Pass
        bigint_t k("db782b4e3453d70a5c25ae2a7e372cd7373ef6a869117f03bff989f4637ac6203e15509dc7892a89d3045e24584a64d7d22452671b04bba05bf004d2238e47e50e73ea42162e152d0415aab5668e4ffb5fba70c2fe467d36660ff4ddc2784e152b9d6a41c3d6046fcd3d7d40949533d599f8d76d6d45e97a18c759b61b96ef29");

        test(n, e, d, c, k);
    }

    {
        bigint_t n("daa9f6cceabbe6d4c3a746ba35cf73e2ad8ba33423e440588068a6856461b119469faa5ae8c34a1706bcc6ffccf87e2bc7c8435c4db172c2ddca00f1a2670f5593d864f20d131c84e6b6d0d2aca8a53393f51d70283ccda0e6fbdca9da2bc58b8917078ef33eeae27e5eccf04f6ce6c6a67f0ccd23df8e138e368ac55f0319e7");
        bigint_t e("9abb97533bbba5f03f42a767d6760ef258b59e3626b40082cb77bf35c0731d6f892df4130eaf5ebafec080621a8d9456e345c4de8ed636f293e94ea277274ab3c9c240eaccd76c776d8114a306f79329139f632c98e34b03de0ca0ae677c56c623d0f0ee9ed7458d66049cc9");
        bigint_t d("07b99416fed71aa6163b2460498dc8b04307acedf14c540aacbe0e11099fdac638a7aa5604b435a33a1d206a660ef5b4dc10a9443e77656580d5de7cb6d88eb357324bef2499fafeb3889f21f53c16c29286689c4dfea8c230bc301c8c81b38029b39e28ce53a9d5fdcbf5a913c77cc20404bd162b85957202cdad19b64f5559");
        bigint_t c("c220f5e14b2f072452e9acfcb1502aa7859e25463236c37ba7da0e36e31e9accb71d674ace6a0808ec4a058173207f612b2812e04268004f92e48bf02312a20bf18dc03eef0d12154b97d8dbf4f10daf5a594ada7576e85f088f65d427fb0871f7d9c867b5016b72f93aebb23cad1db7989994dbd049c4d490436389f6c31525");
        // Result = Pass
        bigint_t k("96d3e28be2e679094ce939556304d53d28263480b6a67108dde9a3ccf4ab67c5f511f9086ead5f04d65520f106cc95f9d933ee3bdb3187dfdaaf1ebb601e9d057f03e1bc9b4e02836085b51e8f27080426b6d2aad7adbb8fee8bc67246329e168b93edbbd4680b5f877ad7993ccfcaf09330b16f90b566b18b9fa786842db2cd");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("946fbef313db4feb475be7623aee647290d318b912a58f4406ad3223cb91ff639987bfb207851d249908547fba3b10437e25622238f0bd1dc6aaba45dd0312e0606766d179f2e5dc9e9b0f764850b6acb729282f1056860bc16242da9fedb2e0a26c2bb8485d1ec78cfccc45980b332f16b331a2ca2c90ec38612babea7ae94d");
        bigint_t e("a6b6836f2ff7266120779fa67b5e4663cce025b16ce02864819d1a6ef480282adfa8844e290594dac85aa91e3efb8d3af0292f4401d18d5c34a652d9e5eafebe6a8330c67004e4649c2a742a439ae9a9a530c1b060238f0521a244e581580bd9fd7b2261b3f5e173ad078a31");
        bigint_t d("23e28b6c311d1273ede5f035fd6ef4bfdde1602b1ac633e4c0bd76f0fead6b16559854a8c4e22cbebe094647af7461d8f48a6b3fdae287ff605cb0802292b47f59b0e5a5f0c3cf0950e4883fe62c7498c4cc2ff1683a2f911d42265311651aaf7075e79eedc4167421fc1bd06f048ea8892d67672724612230ce866921c48ed1");
        bigint_t c("4283055abe9a5eb6b2e73d063a2447ebc515fc7477603c91cb9f09ca7db48e30a75c60942a0805f84fe8b343ae4859394f1fe4a910e5bbe846a68b7fa7554a21686276e0a28e9ea0faff24cb818f4146d96a49a49ad8319af3574eb395ed5f45ccbccea4efd73395fb541a067ba306fcab1eb6ccdce0a68e1adbd9cf0d25d686");
        // Result = Pass
        bigint_t k("7d74f307217927164435cbb94b3b1559c7504c81c53e42f7481461b5f073b38e89a92d34aa2d7e992589fe2b7a32a72666416d6cfea095e60bea7f4fc62ec68d430a2f4811834566bf1c820861d9c5d6387ca2f2e09d3bccfc13a7e38361015b084d1a8b766b3ce1796a69f49a17e924b9010a216c966cdae86c9df725779746");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("c808f0ecab4b7ed4a221296651aaa1198325e6a84c831cfa575e7197b26a2c88803a8219dfd4b1375a2725624e4117a775174328ca3ac6f79e512a021c2fcc13cc9fc854e41842c20a837b27c6e282fc11e40a3535121f0d314a50d0ea5df1c3723e0185474c073e17587ca0489b8c795aee3eaf0428fab71623f86b4efc6a93");
        bigint_t e("2bbefc263e208c3b464aa3728f017932d188a998a10dd18480242453d81985e859cf4060eb9b613400d6aa81313430c0359314430cee62f354d83b1751dc39e358b0239f9e59ceff6db2e18f02e6ecc0d5fbbd09a662bf76ece012141620bcddbdd20a798d11ed9a302f82d5");
        bigint_t d("07f5bdae6528c0e55abfb79f96aa63f748efc95c05e406b067e309aeccf62d4e7fc27839c0ec54ecfef42890e211af4bd23010e9445585dcae485d91108eff679ec8d578bfe06f475a5fc3772d37ac205b5abbda1a903c4e19f4d3172197d94a2f342009ca3cd307c0eb0456c91f2782193be1c8891a6411c5780ce7f2fb551d");
        bigint_t c("2e274ca37cc810a0e3b32901feed1e7eaf9e0134b35ca1037aac003536c608603399798d660f2699489261dce5cfb4092fbad25599d2ccdce1a688402d9870935ae1677791d501831b68f3e976f2c856a82fd61ad38a71890a9b5ea163ee8aed4d06fca824a10926033ec770daddc42fc006f7cb8a0cf6d39c936aa0ff50c035");
        // Result = Pass
        bigint_t k("7f963c7f56c95dc3e8b03942368989fa16ae8c55c4c08cdce415b10623f8be526110a353f3e2887e30472d878945acdfb2cda2e7f2f2de22c6361ce9638020a142014b5818bbbf73564f9112f3d7cd854f81c16d5ce36412fdf0efe0637923d62ebb971bf47d31bb2352d7d6bd213d4ac0a40136c2017a48d742bdf11d6af856");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("ce5b881879dac0e0cdc0c86279e0cb930e56f396190cd5e4b23c1d48b22ca6d5c1f4ceaab5806c03e18351974dcd07822094b0d99c76abb45b79a302e428d621858a0997ea8e2b254cb494c5ecc404781b0d6db70bc8373f21656c862cd70d2ec616f7bcd808ac2e3c346c33f99dd80d188d659ea22404505bfccb3844861eb3");
        bigint_t e("74dfc453110baeb77206b4c8d50f315f36b15a1de4687a971fa8f0f0ace78b1f0ce3833c0732a1e13feda7319eb8e4637598758e3ff0262a9cb0f8ebab4d7f84d2361ff6f67a5077d53ec036950502e6378b4bbe0f9484746d69dec6d731592ec1831137c1cad42f3ce660b");
        bigint_t d("401a65cfad9dbbcc9517c8fff3d8df2f1c8e49086b31fbb69263373f00902df13f5d564829e976c8cf3172e3b7a9e8da7dc9468b0e3a339c25699aa8beed58cfbebb2d8169a8ca2d43d902fac5fcd4168fc668cdaa7ac19b8ccf1f980c83d4612dfefe95c655dd2d44ddcdab795eaf541ffa68647bb0f289abaf6bc3df9e87eb");
        bigint_t c("56cd57972d983e4922d85f82e2976dbb552741f4c8b521ed4a214399560e8dd0b3359f4499fe13b61551036aaea9ccd8496b12a44a7705d0fcfce2313390bc1f30807c71c46427a9b9f75f6b6d262d34c067fd48304ceaec43583cd626112fe29356f706f7f30088d5737a6e6536153782c309c8c35e5aa9e7de5a7176b96f7f");
        // Result = Pass
        bigint_t k("08243ee5974847212303bdffc8451208bcbadfd9a4793f064800110d93d3811891d36fec6b8bc007b24e8381373b6482a1f09fe5ba29603bc9827804dc93f77bcf9e1f30e439d6cdeeede0b52e2fd98ab280a4183d81f997e29b8ef779052c283027b10f9745aa1b0f66e207088020c0f27a61da09ebe29a86cf342264c9331c");

        test(n, e, d, c, k);
    }

    //{
    // n = e30789a24f07e1c958ebbb76d3512a6f59521fd56d26e5cd090f37b3fbd4375f2d0dd52b0138c7c4becc90f113e02e33ec976f39ac6baab1f9f80b1baa7544231746beff105323da123e5815052a90169eeb465ce973f25e5a6d124b77685b75d3e2e764f627efb32f63649c2df96fa4d99885dc30b9ac3d37d5dd851b71330d
    // e = 38866527c4c4e12b06f42473e3e3dbeb562f771c1638122087011d7d22b2408959bbb1563c55124d43e54d3fbcc1914cd32b96d0e6721ab8e35cbd1f85927a3a538abb4968f9244700df6720ded241f514d48cd668d282470e7d6f543300731dd31d29616bee987bad9326d9
    // d = 2f4da1568038e4b1408b2f9d94a229665df873ad38b1c7f0d1df619a13cbbec368283539660cc2217d97ce1c1460718d1478da990ece59ced6edb7bc786883e7369c83a009acf38a2d1eb6643511dc13d9e889fbbd7562e49d22f51a69c957bc4a5a30668e092b2bcd88d2b4b8869272fba0068068e49d8907cece5173a0e403
    // c = b6782051716f0677b2cbd5a69704b2b99e712765b80dd41b9f7a622f6a46b9395f6e73bc0820900589c56b56303fec444e6f005e45fb460fe34e0cc6eee0795300b70b95952e455263f5a166ea59cc204bc512e95d891726fedbc960a71b92a2445cd223421f8c25e961601decd9b9143e7b474d2840b02899298246bd826dee
    // Result = Pass
    // k = 258552b857b95bf5f4f2db3224973380799ae77e062004e25b87fad17706d5d9ec5762789e0197d9b9fffbe8ba60a4cda0b35df9562ac4a8e19ba86832b386328c07a1f663ec8477f7d14295dbfb918fe6f25ca0724b618852c290a2e3f66abf72170560c8e1687fb8104c68c720c78f552d62d6b25890b6afee769ac9e5d6e8
    //}

    {

        bigint_t n("b0e1efdb536c975763911df7eeb139cdafae27c5ea29610955638ec4da6dfe86d2a9dd2bd7c93514a51bdac4f74457255077d1911558b714f91c10749242708759defa3ba10964fa180e7690d475773c5fff4240f0feed247e1cbcc732c214b582716c58d5e07c6b8ae22aeb0c75bbd798203d459847aae06c5b8a5c7c4fc249");
        bigint_t e("b6f97acab38255ba1e1811ac2944a9ff08e89db629787fe8322a2a72898fecd06210c28e5ee1450ea4848e9e1851c7f17e3779eb4cb7d4f270b6de12e05c36356f1cc00b8be098cf36a0380ea55a553ccf6e2ea16caa6f39e89c1a581a13c8e9e03e345bf4d284229a456d03");
        bigint_t d("0215d844c912d19149979c498f997b8660790157541071cc63fb21f284e0abb8275528c0e2879e832b7a87742573f22df8fb668a103b775c0ff69785dff3dbe5420d24ce3ebeb1922d02bd6e6cd5a3e1b12b3bc71d8cd3efeda287f40073eb49b1e69f30330a2d00dc8a12342466bd6622b5f26e363213c0c8afd8092fc2b9ab");
        bigint_t c("4e5837d9a2e4391a4605efef28844e858f168269a3d58f9e2ff138eda6a3207c64632fa2375b37fa1e1bd7b6508ea036802d77da60723a146490a7c1c86179b9d4a0d3a45f58e210929cbfc1ee7992489029fdd8b489b85479ce78ace473166a2675ed8822c384852af06db8092c6ce34772dc0ba8034f87f612755800c13fb8");
        // Result = Pass
        bigint_t k("8bb9cd33b7d4060eb75615bafc391468ec8904443ddcdf4e5edb916c99d6d0d4dfb736419926193495d2adb8bf5d64757a8d6287cc2dd12314ff1917d95864278b8758ae8ba6fad88d130234fd7b8e01ea1fe5b3c5fe2cf22b686d63812fdcfd438cbca82fd4bb8638c2add1288a2d4d7fc2bf4720598437ff08e3a889e3368b");

        test(n, e, d, c, k);
    }

    {
        bigint_t n("89a93f490bafacf6e029b8f9cc3af75a5d59664aaf5dfdc8adb6b0cb5afaf7f6389745efe9a00dbf0256fd128a660a0a8bfbc822e9c80de806d6ea31f3f521f80bde9e52ebc108e2ae07797f20f33adadbe23c16b241158ec81b29e1f789664a8900c4f5ba6e564315ae2c0fd91d3ec3eca88bddef42e3e0abf02494360d9bf5");
        bigint_t e("229a853be5558da54d189215b844fb02b2e27026f80c7c99d2c6404c40a2dda986050e33e926ef7c0fa60c32f92a04dc7bb706d52fbaa8d616ab8d1c139fedfcb334646bb7422dd3d2b45dc4699940c5599a8dbfcb983cae10f7202f7d62bfe612e6cc8013c743e33f50fbe3");
        bigint_t d("308df6730957fb2e41cb1e1be1b8cc5cc8ca79aaf767102aae845b9d3c6cdc0f5d10f14597765b9dd761dfe72d55162d78609288ee68b7c3543b242c8153a8151400326078a30be68ac33ca99ae94d42ef004685c539251a9afdc5f5ca8cad0e681197a375dc3d860579b95303a8173e2cbaddad4181e1e101a9794f87a49bfd");
        bigint_t c("5e89d18ecd20a77fda93bb390a27d2bb726ec5cbc90e31e0c1c56f3e97aac763e9b6e0a2c9827b0e0f526199f3539a7ddb21e0db718c54a2f99c6fb3e2d33a439ab1c280a77ec677f443b435681ad42cbda7c2d990e721a4b4288e08cc59d3f319845d709099f183306d9a549202e690c209510d4da502ad9b72b5847fc6f93d");
        // Result = Pass
        bigint_t k("4dce2499e166840ccac7ddc56fe2212f3d11f7c3487ca1bfc4406f4fbae46378a6a13021ec88a8a60144c28bbd7afaf53e81907801d94efa1e2a700cd5b9d66c3bd3e5e69e2820e8d37932c0d83ba901285430adcea16f120d264d9f22f1d2ab5ca6885d9d2bcf9a2f866d5c4abd616111ee59e17d91ce9906e39f710c317afc");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("d9a098da000ac58efb7d6c989b7468444d369d9865eec8696a4bc54ef47f4d04bb5fb531134d9edc1254c9665f96462c190a1c47ccf970b3a00a922a37e5ceb548ad638c425ee96310db234a97ed04342de813b5b6a4eb1ace5fef5b0c22e2330e4f285a76fb3d955ef13df1e2288c7e8974220f773b436029e5a8b03f7c9d1c"
                   "8743ad2e6edab6604fae212244039c0bc2dffe1fb8a37fa8b935fb314764c76f5aba87e9d5466ec413ce3a72af0eebd0b483f2c298b35248538823d527f40e221557c8b25257a131adc7f81adf9b481c8c24405356ea42009676bd6606e7fede6b76a6a264a97507f15f0f12dec9d7bd473af9e0ae86399cf1f2d4021f13325"
                   "d");
        bigint_t e("d107cf43a692284dbe81e069739b0fa121f605eecc587c058405146906b56e1042a19196663e14b4d6d13288b708cd998db8aa5ad53966d547509426a2e28946ae2eae99c9fd7478de2dfed90b185414b2a83d184147cfdcc184314053e655aa7c5af0e065f2eb139dfd0ddd64d878fb87fd39596a804d48403e12a1aee476e8"
                   "db65a9bb15f50881eb2355531549c1f26832ab8812aa10858b24832e5f8498b43249707d14b88fac7c238b2baa010058c2454aa891f5dba11ff450afcd26afc571858df2230ee7c33e86e814c52780128400e8efd60a3980557b3e95e8e6b01ce80d9a3a08faf1730d065ef1");
        bigint_t d("592cf1cebd9cb99616c57c2281851c02258ff73bedceffec955ab877e21abbc4e8488e836593ae9d1f21cde1bf089f1e03190752cd9cee2e66f478a50a154c654e530534bed03759e15b95f65c7302fc95868f37a2b438e640e25820a3f529c5e1f7caf461a5cff3db3d85b3ee920ba7b39ce115ce23a584e46dc576b81ba4ed"
                   "f3b4fd3d711ab36cb42608752d4ede8265f072671bf9652ffc9706afd6f729d449ea8f2a6be7f8c9d167f89eff4a8a5f6c2b57b29f9981a331204280a110c32f34f158e8bb0202258abd92f775747df6484ad66c120d6ef6242801a1cff83d065497701eb8b6647d69b4dc91c49ca434792a46b8c18ee70354add44d771db2b"
                   "9");
        bigint_t c("534d1f57d948cac580b88b922bc47bc3d64c8cd1262bbf0944b99833ec94d072c1a1496be44d47a9c419dc403855a4b1cb2bb30e56e0cc5fd557d34373d785dbe70d67e30355fc228a353b05432a40874ba84253af5cc52d3ab4118e8ca1e28e6c9c610760e753f87a15912774ccb80b00ca21e85926143c1ed8385a607c4e55"
                   "fa531f1f208bb3f23bc0c4eff4c272068f9939157bc61f5427cc32f017ef31f6363c8a736ec984da763ebea5eb94d83fa31d70223ec5503cfd97e598d883f43aca5e884b702a2f76d298659181cb5180e25faf56c9aa0ebe49413b9acbbefde95ec102ee4e351a8ff8d5a3fbdcee448ff466dffb45fdc0a0b3d31b3d192bb5c"
                   "b");
        // Result = Pass
        bigint_t k("4d2522357254b309b5239d3350d5c0c4d3b2f1967e17da27450b12a470c5283f10b7a2c33ea1008166481245bcb2480ccd4ac0155a82435ea44a2cdc92e545b34ecd1ae87c5d6fb1aaba9ea15053edf23f82c3918d9ec3133a52ba8076df5c918d166c90a605d1755b205d7e994103f8b1b9afc16e6b274e2a0043e0ad54186c"
                   "95a7256910f6e7adfa885fd4c67b1edc63e83e85f8992e4239319b18be8592030b79f4ac9e16a4650d9d7675a4f4564018ab85c0db5790e478df656f886dc461bc33c9ff32a0fd75a73a1c6626934c28c923a1c1efe80d59fb2efeb6aae82c0fc8cbfbf306eda799e0c2a3efb3c609156b2715f28c00ff58075e34ed5d327ce"
                   "0");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("dacf6b379186d92a38e18adf394dd56b2cfdbdb41b22d7c4c6171bc994df4d37edfcbc58f25a6ebf9ce9c4e979ca750f5a5243fcedcf97fbb6c78721028a1b6d6b6ce82d336dc1d8227531fd33103f3166d82fa55e5402459b12f79433c2390078e593a62493ec863622613a9bcba7fbdc09611decc074e4e46a8b657cfeb640"
                   "c43c3f2a6d89ebc797fa0ee76eb782145e8a983eb6f63d4613087b5c8f5a025bfd3580887a6bc3215ffbfb594a3dfde21fc07c8813ff97ff5613d26e4b6f0db333bb706b2194718e86aa22238d64517732b9cb6a203a9937bc960b0e81d695ceff4b1c52120d6905e010a7b191c9caffa2da694faa6e8c3cbe41660de6f46e9"
                   "3");
        bigint_t e("3432563e5c37e78f1d9cc0c6c5ccfa9d2954d79680ea59086d3c6ea61f913b023369caaaa169ce0f7fec2be6abc1d66dff512639d5e64d588e3a6143543a799a239cc571c20ee95eb2307b6809a677d4b83c95398bc7d07ddb2c021fe9ef715de99b66de48e791578d5b285247901acd1352c83f42a1922e814c48bbdb95b94a"
                   "7d304d6628c01f2a3abd74ccf75ee9a47d857fcab7bc4ebab27c94742ae1d486622ec92ad54501ad740938c97c7859f64ed5cdd95e040dd50516c360699e073f33cc3e4cc1b25c55218ee19a46dbd92f3bc6676fd417a39afa130c153ecfe873772e6996fd117ab2c34da147");
        bigint_t d("58f7f8fac9e55ffb2101af7e2223a58aee61cb1bf77352264401a3d129c18adccce81dbcefebac1244dc4db708a2cbdf01f012b3897c267664f8ed29c78ba2d76b46bedeb26cf05f4cebc9b6a92fba04271b635284ea5d353b4d0099a741f2d8d0f54c66ad309e3f5793c8bcecf796b51e8385fb4aa2dcc1f172f010050f0c26"
                   "cd942c7d232d8c74d58c1c41e8928e11eba852eaace66d21eb93c8b338038d9424c91e0d36d9ad028a6725d158f46812c0f1681067be644a30f385358196d95d61a8368a963b88af8af497c63155af579633f5ed24e3132e39e27db6dd05c3d30532abbbb655cd7cb2c211a708935334e16840df9e6e04fded7d23d22506f89"
                   "7");
        bigint_t c("42e599cfa9578874a7dbc3b1638a4d9094d131fd4321681dd1e8fce23bc16edd9de75653f5f049922ab8ff854d9bb5871ee7d175d290c5071326356049055ce318ccac492910b4f02e514776202cdb87cbd32ff38dbb31045b941e0e047eeb47ba7ae4d9ce1e9336bb8ee3ac66a78539a47d93a0fd0873bcb01aff15f694e2b1"
                   "8bda452d6b84ac0cd9571a4ea6737c44b0cd44a4a9e055d91aa01f9ca7b00bc894d4196a88a9c7189194368569f88852f8ac8c2619ac9d00a10ec7cc9e382c35dc51a6bbaef5ae05ea2d7d78369e70088200b14ed6d214db4e4c3fcd63960ea06b37364663d02879ed0a385095cdfe1058b3fc4567bc82122843cfc67e3079a"
                   "f");
        // Result = Pass
        bigint_t k("c47710d3fd466cf4d5de5475e0e21c0291f316445550808b1a28580d9c0330803683b6ccaef0825f1ff07a60676013e7eebc7539ba4323cc09a806811fc56878789a5bae30f179fc40c7de1751590090e7ab54d8fe20994dc52606401e18e199e5fe546a7f90c9a4b730992d3dad6316199462fc67462484f3aab85d76d438ef"
                   "988134761aa13cd51c1a6419461ea634bfa9b1600a738e9be44004846c0b368d051303922629bea38b95ab24011fc6c7207056da08c45229b15b58ce4bb433d2fe184ef242b712b4c246f755a7e3b68e18d137010081c82ff3af7f115919d0eed2b5c806ca71c4b2a44b4318de600a04ab7c8e748ca7cad1eb034be6a04342b"
                   "e");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("a2ba78dacf4a1b12888bd52b7eb4af5736a5f578337642ce72fc33bbc4ae1cb195f56c8158feb16c732d841e840ca1cffe799222849298605356d38ca6f057d1cc8fd8a5fd7775b792aa9c24edbd2c73d848d13f15dd48884b67ddc5d082283cfa43d72bdc4c36a48aad2543508f1fad909554e5da069472c92da84d5f004d79"
                   "4c8f743046c5b516d11f4870d8e1cc4e18a97e48350624aba96136435e7460c9b7e498c36a94ddda52a317a41958d5e1f14286e5cd31718e59bb1aaa7c100e812e68cae3cae7dcd5164b22d4bc84c52e9b95749874cc3d9dcbb98582170d5279425cbc8143d3d96141fac99bcdaf34d880557a531f359de2de618a5a87f969f"
                   "7");
        bigint_t e("f71e81981ba0bd38df957e82116a2d0faae23ff4ddf6e33b5e26d4383c16fb5ed9582082339c848a19f5f1dd124e9b5f1c33e5bca517e267f4de29a6615865619197439387112de739485f106422bccd1b56aa728d45410dc1895ef4b9530f857cdee32a16d66afe84b35074e21151a1a6f9d2a45a7e60f2da06694e458b7ce6"
                   "58cd2eff0b13bd917a919d5f9de068d9f38cac992e04dd91094312eedd7d9c24d559a048c52f873b5b0d54ef1b07580d8bc10c22ed300ee9b3d6a048afac9f568385f68d7f16a11b7efb26b3081ad98339bdc6207789b834a9491fb6b825a763a35cc85533abf6d4dca64645");
        bigint_t d("379587b1ba33f355932fd7aedff39631cf16d341a789cf294ae0057de72a527097e697f5f6825f04770f0df93a3864757fce983821005dc77201f7a60899ae3af8bfea6f8b8a37c5715a2f0e737f923d8b009accb51d59455e14cca7434e5a2dc9aef603c4869c7f47bf858fb0bad82aeddedafe81ad6e2efc2b46e8754a5e85"
                   "fbc4dbe6ef477e1675f27df40bfc8a8932d53c48ce61d1b72685508a3a64f8d52bfd870faad2596a3f7be9c520466e103ef6a6b69e0218c7531e2c62e58b33fbe309dd7e9292c501420dfb66368a9083cee61152adda247fb92c0f7e61169d8ce9e9d92b89cb6a7f5382e4a892a37d9ba76a7ed563f15ff3eab56fdc6fdb1b0"
                   "d");
        bigint_t c("2e5c82295dfb60a6e4a7d519544096779b86aeaacdd269e32404713245bc3754c4cd94ccebeeb135b7b324d624bc5b77aa2d3e73085d100f40d3960ae36e7c758ee3f4c5b56da0279fbfb213b23e5224f16d4027c3a8ba72f616a07703d9d6989487ea3d43def624a5b3c8f9872c4127772ffbed871a2eb44db5d5a02e650c65"
                   "dda2bd5c1eaa7bd6d077aeb02aeff127455cb8eeda1b367e4e987eaeeb7ae883711ab649cd3af7e228f4ac5f23f02acd61dabf5147139878ffa7e2c8c380309b58d91d2dd8f532f3b5100a2efb96b3c1d593583f536a1d496acb4b173ddfad549ac53a2fc842f3af80b40f46394856aaa96dcbe088898d9997eaffc2e1ff275"
                   "7");
        // Result = Pass
        bigint_t k("23fe7f280bac1aa67d089288d082b43f95a1ad3ad43ab408b563bb2c6193f0d3578baa3182c1345f622639fcdca91f3cb87ba9fbe717ec05a80dcf55fb5c1f2deb38cbf00eda9a1d60a617bc52c261bb054e3c5323ecbc72368649dfcd58be10f1de6692eb93eb778033edcfd901f6e6bde391ffb5704322c191d024770c0155"
                   "baaed6323f6390097dab8538ef3340260ba1ffc1d2356624630919bf668a46b04c7c86531866828a789d7b4dbb99be164d34374b0f0cb4c1d36d43122984a9a468fa9808ad6ce28eb064070b2800240321b45b2d50ba98f8ab067ca275e4302ad0f59d20151b263628a34d79adc2e8c2e280db80e3a714cb4cc38802acf6469"
                   "6");

        test(n, e, d, c, k);
    }

    {

        bigint_t n("c13dd5aa8d4697085ef847d809956cd8fa2c7057f206713ce61a3a1c7a56aea2a00e07d46a04479646e0f6cd306646658b5dbafb209036a0e9f61766d83a51cb6f45d7b38ef7be5483fe46b2b80a50b80737d8fa1f7f9987f0e2bad6f3fb5d8b8a4e7fdbfc29e4496c423f675872d17c307c016b4123a7c813ab507aa0bae63b"
                   "6e23157b08191b1afaa11e85b8208a0bd4a14795ecde7608feb1f51d0da6171b2e2c8e4fa625c629a5b5eaef65a0a71a6c08bdd8a3cb2e9d1eb4c4584689da81fd2106e349df8cf8f50fe9cecc709e1462e3270cc9f934223ce463bf955cd68e36502a059895d9c45fc0d5029dc009d3b629380ee7477a1236064e014241dae"
                   "1");
        bigint_t e("fe69502e23f2b2be9056ee6b1e29e3e826f3a43c2ab005aab19c1a0418f2b30dd1fe6e25acc0107938943c961316712fcde09271a73942c98c3e6e906513b8e5bcacc226294078d04ba3a98efb8c4f86f7938a9b78d6563e8daed8ed9475150374c5ba5bc1ff4120506f121a87cad8cc32f2372351c33164d1ad850eac14bd9f"
                   "80e80b0a4537c661d32054563ab01456575fc672441ecd693a8dfa3dba4d70c6ae23fca6db43882f313201bf047dd07800d256afefef3c8954c54fd291a2ff5a95a35bb4e6a9e4ce0047b71fd518909bafd625d9cadd15e5db6f103f81e14229fe6fd48e476cece911c1db7b");
        bigint_t d("19c10372f37b3f400cada8e5099fbdaa5cb0def2adb8e414b78910c74882e11057c4c62e2cd8c8f3f68c3b8282047b41d552a4f76a847eecb1c387a97a133a0762af39cffa26b0904c26410c223a05a2b44cd74b3760c079238e6165fdce1beef815f900c94aa3ea3253d29d9b8d79d40b83854adefea288ba73566be5037048"
                   "b1888b4da89dc712d6ec98898f8584ebcc19dbc540fce1f2311f2b2433d4afcd33ae75fc3d3fe6e4a575982872901f100132a13aa5afb9449db9285a28993325a551532ab7878faf80749972c4510ed504ac55a9d0dcbd2afa4feb80a2f52aff26f79506dc326f535f66397725e7497d77f8d4ba6ea03691fd4e1900a7f18a9"
                   "9");
        bigint_t c("67a8dffa58643b7387419e9b5c1d3e5f5d717e2c96286d5e3eb28ffc9754e1a216c217bec4ef28ffe4240c0bd650c4713b9f9cde5a4dad93c3bda51631c5cb28b54c3df811187c9662e12baa03aa1466ad516cb9e36458c68d36b094e33202078c70da0f0786401f4ba183e1aad43528fbaf7726f81477dfe54636dd73438f25"
                   "e354756b6ee27c82e58b9a1695752f9d6de2448b630510c51315c159a99f796c022096740ed336a779c8fab28a360baf4b125b51eae02e625fcad8704fefc8c330fdc592af0278764e7f2b1e52bfd045dc6645e1564dd69d108d048a2bda8411b7138b2d181649d7aed9b3c1b5b78646690789a16ef3125e7088cb0f0709b64"
                   "0");
        // Result = Pass
        bigint_t k("8a5fc5cc032f0cf4d9cbf2a303fad08f22a2941b25324f56e72e546b0a0d5bf2373afd66b583fc57a26c4470096d7adfd468cb209fd7f1ddfe3711dfd0409979bd9eebac1264d12834691a60e806b4b5139b48789d9f7dc7ad4780cde20ba8ce3e6dc1216ba441a095994c347d530a046d28ba058f8d3d07f1dca451c9185923"
                   "cbe5f0ee647e14923eb4b163c5719f0ee3faf4acf2def1683bc142ab7490cbcf5e983b55084cd19cf76ff86221bc64595af589b78217be75ad61443ce0a71114ab490cf97b14509de56c69e4138c8c0c3767088ec35c9865bc5d4eba2a6587859b683133a039730add26289418b30b38b2503102634820cd4269582482610cf"
                   "9");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 6
        bigint_t n("ba1faaa97aabcb0b18afdf3d1f5d67ab589c9442f24f8ce53f55260eb8d6ed1ba72d102d4a8306ce089810aa55df9874b4d85d57672e11816ef895d6754aeed7baa1805a7c94135678a0f80ed3858c2d5e60b331c1ee648c064deea2278e3c83c38a318796bccd5293ad99845bb2dff97d7a5c27c7fed5b892dfa0da9c231ad6"
                   "7ec374f86aba1ebec52b3104a1c9aa294430f639bfa6d26e8112a17866eadf3ea4b437ecae54f4203bd40dc2d196969079a74e9d20e3d69a4a5eebed2bf4e1a92c254ec04341c8c84212d6dd28a5293946c09fdbad4ea0953a43f29372b4f24a98740937ac93144c7095feff2a7833f58bb268bc55528d49b45a4307febf659"
                   "b");
        bigint_t e("c27b115142cc3adaa5f76bd75e372aa5d28145dffde61a9eba87861121922a4105d7da4f65d425700b9dfb8f74a2a4cb1c576fb353ea27eab88fb6903b2956e19ee24900b25275e7277abf7e5de20674eb85e274c4dec09df8fb50e1402f38af403e9574a916b419fbe9fc104f4492857e8a915167b0648fc94495a92925d0c8"
                   "c061735ded2744e00c39f3bc3d0d8d73c155ae075f032c1073ac8199501907726f4c3fb72ab4ea1951712b08be6e7f9bd6aaef628bcfd1861de45b014983c7c2c93d4dace794bdddad00436cd73b19e6ecfda766b7b9077a770021ae0b1292be562e5c2897bdadc5e15cd2cd");
        bigint_t d("39438fa36d7ae8e05e5100363b19e4e706bceee7cb1ca0bb70b4b1ca3f78e4d69a16e0fe3b7c9d5ba831485c8f70f59be58987033a36da6818c1b8d31d864c615889342833515a3e85e968547fb885e69c8f1210a91fafe39c07879b4f8430974f195d8f6239b2ca185e9db75008d6f9232db050329c357bcabe8c01a5d2a3be"
                   "4610dd59c9e2916a53661cd8725feba92e84e00f2d2611d2d4ee913a6422211ea9ec326c8c6f8fed3ce9acd7c5014adde49cd987f7be03db891ec5800dea1cd997c5a174dfd92bff243858de92d033d5499666b95df5ddcb029592d390262a49eb7e240cd52ae76b3188e620ccb9524d2c71e323e9a4fc78269e678c3836ec4"
                   "5");
        bigint_t c("7d4da0516ff08f3fb62de45d47f461b3211a1466dda18a24873a40c2ac085a24b4a7975e726d8518faeff137b3c7a2baad9547e13444ad95a58f350480c3868cbfc67fda66586f535c9989b279cb92f3e05add718f19794b34cfa90b42f2948a45b23e0e733ab7b683329318c28899286370c05c16a9570ddd0283211b1eeee7"
                   "ee74be42bf99c40a18c2f6afb585201fdddfd6eaf37bada44831ab8c33cefb6a6413e5be7500fe16c2529cc07b4a8da1f7b009654b18e4d91c640894b18a23dd4c979dc9bbf4cda42ae86c923b5f4a7659ea3f134906657201a18483bedace065109cdacb9ff997af78f4e6f1e4dbd66aa95623e12cf9737a1f2069f814764a"
                   "f");
        // Result = Pass
        bigint_t k("7f4372b8b758bde011249d12ebf344b5ca74afcca9f94f15a1c00d164d5d43da9ca8021c5f71f4a3aeeb9d952c75515cec27c6b988d670ff1e1804f7ebc6346e25f867ff17e430da93160db5ef055e4efc75329190b6931d0e7ed7e4e1288dc50d508b74e35863c4bdcb9a1af9435d4153773535f8c304809ded940f6530606c"
                   "204f8ae6dc55887f374747cbbef4f37c6d0dfc1fabd493bacdb05552f9a68cda9e5db9bbcb3be2dcfbaed8a06d56e14cfa00bf0b26de8a7831cddb1935327a7426ad7c7d82fd5b5be3096e95114f81ef0c481abb575e2f98886e293b1acfbd9d44e96d046ccbdc22fd193f7520033bfdfa1d4472a513a4305d027837051abf5"
                   "d");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 7
        bigint_t n("bee45e6ca63b88af2c7c4010eefc78ed53c112ee43f0ed6c3289ed4e5ac04014b9e6c9b8d3922c14ef77ad05c0c7853eb084ead872df7d7a4d0cf6eff52914677ce5b0e5ee703d17028cb7e7e6b381748b4b5eb7adafea963c0c5efabda0ab2f2bcdbf97ec6aa8f631235479e54314ba97c538895b750e7eb558c4d673716ba8"
                   "45fbbf7184062dc8f1c839f1a266b698934133f7b633fc75ebba50487165910dd43087585378cc0aef6317cc3ae59598c9f569c88c26d02afde784d0430461120b73577f0ae868b2be68bfd18a95af50167e759afdd787d80a96805d7aba0419e1ea89415c51cd056ed66a20d188e366cff495354cb6b4e3f18d0881aab58a3"
                   "7");
        bigint_t e("72ef24093579f49f14d57927720bce29049c3ed70911463917edb77f19948765849c633039712bf084de1b5d10a110db0868c2958e258363c3897568f2bd2e1dd018d61f273fa779a8eb53214a5d4d141b8146bf322dbea6d177492196d07a441c638fa6eb8e7e7fd233e9cefca1ae404414a417cd554f4cc99f2d022738874c"
                   "0330e01d6a63c4600dc01d8b49232a17578f41aa36cc64623d8b227ae135fe8f2dbec28c23b66245cafcea80015980b7b31b07b8485a4c94038b217b46174c0c637f82f23522a380c0d33ff76e10cd46b5c892110350f634f3bac00857834a3cb8ddeb275a552a933daba731");
        bigint_t d("4e8880a99b5c105bdf516adddf5e05b5521cfd0195d882591c299e8c8cd9ebc5eefa627b9d6056fd50c29d384fc83316b2ab7a22f69438562288076dac63eccede8f342ced61b754912490a146c91f93cbaf02fc14f2b7b3d3e401e6f04d20d6d4d7b7fbd0aceeddd0c0495e7542be8cb0eade3f464d529a1d5a0c4eedb9f98f"
                   "267338192cdc9f91f42d204470ec10adf09033339476d31330f7257082279444bd476db695b86bab784a940fd60f762108a785a7831e379f399235e7060fb9f89b77ebdc8607880f2952aaf61e8a8f42ec47302cfc531111978a2b7146006f793c9023bb54247e4620dea4155da4e4da3c4a25c9559b1ddb5d4ccd61c69b685"
                   "9");
        bigint_t c("1020cb28246e854ff177516f45a87cf3e67830af6eede87e596227295d876a8bfaa95bf3513447ca3976d27690a5ea7dcf24f9eee1025ffd36c41fed1364d8332c99f8d874592c1f349c1b685a64535414fa832a9e7aec79599112ff1c86b7cace77099d35e61b950974920c4deba1bedf82372a4d5d763cea54682821f5ed99"
                   "0accf80b3be2fb75e183a02c0f44ba0455457bb895b4e78c12928830caba64e8061f2cea2f55b607e353ad8e4877886ea0cab89a1e8626b2f8d5dcdfbedc02ffeaf80d02b8473393386b22fba18af143e899579991db1bf6c6a4cbfda663b125dfdf031a7fc15a610753653bc187dfcd543bbe7d96bbae9fa188bd51295858a"
                   "3");
        // Result = Pass
        bigint_t k("a0197aa5eec1084f99d4208de689f1bbd9985f47f1a1f251d197840a542b0cc864b4079fa275dcb20a6f31cca51303f3efc2250dd6790f33b3e55d77723a1ad9d518f6b6afbc5b16101b19b3fa3cb58758f7a10d6dfe1256610ba2d8d212d9fd0824d2d800cceb0d966f83ecbf7f8dbeed714afd6fa1faa4620dd7caa1d24c3"
                   "c35db1b585b1623796721d23dfa41fdcd59df9f2273f6d0e7e542dafaedd2998cccfc6f35ad0dd2733bb6b06a675deaa515f4ac964c1a3923d362848687b7da4217c641cb72de0c05ab9775108be745179cdac78f1f313b66609a2a5a5898c45551cb7917ee0f9db0730d94dfa7d2ffd8f66045b579aea44a9d0110fbea32331"
                   "c");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 8
        bigint_t n("d37689a77d930ac8a12af47830e94b62e4ac68d670deeebd675ddabe059b5dd1a788828783891cedac28813e069a3f417c6eaebfa6090b5e0e10526d859b46de9ebd0e760c579a83669ce25622306a8e7090916dbf7c40b1b03fc013cd0198a8ae78838c00b8e90cc9d2343095635810f546b9332175fb857b57b88fcf3a56ff"
                   "506e9c81cf7c11caae57044ed2808adf37a15302651e389a1d94ff4c7f30ca1ec481a8f72b94101d36914e8f3482adf9928b7238661f85ce087f7354ce3dbc1924173bb603d08160bc00b8304f81be0470e221c503c2b4ca5dc3d1c751105944ff2428382d000aa2a20db754cfa224c91368aeda229fb73842aa08ebfd27551"
                   "d");
        bigint_t e("f6b3cec45827bfd05ebedd0fc703a3c1e74c941ed566e196c5299a76853289f6c9cb7f96b11f2a040958b184f4b09a437c42863b5a2d0231770a0b2e1b0bd0e77365e4cad226b6159f1b82744fac0a2b0126b2caac3c5942312293c138cea7076e1aa2d854567679ad37ad20a8bde472518f6777f3c4d21f42ce9edff1ce64ac"
                   "7040272d6ca8e0a5bcb1909aa2da00f350357fe76add7dabec19d1543ca611ede38f0bdd9fc2ec0983ea77b266a7cd90e18eb48fe5e4d4cc7965edb46c23f5e6b40f77e9360b0776889fb9252ddd2b01901d522cef5d12aeb5942b6ba5c44a0ac5cffd705c24a0c2da437c45");
        bigint_t d("0ec2f1aa6ef8891b896d8845fbe6a20b77b52463e4f97b8fc6e4aa634e245b4a6cb6ddd294a4f121884c103a4326715d6e0d7b8503e374bd41b0a48485e7bac2ed0f5848a5f43baefdc9812b1625570cb48a4884d0b1af5b78063c1783ee690ecf603e97e24a6b1ee0a12eff3f2521c6cf7962ed4df9c0d707bcfe534f3d9368"
                   "e59568de626234a475e549b48ffb74b6ee527f5534cbcedc0fee133631dc2c6003b0e62f7c655473aa72b69460352a0c24a1f6369bdc779108d27f14cbbd302f87dbf5107ac37aba83851d2815c4dd720ad74906e277d67bbc3f3ddba9e58bbdc273a623b0e51b276fb89741afb884f5b57413cb212852f5d28fdc1cd07ca56"
                   "3");
        bigint_t c("bbdd187566e7156cf520fb4aeacf8137e0f53bcb1e8ac9746c0a4f18263025fb8e01cfe2660d2f79bff2f9f20c2168ef7ae52fa284a1b332c15977610173d76d99bffa33fbb6029a8aff852fac38410d20f20b0092fd09ea6ce72165f0bfb1a9841e81b14c69f699fe50884bf29a755a7925b72e7028346215bc366a199b1a79"
                   "f6274ca182ae733dc4ece88c07d16d9e2e851c7447aa9d6363e19cb9a48f7aa397e1434ec40de14d84db8f3b136590a1e61a20193f09abb4fe5d3d9aba233af010544fff3b0fa892e526639f3ddc452a4aedac677fcfc51f8f38ff5483e0c028098856fb0f3b30d490d44e02aec0f6030205a9a43083556abe7acc31264d751"
                   "3");
        // Result = Pass
        bigint_t k("846741525e22175ae809d4d463563d84858d84c1aa80136c78f68117f878163141a4d7284b6feaef86d6416db745562fa3a3d24b56b6544bd33e87dbbdc617142d03123ebaa6364b34981464e3537125128ec459d70d9120080edd8074c8b973c046b10c40cf06abaa3664190d73203ea4b5ce6d86d65db806c9364122820b78"
                   "15693b1da9a56c5ac0f26927c0f72d57736fc61501b890b8416ef2d31e613ef4bbf39e4d1273ffe06466882bba9e3f50333e7be08d63104988e6b4531b1a0b76565fe4c0af8b765dac2dc217cbddc7d07cd930748883a3847a89dd680cf164d5701069083d43aebc0210746392975e064e854cf0e2de3f8af2258c5fd015b97"
                   "7");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 9
        bigint_t n("cab0e5eb5d84ff2bf60980815098f51fa06eda89b32b330b43fe15f4c14fbf72e3ce364fa107e9f86dafd3decb14f817f37d71c7c94d0fe11823e842117c01b48445a5c5d40d091334f8b7939868f6e1545c1f7153718cf8bd0b4b953e582bde07061f81a0940f3846f97621f8a459b234f2cab4e0ef3f35074c894b514fcc81"
                   "b7ebc337ebc70a7ec53f4e8425b5e6b367c463ee6467ba73095f75414035a06ed614544cdae9eae9d22a84e9870688d38f29e178c685833a5d899e6fd1aa6d316d3d5cfb716521222566952f9097ef2e4d946956ac52d8e6ea48f98294884871c361ca877b304aebcca85c234e08279736e4e890f1a2e45a466ea622f26a231"
                   "3");
        bigint_t e("724ff0e239421813985d3f0dd48eaf71a3318fd42955a451a8e75c696dc40c771d9848f6f38319f0df33b2053ffa6ae579dc48d80d2a408ebd79dec650fabe212b20a4138f16bb6cf2395a1a0df7935b9d424682f2b08eafd13cf8aa40b87d26bb1e23ef46ab94dcea227507d931299c1b8baa6241553b10252ae9411f58887c"
                   "0d796cbe3035847993029b8a7e85f35c892a59dc6eb3a2391182ad6a367d08ad636f1cb75b0d0db64e8479ca0a43ef60a3d4ca0ebcb8edfcf34520f86c43a67487dd4269d78032b9f4d16aa6561cfeac6a04a5727e04552139a2f6a5716cf81afc431d23aa91de3e55153d9f");
        bigint_t d("23509413005ec5e23f8027eab3ea95c3a72ae0a28a164f24f5fce1036fa61e4d5d2432c24fdca5e03f0b92fff04e48b60d658bb557aab7dffca36cc9e49ee1eb0396b4235746a2aca315565891ff71e050868443c1e68defd11b361ed09e8ca7e4f0e7818c227d232ccfdb90d25238a77c0e02368e5e2dd4e48271ff2edc76dd"
                   "838dcc4440b0bb285a0e8481981873c378f84825386faa277c715332ea95f87fb5a39a906337c7c4c83cf0e3b851b4ef0c23929a754751de106bf0ed18f39001905d0ee8734cc43921a72a6b9c6a7af22dc2b11566bb96e549448d634e8c5f565786d11ce94b09fcd24cc85486d7396737f5f0b70dae3206e09fb6475bf6b56"
                   "b");
        bigint_t c("44e9415108a149ea6d200066568e80c6d18d745feebe857fd2861553071230081698bf3158e0ff69090432399e74953c583c91801a8a3530786ae47010d54b891dea9646497be49ec3cbf892929631592952bcbb3bb508bbd0777a789959635580662f588479dfd0c4ffb93bd9fe3d6669575cd6f948e5670334403349a75eb5"
                   "6895f02fb0e2d7483327dcfa1cb4a2ab43d1d89afe4ab37d392a9c69c9c626f1fdfd429276bfb64f39cccb9803be4670dd4a9acfefc35c4599dff7e1c0bea15166288e4a7e17632bfe75eddd352613fd5f49093d77bbc7074cddba64d798b91dcc9e3a206ff2c624a9aaaa905bf4f099f2578dab3c3bdc0b7aac4bbab65b559"
                   "c");
        // Result = Pass
        bigint_t k("a21bafe520e650976d04ccfab9eeca93456c745e9a82ef91df95c36ccc731084f126ac2f74d3529fe38e25c1d1fc78b601587d8f79db40d26a013c0cda60211045e282f218821f5e85e4760c75170a34bcaafba531fd77f2490086ec16a603990b92a49fb775067f9e401714dc825b3f01674653b0cd004654c3580de56fc794"
                   "22c8868863648b9efe7ad6594074bc70e7370996190373b36bab457dc2f6b0744a6574822371481e5fc6ba49de5fc21edb337f2131f194f6439488c3ea6f2c750458dfc93721a381a4c4e0f21f4c229b1c9428d80585323cef540f3bbb1bee23873fe5d431219a0c842f8178f8ab5f69a2534de585f0bf5b1d2bd09e502ab7d"
                   "4");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 10
        bigint_t n("b39b390eb567e531a8a9a4616a45dce89b9a3fd3e9bc592bb83211374c9eaef8ee45ffce12138b095514b65b859a034f6c296f25f168a8c23a7c144362675b488fed726b7ed1cd99a2bb4a41c05ff6954883be8110e80f753d39543f291c3027125d07c3f337cc2c43d6d7c1e2c83624d7a17f25ad1d1b7737bd022248c406e7"
                   "ac028746d31257fa390ce506d8e13fa4ed3dcc69b63ec495fbd6ab67cdf33e657d97a792673d1e94c88a2a87618dc21bb6d72779d546d47f2b5ff4f8e3734a3c4e7238d7a4c6701990df7eb9a6a15f42c2f6728b6c8db7fda5b16303ea7bc3bc803a9689891b98d37930aa0c137ee73db52d9b3fb35840e90f306ae51262b00"
                   "1");
        bigint_t e("e8a10bc0a9d9973d2dd712ce1ef77a79b7aa44ece690ff006298a891bf72c251b970a3b14f70837877517a73e8d430877cb29e155cf863d0c2ca23d1ce16733773c8f71dfded55e3a278f4f90363ef5521a76eb87af53dc003c2e9660d671612890015248ad2a11b7f1b5ea633702a8b1747da14023b5b38feb155f29cbbaaa5"
                   "e3bf848dd89ca5551a4db6068e13464825555080b30f60b88bdb145c7d192455b98a7e7fc8dce339d50a88fa08b30e684e637fc3fcac159b690355781bc014c1303bb78f422bc2801809657bcca4af2c5b64a7582d1883d4b01ef45a671c46c73262a925d506b8a6e4aa92ab");
        bigint_t d("17325b84a08c2cdff7b2671ea0f948e4fe168b22a8b5239c5849d3612ef679a5d4ce68fac111c527b162e63f1a96ee324316d5e480523f97b48868cab65ed5df4d3c64eee2d4eacf3b5d7a42c0b9e699b7e2d41eb67a2300950f80a18aabba7463a2980becefc048a2d0db98810dc1ef986bbbbf5a1f61026ad2ea1093131665"
                   "817bc8ac744e84cd471ca0877bd085947e71697e946f4e2514cad526ed0a00a779e6709e0bd3e2bee3ffbc3dfe24bd9ea8acf3b78865fed242c5670690ee6262191f776fb632cadd3b114017a11986917ad296920898de02bab6f8baa77ee3dfefb917fbe96bddc690c76a53edf7f57784be57442cb54beb607cf68c3127d13"
                   "9");
        bigint_t c("a2e18f72760f49bf611eed99bb9343bd22266ece7abc13ec3395bd89cac7807893ebdd47b516e6cf760ba209b9aeb324c4bc36d447baebec9a2b962f4706a563c86712a892858b0852e4d14f76cc5e1deff94a3935946f288c62ccff873c2775cabff63c134480927c124013fad38ad12a4571a64d32e7c990a226a0924f4fd5"
                   "205951ac433b0d947cfeb8fc4ead8067fd0872c3967c9bf62f536dbb702965ac529feb9f27537a36db0f00b279408808f0aa1637185bd2d8f0dd69f72ab0b121e9f78cbec9720ea220ace2de8376e9dc8c9325abdbb5d6975ba9e2fdc6ce7a5d6ccbfbb1308112fdd33f24287535ed0a582c67c6e874ed5af81da0744f6a0a8"
                   "8");
        // Result = Pass
        bigint_t k("557f33353228982ff7290befb2a45d280bf6722fa4a59b6917fdedefd2ca6e106cf64f48f57605b1cefd9ea3859196eb73708bbc4d644f4b10029df332a07dc35f1a87e74a0a3f1e5284a0f5c2510f7f8c6027fe4f46a487257b776e776a3aff237dd1a3da7d559247d5466f2067be1d8cd5ccf6b57a24ef9c9b08f002926723"
                   "8d15e9f0fbf71583551cf6185750a61ae853ca0b61f375c4dce03336f8d864947af79ae6170452d9a8369033f6eb1d385791ba233f10e72521df69e9b5e56bd280fdb17506742eac8d6b6bad7aa0c259f09b4d79029870b4f4b934270e9f6005387e4197ea29858010b8d2bb42d4443abe4a0a2d3e1ddb704960f7d02ff370c"
                   "2");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 11
        bigint_t n("bd36ab9135d9297c1822cd7d37e8431e2cd4a892fc86b347861fa433abeababa2cadccde47642f20350ecab36f4d352913780d55f53b32e5fe21e402dbe7b462495e682aa5ee3e438f498b09492b16beef675c8478f552f478c560e2e42d97806a9ef2aa56acbca322cc629c2529c9b6880b467f2d843c21f0f623c8ccd4475b"
                   "9cfdf099f99f801a422e2fbeb336e5f569835833241850c66567355d5a74b389a0b7bd11b6cd3a8728e0f12e6323ccc030759fc972ee643294a0e424e224073d0e6a2299afdcebcd3a254726b30e276b2f89d508368aed69f6d1f83f16abeb7d26fb811516472b72162b4cb92a295cbde9293b6a39886b7f0676c55bbce0f55"
                   "7");
        bigint_t e("ccf5f343691ea8eb086ea76475d1dfb31bd729cbbfe64c8d8b2e61778fc3a55aa7324ede2d5b774eb57815f4f5bc0d0bf5afa51a915c3c2b94d605aa4b5f3385b3cae6b3899e20cdea9e3b4a4930af885a0da38c144c364a5cd986e41e0554ee54c3f10c6378bac1ea56d3e623d584af5a2dd238869b27867853e2b66abf2bcc"
                   "7641993c9a62046272cf41b5fdc2c46b194f5bdecfacab508fc6c50e5c28e13e36928e3a933499e23b11ee79b86b3f7d6cb19e0196a54de129bd32e09631beb95105984ee31d1a18d49570db93d4e191d5620dcdef8d6add5ce6438156dbe42d944460917d61927061c96aa9");
        bigint_t d("2c90fd4ec3ad3712753ebb5992c281a763ff43b6996eeb3db40106ae7097bdd084ee6f24a2b88f91fad58677bc81f582a8c717292bcc1be6bae0adc3d96041166516fda380c38fea99e8037130708f29659d3d2e59b7426a9cafae744df52f0c4ef8d43d56a0686e6710ffa27c45a635efa193cc5e2b0e5e39289e11e13b3998"
                   "3ea540d7303c0e31c85347fb85b5e34c8f56b7cff1068541be80ec7064e28de49bb7ef5e7ffd56d7e7eb94da36d9ad7a41627fa8a4a885108e6a071a2fa8a199df84c88e9851534cf33d1ea5f6340596a632ef30bffc5f79b0f95a1839559e1f4eb94047b7418f42fe3a7a4f7f9c0938b0ba29a36f2111aee1c4bf65c53c579"
                   "d");
        bigint_t c("4348e86c66d89592ab1fb8801e4fcd8f432a9f344d68158190594fef39cbf15a3d2ebf4d8ff66b809641aa9fdeba8d4d0ebd0f691abceb90b9e1b0e31af4160ad28093791a312af13781844a39934171e85f90adfc10fd268caa8ba7c9f003a3ec858b766d517aee76266b0a97a49101bb5a4a865b2df64bf1fe09c4b0875f2d"
                   "7286d0cef21e057e3b7a896660b323076ef4647f9fdc7c07d02352c13581d3b14ccb89080f6070a96ec306e6fc89a28ae976061d30e3376311628d26c2a50635629cdfad2ede6677f74a0a12d4eeb49314285aea760ad1669d03e27cbebca1c49c41fa44b6618df1be57bc734eaa0129d751856dda1bef195a4e7a49918e4b6"
                   "3");
        // Result = Pass
        bigint_t k("adb28fde4591ed50602e106db780d562a113aa142779a6eff8ca99bfe1cac1f3b6197d5e92a2b20830d1657bcca8e7d3d1ef9bba9d6cb436aa83da429571b65502fdb56d00ce663b42785f31ed092642596f3f202800b8215466bf7919081c143a4b4e4b59a3efb9e622ad63032fa46ca1f7300aa676635b56b090382a0591b2"
                   "bd70eeab588f33bd3aa68675203d333029a8be9db3f9d045da10ad195af249daa857f877534be907b0a4f78af17a418332ab750293f146600502739b9144573e572257234de024482b774e2e033c8460d64c7aea201ba9fcd0ea1925c834a086cdebbe8142c45e549fa85fc1660fb630c2ac4ee6b1e88b24d3203b3e70038b4"
                   "c");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 12
        bigint_t n("da1e6ca4f885c8f90aedca0ff72ecdd8782d2c78f4c7f4ba488fc23b701ec01ad0276e37037d84f8317052da21992ac321607289e93a2fc4c23508395af069e6af08d256e472e8e87ba19b6018cca36a916fd9175bcdc4089706f83a7240f757044bd125cadd0b3dc17152e30b63281fb9560d3f05de91dc444edb3fca14a282"
                   "33ddfb086307c551c62ef2ce71b9cb9993747322a1c6d9d6372ffe4ff44b0b92be88173cc3291c2f9096bd69d141ecc59fa08085d8773f014760ab1dd8175190841d5baca2c72dcde8fa2c8a3a018ec905c53006c79fb0e4666057b125a0590aa919676adf832367e1d9546d2705f739d84f0413b4b7e32112e99ce0b2925ce"
                   "5");
        bigint_t e("ce7a575525e695a7237e2d7be2859c8a2c48c409fff6e6cab151703cbe7e303842191512ce9cdd7be1bab9c380a6f57c957a28d334dee98899c62e39b1981c21be40660095a33ec7d24b13455a0aff100e73984eb06e7343cdeac1e5c49b63ac9b2b9ea91c5b55e4a97c68be9f7a84f061c16fb3b66a411ca14de505c985d68c"
                   "eb020d227c1ae36abf1851217fef4a753b10a075a5c884d2957f043046f90a760827aa895880c4a0fff00ab20a81afcdd1d5618f1aff2c1e55de13bc730d659afbbe444d8d61813915460b221a94b7284791aef1ddb103f89ea5966a3916f204d9b607e26ce448ea441a80e9");
        bigint_t d("1283e9e838c81b0c31a79adfea03e8dd648fc5212ba84bfc06dc92670892d7f012681fac2d2bd8e26d6441aa69aa3216e41dc705ec96d759a136f764d8900a849ac429ee6ba416a812f6ef8cfb7c6fbcb1c92acd81312b3e291c936a334f32b787f1b31b60587fca2af856794415a65ec4348a85d99a09d39a7c3a9896079016"
                   "6bda8fd3c5b6bf43918058de3eef4463d039e9c78cf1115e6b7b2a8bec6239bda506837a27d6664c5079fb6140daea2f411f94f74b77acab487d932540e2ca66f1574b09dae73c84a576425e7d3b9471795439715df6e52d333291ed1edbe6707b6b3efe223719d34592f9d99ecafc9d9f8252859538a03ef480e0d80331eff"
                   "9");
        bigint_t c("56ee33692682a1ddcad436bb9a64c49d9abbfd77b6a1db724c8f3ff7eb67d8095b6346d9dd4b7f94b3ec7280e3c5eb473903b7938bb008da40271b7c13cb170b15f839e9f13a2e1974bdc04f4d05b54911dff4628298ea65106c2c9fa0415e388d9653c3ecb67513153168505f8cea4b2a38fe0f23c149b9c348bc8919337338"
                   "ca23dd8c56261e2b9c2fb1ada1c352f48df41bc0f693ed7e61a2116106081fb4d4881f6fb5f4ef08139ee58b9b10b4ed41fd949e84779d5df3617450f0282a54b4ae61929387b15381684a15d6dcdadd4e39b1d054d323fd806f2d7e5efb5cbf717ceb1d77472bb3ef742870d9908a6dbd92b97eee10450810b38412d8e97d9"
                   "e");
        // Result = Pass
        bigint_t k("77f8bc1c6b059b5ee2d1c1cca8fc87fc7ea425194fd0f6dbbf067dc6c2970bf62f76639b3b1b1e1d5897761b59b31096f48e70bb68734d9df7d5e1fc2a3ce5325de5edbc81a147fb69fabcdc91824bfc9c2251f720479c93f0eabf03cfb8eb41db20069265430eafe6362fc03f3c3d866267327c5683e41d805a2fdcfcee6dc7"
                   "27233a5b6c49acbdc186455a1d36b67d881d9efbf55b67ed4ce581fd72d3d5df42c8796bd449a4e766dcda43c78dd5fb600e5ba45936d1d384a4a62f04e5d92fe79b21d03aaa28d6d04d6f707a2bdf29e1e14243957337b235949bbe5db109d792e294611aae160164184b06bcd88f0d9c210068861b68dd3517d0535b0a520"
                   "2");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 13
        bigint_t n("90cd69af72c6e77b5c6137f2cef9a39773b26cdb04e77b4a1a7621c23f60255c795ddfa893e1b810eb8cc591fffe9d6dbbe06599bcd4c2297bc73293ea074bf35781fe24684dba05805edf2b4ea3e293d6706436d8f867e3883f6d2b5381ac8c6d206e8f49bbcc052e06c5106af30e17400d81c8de72860f9b878f96ea12365e"
                   "063b8f967d667447328681cc4a9a0323e8597300f24cb8cec6c6291fabf39c427e2698280d457813d28fea30323198ca78c9568669d78744ef4c87d105e75c64638b7b5de28b3e0d5083d4f92d84adcb31e1c4dd839fe2911f1bcb7e1da102428a78c3d8b5a3a2be1a9ea033955210fb0d0a0784a102255686e43c5c259241b"
                   "9");
        bigint_t e("312b6b696713dd3088c8d7bf17418f4746d1ace4fdfc09945445842b65514a078301da08b116d7070de1437a0a08465dbab5d793764e46a95c0e86bf5eeb95f5cc9490b4cb74576f56f0ce45208640a3ae3f91773dee5cbc129d24f473b8b6d5be3d76e1e4bf5644d5ba710c239e7c1ae31f0ba646c8980eefa0c14c6101085b"
                   "6a88d8d085eb83b05bb307fe5c0a193d263090b8e2acd86835d8d02291d22d1783d2703649317bd19b3eae27b24b26a4f41cd2bbe90d2eb53a740741c110f73121061f8f22c56a1fa3be5fc2351c749eac3b832d5003b707b52f3e0c074a24e055f0d592ded199835ee569cd");
        bigint_t d("23897fc0f1120aae3de40b7ee43fe58549578908e30b7c01a40743cfb5647a249b0b22171208c758b08a53229a3951b73a81f31de27337b0bd1a8237b714cdd30ceefd8fb89cc215c3799b1b8f5a16f7e8f8f6f4f885852f0e9995ef3447a512d3fbf5387546f0de7426774618d0251a099f4feb23545ff3cc41bef2ba999434"
                   "2d79de6fe85057c09bef6d897fe29894472f5ac1f4aa43db8f78a2bead9bf4108cc2abd185d21e87e53705dafaff186abb2dacd0d7ea9dc7e16096fa0a5aab8568a7e1035934adfb32e07201c0a112f3129c3d8a154d59e80b2bd315a63c63fad8e9c472fd01af124d0b70cee313bc7468f70f36e0c030122828ccbfcca2584"
                   "1");
        bigint_t c("53ea8be7c838e167048ffa06a86090675dffba160b04be104cf22686201616dee2b2dcfb7a5bc9bed5a03fcc92ec0c92feaef68743b697b1dce26c6b4cfb286510da4eecd2cffe6cdf430f33db9b5f77b460679bd49d13ae9b89d0350c66e62ac6a90844365b29790cbf92c2eb35321e655e35d92681d80e6f2e7b6bc721f734"
                   "ed68338a10c9f2ee425eef012b822c8dfaf187c9a12a8ed942189495e4f2058b6b60f647c7df00d26b664f78014af6ce5c5f85f4ae60de18fb90443bcda2b5382ad67a02c366fc2d6e20d9b0d65437e971f059e1b3ea70d282deb3e6e9f9b1421015e6436c258d0733d4b630136998e423ee28181a91eb0fbf4a081da0bea1b"
                   "1");
        // Result = Pass
        bigint_t k("0ce3ff3ff8768e9a7c07ae8c61d0d0c8a1e5d5cf1bc4de0c51b1e2d40991d03266cb69d29146e354119730845fba05e8787952e4df9f337e48f124f49333a10e224ff7dcf17ec30c8a7f4f06671227e652a99aac9604194643d4822a8df52ef31609c013f221c56c1921300e23d283eb50f53f45af96b46d4df99fb12c0c1a3a"
                   "9aecdb58740aad5f7fc901a1e8730984a58fbdaced538dd924c6eee94f86dddbbc54e45cd72211a6158ec982c10f7f49d0f0af7397fc53c54b0b98e41fed32e9635d8d07d823371c16c8ba1c0660e53d9948b84ad5c20b7679e916e4cc0c68e059a5171bc06ab278044f929019093e73531812e6dd3cdf3b96467144eea44c8"
                   "2");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 14

        // n =
        // d61c70c1d95d144b07dfc8cb9bdafdc0bf47fe948fdc29365d3f129b53dc0b94129eb0f19030faac918e38b61521a944d84d9bda2111ba523cf7b5848e216bec4a149119c72aa24c5f0ae7c6597aea87dd1ff8c40ff168570602b4b8ad01e80d4f97820ac3ed979d898adebc348eb022ecc66f8b952f9e124e886b2035a91e54688d9a177e4183dcc3a9bf1349899cc8445a908f9fb4d6b698dc45f734bb43c3db07ff9e8d9342fc629ae0451bc41738eeb30fce79fc903b1006f58e2e22a8585f9c080f560deed22fe4894d388dffe07ef572b0345fb3fc07a241f73882e8f74eaa3e4557fcab444bb8808f016cc1b8273666d8c48490362027eab709806393
        // e =
        // 4f1a10816ac1774f7032d1fead8293b85a4bffcfdc7238c1ad3cafc862c0f3b637ad578e10f4f46097ddb45022ab30786dd565d650290f9d85d0c38ee9ca5705b07d54dc4f9ac1534d437324b6bb5e3b9506b364ef6471a773b022bd9a0b5d0b02f1af786463b065a6c3b762da11ce3357fed0e0582bea5007e8872d225cb9475b7ac79c38edecd5dd38da0cf5f00a957d87468bd77bc54ea8786166297fe67302a182e934acbabe49121d1c0c0b811a1dd68b3ca9ddc401158b3bb5a639b92441b91c6e976424abbc94834a3f1c6659cf21c7f9681ed35e6e8294cd0cd171e45c3c531354183be66506c155
        // d =
        // 08be6d66ca7bf007f4137309cff63cb84caf1c125b3912a1cc4b749444bf4e491454aa28cd578c7b4e2d54bcb9f1ebc65c67a67954ad75e823f1bd76d48db5c422a10ddef0d8eb0d56ca7f0c9f89904dd5abf9004b781575e5eddb97a9972d9547941c4e0623a1ad15e0ab92db71f78ae567c02619449667d2c60156bc9e00d2360747c3a3a660b0c78c9f85626e3b50cd7cbae7e6b875913876ea1fb55406de167b747ff8250e8ed4e0e2443d8da05c9a89f858ed10deffdf2b98515ba79c48077697253435ed18a9cebc5b36dd7c72fb6f8510a59b412e44bb8477c3e0439346b829887be238f61d278aa7b441f44c832ef39fd0dd1dbb1b37ba502a76c009
        // c =
        // de6a71c63591b31f166ce57e57c9aec1f4030dba0426ee77eb54fcc7d068b7f952d8c6e776e08e34e4dd03d154f7bec64302512feaa6d78470becf1e5fff2af755daa2b2fca34edb3bf2736fae4a9e9f54f8efd1883eb8e833b147c53fe0ff0d4f36ae2dad221a9c8374dd604f0b73d491ad92a8c636b9299af50cc35a535fc4a8b5916272a41b00252ce8ad1d8168ee61e9e0bd8dd504d10f51c720969f72a3c81ff3593473788e87df3d5d02f6a2742b738117c365be68f98f20e5dd0e5f32173c7e19d9d55acf12588478e5b04cee59141ebe522c1274c075be1916e64ff9fed4d8aa4a13e9492e5e6407adf88ce452945fbb2073267fcbcc4ac429706981
        // Result = Fail
    }

    {
        // COUNT = 15
        // n =
        // 91bf251bef75b328aead5a7152d01bceeea88e40de0336bfb75dc0e850889a6e22a63e457ccc255719d6087cfa2f16c88293c774e3bff44316a3fada996fe5a81a80134ecd1c3c8a9f914b7bc9c2e373fab41b35dfe1e74bbbfedf97b91f39aa8c5489dabdc8bb6e3a6fec63e0395160e0437a69f421032c1137dd807532fc0fc4a84a020dd00ea9527bbfec3c2821c277c8fc26095749814cdc0a661e5283648d1ef06e9b75a6bfbed01332c851a81d9cbc8ee9b7e47f5ec794b1831e302e9e52f5fbf607eb979349d9519b4936e27650509ea0bc0ef74e7fd7aec176f8341c9efcad43d55ebb7111e3548e0ddbc21c1afb3f36ad2d32101a468a8fa9a628f7
        // e =
        // c326aeaa1013cf86d4526d1c5b7b78acd6cb0532deaf2679d518cbca03db7ade6d925cace25ab5fe18bcde6e5b0333e1a6dc0ad579565f54fd92b25c8438f5fff8c0db00bdf53595d0bf5be7f8afedba0e32395dc002f9bfd98d76093081a4f0fa09a22b402060ed0de71dc051fa1204165085f81a9c53f07b83134d208d0e49dc278b0558bd0b10882f2c9bdec304684f461f50edbbb96f72b2f5c5eca85db571cd1b7c493a3be536c23a2f3db981fe77346ad82e18af85d079788536be47192081d7184aed05e2580ca16582d23e9d00a70c420000ecb8a2e25e1b057a53d20fc6153c653d77c7035bebb7
        // d =
        // 32eaeb4ffc89beb539a34c4bc5ff910f29c277beb4e5613897b941b63db990bf3226f9bf6e91a1e2be63908fe1eebdb73f9d31aef783bf51a67c3336d93448b3203da6bd2fa5676e5bf3425f9cce2e90b02b55942c46acf95307fb1669b5235faa2af59cc1df5e9cca704d623bb3fbdd1340c3d09c15bbf4e2d5d6efd876bcd9ba922c0651b3546c79e71d4d9f1f1a3593be6e58ce24aaed481bc7d664678db92d757d168b63c699db22f8e6216b41c472e9d6c386583a27421ce42c5ad42f6e1f4ace9613acbbe927a95e1598fd1188946fc4f8b27b1cb023c2b61a20fa2cd7819cfc6164c86584a5735e67af7321f412e7fec8565f6d6cfb85ca02e43f028f
        // c =
        // ee4bd9ab7450472ec11ff32049b103e17710666fe6bdb0e3d44f253fd45e1366c29f666cfc507610cfd7238730a6247cad619c54a343eeb6b0bed34b6b61aac253ff0f18060eae16b44d591bd2aa591b136202e8946a8bb0560e1cc7f2d827a788b3fc36f950984756c9be6298844ac8915bc1b222f92cdaaf87273cd089351e4902564e3ddfdcab9e927ae6e8fe9e8a0f93003ab5ba7b3aa1731d326d3c7a2e7e3645e83a83214a74f2b42f2bdefd6a7553e809b67320db1518252f31bb9edf0f95f08d58030f2dc12f95c3caed116eabe3a1a68bedc3c2f4c068bd84cd4a39e56880c4ff294f5b6b92442c2bf37fa0998b52999ef00bf924b10d63ce392645
    }

    {

        // n =
        // 84da326b6c8aa5d02621803201750014b539bedb67002f56006c0c448cd5cc7881ad5c0ed95d3dc285fea85d30db72b89182352903a88caa2b3393d6a4cd9b9d8a543dfa60d14880dd4bb806ffe632d8b3fbccd78a4d5c5f10f12b0645f2dffa924c8cd8ac85bc92555721360513726d05ad984e1ee75c5473d1d3d30c247f3871785adc4b298e4731756f208470f0efde356fbd18a1240a49e670b91aad23f07b1f343f3e5d3d6f5ddd85870ca29aeb9060810de962df81392835ce7924e537e4e456e8118c8418203c5f2b2629fc2080bf233ad1fa376b4ef367d6ea6516409014ebf515ecbe9c7ae9fed7b026ded71108a307b20596010477754223165245
        // e =
        // 963e85936ae0a94a8fe3e9be076490cb5acbd51c1665b605cad96dfac7b95171609f30c9588e4dc9041928da8f46f0e1074d106fc94521d8c52069130f81cabffd9e240ba50538b684d00d277223f96880ac966479cbbcd41164a8bbd0ca43702f384bfb17cff76b2ba6ad2740e752ab3c8968104197ea873a0a2f3e13b85a655007628cf78ec8e9ebe55c6212e77a9a818a223116dc4580e22b622d6cdf1548d9c0361de30c11312a620eccc4c7480a28dbffc9a824d85966c4b7e894887c7e5a4490e1025c9009f6e420d340620fd2f7d8fb912bcd42659e81deac115960bb79331aaa4fe2acc9edfb71f3
        // d =
        // 1327930132316cc458e2e6a04a3da993d6e3e2c700045957752cfbc4145a8c5dffd1a635614ab362cb97c3559d762575a41bec36c3b4f6bff8e0dd3e85ddb2f883fcef768d18b5dd6cd97498e371a0829c8c80ed2cb4d1e7d113f1e3327e08aac8bf45411bd7c840d7d6174d079fe83863f4ae0c6eca48bb24f55a0e49ae8cb34b9aaa4f58549059049e748b30a553aa56d273623a3eb0d9fd3a98dbc8924dc507cfff9d93d87b9530db0b00a255fd9cdfdc540892ada7d97fff712bea05c203e25ceb66a170d33146310d92bb1f01b1833acf7fa73b4540578c377d3b5947bb40d1b0d3101e215c24451aa85db0f2f51a6e2e8c273c8f1801093f19ea141fcb
        // c =
        // c02eb4df9968f68f64b157f0fa12155625b6fb72a9e385de852f45f676f085eabdf42e112272e980394183dce448966c19366672e57309bcb156cb76c598d0420a08fb8b14810f155f1914d9d99cfd11a139878f0f7a6452f47a995b235225bd30d4e3f4777cb17760a3d291e2f6916c4627276f2ee2dfc9d503784e9806cb83b6beaef4544a17cec44685a9063e9ba68f680eba4a92c249dd5930f72d9c0bbb24302335b2d589411169f4cc4e5c63e6056404196c7255f992e489fde9fc2d8d03910b5c99044ff8d176e8a0c03831552e83d1339a6ae0027e0c4b09d50e7922d9492e1411bfb01c8bd429cd67bb4d1b952e3cb2de62aa8c293a3dc2f7bb36a2

    }

    {

        bigint_t n("b4784cd9b3ba0d7321414dcb1ada0e22bb9f64d98bba019b5cc28f4ae82af35636edb89b43d5ecd40a6ec87d6eb8b245477955e3ff44b838620033c2224328dc61a923045331f8708057920587abd8194ab7c248be45f1b4d216da870fd17da7a024e9bcab6fb2ee3680f2160df09ff3c619a098a88b8bba57ee57d231626e89"
                   "9121aaec1b11e606cbd59b7d6b982da291c91e401dc6377b49488370dcffbdea2841230d7cb878ef910763bb5369878e92c78ba155c4c48eeed803526217c3b6b8d4a3014d49da8dd2d6b02b7b353f9dbd8f1376d31e15568a3ffe8332a2443acab2b1011706fd1b43ca86048f0c1ea3a405730a903ff6479899da079c1c469"
                   "9");
        bigint_t e("215c0da39fd2453152991719f331d018a51a3936a6af3cc4355d18360f684ab518e13e14105d2de7e5ab10561508ab4fc74d1dafb855636251ecac7d51a36b6a9f02e5c1641da0742a6ef7acaf54cef7167429de92a0cc44117d9b944aa597d6d2ed91e31431cda36ffb5532405b3f7d8b6f7624f6d4b8f9cb51a0ce13831fa5"
                   "07b34b129b41124501120c4de0452a78642ade434f622e2b35926ffb2cf99c7754a95e1741e7f24524e5be8e91553f4826cd4c82b6c3ad8f3b5048a19c401aa32cd355522eb6c53be3c8b83809dd4e83de1d14a94f3f3512a077356a040b99f3260074bbc866bdddd8762915");
        bigint_t d("09a5b7951aa459ba65f9696e67fcafc6aa61ff01fdf9a6bcc963d6b51bddf4c6404c347cd753613ebdccd1fc8063bc83535d907317311aea8620b7644f9f67bf3e886c98bf92b66c6a15ae5f466f61d3c1b55e0cbf202d3f26f3c814b4ba1d205204b83b4a320e1ee4e5dc360b7ce986b348f5ea0dcc7055e9086643cf00bcc1"
                   "efc87d4842ff48da0b846649247f9a4ed71f751ec6b9d2929a7d2e8c3898d74ce266757937dbbb8fb2e72ea97faf67c82ec96facefceca0e795a7aff5f95b5a19c4340dfe87fdb9a7ce4048626fc163a4d2a829ed13bbcaf331e7dff96361bc5d005911a277f3293c2a2aeed94797180b5f071362a1ae2d06b59e4571f4ace7"
                   "d");
        bigint_t c("7a82228a2da15b5d74c540f70e03a6a79bc756df53bb643af8ebbcbd0926fe5e39a78cc220f5e14b2f072452e9acfcb1502aa7859e25463236c37ba7da0e36e31e9accb71d674ace6a0808ec4a058173207f612b2812e04268004f92e48bf02312a20bf18dc03eef0d12154b97d8dbf4f10daf5a594ada7576e85f088f65d427"
                   "fb0871f7d9c867b5016b72f93aebb23cad1db7989994dbd049c4d490436389f6c315255067476a282c5b497e9906af543af7a17050ba8b5dc7dbd5b3684eb898500e5087a105f250772cbf601ef3794382e39667e6829323d9778bf766ee08168c3e1921eec9a536ca24fe292f79b9116b29be07c3b59b2a68de1ee7a60b227"
                   "7");
        // Result = Pass
        bigint_t k("277e3a514dd4b262be1955d546c0fd464488082fc0e1fe19c101e0b7203aa920807295eda6dded673d78fe17d417c25b49c765ed54f03203ea5a30463a9e8e60c469527d9faa1a6f0b30e9f687b59337ee5273d044a43d5813aca9545acd7af709f8faaf8ef6b89b403f44ca3217294301371c0f8356fe63071a28bea7cfc24b"
                   "34a02b5bab7877b43c5ea31e3cedd656656fc2160a094802066ba980599ad61d50ce4c40d52c7c376769559c8033619403e56e7a63651cb54866d3b866f36ae236cf643030470cb0a39ba57cf70d4d481bdedb4f1b603a2acc6e1626de31dc3ef96f56ddc61672d53229a12482178d42b0da4e87e6b7a2ff995e84d93e221a8"
                   "b");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 19
        bigint_t n("cd929c5058128e7d913dfcc070018c1a6ecaaf0ee9b92e6cfd27c0fc2b1c959eaacb93654ba56cd710edb1838f335697dd8f956778fe47a7d0ed7a756582b9372773d398dbc3318691ecca8d97494cc42eee4b7ea7b263838a3ee9af6cb037321582d45aec54e9172738d59611cef8302912e9bfc43352f652b7e247d30dc955"
                   "dd17df99d1bb2a1ae7c101e02d27226aa16a73ae65e0cd09881e605fca8c10d61c864ad0eeb59523af085fd2a823be836d9d3a4172f367341fa85fa6f8b59d1ff442b1a57aa4013f7603a6d769d0f097d20fa306fb55f77a18baf92443200d421d9de7d7c5fe4512a1f5f301580dd7bcec70dc9ecd415e8443028ca24531cb5"
                   "3");
        bigint_t e("612a4cce48bdbb2d4d21e73ca5a19218cd143969981af918c585f0066af5966c5e997f2b0ec71bf2525ced593ff0ac61966f90b3f7f5f951dcb39a566097c4152d23e70573b969a790a14cf75dc8c934a04e870f0bde69e8a7dd2c9b2607fde42795e040bc6b75919481f6dd6a6805ee77f9391555365de2ebc5331958998244"
                   "c2b6c3a241e924d04fadd32d2700a2b5335bda394c8e25f8ef9915a450f27ddc214b3d5f6dc544355ea0bd2fdf3193cd514c8980b0539b9a0a4d1819d36cb1003ce1af3a19f9641c28f1f3b01d061b7d4f604abd397847cdca5327c102d01347e0a49ac0195a8d077b28af43");
        bigint_t d("4f5e18dcd237f640a6bd83652041931ff174dce4197a632cc98451cdfebb12d1eae82497e6fd4768792391931015914816e3f0c19df27a5f95a4f56deb6391e9230b4d61c6d03b123fc427e4860c87676cc937224cd25ebb4a23a7b9c095c72a473b9a30768e3648f75c92c181a1b15f85e3565d4800e4279c9219f67b32e78c"
                   "5e4478d02dc812bc259bd35ee2cec06734ba860bb65af679b5bd6708a048ea0a177eb23832e88d804e754adc160db967806090810dd38bbd38880f6c119356b74a5d686dc19c378752d775853692b86e643ebc33799145b5adcadc2e7e98538a485d42c418b8a8f597e7469ddb4a14e2e66342517fb7282039efdcbca755b93"
                   "b");
        bigint_t c("8713a82c0aa2f1aabe3761188207e103bab1299be97feb1383e84d4bfc5a9dad25374055b81eec74316b18f6e001b0623d470c027b7023456000fc61538b663cb7c0e98d77a7fc3ee2277816bacb4d9487c6741e3134255bcea249ddb9d13203e4b78a1d6fc789d279726bbf36ee2261413648353452571d77d81c0655b369c8"
                   "55230b4dc705fbf69d55d93a7cf19591139f7090ac52235500a95d2550b97354b10c5365a8e9bb73f6d6a8d0b55c10eba40ad377c0bdf0c9141ef482d16bb96fa17b4cad39fc2f112d00c1c609b8fa389d7e5b3198192542593dca650152a1e0cd76dd6da3c5c097ead80c679f8bbb40a504ef085df12c887710c51608f5947"
                   "1");
        // Result = Pass
        bigint_t k("a4b9878cf5926f2a3004f2f5c3a896cbeaebe153c3b1badc84ca1650bdce2d24e9ce057853112031bf7ebee84928e8c2447da4579157b13d43f419661c58a820f0feccbd175dad3fc3268238cbe3d8bacfda4e4821efe585bd87cf923f155543f452e6f49ddf1ae261bf53122139f622c42cc7d67cb0b7f15e4297cf79441da4"
                   "b64d8588939112145429bb0813ff992c0afd449b231fd58d8018adaed3858073a5e97354a73929c0b5e9a43e596fa010fdcf133210ae6124d999f9069bb6df30169b5780035ae209086226283f4ee27da19066ded8c4d096dc2f9801ce253a93ee39df7f67614a2eb5da24cf3a18aeffa1701b335294dd9d51d8562f8b6414b"
                   "1");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 20
        bigint_t n("95be1e384fdb82684c50748da0664d76f8d2d966b358fdde18c95bafccf4134804b5a6becf8e1230977ebe91e44a28cc86690668ec0091b7772c3b6c6f3b2db0c0dfb97eb4c785a0e35be3fadc7d7a0c422e59710f05792cb4a38b0cfce2be2ae4f14f3fcd4a3065b59922a3558137346c67517e07e283e8d9f1d64d9d98318a"
                   "612f67f563cec2d627faf09fe130fed4e3522379ef242197b2e455091efa797128a170fd891ff7ab1940bfb1c8a64b1584653256343fcb9d96ee398d19be3913fe4deee15a464c5e2cb3cd14d5e787a86775ca4561c44023a4b745c4d0338204d9f2e9530f24e3ee00f98d6228b3db5eff753aeb0dc67707ebec5d932385a0a"
                   "9");
        bigint_t e("9a6666d28d42123f56eea93a663241d69fc37ac5717eeb6e6b17b1dbe7abe135a43a13e5ffda00871ac27fbafb5a3f9c32315f89420146cc83c0ba6787aea261756d25f23ee7e4f8646795519b4e978d15970db185d0d9daf01b7e863353a288cc65e75c39561171a7afc596bd4af71c255e18d65e3de28c0e0c6338765a3e93"
                   "87da5a2ffc442fc6758d783bec5e20aeb268e21f11e7a057954f45c1734492066d5d18ff3a14d5147092a94ea8fdf30d83ecd512012c98bbf1dabe2b05ae9b271c862bd0a68bdcf7bee91595758e77980ec0ef83d152f4ac56db6a90df51b450c06fa53fdbea925088045691");
        bigint_t d("03e0ada5fda6ff761468aee51ce46b762a5fc35f1c94a293641ce9241341078d3f9fb3767ae51ef6132c992e62892eddd4ecfc0791626687087b97a9b3489122d1f07ea65b3c10eee84240b75feed50a51b38078c323d00587ebf6bea42533d0f51a10f90961bc985d8825a62947dba3de520a4503b0f55453d1e0361e9a58d0"
                   "3f8069c20f20abf367fb883b5e504e412dfa0434683388cdac5a6a01d36946d6cff04a20353a8a0ae668e48622a7310726fc7b27d9438d3377317ea8c43e1afd1da2a8f55ca9fea7019622f08c3657a5bc257db34ab20bdfe746abf08838b765a854b226f9fc9e74e902efc34f7162e7292a2ef0e22385c7bd8a01258afeba8"
                   "9");
        bigint_t c("787b8dbaf52c3f91eb2120f0075ef026aecacf669c3d0075cbdaeca6969f6a081e3a3ddeed1ede4fd9d77d5f2b9a421e0121d81ff1b62c2cef8d723871566a95d113e168f2277a5926b0ae8f4f1712db45e6d83378d99963d73752fff062ac9c58ad803f4d6f9a169a56bd66bbfde8c542c0d88979cfce80498192e25aebb883"
                   "7cb2224d461dc6eefc6eb1ceb6734c44bf58df093cbf53ed0e123cc8f91817b405c7cf79245a874916a393ae8aa1c7c08655f59b08d2b110ee94579b13125095bb9690ab314d638fae9a6aed7db0e0a15d602227272f6f53b0acec42f101eb9065c76ccbb31fe4288dfe3e75d8c8204f0d216f95df00151d1e0402a5db0c700"
                   "c");
        // Result = Pass
        bigint_t k("6ae2bc20b74613348f6c1d26ef2b4c74d4d07b87e2e0a4acc71233715373b7b89402c8aed2a468b49ec5077e1b9251add0c8831960d23f37d289eb88e0fcf0fc7987c11b62662dc5e9f0a017c70af20c4d23ca2fdd1923f954cf6eb58126c00bfd33ddd759124d375ffc6bf2d7a45674847006020ed97b485a612ed9ce3960b4"
                   "fd8a8344d81b539cca6e057c74c7daae333a91679667d75987d69817a8a23704096ac889d9ba09d69f559e8e2a0be4c9b7e4e9ffdf880712549a078e6d1d1a8e554a00e1c172ab475be4d098ef06c90ecdb0815d1c740c3649334118cc07fe2cbeb7919dd81c5ce5cfa9214d6599f466eec4657868b384a2bcf1d1106ecdec5"
                   "2");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 23
        bigint_t n("ab7274933e9f2ae41740fe62bce2a1d54f6a86cd74cd21dd50236761911b06bb47073dfd210eb51dcab5ed1472389a598a65c8a915937aece2af26db13ee74401b1e2cf9627b9737caae78319b6b0b31b51771e9e21898c95a16fdc57e992136bfb1b80bb35e0c439db6aa114b0b122eb925c8fb78736f7ec0a0478c16d544e8"
                   "f17f571150d26f173575a8bb0216c841cad877dd51e03d7734729dee4dc7bb696292785459d8a15bc796d714b095f97ab84535929f24a4e2375545230aaa8331ca8758b19d6a0b36e6efbedff5305385493e6087e4271d45dfc011ba9cf054482fc80e95897d787dc173f3b66778f93e9da7efddee6ceb2866f44661639bbea"
                   "5");
        bigint_t e("85cab747fd4c7f7a7e81ca95f58d50699b9fceb8e74b97d10e54bb10a5b791166482cffbc41fda7551cbb799b3f3a252eb7af8778fa51de6b16b2284ec6505030f0bb7c245e105c9ec3e9ce5102f91360ca67104ecdbedf6cb56a8af53c89b1e314f1298fc408f5bb16effadb6be8dd20d2f15534f8d00a82d4ad8d9b79c26e0"
                   "dd463da28ae9762015aefb8f1373f7795160d17cb3354655694a760f35e0c5b0f81fadc57127fb64148f5156959139b19068747d309c19e3c810c87ed94addff69425e515adb57b5b8496a7f5a401172ef343bb189c1d87ab9de710cc3240e9c34f85e7d82d81956008f2e3");
        bigint_t d("064c0bccf74da00201d8b9edcc21de5388696da9d311bbfb25a8725f72cb15e1c63a17efacc20eebe8ad06735a8c7c110e079a34acc3e74af862d5760cef74b0c9126debfb9b7379164f2356da6ef0d27d706d4a7d7761639ef4964d4fc1c29b57fbf0aec25894e5436a7049df5d4152f9ab456524a51bafc31018cd6c57ae43"
                   "ea23d78fccdea771a2e8e1a37dae74c6e8b8b8666e6e4b48ad5ac3e81c4b3c58f27f95cea42676c3e7d8263c4cf172fd1659884d45f5490348978a6075417a88714f8445c83caa5c624dde9281fc43035c38b575dc84d8d6e705d31bbf812bd875e364ff2072d69b35267b1ec74a410c0e3c0cdd028cece2f0d3da662100a59"
                   "3");
        bigint_t c("78313a6d1ea2fe21488f33d08e04448dc94101356cbe969a186cb9e16a3e282918e8536e3ce755acefc129471a8e1baa89b77c2e41fe9a3e14bef0ba72749998827f0bd17124017b0bd5b93288b360bda851cbef02bb67ca98b773ebb04a6c22df7ec9606763e9d6c3544a5b7f7bfb0e4d98543d56fde486ceddab3ccbc7890d"
                   "576ef7e1c3abf5043fc54388a8efd4e5a11280e3e5cdf9badcbb00776cf4d9a111d6fc1d30050d4fa8b00e82ab16d48acc49b7a758328eadead6db623ad74228363e40dc547719fd269d12112ff9e144f5c5625357358ba821b8a2c7de7bade8ed2e2be5d70c0157e015b7fcdc9ee45d440ce7ad89ddd8f3a8e8bf6cffe5032"
                   "a");
        // Result = Pass
        bigint_t k("9cb07856dfc13ad20ea33a8ed357036e28b80ee0272f744d95885e205461f8704874e31e7783d4f05072c0f5b33449853884069d367a2b17eac3f4f327ef656bc416981b5b3cdfb07c66eb7928c8f8287bbceab539b5c842afdd977d7a5cc3aa551bb3f702a572c74bc4a90ab1008ddcfbdea06b3d9bfaf2e60a08cdf47e697d"
                   "aecb1f60fe48f1f301abc1aeb6a53c04e9da47457bc304d6a37b63fd1280408c3de6dcf12eb3c67641938a3ca57860f65c886e66a050f5b12ed6624332577ed887a46f7cca3ebe9abbac304481edc6c0b0bf2469eb053867d3a9fa1767b8c468ce16dce808baddcc392fa7fa340480dd78c67ad3c4ad95c9f085ecdec4a995e"
                   "8");

        test(n, e, d, c, k);
    }
    {
        // COUNT = 24
        bigint_t n("e7bf89dd14eb3132f9098ce62ea46bed94a97d3bc7de936913d250bf6a3740915249ff91a5655eeaa301c799c807a27653cf3d8a2a4d3d85b9ac8d74dcf58c1e2ae8fabf52ce40866be0564cdc45430587f53def99e24e5da73f8d8ed3ff674b597e2eba42f5f2d7c2fa33ee5271737677d807cc0823f6f1bfa3fa4a497e299e"
                   "8459aa64a62bf5122f9b8cdb40a0cc55a4fb06897db080bdbc56b1dbdaa531443a285189a047b8c60d63e11d5f3d502a9d0f2e655ee30607fa5fc8481ecacc709f2cb356c77d3d94b58ff0f3af8ef68b1c2a2b4f0ebfdfd229e609fcd672df3e18bd76e0c5209f503a52f5bea21b8a34a94901888dd7173e836710c1ff26f9c"
                   "3");
        bigint_t e("8056940c538fa279070927c795acee2faa60ab159896bbf08916e62800f88238aeacc82e14c1610fd7e1bcc7f157509fe97771297cee0a198d4c31482f8ed3735ac66274a591f9a88b1ffeda31e21d18492f5514e9765b0e54cab62577aa18f9be8c135ac2c4a8b78fdd7874951901b5bd0fe6ed90e574e5a5c03aedf61f0b14"
                   "4703bea39ed743cb90c54b7d1792c2d88de5082bfa47b2720befb20c310d5188436182bdc8447f0ecf9a8c1bb3874694a548931dd5b08516384351fed49dd260f14939428966efa8ca34c4ecf5b0aaabd1b186c2e3417900406623e3f2f8155da59b5d478c821a2d82990ba3");
        bigint_t d("4f54cce4d1fe19eb7a5157515169c8e49bac53a2c7b5aa43c1571e8ac22ef215321feb037b4a19c79d008064a380b69870a14990869b42a56fc7e0904c4052f49157eaa7b251082d29905f0e3054f165a3f79b2518e85be81774d045fba5aa722a2651066a980cfd2da0d2e0a916d09b78785632bf17aa013d9a1cfa5fc24303"
                   "5c0b6049e37a47958342d9c9c886d6bbbff3b188a2a5e812cce482f30cc545d88d75f1be34d872697f3d88c11a4f38ac7d2f551b53364de6c4e28cd390a0fa28fb2dbb386b04c6ae77a97d2ea21f5a59e30fda7a32f672321923a3b31af81264e09923e380f93ef22b6d9e6da310fd2faa465557d51b4d44050bfc01468d29a"
                   "b");
        bigint_t c("e6beb0d81a1af53f804e5e3357f52dc0081e4211a0f134368296a0245e52c1fd5df341929e6f4f27e6c8f27693329df6d278035c129c2c62b9bec9c8153ec59995251cb039c1f07a4790dafe40607e1a12b3620f8f6800401e981936b0bf4de8f61ce939d25a00c30b6fbdd344c5f7b42f970631fe9fd5551aee3975181b8171"
                   "e99f4f365080a687992d43fe89a9314d91ec98ca4687d62b1686d08d339a8abcd478f6ae9d7c0a505b921586f556536ca07abfe35168284978298787eb869051206d87a9a09655a5b865db737213869ac4092482068cf637a99f066a2725bab93647aa304115af0f18703ccde727f25e65606eb24d3a667e12b0f43ecfc1317"
                   "b");
        // Result = Pass
        bigint_t k("2b0c4278cd9d89fc0cbaef5d8250fc6f4b7ea829a584aeefeebb0a3fef93fc01121a1c17a9996ab11243ee4ab5228cb092cb5f31813abb27df948a7853ade49d550e0d703b892d30c362cd50bc5b800d46cf3a9d74d1816a20b27692f3e46902a01d77a031a8e3824d740254c85fe54d31061fc448f4f9230d16c83cf1166150"
                   "f386470228c19fead9a2b6506c9251971c06be189db123a0e436f8c8178f3b295217e85219cc9016964280c1e37756a740e84fe744d4bf541003892a8458d201b7ddb9932761e1706d9eae8ee7835368efd6ca512bd71e732487e137647f250f9682984984e596a85bf4e14fec3caf32f809c8b2b622ad064cdb59e0ac3864a"
                   "2");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 25
        bigint_t n("86bb1ddc17af216ab1c963a34fb53918f01001b042e94534dfe2b4e1d55b1f11797261f297d2c8d40a3bd565253863e142af9ca30a25688e0796e263dbc9008e7b84bb581df4b7e91c105e39eff549f8d0ae6de8c70a1697e1522cff3271ac7d33db74ba8178c1d168b95599bc3a89e3538daf71f0d8a557d43bf6ffc5a6c6c4"
                   "2fca154f04dfb1b69cd3aac3c4d2af89ac5d2c9db0457b6e0439af27af6fb73a928d1604dd0fbb9dee7c880a6840870070754a12dab46aa6d2a201c5556505cbe318c74153589fcb4a5af181fff097cdf1790d8fc72261601c5e68833b1fedb7345e96c0bdeb4fd96506c94530a2cba2f496f816fac60261ea32706aab5a04c"
                   "9");
        bigint_t e("83e41e7a7b626fb30238f3bc6d6e3c8968eb271e65bba444567e33159e1cf75e8112f7800783ce43bde24b729a31194e02e4035746abf9e7c57817a1978c4d95e07c22b31b353da0ba311c6cad70fc28352ba7ab1b2bc69f71b596643482a51756791d4fbfe0dd62537b23a3ebeb426768606f2e47be78c0f32e42825c176351"
                   "dcf27b2fef93cd6eb00fb0791d91fead4c04a0996bbdba0470b38b18a81a2e75bbc39acb62a8337406c02663c1fa029ad13f838194bc88b56990e956e9d5098b3bb61111e4bc9f399afa1198ce584cf5bd9df4986de1ad3b0df8a92908e3e2d412b714d0382ee16cc924bbeb");
        bigint_t d("0a6520fe46e94757427ef2c8c02245f0123a1b763f043b1ac557ded75417bfefb4258c1626ed72605d6783806ea4586ffd279550a83cb3a7f8bb298cf5c0309d9b999de0451d73a6d1fa6d6b8b8d86317fd69821bdb30379acc12604cbd1f2b9213bfa9c7638d09e533a3d793c8f75401bcd0323a9074c2178cecd8dee862e63"
                   "a6f036d8068e016077f76fca9a1ebfc91c1c297068c44c82bcdfafc48ebd23789b3bb8c8aa02be0368e1ad183a54bc73822e3446f849acf0a1b9c61f72f274645682cbd3895d87af29fd1a2aaac16d3e94c444f04923aedf52ec6b3dd0b6167eef8095c42915573ea49447199561be4f5dd30a3feecd30b537654c5c0dae79f"
                   "f");
        bigint_t c("2e39b323294ac082bbc111a938b23d648b711ac1d6a580c7599e506595e9b5a9b121aa331c07bde874d512eba7d0aac9581de5f384d54672655c77d696ee8d6f797a8cc0d6b42e31c466e004cd060e8a4d8ea0971b88c5336b68ef92f31f1f474ded62b49d779bc5731984def37df011336d5395e4e58373356dc083f4a5f397"
                   "f62233f6ba788d0c4998055f605dd8c5d16104d527150a9b8911f390e1a891c83dc0076e75e08c6e0e8b6b715ecd4e0ef113bd3e2c40e21130fc8fa0014f8243e870e70415d71e538999bffb33f5db54592a85ba3a8c913ff2d79c9d9dc34d6fbfd9d9a0e484cd23836b08e428fe06ffd24f65319a24a18b964a236efd2c7ab"
                   "3");
        // Result = Pass
        bigint_t k("309b1b04d49f140444d165438332a0525635cecadc70825c979b2354981588b59852573210772794f28df61bf6cdb7ac874e68949659fa21e0dcc617c9699a49e934476d0e8ee67ad9de7ef061390dba6fd9e5409a6c50baabb794dd51cf373418565e18be660b6389019596cfae2630d299f4e7ac79595213014fc1ec121c15"
                   "35e54284d716fc0f6e00bef8cd18b2dabaeab3298bb489e802145d324825e03e642cee5d7dd0ab5130e2bf4109d25f70b2c69be91738badb30c870336fc28df5d28d9618efffb2f5f49cbfc1c35fbbca00a47b12e6dc9230d943e9277bb1c96fb12fc86285e5f0ddb742a8b3c53009484f1f17e8d560de5462ef2c27181f8a9"
                   "e");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 26
        bigint_t n("9d27882f5756110a353125f496f0dd0dd96eec6abe26dd900d3a6e68e8c614f8d183901211e65f02520fed0348b2dcd1ede54021d4380165079451b4a45e65dad6c29007b4cd9df36a1e90c4103cd2ecc075eab2eaf58de5e627e4d6b15960498c674266816b7b50e86639c7e8f8676c5077225b49370a90abbc40ed3f3e68f9"
                   "5c74aa02f737f1e74da267945702c5c06b4856867d238d8119968e7c7f5fc9bfb4b83d78fa21c03be825bad7ad5b5c1c6bf5b3a5000b5cefcde0d4705f84967f3589e1623752e08d2c261cbdbe3129eb4f4eb5466c55756939fcbbaaf1799e86f9e314325d5c313ff84918b4473ab96dbc80a067241d1e5fa686731a7426dfc"
                   "b");
        bigint_t e("9bd77929f91ca5229d1177d540494140d9d60ec8a3256434d8d6c612bc131709d5c10f1071a49a93ff5772e09f8c9eee8a8dce2d19a9dfbe8fc9915b02547e54e242d7dea02f626871e90b017130c6d5c9c5e9a2156a10accea981936ab65ff99f03f496191d44f61b2a0e063a00552b5756ff8da50aae48707ef6a061e144b7"
                   "c7161e16f38e2011d562ebf23b602611168d79c7d1ff0b5e1765eacb5c3d90d71228e43716acff238f3e5a08c4bca6e436d33a4a96066b425c2756c113bfda287a1a5d2af9574f745d06990220d829b5cf72e0d7074539c30342c90515a60fb1e85b0c5d1cc970760c888d45");
        bigint_t d("2aa333f9c14ecb4a4aec82be8b9db293427c3e8ecf4fd60575ef34a7afa01fb08774eaad3239a43b3364a5cade59ae7f65a2e252b50a8ca4d38db14c7031fb2e0e9d1b54ae1c02b968626f738d8d295c07349ad8d3789e13e31d986928fb11534d5a5574f8bb36238a3a91e9a7a6e6479512bca4e6e502b8a7a6ccf2aa079e8b"
                   "6ebb365b5b2c6de3ddfa2010d91cdc0c0a8e08daddade06d68cf0dff48d6c2dd833cc899fa0a7745288a6453a951a13578c9c62e458ed442ad0bcc3fefcadc5892d5213352f7579cc182b36e67f7ece74cf73dffe3279d7c113a1412442ed41593f7964c5bcb6d75e23ca0143db7cc043fe41b9bc2d9c7d4b4e4c8b289be250"
                   "d");
        bigint_t c("8d049e80ba9ac7856995fac26f8b44a1e243213ac6c8410fe9bc8bf90cef6362433028b59f63011309d4da4871cba9fd2961f62e8b919adfe292c70b886448de4baa0a2415b7dc00cdeb73e17840b1b2a4202751067ad84362143fa6ab2394666c194cb466bedfb47a82cd76ca11e3281c29ba8a7fa882a230eafdb0be51ce64"
                   "6d25f44fd89f9298db40f1f0b067c7c75823bac03d46206315bb091309197f3f17770cdcb5837d12b7cee635726ae6f822b72cdd897a3af0d92f69b5d4a12e5f32b79b424391298bd5d3b42f5841c721408d1ac8ab6c59b044ef27806711632c868daa6bcbf21d6cfef864c4aa15f3ac7b4c8b69eb46040b1da34b5a0992a70"
                   "e");
        // Result = Pass
        bigint_t k("67024c0694f751a67597e269d038603ef808218014c1a12046ef82a6b4bd63d5328bf15ac40ec197dfc63b8a9561aca78841573634303113305b6ea92e60a87d5400b0fafce42ae4ec41c935937856c80a39c65032914ffa682ddf233bd058e8b70a9bc509a490e56bf0de2c3697a53c826dcda6494ccc15de579f3e430ba926"
                   "c345c466f38418f9e8bb61b43022cda1829378827b7181157c216e42f54ad32b0f5a952836131c089c22aa9eee4e6a9c88694be91735bb08a41563ea2254b5cb50453c4bffd22e7c86b5c0223d55e57d298964e5b4b18bf9a96ab0013f65c80e5aecd3186333a943c7abaedb856b51d22c2c4378f9921e0c7f3c3f937ab7de8"
                   "b");

        test(n, e, d, c, k);
    }

    {
        // COUNT = 27
        bigint_t n("b2db46421619b868b90e66b3378fbec47648f4e00b60ab6de51327bfe7960f72acb5598ff79046b4441a9e0145f2195a2372265827a28ae03bc5fa25707adbad06af93cbad705529dd8a4022c1b304e93711bc71ea56423ec5f4052b0483a396dec1e60cd372b01bc96bdfe4d72c676f821f5b8bf5f767df767b759d50382bd7"
                   "11050b09761ae1ca8ee70e70b3cabd545bbb7228ff8e1b2ca1358e21b1a48a63bd618d23093f0b48e2d7b8ecc69352e97fabec8d3422ef20ba48b3f3c940271a5043ae724453af99c4a71c5577cdf6e57fb0b3de1511bc77e64ebfbaa9f8f942a53a054896321a98d612172a340d71b5db74bbc7b37de5e88370313406f4fe9"
                   "3");
        bigint_t e("537f02211de320ca3636419da9d6c0aac945f351713f060f46bd40fd62e550c51e1ac8e7cd9e29b5ae8200eda88faa0a2ae624b1265078260a32b9f61ba704ceece1ba4ff0968db859b250aad0f88eb3099e2ffbb3950aeaa7c4d685443cdb6d485f521d6b73e5bf4275c0b437bbc2a109b78a978173556fd7946a198823f0d5"
                   "661338107e4f483662ab61d11c498ee311c94f8620422c44c654d19b761dd6ef5b34124af53bce2e05337c30a5245ba426d8cb8d3a6563e863e6473fda5c8e96c062ac361aa16a86997f9c041a7432a90d8d0bdb4b6a250ab23dd57c29cecb0eb313d3b0cd6a6dccf84a88a5");
        bigint_t d("0f23f1402aa3826a9927837fb475d1dd26b8f9d99641d0463cc5f5e80d7f9a315ce2e0f716a5f4ae71fc2e83f49a01c9eb61580f3a43303aa6d0386daa3404b29739d4b0a5d5a7075529d7d9f0af0fa74e46ed0b31e1eeb10f45b5f439b7edb8afa83fb5da1e74d76699026eaf3ae20eacd55a2c34dc7eafb5f2467edd1ca600"
                   "78104fdf187eef771c5704a967611bf22e50e0b0445b1a2ae5700eaa1948be4384c31488a10a7aac2bdf0a7ac2d84613d1f7dbe81f047052badcef2fd2b2313f587856df9f93bb3c80b034fb05ed7b21ab728edd143f5973b2231272f45506687af2580ee7597f0a26bdb5456aabc24b9aaac636c59c9cc7cded4e39466160f"
                   "5");
        bigint_t c("670493e17ad34a8c4356697ec21d8af7048dd09ced91cec9348da428af946b83d1dad4d6716c0ba9cc1ee29ccec52b448da7653517309c606de78aa0cfa0028f3af28f9e30eb5865b7034637a44f69fa19f798354dea586b8bd000a87e411a59ab340b603fbaf7680becdf76cd210cc1b165b1c41727cc9197324d6842df7a88"
                   "2b889245273fb05bd0c0f382d0a5dc435cd9f709fe4fbe7b98f4ba09a6e2ebc5c3d66a756fe44de60f69cb833542a125223bb32d89caf7d196fe8fb32fb235b87a06dc179f1094b0cfceafa0855f22120b732e5741013f3a9a38148d67b72008580030543f2a4f6218d7e601476628b0206aadb0ac5a5d5eaa8a90c0d559745"
                   "e");
        // Result = Pass
        bigint_t k("994a32d2b5f2eb05a6b4db325435e7e34364d9f7c96af27851a1bfc70950aaae1b450affd2e70d34d204edd262b11ec37cdd0a1c5b2ab249821c786e494ced89bee4499472f2574838cacbce728fd3a923f7e0ba10742900e998e0740ecb7c2b1c20de676865c1cc30196bde1f2eaf2fb5dfbfd44b33879bdaa657ad3edf72e2"
                   "87005bf07781e6f86ef2d3f3c67da2d5d8fb2749cf774286b95777cfbda17dd2fcce6588e12aea2d022904ba733f08485646f8936e12c0f4cd1e8f8e59c16251bd0f3ef3b6c3088b659c81f265d61493e3c9117d72e8ebb16fdd4a989723f8c062f9eeefc567aadc586d8cde78701aa50a0cc94dd8f33442593c1491da5f7b8"
                   "9");

        test(n, e, d, c, k);
    }
}

TEST(Test_Rsa, FIPS_186_4_RSA_PKCS1_v1_5_RSASP1_Signature_Primitive_Component)
{
    // FIPS 186-4 RSA PKCS1-v1_5 RSASP1 Signature Primitive Component Test Vectors
    {
        auto test = [](const bigint_t& n, const bigint_t& p, const bigint_t& q, const bigint_t& e, const bigint_t& d, const bigint_t& EM, const bigint_t& S) -> void {
            auto N = p * q;
            EXPECT_EQ(n, N);

            EXPECT_TRUE(EM <= n);

            const bigint_t s = cry::pow_mod(EM, d, n);
            EXPECT_EQ(s, S);
        };

        // COUNT = 0
        test(bigint_t("bad47a84c1782e4dbdd913f2a261fc8b65838412c6e45a2068ed6d7f16e9cdf4462b39119563cafb74b9cbf25cfd544bdae23bff0ebe7f6441042b7e109b9a8afaa056821ef8efaab219d21d6763484785622d918d395a2a31f2ece8385a8131e5ff143314a82e21afd713bae817cc0ee3514d4839007ccb55d68409c97a18a"
                      "b62fa6f9f89b3f94a2777c47d6136775a56a9a0127f682470bef831fbec4bcd7b5095a7823fd70745d37d1bf72b63c4b1b4a3d0581e74bf9ade93cc46148617553931a79d92e9e488ef47223ee6f6c061884b13c9065b591139de13c1ea2927491ed00fb793cd68f463f5f64baa53916b46c818ab99706557a1c2d50d232577"
                      "d1"),
             bigint_t(
                 "e7c9e4b3e5d7ac9e83be08328105356dfeefe222f26c95378effd2150fadf7ba23f5b4705d82e4f1bc45057067c7def73e2100f756ee6d547965fa4f24b85d68867f03d7c886d1dbcca4c589745701b362a1f1417f471d8475b6b7a16a4c48ef1f556edc3f0ff6ba13d365d6e82751f207d91101c8eea1013ccdd9e1de4c387f"),
             bigint_t(
                 "ce58602e051f0f4647c4ec57f682e5737fc482a8a1ffac9043bba4fba3387d7dd2154507af1e28bd81b61fcdfe35f9734e0d9b53682ec785f1f6e6224f63d10bf78484b83a4254f333d0fb3f3e9e1834bede52e3078ac279a862fb90af266d7591c81f20b718d07d51bfc221b66a25403b4ac1a68d673fdd959b01ecf3d0a7af"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("40d60f24b61d76783d3bb1dc00b55f96a2a686f59b3750fdb15c40251c370c65cada222673811bc6b305ed7c90ffcb3abdddc8336612ff13b42a75cb7c88fb936291b523d80acce5a0842c724ed85a1393faf3d470bda8083fa84dc5f31499844f0c7c1e93fb1f734a5a29fb31a35c8a0822455f1c850a49e8629714ec6a265"
                      "7efe75ec1ca6e62f9a3756c9b20b4855bdc9a3ab58c43d8af85b837a7fd15aa1149c119cfe960c05a9d4cea69c9fb6a897145674882bf57241d77c054dc4c94e8349d376296137eb421686159cb878d15d171eda8692834afc871988f203fc822c5dcee7f6c48df663ea3dc755e7dc06aebd41d05f1ca2891e2679783244d06"
                      "8f"),
             bigint_t("70992c9d95a4908d2a94b3ab9fa1cd643f120e326f9d7808af50cac42c4b0b4eeb7f0d4df303a568fbfb82b0f58300d25357645721bb71861caf81b27a56082c80a146499fb4eab5bde4493f5d00f1a437bbc360dfcd8056fe6be10e608adb30b6c2f7652428b8d32d362945982a46585d2102ef7995a8ba6e8ad8fd16bd7ae"
                      "8f53c3d7fcfba290b57ce7f8f09c828d6f2d3ce56f131bd9461e5667e5b73edac77f504dac4f202a9570eb4515b2bf516407db831518db8a2083ec701e8fd387c430bb1a72deca5b49d429cf9deb09cc4518dc5f57c089aa2d3420e567e732102c2c92b88a07c69d70917140ab3823c63f312d3f11fa87ba29da3c7224b4fb4"
                      "bc"),
             bigint_t("7e65b998a05f626b028c75dc3fbf98963dce66d0f4c3ae4237cff304d84d8836cb6bad9ac86f9d1b8a28dd70404788b869d2429f1ec0663e51b753f7451c6b4645d99126e457c1dac49551d86a8a974a3131e9b371d5c214cc9ff240c299bd0e62dbc7a9a2dad9fa5404adb00632d36332d5be6106e9e6ec81cac45cd339cc8"
                      "7abbe7f89430800e16e032a66210b25e926eda243d9f09955496ddbc77ef74f17fee41c4435e78b46965b713d72ce8a31af641538add387fedfd88bb22a42eb3bda40f72ecad941dbffdd47b3e77737da741553a45b630d070bcc5205804bf80ee2d51612875dbc4796960052f1687e0074007e6a33ab8b2085c033f9892b6f"
                      "74"));

        // COUNT = 2
        test(bigint_t("b968e2aaaee6ac4037af9268d0a90ae37b103c4eb302d0ecc23077752f395e6521647a15ab01117808ed7ddd1279090c42f83deb5029b03216e370a21fcdacc6119385d6e7b278862ff7bfc1461fc7d22ccf02a06c9f4eb63785950bad3b2db9279a973e2fc1e2be6c0d8eb124e0ea97e4642bec8ad0cd16f9ba7c3af22f2d1"
                      "bdbc7e4d7ae1781d63d22c6a285a52d97730ce3746b94dd4fa4fd3dbd0526fcb141b1fdb99603f41425be8298ff8514a83c639f442f03447908f23603196941d666d2e24cb42a0841c0d99bcbd9db2289c5e2d9863670120b1b2f0ffecf4146d21bfffb1a07e00bc2f18e5909f390e9820b977ca7e3a2c67fdf3147c70a4df4"
                      "45"),
             bigint_t(
                 "c71485fa4116382afe9142d7d5580d798a41e7b4f73f505618978002f137f5c209fd1f377e824e7e0c4b3e44dc1f8e250272efdb715a48d434d8a01e3965c7b1f65aec349d963815d78998221fc2c3be3508cc69fa22b4fc3c9a2365309c5eae72094b2c22ae176704806a1994b7e4fe5109558a0bfad6964bd7d6c14cbda74b"),
             bigint_t(
                 "ee6bc4246895352b598c19207475131327b7daedaab3e007c28cc96c1901eb64a70132b313b36ee189eb45395856eb7ff5ae2e7a53282653c93475577690d6e0e18416a9b287bff910a72b7d8074cf0efc9b99558fb671a4d9f66c0a98d9f7fcc78e87e2bbe78e6d79ec56738ff81392669790de2f5d61546aa57f645622c8af"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("5bd8ff0c81b728d6691f931e545ba156be647513a16f403341218453eb5076980f9a38365aaf9d4942d6ef76600bd0752009dc96a0effde65a5b5235261f1bbb4502b91714515f8e4ddde3c2a96bddef9a9df415a7a9a572f3ff3b2dfd23674ba5385a3b06aa7c1a4be9cd69a8b4b8d204e4b0b3adb2b11e7418b328fa476f8"
                      "6d836b989750cdb7cdde5b888db3b747e82d5e878b473607aa8bcce8d80c40bd43149a84f5579c6b4dcb388a6ebb7710805bd1224c962c809ad2583623a3dfd6ddeb7a5d40840c79780a7e7eac4a4e7740a5603c184470118d46ff68ca3587c6e2ed35d1ee6560237c515fbf5bc0047abad25e71962d3885d90bbcf73df0a2c"
                      "33"),
             bigint_t("3afbdaec9d872f6619ce373cbf5d9e1d48dbc2d5bf8d71e5d234bb5c7ec319cf9ddbf1b1aa1e82159adb589f7c3929f68a1c6ad323688569bd9aeb447d26e0537e07a79aad5632a5e8c0846e1ec15dd18e97ea5bbfdbb7fff34b80b8d81eb98e6aea4ee416e479f951b61411f2c0a22e005ad30a6481cff7019fa10ac04c0f8"
                      "46762389f7f241797e12f865a883fcff4735d50867082ed6af02a3b6a968a6a052ed85af524e37afb15d29969e3becc8f03895366ff6a5c05f544fe5f67a81999786c9aaabb00ac8b6442d807669a0a295225c11ae3902a228d8c9535acb2892083cd677fd72103073ec760c6e1317d3e81455c0e09689516bc21681c514686"
                      "bc"),
             bigint_t("7237bc0432abdf7e9c87bcf9a7e26b19b245445ce40d5b7440e343542402d504b2d0885e1cff33c753af3c69cfa5965c36225adfd262e5a4ce80141be5f8c5f8aac2379dede905efdab52e6b8f3601d160179846d429abef8a7623c52c72f43299838ebf0513d36606718057d22f648e4ecff4d5950907eda7f4de76557620b"
                      "73d687525f29d1829d2baf104e24eedec19a25fe57276c176af6fea1cd67ad3f5243dbea52205be786fdacb131412e2b29fe173d0d824315893fc603d755826b5bf29d52e230e82e2afcd2708cf81bf0e4a38741473a0f11a5b578e0a5dfd62c0ad83189fd1fefd2bfb53c8fb67c0747ab0591db02d3b997dafcd35139e8f34"
                      "d3"));

        // COUNT = 5
        test(bigint_t("9cc61aadbc7866db61cb03f389782e7369bc9314337efd861e334c7460bac0e6dd9c32e899b88c0e820093a6ef56403b78c459a512538a239c75cb08f3197caeb925a12b7e1685a22632a9022bad2f3e6fb17995292046b8d5bd801474b58797a1d712e902f9c02e61f16d8e72d9880d7fafa40cc7b8d48d1720700e3c310cf"
                      "0b4336e560776e08ca1ee76e5e735c48d5e3e1cd86bb3ed130719ca778cd1bf2ad5b7126f580417ccd0f1c6709ff7eb238ca9679c6f1717ac523689bbde8e5145fa626ff4853e6bc18c3f8bfc1ef616f3dc98ed3d83329049d3181a050341a7319c6bd0b666a40c56714d0041643980f1824c8c805f2b5273cd6fd729ce67a2"
                      "33"),
             bigint_t(
                 "db8c4fcd2f248ebc8674d8175f3046f66fe5ae6cef965685e518d899778ccbf340a196bd076318d94ea5c7c1082a5372a46b8279d1a430327e9f7b8716028bc9b8880d777487fab69dfac305fd4951b6a5dd07562ae57b5bdc01792b4a5ffae91484b6c1a4d6034b887ed00db4986dedde16115db4859f20e51dfaca3891fda1"),
             bigint_t(
                 "b6cda1986b72f5ca4b5313285d4e5bee2fe515854cdfedc7419cf4c000115ca1f2bb9029f474da7486f8f14be026b0e90521100c89af5b14fe644fe2f66aecbffd15198426b08877bfe03b7d16d347f765cc3d82c64c7c07612f2530842f1933015697948bb94ed4ef12eb1bc2f630f99bc7bff8310885356779ce7118430753"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("12730bd3b350d605ee49c672c414451f26ebdec12b583ffca7143164a6b7839907ae689d4fedd469a6a9ebb663071af304c0d2ade6ecb6de7efcabc2a75d31fb9be54e44c08764fd929236aa7f3a57cb78b8818ee8b298300b534c3ddc083ae740cfff3535b8ef8165dc5420c5ea4ca50e083cfd96096dd616c925f6bee5a4d"
                      "8857ac345c6023921a3ffcd4266577bda1063a1ac211618050565aef579d842d9e42bc1d89fc4401d437560278286dd34dbc8da58c945a381804782f73d0bc985ca700f28dfdd8723befb6d5cb7c6a583f0552bd37e0d1cf151e5d67b30aced3b44c53fda93b2b7c13f5ce7022c4365eb795792a62a7eb15d1e7b0e94a8e976"
                      "a1"),
             bigint_t("03fff4f85fe62f42a8873ec273193b29df5e3dd023cd4fd3a365fec4623be736287c900038599b78e76247b3dfdbabf6dd74212818bada5b1cb02b8e2250cec914b68ca3d45cd361c0ac64347aa55fdda755fac0d2a7e360309b756ed32d1d57966c66d7109e914f1b6fd327ccf07b1c118b5c0ee473f8a00990f2a1f32787f"
                      "261d8cd9892efa3804863c40ead28933c6baa8983c45029d5a5b1681544b2e42964f6db6d4e29989c8193af366ecb3290f164636abc2588f63d4eb180523b0a0e293c631382b865af1a218edb015f67e5dbd3728f4246fd2c6dc5bab60d541cb3283c2633502bb3b6071a1e5b514db18fc2adca155ac241b859b12ee40903c0"
                      "bc"),
             bigint_t("6eb6e140cbc3e3eed591970997da7c55ca67b958320392342ba59c3351a78a48490c69e8d6db22cea59f48669e7b8955c0a6e50457db2cd798179d276c845b90bc2739ebdb9e2fb0cb75ffdade1145b7bd446aa164141c649bbd3bd4cef8b3730436120ac7747fc2bbbeb2991182a11c61105fdbfc4d5e94ede81122932b989"
                      "8605c3101c4c479b369cf40deab87e60ee0de6e0f6ca964c4b7b9e4e25e981b6e6b985bc231fb29d11b9a169b0a790f9390a76ca4dbe9dfb0fc53314688c0008bdf6027a594de193ef47cc0f4505334910017e9297a194b26384058287f6036c70172ba1e2897044f54bdf148904b2ad406d196dda716d2278e859b8516a86c"
                      "7e"));

        // COUNT = 7
        test(bigint_t("c25b9f2a113e6da122df3784f9ae4da81f2b3383d0ac214945a6cd6c9e0521e1da7bb607ed0c759847c6530e3a3c9bb8260f37dfed7c89b7abf0d9197f4e3bbd8ce61c26b994a47092b4424747c7aef64385f3d793bcc14fa3870fdc4aac72d3cce366ebd3004f0df540fd275ce1d6f845a6d345d050c08ae3800b4c1abf02a"
                      "084c36ddd0e5a3fd2421c596c6e1499394d0e6344d582c9a5c7e2dd4803c953444f24a61b9993bd4f47a599653499f9f98bb61b38cf2694989ac3e02de3cc4cb2856dc8bd855e25d254297a99c06548d116b3a7e233b10873a6214c70dbcb201f732e2b368cb0a93009b2eac103bb18179dd1dbf4fd02dd361e5a66c384d933"
                      "87"),
             bigint_t(
                 "ee1e0456f5adedf3d00d104af76e2a233c40662544cf9f46639d49ff0d4b9965958c33a4ce8a38a01ff99c1c2b9515ae27acafb801e20a56e1602e1cd53218d0054597e166e18c60f4f1270813b90bdd79309c6e19f51dca3c617139972f502d74973efdf73f6286b565034dc217fa0d802e5e044d308d05014c9d2d38550d21"),
             bigint_t(
                 "d0f44c5cd9e40ef64ce3338898d4647c300e978391d9d222a571844dfa5c7a2998a45dce32770be59460ca17c363683c7b9a0a29844435fdc9dde4ce1ece329e85fb463c64318c6d4a7ba5f36047ff0ef47049f22c6c84c9586a1a38868dbad570b20369675fa495da7102a8945979c56bb435d567222aefcd785e79472e43a7"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("222f494c75e1fc185670da6c0a1a6d503fc3327961cf94bf3580042bb1938785190f9f51bf90ec720f55c3b001f03d1763a2539a9043d2593ae5ced99c399f6a5488f8b49af998feafdf02b554dbbb5b573f0d478295b58e935da31d8508d5dd88f10a5e3a2b98bc04c21c84bae2821402f437bed4c80f459e841bf893cfbad"
                      "f403849bfc26a28d27a0f49f12c3ad060865643c1ef4e3d25da0ea45fe2ceda4cba7213dc1f91cbaaf0bc4e51c14488f47a4f74d88c5f8172bd83543a0cc15e0b9dca1eae5f6ee9e828d992d3ffedb5b12d47be3f63050f0f1d1d6f279f380ce08bd89256366d258bb56873d96e2740bc506b9ccc27b9cba3b422bcc74af3f3"
                      "41"),
             bigint_t("0c4e6cd7e0d36d2235d85c78a51649909b26be7145933bf02cff70bb63cc305d3e5d797238d39bf5d60a1b87b8375a60f538fbfe75cd39b81ddb2ae0e36b3afdb831320fbd6a0fe0beea24c7d1c440b9cc3eec4bf6bac487de98d7e251442efa210e78ee2117bc151081641b68c6ef73b31f9f678fb89fe75890b66198cfca7"
                      "99f4f38dcd9a38be304df90e7880b43adcd0742ebf31b1e0ddb6857155809810091ffeada5fba085c4a897db17ce6a9d1a4bc251cb10e767d1cb2e73a0e4211766eb374ce54d9f6533631e8c0e499713616afb2ceb506254915a3ccdde6f1a008abb5133b902ac5c1e7b3a3b047ba5f21b8fde6dfd3c715064546e307b28cd6"
                      "bc"),
             bigint_t("8b526331ae640aa70317816868b05a0ef4cdd6b293242fbb96d3afe374f1407102fcbfc8c999e6dc6c93e44418dd9d484a698af9ccde81f87e695a0b48b1be61e6d7ef4c7847b7fcddcd7be9a41b6516a23aec0ef9b6e3eb1a439691300119c4950b9187478cbc7a92867e279b940f86881d896ab1ade4db961908341def9d5"
                      "486bdb9f2bf8fc996391b8f47576a49cca50cc90744fab3b687d72f0400f96143281bcfde9633d3c4367f5872ed8e2e6e5ac546b9cfa92114ad4c901b0fe1777a7f9a008807f38099c0ca4be29cf5182798aa04b74bb042fe5505cd5a9e766da8b6f81ffb7911f2fb8c8326754ba5b55a8932be5c640dab3b06bba69b7ef0bb"
                      "85"));

        // COUNT = 9
        test(bigint_t("9ae1bda92193873d8b43ac607463544bcd3cc847c9b6b54a8f7772b57e810f7a5cb2593a2a65d2be9cd81bc0c2281df19f45ae70e75aeff6cfda6b76bffcc493c50f7df53c24ed04e27b32ffca5f480c5e6514f4b3426feb0d43f72818eccc6ed0e830a020c055cf9edbf32f734ffd397b16820938bea0dd3197a38dbcc448e"
                      "e4a31dae3a471ccbbeb77edcb28ea746294ddd1f268651be003b28760a6f32e60dde567fe1a726e2f75d5f9807f120d9bde82f1126a81908579048ac32958d3f1dc33b7931e5bf05ad104f9f68ff4b405f927fd28c98d7b55c5dc2d65b45b900f17425b425bf7f951d1a53c598860bddb95c00a6bccf92c5cb04109992fc25c"
                      "e7"),
             bigint_t(
                 "cb39fa99dd13f25d5bdf54a328a354547b085a0c06f0873fd2bc2a10692eea4ad8f52ed8a31ab8d83a817a1319d28d80023e437ef9b169b7c9c3da01ee55340a8ea2b2160a582b263945a89e5b2b3e1483cc7fdaa92d07e68f8c0e246e53c8ce617d60dd5685f729a4772c81953afaae956ad7c752a63b733fc3d7179382e381"),
             bigint_t(
                 "c319ebb32b8b3e73c8b17bf80a52c761ac1bd7bcf51e6f44e1aa7262fd786bcbac739e18e2672887dbbfdef2942921c86678c002468f21075c8f879c7fc326bf6f02d76c24dd85ada82983f6797030b9400077d56d7adb86d529d86a832de55c3e0494447bdd0944d6b1bebc29ad6ad39cbe490800323079dc7842cb42b2d467"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("00b331892f51055f2faecf0788de855fcd0b5aad1f5329d6f72e3178e6a098476371855190211356ddcd03ee90efa7d17acac22b07a13fbc025079737b86a4f6001eb6856566b7e33532ad57892ba0032afa3259b59841f391e322f96c966ebe653e1982ea04ca3f1892531d7d8ed48920dfe2f0c08f208615bbef2e8a5a8a1"
                      "a96962aee10270d4b00a48f34b82bbbc7b0d9861afd2434439eed5e9b4e3abea77675074b7e5195206d5e1edb19817408e3653ffb021fe107bee13604778976cb2a16e534102006a7e39d3b7a8aeaa1ea52531bb3f06f44bfc50de9fc6c4d17b5e2790b2fd54040ce593d5fe050ea1a5c6af29e0892d1dbe75a52a8854074b2"
                      "01"),
             bigint_t("7b1fe1148b7776a05ce429fe4f05e4db101deec7fb0bae8d822734ee3032396550e65215b4774b07e37f58321ad4fcab32a730414a851dbe2c68befea6a225ceacf643a1aa0a2e077f052ed3db41f65c12f53ccc43329e59abf3b1560c7fc9de1ae762e0bcebd0f27f078c0f0c624a79067950433274a0bd2e9325bcd1dcf63"
                      "d13ddebda0e2edd8224d94f240c87d93ceea6c4b6cf604eb5b9d904072a4b8bcd3e5e16b0ca8dc8751ae669365595658b8d3a45280a99680ae2be4e2bbb194134f0a6760a28580fffa667d293c13ce562474c2e2235025de878f8f6ce35456b64fcad7a67332b5318f0255340a1454b571d3f824b41f4c5e3e068740048341f"
                      "bc"),
             bigint_t("4c0787990473cca7bbcc5c697743ea94ca8803402011e680e889f488989d3cdd324ef1355df407e56b0ca415b2ee896a508dd93379397fe59e805552671f81248f1aaa1220bc5a2917d1f0d13ffc4f0e67f9cd76d81a2b1048c00e6e44b1de76bc2ca1465d07d0713e0c6f55eb065d55f8958616f8f7085e5b3e66579b37917"
                      "c8f67dea2f8ed63fa7b51c3d571de1d8185778ed53b11521f2f79323aed476eb6c950e5da3519c46787ae32b37211aba1019162f25e3b09b8a8a0d8fad230c00efba623d030dcb9061f1d3fa8712c12b6319406b36f178a364771a6bde2d90f4692a23eec16e61669ddc6d3f7abf743884b5639d175f32e276029d4b85c5353"
                      "b4"));

        // COUNT = 13
        test(bigint_t("9f6c1310001d11601224f8167bb9066760ec6a2f393c67dadb5a8aa5388694b3db66f37450473c1d676e219c46d3286cdb96a9086cdf4a8c08718df0236c8a582b18ef72dd511e730a4270b27587b344c72ed267687777e0ebb507cf0972b072834192d72e4ae32be704713fb27bda22198d5e23e08f1be66883cf049ce30e3"
                      "c038b522fe6cfbe0e0c901a147419a5af7b8fd65bab31ec1e7117bced3eb59a072b25bf99f1bb09e0931c26b2297aec46526cad53436257fe93aa04d2b9e2717d3b8cd34f97a735a1af0fcf3e92ac485056cb0649f027895ac28e645bcae46562eeff213170020a0d204e47e67794e04b6c922c7ac0138b15074f85f671d1ee"
                      "8d"),
             bigint_t(
                 "bccefb750a34d26d3b1dc984cf54aae4c501f944189ccb913251e42f6957a572997777431f3bcc96d748a8bc4e4fedc6dce15b969e66801b0cfbde625d2478e95bace3e5f5c5654df79dd45cb35174d32c340ab16cb6e529de63fbcc6d1cdd6db9d8618d1ff8a4ee7a4d792f97a8e059cb3960e6645c67db9e623dd35ad4afc1"),
             bigint_t(
                 "d827e8589f80a644ed1b821bab99c03e8e49ca7535b5d1e79b21a422e66eb3cd3107f1f925b0c3ac81b79c1658a2568fa68204c99625d2699910a4f2f9fdf490900379c1fdc29ec7b2840322304fe53cf081fe175fda9547c665ceb7a54c84a19ad724819952d379e9108e734318a41a6ef27c07b9175a75692c42d7af2d71cd"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("0c6cebff6f42d4bbdc2e2d77f324b4af120adb2bdb162e320caebde7aac4328ae22ae006cccb00907a2347535ae1d260c9acb0780ba44bb726615362c82009985da38fe63001c691c8d6ecd9bbf75bad9ca8b59581baee0905d61249bcbec7a9e539132cda5ad3e1f41b90881a6d34cf3b969d23c6b8e0bfd09d956112a94fc"
                      "e9617d392ac98839769f0aa1eb7e8faa339f034539b517c5bc4a963c03fa73e314c032af01a2beda6a784ccd212e67bd0611ea474bd4f4915fbc32ef88addd4723da33d3857be66aa3728250b37a56e746b127fbfb4fa59272a3ecc8ed7be1ec04162bb00b5a4fd2d0ddcdbe5692fc014c1c4dfa6627679bb6d0faa4dfa4564"
                      "01"),
             bigint_t("4f858749ad637115b4807c3b53839c6134469bafff7d4b83343af111d9a66e1a9df22077bef30b3e5d2a4fcf29ca319dc87a8fdc4c7aaa7a98168a1cc2240d924b24a776597928863f8ba9691317eb1a3ed233166d9088b0c817655895f4b418650d59fd76d350d8f04459a469f80506df9a7c880cf9294ef06384daa5070d3"
                      "e016e1655cddf33d5a3f02464253200d669e67c02d122b12f5e0e86d1b165bd84e286c2de381e91dc792b74fd42285e6a238a186312be671682860f5055232d806cdce24c82ae8f3c1dd68936c44ac0b90f2ef01d9b2cbe8c8d870e66f78ce34e7468eb9b01343cac3a202a3425165a5cfbd74ed30da0113f424d1b3d5c9222"
                      "bc"),
             bigint_t("37c44b047e240c3322b79fdba6c732f6a9442d6231611a3ad3b8d68422fc642a4218b3b784fcd62e49ba65d51f79c1d6df92c2742a550b11a41174de742c1890a39db0a362585f8fe363417f9421f2a375f16a05eae938b24dd4211c058706d66c88acb095f69335513b1fadd464cd219fb5bbc19827ee340614dde05dfcb0c"
                      "d66744ed249b9bb30b68a2611dc64b8a55ec0dbdd2f70d52b20b6b765fd2634bee50be3dd67cae116d2e74518f68c2fe6a8f1f076ff17057c0629bf9e8b642c93c3612e5c9f79af371e5defe1524506437c3e23b0aa198b4a66a0c6617ea9f7aa39ddda0f72888ed5ced5aae4be60afaf68b72837244a9ac2d4f85eb36c25c4"
                      "43"));

        // COUNT = 14
        test(bigint_t("934472d6bb967882781d34211ce267dadf4415de32bc7e9eb9aad88c9f88437e5d99e777dd997ff78ec28d5b884881a4dab8eff3c44c23ab78d5d22a92fe1fe1acee6895beff6cb95db365bdcfd06f642b0a1b73a9b83ca45f24c3dbd924be6b8300bf890458302e9f2f8762fe95e370afba5747ec92a308208e45d46dde71c"
                      "a64235f763ff662495007186e0840ddae74d136f6f8a502367801f744e2d7931ffce25dc23d04f5a7324f585c89d7855b33e00a3fe4b61b7152fd77e16453d01256911945d1d4c9fd51b43bc7fbc7ee33ad57cca94d942d1b85811c4843cf67d3c4ea8777ee75722ab2e6f86583453dba9e87ffcdf32e6c3604023feeaf5a9f"
                      "89"),
             bigint_t(
                 "bbbc8bb69dfe19b22c128b5dbb86083f28919a2b529db6d653994f3725efd29f3a3ae053495e4d9a01a8b9002368d699a9708e5957a93abdd0b8fe331fba54caed7febf1bd375fce6b56a00e008d5c04c164eac7266b8b0e9e9c4ef85a5910d347a77b2b968d11215b00ac2da8d1da7a4db77fb3af5a03da9d68cf8fc00407c3"),
             bigint_t(
                 "c8d0d565d232418277b872d023e7c21a7228fdafa63a1c9a325909f6537eb3a3c1ce18e29bbd71f330e9eccca33166c4f71188a8a2b82394f68c20c38bedfe709d1a0f3741c7e7dd0547c92f2f592036fe86a12ed9076fc879ccb6127eac63e9f302d679deb5ff5478817644fb0801adb7b51ea750e5f80a40f300e8ee7f12c3"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("07fc5bba84e49acca9ba72590dff1700f994c5c222aa870be611e623f0ae0bc993f0cf14f8697a54d07880aad87fe4b682fd29573a2b4e248917c089a4d143fafc622c935b8e9a9f8dfdb6eed7e5e6771e405ce464ed2e8353fccde42393c856fdd272107a31aab7347ef823fd9ddf7dfd017ea47ca727a66956ddb6db185cb"
                      "f8c4ef96d21afb6dd94f368bc56efb9ea22d8bc30615587e0f3088c1aefe68d972f1470228e2281825fbef9f32d7400187f089c07f2523db3cf81792237f910a57887acd484f84fff4eff31c76b5937d1691e082a264ff5865c0b6f8dd9e1c69d4fca3deb97bd36d57f3c5c150b59802a84c3efd65dbc614f27daad20f9d860"
                      "b3"),
             bigint_t("35ae44f63e8d1525652ef5b60c6c38f7d7a85cc1adb80c0750b7c901c18944279b40dd75c51a5d2c002bfce70df4ceea80bef3e8093dbff61b1fb13bba48e49cbd276445647524f3dd4698f299688dc6b7db36f906d631fd287dafc2ba1d91545c9651b75b6efbb371bd986f1ea24806d38af28fbe2983fe6d18abb05002b23"
                      "9157d9c17bd7504cbec46f8766ddd2fa8708fbb3e325679a55caed9b823caec265f4627490e536252b252c41111046290c5cb2438b01317260f70ffa6f98cd6229a872486566e62cc5b14b517130852aca8e3c2236dae222f410229878b291679d28579cde3d711d235eb395a6120d31b3ace59431d8bf2201de7fd4a877ed7"
                      "bc"),
             bigint_t("0e32579a2cba08ceaa6c07bd09591bd819b7b649cc3edcab72734d0ae6eecf9f41a38b6cffcce4c7bb7ce45f32592d50692dd7320785cc6a4e418ac2af4b381490598a72314b0b0186d97bd282092439bb244af3371bebcd9eeac4c5805b70389a84ab81f0d9651c1788e511eb0ae54fcab5c7acc4b1673edc0c1831e5e1443"
                      "e7f00db6698e3e7f53d353bc98c16efbde0d0615f3044dba7a75c1fd9b5ccb0a9c4381d4b68c4d7194fdd27227598b18563c120eac08b42147317296461327c813740771d217dc49a4df6d300f6d4ef17b91989bf87fd05ed4716f60f3498d73f08e4f2af54548a1e07db2240c26e7da0ebd58402c5075aebafe21605579cd3"
                      "40"));

        // COUNT = 18
        test(bigint_t("cdbc7d659f98aba9a7ba2e8be0e6f5664ffe1cef25395d21965f9e08fb99574834c6acb8bc6fb3281ef0bb6c2e07211f056c3fc7686ac21d0bf737c0ea17aacd234bae70fa8c7520ffd7e6663677bdac6432e4d2c47fd45bef7c11d75f27d4be0e8c4caeedaa8b6fe58415679ab15e604967c1f92c9f8705fe80c3ded59793b"
                      "8573dcb221c23446828250edf89006677deac83b64a36f82a123711b2f0a8073e55345375e250d949b8334c1055dfea02dbfdbe83651153ab1d8e309bd14c04ed85f1a406c78e3723993ddb2e790cf8319cce0ab2b433a77bfaac8e153e78467b9ce5a40caf8c685e738a8756f31ed6657644e68523f76e8f1a59cc0327f426"
                      "21"),
             bigint_t(
                 "ee4923856ec362f34e3f905953ac44ca4f87c34d764eafdc598d93e4aee9deac43ff28aa4192eff5cd58073bf1aa892f9b6b094b91865981afc88d7275b30c645be960edf0cc9450431f5c97ec3cb420dbc146394d4f71e285c3ffb89612fe697eb69a4fe77855b8e21728a54d759fad40c34d0e2fd68c0bcf2dc71831c2f579"),
             bigint_t(
                 "dd07e59f0ae14733286e32b90995e4d6c07da015fa1b4d3f63a3269a0cc0c66b71b8c6c814383e57d0416f008da83f3a9c28dd3c8e805fbb210a7a62ff008eba7b26b815bdc54c8b6e42a906a38f0f32d41e783b0aedf44c527773a0c26dc325e861eafa9e3fab95750a4b3efde2da6f9739c3cd26cbaf55710c75f43ab0d3e9"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("0065d28bbd58447c0e670541443591daff8edb728bddb8b1f376532819579afc52de4fc7608bac2d43f6ce24708fb82bf127ad706b2877e42fc3d0a94a4481e872628e534e4f6c93aea72ef47abf0bcfdabd6b11e678b16a6f6f129a973c3281362e29ac8ffa63e6f627fdcf583e2ebb0ba4ccbe1b956f897a55be4baec2723"
                      "00d0c9e333226f14d55ac5995d4367212bf9e75ff59eaa2152e6a77bdbea3965b8deabc3815115350ae868777aab2cdd71d7f705b471715f6e61e389e270a8be455dbe82f3b0e922a0e4b7b1bdd3226c6f4f405a347effbb5922311e8cd2f44c30f4f9cfc0d96c73c2f47c150b8ed3af0a5d57a22b0cd2e0d3abe70e9bc584c"
                      "59"),
             bigint_t("0dfcaa3bd30c602238bc936160a618f96fb6de645df2b9f1d33471b1a6f4932dc535b169c6044641cde5289c3a434400593d87116e7a5961115855eaddfbc345d2677610cb475dfd435c70fbf36e3287d7d02511758fc3744b6b22b3823205c78ba45213769fe49b670a118df46b7e877359e0ff5ffbfd76e196f018e0ba8cf"
                      "b2616888aee891113890d78cd91555ca574ee73eafb773a687ca437efe10945c681135cc2d5859ef446e1ffdbde87d8195534301952fe02d0bc101bc7ff8729d1dd7fb601ee2bf4cc410fc91f40131f9abd62d757e4ee132cc8f20dd2cd83c90edf88021402f731ec17a0208e8e71bc940e45a06fa54a1644fca3585d5dfee1"
                      "bc"),
             bigint_t("429d98e4f9be2f259787cfed7eb8f9cbdb259b1707019e73c07e2baec34f479d60f613dade3e09a4c1cc703e558e07382efe9c14e7a02220b18e7b2e44d4843cfdf276d60210d3db28f7e3f3bc14ad720a706a5608b5b33b4ee6e9f9fcb2b725ea1c51db9ced1b4605a8c0cf29d3e2403699ac2e96206f51a59c85c8e68fe45"
                      "a8a71e69c14e865cd2bf92deb756eae2410cd4c51c1b7fad52cfc2f280081a0db68b7160a2120bb2858a5dcffe3d88823a60035b9d85bb29eb9e47e012d4044e6b53c32b455623f3a7a89f15d79f19774d43bd8e6bc753acba2864363591776c6a254a77d61745c51e1cb15b7890009f575545fcd5f9938286732e52292d10a"
                      "f3"));

        // COUNT = 20
        test(bigint_t("98e9a73ffaa11016b424f7b7c7704170246013400683821fb07c2d5fe5651393f4aa38a6b2b3717867cf8a46d626d41f21678dbf1766f797500e8f01ffdb7db386a6b7b248b17c54b9cf219f29ef32c2a6e9996fb3d8cc34dce6c0346b96bc26456073123d4eb2e7b931bce0144838127a49152a22a0127610d2d5a265a6df7"
                      "bc39f4c32553ec7dc85d4344343d27a7edcd03fc430448c7f898fc6aad5aa8eaf38813d866fa7875ac31ff91232854f058ed8e4e86378b0dc0e213484dba45960b2b9462f700447eb828d99dffb5edce8142f43d3944f1664bcbe455ec6261e9544d178f59039a6f1cd7a9e863a291546f889f3a03dde97058ca391cf3cca89"
                      "0f"),
             bigint_t(
                 "c0e3169d90d48aec9f68959b9db0e3879955337c74cd970997a7e5980fcdc30258290b3ea8a56c1574fb342df53c9d968367904d1401466a5c16cfdad9ddc12651360b51654be0df885495b3e7e5cec21f40141adc9ee87dfc31bca5f0ff6e189b7a311ab1cefadd92669e84a0bf9725a6db14763413d25421bc044bfd36de7f"),
             bigint_t(
                 "caf22b724eefd88b041717145e1c74c1018c815146849588cdbc9846a880955a82faeaa1c4784525ec10ca98af5aaf1092fb98d2d08bcec4528f7cfff56aa49daec5510a9f836d1253c9519afa790fdcacee0e31060c9a206555e9fb5d396548b48464213224df324dfb47604efcda3de74b5049719e6718766ce5651e8a2d71"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("0481e55c0af0c2cf25d9edc8128e6f620ab31ef8cfadad0bc46c31fae0d705128d99e8ed4258ecc583ab32a46f9299c51dd0762005ba1373f7e2fdf9d7f2148ca806f3ba4324a1896e7d646728841f07183176eee67fc3abe3e914b856dae21fef57ee51c17c8ce48213d7c149326f1e0851c2d8739dd6a823f5d060a6f0c4d"
                      "1f9a15b2c1f320f91de6411d2586df7a4b4212a77cfaf0754850640f14204f7aaf4dfecde9560e7c4140edfb2be1e751e6b7198fff4d1d021cbade29f4e19a074398699efbbfa1b4b9874341b77d50548b6b67b27b5fcf591aec5a23c9688e84dc1efe77b8fd04794f9f95fbe8d643cc3d587d56c846a7bb5d305bc92957463"
                      "51"),
             bigint_t("4d95d5189300c1a4e75db7d4c686a624f97319ec7849eb77ce17e3e084401e4a6ea8d6cb42480dc5271167ffdd36d266e0f1971977cbf5f0fb811d5a750b03ca6eed0f781bb3dd02508e2d51ae31bd0a1cd34bd4c9773fbd7109b27bf9e9f9602031a382f60b8c7be0f805f05069229376910293e83700c1db45284d565d718"
                      "c672ce35f4cce07a71bf185f0d9e897e2c92b060b1fc0f04907deef4ee54756fd24b272b699348eda1a39c07ca2b26ca5e56bb85c6eb76a1166be4cc66b4f3b92fbb7bc0aac4c8002d06e894e79e98d5b18dd8a569b753f37b09ee0b79e5825a488ab890d4b871387a02d69614066e40f88d21399aea3780d75185c7e502a29"
                      "bc"),
             bigint_t("6a875b3a0c21eea28c926bb4422cfe2814bcdf42aadf219d3a562f49f9624e722adf8c53e88de67cdbf948119446b1b081e949dd704589b744e52d3d49867b1785cc118bca50e9696dabb492eecc31af0d40143d67addf20b7975318a4537a842243323af12a2c139cc3007355dd1514ffad2e6f9e6d82bc552e645b5306b07"
                      "5722342677cc22453f665841a08f916311e884d6a53e7791d636be3ef5b7d88bfd59b792cc558047b1f4e6febf484a40ed0da68425cce96bc110ad3def6712954d92d8cd0cbc98201cf6d7b4de47e5fbdd7a68cce37e6ee1fbf598283550219d3330688b765c483017b6a8fabdd2ad7fc60cc6abf12effb07368022843de020"
                      "76"));

        // COUNT = 23
        test(bigint_t("b233a6f3852556ca64dcaaf7a7a6040280ffbc1ebc2ca20538be2cfdc57e2e91a240ea1f1c3e4b2255fcff5322539b98fdee3951af8b1e91e9489329513d726147f0f47a14dd1e4b99c0ea0844fae41a08101a5e702e53a6f363b445ccf194304062b4973e9385ce7c9c0cd11282c50cd2e119efeb9e09a44125f87843c5639"
                      "8bf6152403e71cb10e30131f59513cdfc3a488aba00365d73c94e4f552b98e5b67be0be78ee3e8519341a887e7a6e848ac2dc58c77ed58234d4afc08ef4e782a0ef23a9f68feba2422c79f7ff62df0bcc6dcc4aa2efb988cf789608df26f7834a4414756c6cd42d04784117b4dfd589207a80348b1677d7c7a528c0bbaf11f2"
                      "3f"),
             bigint_t(
                 "dab80fa0a0344ea146bf20e5731c69d5ecba40cce048277b15908de91eb820ddf3ee556823e8b85460c58a485b9c9f632f2be8e41607e10b298f721cb1fe68b50a24ba2b5b124b49701028ce5c68a173cce0bf7b0e4db0a231695ced525cb367bbcbb8fcb75bdf2f293222ccc3334c412a4ae49b7ac23475f692783986b874c1"),
             bigint_t(
                 "d09398d13869df340e940c8b4cf75619ff38cd2f022cdcd16a1e515769129c6a9391654f95640b024c34d7f784993be23170f52082e2261d2a715a4b6da200c4a0a45b6efe5e9ced76a211b2571854ea705d82cdfd08305feaa900fa3a8cd9b5d4ca52d11a3c95922757e727a029019319d0c9c833298bdec0806c84374126ff"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("2d120a4e3a2534fbd0a1cd2e00fd68b3376c36e8b1203ac44653372ee205e58258fec118283d0b24be89d40bf339213803357ed2cdaaca8b0ded18a7511050ff10c27d1ae96ef98c1671a7df7a8ed8254b57c34b9264ed2c630f9b17325c016668ca6e317896b8377ee5bb0a1ffbea19b1b654288db7b569913ea8c6c62144e"
                      "c7f9c710bae4e64cc9cab6022367b2292733784ead2bcc5af3d9c312d4f36c39bf7bc97c6047d86cb8c00921bb046d76d2089bca351a28ac7213edeaa79b3d7454ab0fc280c7162df9a010848e3d2d069c3a710aea7a89ad6c268dcd36de7cd15063ad93ccfb73d85e2a4163ad26099224ff981c2b2db07cd3e3152283a93b4"
                      "c1"),
             bigint_t("20f14fe5d3c81e2849a758ab757a0631a6c8f1f5eb90721f9901c0192867f11c2f872eb530f411030214b65bf3240b52a7872e024e658fa66694b57219905f7e811cee23dac8e27c10e1182048e4ebd06ddace6e4c4e023d1fdfaa2651d675a6652aa108b062116483a51cd710ae1e8781cd75c500f28cef69a9d99b2561684"
                      "87a6b9648c129a1adf88b6c00a889b0ac0cafca159dfe59c86bc705a10bbe7cb9da644d7beb5ff85e38d7e575a9027fddb0bd049eeb905e49d2519e11f086151c31497fb328a55bfdcaff43890a7a7c8e0e8e61dfb5b3f761273429484b142227e139ae57c7d6b7f9f8cb3b3d392b4262dd0c679dbf43e7d61bb42d830c78a6"
                      "bc"),
             bigint_t("1959992867064adf296177c30d982deb2c21f4ac4da19b1c0ece7ff8d3a062b140ffbf3d4d01463f001d9b012b999072e1f916c17ae9d8a5aab02c086aec3b333256e26cc8a770a66f44e5cc77be217caf22a70faf21827f1a953d1c0cfce59be7a8b7b972c14e7b838831440600770d1c3407ff8e3fe535e384fd171a8eeb7"
                      "ad9512b4209a7177dcf305de4e7ddf67990775a8136fefbdba3457016e1ceb449c2fca76131b972b45ddd98e7ea04f2814c165d1efb0060790c59e619cdb27dda767ebbc12ea092bc74d5e2735a89eb93629aa2728c5a3226306a718ec156e2dfe2eace62f4a2339b5118772352a4b53a3b8205ffd8e41718a0247dc25cdf43"
                      "c3"));

        // COUNT = 24
        test(bigint_t("bf066f288105ddf1588b75137b300f34c56e8eba99c14055f6fca196e756d6cefd07c3a64301e91d6f8cb2069f601a75e99b283beefd1a170b171b077145f27da86b09a7f1f093a6dfacae422fbe805ae904dc63546f1899a945ff1a85c52cd6af167c14c1aa1b4e519dd1f9424343a6a22fed376a242db5a117aa5ed7dee8f"
                      "4acced8834222b2435e865299493e278140e5951014d82cb6c82403c72895febc3cd208afd368caea0d4634e32e33488b8b7e62fe7461da42f7acb24bce7cb9e1cb5e379ca798a1b683bceeb65ebbc1379d3a551f38f16627d9475780898aea6fd1fb23f85870d886d417524e7fc258d9b2aebd3af5913488143e56aee26b07"
                      "8b"),
             bigint_t(
                 "c01683670b8ba1c56d653fff169048e6f7fc190b7536903b0a4527ce48f5e6ed25ff26e82f95aab1c9011e5aeb5988dd2983dbc3c1e496d29b43de3a4aa6096278e0773fee4fae86dff499388d74409873e0991f79575bbeec1e68e9c35d08e70021ca228af14b4a022750d9682ee138a4e63777f0a3967999e147297082cd93"),
             bigint_t(
                 "fe9564dbe2915b9f3d93225aff9d7c60918cfaaaaad71a557702d7b74abceb16d88dbf0c9df0559531529262d28130f556a359e879146ebb174544ff93026b70f46bf68a4a589fd67bf9605532f0612e48464403be3992d7cd543da38a9721ee475e1f1112c13180aa70cb75ce9e6e740cd46ef79355960f12b5feb68c005929"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("3b64ebc10344e8ba36d6bda33e0536652911f40ceeeb986e622a30aff3e41b4c267e7885c485de8e89758c90e49304072f884f9d3af4b55cfed4c351b015988d1b85946e049ad1bdec040bf68653cc22f81acbcf929c2c60f2b1d1dcd444f76914ed7fcd105e2cb950452ad93e921d81bd21fbb6e428b591a8b1ec062b46a2d"
                      "58dd78d2de3321707bb7a617db509de10a6037c30951d361462a9dc24ebff2a936ac12248564f2ac4438ae62da1ddd71a8ffd1e1957c25188bf238c437ec6f165c337f2b19c4edb0b78862fd1de51a673f9778d31ecd7aab593d914dfbbe26656cdc1fd06d82d81e661bd7af39222a624b90c2bde3bc6928e06137af569a48c"
                      "51"),
             bigint_t("5667007e5b4f3128e0d0d5c00783f5fc73c1de40bba350db7465bb8d173536340b57006951e7405de9315f76dfddb1fded9aecff704a20364a834925adccbee7f3789526487da4f670363e681bcfae60a3a8a0e9aca136d90ce462e94ff593d7d1d5dc4dac35013e8f48587cdfbde718576661624ddd514ebb0f971829f9a45"
                      "6935cdb5e8b7ed7ab5c275a5b876171cc00f65d3734bcef9f839129dcbd4654b9ecf0d4e0c2807e09ab2b0a2907ddb05a66841f4e06e98055dc8af51304e364cb09c42a5c7f756e9047d3c0434fe1df25c51af95ab2cb6421be2455a654b009c0971f2b7b543a606db9a1f05e5aa26d84ace565a22e2877d638413e8a82f7f5"
                      "bc"),
             bigint_t("68fae70e4997ee2bdbbe858dd97c5d2c39ae157d510e95c24dda9fc63a2b056cf0159c4a84f2e0d44a5af026e89c205ac92fefc2724af03c43309a7dee120d252083cbd43f82901b39a14f1c42fa055f1723ff7f6326f257c382acfe6486676b0e94b1c000449448a0a89f4ab5b84504d4130dea6752cb3aa22e8f57c48f388"
                      "6580de1153bf9761ea0a7c6c88411061b03e97c9b2a376661cea7ccb969e8e25c2d1eac4c98038483c9b8de73d26e033f9aa47587d44db54e2b00224f64e1aed09e5b7eda74568e338b11a18e4be1e1618b8cf0c4c18f7c9f9bf67fc5226f2131fbabf3f026c16f917058a0b9032a66c9da8bf2a5603781f04a36dcfde48b9f"
                      "13"));

        // COUNT = 26
        test(bigint_t("ac9f9c620fb18a36ad93a206e86471a0cd420637cc5e03114813a6e0c37de6bc57a189cde2305aeee9fab2928edf6ec478d7895c7432f64c8183c9cf211e58ea32b5202dbf29dd5f9ece3fb86c298b562a7e3bd47d626ec3af777c9b6b13272cb6f10246ae69de01c839711e2907e1cf5aa4760ea6a1f60da8d2797e700c4e3"
                      "1ab4a8a4df48008c8c392ae759d3d1f9cf582c1caca86e77540a53a90628bdc4edcc91cbfdddcd6a1d156f28180aed8b6494bc0f622b77a549b404ab1280b4322bedf766b4d1a6caf5a7b088c3b7c1878f4a6007a50bdbad4e5a421247d1f436986a2554601010e758031efd6daeb089f13288ddecbc51816fa69b79e29b3b6"
                      "29"),
             bigint_t(
                 "b6eed166bdef9c4122aa2f1f4f5aa372bf90382ed408f823b05576cc5b0d54588baf7a2001143126956f41b3de0c4ff691e09deb5a2281f93b3287f6d75b23595a30f108745fe3c4ad01d5245e5eb93fb72873bdfc596bf32114851b8c3f4e58b4d47c1a173c65876b67352794919bfebcf90c5facdbd4076b2724e7fb555467"),
             bigint_t(
                 "f192a3cd8327a7882fbc11a3291b364d3ed5b385219f962cecae1b7b0bbcf57d70a2e619bce7a034cb88d8d6c769f39b7265375df1d246d7151e9a1d5fab5ca69682154a21956f220450dbca1b2d33cb7defa5ae969c217ecba4dec498593f9f9dce52a268500cfeca175f72a444d06caa05ab9bac1f79fd5cf74df964c486ef"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("1d3bb87588d7d5a2b5d9730e7d6b892ca4a6023c8fd0f9ef02c84531fa1947aa6ca2b01e1c287935cd8d0591b93d505a586693de02d5a08d4b3fa0bd018c4c9d78b1e0bdae8a6df16408e4949c489acea04a99623e4b9d9b690b30a0774d1a8cc17c6618c72fa9879ebb4f933e7a95bc924a98ad997d8318441744b8eb4bc22"
                      "ff240d06076580f487d077b9a636795c6ba58794f0b848e0f1ce0e3095d0d267b6ef3dcb05014c0da716c3661993567586991fb9bdd10c29437467217c5df3b169958c031074ef2a35f3634444494221ea75a661f171d1fc68b85e140824769d1be6f0eff195bc08e911577a578834a029c2cc78a89a56fa18e3fdde7a4b277"
                      "f3"),
             bigint_t("52c6aeed1595f2ae4c14671a3abf51745bab58222e390004d857a3e59ea78a23b9303efe71ccda000af5c915b9a849ef4084613113449fcee42791e66247a6f66270891deb98b55f5a6b72f823d92d28ce08e9f2c5a36a631551d09c48693d0c8affe60f85f349c1edfd392fe32c1fd8f6b12dfd89253ae671b2e59e9b49775"
                      "13f625a5fff122ecb707e4824e823dc00e8060853162589795c86fb302abe989a03867dfc94ef5ec098554e665096f753c1560b8a083f6a3ab802c9df69bf3f3adb9cd60f4013c846033c29ac8b1a5be3dd997357f9ed7a9cf9f12443bf22bc09f5cfd605d4917f051b41add839651e870268ad1aea8ac8f07775ac57ae98ec"
                      "bc"),
             bigint_t("3d430d46be714a410d7cb36b6ad146eb86ebcc9782a80b84620ea7688debe51181212a344f6601d366c5c3023599b96852369afb3e7cdbdeeead7a5d8073b8749ff6feec90191032739a220d809af484f2652358c0ff21d6bfdb3eff7faad7320d57731ce4a91ca93a376bde7fa4f4a8d9bb05be909eb3507aed997f79458bd"
                      "63d2e4215130ef67d1f81a62f456957e3dde2cef57751455cc0496dbbe48cdcc9206d76f70455dcccc8ac7aa96a1a391d4ce447e7bc956eee0b09af623fa733e8534e580594eaa09544544961c8cabe666cbadd12d3dcd9d3564b000b9aa7737a35bdd3336c61c903eb7be9a510d43eac47f603ec4a1ef9ee1bf30535505079"
                      "5b"));

        // COUNT = 27
        test(bigint_t("abde7ffa9a4bdc822b6006521650a5a351c5856814d48e67bb593f68c90d322c7136626d002314e0518dff53ace11f87d1da0e92cdf3bab5bb06e558dd6ab9e6b781452edf8696291bdccdb80a6d59c2d848ed78da78369d746c7783f11657ed8dda01003c85f8b15e4ed491a838a6f0c7c95c83e200134aedb15559c9b0168"
                      "78a72fa7b83b68d52fd09c24015e3ef607c49b6d5190c1e7aa1c942249aea7127aee4654b7fb66e4043ebe78448cbbf89d7d7b3dbbe7aaff4a514f2f9effefb4c148be06fe349d1c55671363b9848ec9ced574ebaa1d30fccf53e64eb2214ab9bff9836df0700e6b22c9b0a7607c37cea6522ec04c9dfbd5cf391747180e94d"
                      "c3"),
             bigint_t(
                 "beff4456ad6631d6cc5d20f24f0309d69af57fb58d24e87a1d0321801631979bdaabf703f63c53e7cc7c36164c0e7c41ba37d0642d732e188d900cb836bea6c46dad29486644375519cba1323574d278bd3d802726720eb03c189aa3a7f818cb62d2048a1ae40e286100c5647c229f4c6fb91c3db7085ec28fcf4bb4cf791b15"),
             bigint_t(
                 "e65cb20e5042f4a72dc284fb48953ee13a25f9bf0b66d6b1d01cb1c1c4a81e525aa54df7aa837e0b91ec59ec3326674ad7718742c515bc32564d9cc3151e97b72486922f1fb2f0b0ee120250fa5c32225754bf805e3c4bd8f2a2a1a52b25e2aabbcf691f4f3735c1e083b911abbd3c7c730a0ca6060103c6b88b3516f9589b77"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("08b6e87051808536c8723effcc87dc4e32a83c4271c3f1508b2c544dc0dbfd6e4973437c59646dfb8dc7106a32badb796c878cf39ea3bc14d8ddd9a52edce97e2c51ceab38c43f3ae861c2c0912ec83d5bf01eb2610975637872c3809a29d2dd85b2c71836f96809be496af7d3bac3a433cd1ef6c4efd20a9b90c14e232edd8"
                      "57e7f3342dd1b32da9c2238bb6a11f5ea5947bbeef7b50aa01802fb4f5e03bb2a87e290e1dbe14f3d8ae9a4f73cbc7c7d1420e7a5f9ad51a370db97cb67111adccbaf2278c16a9e96c7aa2de751ccbaa3d554f095bc858ef757ca70b60220f9ff81afa1b629356f92897604353e29f1929c64b4a5295c7910807ef1cdb64ee3"
                      "e9"),
             bigint_t("64dbfa7a11f38185469ca11c96b065043d488f9f3900357aea6ec1047b6130a9fdb87703028c2b9f1449f001f15cf81f8adbfc5cbe14a56979cc4e0ce8e1ecdd12330540065dca6bda464248ab7554a172ce1ff5c3313e04716589cd056685aee312d4c4187f461d47558c6c6ce4b552dce95d8bef986eebabf69bd5ea4da33"
                      "96f3c9b80c14522e67602a2b36afdadb935ea920eff4b77a82d7ccc5f0c00ea193cf7ad461bb74bfb472f3d44cef391467bc769a1e6337b67bd3b261e1254fd580374accacf388936789ca02bcec922a7797ac94bc372520400f60d4b8e6c555623b19edfbee4737f38524f45dd43ab03cfb6f04563ed50483287791249e207"
                      "bc"),
             bigint_t("4f5bb8c8bc6a93b10efd994898f5f93b1d544c6800c56af214ebd12d82acb85b7c7a26a207f748ad64734257c2a17225423beb991fe09c9ffaee94b97104b3c2dfe1b6b169f3b9c0c8ec9417594c2c51f79d77d948eb28a5cd426ba186fe9f518864c45897fcd7366ccee4e553714f0ecedfa89ebd1dac26016cd833265e5b9"
                      "8c06781d45a46373e4d713d12d20016c26a6c868639dbc01b317f63022479b9306d2fdc1ebc27ffe6c88e4490f02f7705e310000d873c6fdae96465eb3ebfe82c0d5eaf790bb1528bc5499acf6aec52f8e97c1a3b38860840ca2857f809adce76ee3f453a1680348a5beba889d908cfe1bce67c1ac19899d86cf7ca14ff5c5a"
                      "51"));

        // COUNT = 28
        test(bigint_t("a2fd6700bdb5cd6d3244e83c6437c677f36a1ccd1becdd4805a254a7d80a6d00b35cffce126528e6d5cb7a3ebf7c7138eb9fe8e50caf470781636cd03c21ecb4b8136e108faf4064879fee09a660ad7b4c1bd1277becfc9dc7f6aa279034ac81602ce6df0c95d6386abc4dce40216efe1222bf9df6630c47eaf4bab4787019b"
                      "934215edb708d054572462e4dae059720bcaba60693e624cc1c05cae4d2a7123b99adda76bc2adc95e490f6b011ff4f9bc083946edf7092a8ecbc52c1103b76946b7b72b0f06e3f588ab79d71ef256b39629422c5dfba1e380cc70112118c7e1cac8ad85c1f6b29362f067c84c6bd3be5c98f21fbcfd6852c7608e34a27129e"
                      "5f"),
             bigint_t(
                 "c4c4876ef323af2ce49fd9f862bbc939c3f0ab398286f19aa77cc80be6b32cd76b685e633ce4621b385ec90c8be8843e647134fdd91ab8603aff3bb0d6a556e8b2bb73bbc5f699b30db11ef613d5386e3a69b623504d5025780d34d0b7f0167c6fd704d9205d05ad76f4fb9d8f9041c089508b92b7094e13701839af5238fc6f"),
             bigint_t(
                 "d40ddbb35ba8654b1d682cad97b3b774749b6efb7227779c66432c73f770976e5db8b4d671b26027e1b72e9312895d940870c0818f5df59c34ae7c52977e1d64754c2e1f8612b3a0a9dfdcef681b16863187b61e5df645d53d60eb51e0585700300be2ab61bc570f3ec1269c49ddf7a6c4949bf2c05cb291a2278cb220975511"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("08ad209738d7680610ba50fa80ade4bcc1e7bb7cc25320e71295b4ed47b781074685042ba8c305cdb3ed24516ec965bae2a01d3a5980831c1d2bce6bb9fd11a7ffc72e56bcaf56e82dbd650dd27381b34c8be1330edfc0b022d2e34a93dc99f4e67512c7d0d85bbcf3c1990ab4fde7f5fd152874ca318a427c7966999b7256e"
                      "1753d0672b11f4a32456ffeeb131e418cfb078ade851be70fba361c2c3cefe318861a2db3a1a36950a598028d3cf9770753e7b3443565b620907a548bbfb3337bd900089fe179162bb7946e33c1cb93f8ffd562e33541856319369152c376776ec2fdb5a3ba00d40d00ae3700421305f8f7059cb596b8f69fc114c045e0429f"
                      "41"),
             bigint_t("6e7d1908df8aef33acda95d0ee86803056a452b9293245e4f3f965919b21cd2e3771c321fbb8547f91448ecb8442bbe26097b06634c959213327061a699a3e5ed71031a8b5edc14c86c37da72cd57bc28cadfde420c92feeb8d72a54f929a361af35333c64b074bf6c682849746e769d23b90fcecc58908139d6f7765d86b00"
                      "2d37c31a6c28c092cba70e566d5a4b22ba2e8f7ed76e4d4fb531cea5839811677356d55080b11f854b883331f866a5e1e2f1fd1290b41395d73cd1c82eb145530bffa2252bf7adc238360640e351c64777a171818eb3feab21a826aeacfadfff60ce52749274de3fb5c481d794b14f99088951f988565ab704de57b9e1153ae"
                      "bc"),
             bigint_t("8b7e146c62062bc84f9557e717d6b1275309023405cb9d0b2f67d4d5377514245a90a67f75bfea1551e2e84a7a398d0d53c6bef92b26f7133f6bb5b483f3c28204d06b846f763cba070c916d559aa300e12ddb0c44fbc5e244521e9d3d8d3d4205ec2ac94596a85f5910b0a9d17e255cf7acf59ff02983de58ea7d9b2f1cf3d"
                      "1e155878acdc81bfaee6f4371d7c3c9ed3e61eed461ae1887ec2a62d60bd08c8ec04d8d5ae961343f509494c104221beff95e2c9e9f833dfbfa166b96644f293e5764a89361157cc845ab6b8e4b88110be287ad5e33a9f8a7730f72f6e16ee0d0c361a33fe745ee25b3941dde50e105276cfc123b4d448f983e31430fa53fac"
                      "fa"));

        // COUNT = 29
        test(bigint_t("df076349ce47632265f0bb8a2ff18b351494f94c5af6ab4abb6b06bae831b286e80419cd03d74fd359c2555bcff12aa9beee716f6b0dcab05f73f9a5896e3fdede9467be6a91f2a3e4091648afa3b3c8e45b28ab42db982a3823cd0d3c37f55924e5ff7488f4db3280df21d84dfef0eb8d3f647b002796147aaaa64bc5ea911"
                      "2d0cb179d0983ccbcfb69dfe750980e43c9503b09e3b11774feab0f1beaf3b782cd5fb8acf42e6fc5b90696e4e3cd667e071255553882c6dcbbd904df698a5fa5823dbdd25a7d460fe980af18270fc212552c13f6b6b928572aaf85517b9444d899d803dab3811d41d9d5bff400e15b0e127210a24dcbb65456035f62d3a333"
                      "dd"),
             bigint_t(
                 "f6e872fbb4a52161b5c1668dfb272f5bb6187ea5f4619a20b3c292b8694f5d39c398952a8846e967125c0ec34a353bd59b6cd200d1b9e7b7012a1a8480c2c82a9397e772387172e33a8f8103ec738d0a767fdd097f19d9e135b020da02a16f85f034b78374ae09173be7607880e1f902d9d571d834b34cad52a581647c452c93"),
             bigint_t(
                 "e73dd5bc93265b3a0d754ca93936275d6e82bff36892f921f6c9ba2436fe82ac4b55f34ccb83c8cbe3594fb79d8f585deb51fcafaa08280c21674b66a1b2ccfc47d17879398ddbd7d4bcf4530779f664ed7db57b640b0df64eab7f3dd31fed14627468dbe7b7b0253ece5da18ae6ea9b1586c2f0c003572d95e13bc87d8dd3cf"),
             bigint_t("000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
                      "000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000100"
                      "01"),
             bigint_t("0d75222a196ed84e028db09b49824f2e9043def91b02085af58d233652fd70b2d03c331c5a4f7b2b1b1eed4ca173016a85b15899eb01458a37f0450f9687d705c933764118a81ed681d3848b3541f0febdbcbc6afd194808c02374e3880f2e58473033a661ff385cc746ef81604f72551c4c47715e9484eb4e6d8ea442d8811"
                      "15f3acf9b210f5092789429eda47e6c85caa37b1a051f49f0c97fa3a3f30aa7c476ff5873dbee38c9aacc73557dd6406e2b691ed640e77604eee951ef967c6c2d913e7e3e092854acc38fe42e42ba1dc2724026a26a2c6769ec6ce287847881f8040e91be6706a995e364088890cffda10014f91397dafc26e492e12cd3ac4a"
                      "f7"),
             bigint_t("7e7d47db2a54c2445e5a5a00243dac1d9f09d15eee612b848ed391a21572a190bd1da02126ec908760685d6b7850f5a3815fb9e23642bec315b83df2bfeddb9bcdce908537571926c690de9cccff89dcd3fb0c253ab554a2f7d1937efdb1acd6da913b028465ba7f4407bf5be0585596111fb5b21d1ca4682fe4820c0dd6a48"
                      "698ceb8153edf749f5fe9f152014ac9fb7edd3d9b6c1f85087e679b91d4450ce0c38bf2bbb03953f87dcf56c3e0205952288045b6852905de2baadd3ca9addff2b11b64841714cda6cd1ffdae5f9ebb0c82d2706aae3f922e441f1cbf71e366aef874a25eaab089786511c45d5f54df8d09bf072e873b8b0856334477c1c7a4"
                      "bc"),
             bigint_t("68780ba640953112c66fea490a48557a5775f46130631ac9c43b68a3dcf4f6c6c3c140c748af7d8e43fc578ebcdcf9d1f1ed0ec2a88f7e9570604e0355a4228ccd7bc91b0fcd3adbd5b4a45e95bb759cbd1dbd1c0dc96f2bd9078b9ad68e57b7f2989539532a5cf241bdd8693612fa778cf6bc0d147f78626256dd9f6deaec5"
                      "3592e9feeac2b3af4c4bffc77c424c796f47a83932357c29e8e2ddf63f2288323c4bb06ee2e53e9714d4ef924c1e57d1f822c16b399e9de1bae04e27f42ed80f71bb41317a06c708fd3625b7a51548e4f791961e6eb90565a98b76e854bf3b6c1db8678e4e2087a9f858b822b0ec2e502491a9a869104a0a586807cbbf01363"
                      "2c"));
    }
}
