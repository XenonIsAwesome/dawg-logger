//
// Created by user on 11/4/25.
//

#pragma once

#include <concepts>
#include <exception>
#include <type_traits>

namespace DawgLog {

template<typename E>
concept ExceptionType = std::is_base_of_v<std::exception, E>;

}
