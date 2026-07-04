package core.kernel;

public class BcsChildEvent extends BcsEvent {
    private final Object child; // Maps to BcsObject

    public BcsChildEvent(EventType type, Object child) {
        super(type);
        this.child = child;
    }

    public Object getChild() {
        return child;
    }
}
