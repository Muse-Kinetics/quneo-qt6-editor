# QuNeo Editor

A cross-platform Qt6 application for configuring the QuNeo MIDI controller hardware. This editor allows users to customize MIDI mappings, adjust control sensitivity, update firmware, and manage presets for the QuNeo device.

## Features

- **MIDI Configuration**: Customize MIDI CC mappings, channels, and velocity curves for all QuNeo controls
- **Preset Management**: Save, load, and organize custom presets
- **Firmware Updates**: Update QuNeo firmware directly through the editor
- **Hardware Testing**: Test pad sensitivity, sliders, and rotary controls
- **Cross-Platform**: Supports Windows, macOS (Intel/Apple Silicon), and possibly Linux
- **Template Library**: Includes templates for popular DAWs (Ableton Live, Logic Pro, Reason, etc.)

## System Requirements

### Minimum Requirements
- **Qt 6.2.1 or later** (Qt 6.9.2 recommended)
- **Windows**: Windows 10 or later
- **macOS**: macOS 10.15 (Catalina) or later
- **Linux**: ALSA-compatible system

### Build Requirements
- Qt6 development environment
- C++ compiler with C++17 support
- Platform-specific MIDI libraries (automatically linked)

## Building from Source

### Prerequisites
1. Install Qt 6.9.2 or compatible version
2. Clone this repository
3. Ensure you have the required development tools for your platform

### Building
```bash
# Navigate to the QT6-QuNeo directory
cd QT6-QuNeo

# Generate Makefile (or use Qt Creator)
qmake QuNeo.pro

# Build the application
make

# For deployment builds (Windows installer creation)
make deploy
```

**Note**: The `make deploy` step is required for Windows deployment and installer creation when `DEPLOY=1` is set in `QuNeo.pro`.

### Qt Creator
1. Open `QT6-QuNeo/QuNeo.pro` in Qt Creator
2. Configure your kit for the target platform
3. Build the project

## Deployment

### Windows Deployment

The project includes an automated Windows deployment system that creates a signed installer (you must have your own signing key)

#### Prerequisites
- Windows SDK (for signtool.exe)
- Qt Installer Framework (QtIFW)
- Code signing certificate configured in Windows certificate store

#### Steps
1. **Enable deployment** in `QuNeo.pro`:
   ```qmake
   DEPLOY = 1
   ```

2. **Create a deploy build step** in Qt Creator:
   - Go to Projects → Build Steps
   - Add "Make" step with target: `deploy`

3. **Build with deployment**:
   ```bash
   qmake QuNeo.pro
   make deploy
   ```

The deployment process will:
- Copy the executable to the installer package directory
- Run `windeployqt` to include Qt dependencies
- Sign the executable with your certificate
- Copy application content and documentation
- Create and sign the final installer using Qt Installer Framework

#### Output
- Signed installer: `QuNeo Editor v[VERSION] Windows Installer.exe`
- Located in the `win-deploy` directory

### macOS Deployment

macOS deployment uses the signing and notarization system for distribution.

#### Prerequisites
- Apple Developer account and certificates
- Environment variables configured:
  ```bash
  export DEVELOPER_ID="Developer ID Application: Your Name (TEAMID)"
  export APPLE_KEYCHAIN_PROFILE="YourKeychainProfile"
  export APPLE_TEAM_ID="TEAMID"
  export APPLE_ID="your-apple-id@email.com"
  export APPLE_APP_PASSWORD="app-specific-password"
  ```

#### Steps
1. Build the application in Qt Creator or command line
2. Navigate to the signing directory:
   ```bash
   cd "Signing and Notarization"
   ```
3. Run the deployment script:
   ```bash
   ./build\ sign\ package\ mac\ qt6_universal.sh
   ```

The script will:
- Create a universal binary (Intel + Apple Silicon)
- Sign the application with your Developer ID
- Create a DMG installer
- Notarize the DMG with Apple
- Provide a ready-to-distribute signed package

## Project Structure

```
QuNeo/
├── QT6-QuNeo/                    # Main Qt project
│   ├── QuNeo.pro                 # Qt project file
│   ├── main.cpp                  # Application entry point
│   ├── mainwindow.cpp/h/ui       # Main application window
│   ├── ui/                       # UI components
│   ├── data/                     # Data handling classes
│   ├── midi/                     # MIDI communication
│   └── resources/                # Fonts, images, stylesheets
├── Content/                      # Application content
│   ├── Documentation/            # User manuals and guides
│   ├── Presets/                  # Factory presets
│   ├── Software Templates/       # DAW integration templates
│   └── SysEx/                    # Firmware files
├── Signing and Notarization/     # macOS deployment scripts
└── win-deploy/                   # Windows installer configuration
```

## Contributors

- **Connor Lacey** - Original author
- **Eric Bateman** - Lead developer (2021-present)
- **Emmet Corman** - Contributing developer

## Contributing

Pull requests are welcome! However, all updates should be thoroughly tested with actual QuNeo hardware on both macOS and Windows platforms before submission.

### Testing Requirements
- Test all MIDI functionality with connected QuNeo hardware
- Verify preset loading/saving works correctly
- Test firmware update process
- Ensure UI responsiveness across different screen sizes
- Validate on both macOS and Windows

### Hardware Requirements for Contributors
Contributors need access to QuNeo hardware for testing. If you're interested in contributing but don't have hardware access, you can contact us about hardware availability:

**Email**: contact@musekinetics.com

Please note that contributors may need to cover shipping costs and a handling fee for hardware loan programs. Development hardware is sourced from customer returns and old b-stock units, some of which may have tacky soft-touch coating. Unfortunately there is no way to safely replace/clean the QuNeo enclosures as they are sonically welded together.

## Support

QuNeo is not supported by Muse Kinetics and this source code is provided as-is with no warranty. For support, feature requests, or bug reports, please create an issue in this repository and encourage other open source developers to assist you.

## License

MIT License

Copyright (c) 2025 Muse Kinetics

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

## Third-Party Licenses

### Qt Framework

This application is built using the Qt framework, which is available under the GNU Lesser General Public License (LGPL) v3.

- **Qt Website**: https://www.qt.io/
- **Qt Open Source License**: LGPL v3
- **Qt Source Code**: https://www.qt.io/download-open-source
- **Qt License Details**: https://www.qt.io/licensing/open-source-lgpl-obligations

The QuNeo Editor application dynamically links with Qt libraries and does not modify the Qt framework source code. Users have the right to obtain, modify, and redistribute the Qt libraries under the terms of the LGPL v3 license.

### Qt LGPL v3 Summary

The Qt framework is licensed under LGPL v3, which allows:
- Use in both open source and commercial applications
- Dynamic linking without affecting your application's license
- Distribution of Qt libraries alongside your application

For complete LGPL v3 license terms, see: https://www.gnu.org/licenses/lgpl-3.0.html

## Version History

See [CHANGELOG.md](CHANGELOG.md) for detailed version history and release notes.


