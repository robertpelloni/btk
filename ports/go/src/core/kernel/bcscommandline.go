package kernel

import (
	"fmt"
	"os"
)

type BcsCommandLineOption struct {
	Names       []string
	Description string
	ValueName   string
	Default     string
}

type BcsCommandLineParser struct {
	options              []BcsCommandLineOption
	parsedOptions        map[string][]string
	positionalArgs       []string
	appDescription       string
	unknownOptions       []string
	hasVersionOption     bool
	hasHelpOption        bool
}

func NewBcsCommandLineParser() *BcsCommandLineParser {
	return &BcsCommandLineParser{
		options:        make([]BcsCommandLineOption, 0),
		parsedOptions:  make(map[string][]string),
		positionalArgs: make([]string, 0),
		unknownOptions: make([]string, 0),
	}
}

func (p *BcsCommandLineParser) AddOption(option BcsCommandLineOption) bool {
	p.options = append(p.options, option)
	return true
}

func (p *BcsCommandLineParser) SetApplicationDescription(desc string) {
	p.appDescription = desc
}

func (p *BcsCommandLineParser) AddHelpOption() {
	p.AddOption(BcsCommandLineOption{
		Names:       []string{"h", "help"},
		Description: "Displays this help.",
	})
	p.hasHelpOption = true
}

func (p *BcsCommandLineParser) AddVersionOption() {
	p.AddOption(BcsCommandLineOption{
		Names:       []string{"v", "version"},
		Description: "Displays version information.",
	})
	p.hasVersionOption = true
}

func (p *BcsCommandLineParser) Process(args []string) {
	p.Parse(args)
	if p.IsSet("help") {
		p.ShowHelp(0)
	}
	if p.IsSet("version") {
		p.ShowVersion()
	}
}

func (p *BcsCommandLineParser) Parse(args []string) bool {
	// Simple mock parser mapping for Go
	for i := 1; i < len(args); i++ {
		arg := args[i]
		if arg == "-h" || arg == "--help" {
			p.parsedOptions["help"] = []string{"true"}
		} else if arg == "-v" || arg == "--version" {
			p.parsedOptions["version"] = []string{"true"}
		} else {
			p.positionalArgs = append(p.positionalArgs, arg)
		}
	}
	return true
}

func (p *BcsCommandLineParser) IsSet(name string) bool {
	_, exists := p.parsedOptions[name]
	return exists
}

func (p *BcsCommandLineParser) Value(name string) string {
	if vals, ok := p.parsedOptions[name]; ok && len(vals) > 0 {
		return vals[0]
	}
	return ""
}

func (p *BcsCommandLineParser) ShowHelp(exitCode int) {
	fmt.Println(p.appDescription)
	fmt.Println("Options:")
	for _, opt := range p.options {
		fmt.Printf("  -%s, --%s : %s\n", opt.Names[0], opt.Names[len(opt.Names)-1], opt.Description)
	}
	os.Exit(exitCode)
}

func (p *BcsCommandLineParser) ShowVersion() {
	fmt.Println("Version 1.0.0") // Mock
	os.Exit(0)
}
