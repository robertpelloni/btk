package core.kernel;

public class BcsTimerEvent extends BcsEvent {
    private final int timerId;

    public BcsTimerEvent(int timerId) {
        super(EventType.Timer);
        this.timerId = timerId;
    }

    public int getTimerId() {
        return timerId;
    }
}
