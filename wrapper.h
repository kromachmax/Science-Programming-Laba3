#ifndef WRAPPER_H
#define WRAPPER_H

#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <functional>
#include <algorithm>
#include <vector>

template<typename ...Args>
class FunctorImpl;

template <typename T, typename R, typename ...Args>
class FunctorImpl<R (T::*)(Args ...)>{
public:

    typedef R(T::*FT)(Args ...);
    typedef T HostType;

    FunctorImpl(FT fn, T* obj, const std::map<std::string, int>& args_map)
        : m_fn(fn), m_obj(obj) {
        for(const auto& arg : args_map){
            arg_names.push_back(arg.first);
        }
    };

    R operator() (Args... args)
    {
        return (m_obj->*m_fn)(args ...);
    }

    R operator() (T* obj, Args... args)
    {
        return (obj->*m_fn)(args ...);
    }


    int operator () (std::unordered_map<std::string, int> args_map) {
        for (const auto& arg_name : arg_names) {
            if (args_map.find(arg_name) == args_map.end()) {
                throw std::runtime_error("Could not find argument " + arg_name);
            }

            arg_values.push_back(args_map[arg_name]);
        }

        return unpack_vector(arg_values);
    }

private:

    FT m_fn;
    T* m_obj;
    std::vector <std::string> arg_names;
    std::vector<int> arg_values;

private:

    template<std::size_t... S>
    int unpack_vector(const std::vector<int>& vec, std::index_sequence<S...>) {
        return operator()(vec[S]...);
    }

    int unpack_vector(const std::vector<int>& vec) {
        return unpack_vector(vec, std::make_index_sequence<sizeof...(Args)>());
    }
};

template<typename FT>
struct Wrapper : public FunctorImpl<FT>
{
    typedef typename FunctorImpl<FT>::HostType HostType;
    Wrapper(HostType* obj, FT fn, const std::map<std::string, int>& mp) : FunctorImpl<FT>(fn, obj, mp) {};
};

#endif // WRAPPER_H
