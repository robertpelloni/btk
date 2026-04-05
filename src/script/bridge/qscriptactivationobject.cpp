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
#include "qscriptactivationobject_p.h"

#include "Heap.h"
#include "JSVariableObject.h"

namespace JSC {
   ASSERT_CLASS_FITS_IN_CELL(QT_PREPEND_NAMESPACE(QScript::QScriptActivationObject));
}

namespace QScript {

const JSC::ClassInfo QScriptActivationObject::info = { "QScriptActivationObject", nullptr, nullptr, nullptr };

QScriptActivationObject::QScriptActivationObject(JSC::ExecState *callFrame, JSC::JSObject *delegate)
   : JSC::JSVariableObject(callFrame->globalData(), callFrame->globalData().activationStructure.get(), &m_symbolTable, callFrame->registers()),
     m_delegate(callFrame->globalData(), this, delegate)
{
}

QScriptActivationObject::~QScriptActivationObject()
{
}

void QScriptActivationObject::visitChildren(JSC::MarkStack &markStack)
{
   JSC::JSVariableObject::visitChildren(markStack);

   if (m_delegate) {
      markStack.append(&m_delegate);
   }
}

void QScriptActivationObject::setDelegate(JSC::JSObject *delegate)
{
   m_delegate.set(*JSC::Heap::heap(this)->globalData(), this, delegate);
}

bool QScriptActivationObject::getOwnPropertySlot(JSC::ExecState *exec, const JSC::Identifier &propertyName,
   JSC::PropertySlot &slot)
{
   if (delegate() != nullptr) {
      return delegate()->getOwnPropertySlot(exec, propertyName, slot);
   }

   return JSC::JSVariableObject::getOwnPropertySlot(exec, propertyName, slot);
}

bool QScriptActivationObject::getOwnPropertyDescriptor(JSC::ExecState *exec, const JSC::Identifier &propertyName,
   JSC::PropertyDescriptor &descriptor)
{
   if (delegate() != nullptr) {
      return delegate()->getOwnPropertyDescriptor(exec, propertyName, descriptor);
   }

   return JSC::JSVariableObject::getOwnPropertyDescriptor(exec, propertyName, descriptor);
}

void QScriptActivationObject::getOwnPropertyNames(JSC::ExecState *exec, JSC::PropertyNameArray &propertyNames,
   JSC::EnumerationMode mode)
{
   if (delegate() != nullptr) {
      delegate()->getOwnPropertyNames(exec, propertyNames, mode);
      return;
   }

   JSC::JSVariableObject::getOwnPropertyNames(exec, propertyNames, mode);
}

void QScriptActivationObject::putWithAttributes(JSC::ExecState *exec, const JSC::Identifier &propertyName,
   JSC::JSValue value, unsigned attributes)
{
   if (delegate() != nullptr) {
      delegate()->putWithAttributes(exec, propertyName, value, attributes);
      return;
   }

   if (symbolTablePutWithAttributes(exec->globalData(), propertyName, value, attributes)) {
      return;
   }

   JSC::PutPropertySlot slot;
   JSObject::putWithAttributes(exec, propertyName, value, attributes, true, slot);
}

void QScriptActivationObject::put(JSC::ExecState *exec, const JSC::Identifier &propertyName, JSC::JSValue value,
   JSC::PutPropertySlot &slot)
{
   if (delegate() != nullptr) {
      delegate()->put(exec, propertyName, value, slot);
      return;
   }

   JSC::JSVariableObject::put(exec, propertyName, value, slot);
}

void QScriptActivationObject::put(JSC::ExecState *exec, unsigned propertyName, JSC::JSValue value)
{
   if (delegate() != nullptr) {
      delegate()->put(exec, propertyName, value);
      return;
   }

   JSC::JSVariableObject::put(exec, propertyName, value);
}

bool QScriptActivationObject::deleteProperty(JSC::ExecState *exec, const JSC::Identifier &propertyName)
{
   if (delegate() != nullptr) {
      return delegate()->deleteProperty(exec, propertyName);
   }

   return JSC::JSVariableObject::deleteProperty(exec, propertyName);
}

void QScriptActivationObject::defineGetter(JSC::ExecState *exec, const JSC::Identifier &propertyName,
   JSC::JSObject *getterFunction, unsigned attributes)
{
   if (delegate() != nullptr) {
      delegate()->defineGetter(exec, propertyName, getterFunction, attributes);
   } else {
      JSC::JSVariableObject::defineGetter(exec, propertyName, getterFunction, attributes);
   }
}

void QScriptActivationObject::defineSetter(JSC::ExecState *exec, const JSC::Identifier &propertyName,
   JSC::JSObject *setterFunction, unsigned attributes)
{
   if (delegate() != nullptr) {
      delegate()->defineSetter(exec, propertyName, setterFunction, attributes);
   } else {
      JSC::JSVariableObject::defineSetter(exec, propertyName, setterFunction, attributes);
   }
}

JSC::JSValue QScriptActivationObject::lookupGetter(JSC::ExecState *exec, const JSC::Identifier &propertyName)
{
   if (delegate() != nullptr) {
      return delegate()->lookupGetter(exec, propertyName);
   }
   return JSC::JSVariableObject::lookupGetter(exec, propertyName);
}

JSC::JSValue QScriptActivationObject::lookupSetter(JSC::ExecState *exec, const JSC::Identifier &propertyName)
{
   if (delegate() != nullptr) {
      return delegate()->lookupSetter(exec, propertyName);
   }
   return JSC::JSVariableObject::lookupSetter(exec, propertyName);
}

} // namespace QScript
