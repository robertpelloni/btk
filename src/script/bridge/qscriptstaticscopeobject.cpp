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
#include "qscriptstaticscopeobject_p.h"

#include "Heap.h"

namespace JSC {
   ASSERT_CLASS_FITS_IN_CELL(QT_PREPEND_NAMESPACE(QScriptStaticScopeObject));
}

const JSC::ClassInfo QScriptStaticScopeObject::info = { "QScriptStaticScopeObject", nullptr, nullptr, nullptr };

QScriptStaticScopeObject::QScriptStaticScopeObject(JSC::Structure *structure,
      int propertyCount, const PropertyInfo *props)
   : JSC::JSVariableObject(*JSC::Heap::heap(structure)->globalData(), structure, &m_symbolTable, nullptr),
     m_canGrow(false),
     m_registerArraySize(0)
{
   int index = growRegisterArray(propertyCount);

   for (int i = 0; i < propertyCount; ++i, --index) {
      const PropertyInfo &prop = props[i];
      JSC::SymbolTableEntry entry(index, prop.attributes);
      symbolTable().add(prop.identifier.impl(), entry);
      registerAt(index).set(*JSC::Heap::heap(this)->globalData(), this, prop.value);
   }
}

QScriptStaticScopeObject::QScriptStaticScopeObject(JSC::Structure *structure)
   : JSC::JSVariableObject(*JSC::Heap::heap(structure)->globalData(), structure, &m_symbolTable, nullptr),
     m_canGrow(true),
     m_registerArraySize(0)
{
}

QScriptStaticScopeObject::~QScriptStaticScopeObject()
{
}

bool QScriptStaticScopeObject::getOwnPropertySlot(JSC::ExecState *, const JSC::Identifier &propertyName,
   JSC::PropertySlot &slot)
{
   return symbolTableGet(propertyName, slot);
}

bool QScriptStaticScopeObject::getOwnPropertyDescriptor(JSC::ExecState *, const JSC::Identifier &propertyName,
   JSC::PropertyDescriptor &descriptor)
{
   return symbolTableGet(propertyName, descriptor);
}

void QScriptStaticScopeObject::putWithAttributes(JSC::ExecState *exec, const JSC::Identifier &propertyName,
   JSC::JSValue value, unsigned attributes)
{
   if (symbolTablePutWithAttributes(exec->globalData(), propertyName, value, attributes)) {
      return;
   }

   Q_ASSERT(m_canGrow);
   addSymbolTableProperty(propertyName, value, attributes);
}

void QScriptStaticScopeObject::put(JSC::ExecState *exec, const JSC::Identifier &propertyName, JSC::JSValue value,
   JSC::PutPropertySlot &)
{
   if (symbolTablePut(exec->globalData(), propertyName, value)) {
      return;
   }

   Q_ASSERT(m_canGrow);
   addSymbolTableProperty(propertyName, value, /*attributes=*/0);
}

bool QScriptStaticScopeObject::deleteProperty(JSC::ExecState *, const JSC::Identifier &)
{
   return false;
}

void QScriptStaticScopeObject::visitChildren(JSC::MarkStack &markStack)
{
   JSC::JSVariableObject::visitChildren(markStack);

   JSC::WriteBarrier<JSC::Unknown> *registerArray = m_registerArray.get();
   if (! registerArray) {
      return;
   }
   markStack.appendValues(registerArray, m_registerArraySize);
}

void QScriptStaticScopeObject::addSymbolTableProperty(const JSC::Identifier &name, JSC::JSValue value,
   unsigned attributes)
{
   int index = growRegisterArray(1);
   JSC::SymbolTableEntry newEntry(index, attributes | JSC::DontDelete);
   symbolTable().add(name.impl(), newEntry);
   registerAt(index).set(*JSC::Heap::heap(this)->globalData(), this, value);
}

/*!
  Grows the register array by \a count elements, and returns the offset of
  the newly added elements (note that the register file grows downwards,
  starting at index -1).
*/
int QScriptStaticScopeObject::growRegisterArray(int count)
{
   size_t oldSize = m_registerArraySize;
   size_t newSize = oldSize + count;

   WTF::OwnArrayPtr<JSC::WriteBarrier<JSC::Unknown>> registerArray = WTF::adoptArrayPtr(new JSC::WriteBarrier<JSC::Unknown>[newSize]);

   for (size_t i = 0; i < oldSize; ++i) {
      registerArray[count + i].setWithoutWriteBarrier(m_registerArray[i].get());
   }

   setRegisters(registerArray.get() + newSize, registerArray.release());
   m_registerArraySize = static_cast<int>(newSize);
   return -static_cast<int>(oldSize) - 1;
}
