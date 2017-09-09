/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "NeuralNetworksWrapper.h"

#include <gtest/gtest.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>

using namespace android::nn::wrapper;

namespace {

typedef float Matrix3x4[3][4];

// Tests the various ways to pass weights and input/output data.
class MemoryTest : public ::testing::Test {
protected:
    virtual void SetUp() { ASSERT_EQ(Initialize(), Result::NO_ERROR); }
    virtual void TearDown() { Shutdown(); }

    const Matrix3x4 matrix1 = {{1.f, 2.f, 3.f, 4.f}, {5.f, 6.f, 7.f, 8.f}, {9.f, 10.f, 11.f, 12.f}};
    const Matrix3x4 matrix2 = {{100.f, 200.f, 300.f, 400.f},
                               {500.f, 600.f, 700.f, 800.f},
                               {900.f, 1000.f, 1100.f, 1200.f}};
    const Matrix3x4 matrix3 = {{20.f, 30.f, 40.f, 50.f},
                               {21.f, 22.f, 23.f, 24.f},
                               {31.f, 32.f, 33.f, 34.f}};
    const Matrix3x4 expected3 = {{121.f, 232.f, 343.f, 454.f},
                                 {526.f, 628.f, 730.f, 832.f},
                                 {940.f, 1042.f, 1144.f, 1246.f}};
    const Matrix3x4 expected3b = {{22.f, 34.f, 46.f, 58.f},
                                  {31.f, 34.f, 37.f, 40.f},
                                  {49.f, 52.f, 55.f, 58.f}};
};

// Check that the values are the same. This works only if dealing with integer
// value, otherwise we should accept values that are similar if not exact.
int CompareMatrices(const Matrix3x4& expected, const Matrix3x4& actual) {
    int errors = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 4; j++) {
            if (expected[i][j] != actual[i][j]) {
                printf("expected[%d][%d] != actual[%d][%d], %f != %f\n", i, j, i, j,
                       static_cast<double>(expected[i][j]), static_cast<double>(actual[i][j]));
                errors++;
            }
        }
    }
    return errors;
}

TEST_F(MemoryTest, TestAllocatedMemory) {
    // Layout where to place matrix2 and matrix3 in the memory we'll allocate.
    // We have gaps to test that we don't assume contiguity.
    constexpr uint32_t offsetForMatrix2 = 20;
    constexpr uint32_t offsetForMatrix3 = offsetForMatrix2 + sizeof(matrix2) + 30;
    constexpr uint32_t memorySize = offsetForMatrix3 + sizeof(matrix3) + 60;

    Memory weights(memorySize);
    ASSERT_TRUE(weights.isValid());
    uint8_t* data = nullptr;
    ASSERT_EQ(weights.getPointer(&data), Result::NO_ERROR);
    ASSERT_NE(data, nullptr);
    memcpy(data + offsetForMatrix2, matrix2, sizeof(matrix2));
    memcpy(data + offsetForMatrix3, matrix3, sizeof(matrix3));

    Model model;
    OperandType matrixType(Type::TENSOR_FLOAT32, {3, 4});
    OperandType scalarType(Type::INT32, {});
    int32_t activation(0);
    auto a = model.addOperand(&matrixType);
    auto b = model.addOperand(&matrixType);
    auto c = model.addOperand(&matrixType);
    auto d = model.addOperand(&matrixType);
    auto e = model.addOperand(&matrixType);
    auto f = model.addOperand(&scalarType);

    model.setOperandValueFromMemory(e, &weights, offsetForMatrix2, sizeof(Matrix3x4));
    model.setOperandValueFromMemory(a, &weights, offsetForMatrix3, sizeof(Matrix3x4));
    model.setOperandValue(f, &activation, sizeof(activation));
    model.addOperation(ANEURALNETWORKS_ADD, {a, c, f}, {b});
    model.addOperation(ANEURALNETWORKS_ADD, {b, e, f}, {d});
    model.setInputsAndOutputs({c}, {d});
    ASSERT_TRUE(model.isValid());

    // Test the two node model.
    constexpr uint32_t offsetForMatrix1 = 20;
    Memory input(offsetForMatrix1 + sizeof(Matrix3x4));
    ASSERT_TRUE(input.isValid());
    ASSERT_EQ(input.getPointer(&data), Result::NO_ERROR);
    memcpy(data + offsetForMatrix1, matrix1, sizeof(Matrix3x4));

    constexpr uint32_t offsetForActual = 32;
    Memory actual(offsetForActual + sizeof(Matrix3x4));
    ASSERT_TRUE(actual.isValid());
    ASSERT_EQ(actual.getPointer(&data), Result::NO_ERROR);
    memset(data, 0, offsetForActual + sizeof(Matrix3x4));

    Request request2(&model);
    ASSERT_EQ(request2.setInputFromMemory(0, &input, offsetForMatrix1, sizeof(Matrix3x4)),
              Result::NO_ERROR);
    ASSERT_EQ(request2.setOutputFromMemory(0, &actual, offsetForActual, sizeof(Matrix3x4)),
              Result::NO_ERROR);
    ASSERT_EQ(request2.compute(), Result::NO_ERROR);
    data = nullptr;
    ASSERT_EQ(actual.getPointer(&data), Result::NO_ERROR);
    ASSERT_EQ(CompareMatrices(expected3, *reinterpret_cast<Matrix3x4*>(data + offsetForActual)), 0);
}

