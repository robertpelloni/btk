/***********************************************************************
*
* BTK visual developer overlay scaffold for owner-aware focus diagnostics.
* This widget renders the current BTK focus diagnostics in a lightweight
* always-on-top debug surface that can later evolve into a richer HUD.
*
***********************************************************************/

#ifndef BTK_FOCUS_OVERLAY_H
#define BTK_FOCUS_OVERLAY_H

#include <btkfocusdiagnostics.h>
#include <qwidget.h>

class Q_GUI_EXPORT BtkFocusOverlay : public QWidget
{
   GUI_CS_OBJECT(BtkFocusOverlay)

 public:
   explicit BtkFocusOverlay(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::ToolTip);

   BtkFocusOverlay(const BtkFocusOverlay &) = delete;
   BtkFocusOverlay &operator=(const BtkFocusOverlay &) = delete;

   ~BtkFocusOverlay();

   void setTargetWidget(QWidget *widget);
   QWidget *targetWidget() const;

   void setAutoRefreshEnabled(bool enabled);
   bool autoRefreshEnabled() const;

   void setRefreshInterval(int msec);
   int refreshInterval() const;

   void refreshDiagnostics();
   QString diagnosticsText() const;
   BtkFocusDiagnosticsSnapshot snapshot() const;

   QSize sizeHint() const override;

 protected:
   void paintEvent(QPaintEvent *event) override;
   void showEvent(QShowEvent *event) override;
   void timerEvent(QTimerEvent *event) override;

 private:
   void updateTimer();
   QString buildDisplayText() const;

   QWidget *m_targetWidget = nullptr;
   BtkFocusDiagnosticsSnapshot m_snapshot;
   QString m_diagnosticsText;
   int m_refreshInterval = 250;
   int m_timerId = 0;
   bool m_autoRefreshEnabled = true;
};

#endif
