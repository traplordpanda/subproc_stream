#include <iostream>
#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string_view.h>
#include <string_view>

#include "subproc_stream.hpp"

namespace nb = nanobind;

NB_MODULE(subproc_stream_ext, m) {
    using namespace subproc_stream;
    nb::class_<SubProc>(m, "SubProc")
        .def(nb::init<const std::string_view>(), nb::rv_policy::take_ownership)
        .def("get_return_code", &SubProc::get_return_code)
        .def("exec", &SubProc::exec)
        .def("print_test",
             [] { std::cout << "Hello from subproc_stream_ext.cpp\n"; });
}