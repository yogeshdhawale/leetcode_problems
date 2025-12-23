
#ifndef __LOG_H
#define __LOG_H

#include <iostream>
#include <cassert>
#include <algorithm>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <memory>

#include <numeric>
#include <chrono>
#include <string_view>
#include <ranges>


/*** DEBUG LOG ***/
#define __DEBUG_LOG_OFF         0
#define __DEBUG_LOG_ON          1
#define __DEBUG_LOG_VERBOSE     2

#define __DBG_PRNT_ITEMS        20

using std::ostream;
using std::string;
using std::vector;
using std::map;
using std::pair;

ostream& operator<<(ostream& out, string s) {
    out << s.c_str();
    return out;
}

template <typename T, typename U>
ostream& operator<<(ostream& out, const pair<T, U>& nums)
{
    out << "{" << nums.first << ":";
    if (!std::is_same<U, bool>::value)
        out << nums.second;
    out << "}";
    return out;
}

template <typename T>
ostream& operator<<(ostream& out, const vector<T>& nums)
{
    size_t size = nums.size();
    if (size > __DBG_PRNT_ITEMS) {
        out << "\"V_" << size << "\"";
        size = __DBG_PRNT_ITEMS;
    }

    out << "[";
    for (size_t i = 0; i < size; i++) out << nums[i] << ",";

    if (nums.size() > __DBG_PRNT_ITEMS) out << "\"....\"";
    out << "]";
    return out;
}



template<typename Adaptor>
    requires (std::ranges::input_range<typename Adaptor::container_type>)
std::ostream& operator <<(std::ostream& out, const Adaptor& adaptor)
{
    struct Printer : Adaptor // to access protected Adaptor::Container c;
    {
        void print(ostream& out) const
        {
            out << this->c;
        }
    };

    static_cast<Printer const&>(adaptor).print(out);

    return out;
}

template <template <class, class> class Container, class T, class Alloc = std::allocator <T> >
std::ostream& operator << (std::ostream& out, const Container <T, Alloc>& container) {

    if (container.size() > __DBG_PRNT_ITEMS) out << "\"C_" << container.size() << "\" : [";
    else out << " C_:<";
    for (int i = 0; auto data : container)
    {
        out << data << ",";
        i++;
        if (i > __DBG_PRNT_ITEMS) break;
    }

    if (container.size() > __DBG_PRNT_ITEMS) {
        out << "\"....\"";
    }
    out << ">";
    return out;
}


#define __LOGPREFIX "\t---\t"
//#define LOGDATE __TIMESTAMP__
#define LOGDATE __TIME__

#define LOG_MSG(msg)    clog << "[" << LOGDATE << "] " << msg << endl

#ifdef __DEBUG_LOG
#define LOG(x)  if (__DEBUG_LOG != __DEBUG_LOG_OFF)     LOG_MSG(x)
#define LOGV(x) if (__DEBUG_LOG == __DEBUG_LOG_VERBOSE) LOG_MSG(x)
#else
#define LOG(x)
#define LOGV(x)
#endif

#endif