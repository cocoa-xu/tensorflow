/* Copyright 2024 The OpenXLA Authors.

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

#ifndef XLA_SERVICE_CPU_RUNTIME_LOGICAL_ID_THUNK_H_
#define XLA_SERVICE_CPU_RUNTIME_LOGICAL_ID_THUNK_H_

#include <cstdint>
#include <memory>

#include "absl/status/statusor.h"
#include "xla/service/buffer_assignment.h"
#include "xla/service/computation_placer.h"
#include "xla/service/cpu/runtime/thunk.h"
#include "xla/service/global_device_id.h"
#include "xla/tsl/concurrency/async_value_ref.h"

namespace xla::cpu {

enum class LogicalIdKind {
  kPartitionId,
  kReplicaId,
};

template <LogicalIdKind type>
class LogicalIdThunk : public Thunk {
 public:
  static absl::StatusOr<std::unique_ptr<LogicalIdThunk>> Create(
      Info info, BufferAllocation::Slice logical_id_buffer);

  tsl::AsyncValueRef<ExecuteEvent> Execute(const ExecuteParams& params) final;

  BufferUses buffer_uses() const final;

 private:
  LogicalIdThunk(Info info, BufferAllocation::Slice logical_id_buffer);

  absl::StatusOr<int32_t> GetIdForDevice(
      const DeviceAssignment* device_assignment,
      GlobalDeviceId device_id) const;

  BufferAllocation::Slice logical_id_buffer_;
};

class ReplicaIdThunk final : public LogicalIdThunk<LogicalIdKind::kReplicaId> {
};

class PartitionIdThunk final
    : public LogicalIdThunk<LogicalIdKind::kPartitionId> {};

}  // namespace xla::cpu

#endif  // XLA_SERVICE_CPU_RUNTIME_LOGICAL_ID_THUNK_H_
