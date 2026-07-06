use std::collections::HashMap;

pub struct BcsCommandLineOption {
    pub names: Vec<String>,
    pub description: String,
    pub value_name: String,
    pub default_value: String,
}

pub struct BcsCommandLineParser {
    options: Vec<BcsCommandLineOption>,
    parsed_options: HashMap<String, Vec<String>>,
    positional_args: Vec<String>,
    app_description: String,
}

impl BcsCommandLineParser {
    pub fn new() -> Self {
        Self {
            options: Vec::new(),
            parsed_options: HashMap::new(),
            positional_args: Vec::new(),
            app_description: String::new(),
        }
    }

    pub fn add_option(&mut self, option: BcsCommandLineOption) -> bool {
        self.options.push(option);
        true
    }

    pub fn set_application_description(&mut self, desc: &str) {
        self.app_description = desc.to_string();
    }

    pub fn add_help_option(&mut self) {
        self.add_option(BcsCommandLineOption {
            names: vec!["h".to_string(), "help".to_string()],
            description: "Displays this help.".to_string(),
            value_name: String::new(),
            default_value: String::new(),
        });
    }

    pub fn add_version_option(&mut self) {
        self.add_option(BcsCommandLineOption {
            names: vec!["v".to_string(), "version".to_string()],
            description: "Displays version information.".to_string(),
            value_name: String::new(),
            default_value: String::new(),
        });
    }

    pub fn parse(&mut self, args: Vec<String>) -> bool {
        for arg in args.into_iter().skip(1) {
            if arg == "-h" || arg == "--help" {
                self.parsed_options.insert("help".to_string(), vec!["true".to_string()]);
            } else if arg == "-v" || arg == "--version" {
                self.parsed_options.insert("version".to_string(), vec!["true".to_string()]);
            } else {
                self.positional_args.push(arg);
            }
        }
        true
    }

    pub fn process(&mut self, args: Vec<String>) {
        self.parse(args);
        if self.is_set("help") {
            self.show_help(0);
        }
        if self.is_set("version") {
            self.show_version();
        }
    }

    pub fn is_set(&self, name: &str) -> bool {
        self.parsed_options.contains_key(name)
    }

    pub fn show_help(&self, exit_code: i32) {
        println!("{}", self.app_description);
        println!("Options:");
        for opt in &self.options {
            println!("  -{}, --{} : {}", opt.names[0], opt.names.last().unwrap(), opt.description);
        }
        std::process::exit(exit_code);
    }

    pub fn show_version(&self) {
        println!("Version 1.0.0");
        std::process::exit(0);
    }
}
