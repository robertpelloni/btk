use std::sync::{Arc, Mutex, OnceLock};
use super::bcs_object::BcsObject;
use super::bcs_event_loop::BcsEventLoop;
use super::bcs_event_dispatcher::BcsEventDispatcher;

pub trait BcsModule: Send + Sync {
    fn initialize(&self) -> Result<(), String>;
    fn start(&self) -> Result<(), String>;
    fn shutdown(&self) -> Result<(), String>;
    fn name(&self) -> String;
}

pub struct BcsKernel {
    pub base: Arc<BcsObject>,
    pub event_loop: Arc<BcsEventLoop>,
    modules: Mutex<Vec<Arc<dyn BcsModule>>>,
    is_started: Mutex<bool>,
    is_shutting_down: Mutex<bool>,
}

static INSTANCE: OnceLock<Arc<BcsKernel>> = OnceLock::new();

impl BcsKernel {
    pub fn instance() -> Arc<BcsKernel> {
        INSTANCE.get_or_init(|| {
            let base = BcsObject::new();
            let dispatcher = Arc::new(BcsEventDispatcher::new());
            let event_loop = Arc::new(BcsEventLoop::new(dispatcher, Some(base.clone())));

            Arc::new(Self {
                base,
                event_loop,
                modules: Mutex::new(Vec::new()),
                is_started: Mutex::new(false),
                is_shutting_down: Mutex::new(false),
            })
        }).clone()
    }

    pub fn register_module(&self, module: Arc<dyn BcsModule>) {
        if !*self.is_started.lock().unwrap() {
            self.modules.lock().unwrap().push(module);
        }
    }

    pub fn startup(&self) -> Result<(), String> {
        let mut started = self.is_started.lock().unwrap();
        if *started {
            return Ok(());
        }
        *started = true;

        let modules = self.modules.lock().unwrap().clone();

        // Phase 1: Initialize
        for module in &modules {
            println!("BcsKernel: Initializing module {}", module.name());
            module.initialize()?;
        }

        // Phase 2: Start
        for module in &modules {
            println!("BcsKernel: Starting module {}", module.name());
            module.start()?;
        }

        Ok(())
    }

    pub fn exec(&self) -> i32 {
        if let Err(e) = self.startup() {
            println!("BcsKernel Startup Error: {}", e);
            return -1;
        }

        println!("BcsKernel: Entering main event loop...");
        let code = self.event_loop.exec();

        self.shutdown();
        code
    }

    pub fn shutdown(&self) {
        let mut shutting_down = self.is_shutting_down.lock().unwrap();
        if *shutting_down {
            return;
        }
        *shutting_down = true;

        println!("BcsKernel: Shutting down...");

        let modules = self.modules.lock().unwrap().clone();

        // Reverse order shutdown
        for module in modules.iter().rev() {
            println!("BcsKernel: Shutting down module {}", module.name());
            let _ = module.shutdown();
        }

        self.base.destroy();
    }
}
