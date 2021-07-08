/*
* The MIT License (MIT)
*
* Copyright (c) Microsoft Corporation
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
* OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
*/

/**
 Test type inferencing.

 Many tests use a style of test ported over from an old test runner. Any new
 set of tests should write their own testing code that would be easier to work
 with.
*/
#include <catch2/catch.hpp>
#include <memory>
#include <codemodel.h>
#include <jitinfo.h>
#include <ilgen.h>

extern BaseModule g_module;
extern ICorJitCompiler* g_jit;

typedef int32_t (*Returns_int32)();
typedef uint32_t (*Returns_uint32)();
typedef int64_t (*Returns_int64)();
typedef double (*Returns_double)();


TEST_CASE("Test numerics") {
    SECTION("test ld_i4 emitter") {
        int32_t value = GENERATE(1, -1, 0, 100, 127, -127, 128, -128, 129, -129, -100, 1000, 202, -102, 65555, 2147483647, -2147483647);
        auto test_module = new UserModule(g_module);
        auto gen = new ILGenerator(
                test_module,
                 CORINFO_TYPE_INT,
                 std::vector < Parameter > {
                 });
        gen->ld_i4(value);
        gen->ret();
        auto* jitInfo = new CorJitInfo("test_module", "test_32_int", test_module, true);
        JITMethod method = gen->compile(jitInfo, g_jit, 100);
        REQUIRE(method.m_addr != nullptr);
        int32_t result = ((Returns_int32)method.getAddr())();
        CHECK(result == value);
    }

    SECTION("test ld_u4 emitter") {
        int32_t value = GENERATE(1, 0, 100, 1000, 202, 65555, 4294967295);
        auto test_module = new UserModule(g_module);
        auto gen = new ILGenerator(
                test_module,
                CORINFO_TYPE_INT,
                std::vector < Parameter > {
                });
        gen->ld_u4(value);
        gen->ret();
        auto* jitInfo = new CorJitInfo("test_module", "test_32_int", test_module, true);
        JITMethod method = gen->compile(jitInfo, g_jit, 100);
        REQUIRE(method.m_addr != nullptr);
        uint32_t result = ((Returns_uint32)method.getAddr())();
        CHECK(result == value);
    }

    SECTION("test ld_i8 emitter") {
        int64_t value = GENERATE(1, 0, 100, 1000, 202, 65555, 4294967295, 9223372036854775807);
        auto test_module = new UserModule(g_module);
        auto gen = new ILGenerator(
                test_module,
                CORINFO_TYPE_LONG,
                std::vector < Parameter > {
                });
        gen->ld_i8(value);
        gen->ret();
        auto* jitInfo = new CorJitInfo("test_module", "test_32_int", test_module, true);
        JITMethod method = gen->compile(jitInfo, g_jit, 100);
        REQUIRE(method.m_addr != nullptr);
        int64_t result = ((Returns_int64)method.getAddr())();
        CHECK(result == value);
    }

    SECTION("test ld_r8 emitter") {
        double value = GENERATE(1., 0., 100., 1000., 202., 65555., 4294967295., .2222);
        auto test_module = new UserModule(g_module);
        auto gen = new ILGenerator(
                test_module,
                CORINFO_TYPE_DOUBLE,
                std::vector < Parameter > {
                });
        gen->ld_r8(value);
        gen->ret();
        auto* jitInfo = new CorJitInfo("test_module", "test_32_int", test_module, true);
        JITMethod method = gen->compile(jitInfo, g_jit, 100);
        REQUIRE(method.m_addr != nullptr);
        double result = ((Returns_double)method.getAddr())();
        CHECK(result == value);
    }
}