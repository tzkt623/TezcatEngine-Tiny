#pragma once


#include <string>
#include <vector>
#include <stack>
#include <cstdio>
#include <list>
#include <functional>
#include <unordered_map>
#include <unordered_set>
#include <initializer_list>
#include <regex>
#include <typeindex>
#include <memory>
#include <iostream>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <chrono>
#include <random>
#include <array>
#include <string_view>
#include <tuple>


template<typename T>
using ref = std::shared_ptr<T>;

template<typename T>
using uref = std::unique_ptr<T>;