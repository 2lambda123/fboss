// (c) Facebook, Inc. and its affiliates. Confidential and proprietary.

#pragma once

#include <atomic>

#include <fboss/fsdb/oper/DeltaValue.h>
#include <fboss/thrift_cow/storage/Storage.h>
#include <folly/Expected.h>
#include <folly/dynamic.h>
#include <folly/experimental/coro/AsyncGenerator.h>
#include <folly/experimental/coro/Sleep.h>
#include <folly/logging/xlog.h>
#include <chrono>
#include "fboss/fsdb/if/gen-cpp2/fsdb_common_types.h"
#include "fboss/fsdb/if/gen-cpp2/fsdb_oper_types.h"

namespace facebook::fboss::fsdb {

/*
 * This object
 */
template <typename Root, typename Impl>
class SubscribableStorage {
 public:
  // TODO: more flexibility here for all forward iterator types
  using RootT = Root;
  using ConcretePath = std::vector<std::string>;
  using PathIter = std::vector<std::string>::const_iterator;
  using ExtPathIter = std::vector<OperPathElem>::const_iterator;

  template <typename T>
  using Result = folly::Expected<T, StorageError>;

  virtual ~SubscribableStorage() {}

  /*
   * New apis, start/stop + subscribe.
   */

  void start() {
    static_cast<Impl*>(this)->start_impl();
  }
  void stop() {
    static_cast<Impl*>(this)->stop_impl();
  }

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, RootT>, void>>
  folly::coro::AsyncGenerator<DeltaValue<typename Path::DataT>&&> subscribe(
      SubscriberId subscriber,
      Path&& path) {
    return this->template subscribe<typename Path::DataT, typename Path::TC>(
        subscriber, path.begin(), path.end());
  }
  template <typename T, typename TC>
  folly::coro::AsyncGenerator<DeltaValue<T>&&> subscribe(
      SubscriberId subscriber,
      const ConcretePath& path) {
    return this->template subscribe<T, TC>(
        subscriber, path.begin(), path.end());
  }
  template <typename T, typename TC>
  folly::coro::AsyncGenerator<DeltaValue<T>&&>
  subscribe(SubscriberId subscriber, PathIter begin, PathIter end) {
    return static_cast<Impl*>(this)->template subscribe_impl<T, TC>(
        subscriber, begin, end);
  }

  template <
      typename Path,
      typename = std::enable_if_t<
          std::is_same_v<typename folly::remove_cvref_t<Path>::RootT, RootT>,
          void>>
  folly::coro::AsyncGenerator<DeltaValue<OperState>&&> subscribe_encoded(
      SubscriberId subscriber,
      Path&& path,
      OperProtocol protocol) {
    return this->subscribe_encoded(
        subscriber, path.begin(), path.end(), protocol);
  }
  folly::coro::AsyncGenerator<DeltaValue<OperState>&&> subscribe_encoded(
      SubscriberId subscriber,
      const ConcretePath& path,
      OperProtocol protocol) {
    return this->subscribe_encoded(
        subscriber, path.begin(), path.end(), protocol);
  }
  folly::coro::AsyncGenerator<DeltaValue<OperState>&&> subscribe_encoded(
      SubscriberId subscriber,
      PathIter begin,
      PathIter end,
      OperProtocol protocol) {
    return static_cast<Impl*>(this)->subscribe_encoded_impl(
        subscriber, begin, end, protocol);
  }

  folly::coro::AsyncGenerator<std::vector<DeltaValue<TaggedOperState>>&&>
  subscribe_encoded_extended(
      SubscriberId subscriber,
      std::vector<ExtendedOperPath> paths,
      OperProtocol protocol) {
    return static_cast<Impl*>(this)->subscribe_encoded_extended_impl(
        subscriber, std::move(paths), protocol);
  }

