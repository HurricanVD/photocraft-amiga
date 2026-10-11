//! Native reference oracle using the ORIGINAL PhotoCraft Rust crates.
//! This harness lives outside the upstream Cargo workspace, and only
//! observes public Rust crate APIs. Its output is compared with C99.

use photocraft_color::{ColorMode, PixelFormat, SampleType};
use photocraft_doc::{Document, Layer, LayerId};
use photocraft_geom::{Rect, Size, TileCoord};
use photocraft_raster::Surface;

fn rect_line(label: &str, r: Rect) {
    println!("{label} {} {} {} {}", r.x0, r.y0, r.x1, r.y1);
}
fn coord_line(x: i32, y: i32) {
    let c = TileCoord::containing(x, y);
    println!("tile {x} {y} {} {}", c.tx, c.ty);
}
fn pixel_line(label: &str, s: &Surface, x: i32, y: i32) {
    let v = s.pixel(x, y);
    let q = |value: f32| (value.clamp(0.0, 1.0) * 255.0 + 0.5) as u8;
    println!(
        "{label} {} {} {} {}",
        q(v[0]), q(v[1]), q(v[2]), q(v[3])
    );
}

fn typed_case(label: &str, fmt: PixelFormat) {
    let rect = Rect::new(-1, 255, 1, 257);
    let bpp = fmt.bytes_per_pixel();
    let input: Vec<u8> = (0..4 * bpp).map(|i| ((i * 31 + 7) % 251) as u8).collect();
    let mut original = Surface::new(fmt);
    original.write_interleaved(rect, &input);
    print_hex(&format!("typed-{label}-initial"), &original.to_interleaved(rect));
    println!("typed-{label}-tiles {}", original.tile_count());
    let mut clone = original.clone();
    let replacement = vec![0xa5u8; bpp];
    clone.write_interleaved(Rect::new(0, 256, 1, 257), &replacement);
    print_hex(&format!("typed-{label}-snapshot"), &clone.to_interleaved(rect));
    print_hex(&format!("typed-{label}-original"), &original.to_interleaved(rect));
}

fn print_hex(label: &str, bytes: &[u8]) {
    print!("{label} ");
    for b in bytes { print!("{b:02x}"); }
    println!();
}
fn typed_main() {
    typed_case("rgba8", PixelFormat::RGBA8);
    typed_case("rgba16", PixelFormat::RGBA16);
    typed_case("rgba32f", PixelFormat::RGBA32F);
    typed_case("graya8", PixelFormat::GRAYA8);
    typed_case("cmyka8", PixelFormat::CMYKA8);
    let def = Surface::with_default(PixelFormat::GRAYA8, &[128.0 / 255.0, 1.0]);
    print_hex("typed-graya8-default",
        &def.to_interleaved(Rect::new(-500, 400, -499, 401)));
}

fn document_order(label: &str, d: &Document) {
    println!(
        "{label} {} {} {}",
        d.layers[0].id.0, d.layers[1].id.0, d.layers[2].id.0
    );
}

// Reference contracts: original doc::Layer defaults and Document::shift.
// This is limited to flat raster layer metadata, not groups or compositing.
fn document_main() {
    let mut d = Document::new(
        "Three layers",
        Size::new(20, 30),
        ColorMode::Rgb,
        SampleType::U8,
    );
    for (id, name) in [(11, "Bottom"), (22, "Middle"), (33, "Top")] {
        let mut layer = Layer::raster(name, PixelFormat::RGBA8);
        layer.id = LayerId(id);
        d.layers.push(layer);
    }

    document_order("initial", &d);
    println!("defaults {:.2} {:.2}", d.layers[0].opacity, d.layers[0].fill_opacity);
    println!("raise {}", d.shift(LayerId(11), 2) as u8);
    document_order("raised", &d);
    println!("lower {}", d.shift(LayerId(11), -1) as u8);
    document_order("lowered", &d);
    println!("boundary {}", d.shift(LayerId(11), 2) as u8);
    println!("absent {}", d.shift(LayerId(444), 0) as u8);
    println!("zero {}", d.shift(LayerId(11), 0) as u8);
    document_order("stayed", &d);

    d.layers[0].opacity = 0.0;
    d.layers[0].fill_opacity = 0.75;
    d.layers[1].opacity = 0.5;
    d.layers[1].fill_opacity = 0.25;
    println!(
        "meta {:.2} {:.2} {:.2} {:.2}",
        d.layers[0].opacity, d.layers[0].fill_opacity,
        d.layers[1].opacity, d.layers[1].fill_opacity,
    );

    let mut snapshot = d.clone();
    println!("clone-shift {}", snapshot.shift(LayerId(33), -2) as u8);
    snapshot.layers[1].opacity = 1.0;
    snapshot.layers[1].fill_opacity = 1.0;
    document_order("original", &d);
    document_order("snapshot", &snapshot);
    println!(
        "unchanged {:.2} {:.2}",
        d.layers[1].opacity, d.layers[1].fill_opacity,
    );
}

