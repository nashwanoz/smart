#include <napi.h>
#include <windows.h>
#include <cstring>

Napi::Value ProtectMemory(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected 2 arguments (buffer, size)").ThrowAsJavaScriptException();
    return env.Null();
  }
  
  Napi::Uint8Array buffer = info[0].As<Napi::Uint8Array>();
  size_t size = info[1].As<Napi::Number>().Uint32Value();
  
  void* ptr = buffer.Data();
  
  DWORD oldProtect;
  BOOL result = VirtualProtect(ptr, size, PAGE_NOACCESS, &oldProtect);
  
  if (!result) {
    DWORD errorCode = GetLastError();
    Napi::Error::New(env, "Failed to protect memory. Error code: " + std::to_string(errorCode)).ThrowAsJavaScriptException();
    return env.Null();
  }
  
  return env.Null();
}

Napi::Value UnprotectMemory(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  if (info.Length() < 2) {
    Napi::TypeError::New(env, "Expected 2 arguments (buffer, size)").ThrowAsJavaScriptException();
    return env.Null();
  }
  
  Napi::Uint8Array buffer = info[0].As<Napi::Uint8Array>();
  size_t size = info[1].As<Napi::Number>().Uint32Value();
  
  void* ptr = buffer.Data();
  
  DWORD oldProtect;
  VirtualProtect(ptr, size, PAGE_READWRITE, &oldProtect);
  
  return env.Null();
}

Napi::Value ClearMemory(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected 1 argument (buffer)").ThrowAsJavaScriptException();
    return env.Null();
  }
  
  Napi::Uint8Array buffer = info[0].As<Napi::Uint8Array>();
  size_t size = buffer.ByteLength();
  void* ptr = buffer.Data();
  
  memset(ptr, 0, size);
  
  return env.Null();
}

Napi::Value RandomizeMemory(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  
  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Expected 1 argument (buffer)").ThrowAsJavaScriptException();
    return env.Null();
  }
  
  Napi::Uint8Array buffer = info[0].As<Napi::Uint8Array>();
  size_t size = buffer.ByteLength();
  void* ptr = buffer.Data();
  
  for (size_t i = 0; i < size; i++) {
    ((unsigned char*)ptr)[i] = (unsigned char)(rand() % 256);
  }
  
  return env.Null();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("protectMemory", Napi::Function::New(env, ProtectMemory));
  exports.Set("unprotectMemory", Napi::Function::New(env, UnprotectMemory));
  exports.Set("clearMemory", Napi::Function::New(env, ClearMemory));
  exports.Set("randomizeMemory", Napi::Function::New(env, RandomizeMemory));
  return exports;
}

NODE_API_MODULE(secure_memory, Init)
