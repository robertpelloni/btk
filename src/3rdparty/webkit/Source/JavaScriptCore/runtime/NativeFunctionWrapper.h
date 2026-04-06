/*
 * Compatibility wrapper for legacy QtScript-era host functions.
 */

#ifndef NativeFunctionWrapper_h
#define NativeFunctionWrapper_h

#include "ArgList.h"
#include "JSFunction.h"

namespace JSC {

    typedef JSValue (JSC_HOST_CALL *LegacyNativeFunction)(ExecState*, JSObject*, JSValue, const ArgList&);

    class NativeFunctionWrapper : public JSFunction {
    public:
        NativeFunctionWrapper(ExecState* exec, Structure* structure, int length, const Identifier& name, LegacyNativeFunction function)
            : JSFunction(exec, exec->lexicalGlobalObject(), structure, length, name, proxyCall)
            , m_function(function)
        {
        }

        NativeFunctionWrapper(ExecState* exec, Structure* structure, int length, const Identifier& name, NativeFunction function)
            : JSFunction(exec, exec->lexicalGlobalObject(), structure, length, name, function)
            , m_function(nullptr)
        {
        }

    private:
        static EncodedJSValue JSC_HOST_CALL proxyCall(ExecState* exec)
        {
            NativeFunctionWrapper* self = static_cast<NativeFunctionWrapper*>(exec->callee());
            ArgList args(exec);
            return JSValue::encode(self->m_function(exec, self, exec->hostThisValue(), args));
        }

        LegacyNativeFunction m_function;
    };

} // namespace JSC

#endif // NativeFunctionWrapper_h
