/*
 * Copyright 2018 Google
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

#ifndef FIRESTORE_CORE_SRC_FIREBASE_FIRESTORE_MODEL_FIELD_MASK_H_
#define FIRESTORE_CORE_SRC_FIREBASE_FIRESTORE_MODEL_FIELD_MASK_H_

#include <initializer_list>
#include <string>
#include <utility>
#include <vector>

#include "Firestore/core/src/firebase/firestore/model/field_path.h"

namespace firebase {
namespace firestore {
namespace model {

/**
 * Provides a set of fields that can be used to partially patch a document.
 * FieldMask is used in conjunction with FieldValue of Object type.
 *
 * Examples:
 *   foo - Overwrites foo entirely with the provided value. If foo is not
 *       present in the companion FieldValue, the field is deleted.
 *   foo.bar - Overwrites only the field bar of the object foo. If foo is not an
 *       object, foo is replaced with an object containing bar.
 */
class FieldMask {
 public:
  using const_iterator = std::vector<FieldPath>::const_iterator;

  FieldMask(std::initializer_list<FieldPath> list) : fields_{list} {
  }
  explicit FieldMask(const std::vector<FieldPath>& fields) : fields_{fields} {
  }
  explicit FieldMask(std::vector<FieldPath>&& fields)
      : fields_{std::move(fields)} {
  }

  const_iterator begin() const {
    return fields_.begin();
  }
  const_iterator end() const {
    return fields_.end();
  }

  std::string ToString() const {
    // Ideally, one should use a string builder. Since this is only non-critical
    // code for logging and debugging, the logic is kept simple here.
    std::string result("{ ");
    for (const FieldPath& field : fields_) {
      result += field.CanonicalString() + " ";
    }
    return result + "}";
  }

#if defined(__OBJC__)
  FieldMask() {
  }

  NSUInteger Hash() const {
    NSUInteger hashResult = 0;
    for (const FieldPath& field : fields_) {
      hashResult = hashResult * 31u + field.Hash();
    }
    return hashResult;
  }
#endif

  friend bool operator==(const FieldMask& lhs, const FieldMask& rhs);

 private:
  std::vector<FieldPath> fields_;
};

inline bool operator==(const FieldMask& lhs, const FieldMask& rhs) {
  return lhs.fields_== rhs.fields_;
}

}  // namespace model
}  // namespace firestore
}  // namespace firebase

#endif  // FIRESTORE_CORE_SRC_FIREBASE_FIRESTORE_MODEL_FIELD_MASK_H_
