/***********************************************************************
*
* Copyright (c) 2012-2024 Barbara Geller
* Copyright (c) 2012-2024 Ansel Sermersheim
*
* Copyright (c) 2015 The Qt Company Ltd.
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
*
* This file is part of CopperSpice.
*
* CopperSpice is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* https://www.gnu.org/licenses/
*
***********************************************************************/

#include "config.h"
#include "qscriptfunction_p.h"

#include "qscriptengine_p.h"
#include "qscriptcontext.h"
#include "qscriptcontext_p.h"
#include "qscriptvalue_p.h"
#include "qscriptactivationobject_p.h"
#include "qscriptobject_p.h"

#include "JSGlobalObject.h"
#include "DebuggerCallFrame.h"
#include "Debugger.h"

namespace JSC {
ASSERT_CLASS_FITS_IN_CELL(QT_PREPEND_NAMESPACE(QScript::FunctionWrapper));
ASSERT_CLASS_FITS_IN_CELL(QT_PREPEND_NAMESPACE(QScript::FunctionWithArgWrapper));
}

namespace QScript {

const JSC::ClassInfo FunctionWrapper::info = { "QtNativeFunctionWrapper", &JSC::JSFunction::s_info, nullptr, nullptr };
const JSC::ClassInfo FunctionWithArgWrapper::info = { "QtNativeFunctionWithArgWrapper", &JSC::JSFunction::s_info, nullptr, nullptr };

FunctionWrapper::FunctionWrapper(JSC::ExecState *exec, int length, const JSC::Identifier &name,
   QScriptEngine::FunctionSignature function)
   : JSC::PrototypeFunction(exec, length, name, proxyCall), data(new Data())
{
   data->function = function;
}

FunctionWrapper::~FunctionWrapper()
{
   delete data;
}

JSC::ConstructType FunctionWrapper::getConstructData(JSC::ConstructData &consData)
{
   consData.native.function = proxyConstruct;
   return JSC::ConstructTypeHost;
}

JSC::EncodedJSValue FunctionWrapper::proxyCall(JSC::ExecState *exec)
{
   FunctionWrapper *self = static_cast<FunctionWrapper *>(exec->callee());
   QScriptEnginePrivate *eng_p = QScript::scriptEngineFromExec(exec);
   JSC::ArgList args(exec);

   JSC::ExecState *oldFrame = eng_p->currentFrame;
   eng_p->pushContext(exec, exec->hostThisValue(), args, self);
   QScriptContext *ctx = eng_p->contextForFrame(eng_p->currentFrame);

   QScriptValue result = self->data->function(ctx, QScriptEnginePrivate::get(eng_p));
   if (!result.isValid()) {
      result = QScriptValue(QScriptValue::UndefinedValue);
   }

   eng_p->popContext();
   eng_p->currentFrame = oldFrame;

   return JSC::JSValue::encode(eng_p->scriptValueToJSCValue(result));
}

JSC::EncodedJSValue FunctionWrapper::proxyConstruct(JSC::ExecState *exec)
{
   FunctionWrapper *self = static_cast<FunctionWrapper *>(exec->callee());
   QScriptEnginePrivate *eng_p = QScript::scriptEngineFromExec(exec);
   JSC::ArgList args(exec);

   JSC::ExecState *oldFrame = eng_p->currentFrame;
   eng_p->pushContext(exec, JSC::JSValue(), args, self, true);
   QScriptContext *ctx = eng_p->contextForFrame(eng_p->currentFrame);

   QScriptValue result = self->data->function(ctx, QScriptEnginePrivate::get(eng_p));

   if (!result.isObject()) {
      result = ctx->thisObject();
   }

   eng_p->popContext();
   eng_p->currentFrame = oldFrame;

   return JSC::JSValue::encode(eng_p->scriptValueToJSCValue(result));
}

FunctionWithArgWrapper::FunctionWithArgWrapper(JSC::ExecState *exec, int length, const JSC::Identifier &name,
   QScriptEngine::FunctionWithArgSignature function, void *arg)
   : JSC::PrototypeFunction(exec, length, name, proxyCall),
     data(new Data())
{
   data->function = function;
   data->arg = arg;
}

FunctionWithArgWrapper::~FunctionWithArgWrapper()
{
   delete data;
}

JSC::ConstructType FunctionWithArgWrapper::getConstructData(JSC::ConstructData &consData)
{
   consData.native.function = proxyConstruct;
   return JSC::ConstructTypeHost;
}

JSC::EncodedJSValue FunctionWithArgWrapper::proxyCall(JSC::ExecState *exec)
{
   FunctionWithArgWrapper *self = static_cast<FunctionWithArgWrapper *>(exec->callee());
   QScriptEnginePrivate *eng_p = QScript::scriptEngineFromExec(exec);
   JSC::ArgList args(exec);

   JSC::ExecState *oldFrame = eng_p->currentFrame;
   eng_p->pushContext(exec, exec->hostThisValue(), args, self);
   QScriptContext *ctx = eng_p->contextForFrame(eng_p->currentFrame);

   QScriptValue result = self->data->function(ctx, QScriptEnginePrivate::get(eng_p), self->data->arg);

   eng_p->popContext();
   eng_p->currentFrame = oldFrame;

   return JSC::JSValue::encode(eng_p->scriptValueToJSCValue(result));
}

JSC::EncodedJSValue FunctionWithArgWrapper::proxyConstruct(JSC::ExecState *exec)
{
   FunctionWithArgWrapper *self = static_cast<FunctionWithArgWrapper *>(exec->callee());
   QScriptEnginePrivate *eng_p = QScript::scriptEngineFromExec(exec);
   JSC::ArgList args(exec);

   JSC::ExecState *oldFrame = eng_p->currentFrame;
   eng_p->pushContext(exec, JSC::JSValue(), args, self, true);
   QScriptContext *ctx = eng_p->contextForFrame(eng_p->currentFrame);

   QScriptValue result = self->data->function(ctx, QScriptEnginePrivate::get(eng_p), self->data->arg);
   if (!result.isObject()) {
      result = ctx->thisObject();
   }

   eng_p->popContext();
   eng_p->currentFrame = oldFrame;

   return JSC::JSValue::encode(eng_p->scriptValueToJSCValue(result));
}

} // namespace
