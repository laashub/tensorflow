/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef TENSORFLOW_CORE_KERNELS_DATA_EXPERIMENTAL_DATA_SERVICE_OPS_H_
#define TENSORFLOW_CORE_KERNELS_DATA_EXPERIMENTAL_DATA_SERVICE_OPS_H_

#include "tensorflow/core/framework/dataset.h"
#include "tensorflow/core/framework/op_kernel.h"
#include "tensorflow/core/kernels/data/iterator_ops.h"

namespace tensorflow {
namespace data {

// Registers a dataset with the tf.data service.
//
// The address and protocol inputs are used to connect to the tf.data master.
// The external state policy attribute determines whether to ignore, warn, or
// error out when the dataset contains external state.
// The op produces a dataset id for identifying the registered dataset.
class RegisterDatasetOp : public OpKernel {
 public:
  static constexpr const char* const kAddress = "address";
  static constexpr const char* const kProtocol = "protocol";
  static constexpr const char* const kExternalStatePolicy =
      "external_state_policy";

  explicit RegisterDatasetOp(OpKernelConstruction* ctx);

  void Compute(OpKernelContext* ctx) override;

 private:
  SerializationContext::ExternalStatePolicy external_state_policy_;
};

// Begins a new epoch for a tf.data service dataset.
//
// The dataset_id input identifies which dataset to start a new epoch for.
// The address and protocol inputs are used to connect to the tf.data service
// master.
// The op produces an epoch id to identify the newly created epoch.
class BeginEpochOp : public OpKernel {
 public:
  static constexpr const char* const kDatasetId = "dataset_id";
  static constexpr const char* const kAddress = "address";
  static constexpr const char* const kProtocol = "protocol";

  explicit BeginEpochOp(OpKernelConstruction* ctx);

  void Compute(OpKernelContext* ctx) override;
};

// Creates a new iterator for iterating over a tf.data service dataset.
//
// The epoch_id input identifies which epoch to read from. Multiple iterators
// may read from the same epoch, causing the elements of the epoch to be split
// across all iterators.
class MakeDataServiceIteratorOp : public MakeIteratorOp {
 public:
  static constexpr const char* const kEpochId = "epoch_id";

  explicit MakeDataServiceIteratorOp(OpKernelConstruction* ctx)
      : MakeIteratorOp(ctx) {}

 protected:
  Status DoCompute(OpKernelContext* ctx) override;
};

}  // namespace data
}  // namespace tensorflow
#endif  // TENSORFLOW_CORE_KERNELS_DATA_EXPERIMENTAL_DATA_SERVICE_OPS_H_
