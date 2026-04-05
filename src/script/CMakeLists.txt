add_library(CsScript SHARED "")
add_library(CopperSpice::CsScript ALIAS CsScript)

set_target_properties(CsScript PROPERTIES OUTPUT_NAME CsScript${BUILD_ABI})

add_definitions(
    -DQLALR_NO_SCRIPTGRAMMAR_DEBUG_INFO
    -DJS_NO_EXPORT
    -DQLALR_NO_QSCRIPTGRAMMAR_DEBUG_INFO
    -DWTF_USE_JAVASCRIPTCORE_BINDINGS=1
    -DWTF_CHANGES=1
    -DBUILDING_CS=1
    -DWTF_USE_ACCELERATED_COMPOSITING
    -DJSC=QTJSC
    -Djscyyparse=qtjscyyparse
    -Djscyylex=qtjscyylex
    -Djscyyerror=qtjscyyerror
    -DWTF=QTWTF
    -DUSE_SYSTEM_MALLOC
    -DQT_BUILD_SCRIPT_LIB
)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

   if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL "7.1.0")
      set_target_properties(CsScript
         PROPERTIES
         COMPILE_OPTIONS -Wno-expansion-to-defined
      )
   endif()

endif()

set(EXTRA_SCRIPT_CXXFLAGS)
set(EXTRA_SCRIPT_LDFLAGS)

target_include_directories(CsScript
   PUBLIC
   $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include/QtScript>
   $<INSTALL_INTERFACE:include/QtScript>
)

include_directories(
    ${CMAKE_BINARY_DIR}/privateinclude/QtCore/private
    ${CMAKE_BINARY_DIR}/privateinclude/QtScript/private

    ${CMAKE_CURRENT_SOURCE_DIR}/api
    ${CMAKE_CURRENT_SOURCE_DIR}/bridge
    ${CMAKE_CURRENT_SOURCE_DIR}/parser

    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecompiler
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/debugger
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/heap
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/unicode
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/unicode
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/qt
)

include(api/api.cmake)
include(bridge/bridge.cmake)
include(parser/parser.cmake)

