// Copyright (c) 2018 Doyub Kim
//
// I am making my contributions/submissions to this project solely in my
// personal capacity and am not conveying any rights to any intellectual
// property of any third parties.

#pragma once

#ifndef _MANAGED
#error This is for managed code only (/clr)
#endif

#include <memory>

#pragma region Namespace Helpers

#ifndef JET_CLR_SDK
#define JET_CLR_SDK Jet::CLR
#endif

#ifndef JET_NATIVE_SDK
#define JET_NATIVE_SDK jet
#endif

#ifndef JET_NAMESPACE_CLR_SDK
#define JET_NAMESPACE_CLR_SDK namespace Jet::CLR
#endif

#ifndef JET_BEGIN_NAMESPACE_CLR_SDK
#define JET_BEGIN_NAMESPACE_CLR_SDK \
    namespace Jet {                 \
    namespace CLR
#endif

#ifndef JET_END_NAMESPACE_CLR_SDK
#define JET_END_NAMESPACE_CLR_SDK }
#endif

#pragma endregion

#pragma region Default Convertors

JET_BEGIN_NAMESPACE_CLR_SDK {
    // Simple native-to-CLI converter (simple passing)
    template <typename T>
    inline T convertFromNative(T value) {
        return value;
    }

    // Simple native-to-CLI converter (simple passing)
    template <typename T>
    inline T convertToNative(T value) {
        return value;
    }
}
JET_END_NAMESPACE_CLR_SDK

#pragma endregion

#pragma region Core Implementation

#define __JET_DEFINE_NATIVE_CORE(NativeType)                               \
    internal:                                                              \
    typedef NativeType ActualType;                                         \
    typedef std::shared_ptr<ActualType> ActualTypeSharedPtr;               \
                                                                           \
 private:                                                                  \
    ActualType* getActualPtr() {                                           \
        return static_cast<ActualType*>(getNativePtr());                   \
    }                                                                      \
    ActualTypeSharedPtr getActualSharedPtr() {                             \
        return std::static_pointer_cast<ActualType>(getNativeSharedPtr()); \
    }

// For inherited classes
#define JET_DEFINE_NATIVE_CORE_FOR_DERIVED(NativeType) \
    __JET_DEFINE_NATIVE_CORE(NativeType)

// This is only used for top-most base classes or flat sealed classes.
#define JET_DEFINE_NATIVE_CORE_FOR_BASE(NativeType)                           \
    internal:                                                                 \
    typedef NativeType BaseNativeType;                                        \
    typedef std::shared_ptr<BaseNativeType> BaseNativeTypeSharedPtr;          \
    BaseNativeType* getNativePtr() { return __nativePtr; }                    \
    BaseNativeTypeSharedPtr& getNativeSharedPtr() {                           \
        return *__nativeSharedPtr;                                            \
    }                                                                         \
                                                                              \
 private:                                                                     \
    BaseNativeType* __nativePtr = nullptr;                                    \
    BaseNativeTypeSharedPtr* __nativeSharedPtr = nullptr;                     \
    internal:                                                                 \
    void __initializeNativePointer(BaseNativeType* nativePtr) {               \
        if (__nativeSharedPtr != nullptr) delete __nativeSharedPtr;           \
        __nativePtr = nativePtr;                                              \
        __nativeSharedPtr = new BaseNativeTypeSharedPtr(__nativePtr);         \
    }                                                                         \
    void __initializeNativePointerWithSharedPtr(                              \
        const BaseNativeTypeSharedPtr& nativeSharedPtr) {                     \
        if (__nativeSharedPtr != nullptr) {                                   \
            delete __nativeSharedPtr;                                         \
            __nativeSharedPtr = nullptr;                                      \
        }                                                                     \
        if (nativeSharedPtr != nullptr) {                                     \
            __nativePtr = nativeSharedPtr.get();                              \
            __nativeSharedPtr = new BaseNativeTypeSharedPtr(nativeSharedPtr); \
        } else {                                                              \
            __initializeNativePointer(nullptr);                               \
        }                                                                     \
    }                                                                         \
    void __finalizeNativePointer() {                                          \
        if (__nativeSharedPtr != nullptr) delete __nativeSharedPtr;           \
        __nativeSharedPtr = nullptr;                                          \
        __nativePtr = nullptr;                                                \
    }                                                                         \
    __JET_DEFINE_NATIVE_CORE(NativeType);

#pragma endregion

#pragma region Constructor / Finalizer

#define JET_INITIALIZE_NATIVE_CORE __initializeNativePointer(new ActualType());

#define JET_INITIALIZE_NATIVE_CORE_1(arg0) \
    __initializeNativePointer(new ActualType(arg0));

#define JET_INITIALIZE_NATIVE_CORE_2(arg0, arg1) \
    __initializeNativePointer(new ActualType(arg0, arg1));

#define JET_INITIALIZE_NATIVE_CORE_3(arg0, arg1, arg2) \
    __initializeNativePointer(new ActualType(arg0, arg1, arg2));

#define JET_INITIALIZE_NATIVE_CORE_4(arg0, arg1, arg2, arg3) \
    __initializeNativePointer(new ActualType(arg0, arg1, arg2, arg3));

#define JET_INITIALIZE_NATIVE_CORE_5(arg0, arg1, arg2, arg3, arg4) \
    __initializeNativePointer(new ActualType(arg0, arg1, arg2, arg3, arg4));

#define JET_INITIALIZE_NATIVE_CORE_6(arg0, arg1, arg2, arg3, arg4, arg5) \
    __initializeNativePointer(                                           \
        new ActualType(arg0, arg1, arg2, arg3, arg4, arg5));

#define JET_INITIALIZE_NATIVE_CORE_7(arg0, arg1, arg2, arg3, arg4, arg5, arg6) \
    __initializeNativePointer(                                                 \
        new ActualType(arg0, arg1, arg2, arg3, arg4, arg5, arg6));

#define JET_INITIALIZE_NATIVE_CORE_8(arg0, arg1, arg2, arg3, arg4, arg5, arg6, \
                                     arg7)                                     \
    __initializeNativePointer(                                                 \
        new ActualType(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7));

