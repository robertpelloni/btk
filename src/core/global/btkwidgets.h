/***********************************************************************
*
* BTK Widget type aliases.
* These aliases provide a clean BTK-branded API surface over the
* existing Q*-based widget types, enabling incremental migration
* without breaking existing code.
*
* Both BTK and Qt-style names work:
*   BTKWidget == QWidget
*   BTKButton == QPushButton
*   etc.
*
***********************************************************************/

#ifndef BTK_WIDGET_ALIASES_H
#define BTK_WIDGET_ALIASES_H

#include <qwidget.h>
#include <qpushbutton.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qplaintextedit.h>
#include <qcheckbox.h>
#include <qradiobutton.h>
#include <qtoolbutton.h>
#include <qframe.h>
#include <qgroupbox.h>
#include <qtabwidget.h>
#include <qstackedwidget.h>
#include <qsplitter.h>
#include <qscrollarea.h>
#include <qlistwidget.h>
#include <qtreewidget.h>
#include <qtablewidget.h>
#include <qlistview.h>
#include <qtreeview.h>
#include <qtableview.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qdoublevalidator.h>
#include <qslider.h>
#include <qprogressbar.h>
#include <qdial.h>
#include <qscrollbar.h>
#include <qmenubar.h>
#include <qmenu.h>
#include <qtoolbar.h>
#include <qstatusbar.h>
#include <qaction.h>
#include <qmainwindow.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qfiledialog.h>
#include <qcolordialog.h>
#include <qfontdialog.h>
#include <qinputdialog.h>
#include <qprogressdialog.h>
#include <qwizard.h>
#include <qcalendarwidget.h>
#include <qdatetimeedit.h>

// Base widget
using BTKWidget      = QWidget;
using BTKWindow      = QMainWindow;
using BTKDialog      = QDialog;

// Buttons
using BTKButton      = QPushButton;
using BTKCheckBox    = QCheckBox;
using BTKRadioButton = QRadioButton;
using BTKToolButton  = QToolButton;

// Text display/edit
using BTKLabel       = QLabel;
using BTKLineEdit    = QLineEdit;
using BTKTextEdit    = QTextEdit;
using BTKPlainEdit   = QPlainTextEdit;

// Containers
using BTKFrame       = QFrame;
using BTKGroupBox    = QGroupBox;
using BTKTabWidget   = QTabWidget;
using BTKStackedWidget = QStackedWidget;
using BTKSplitter    = QSplitter;
using BTKScrollArea  = QScrollArea;

// Item views
using BTKListView    = QListView;
using BTKTreeView    = QTreeView;
using BTKTableView   = QTableView;
using BTKListWidget  = QListWidget;
using BTKTreeWidget  = QTreeWidget;
using BTKTableWidget = QTableWidget;

// Input widgets
using BTKComboBox    = QComboBox;
using BTKSpinBox     = QSpinBox;
using BTKDoubleSpinBox = QDoubleSpinBox;
using BTKSlider      = QSlider;
using BTKDial        = QDial;
using BTKScrollBar   = QScrollBar;
using BTKProgressBar = QProgressBar;
using BTKCalendar    = QCalendarWidget;
using BTKDateTimeEdit = QDateTimeEdit;

// Actions and menus
using BTKAction      = QAction;
using BTKMenuBar     = QMenuBar;
using BTKMenu        = QMenu;
using BTKToolBar     = QToolBar;
using BTKStatusBar   = QStatusBar;

// Dialogs
using BTKMessageBox  = QMessageBox;
using BTKFileDialog  = QFileDialog;
using BTKColorDialog = QColorDialog;
using BTKFontDialog  = QFontDialog;
using BTKInputDialog = QInputDialog;
using BTKProgressDialog = QProgressDialog;
using BTKWizard      = QWizard;

#endif
