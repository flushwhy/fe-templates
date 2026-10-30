# Blender Add-on

A minimal Blender 4.2+ extension starter using the current extension manifest format.

## Customize

1. Edit `blender_manifest.toml`: change the extension ID, name, version, maintainer, tags, and minimum Blender version.
2. Update the panel and its registration in `__init__.py`.
3. Keep the manifest ID stable; Blender uses it to identify the extension.

## Validate and package

From this directory, use Blender's extension commands:

```sh
blender --command extension validate .
blender --command extension build
```

The build command creates a distributable ZIP in the current directory. You can also create a ZIP with ordinary archive tooling; Blender is only needed for Blender-specific validation and extension workflows.

Install the resulting ZIP through Blender's Get Extensions / Install from Disk interface.

This starter does not require third-party Python packages.
