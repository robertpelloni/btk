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

#ifndef QSCRIPTSTATICSCOPEOBJECT_P_H
#define QSCRIPTSTATICSCOPEOBJECT_P_H

#include "JSVariableObject.h"

class QScriptStaticScopeObject : public JSC::JSVariableObject
{
 public:
   struct PropertyInfo {
      PropertyInfo(const JSC::Identifier &i, JSC::JSValue v, unsigned a)
         : identifier(i), value(v), attributes(a) {
      }
      PropertyInfo() {}

      JSC::Identifier identifier;
      JSC::JSValue value;
      unsigned attributes;
   };

   QScriptStaticScopeObject(JSC::Structure *structure,
      int propertyCount, const PropertyInfo *);
   QScriptStaticScopeObject(JSC::Structure *structure);
   virtual ~QScriptStaticScopeObject();

   virtual bool isDynamicScope(bool &requiresDynamicChecks) const {
      requiresDynamicChecks = false;
      return false;
   }

   virtual bool getOwnPropertySlot(JSC::ExecState *, const JSC::Identifier &propertyName, JSC::PropertySlot &);
   virtual bool getOwnPropertyDescriptor(JSC::ExecState *, const JSC::Identifier &propertyName, JSC::PropertyDescriptor &);

   virtual void putWithAttributes(JSC::ExecState *exec, const JSC::Identifier &propertyName, JSC::JSValue value,
      unsigned attributes);
   virtual void put(JSC::ExecState *, const JSC::Identifier &propertyName, JSC::JSValue value, JSC::PutPropertySlot &);

   virtual bool deleteProperty(JSC::ExecState *, const JSC::Identifier &propertyName);

   virtual void visitChildren(JSC::MarkStack &);

   virtual const JSC::ClassInfo *classInfo() const {
      return &info;
   }
   static const JSC::ClassInfo info;

   static JSC::Structure *createStructure(JSC::JSGlobalData &globalData, JSC::JSValue proto) {
      return JSC::Structure::create(globalData, proto, JSC::TypeInfo(JSC::ObjectType, StructureFlags), AnonymousSlotCount, &info);
   }

 protected:
   static const unsigned StructureFlags = JSC::OverridesGetOwnPropertySlot | JSC::NeedsThisConversion |
      JSC::OverridesVisitChildren | JSC::OverridesGetPropertyNames | JSC::JSVariableObject::StructureFlags;

   JSC::SymbolTable m_symbolTable;
   bool m_canGrow;
   int m_registerArraySize;

 private:
   void addSymbolTableProperty(const JSC::Identifier &, JSC::JSValue, unsigned attributes);
   int growRegisterArray(int);
};

#endif
