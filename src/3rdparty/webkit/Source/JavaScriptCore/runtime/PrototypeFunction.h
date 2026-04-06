/*
 * Compatibility wrapper for legacy QtScript-era prototype functions.
 */

#ifndef PrototypeFunction_h
#define PrototypeFunction_h

#include "JSGlobalObject.h"
#include "NativeFunctionWrapper.h"

namespace JSC {

    class PrototypeFunction : public NativeFunctionWrapper {
    public:
        PrototypeFunction(ExecState* exec, int length, const Identifier& name, LegacyNativeFunction function)
            : NativeFunctionWrapper(exec, exec->lexicalGlobalObject()->prototypeFunctionStructure(), length, name, function)
        {
        }

        PrototypeFunction(ExecState* exec, int length, const Identifier& name, NativeFunction function)
            : NativeFunctionWrapper(exec, exec->lexicalGlobalObject()->prototypeFunctionStructure(), length, name, function)
        {
        }
    };

} // namespace JSC

#endif // PrototypeFunction_h