#define JET_INITIALIZE_NATIVE_CORE_9(arg0, arg1, arg2, arg3, arg4, arg5, arg6, \
                                     arg7, arg8)                               \
    __initializeNativePointer(                                                 \
        new ActualType(arg0, arg1, arg2, arg3, arg4, arg5, arg6, arg7, arg8));

#define JET_INITIALIZE_NATIVE_CORE_WITH_SHARED_PTR(nativeSharedPtr) \
    __initializeNativePointerWithSharedPtr(nativeSharedPtr);

#define JET_FINALIZE_NATIVE_CORE_FOR_BASE __finalizeNativePointer();

// Defines destructor for dervied class only
#define JET_DEFAULT_DESTRUCTOR_FOR_DERIVED(Type) \
 public:                                         \
    virtual ~Type() { this->!Type(); }           \
    !Type() {}

// Defines destructor for base class or flat classes
#define JET_DEFAULT_DESTRUCTOR_FOR_BASE(Type) \
 public:                                      \
    virtual ~Type() { this->!Type(); }        \
    !Type() { JET_FINALIZE_NATIVE_CORE_FOR_BASE }

#pragma endregion

#pragma region Member Variable Wrappers

#define JET_MEMBER_VARIABLE_TO_PROPERTY(type, varName, propertyName)        \
    property type propertyName {                                            \
        type get() {                                                        \
            return JET_CLR_SDK::convertFromNative(getActualPtr()->varName); \
        }                                                                   \
        void set(type value) {                                              \
            getActualPtr()->varName = JET_CLR_SDK::convertToNative(value);  \
        }                                                                   \
    }

#pragma endregion

#pragma region Member Function Wrappers

// With no args
#define JET_MEMBER_FUNCTION_TO_PROPERTY(type, getterName, setterName,          \
                                        propertyName)                          \
    property type propertyName {                                               \
        type get() {                                                           \
            return ::JET_CLR_SDK::convertFromNative(                           \
                getActualPtr()->getterName());                                 \
        }                                                                      \
        void set(type value) {                                                 \
            getActualPtr()->setterName(::JET_CLR_SDK::convertToNative(value)); \
        }                                                                      \
    }