  template <
      typename Path,
      typename = std::enable_if_t<
          std::is_same_v<typename folly::remove_cvref_t<Path>::RootT, RootT>,
          void>>
  folly::coro::AsyncGenerator<OperDelta&&>
  subscribe_delta(SubscriberId subscriber, Path&& path, OperProtocol protocol) {
    return this->subscribe_delta(
        subscriber, path.begin(), path.end(), protocol);
  }
  folly::coro::AsyncGenerator<OperDelta&&> subscribe_delta(
      SubscriberId subscriber,
      const ConcretePath& path,
      OperProtocol protocol) {
    return this->subscribe_delta(
        subscriber, path.begin(), path.end(), protocol);
  }
  folly::coro::AsyncGenerator<OperDelta&&> subscribe_delta(
      SubscriberId subscriber,
      PathIter begin,
      PathIter end,
      OperProtocol protocol) {
    return static_cast<Impl*>(this)->subscribe_delta_impl(
        subscriber, begin, end, protocol);
  }

  folly::coro::AsyncGenerator<std::vector<TaggedOperDelta>&&>
  subscribe_delta_extended(
      SubscriberId subscriber,
      std::vector<ExtendedOperPath> paths,
      OperProtocol protocol) {
    return static_cast<Impl*>(this)->subscribe_delta_extended_impl(
        subscriber, std::move(paths), protocol);
  }

  // wrapper calls to underlying storage

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, RootT>, void>>
  Result<typename Path::DataT> get(const Path& path) const {
    return this->template get<typename Path::DataT>(path.begin(), path.end());
  }
  template <typename T>
  Result<T> get(const ConcretePath& path) const {
    return this->template get<T>(path.begin(), path.end());
  }
  template <typename T>
  Result<T> get(PathIter begin, PathIter end) const {
    return static_cast<Impl const*>(this)->template get_impl<T>(begin, end);
  }

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, Root>, void>>
  Result<OperState> get_encoded(const Path& path, OperProtocol protocol) const {
    return this->get_encoded(path.begin(), path.end(), protocol);
  }
  Result<OperState> get_encoded(const ConcretePath& path, OperProtocol protocol)
      const {
    return this->get_encoded(path.begin(), path.end(), protocol);
  }
  Result<OperState>
  get_encoded(PathIter begin, PathIter end, OperProtocol protocol) const {
    return static_cast<Impl const*>(this)->get_encoded_impl(
        begin, end, protocol);
  }

  Result<std::vector<TaggedOperState>> get_encoded_extended(
      ExtPathIter begin,
      ExtPathIter end,
      OperProtocol protocol) const {
    return static_cast<Impl const*>(this)->get_encoded_extended_impl(
        begin, end, protocol);
  }

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, RootT>, void>>
  std::optional<StorageError> set(
      const Path& path,
      typename Path::DataT value) {
    return this->set(path.begin(), path.end(), std::move(value));
  }

  template <typename T>
  std::optional<StorageError> set(const ConcretePath& path, T&& value) {
    return this->set(path.begin(), path.end(), std::forward<T>(value));
  }
  template <typename T>
  std::optional<StorageError> set(PathIter begin, PathIter end, T&& value) {
    return static_cast<Impl*>(this)->set_impl(
        begin, end, std::forward<T>(value));
  }

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, Root>, void>>
  std::optional<StorageError> set_encoded(
      const Path& path,
      const OperState& state) {
    return this->set_encoded(path.begin(), path.end(), state);
  }
  std::optional<StorageError> set_encoded(
      const ConcretePath& path,
      const OperState& state) {
    return this->set_encoded(path.begin(), path.end(), state);
  }
  std::optional<StorageError>
  set_encoded(PathIter begin, PathIter end, const OperState& state) {
    return static_cast<Impl*>(this)->set_encoded_impl(begin, end, state);
  }

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, RootT>, void>>
  std::optional<StorageError> add(
      const Path& path,
      typename Path::DataT value) {
    return this->add(path.begin(), path.end(), std::move(value));
  }

