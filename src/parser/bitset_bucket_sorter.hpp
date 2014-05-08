#ifndef BITSET_BUCKET_SORTER
#define BITSET_BUCKET_SORTER

#define BOOST_MPL_CFG_NO_PREPROCESSED_HEADERS
#define BOOST_MPL_LIMIT_VECTOR_SIZE 30
#include <boost/mpl/vector_c.hpp>
#include <boost/mpl/vector.hpp>
#include <boost/mpl/accumulate.hpp>
#include <boost/mpl/long.hpp>
#include <boost/mpl/if.hpp>
#include <boost/mpl/equal_to.hpp>
#include <boost/mpl/shift_left.hpp>
#include <boost/mpl/shift_right.hpp>
#include <boost/mpl/bitor.hpp>
#include <boost/mpl/modulus.hpp>
#include <boost/mpl/less.hpp>
#include <boost/mpl/times.hpp>
#include <boost/mpl/erase.hpp>
#include <boost/mpl/insert.hpp>
#include <boost/mpl/advance.hpp>
#include <boost/mpl/begin.hpp>
#include <boost/mpl/at.hpp>

template<typename seq, typename pos, typename item>
struct replace_pos {


    typedef typename boost::mpl::erase<seq, typename boost::mpl::advance<typename boost::mpl::begin<seq>::type, pos>::type>::type temp;
    typedef typename boost::mpl::insert<temp, typename boost::mpl::advance<typename boost::mpl::begin<temp>::type, pos>::type, item >::type type;

};

// mutli-pass bucket sort
template<typename token_set, int bucket_number, int limit>
struct bitset_bucket {

    typedef typename boost::mpl::if_<boost::mpl::less<boost::mpl::times<boost::mpl::long_<bucket_number>, boost::mpl::int_<32> >, boost::mpl::long_<limit> >,
            typename boost::mpl::accumulate<token_set, boost::mpl::long_<0>,
                boost::mpl::if_<
                    boost::mpl::equal_to<boost::mpl::shift_right<boost::mpl::_2, boost::mpl::long_<5> >, boost::mpl::long_<bucket_number> >,
                    boost::mpl::bitor_<boost::mpl::_1, boost::mpl::shift_left<boost::mpl::long_<1>, boost::mpl::modulus<boost::mpl::_2, boost::mpl::long_<32> > > >,
                    boost::mpl::_1 >
                >::type,
            boost::mpl::long_<0> >::type type;

};

/*
template<typename token_set>
struct bitset_buckets {

    static const int num_token_longs = 24;
    static const unsigned long data[num_token_longs];

};


template<typename token_set>
const unsigned long bitset_buckets<token_set>::data[bitset_buckets<token_set>::num_token_longs] = 
        { bitset_bucket<token_set, 0>::type::value, bitset_bucket<token_set, 1>::type::value, bitset_bucket<token_set, 2>::type::value, bitset_bucket<token_set, 3>::type::value
        , bitset_bucket<token_set, 4>::type::value, bitset_bucket<token_set, 5>::type::value, bitset_bucket<token_set, 6>::type::value, bitset_bucket<token_set, 7>::type::value
        , bitset_bucket<token_set, 8>::type::value, bitset_bucket<token_set, 9>::type::value, bitset_bucket<token_set, 10>::type::value, bitset_bucket<token_set, 11>::type::value
        , bitset_bucket<token_set, 12>::type::value, bitset_bucket<token_set, 13>::type::value, bitset_bucket<token_set, 14>::type::value, bitset_bucket<token_set, 15>::type::value
        , bitset_bucket<token_set, 16>::type::value, bitset_bucket<token_set, 17>::type::value, bitset_bucket<token_set, 18>::type::value, bitset_bucket<token_set, 19>::type::value
        , bitset_bucket<token_set, 20>::type::value, bitset_bucket<token_set, 21>::type::value, bitset_bucket<token_set, 22>::type::value, bitset_bucket<token_set, 23>::type::value
};
*/

template<typename token_set>
struct bitset_bucket_sorter {

    typedef typename boost::mpl::accumulate<token_set, boost::mpl::vector_c<unsigned long,
                                            0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0,
                                            0, 0, 0, 0, 0, 0, 0, 0>,

        replace_pos<boost::mpl::_1, boost::mpl::shift_right<boost::mpl::_2, boost::mpl::long_<5> >,
                boost::mpl::bitor_<
                boost::mpl::at<boost::mpl::_1, boost::mpl::shift_right<boost::mpl::_2, boost::mpl::long_<5> > >,
                boost::mpl::shift_left<boost::mpl::long_<1>, boost::mpl::modulus<boost::mpl::_2, boost::mpl::long_<32> > > 
            >
        >
    >::type type;

};


template<typename token_set>
struct bitset_buckets {

    typedef typename bitset_bucket_sorter<token_set>::type buckets;

    static const int num_token_longs = 24;
    static const unsigned long data[num_token_longs];

};

template<typename token_set>
const unsigned long bitset_buckets<token_set>::data[bitset_buckets<token_set>::num_token_longs] = 
{ 
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<0> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<1> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<2> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<3> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<4> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<5> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<6> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<7> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<8> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<9> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<10> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<11> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<12> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<13> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<14> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<15> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<16> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<17> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<18> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<19> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<20> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<21> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<22> >::type::type::value,
    boost::mpl::at<bitset_buckets<token_set>::buckets, boost::mpl::long_<23> >::type::type::value

};

#endif