list(APPEND JSCORE_SCRIPT_INCLUDES
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/CodeBlock.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/Opcode.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/SamplingTool.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/JumpTable.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/Instruction.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/StructureStubInfo.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/EvalCodeCache.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/config.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/JavaScriptCorePrefix.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/OpaqueJSString.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSStringRefBSTR.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JavaScript.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSValueRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSCallbackFunction.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/APICast.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSObjectRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSBasePrivate.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSContextRefPrivate.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSCallbackObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSContextRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSStringRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/WebKitAvailability.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSClassRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSBase.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/APIShims.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSStringRefCF.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSCallbackConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSProfilerPrivate.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSRetainPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSCallbackObjectFunctions.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JavaScriptCore.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter/Interpreter.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter/CallFrameClosure.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter/RegisterFile.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter/Register.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter/CachedCall.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter/CallFrame.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/ArrayPrototype.lut.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/RegExpConstructor.lut.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/RegExpObject.lut.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/GeneratedJITStubs_RVCT.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/MathObject.lut.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/JSONObject.lut.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/Lexer.lut.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/NumberConstructor.lut.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/StringPrototype.lut.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/generated/DatePrototype.lut.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JITInlineMethods.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/ExecutableAllocator.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JITStubs.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JITCode.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JITStubCall.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JIT.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/OpaqueJSString.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/JavaScript.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/JSValueRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/APICast.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/JSObjectRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/JSContextRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/JSStringRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/WebKitAvailability.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/JSBase.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/JSStringRefCF.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/JSRetainPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/ForwardingHeaders/JavaScriptCore/JavaScriptCore.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/debugger/Debugger.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/debugger/DebuggerActivation.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/debugger/DebuggerCallFrame.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecompiler/Label.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecompiler/LabelScope.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecompiler/RegisterID.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecompiler/BytecodeGenerator.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/CodeLocation.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/ARMAssembler.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/MacroAssemblerCodeRef.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/AssemblerBuffer.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/MacroAssemblerX86_64.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/ARMv7Assembler.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/LinkBuffer.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/RepatchBuffer.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/MacroAssemblerARM.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/MacroAssemblerX86Common.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/MacroAssemblerARMv7.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/MacroAssembler.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/AssemblerBufferWithConstantPool.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/AbstractMacroAssembler.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/MacroAssemblerX86.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/X86Assembler.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ClassInfo.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ArgList.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/TimeoutChecker.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NumberConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSGlobalData.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Identifier.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/BooleanPrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/DateConversion.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/DateConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSZombie.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/WeakRandom.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/FunctionConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ScopeChainMark.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSGlobalObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/BatchedTransitionOptimizer.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSVariableObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/PropertyDescriptor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/BooleanConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Completion.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Structure.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/SymbolTable.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/heap/MarkStack.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ErrorPrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSType.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/DatePrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ObjectConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSArray.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NumericStrings.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSActivation.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StringConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSONObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ObjectPrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NativeErrorPrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ArrayConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSValue.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/DateInstance.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSAPIValueWrapper.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSGlobalObjectFunctions.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StringPrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/RegExpMatchesArray.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSByteArray.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Error.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/InitializeThreading.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ArrayPrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/PropertyMapHashTable.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSTypeInfo.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StructureTransitionTable.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NumberPrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/RegExpConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Tracing.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/GetterSetter.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NumberObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Executable.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSPropertyNameIterator.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Lookup.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/UString.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/MathObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/SmallStrings.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/RegExpObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/FunctionPrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/InternalFunction.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/RegExpPrototype.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/BooleanObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Protect.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Arguments.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StructureChain.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSFunction.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/WeakGCMap.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSLock.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/PropertySlot.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Operations.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NativeErrorConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ConstructData.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StringObjectThatMasqueradesAsUndefined.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ScopeChain.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ErrorInstance.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/LiteralParser.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSCell.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ExceptionHelpers.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/PropertyNameArray.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSString.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/DateInstanceCache.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/text/StringBuilder.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/PutPropertySlot.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ErrorConstructor.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSStaticScopeObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSWrapperObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/CommonIdentifiers.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSNotAnObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/RegExp.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/CallData.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StringObject.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/SourceProvider.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/ResultType.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/ParserArena.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/Nodes.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/NodeInfo.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/Lexer.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/SourceCode.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/NodeConstructors.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/Parser.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/ThreadSpecific.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/RefCountedLeakCounter.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/PassRefPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/HashFunctions.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/HashMap.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/HashIterators.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/SegmentedVector.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Vector.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/TypeTraits.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/TCPackedCache.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/ASCIICType.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/dtoa.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/RefPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/TCSpinLock.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/OwnArrayPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Platform.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/PossiblyNull.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/MessageQueue.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Forward.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/TCSystemAlloc.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/CurrentTime.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/MainThread.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/FastAllocBase.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/CrossThreadRefCounted.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/MathExtras.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Threading.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/StdLibExtras.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/RandomNumberSeed.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/RandomNumber.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/DateMath.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/GetPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/ByteArray.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/OwnPtrCommon.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/ListRefPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/HashTraits.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/VectorTraits.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/AVLTree.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Assertions.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/unicode/Collator.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/unicode/qt4/UnicodeQt4.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/unicode/UTF8.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/unicode/Unicode.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Locker.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/StringExtras.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/DisallowCType.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/FastMalloc.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/RefCounted.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/HashTable.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/MallocZoneSupport.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/OwnPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Deque.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Noncopyable.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/TCPageMap.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/HashSet.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/RefPtrHashMap.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/ThreadIdentifierDataPthreads.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/PassOwnPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/NotFound.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/VMTags.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/RetainPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/OwnFastMallocPtr.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/AlwaysInline.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/HashCountedSet.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/UnusedParam.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/ListHashSet.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/Profiler.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/Profile.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/ProfileGenerator.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/ProfileNode.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/ProfilerServer.h
    ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/CallIdentifier.h

    # https://bugreports.qt.io/browse/QTBUG-9958

)

