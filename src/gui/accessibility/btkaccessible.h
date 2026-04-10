/***********************************************************************
*
* BTK Accessibility Framework.
*
* BtkAccessible provides the foundation for screen reader support,
* keyboard navigation, and assistive technology integration across
* Windows (UIA/MSAA), macOS (NSAccessibility), and Linux (ATK/AT-SPI).
*
* Architecture:
*   BtkAccessible (role/name/state) <- BtkAccessibleInterface (bridge)
*                                       -> Platform-specific backend
*
* Every widget can expose an accessible representation through
* the BtkAccessibleInterface. The platform bridge translates
* these to native accessibility events.
*
***********************************************************************/

#ifndef BTK_ACCESSIBLE_H
#define BTK_ACCESSIBLE_H

#include <qglobal.h>
#include <qstring.h>
#include <qobject.h>
#include <qrect.h>
#include <qlist.h>
#include <qpair.h>
#include <qvariant.h>

class QWidget;

/**
 * Accessible role describing the semantic type of a UI element.
 * Aligned with UIA Control Types, ARIA roles, and ATK roles.
 */
enum class BtkAccessibleRole {
   NoRole,
   Client,           // Generic client area
   TitleBar,
   Pane,
   Dialog,
   Window,
   StaticText,       // Label
   EditableText,     // Text input
   Button,
   CheckBox,
   RadioButton,
   ToggleButton,
   Slider,
   ProgressBar,
   SpinBox,
   ComboBox,
   Tab,
   TabList,
   TabPanel,
   Menu,
   MenuItem,
   MenuBar,
   ToolBar,
   StatusBar,
   ScrollBar,
   ScrollArea,
   List,
   ListItem,
   Tree,
   TreeItem,
   Table,
   TableCell,
   TableRow,
   TableColumn,
   GroupBox,
   Separator,
   ToolTip,
   Link,
   Heading,
   Grid,
   Chart,
   Dial,
   Custom,           // Application-specific role
};

/**
 * Accessible state flags describing the current state of a UI element.
 */
enum BtkAccessibleStateFlag {
   BtkAccessibleState_Disabled     = 0x00000001,
   BtkAccessibleState_Selected     = 0x00000002,
   BtkAccessibleState_Focused      = 0x00000004,
   BtkAccessibleState_Pressed      = 0x00000008,
   BtkAccessibleState_Checkable    = 0x00000010,
   BtkAccessibleState_Checked      = 0x00000020,
   BtkAccessibleState_Editable     = 0x00000040,
   BtkAccessibleState_Expanded     = 0x00000080,
   BtkAccessibleState_Collapsed    = 0x00000100,
   BtkAccessibleState_Busy         = 0x00000200,
   BtkAccessibleState_Modal        = 0x00000400,
   BtkAccessibleState_MultiLine    = 0x00000800,
   BtkAccessibleState_MultiSelectable = 0x00001000,
   BtkAccessibleState_ReadOnly     = 0x00002000,
   BtkAccessibleState_Visible      = 0x00004000,
   BtkAccessibleState_Active       = 0x00008000,
   BtkAccessibleState_Searchable   = 0x00010000,
   BtkAccessibleState_Default      = 0x00020000,
   BtkAccessibleState_HasPopup     = 0x00040000,
   BtkAccessibleState_Required     = 0x00080000,
   BtkAccessibleState_Invalid      = 0x00100000,
   BtkAccessibleState_Sensitive    = 0x00200000,
};

Q_DECLARE_FLAGS(BtkAccessibleStates, BtkAccessibleStateFlag)

/**
 * Accessible event types for notifications to assistive technologies.
 */
enum class BtkAccessibleEvent {
   NameChanged,
   DescriptionChanged,
   ValueChanged,
   StateChanged,
   FocusGained,
   FocusLost,
   SelectionChanged,
   TextInserted,
   TextRemoved,
   TextReplaced,
   VisibleDataChanged,
   ChildrenChanged,
   ActiveDescendantChanged,
   PropertyChanged,
   WindowActivated,
   WindowDeactivated,
   MenuOpened,
   MenuClosed,
   PopupMenuStart,
   PopupMenuEnd,
   Announcement,      // Screen reader announcement
   Invalidated,       // Full tree invalidation
};

/**
 * Text boundary types for accessible text navigation.
 */
enum class BtkAccessibleTextBoundary {
   Character,
   Word,
   Sentence,
   Paragraph,
   Line,
   All,
};

/**
 * BtkAccessibleInterface defines the contract that accessible objects
 * must implement to expose their semantics to assistive technologies.
 *
 * Widgets should implement this interface to provide:
 * - Semantic role (what am I?)
 * - Name/description (what is my purpose?)
 * - State (what is my current condition?)
 * - Value (what data do I hold?)
 * - Relationships (parent, children, controls, flows-to)
 * - Actions (what can the user do with me?)
 */
