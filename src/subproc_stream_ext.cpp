#include <iostream>
#include <nanobind/nanobind.h>
#include <nanobind/stl/filesystem.h>
#include <nanobind/stl/string_view.h>
#include <string_view>

#include "subproc_stream.hpp"

namespace nb = nanobind;

NB_MODULE(subproc_stream_ext, m) {
    using namespace subproc_stream;
    m.doc() = "A simple unidirectional wrapper class around pipes with optional logging"; 
    nb::class_<SubProc<false>>(m, "SubProc")
        .def(nb::init<const std::string_view>(), nb::rv_policy::take_ownership)
        .def("exec", &SubProc<false>::exec)
        .def("print_test",
             [] { std::cout << "Hello from subproc_stream_ext.cpp\n"; });
    nb::class_<SubProc<true>>(m, "SubProcLog")
        .def(nb::init<std::string_view, std::string_view>(), nb::rv_policy::take_ownership)
        .def("exec", &SubProc<true>::exec);
}