target_sources(CsScript
   PRIVATE
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSBase.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSCallbackConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSCallbackFunction.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSCallbackObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSClassRef.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSContextRef.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSObjectRef.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSStringRef.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/JSValueRef.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/API/OpaqueJSString.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/ARMAssembler.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/assembler/MacroAssemblerARM.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/CodeBlock.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/JumpTable.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/Opcode.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/SamplingTool.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecode/StructureStubInfo.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecompiler/BytecodeGenerator.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/bytecompiler/NodesCodegen.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/debugger/DebuggerActivation.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/debugger/DebuggerCallFrame.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/debugger/Debugger.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter/CallFrame.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter/Interpreter.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/interpreter/RegisterFile.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/ExecutableAllocatorFixedVMPool.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/ExecutableAllocator.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JITArithmetic.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JITCall.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JIT.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JITOpcodes.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JITPropertyAccess.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/jit/JITStubs.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/Lexer.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/Nodes.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/ParserArena.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/parser/Parser.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/Profile.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/ProfileGenerator.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/ProfileNode.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/profiler/Profiler.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ArgList.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Arguments.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ArrayConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ArrayPrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/BooleanConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/BooleanObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/BooleanPrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/CallData.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/CommonIdentifiers.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Completion.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ConstructData.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/DateConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/DateConversion.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/DateInstance.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/DatePrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ErrorConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Error.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ErrorInstance.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ErrorPrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ExceptionHelpers.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Executable.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/FunctionConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/FunctionPrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/GetterSetter.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Identifier.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/InitializeThreading.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/InternalFunction.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSActivation.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSAPIValueWrapper.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSArray.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSByteArray.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSCell.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSFunction.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSGlobalData.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSGlobalObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSGlobalObjectFunctions.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSLock.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSNotAnObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSONObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSPropertyNameIterator.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSStaticScopeObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSString.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSValue.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSVariableObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/JSWrapperObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/LiteralParser.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Lookup.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/heap/MarkStackPosix.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/heap/MarkStackWin.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/heap/MarkStack.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/MathObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NativeErrorConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NativeErrorPrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NumberConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NumberObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/NumberPrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ObjectConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ObjectPrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Operations.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/PropertyDescriptor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/PropertyNameArray.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/PropertySlot.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/RegExpConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/RegExp.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/RegExpObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/RegExpPrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/ScopeChain.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/SmallStrings.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StringConstructor.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StringObject.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StringPrototype.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/StructureChain.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/Structure.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/TimeoutChecker.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/runtime/UString.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Assertions.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/ByteArray.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/CurrentTime.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/DateMath.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/dtoa.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/FastMalloc.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/HashTable.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/MainThread.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/qt/MainThreadQt.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/qt/ThreadingQt.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/RandomNumber.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/RefCountedLeakCounter.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/ThreadingNone.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/Threading.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/TypeTraits.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/unicode/CollatorDefault.cpp
   ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/wtf/unicode/UTF8.cpp
)

macro_generate_public("${SCRIPT_PUBLIC_INCLUDES}"   QtScript)
macro_generate_private("${SCRIPT_PRIVATE_INCLUDES}" QtScript)
macro_generate_misc("${SCRIPT_INCLUDES}"            QtScript)

if(CMAKE_SYSTEM_NAME MATCHES "Darwin")
   add_definitions(-DENABLE_JSC_MULTIPLE_THREADS=0)

   list(APPEND EXTRA_SCRIPT_LDFLAGS
      -framework
      CoreFoundation
   )

elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
   target_link_libraries(CsScript
      PRIVATE
      winmm
   )

   if(NOT MSVC)
      list(APPEND EXTRA_SCRIPT_LDFLAGS
         -Wl,-s
      )
   endif()

   list(APPEND SCRIPT_INCLUDES
      ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/os-win32/stdbool.h
      ${CMAKE_SOURCE_DIR}/src/3rdparty/webkit/Source/JavaScriptCore/os-win32/stdint.h
   )

endif()

function_variable_fixup("${EXTRA_SCRIPT_CXXFLAGS}" EXTRA_SCRIPT_CXXFLAGS)
function_variable_fixup("${EXTRA_SCRIPT_LDFLAGS}"  EXTRA_SCRIPT_LDFLAGS)

target_sources(CsScript
   PRIVATE
   ${SCRIPT_INCLUDES}
)

target_link_libraries(CsScript
   PRIVATE
   CsCore
)

set_target_properties(CsScript
   PROPERTIES
   COMPILE_FLAGS ${EXTRA_SCRIPT_CXXFLAGS}
   LINK_FLAGS ${EXTRA_SCRIPT_LDFLAGS}
)

function_generate_resources(CsScript)

macro_generate_package(
   CsScript
   QtScript
   "${EXTRA_SCRIPT_CXXFLAGS}"
   "CsCore"
)

install(
   TARGETS CsScript
   EXPORT CopperSpiceLibraryTargets ${INSTALL_TARGETS_DEFAULT_ARGS}
   RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
   LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
   ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(
   DIRECTORY ${CMAKE_BINARY_DIR}/include/QtScript
   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
   COMPONENT Devel
)
