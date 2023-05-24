#include <napi.h>
#include <Windows.h>
#include <iostream>
#include <string>
typedef int(__stdcall *Counter)();

// 这里是dll文件的句柄
static HMODULE module;
// dll中的函数指针
Counter dllcounter = nullptr;

Napi::Value counter(const Napi::CallbackInfo &info)
{
    // 参数长度不为1
    if (info.Length() != 1)
    {
        Napi::Error::New(info.Env(), "Expected exactly one argument")
            .ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    // 第一个参数不为字符串
    if (!info[0].IsString())
    {
        Napi::Error::New(info.Env(), "Expect a string")
            .ThrowAsJavaScriptException();
        return info.Env().Undefined();
    }
    if (!dllcounter)
    {
        // dll文件路径
        std::string dllPath = info[0].As<Napi::String>();
        // 加载dll
        module = reinterpret_cast<HMODULE>(LoadLibraryA(dllPath.c_str()));
        dllcounter = (Counter)GetProcAddress(module, "counter");
        if (dllcounter == nullptr)
        {
            // dll加载失败
            Napi::Error::New(info.Env(), "Dll module load failed")
                .ThrowAsJavaScriptException();
            return info.Env().Undefined();
        }
        int count = dllcounter();
        return Napi::Number::New(info.Env(), count);
    }
    else
    {
        int count = dllcounter();
        return Napi::Number::New(info.Env(), count);
    }
}
static Napi::Object Init(Napi::Env env, Napi::Object exports)
{
    exports["counter"] = Napi::Function::New(env, counter);
    return exports;
}

NODE_API_MODULE(NODE_GYP_MODULE_NAME, Init)