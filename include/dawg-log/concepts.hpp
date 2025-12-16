//
// Created by user on 11/4/25.
//

#pragma once

#include <exception>
#include <concepts>

namespace DawgLog {

template <typename E>
concept ExceptionType = std::is_base_of_v<std::exception, E>;

}