TEST_F(MemoryTest, TestFd) {
    // Create a file that contains matrix2 and matrix3.
    char path[] = "/data/local/tmp/TestMemoryXXXXXX";
    int fd = mkstemp(path);
    const uint32_t offsetForMatrix2 = 20;
    const uint32_t offsetForMatrix3 = 200;
    static_assert(offsetForMatrix2 + sizeof(matrix2) < offsetForMatrix3, "matrices overlap");
    lseek(fd, offsetForMatrix2, SEEK_SET);
    write(fd, matrix2, sizeof(matrix2));
    lseek(fd, offsetForMatrix3, SEEK_SET);
    write(fd, matrix3, sizeof(matrix3));
    fsync(fd);

    Memory weights(offsetForMatrix3 + sizeof(matrix3), PROT_READ, fd);
    ASSERT_TRUE(weights.isValid());

    Model model;
    OperandType matrixType(Type::TENSOR_FLOAT32, {3, 4});
    OperandType scalarType(Type::INT32, {});
    int32_t activation(0);
    auto a = model.addOperand(&matrixType);
    auto b = model.addOperand(&matrixType);
    auto c = model.addOperand(&matrixType);
    auto d = model.addOperand(&matrixType);
    auto e = model.addOperand(&matrixType);
    auto f = model.addOperand(&scalarType);

    model.setOperandValueFromMemory(e, &weights, offsetForMatrix2, sizeof(Matrix3x4));
    model.setOperandValueFromMemory(a, &weights, offsetForMatrix3, sizeof(Matrix3x4));
    model.setOperandValue(f, &activation, sizeof(activation));
    model.addOperation(ANEURALNETWORKS_ADD, {a, c, f}, {b});
    model.addOperation(ANEURALNETWORKS_ADD, {b, e, f}, {d});
    model.setInputsAndOutputs({c}, {d});
    ASSERT_TRUE(model.isValid());

    // Test the three node model.
    Matrix3x4 actual;
    memset(&actual, 0, sizeof(actual));
    Request request2(&model);
    ASSERT_EQ(request2.setInput(0, matrix1, sizeof(Matrix3x4)), Result::NO_ERROR);
    ASSERT_EQ(request2.setOutput(0, actual, sizeof(Matrix3x4)), Result::NO_ERROR);
    ASSERT_EQ(request2.compute(), Result::NO_ERROR);
    ASSERT_EQ(CompareMatrices(expected3, actual), 0);

    close(fd);
    unlink(path);
}

}  // end namespace