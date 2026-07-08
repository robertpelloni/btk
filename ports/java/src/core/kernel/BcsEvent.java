package core.kernel;

import java.util.concurrent.atomic.AtomicBoolean;

public abstract class BcsEvent {

    public enum EventType {
        None,
        Timer,
        MouseButtonPress,
        MouseButtonRelease,
        MouseButtonDblClick,
        MouseMove,
        KeyPress,
        KeyRelease,
        FocusIn,
        FocusOut,
        Enter,
        Leave,
        Paint,
        Move,
        Resize,
        Create,
        Destroy,
        Show,
        Hide,
        Close,
        Quit,
        ApplicationActivate,
        ApplicationDeactivate,
        DynamicPropertyChange,
        User
    }

    private final EventType type;
    private boolean spontaneous = false;
    private final AtomicBoolean accepted = new AtomicBoolean(true);

    protected BcsEvent(EventType type) {
        this.type = type;
    }

    public EventType getType() {
        return type;
    }

    public boolean isSpontaneous() {
        return spontaneous;
    }

    public void setSpontaneous(boolean spontaneous) {
        this.spontaneous = spontaneous;
    }

    public boolean isAccepted() {
        return accepted.get();
    }

    public void setAccepted(boolean accepted) {
        this.accepted.set(accepted);
    }

    public void accept() {
        setAccepted(true);
    }

    public void ignore() {
        setAccepted(false);
    }
}
