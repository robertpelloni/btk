package core.kernel;

import java.util.ArrayList;
import java.util.List;

public class BcsCommandLineOption {
    private List<String> names = new ArrayList<>();
    private String description = "";
    private String valueName = "";
    private String defaultValue = "";

    public BcsCommandLineOption() {}

    public List<String> getNames() { return names; }
    public void setNames(List<String> names) { this.names = names; }
    public String getDescription() { return description; }
    public void setDescription(String desc) { this.description = desc; }
}
