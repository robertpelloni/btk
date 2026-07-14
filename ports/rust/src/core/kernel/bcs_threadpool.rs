use std::sync::{Arc, Mutex, Condvar};
use std::thread;
use std::collections::VecDeque;
use crate::core::kernel::bcs_object::BcsObject;

type Job = Box<dyn FnOnce() + Send + 'static>;

pub struct BcsThreadPool {
    pub base: Arc<BcsObject>,
    jobs: Arc<(Mutex<VecDeque<Job>>, Condvar)>,
    workers: Mutex<Vec<Option<thread::JoinHandle<()>>>>,
    shutdown: Arc<Mutex<bool>>,
}

impl BcsThreadPool {
    pub fn new(parent: Option<Arc<BcsObject>>, num_workers: usize) -> Arc<Self> {
        let base = if let Some(p) = parent {
            BcsObject::new_with_parent(p)
        } else {
            BcsObject::new()
        };

        let jobs = Arc::new((Mutex::new(VecDeque::new()), Condvar::new()));
        let shutdown = Arc::new(Mutex::new(false));
        let mut workers = Vec::with_capacity(num_workers);

        for _ in 0..num_workers {
            let jobs_clone = Arc::clone(&jobs);
            let shutdown_clone = Arc::clone(&shutdown);

            let handle = thread::spawn(move || loop {
                let (lock, cvar) = &*jobs_clone;
                let mut queue = lock.lock().unwrap();

                while queue.is_empty() && !*shutdown_clone.lock().unwrap() {
                    queue = cvar.wait(queue).unwrap();
                }

                if *shutdown_clone.lock().unwrap() && queue.is_empty() {
                    break;
                }

                if let Some(job) = queue.pop_front() {
                    drop(queue);
                    job();
                }
            });

            workers.push(Some(handle));
        }

        Arc::new(Self {
            base,
            jobs,
            workers: Mutex::new(workers),
            shutdown,
        })
    }

    pub fn submit<F>(&self, f: F)
    where
        F: FnOnce() + Send + 'static,
    {
        if *self.shutdown.lock().unwrap() {
            return;
        }

        let (lock, cvar) = &*self.jobs;
        let mut queue = lock.lock().unwrap();
        queue.push_back(Box::new(f));
        cvar.notify_one();
    }

    pub fn shutdown(&self) {
        {
            let mut shutdown = self.shutdown.lock().unwrap();
            *shutdown = true;
        }

        let (_, cvar) = &*self.jobs;
        cvar.notify_all();

        // Join workers
        let mut workers = self.workers.lock().unwrap();
        for worker in workers.iter_mut() {
            if let Some(thread) = worker.take() {
                let _ = thread.join();
            }
        }
    }
}