#define JET_MEMBER_FUNCTION_TO_VIRTUAL_PROPERTY(type, getterName, setterName, \
                                                propertyName)                 \
    virtual property type propertyName {                                      \
        type get() {                                                          \
            return ::JET_CLR_SDK::convertFromNative(                          \
                getActualPtr()->getterName());                                \
        }                                                                     \
        void set(type value) {                                                \
            getActualPtr()->setterName(JET_CLR_SDK::convertToNative(value));  \
        }                                                                     \
    }

#define JET_MEMBER_FUNCTION_TO_GET_PROPERTY(type, funcName, propertyName) \
    property type propertyName {                                          \
        type get() {                                                      \
            return ::JET_CLR_SDK::convertFromNative(                      \
                getActualPtr()->funcName());                              \
        }                                                                 \
    }

#define JET_MEMBER_FUNCTION(type, funcName, newFuncName)                     \
    type newFuncName() {                                                     \
        return ::JET_CLR_SDK::convertFromNative(getActualPtr()->funcName()); \
    }

#define JET_MEMBER_FUNCTION_NO_RETURN(funcName, newFuncName) \
    void newFuncName() { getActualPtr()->funcName(); }

// With 1 arg
#define JET_MEMBER_FUNCTION_TO_SET_PROPERTY(type, setterName, propertyName)    \
    property type propertyName {                                               \
        void set(type value) {                                                 \
            getActualPtr()->setterName(::JET_CLR_SDK::convertToNative(value)); \
        }                                                                      \
    }

#define JET_MEMBER_FUNCTION_1(returnType, functionName, newFunctionName,      \
                              arg0Type, arg0Name)                             \
    returnType newFunctionName(arg0Type arg0Name) {                           \
        return ::JET_CLR_SDK::convertFromNative(getActualPtr()->functionName( \
            ::JET_CLR_SDK::convertToNative(arg0Name)));                       \
    }

#define JET_MEMBER_FUNCTION_DEFAULT_OVERLOAD_1(                                \
    returnType, functionName, newFunctionName, arg0Type, arg0Name)             \
    JET_MEMBER_FUNCTION_1(returnType, functionName, newFunctionName, arg0Type, \
                          arg0Name)

#define JET_MEMBER_FUNCTION_OVERRIDE_1(returnType, functionName,              \
                                       newFunctionName, arg0Type, arg0Name)   \
    returnType newFunctionName(arg0Type arg0Name) override {                  \
        return ::JET_CLR_SDK::convertFromNative(getActualPtr()->functionName( \
            ::JET_CLR_SDK::convertToNative(arg0Name)));                       \
    }

#define JET_MEMBER_FUNCTION_NO_RETURN_1(functionName, newFunctionName, \
                                        arg0Type, arg0Name)            \
    void newFunctionName(arg0Type arg0Name) {                          \
        getActualPtr()->functionName(                                  \
            ::JET_CLR_SDK::convertToNative(arg0Name));                 \
    }

#define JET_MEMBER_FUNCTION_DEFAULT_OVERLOAD_NO_RETURN_1(                    \
    functionName, newFunctionName, arg0Type, arg0Name)                       \
    JET_MEMBER_FUNCTION_NO_RETURN_1(functionName, newFunctionName, arg0Type, \
                                    arg0Name)

#define JET_MEMBER_FUNCTION_OVERRIDE_NO_RETURN_1(      \
    functionName, newFunctionName, arg0Type, arg0Name) \
    void newFunctionName(arg0Type arg0Name) override { \
        getActualPtr()->functionName(                  \
            ::JET_CLR_SDK::convertToNative(arg0Name)); \
    }

#define JET_MEMBER_FUNCTION_RETURN_BY_REF_1(wrapperReturnType,               \
                                            nativeReturnType, funcName,      \
                                            newFuncName, argType0, arg0Name) \
    wrapperReturnType newFuncName(argType0 arg0Name) {                       \
        nativeReturnType __result;                                           \
        getActualPtr()->funcName(::JET_CLR_SDK::convertToNative(arg0Name),   \
                                 __result);                                  \
        return ::JET_CLR_SDK::convertFromNative(__result);                   \
    }

#define JET_MEMBER_FUNCTION_OVERRIDE_RETURN_BY_REF_1(                      \
    wrapperReturnType, nativeReturnType, funcName, newFuncName, argType0,  \
    arg0Name)                                                              \
    wrapperReturnType newFuncName(argType0 arg0Name) override {            \
        nativeReturnType __result;                                         \
        getActualPtr()->funcName(::JET_CLR_SDK::convertToNative(arg0Name), \
                                 __result);                                \
        return ::JET_CLR_SDK::convertFromNative(__result);                 \
    }