class Q_GUI_EXPORT BtkAccessibleInterface
{
 public:
   virtual ~BtkAccessibleInterface();

   // Identity
   virtual bool isValid() const = 0;
   virtual QObject* object() const = 0;

   // Hierarchy
   virtual BtkAccessibleInterface* parent() const = 0;
   virtual QList<BtkAccessibleInterface*> children() const = 0;
   virtual int childCount() const = 0;
   virtual int indexOfChild(const BtkAccessibleInterface* child) const = 0;

   // Role and semantics
   virtual BtkAccessibleRole role() const = 0;
   virtual QString roleString() const;

   // Text properties
   virtual QString name() const = 0;
   virtual void setName(const QString& name);
   virtual QString description() const = 0;
   virtual void setDescription(const QString& desc);
   virtual QString value() const;
   virtual QString helpText() const;

   // State
   virtual BtkAccessibleStates state() const = 0;

   // Geometry
   virtual QRect rect() const = 0;
   virtual bool isVisible() const;

   // Relationships
   virtual QList<QPair<BtkAccessibleInterface*, QString>> relations() const;
   virtual BtkAccessibleInterface* labelFor() const;
   virtual BtkAccessibleInterface* labeledBy() const;
   virtual BtkAccessibleInterface* controllerFor() const;
   virtual BtkAccessibleInterface* controlledBy() const;
   virtual BtkAccessibleInterface* flowsTo() const;
   virtual BtkAccessibleInterface* flowsFrom() const;

   // Actions
   virtual QStringList actionNames() const;
   virtual void doAction(const QString& actionName);
   virtual QStringList keyBindingsForAction(const QString& actionName) const;

   // Text interface (for editable text widgets)
   virtual QString text(int startOffset, int endOffset) const;
   virtual int characterCount() const;
   virtual int cursorPosition() const;
   virtual void setCursorPosition(int position);
   virtual QString textBeforeOffset(int offset, BtkAccessibleTextBoundary boundary) const;
   virtual QString textAfterOffset(int offset, BtkAccessibleTextBoundary boundary) const;
   virtual QString textAtOffset(int offset, BtkAccessibleTextBoundary boundary) const;
   virtual void insertText(int offset, const QString& text);
   virtual void removeText(int startOffset, int endOffset);
   virtual void replaceText(int startOffset, int endOffset, const QString& text);

   // Value interface (for sliders, spin boxes, etc.)
   virtual QVariant currentValue() const;
   virtual void setCurrentValue(const QVariant& value);
   virtual QVariant maximumValue() const;
   virtual QVariant minimumValue() const;
   virtual QVariant minimumStepSize() const;

   // Table interface (for table/grid views)
   virtual int rowCount() const;
   virtual int columnCount() const;
   virtual BtkAccessibleInterface* cellAt(int row, int col) const;
   virtual QList<BtkAccessibleInterface*> selectedCells() const;
   virtual bool isRowSelected(int row) const;
   virtual bool isColumnSelected(int col) const;
   virtual void selectRow(int row);
   virtual void selectColumn(int col);
   virtual void unselectRow(int row);
   virtual void unselectColumn(int col);
};

/**
 * BtkAccessible is the accessible representation of a widget.
 * It wraps a BtkAccessibleInterface and provides a convenience API
 * for widget authors.
 */
class Q_GUI_EXPORT BtkAccessible : public QObject
{
   CS_OBJECT(BtkAccessible)

 public:
   explicit BtkAccessible(QObject* parent = nullptr);
   virtual ~BtkAccessible();

   static BtkAccessible* queryAccessible(QObject* object);
   static void setAccessibleInterface(QObject* object, BtkAccessibleInterface* iface);
   static void notifyEvent(QObject* object, BtkAccessibleEvent event);

   BtkAccessibleRole role() const;
   void setRole(BtkAccessibleRole role);

   QString name() const;
   void setName(const QString& name);

   QString description() const;
   void setDescription(const QString& desc);

   BtkAccessibleStates state() const;

   BtkAccessibleInterface* interface() const;

   CS_SIGNAL_1(Public, void nameChanged())
   CS_SIGNAL_2(nameChanged)

   CS_SIGNAL_1(Public, void descriptionChanged())
   CS_SIGNAL_2(descriptionChanged)

   CS_SIGNAL_1(Public, void stateChanged())
   CS_SIGNAL_2(stateChanged)

   CS_SIGNAL_1(Public, void valueChanged())
   CS_SIGNAL_2(valueChanged)

 private:
   BtkAccessibleInterface* m_interface;
   BtkAccessibleRole m_role;
   QString m_name;
   QString m_description;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(BtkAccessibleStates)

#endif
