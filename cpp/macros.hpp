#pragma once

#define HFN0 jsi::Function::createFromHostFunction(rt, jsi::PropNameID::forAscii(rt, ""), 0, [](jsi::Runtime &rt, [[maybe_unused]] const jsi::Value &that, const jsi::Value *args, [[maybe_unused]] size_t count) -> jsi::Value
#define HFN(c1) jsi::Function::createFromHostFunction(rt, jsi::PropNameID::forAscii(rt, ""), 0, [c1](jsi::Runtime &rt, [[maybe_unused]] const jsi::Value &that, const jsi::Value *args, [[maybe_unused]] size_t count) -> jsi::Value
#define HFN2(c1, c2) jsi::Function::createFromHostFunction(rt, jsi::PropNameID::forAscii(rt, ""), 0, [c1, c2](jsi::Runtime &rt, [[maybe_unused]] const jsi::Value &that, const jsi::Value *args, [[maybe_unused]] size_t count) -> jsi::Value
#define HFN3(c1, c2, c3) jsi::Function::createFromHostFunction(rt, jsi::PropNameID::forAscii(rt, ""), 0, [c1, c2, c3](jsi::Runtime &rt, [[maybe_unused]] const jsi::Value &that, const jsi::Value *args, [[maybe_unused]] size_t count) -> jsi::Value
