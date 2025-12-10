#include "utils.hpp"
#include "OPThreadPool.hpp"
#include "types.hpp"
#include <fstream>
#include <memory>
#include <sys/stat.h>
#include <utility>

namespace opawslc {

namespace jsi = facebook::jsi;
namespace react = facebook::react;

auto _thread_pool = std::make_shared<ThreadPool>();

inline jsi::Function host_fn(jsi::Runtime &rt, jsi::HostFunctionType lambda) {
  return jsi::Function::createFromHostFunction(
      rt, jsi::PropNameID::forAscii(rt, ""), 0, std::move(lambda));
};

jsi::Value
promisify(jsi::Runtime &rt, std::function<std::any()> lambda,
          std::function<jsi::Value(jsi::Runtime &rt, std::any result)>
              resolve_callback) {
  auto promise_constructor = rt.global().getPropertyAsFunction(rt, "Promise");

  auto executor = host_fn(rt, [lambda = std::move(lambda),
                               resolve_callback = std::move(resolve_callback)](
                                  jsi::Runtime &rt, const jsi::Value &thiz,
                                  const jsi::Value *args, size_t count) {
    auto resolve = std::make_shared<jsi::Value>(rt, args[0]);
    auto reject = std::make_shared<jsi::Value>(rt, args[1]);

    auto task = [lambda = lambda, resolve_callback = resolve_callback,
                 resolve = std::move(resolve), reject = std::move(reject)]() {
      try {
        std::any result = lambda();

        //        if (opsqlite::invalidated) {
        //          return;
        //        }

        opawslc::invoker->invokeAsync(
            [result = std::move(result), resolve = resolve,
             resolve_callback = resolve_callback](jsi::Runtime &rt) {
              auto jsi_result = resolve_callback(rt, result);
              resolve->asObject(rt).asFunction(rt).call(rt, jsi_result);
            });
      } catch (std::runtime_error &e) {
        // On Android RN is broken and does not correctly match
        // runtime_error to the generic exception We have to
        // explicitly catch it
        // https://github.com/facebook/react-native/issues/48027
        auto what = e.what();
        opawslc::invoker->invokeAsync(
            [what = std::string(what), reject = reject](jsi::Runtime &rt) {
              auto errorCtr = rt.global().getPropertyAsFunction(rt, "Error");
              auto error = errorCtr.callAsConstructor(
                  rt, jsi::String::createFromAscii(rt, what));
              reject->asObject(rt).asFunction(rt).call(rt, error);
            });
      } catch (std::exception &exc) {
        auto what = exc.what();
        opawslc::invoker->invokeAsync(
            [what = std::string(what), reject = reject](jsi::Runtime &rt) {
              auto errorCtr = rt.global().getPropertyAsFunction(rt, "Error");
              auto error = errorCtr.callAsConstructor(
                  rt, jsi::String::createFromAscii(rt, what));
              reject->asObject(rt).asFunction(rt).call(rt, error);
            });
      }
    };

    _thread_pool->queueWork(task);

    return jsi::Value(nullptr);
  });

  auto promise = promise_constructor.callAsConstructor(rt, executor);
  return promise;
}

} // namespace opawslc
