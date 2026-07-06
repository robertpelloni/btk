package core.kernel;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Arrays;

public class BcsCommandLineParser {
    private final List<BcsCommandLineOption> options = new ArrayList<>();
    private final Map<String, List<String>> parsedOptions = new HashMap<>();
    private final List<String> positionalArgs = new ArrayList<>();
    private String appDescription = "";

    public void addOption(BcsCommandLineOption option) {
        options.add(option);
    }

    public void setApplicationDescription(String desc) {
        this.appDescription = desc;
    }

    public void addHelpOption() {
        BcsCommandLineOption opt = new BcsCommandLineOption();
        opt.setNames(Arrays.asList("h", "help"));
        opt.setDescription("Displays this help.");
        addOption(opt);
    }

    public void addVersionOption() {
        BcsCommandLineOption opt = new BcsCommandLineOption();
        opt.setNames(Arrays.asList("v", "version"));
        opt.setDescription("Displays version information.");
        addOption(opt);
    }

    public boolean parse(String[] args) {
        for (int i = 0; i < args.length; i++) {
            String arg = args[i];
            if (arg.equals("-h") || arg.equals("--help")) {
                parsedOptions.put("help", Arrays.asList("true"));
            } else if (arg.equals("-v") || arg.equals("--version")) {
                parsedOptions.put("version", Arrays.asList("true"));
            } else {
                positionalArgs.add(arg);
            }
        }
        return true;
    }

    public void process(String[] args) {
        parse(args);
        if (isSet("help")) showHelp(0);
        if (isSet("version")) showVersion();
    }

    public boolean isSet(String name) {
        return parsedOptions.containsKey(name);
    }

    public void showHelp(int exitCode) {
        System.out.println(appDescription);
        System.out.println("Options:");
        for (BcsCommandLineOption opt : options) {
            List<String> n = opt.getNames();
            System.out.printf("  -%s, --%s : %s\n", n.get(0), n.get(n.size() - 1), opt.getDescription());
        }
        System.exit(exitCode);
    }

    public void showVersion() {
        System.out.println("Version 1.0.0");
        System.exit(0);
    }
}
