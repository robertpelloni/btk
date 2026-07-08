package core.kernel;

public class BcsCustomEvent extends BcsEvent {
    private final Object data;

    public BcsCustomEvent(EventType type, Object data) {
        super(type);
        this.data = data;
    }

    public Object getData() {
        return data;
    }
}
