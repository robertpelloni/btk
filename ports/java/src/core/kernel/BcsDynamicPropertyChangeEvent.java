package core.kernel;

public class BcsDynamicPropertyChangeEvent extends BcsEvent {
    private final byte[] propertyName;

    public BcsDynamicPropertyChangeEvent(byte[] propertyName) {
        super(EventType.DynamicPropertyChange);
        this.propertyName = propertyName;
    }

    public byte[] getPropertyName() {
        return propertyName;
    }
}
