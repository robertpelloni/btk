use std::sync::{Arc, Mutex};
use std::time::Duration;
use std::thread;
use std::sync::atomic::{AtomicI32, Ordering};
use super::bcs_object::BcsObject;
use super::bcs_kernel::BcsKernel;
use super::bcs_event::{BcsEventT, BcsTimerEvent};

static TIMER_ID_COUNTER: AtomicI32 = AtomicI32::new(1);

pub struct BcsTimer {
    pub base: Arc<BcsObject>,
    interval: Mutex<Duration>,
    single_shot: Mutex<bool>,
    is_active: Mutex<bool>,
    timer_id: i32,
    stop_signal: Mutex<Option<std::sync::mpsc::Sender<()>>>,
}

impl BcsTimer {
    pub fn new(parent: Option<Arc<BcsObject>>) -> Arc<Self> {
        let base = if let Some(p) = parent {
            BcsObject::new_with_parent(p)
        } else {
            BcsObject::new()
        };

        Arc::new(Self {
            base,
            interval: Mutex::new(Duration::from_millis(0)),
            single_shot: Mutex::new(false),
            is_active: Mutex::new(false),
            timer_id: TIMER_ID_COUNTER.fetch_add(1, Ordering::SeqCst),
            stop_signal: Mutex::new(None),
        })
    }

    pub fn set_interval(&self, msec: u64) {
        *self.interval.lock().unwrap() = Duration::from_millis(msec);
    }

    pub fn set_single_shot(&self, single_shot: bool) {
        *self.single_shot.lock().unwrap() = single_shot;
    }

    pub fn is_active(&self) -> bool {
        *self.is_active.lock().unwrap()
    }

    pub fn start(self: &Arc<Self>, msec: Option<u64>) {
        if let Some(ms) = msec {
            self.set_interval(ms);
        }

        self.stop();

        *self.is_active.lock().unwrap() = true;
        let (tx, rx) = std::sync::mpsc::channel();
        *self.stop_signal.lock().unwrap() = Some(tx);

        let interval = *self.interval.lock().unwrap();
        let single_shot = *self.single_shot.lock().unwrap();
        let timer_id = self.timer_id;
        let kernel = BcsKernel::instance();

        thread::spawn(move || {
            if single_shot {
                if rx.recv_timeout(interval).is_err() {
                    // Timeout hit
                    let event = Box::new(BcsTimerEvent::new(timer_id));
                    kernel.event_loop.dispatcher.post_event(event);
                }
            } else {
                loop {
                    if rx.recv_timeout(interval).is_ok() {
                        // Stopped
                        break;
                    }
                    // Timeout hit
                    let event = Box::new(BcsTimerEvent::new(timer_id));
                    kernel.event_loop.dispatcher.post_event(event);
                }
            }
        });
    }

    pub fn stop(&self) {
        let mut active = self.is_active.lock().unwrap();
        if !*active {
            return;
        }
        *active = false;

        if let Some(tx) = self.stop_signal.lock().unwrap().take() {
            let _ = tx.send(());
        }
    }
}

pub struct BcsCoreApplication {
    pub base: Arc<BcsObject>,
    kernel: Arc<BcsKernel>,
}

impl BcsCoreApplication {
    pub fn new() -> Arc<Self> {
        Arc::new(Self {
            base: BcsObject::new(),
            kernel: BcsKernel::instance(),
        })
    }

    pub fn exec(&self) -> i32 {
        self.kernel.exec()
    }

    pub fn quit(&self) {
        self.kernel.shutdown();
    }
}