fn main() {
    if std::env::args().nth(1).as_deref() == Some("document") {
        document_main();
        return;
    }
    if std::env::args().nth(1).as_deref() == Some("typed") {
        typed_main();
        return;
    }
    let a = Rect::new(0, 0, 10, 10);
    let b = Rect::new(5, 5, 15, 15);
    rect_line("rect-xywh", Rect::from_xywh(10, 20, 30, 40));
    rect_line("rect-intersect", a.intersect(&b));
    rect_line("rect-union", a.union(&b));
    rect_line("rect-disjoint", a.intersect(&Rect::new(20, 20, 30, 30)));
    rect_line(
        "rect-translate",
        Rect::new(i32::MAX - 10, i32::MIN + 10, i32::MAX, i32::MIN + 20)
            .translate(100, -100),
    );
    println!(
        "rect-extreme-width {}",
        Rect::new(i32::MIN, 0, i32::MAX, 1).width()
    );
    for (x, y) in [
        (-257, -256),
        (-256, -257),
        (-1, -1),
        (0, 0),
        (255, 255),
        (256, 256),
    ] {
        coord_line(x, y);
    }
    rect_line("tile-rect", TileCoord::new(1, -2).rect());
    println!("format-rgba8 {}", PixelFormat::RGBA8.bytes_per_pixel());
    println!("format-rgba16 {}", PixelFormat::RGBA16.bytes_per_pixel());
    println!("format-rgba32f {}", PixelFormat::RGBA32F.bytes_per_pixel());
    println!("format-cmyka-channels {}", PixelFormat::CMYKA8.channels());

    let mut s = Surface::new(PixelFormat::RGBA8);
    println!("surface-empty {}", s.tile_count());
    pixel_line("surface-default", &s, 300, -200);
    let red = [1.0, 0.0, 0.0, 1.0];
    let green = [0.0, 1.0, 0.0, 1.0];
    let blue = [0.0, 0.0, 1.0, 1.0];
    let zero = [0.0, 0.0, 0.0, 0.0];
    s.write_pixel(0, 0, &red);
    s.write_pixel(256, 0, &green);
    s.write_pixel(-1, -1, &blue);
    s.write_pixel(-256, -257, &red);
    println!("surface-tiles {}", s.tile_count());
    let mut snapshot = s.clone();
    snapshot.write_pixel(0, 0, &green);
    snapshot.write_pixel(-1, -1, &zero);
    pixel_line("original-red", &s, 0, 0);
    pixel_line("snapshot-green", &snapshot, 0, 0);
    pixel_line("original-blue", &s, -1, -1);
    pixel_line("snapshot-cleared", &snapshot, -1, -1);
    pixel_line("shared-neighbor", &snapshot, 256, 0);
    snapshot.prune();
    println!("snapshot-pruned-tiles {}", snapshot.tile_count());
    println!("original-still-tiles {}", s.tile_count());
    drop(s);
    pixel_line("snapshot-after-drop", &snapshot, 256, 0);

    let gray = [128.0 / 255.0, 128.0 / 255.0, 128.0 / 255.0, 1.0];
    let mut mask = Surface::with_default(PixelFormat::RGBA8, &gray);
    pixel_line("nonzero-default", &mask, 999, -999);
    mask.write_pixel(999, -999, &gray);
    mask.prune();
    println!("default-tile-pruned {}", mask.tile_count());
}
