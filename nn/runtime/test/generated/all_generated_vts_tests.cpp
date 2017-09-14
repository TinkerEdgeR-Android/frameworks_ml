// DO NOT EDIT;
// Generated by ml/nn/runtime/test/specs/generate_vts_test.sh

namespace add {
std::vector<MixedTypedExample> examples = {
// Generated add test
#include "examples/add.example.cpp"
};
// Generated model constructor
#include "vts_models/add.model.cpp"
} // namespace add
TEST_F(NeuralnetworksHidlTest, add) {
    generated_tests::Execute(device, add::createTestModel, add::examples);
}

namespace avg_pool_float {
std::vector<MixedTypedExample> examples = {
// Generated avg_pool_float test
#include "examples/avg_pool_float.example.cpp"
};
// Generated model constructor
#include "vts_models/avg_pool_float.model.cpp"
} // namespace avg_pool_float
TEST_F(NeuralnetworksHidlTest, avg_pool_float) {
    generated_tests::Execute(device, avg_pool_float::createTestModel, avg_pool_float::examples);
}

namespace avg_pool_quant8 {
std::vector<MixedTypedExample> examples = {
// Generated avg_pool_quant8 test
#include "examples/avg_pool_quant8.example.cpp"
};
// Generated model constructor
#include "vts_models/avg_pool_quant8.model.cpp"
} // namespace avg_pool_quant8
TEST_F(NeuralnetworksHidlTest, avg_pool_quant8) {
    generated_tests::Execute(device, avg_pool_quant8::createTestModel, avg_pool_quant8::examples);
}

namespace concat_float {
std::vector<MixedTypedExample> examples = {
// Generated concat_float test
#include "examples/concat_float.example.cpp"
};
// Generated model constructor
#include "vts_models/concat_float.model.cpp"
} // namespace concat_float
TEST_F(NeuralnetworksHidlTest, concat_float) {
    generated_tests::Execute(device, concat_float::createTestModel, concat_float::examples);
}

namespace hashtable_lookup {
std::vector<MixedTypedExample> examples = {
// Generated hashtable_lookup test
#include "examples/hashtable_lookup.example.cpp"
};
// Generated model constructor
#include "vts_models/hashtable_lookup.model.cpp"
} // namespace hashtable_lookup
TEST_F(NeuralnetworksHidlTest, hashtable_lookup) {
    generated_tests::Execute(device, hashtable_lookup::createTestModel, hashtable_lookup::examples);
}

namespace l2_pool_float {
std::vector<MixedTypedExample> examples = {
// Generated l2_pool_float test
#include "examples/l2_pool_float.example.cpp"
};
// Generated model constructor
#include "vts_models/l2_pool_float.model.cpp"
} // namespace l2_pool_float
TEST_F(NeuralnetworksHidlTest, l2_pool_float) {
    generated_tests::Execute(device, l2_pool_float::createTestModel, l2_pool_float::examples);
}

namespace max_pool_float {
std::vector<MixedTypedExample> examples = {
// Generated max_pool_float test
#include "examples/max_pool_float.example.cpp"
};
// Generated model constructor
#include "vts_models/max_pool_float.model.cpp"
} // namespace max_pool_float
TEST_F(NeuralnetworksHidlTest, max_pool_float) {
    generated_tests::Execute(device, max_pool_float::createTestModel, max_pool_float::examples);
}

namespace max_pool_quant8 {
std::vector<MixedTypedExample> examples = {
// Generated max_pool_quant8 test
#include "examples/max_pool_quant8.example.cpp"
};
// Generated model constructor
#include "vts_models/max_pool_quant8.model.cpp"
} // namespace max_pool_quant8
TEST_F(NeuralnetworksHidlTest, max_pool_quant8) {
    generated_tests::Execute(device, max_pool_quant8::createTestModel, max_pool_quant8::examples);
}

namespace relu1_float {
std::vector<MixedTypedExample> examples = {
// Generated relu1_float test
#include "examples/relu1_float.example.cpp"
};
// Generated model constructor
#include "vts_models/relu1_float.model.cpp"
} // namespace relu1_float
TEST_F(NeuralnetworksHidlTest, relu1_float) {
    generated_tests::Execute(device, relu1_float::createTestModel, relu1_float::examples);
}

namespace relu6_float {
std::vector<MixedTypedExample> examples = {
// Generated relu6_float test
#include "examples/relu6_float.example.cpp"
};
// Generated model constructor
#include "vts_models/relu6_float.model.cpp"
} // namespace relu6_float
TEST_F(NeuralnetworksHidlTest, relu6_float) {
    generated_tests::Execute(device, relu6_float::createTestModel, relu6_float::examples);
}

namespace relu_float {
std::vector<MixedTypedExample> examples = {
// Generated relu_float test
#include "examples/relu_float.example.cpp"
};
// Generated model constructor
#include "vts_models/relu_float.model.cpp"
} // namespace relu_float
TEST_F(NeuralnetworksHidlTest, relu_float) {
    generated_tests::Execute(device, relu_float::createTestModel, relu_float::examples);
}