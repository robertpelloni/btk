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
#include "qscriptobject_p.h"

namespace JSC {
ASSERT_CLASS_FITS_IN_CELL(QT_PREPEND_NAMESPACE(QScriptObject));
ASSERT_CLASS_FITS_IN_CELL(QT_PREPEND_NAMESPACE(QScriptObjectPrototype));
}

// masquerading as JSC::JSObject
const JSC::ClassInfo QScriptObject::info = { "Object", nullptr, nullptr, nullptr };

QScriptObject::Data::~Data()
{
   delete delegate;
}

QScriptObject::QScriptObject(JSC::JSGlobalData *globalData, JSC::Structure *sid)
   : JSC::JSNonFinalObject(*globalData, sid), d(nullptr)
{
}

QScriptObject::~QScriptObject()
{
   delete d;
}

bool QScriptObject::getOwnPropertySlot(JSC::ExecState *exec,
   const JSC::Identifier &propertyName,
   JSC::PropertySlot &slot)
{
   if (!d || !d->delegate) {
      return JSC::JSObject::getOwnPropertySlot(exec, propertyName, slot);
   }

   return d->delegate->getOwnPropertySlot(this, exec, propertyName, slot);
}

bool QScriptObject::getOwnPropertyDescriptor(JSC::ExecState *exec,
   const JSC::Identifier &propertyName,
   JSC::PropertyDescriptor &descriptor)
{
   if (!d || !d->delegate) {
      return JSC::JSObject::getOwnPropertyDescriptor(exec, propertyName, descriptor);
   }
   return d->delegate->getOwnPropertyDescriptor(this, exec, propertyName, descriptor);
}

void QScriptObject::put(JSC::ExecState *exec, const JSC::Identifier &propertyName,
   JSC::JSValue value, JSC::PutPropertySlot &slot)
{
   if (!d || !d->delegate) {
      JSC::JSObject::put(exec, propertyName, value, slot);
      return;
   }
   d->delegate->put(this, exec, propertyName, value, slot);
}

bool QScriptObject::deleteProperty(JSC::ExecState *exec,
   const JSC::Identifier &propertyName)
{
   if (!d || !d->delegate) {
      return JSC::JSObject::deleteProperty(exec, propertyName);
   }
   return d->delegate->deleteProperty(this, exec, propertyName);
}

void QScriptObject::getOwnPropertyNames(JSC::ExecState *exec, JSC::PropertyNameArray &propertyNames,
   JSC::EnumerationMode mode)
{
   if (!d || !d->delegate) {
      JSC::JSObject::getOwnPropertyNames(exec, propertyNames, mode);
      return;
   }
   d->delegate->getOwnPropertyNames(this, exec, propertyNames, mode);
}

bool QScriptObject::compareToObject(JSC::ExecState *exec, JSC::JSObject *other)
{
   if (!d || !d->delegate) {
      (void) exec;
      return this == other;
   }
   return d->delegate->compareToObject(this, exec, other);
}

void QScriptObject::visitChildren(JSC::MarkStack &markStack)
{
   if (!d) {
      d = new Data();
   }

   if (d->isMarking) {
      return;
   }

   bool temp = d->isMarking;
   d->isMarking = true;

   JSC::JSObject::visitChildren(markStack);

   if (d->data) {
   }

   if (d->delegate) {
      d->delegate->markChildren(this, markStack);
   }

   d->isMarking = temp;
}

JSC::CallType QScriptObject::getCallData(JSC::CallData &data)
{
   if (!d || !d->delegate) {
      return JSC::JSObject::getCallData(data);
   }
   return d->delegate->getCallData(this, data);
}

JSC::ConstructType QScriptObject::getConstructData(JSC::ConstructData &data)
{
   if (!d || !d->delegate) {
      return JSC::JSObject::getConstructData(data);
   }
   return d->delegate->getConstructData(this, data);
}

bool QScriptObject::hasInstance(JSC::ExecState *exec, JSC::JSValue value, JSC::JSValue proto)
{
   if (!d || !d->delegate) {
      return JSC::JSObject::hasInstance(exec, value, proto);
   }
   return d->delegate->hasInstance(this, exec, value, proto);
}

QScriptObjectPrototype::QScriptObjectPrototype(JSC::ExecState *exec, JSC::Structure *structure,
   JSC::Structure * /*prototypeFunctionStructure*/)
   : QScriptObject(&exec->globalData(), structure)
{
}

QScriptObjectDelegate::QScriptObjectDelegate()
{
}

QScriptObjectDelegate::~QScriptObjectDelegate()
{
}

bool QScriptObjectDelegate::getOwnPropertySlot(QScriptObject *object, JSC::ExecState *exec,
   const JSC::Identifier &propertyName,
   JSC::PropertySlot &slot)
{
   return object->JSC::JSObject::getOwnPropertySlot(exec, propertyName, slot);
}

bool QScriptObjectDelegate::getOwnPropertyDescriptor(QScriptObject *object, JSC::ExecState *exec,
   const JSC::Identifier &propertyName,
   JSC::PropertyDescriptor &descriptor)
{
   return object->JSC::JSObject::getOwnPropertyDescriptor(exec, propertyName, descriptor);
}


void QScriptObjectDelegate::put(QScriptObject *object, JSC::ExecState *exec,
   const JSC::Identifier &propertyName,
   JSC::JSValue value, JSC::PutPropertySlot &slot)
{
   object->JSC::JSObject::put(exec, propertyName, value, slot);
}

bool QScriptObjectDelegate::deleteProperty(QScriptObject *object, JSC::ExecState *exec,
   const JSC::Identifier &propertyName)
{
   return object->JSC::JSObject::deleteProperty(exec, propertyName);
}

void QScriptObjectDelegate::getOwnPropertyNames(QScriptObject *object, JSC::ExecState *exec,
   JSC::PropertyNameArray &propertyNames,
   JSC::EnumerationMode mode)
{
   object->JSC::JSObject::getOwnPropertyNames(exec, propertyNames, mode);
}

void QScriptObjectDelegate::markChildren(QScriptObject *object, JSC::MarkStack &markStack)
{
   // ### should this call the virtual function instead??
   object->JSC::JSObject::visitChildren(markStack);
}

JSC::CallType QScriptObjectDelegate::getCallData(QScriptObject *object, JSC::CallData &data)
{
   return object->JSC::JSObject::getCallData(data);
}

JSC::ConstructType QScriptObjectDelegate::getConstructData(QScriptObject *object, JSC::ConstructData &data)
{
   return object->JSC::JSObject::getConstructData(data);
}

bool QScriptObjectDelegate::hasInstance(QScriptObject *object, JSC::ExecState *exec,
   JSC::JSValue value, JSC::JSValue proto)
{
   return object->JSC::JSObject::hasInstance(exec, value, proto);
}

bool QScriptObjectDelegate::compareToObject(QScriptObject *object, JSC::ExecState *exec, JSC::JSObject *o)
{
   (void) exec;
   return object == o;
}
