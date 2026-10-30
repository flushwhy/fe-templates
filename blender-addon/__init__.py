"""A minimal Blender 4.2+ extension starter."""

import bpy


class VIEW3D_PT_my_blender_addon(bpy.types.Panel):
    """A simple panel in the 3D View sidebar."""

    bl_label = "My Blender Add-on"
    bl_idname = "VIEW3D_PT_my_blender_addon"
    bl_space_type = "VIEW_3D"
    bl_region_type = "UI"
    bl_category = "My Add-on"

    def draw(self, context):
        layout = self.layout
        layout.label(text="Hello from your add-on!")


_CLASSES = (
    VIEW3D_PT_my_blender_addon,
)


def register():
    for cls in _CLASSES:
        bpy.utils.register_class(cls)


def unregister():
    for cls in reversed(_CLASSES):
        bpy.utils.unregister_class(cls)
