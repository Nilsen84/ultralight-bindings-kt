# Ultralight Kotlin Bindings

This project provides Kotlin bindings for the [Ultralight](https://ultralig.ht/) library, a lightweight, pure GPU HTML UI engine for C++. It allows you to embed Ultralight in your Kotlin/Java applications with ease, leveraging a JNI bridge.

## Features

- **Direct Kotlin API**: Idiomatic Kotlin wrappers around Ultralight's core C API.
- **High Performance**: Direct JNI calls for minimal overhead.
- **Off-screen Rendering**: Render HTML/CSS/JS directly to pixel buffers for use in games or other graphics applications.
- **Input Handling**: Forward mouse and keyboard events to the web view.
- **Custom Filesystem**: Load resources from Kotlin (e.g., from JARs or classpath).
- **Clipboard Support**: Integrate with the system clipboard.

## Project Structure

- `shared`: The core Kotlin library containing the API definitions and JNI loader.
- `native`: The C++ JNI implementation that bridges Kotlin and Ultralight.
- `platform`: A module for packaging platform-specific native libraries.

## Installation

Add the repository and dependencies to your `build.gradle.kts`:

```kotlin
repositories {
    maven("https://repo.repsy.io/nilsen84/maven")
}

dependencies {
    implementation("io.github.nilsen84:ultralight-bindings:1.0.0-SNAPSHOT")
    // Use the appropriate classifier for your platform: windows-x64, linux-x64, mac-x64, mac-arm64
    runtimeOnly("io.github.nilsen84:ultralight-bindings:1.0.0-SNAPSHOT:windows-x64")
}
```

## Usage

Here is a simple example of how to create a renderer and load a URL:

```kotlin
import io.github.nilsen84.ultralight.Ultralight
import io.github.nilsen84.ultralight.UltralightRenderer
import io.github.nilsen84.ultralight.UltralightView

fun main() {
    // 1. Create the renderer
    // You can optionally pass a custom filesystem, logger, or clipboard handler.
    val renderer = Ultralight.createRenderer()

    // 2. Create a View
    val view = renderer.createView(800, 600)

    // 3. Load a URL
    view.loadUrl("https://google.com")

    // 4. Main Loop
    while (true) {
        renderer.update()
        renderer.refreshDisplay()
        renderer.render()
        
        // Access the pixel buffer if needed
        val surface = view.surface()
        if (surface.dirtyBounds().isValid) {
            surface.lockPixels().use { buffer ->
                val byteBuffer = buffer.asByteBuffer()
                // ... copy pixels to your application's texture or window ...
            }
             surface.clearDirtyBounds()
        }

        Thread.sleep(16) // ~60 FPS
    }
}
```

## Contributing

Contributions are welcome! Please open an issue or submit a pull request.
