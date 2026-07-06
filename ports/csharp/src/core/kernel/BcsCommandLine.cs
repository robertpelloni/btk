using System;
using System.Collections.Generic;

namespace Bcs.Core.Kernel
{
    public class BcsCommandLineOption
    {
        public List<string> Names { get; set; } = new List<string>();
        public string Description { get; set; } = string.Empty;
        public string ValueName { get; set; } = string.Empty;
        public string DefaultValue { get; set; } = string.Empty;
    }

    public class BcsCommandLineParser
    {
        private List<BcsCommandLineOption> _options = new List<BcsCommandLineOption>();
        private Dictionary<string, List<string>> _parsedOptions = new Dictionary<string, List<string>>();
        private List<string> _positionalArgs = new List<string>();
        private string _appDescription = string.Empty;

        public void AddOption(BcsCommandLineOption option)
        {
            _options.Add(option);
        }

        public void SetApplicationDescription(string desc)
        {
            _appDescription = desc;
        }

        public void AddHelpOption()
        {
            AddOption(new BcsCommandLineOption {
                Names = new List<string> { "h", "help" },
                Description = "Displays this help."
            });
        }

        public void AddVersionOption()
        {
            AddOption(new BcsCommandLineOption {
                Names = new List<string> { "v", "version" },
                Description = "Displays version information."
            });
        }

        public bool Parse(string[] args)
        {
            for (int i = 1; i < args.Length; i++)
            {
                var arg = args[i];
                if (arg == "-h" || arg == "--help")
                {
                    _parsedOptions["help"] = new List<string> { "true" };
                }
                else if (arg == "-v" || arg == "--version")
                {
                    _parsedOptions["version"] = new List<string> { "true" };
                }
                else
                {
                    _positionalArgs.Add(arg);
                }
            }
            return true;
        }

        public void Process(string[] args)
        {
            Parse(args);
            if (IsSet("help")) ShowHelp(0);
            if (IsSet("version")) ShowVersion();
        }

        public bool IsSet(string name)
        {
            return _parsedOptions.ContainsKey(name);
        }

        public void ShowHelp(int exitCode)
        {
            Console.WriteLine(_appDescription);
            Console.WriteLine("Options:");
            foreach (var opt in _options)
            {
                Console.WriteLine($"  -{opt.Names[0]}, --{opt.Names[opt.Names.Count - 1]} : {opt.Description}");
            }
            Environment.Exit(exitCode);
        }

        public void ShowVersion()
        {
            Console.WriteLine("Version 1.0.0");
            Environment.Exit(0);
        }
    }
}
