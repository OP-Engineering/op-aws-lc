mod error;
mod hmac;
mod rsa;
mod utils;

pub use error::*;
pub use hmac::*;
pub use rsa::*;
pub use utils::*;

#[unsafe(no_mangle)]
pub extern "C" fn init_lib() {
    #[cfg(target_os = "android")]
    {
        use android_logger::{Config, FilterBuilder};

        let filter = FilterBuilder::new()
            // .parse("trace,rustls=off")
            // .parse("trace,eventsource_client=off")
            // .parse("trace,mio=off")
            // .parse("trace,reqwest=off")
            .build();

        android_logger::init_once(
            Config::default()
                .with_max_level(log::LevelFilter::max())
                .with_tag("op-aws-lc")
                .with_filter(filter),
        );
    }

    log::info!("OP-AWS-LC Rust Bindings Initialized");
}