  template <typename T>
  std::optional<StorageError> add(const ConcretePath& path, T&& value) {
    return this->add(path.begin(), path.end(), std::forward<T>(value));
  }
  template <typename T>
  std::optional<StorageError> add(PathIter begin, PathIter end, T&& value) {
    return static_cast<Impl*>(this)->add_impl(
        begin, end, std::forward<T>(value));
  }

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, RootT>, void>>
  void remove(const Path& path) {
    this->remove(path.begin(), path.end());
  }
  void remove(const ConcretePath& path) {
    this->remove(path.begin(), path.end());
  }
  void remove(PathIter begin, PathIter end) {
    static_cast<Impl*>(this)->remove_impl(begin, end);
  }

  std::optional<StorageError> patch(thrift_cow::Patch&& patch) {
    return static_cast<Impl*>(this)->patch_impl(std::move(patch));
  }

  std::optional<StorageError> patch(const fsdb::OperDelta& delta) {
    return static_cast<Impl*>(this)->patch_impl(delta);
  }

  std::optional<StorageError> patch(const fsdb::TaggedOperState& state) {
    return static_cast<Impl*>(this)->patch_impl(state);
  }

#ifdef ENABLE_DYNAMIC_APIS
  using DynamicResult = Result<folly::dynamic>;

  template <
      typename Path,
      typename = std::enable_if_t<
          std::is_same_v<typename folly::remove_cvref_t<Path>::RootT, RootT>,
          void>>
  folly::coro::AsyncGenerator<DeltaValue<folly::dynamic>&&> subscribe_dynamic(
      SubscriberId subscriber,
      Path&& path) {
    return this->subscribe_dynamic(subscriber, path.begin(), path.end());
  }
  folly::coro::AsyncGenerator<DeltaValue<folly::dynamic>&&> subscribe_dynamic(
      SubscriberId subscriber,
      const ConcretePath& path) {
    return this->subscribe_dynamic(subscriber, path.begin(), path.end());
  }
  folly::coro::AsyncGenerator<DeltaValue<folly::dynamic>&&>
  subscribe_dynamic(SubscriberId subscriber, PathIter begin, PathIter end) {
    return static_cast<Impl*>(this)->subscribe_dynamic_impl(
        subscriber, begin, end);
  }

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, RootT>, void>>
  DynamicResult get_dynamic(const Path& path) const {
    return get_dynamic(path.begin(), path.end());
  }
  DynamicResult get_dynamic(const ConcretePath& path) const {
    return get_dynamic(path.begin(), path.end());
  }
  DynamicResult get_dynamic(PathIter begin, PathIter end) const {
    return static_cast<Impl const*>(this)->get_dynamic_impl(begin, end);
  }

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, RootT>, void>>
  std::optional<StorageError> set_dynamic(
      const Path& path,
      const folly::dynamic& value) {
    return this->set_dynamic(path.begin(), path.end(), value);
  }
  std::optional<StorageError> set_dynamic(
      const ConcretePath& path,
      const folly::dynamic& value) {
    return set_dynamic(path.begin(), path.end(), value);
  }
  std::optional<StorageError>
  set_dynamic(PathIter begin, PathIter end, const folly::dynamic& value) {
    return static_cast<Impl*>(this)->set_dynamic_impl(begin, end, value);
  }

  template <
      typename Path,
      typename =
          std::enable_if_t<std::is_same_v<typename Path::RootT, RootT>, void>>
  std::optional<StorageError> add_dynamic(
      const Path& path,
      const folly::dynamic& value) {
    return this->add_dynamic(path.begin(), path.end(), value);
  }
  std::optional<StorageError> add_dynamic(
      const ConcretePath& path,
      const folly::dynamic& value) {
    return add_dynamic(path.begin(), path.end(), value);
  }
  std::optional<StorageError>
  add_dynamic(PathIter begin, PathIter end, const folly::dynamic& value) {
    return static_cast<Impl*>(this)->add_dynamic_impl(begin, end, value);
  }
#endif
};

} // namespace facebook::fboss::fsdb