// With 2 args
#define JET_MEMBER_FUNCTION_2(returnType, functionName, newFunctionName,      \
                              arg0Type, arg0Name, arg1Type, arg1Name)         \
    returnType newFunctionName(arg0Type arg0Name, arg1Type arg1Name) {        \
        return ::JET_CLR_SDK::convertFromNative(getActualPtr()->functionName( \
            ::JET_CLR_SDK::convertToNative(arg0Name),                         \
            ::JET_CLR_SDK::convertToNative(arg1Name)));                       \
    }

#define JET_VIRTUAL_MEMBER_FUNCTION_NO_RETURN_2(                           \
    functionName, newFunctionName, arg0Type, arg0Name, arg1Type, arg1Name) \
    virtual void newFunctionName(arg0Type arg0Name, arg1Type arg1Name) {   \
        getActualPtr()->functionName(                                      \
            ::JET_CLR_SDK::convertToNative(arg0Name),                      \
            ::JET_CLR_SDK::convertToNative(arg1Name));                     \
    }

#pragma endregion

#pragma region Interop Interface

#define JET_WRAPPER_INTERFACE_IMPLEMENTATION(FunctionName, NativeTypeName)   \
    void FunctionName(JET_WRAPPER_INTPTR nativeSharedPtrAddr) {              \
        std::shared_ptr<NativeTypeName>* result =                            \
            reinterpret_cast<std::shared_ptr<NativeTypeName>*>(              \
                (void*)nativeSharedPtrAddr);                                 \
        *result =                                                            \
            std::dynamic_pointer_cast<NativeTypeName>(getNativeSharedPtr()); \
    }

#define JET_GET_NATIVE_SHARED_PTR_FROM_WRAPPER(refPtr, wrapperGetFunction, \
                                               nativeSharedPtr)            \
    { refPtr->wrapperGetFunction(JET_WRAPPER_INTPTR(&nativeSharedPtr)); }

#define JET_GET_IMPL(GetFunction) \
    virtual JET_OBJECT ^ GetFunction() { return _impl; }

#pragma endregion

#pragma region Misc Helpers

#define JET_VIRTUAL_PROPERTY_CALLING_IMPL(type, name) \
    virtual property type name {                      \
        void set(type value) { _impl->name = value; } \
        type get() { return _impl->name; }            \
    }

#define JET_VIRTUAL_SET_PROPERTY_CALLING_IMPL(type, name) \
    virtual property type name {                          \
        void set(type value) { _impl->name = value; }     \
    }

#define JET_VIRTUAL_GET_PROPERTY_CALLING_IMPL(type, name) \
    virtual property type name {                          \
        type get() { return _impl->name; }                \
    }

#define JET_VIRTUAL_MEMBER_FUNCTION_CALLING_IMPL(returnType, name) \
    virtual returnType name() { return _impl->name(); }

#define JET_VIRTUAL_MEMBER_FUNCTION_CALLING_IMPL_NO_RETURN(name) \
    virtual void name() { _impl->name(); }

#define JET_VIRTUAL_MEMBER_FUNCTION_CALLING_IMPL_1(returnType, name, arg0Type, \
                                                   arg0Name)                   \
    virtual returnType name(arg0Type arg0Name) { return _impl->name(arg0Name); }

#define JET_VIRTUAL_MEMBER_FUNCTION_CALLING_IMPL_NO_RETURN_1(name, arg0Type, \
                                                             arg0Name)       \
    virtual void name(arg0Type arg0Name) { _impl->name(arg0Name); }

#define JET_VIRTUAL_MEMBER_FUNCTION_CALLING_IMPL_2(                 \
    returnType, name, arg0Type, arg0Name, arg1Type, arg1Name)       \
    virtual returnType name(arg0Type arg0Name, arg1Type arg1Name) { \
        return _impl->name(arg0Name, arg1Name);                     \
    }

#define JET_VIRTUAL_MEMBER_FUNCTION_CALLING_IMPL_NO_RETURN_2( \
    name, arg0Type, arg0Name, arg1Type, arg1Name)             \
    virtual void name(arg0Type arg0Name, arg1Type arg1Name) { \
        _impl->name(arg0Name, arg1Name);                      \
    }

