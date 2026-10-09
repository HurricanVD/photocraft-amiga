//! Native reference oracle using the ORIGINAL PhotoCraft Rust crates.
//! This harness lives outside the upstream Cargo workspace, and only
//! observes public Rust crate APIs. Its output is compared with C99.

use photocraft_color::PixelFormat;
use photocraft_geom::{Rect, TileCoord};
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
fn main() {
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
