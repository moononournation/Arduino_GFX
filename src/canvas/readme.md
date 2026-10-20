# About the canvas mono class in the GFX library

Some display controllers on the market can render a small amount of graphical data, but they do not provide much functionality beyond data transfer. Examples include SSD1306 and SH1106 OLED display controller chips.

These displays require an in-memory copy of the graphics data, and updates are performed by transferring the internal memory to the display in a bulk operation.

For this reason, the Canvas Mono class must be initialized so that its internal memory layout matches the memory inside the graphics controller chip, including the display width and height, the memory orientation, and whether each byte is arranged horizontally or vertically.

When the canvas orientation changes, graphics primitives such as lines and characters are positioned within the display memory accordingly. When rotating by 90° or 270°, the effective width and height change, and the internal buffer and drawing logic must be adjusted to match the rotated coordinate system.

RGB565, RGB888, grayscale, indexed, and packed-color bitmap draw calls use Floyd-Steinberg error diffusion directly into the monochrome framebuffer. The canvas uses two per-column error rows, without an intermediate image buffer.
