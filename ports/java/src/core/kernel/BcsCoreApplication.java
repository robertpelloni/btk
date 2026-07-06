package core.kernel;

public class BcsCoreApplication extends BcsObject {
    private static volatile BcsCoreApplication instance;
    private final BcsKernel kernel;

    private BcsCoreApplication() {
        super(null);
        this.kernel = BcsKernel.getInstance();
    }

    public static BcsCoreApplication getInstance() {
        if (instance == null) {
            synchronized (BcsCoreApplication.class) {
                if (instance == null) {
                    instance = new BcsCoreApplication();
                }
            }
        }
        return instance;
    }

    public int exec() {
        return kernel.exec();
    }

    public void quit() {
        kernel.shutdown();
    }
}