#define JET_PROPERTY_CALLING_IMPL(type, name)         \
    property type name {                              \
        void set(type value) { _impl->name = value; } \
        type get() { return _impl->name; }            \
    }

#define JET_SET_PROPERTY_CALLING_IMPL(type, name)     \
    property type name {                              \
        void set(type value) { _impl->name = value; } \
    }

#define JET_GET_PROPERTY_CALLING_IMPL(type, name) \
    property type name {                          \
        type get() { return _impl->name; }        \
    }

#define JET_MEMBER_FUNCTION_CALLING_IMPL(returnType, name) \
    returnType name() { return _impl->name(); }

#define JET_MEMBER_FUNCTION_CALLING_IMPL_NO_RETURN(name) \
    void name() { _impl->name(); }

#define JET_MEMBER_FUNCTION_CALLING_IMPL_1(returnType, name, arg0Type, \
                                           arg0Name)                   \
    returnType name(arg0Type arg0Name) { return _impl->name(arg0Name); }

#define JET_MEMBER_FUNCTION_CALLING_IMPL_NO_RETURN_1(name, arg0Type, arg0Name) \
    void name(arg0Type arg0Name) { _impl->name(arg0Name); }

#define JET_MEMBER_FUNCTION_CALLING_IMPL_2(returnType, name, arg0Type,   \
                                           arg0Name, arg1Type, arg1Name) \
    returnType name(arg0Type arg0Name, arg1Type arg1Name) {              \
        return _impl->name(arg0Name, arg1Name);                          \
    }

#define JET_MEMBER_FUNCTION_CALLING_IMPL_NO_RETURN_2(name, arg0Type, arg0Name, \
                                                     arg1Type, arg1Name)       \
    void name(arg0Type arg0Name, arg1Type arg1Name) {                          \
        _impl->name(arg0Name, arg1Name);                                       \
    }

#define JET_MEMBER_FUNCTION_CALLING_IMPL_NO_RETURN_3(                    \
    name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name)    \
    void name(arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name) { \
        _impl->name(arg0Name, arg1Name, arg2Name);                       \
    }

#define JET_MEMBER_FUNCTION_CALLING_IMPL_NO_RETURN_4(                  \
    name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name,  \
    arg3Type, arg3Name)                                                \
    void name(arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, \
              arg3Type arg3Name) {                                     \
        _impl->name(arg0Name, arg1Name, arg2Name, arg3Name);           \
    }

#define JET_MEMBER_FUNCTION_CALLING_IMPL_NO_RETURN_5(                  \
    name, arg0Type, arg0Name, arg1Type, arg1Name, arg2Type, arg2Name,  \
    arg3Type, arg3Name, arg4Type, arg4Name)                            \
    void name(arg0Type arg0Name, arg1Type arg1Name, arg2Type arg2Name, \
              arg3Type arg3Name, arg4Type arg4Name) {                  \
        _impl->name(arg0Name, arg1Name, arg2Name, arg3Name, arg4Name); \
    }

#pragma endregion

#pragma region Common Language Forwarding

#define JET_WRAPPER_NEW gcnew

#define JET_WRAPPER_INTPTR System::IntPtr

#define JET_OBJECT System::Object

#define JET_WRAPPER_INPUT_ARRAY System::Collections::Generic::List

#define JET_WRAPPER_ACTUAL_ARRAY System::Collections::Generic::List

#define JET_WRAPPER_IENUMERABLE System::Collections::Generic::IEnumerable

#define JET_WRAPPER_ARRAY_ADD Add

#define JET_ARGUMENT_EXCEPTION System::ArgumentException

#define JET_WRAPPER_SDK JET_CLR_SDK

#define JET_NAMESPACE_WRAPPER_SDK JET_NAMESPACE_CLR_SDK

#define JET_BEGIN_NAMESPACE_WRAPPER_SDK JET_BEGIN_NAMESPACE_CLR_SDK

#define JET_END_NAMESPACE_WRAPPER_SDK JET_END_NAMESPACE_CLR_SDK

#pragma endregion

#pragma region Exceptions

#define JET_WRAPPER_THROW_INVALID_ARG(message) \
    throw gcnew System::ArgumentException(gcnew System::String(message));

#pragma endregion
