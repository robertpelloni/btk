/***********************************************************************
*
* Copyright (c) 2012-2024 Barbara Geller
* Copyright (c) 2012-2024 Ansel Sermersheim
*
* Copyright (c) 2015 The Qt Company Ltd.
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
*
* This file is part of BTK.
*
* BTK is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* BTK is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* https://www.gnu.org/licenses/
*
***********************************************************************/

#ifndef QSCRIPTACTIVATIONOBJECT_P_H
#define QSCRIPTACTIVATIONOBJECT_P_H

#include "JSVariableObject.h"

namespace QScript {

class QScriptActivationObject : public JSC::JSVariableObject
{
 public:
   QScriptActivationObject(JSC::ExecState *callFrame, JSC::JSObject *delegate = nullptr);
   virtual ~QScriptActivationObject();
   virtual void visitChildren(JSC::MarkStack &markStack);
   virtual bool isDynamicScope(bool &requiresDynamicChecks) const {
      requiresDynamicChecks = false;
      return true;
   }

   virtual bool getOwnPropertySlot(JSC::ExecState *, const JSC::Identifier &propertyName, JSC::PropertySlot &);
   virtual bool getOwnPropertyDescriptor(JSC::ExecState *, const JSC::Identifier &propertyName, JSC::PropertyDescriptor &);
   virtual void getOwnPropertyNames(JSC::ExecState *, JSC::PropertyNameArray &,
      JSC::EnumerationMode mode = JSC::ExcludeDontEnumProperties);

   virtual void putWithAttributes(JSC::ExecState *exec, const JSC::Identifier &propertyName, JSC::JSValue value,
      unsigned attributes);
   virtual void put(JSC::ExecState *, const JSC::Identifier &propertyName, JSC::JSValue value, JSC::PutPropertySlot &);
   virtual void put(JSC::ExecState *, unsigned propertyName, JSC::JSValue value);

   virtual bool deleteProperty(JSC::ExecState *, const JSC::Identifier &propertyName);

   virtual void defineGetter(JSC::ExecState *, const JSC::Identifier &propertyName, JSC::JSObject *getterFunction,
      unsigned attributes = 0);
   virtual void defineSetter(JSC::ExecState *, const JSC::Identifier &propertyName, JSC::JSObject *setterFunction,
      unsigned attributes = 0);
   virtual JSC::JSValue lookupGetter(JSC::ExecState *, const JSC::Identifier &propertyName);
   virtual JSC::JSValue lookupSetter(JSC::ExecState *, const JSC::Identifier &propertyName);

   virtual const JSC::ClassInfo *classInfo() const {
      return &info;
   }
   static const JSC::ClassInfo info;

   JSC::JSObject *delegate() const {
      return m_delegate.get();
   }
   void setDelegate(JSC::JSObject *delegate);

 private:
   JSC::SymbolTable m_symbolTable;
   JSC::WriteBarrier<JSC::JSObject> m_delegate;
};

} // namespace QScript

#endif
