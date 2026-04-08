/***********************************************************************
*
* BTK compatibility aliases for the CsSignal annex library.
* These aliases provide an incremental migration path from Cs* naming
* toward BTK-branded public API usage without changing ABI.
*
* BTK signals and slots provide type-safe callback mechanisms compatible
* with Qt's signal/slot pattern while using modern C++ without moc.
*
***********************************************************************/

#ifndef BTK_SIGNAL_COMPAT_H
#define BTK_SIGNAL_COMPAT_H

#include <cs_signal.h>
#include <cs_slot.h>

// BTK namespace alias for the full CsSignal namespace
namespace BtkSignal = CsSignal;

// Signal/slot base types
using BtkConnectionKind = CsSignal::ConnectionKind;
using BtkDisconnectKind = CsSignal::DisconnectKind;
using BtkSignalBase     = CsSignal::SignalBase;
using BtkSlotBase       = CsSignal::SlotBase;
using BtkPendingSlot    = CsSignal::PendingSlot;

// Connection kind constants (matching Qt6 connection semantics)
constexpr auto BtkAutoConnection    = CsSignal::ConnectionKind::AutoConnection;
constexpr auto BtkDirectConnection  = CsSignal::ConnectionKind::DirectConnection;
constexpr auto BtkQueuedConnection  = CsSignal::ConnectionKind::QueuedConnection;
constexpr auto BtkBlockingQueuedConnection = CsSignal::ConnectionKind::BlockingQueuedConnection;

#endif
